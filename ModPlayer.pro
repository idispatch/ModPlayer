APP_NAME = ModPlayer

CONFIG += qt warn_on cascades10 mobility
MOBILITY += sensors
QT += network
LIBS += -lbb -lbbdata -lbbutility -lbbsystem -lbbplatform -lbbmultimedia -lbbcascadespickers -lasound -lcrypto -lscreen -lcurl -lpackageinfo -lbbdevice -lQtLocationSubset -ltag -lFlurry -laudio_manager
DEFINES += __BLACKBERRY__ __BB10__ __QNXNTO__ HAVE_CONFIG_H=1 _FILE_OFFSET_BITS=64 _LARGEFILE64_SOURCE=1

#QMAKE_CXXFLAGS_RELEASE += O2 -flto -ffast-math
QMAKE_CXXFLAGS_RELEASE += -flto -ffast-math
QMAKE_LFLAGS += -flto

# -mcpu=cortex-a15 for QCT CPU

device {
    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG 
    }
    CONFIG(release, debug|release) {
        DEFINES += NDEBUG
    }
}
simulator {
    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG 
    }
    CONFIG(release, debug|release) {
        DEFINES += NDEBUG
    }
}

include(config.pri)
