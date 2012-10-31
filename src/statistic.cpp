/*
 * (C) Copyright 2012 Kuban State University (http://kubsu.ru/),
 *     Faculty of Physics and Technology, Physics and Information Systems Chair
 *
 * All rights reserved. This file is part of StatistiQ, a program
 * for getting various statistical information from measurement data.
 *
 * Developed by Ivan Novikov (http://github.com/NIA)
 * under the direction of Leontiy Grigorjan
 *
 */

#include "statistic.h"

#include <cmath>

namespace {
    inline double squared(double value) {
        return value * value;
    }
    inline double cubed(double value) {
        return value * value * value;
    }

    inline QString valueToItem(double value) {
        return QString::number(value, 'g', 10);
    }
    inline double itemToValue(QString item) {
        return item.toDouble();
    }

    QList<QStandardItem*> dataToItems(QList<double> data) {
        QList<QStandardItem*> result;
        foreach(double d, data) {
            result << new QStandardItem(valueToItem(d));
        }
        return result;
    }

    // minimum histogram area (to avoid 0-length intervals)
    const double MIN_HIST_AREA = 0.01;
}

Statistic::Statistic(QObject *parent, QList<double> data, QString header, bool modified) :
    QObject(parent), data(data), model(0, 0, this), header_(header), modified(modified),
    histogramIntervals(10), histogramFraction(false), dataPoints_(data.size())
{
    initModel();
    recalculate();
}

void Statistic::setHeader(QString newValue) {
    header_ = newValue;
    emit si_statisticChanged();
}

void Statistic::setModified(bool newValue) {
    modified = newValue;
    if(modified) {
        emit si_modified();
    }
}

double Statistic::stdDeviation() const {
    return sqrt(dispersion_);
}

void Statistic::initModel() {
    model.appendColumn(dataToItems(data));
    model.setHorizontalHeaderItem(0, new QStandardItem(tr("Value")));
    connect(&model, SIGNAL(itemChanged(QStandardItem*)), SLOT(sl_itemChanged(QStandardItem*)));
    setAllDataPoints();
}

void Statistic::append(QList<double> newData) {
    data << newData;
    setAllDataPoints();

    foreach(QStandardItem * item, dataToItems(newData)) {
        model.appendRow(item);
    }

    recalculate();
    setModified(true);
}

void Statistic::setDataPoint(int i) {
    dataPoints_[i] = QPointF(i + 1, data[i]);
}

void Statistic::setAllDataPoints() {
    dataPoints_.resize(data.size());
    for(int i = 0; i < data.size(); ++i) {
        setDataPoint(i);
    }
}

void Statistic::remove(QModelIndexList rows) {
    if(rows.isEmpty()) { return; }

    // First, take numberic indices from ModelIndexList
    QList<int> indices;
    foreach(QModelIndex ind, rows) {
        indices << ind.row();
    }

    // Second, sort the indices in reverse order
    // to avoid shifting indices while deleting
    qSort(indices.begin(), indices.end(), qGreater<int>());

    // Finally, go delete them!
    foreach(int i, indices) {
        data.removeAt(i);
        model.removeRow(i);
    }
    setAllDataPoints();
    recalculate();
    setModified(true);
}

void Statistic::recalculate() {
    min_ = max_ = average_ = dispersion_ = 0;
    thirdMoment_ = fourthMoment_ = 0;
    number_ = data.count();

    if(number_ != 0) {
        min_ = max_ = data[0];

        // Average, Min, Max
        foreach(double value, data) {
            average_ += value;
            min_ = qMin(min_, value);
            max_ = qMax(max_, value);
        }
        average_ /= number_;

        if(number_ > 1) {
            // Dispersion and moments
            foreach(double value, data) {
                dispersion_ += squared(value - average_);
                thirdMoment_ += cubed(value - average_);
                fourthMoment_ += squared(squared(value - average_));
            }
            dispersion_   /= (number_ - 1);
            thirdMoment_  /= number_;
            fourthMoment_ /= number_;
        }
    }
    resampleHistogram();

    emit si_statisticChanged();
}

void Statistic::resampleHistogram() {
    histogramSamples_.resize(histogramIntervals);
    double interval = qMax(max_ - min_, MIN_HIST_AREA)/histogramIntervals;
    // Create intervals
    for(int i = 0; i < histogramIntervals; ++i) {
        histogramSamples_[i].interval = QwtInterval(min_ + i*interval, min_ + (i+1)*interval, QwtInterval::ExcludeMaximum);
        histogramSamples_[i].value = 0;
    }

    // Count occurences of each interval
    foreach(double value, data) {
        int i = qMin( int( (value - min_)/interval ), histogramIntervals-1 );
        histogramSamples_[i].value++;
    }

    // Normalize to 1 if needed
    if(histogramFraction) {
        for(int i = 0; i < histogramIntervals; ++i) {
            histogramSamples_[i].value /= number_;
        }
    }
    emit si_histogramChanged();
}

void Statistic::sl_itemChanged(QStandardItem *item) {
    sl_valueChanged(item->row(), itemToValue(item->text()));
}

void Statistic::sl_valueChanged(int index, double newValue) {
    if(index < 0 || index >= data.size()) {
        return;
    }
    if(data[index] == newValue) {
        // nothing changed
        return;
    }
    data[index] = newValue;
    setDataPoint(index);
    model.item(index)->setText(valueToItem(newValue));
    recalculate();
    setModified(true);
}

void Statistic::sl_histogramIntervalsChanged(int newValue) {
    if(newValue == histogramIntervals) {
        return;
    }
    histogramIntervals = newValue;
    resampleHistogram();
}

void Statistic::sl_histogramFractionChanged(int newValue) {
    bool fraction = (newValue != 0);
    if(fraction == histogramFraction) {
        return;
    }
    histogramFraction = fraction;
    resampleHistogram();
}

QString Statistic::numberStr() const { return valueToItem(number_); }
QString Statistic::minStr() const { return valueToItem(min_); }
QString Statistic::maxStr() const { return valueToItem(max_); }
QString Statistic::averageStr() const { return valueToItem(average_); }
QString Statistic::dispersionStr() const { return valueToItem(dispersion_); }
QString Statistic::stdDeviationStr() const { return valueToItem(stdDeviation()); }
QString Statistic::thirdMomentStr() const { return valueToItem(thirdMoment_); }
QString Statistic::fourthMomentStr() const { return valueToItem(fourthMoment_); }

QString Statistic::dataAsHtmlTable(int columns) const {
    QString html = "<table border='1' cellpadding='3'>";
    html += QString("<tr><th>%1</th><th colspan='%2'>%3</th></tr>")
            .arg(tr("No."))
            .arg(columns)
            .arg(tr("Value"));

    int lastNo = data.size();
    int col = 0;
    for(int i = 0; i < data.size(); ++i) {
        if(col == 0) {
            html += QString("<tr><th>%1 - %2</th>").arg(i+1).arg(qMin(i+columns, lastNo));
        }

        html += QString("<td>%1</td>").arg(valueToItem(data[i]));

        ++col;
        if(col == columns) {
            html += "</tr>";
            col = 0;
        }
    }
    html += "</table>";
    return html;
}
