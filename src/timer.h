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

#ifndef TIMER_H
#define TIMER_H

#include <QDateTime>

class Timer {
public:
    Timer() {
        start = QDateTime::currentMSecsSinceEpoch();
    }

    qint64 msecs() {
        return QDateTime::currentMSecsSinceEpoch() - start;
    }

private:
    qint64 start;
};

#endif // TIMER_H
