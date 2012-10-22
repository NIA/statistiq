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
    explicit Statistic(QObject *parent, QList<double> data, QString header = "");

    QStandardItemModel * itemModel() { return &model; }
    QString header() { return header_; }

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

    int histogramIntervalsNumber() { return histogramIntervals; }
    bool isHisgogramFraction() { return histogramFraction; }
    void setHistogramIntervalsNumber(int value) { sl_histogramIntervalsChanged(value); }
    void setHistogramFraction(bool value) { sl_histogramFractionChanged(value); }

signals:
    void si_statisticChanged();
    void si_histogramChanged();

public slots:
    void sl_itemChanged(QStandardItem * item);
    void sl_valueChanged(int index, double newValue);
    void sl_histogramIntervalsChanged(int newValue);
    void sl_histogramFractionChanged(int newValue);
    
private:
    QList<double> data;
    QStandardItemModel model;
    QString header_;

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
};

#endif // STATISTIC_H
