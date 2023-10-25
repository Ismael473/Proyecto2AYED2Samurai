QT += core gui multimedia multimediawidgets widgets serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SamuraiA.cpp \
    SamuraiB.cpp \
    Server.cpp \
    client.cpp \
    genetic.cpp \
    main.cpp \
    obstaculo.cpp \
    samurai_stats_viewer.cpp

HEADERS += \
    SamuraiA.h \
    SamuraiB.h \
    Server.h \
    client.h \
    genetic.h \
    obstaculo.h \
    samurai_stats_viewer.h

FORMS += \
    client.ui

CONFIG += c++17

# Configuración para TagLib con vcpkg
VCPKG_PATH = C:/vcpkg-master/installed/x64-windows
INCLUDEPATH += $$VCPKG_PATH/include
#LIBS += -L$$VCPKG_PATH/lib -ltag

# Copia automática de tag.dll al directorio de salida
VCPKG_BIN = C:/vcpkg-master/installed/x64-windows/bin

win32:CONFIG(release, debug|release): {
    QMAKE_POST_LINK += copy /y $$quote($$VCPKG_BIN/tag.dll) $$quote($$OUT_PWD/release/)
}
win32:CONFIG(debug, debug|release): {
    QMAKE_POST_LINK += copy /y $$quote($$VCPKG_BIN/tagd.dll) $$quote($$OUT_PWD/debug/)
}

# Reglas predeterminadas para el despliegue
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
