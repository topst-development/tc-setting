greaterThan(QT_MAJOR_VERSION, 4) {
    QT += qml quick
    QT.gui.CONFIG -= opengl
}

TEMPLATE = app
DESTDIR = bin
CONFIG += release
DEFINES -= UNICODE \
    _UNICODE

CONFIG(debug, debug|release) {
    OBJECTS_DIR = build/debug/obj
    MOC_DIR = build/debug/moc
    UI_HEADERS_DIR = build/debug/ui_header
    TARGET = Setting
    INCLUDEPATH += build/debug/ui_header
}
else {
    OBJECTS_DIR = build/release/obj
    MOC_DIR = build/release/moc
    UI_HEADERS_DIR = build/release/ui_header
    TARGET = Setting
    INCLUDEPATH += build/release/ui_header
}

INVITE_LIBS = TcUtils \
	tc-color-adjustment \
	libxml-2.0 \
    dbus-1

CONFIG += link_pkgconfig
PKGCONFIG += $$INVITE_LIBS
INCLUDEPATH += ./include
INCLUDEPATH += ./lib/include
INCLUDEPATH += ../include

SOURCES = src/main.cpp \
		  src/SettingIFManager.cpp \
		  src/SettingDBusManager.cpp \
		  src/Setting.cpp \
		  src/DBusMsgDefNames.c \
		  src/AudioManager.cpp \
		  src/ColorAdjustmentManager.cpp


HEADERS = \
		  include/Setting.h \
		  include/SettingDBusManager.h \
		  include/DBusMsgDef.h \
		  include/SettingIFManager.h \
		  include/AudioManager.h \
		  include/ColorAdjustmentManager.h

LIBS += -lpthread -lrt -ltccoloradjustment


equals(TSOUND, true) {
	DEFINES += ENABLE_TSOUND

	SOURCES += src/BandEQManager.cpp \
		  src/PowerBassTrebleEnhancementManager.cpp \
		  src/BalanceFaderManager.cpp \
		  src/EQController.cpp \
		  src/am3d/tc_audio_dsp.c

	HEADERS += include/EQMacro.h \
		  include/BandEQManager.h \
		  include/PowerBassTrebleEnhancementManager.h \
		  include/BalanceFaderManager.h \
		  include/EQController.h \
		  src/am3d/tc_audio_dsp.h
}

equals(BLUETOOTH, true) {
	DEFINES += ENABLE_BT

	SOURCES += src/BluetoothManager.cpp

	HEADERS += include/BluetoothManager.h
}

equals(FWUPDATE, 1) {
	DEFINES += ENABLE_FWUPDATE

	SOURCES += src/FWUpdateManager.cpp \
			   src/FWUpdateThread.cpp

	HEADERS += include/FWUpdateManager.h \
				include/FWUpdateThread.h

	LIBS += -lTcUpdateEngine
}

equals(DISABLE_3DLUT, 1) {
	DEFINES += DISABLE_HUESATURATION
}

RESOURCES += res/Setting.qrc

target.path = /usr/bin
INSTALLS += target
