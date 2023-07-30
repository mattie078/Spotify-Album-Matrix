Uses Qt5.15 to cross compile from WSL Ubuntu to Raspberry Pi.
https://github.com/UvinduW/Cross-Compiling-Qt-for-Raspberry-Pi-4 

For CURL you have to manually build the latest version with the latest manual build of OpenSSL

Manual build CURL:
sudo ./configure --prefix=/usr/local/curl-armhf --disable-shared --disable-ldap --enable-ipv6 --enable-unix-sockets --with-ssl=/opt/openssl-1.1.1e/ --host=arm-linux-gnueabihf

Manual build OpenSSL:
sudo ./configure linux-generic32 --prefix=/opt/openssl-1.1.1e --openssldir=/opt/openssl-1.1.1e -static

    Make sure that the gcc and g++ from the arm toolchain is the standard on the system using sudo update-alternatives.
    Because OpenSSL builds don't use the CC and CXX variable while CURL does. 