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

#include "logwindow.h"
#include <QDateTime>
#include <QIcon>

LogWindow::LogWindow(QWidget *parent) :
    super(parent)
{
    connect(Logger::instance(), SIGNAL(si_messageAdded(Logger::Level,QString)), SLOT(sl_messageAdded(Logger::Level,QString)));

    setMinimumSize(600, 350);
    setWindowTitle(tr("StatistiQ log"));
    setWindowIcon(QIcon(":/icons/images/log.ico"));

    setUndoRedoEnabled(false);
    setReadOnly(true);
    setLineWrapMode(QPlainTextEdit::WidgetWidth);
    setTextInteractionFlags(Qt::NoTextInteraction|Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    setCenterOnScroll(true);
}

void LogWindow::sl_messageAdded(Logger::Level level, QString message) {
    QString formattedMessage = "";
    formattedMessage += QString("<font color='%1'>").arg(levelColor(level));
    formattedMessage += QString("[%1] ").arg(QDateTime::currentDateTime().toString(Qt::DefaultLocaleShortDate));
    formattedMessage += (levelName(level) + ": ");
    formattedMessage += message;
    formattedMessage += "</font>";

    appendHtml(formattedMessage);
}

QString LogWindow::levelName(Logger::Level level) {
    switch(level) {
    case Logger::Trace:
        return tr("TRACE");
    case Logger::Info:
        return tr("INFO");
    case Logger::Warning:
        return tr("WARNING");
    case Logger::Error:
        return tr("ERROR");
    default:
        return "";
    }
}

QString LogWindow::levelColor(Logger::Level level) {
    switch(level) {
    case Logger::Trace:
        return "gray";
    case Logger::Info:
        return "black";
    case Logger::Warning:
        return "maroon";
    case Logger::Error:
        return "red";
    default:
        return "";
    }
}

void LogWindow::closeEvent(QCloseEvent *) {
    emit si_closed();
}