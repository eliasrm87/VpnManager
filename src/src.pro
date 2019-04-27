#-------------------------------------------------
#
# Project created by QtCreator 2017-12-09T01:22:54
#
#-------------------------------------------------

QT       += core gui network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vpnManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += link_pkgconfig
PKGCONFIG += libnm

target.files = vpnManager
target.path  = /usr/local/bin/
INSTALLS += target

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        wgtserver.cpp \
        networkmanager.cpp \
        settingsdialog.cpp \
        providers/abstractserver.cpp \
        providers/country.cpp \
        providers/ipvanish/ipvanish.cpp \
        providers/ipvanish/ipvserver.cpp \
        providers/nordvpn/nordvpn.cpp \
        providers/nordvpn/nvpnserver.cpp \
        providers/provider.cpp

HEADERS += \
        mainwindow.h \
        wgtserver.h \
        networkmanager.h \
        settingsdialog.h \
        providers/abstractserver.h \
        providers/country.h \
        providers/ipvanish/ipvanish.h \
        providers/ipvanish/ipvserver.h \
        providers/nordvpn/nordvpn.h \
        providers/nordvpn/nvpnserver.h \
        providers/provider.h

RESOURCES +=
