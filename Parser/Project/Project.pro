QT += core gui widgets svg svgwidgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    graphviz_renderer.cpp \
    parser.cpp

HEADERS += \
    mainwindow.h \
    graphviz_renderer.h \
    parser.h

FORMS += \
    mainwindow.ui


# Deployment rules (keep them as they are)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
