TEMPLATE = app

QT += qml quick widgets
QT += sql
QT += printsupport
CONFIG += c++11

SOURCES += main.cpp \
    qdatabasework.cpp \
    qmodelparent.cpp \
    qmlbuttonclicks.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    qdatabasework.h \
    qmodelparent.h \
    qmlbuttonclicks.h
