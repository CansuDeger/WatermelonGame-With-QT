#CENNET CANSU DEGER
#23100011023
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17



SOURCES += \
    main.cpp \
    watermelongame.cpp

HEADERS += \
    watermelongame.h


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
