#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>
#include <QStandardItemModel>
#include <qwt_series_data.h>

class Statistic : public QObject
{
    Q_OBJECT
public:
    explicit Statistic(QObject *parent, QList<double> data);

    QStandardItemModel * itemModel() { return &model; }
    int number() const { return number_; }
    double min() const { return min_; }
    double max() const { return max_; }
    double average() const { return average_; }
    double dispersion() const { return dispersion_; }
    double stdDeviation() const;

    QVector<QwtIntervalSample> histogramSamples() const { return histogramSamples_; }

signals:
    void si_statisticChanged();

public slots:
    void sl_itemChanged(QStandardItem * item);
    void sl_valueChanged(int index, double newValue);
    
private:
    QList<double> data;
    QStandardItemModel model;

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
    QVector<QwtIntervalSample> histogramSamples_;

    void initModel();
    void recalculate();
    void resampleHistogram();
};

#endif // STATISTIC_H
