APP_NAME = ModPlayer

CONFIG += qt warn_on cascades10
LIBS += -lasound
DEFINES += __BLACKBERRY__ __BB10__ __QNXNTO__ HAVE_CONFIG_H=1

include(config.pri)
