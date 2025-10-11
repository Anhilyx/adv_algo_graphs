QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/errorwidget.cpp \
    src/graphwidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/matrice.cpp \
    src/matricewidget.cpp

HEADERS += \
    src/errorwidget.h \
    src/graphwidget.h \
    src/mainwindow.h \
    src/matrice.h \
    src/matricewidget.h

FORMS += \
    src/mainwindow.ui

DISTFILES += \
    examples/*

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
