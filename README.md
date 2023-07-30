<h1>Displays Spotify's current album cover on a 64x64 matrix board</h1>

The idea came from Live Spotify Album Art Display on [Sparkfun](https://learn.sparkfun.com/tutorials/live-spotify-album-art-display/all) so for the full story, go there.

**WARNING**: This implementation is tailored to my own needs and is highly likely not applicable for your use case. <br>
Though, feel free to look through the code and documentation!

<h2></h2>

Developing on a Raspberry Pi is annoyingly slow. See build times up to 5 minutes per simple change. <br>
So for my need I build the application using WSL on Ubuntu 20.04.

I used this guide for Qt5.15 to cross compile from WSL Ubuntu to Raspberry Pi. Mainly because with all the other guides I had major issues with the linker not finding all the necessary ARMv7 libraries.
[The guide I used](https://github.com/UvinduW/Cross-Compiling-Qt-for-Raspberry-Pi-4).

<h2></h2>

Because we need to fetch our token from Spotify's API we use CURL (and optionally SSL). I made this project with SSL compatilation in mind but I don't actively use it in my code. So this step is optional if you don't need it. <br> <br>
For OpenSSL I had the greatest succes installing the specific version using [this guide](https://stackoverflow.com/questions/60821697/how-to-build-openssl-for-arm-linux).

Manual build OpenSSL: <br>

    sudo ./configure linux-generic32 --prefix=/opt/openssl-1.1.1e --openssldir=/opt/openssl-1.1.1e -static

<h2></h2>

For CURL you have to manually build the latest version with the latest manual build of OpenSSL. See [this guide](https://curl.se/docs/install.html). <br>

Manual build CURL: <br>

    sudo ./configure --prefix=/usr/local/curl-armhf --disable-shared --disable-ldap --enable-ipv6 --enable-unix-sockets --with-ssl=/opt/openssl-1.1.1e/ --host=arm-linux-gnueabihf

<h2></h2>

**IMPORTANT:** <br>
Make sure that the gcc and g++ from the arm toolchain is the standard on the system using: 
    
    sudo update-alternatives
    
Because OpenSSL builds don't use the CC and CXX variable while CURL does. 

<h2>Last but not least</h2>

Don't forget to fill in the .env variables otherways the program will crash itself! :)
