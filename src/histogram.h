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

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <qwt_plot_histogram.h>

class Histogram : public QwtPlotHistogram
{
public:
    Histogram(QwtPlot *plot) {
        attach(plot);
        setStyle(QwtPlotHistogram::Columns);
        setBrush(QBrush(QColor(0, 200, 30, 200)));
    }
};

#endif // HISTOGRAM_H
