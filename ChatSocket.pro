QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Chat/chat_item.cpp \
    Chat/chat_item2.cpp \
    file_transfer/receive_file.cpp\
    file_transfer/send_file.cpp\
    chat_window.cpp \
    logon.cpp \
    main.cpp \
    mainwindow.cpp \
    mesage_widget.cpp \
    mysqlite.cpp \
    regster.cpp

HEADERS += \
    Chat/chat_item.h \
    Chat/chat_item2.h \
    file_transfer/receive_file.h\
    file_transfer/send_file.h\
    chat_window.h \
    logon.h \
    mainwindow.h \
    mesage_widget.h \
    mysqlite.h \
    regster.h

FORMS += \
    Chat/chat_item.ui \
    Chat/chat_item2.ui \
    chat_window.ui \
    logon.ui \
    mainwindow.ui \
    mesage_widget.ui \
    regster.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Chat/image.qrc \
    image.qrc

SUBDIRS += \
    Chat/Chat.pro

DISTFILES += \
    Chat/head.jpg
