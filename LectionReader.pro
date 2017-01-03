TEMPLATE = app

INCLUDEPATH += /usr/local/opencv2/
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc
QT += qml quick widgets
QT += sql
QT += printsupport
QT += core
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
