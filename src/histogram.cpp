#include "histogram.h"
#include <qwt_plot.h>
#include <qwt_plot_picker.h>

namespace {

    class HistogramPlotPicker : public QwtPlotPicker {
    public:
        HistogramPlotPicker(QwtPlotCanvas * canvas) : QwtPlotPicker(canvas) {}
    protected:
        virtual QwtText trackerText(const QPoint & p) const {
            QPointF p1 = invTransform(p);
            return QwtText(tr("%1; %2").arg(p1.x()).arg(p1.y()));
        }
    };

}

Histogram::Histogram(QwtPlot *plot)
{
    attach(plot);
    setStyle(QwtPlotHistogram::Columns);
    setBrush(QBrush(QColor(0, 200, 30)));

    // Histogram tooltips
    HistogramPlotPicker * picker = new HistogramPlotPicker(plot->canvas());
    picker->setTrackerMode(QwtPicker::AlwaysOn);
    plot->setMouseTracking(true);

}
