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

#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>
#include <QStandardItemModel>
#include <QPointF>
#include <qwt_series_data.h>

class Statistic : public QObject
{
    Q_OBJECT

public:
    explicit Statistic(QObject *parent, QList<double> data, QString header = "", bool modified = false);
    void append(QList<double> newData);
    void remove(QModelIndexList rows);

    QStandardItemModel * itemModel() { return &model; }
    QList<double> rawData() { return data; }
    QString header() { return header_; }
    void setHeader(QString newValue);

    QString filePath() { return filePath_; }
    void setFilePath(QString filePath) { filePath_ = filePath; }
    bool isModified() { return modified; }
    void setModified(bool newValue);

    int number() const { return number_; }
    double min() const { return min_; }
    double max() const { return max_; }
    double average() const { return average_; }
    double dispersion() const { return dispersion_; }
    double stdDeviation() const;
    double thirdMoment() const { return thirdMoment_; }
    double fourthMoment() const { return fourthMoment_; }

    // String versions of values getters

    QString numberStr() const;
    QString minStr() const;
    QString maxStr() const;
    QString averageStr() const;
    QString dispersionStr() const;
    QString stdDeviationStr() const;
    QString thirdMomentStr() const;
    QString fourthMomentStr() const;

    QVector<QwtIntervalSample> histogramSamples() const { return histogramSamples_; }
    QVector<QPointF> dataPoints() const { return dataPoints_; }
    QString dataAsHtmlTable(int columns) const;

    int histogramIntervalsNumber() { return histogramIntervals; }
    bool isHisgogramFraction() { return histogramFraction; }
    void setHistogramIntervalsNumber(int value) { sl_histogramIntervalsChanged(value); }
    void setHistogramFraction(bool value) { sl_histogramFractionChanged(value); }

signals:
    void si_statisticChanged();
    void si_histogramChanged();
    void si_modified();

public slots:
    void sl_itemChanged(QStandardItem * item);
    void sl_valueChanged(int index, double newValue);
    void sl_histogramIntervalsChanged(int newValue);
    void sl_histogramFractionChanged(int newValue);
    
private:
    QList<double> data;
    QStandardItemModel model;
    QString header_;
    QString filePath_;
    bool modified;

    // -- Options --
    // The number of the intervals for building histogram
    int histogramIntervals;
    // If true, show fractions (e.g. 0.03) instead of numbers in histogram
    bool histogramFraction;

    // -- Statistical values --
    int number_;
    double min_;
    double max_;
    double average_;
    double dispersion_;
    double thirdMoment_;
    double fourthMoment_;
    QVector<QwtIntervalSample> histogramSamples_;
    QVector<QPointF> dataPoints_;

    void initModel();
    void recalculate();
    void resampleHistogram();
    void setDataPoint(int i);
    void setAllDataPoints();
};

#endif // STATISTIC_H
