#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <qwt_plot_histogram.h>

class Histogram : public QwtPlotHistogram
{
public:
    Histogram(QwtPlot * plot);
};

#endif // HISTOGRAM_H
