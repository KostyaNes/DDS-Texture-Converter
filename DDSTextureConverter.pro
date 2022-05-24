QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ApplicationHandlersManager.cpp \
    ApplicationSettings.cpp \
    ConversionController.cpp \
    DDSPreviewerWidget.cpp \
    DDSReader.cpp \
    DDSWriter.cpp \
    DXT1Converter.cpp \
    DXT3Converter.cpp \
    DXT5Converter.cpp \
    GenericConverter.cpp \
    OpenGLWidget.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    ApplicationHandlersManager.h \
    ApplicationSettings.h \
    AssetProcessingNotificationsHandler.h \
    AssetProcessingProgressHandler.h \
    ConversionController.h \
    DDSDefs.h \
    DDSPreviewerWidget.h \
    DDSReader.h \
    DDSWriter.h \
    DXT1Converter.h \
    DXT3Converter.h \
    DXT5Converter.h \
    GenericConverter.h \
    ImageDefs.h \
    ImageFunc.h \
    MainWindow.h \
    OpenGLWidget.h

FORMS += \
    DDSPreviewerWidget.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    dxtn_uml.qmodel \
    ui_uml.qmodel
