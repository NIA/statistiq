QT       += core gui

TARGET = statistiq
TEMPLATE = app

SOURCES += src/main.cpp\
           src/mainwindow.cpp \
    src/reader.cpp \
    src/statistic.cpp

HEADERS += src/mainwindow.h \
    src/reader.h \
    src/statistic.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    statistiq.rc

RESOURCES += \
    statistiq.qrc

win32 {
    RC_FILE     += statistiq.rc
    OTHER_FILES += statistiq.rc

    INCLUDEPATH  += C:/Qwt-6.0.1/include/
    release:LIBS += C:/Qwt-6.0.1/lib/qwt.dll
    debug:LIBS   += C:/Qwt-6.0.1/lib/qwtd.dll
}
