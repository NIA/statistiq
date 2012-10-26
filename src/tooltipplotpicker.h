#ifndef TOOLTIPPLOTPICKER_H
#define TOOLTIPPLOTPICKER_H

#include <qwt_plot.h>
#include <qwt_plot_picker.h>

class TooltipPlotPicker : public QwtPlotPicker {
public:
    TooltipPlotPicker(QwtPlot * plot)
        : QwtPlotPicker(plot->canvas())
    {
        setTrackerMode(QwtPicker::AlwaysOn);
        plot->setMouseTracking(true);
    }
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


#endif // TOOLTIPPLOTPICKER_H
