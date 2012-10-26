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
