QT       += core gui

TARGET = statistiq
TEMPLATE = app


SOURCES += src/main.cpp\
           src/mainwindow.cpp \
    src/reader.cpp

HEADERS += src/mainwindow.h \
    src/reader.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    statistiq.rc

RESOURCES += \
    statistiq.qrc

win32 {
  RC_FILE     += statistiq.rc
  OTHER_FILES += statistiq.rc
}
