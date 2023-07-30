SOURCES += spotify-authentication.cc

LIBS += -L../lib
LIBS += -L/opt/openssl-1.1.1e/lib
LIBS += -L/usr/local/curl-armhf/lib
LIBS += -lrgbmatrix -lrt -lm -lpthread -lcurl -lssl -ldl -lcrypto

INCLUDEPATH += ../include
INCLUDEPATH += /usr/local/curl-armhf/include