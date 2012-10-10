#include "statistic.h"

#include <cmath>

namespace {
    double squared(double value) {
        return value * value;
    }
}

Statistic::Statistic(QObject *parent, QList<double> data) :
    QObject(parent), data(data)
{
    recalculate();
}

double Statistic::stdDeviation() const {
    return sqrt(dispersion_);
}

void Statistic::recalculate() {
    average_ = 0;
    dispersion_ = 0;
    number_ = data.count();

    if(number_ != 0) {
        // Average
        foreach(double value, data) {
            average_ += value;
        }
        average_ /= number_;

        // Dispersion
        foreach(double value, data) {
            dispersion_ += squared(value - average_);
        }
        dispersion_ /= number_;
    }

    emit si_statisticChanged();
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
    recalculate();
}
