QT       += core gui

TARGET = statistiq
TEMPLATE = app

SOURCES += src/main.cpp\
           src/mainwindow.cpp \
    src/reader.cpp \
    src/statistic.cpp \
    src/reportwindow.cpp \
    src/newstatdialog.cpp \
    src/logger.cpp \
    src/logwindow.cpp

HEADERS += src/mainwindow.h \
    src/reader.h \
    src/statistic.h \
    src/histogram.h \
    src/reportwindow.h \
    src/tooltipplotpicker.h \
    src/newstatdialog.h \
    src/logger.h \
    src/logwindow.h \
    src/timer.h

FORMS    += mainwindow.ui \
    reportwindow.ui \
    newstatdialog.ui

TRANSLATIONS += statistiq_ru.ts

OTHER_FILES += \
    statistiq.rc

RESOURCES += \
    statistiq.qrc

win32 {
    RC_FILE     += statistiq.rc
    OTHER_FILES += statistiq.rc

    INCLUDEPATH  += C:/Qwt-6.0.1/include/

    CONFIG(debug, debug|release) {
        LIBS += C:/Qwt-6.0.1/lib/qwtd.dll
    } else {
        LIBS += C:/Qwt-6.0.1/lib/qwt.dll
    }
} else {
    INCLUDEPATH += /usr/include/qwt/
    LIBS += -lqwt
}
