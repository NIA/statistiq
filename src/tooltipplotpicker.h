/*
 * (C) Copyright 2012 Kuban State University (http://kubsu.ru/),
 *     Faculty of Physics and Technology, Physics and Information Systems Chair
 *
 * All rights reserved. This file is part of ProcessData, a program
 * for getting various statistical information from measurement data.
 *
 * Developed by Ivan Novikov (http://github.com/NIA)
 * under the direction of Leontiy Grigorjan
 *
 */

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
