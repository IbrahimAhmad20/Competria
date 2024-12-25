QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += qscintilla2
CONFIG += c++17
LIBS += -L"C:\Qt\6.8.0\mingw_64\lib" -lqscintilla2_qt6
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admindashboard.cpp \
    adminlogin.cpp \
    createuser.cpp \
    database.cpp \
    main.cpp \
    mainwindow.cpp \
    problemmanager.cpp \
    sessionmanager.cpp \
    solveproblemwindow.cpp \
    submissionmanager.cpp \
    userdashboard.cpp \
    userlogin.cpp \
    usermanager.cpp

HEADERS += \
    admindashboard.h \
    adminlogin.h \
    createuser.h \
    database.h \
    mainwindow.h \
    problemmanager.h \
    sessionmanager.h \
    solveproblemwindow.h \
    submissionmanager.h \
    userdashboard.h \
    userlogin.h \
    usermanager.h

FORMS += \
    admindashboard.ui \
    adminlogin.ui \
    createuser.ui \
    mainwindow.ui \
    problemmanager.ui \
    solveproblemwindow.ui \
    userdashboard.ui \
    userlogin.ui \
    usermanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
