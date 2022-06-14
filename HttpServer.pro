QT       += \
        core gui \
        network \
        sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/restfulserver.cpp \
    src/httprequest.cpp \
    src/httpresponse.cpp \
    src/messagehandler.cpp \
    src/messagereader.cpp \
    src/widget.cpp \

HEADERS += \
    headers/restfulserver.h \
    headers/httprequest.h \
    headers/httpresponse.h \
    headers/messagehandler.h \
    headers/messagereader.h \
    headers/widget.h \

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
