QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QMAKE_CXXFLAGS += -std=c++11

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

    greaterThan(QT_MAJOR_VERSION, 4) {
        INCLUDEPATH  += C:/Qwt-6.1.0-rc3/include/
        LIBS +=       -LC:/Qwt-6.1.0-rc3/lib
    } else {
        INCLUDEPATH  += C:/Qwt-6.0.1/include/
        LIBS +=       -LC:/Qwt-6.0.1/lib
    }
    CONFIG(debug, debug|release) {
        LIBS += -lqwtd
    } else {
        LIBS += -lqwt
    }
}
