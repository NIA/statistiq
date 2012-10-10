#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>

class Statistic : public QObject
{
    Q_OBJECT
public:
    explicit Statistic(QObject *parent, QList<double> data);

    int number() const { return number_; }
    double average() const { return average_; }
    double dispersion() const { return dispersion_; }
    double stdDeviation() const;

signals:
    void si_statisticChanged();

public slots:
    void sl_valueChanged(int index, double newValue);
    
private:
    QList<double> data;

    int number_;
    double average_;
    double dispersion_;

    void recalculate();

};

#endif // STATISTIC_H
