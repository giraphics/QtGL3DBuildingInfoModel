include(GraphicsCommon.pri)

QT += widgets

SOURCES += \
    main.cpp \
    scrolldialog.cpp \
    scroller.cpp \
    OpenGLWidget.cpp

QMAKE_MAC_SDK = macosx10.12

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target

HEADERS += \
    scrolldialog.h \
    scroller.h \
    OpenGLWidget.h

RESOURCES += \
    modelqrc.qrc

FORMS += \
    scrolldialog.ui
