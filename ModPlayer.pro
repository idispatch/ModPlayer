APP_NAME = ModPlayer

CONFIG += qt warn_on cascades10
QT += network
LIBS += -lbb -lbbdata -lbbutility -lbbsystem -lbbmultimedia -lasound -lcrypto -lscreen -lcurl -lpackageinfo -lbbdevice -lQtLocationSubset
DEFINES += __BLACKBERRY__ __BB10__ __QNXNTO__ HAVE_CONFIG_H=1

include(config.pri)
