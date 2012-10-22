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
            QwtText text(tr("%1; %2").arg(p1.x()).arg(p1.y()));
            text.setBackgroundBrush(QBrush(QColor(255, 255, 255)));
            QFont font = text.font();
            font.setPointSize(12);
            text.setFont(font);
            return text;
        }
    };

}

Histogram::Histogram(QwtPlot *plot)
{
    attach(plot);
    setStyle(QwtPlotHistogram::Columns);
    setBrush(QBrush(QColor(0, 200, 30, 200)));

    // Histogram tooltips
    HistogramPlotPicker * picker = new HistogramPlotPicker(plot->canvas());
    picker->setTrackerMode(QwtPicker::AlwaysOn);
    plot->setMouseTracking(true);

}
