<h1>puNES</h1>

[![Gentoo package](https://repology.org/badge/version-for-repo/gentoo/punes.svg)](https://repology.org/project/punes/versions)
[![AUR package](https://repology.org/badge/version-for-repo/aur/punes.svg)](https://repology.org/project/punes/versions)
[![SlackBuilds package](https://repology.org/badge/version-for-repo/slackbuilds/punes.svg)](https://repology.org/project/punes/versions)
[![Rosa 2019.1 package](https://repology.org/badge/version-for-repo/rosa_2019_1/punes.svg)](https://repology.org/project/punes/versions)
[![LiGurOS stable package](https://repology.org/badge/version-for-repo/liguros_stable/punes.svg)](https://repology.org/project/punes/versions)
[![Wikidata package](https://repology.org/badge/version-for-repo/wikidata/punes.svg)](https://repology.org/project/punes/versions)

Description
-----------

Nintendo Entertaiment System emulator

WIP Builds ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/punesemu/puNES?style=flat) [![CodeFactor](https://www.codefactor.io/repository/github/punesemu/punes/badge/master)](https://www.codefactor.io/repository/github/punesemu/punes/overview/master) [![Build status](https://ci.appveyor.com/api/projects/status/t820vxmvtvbfbll7?svg=true)](https://ci.appveyor.com/project/punesemu/punes) 
-----------
always updated to the last commit:
* Windows 32 bit : [`OpenGL`](https://ci.appveyor.com/api/projects/punesemu/puNES/artifacts/punes32.wip.opengl.zip) - [`D3D9`](https://ci.appveyor.com/api/projects/punesemu/puNES/artifacts/punes32.wip.d3d9.zip)
* Windows 64 bit : [`OpenGL`](https://ci.appveyor.com/api/projects/punesemu/puNES/artifacts/punes64.wip.opengl.zip) - [`D3D9`](https://ci.appveyor.com/api/projects/punesemu/puNES/artifacts/punes64.wip.d3d9.zip)

Note : 32 bit versions are Windows XP compatible.

Support:
-----------

If you want pay a beer : [![Beerpay](https://beerpay.io/punesemu/puNES/badge.svg?style=beer)](https://beerpay.io/punesemu/puNES) or <span class="badge-paypal"><a href="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=QPPXNRL5NAHDC" title="Donate to this project using Paypal"><img src="https://img.shields.io/badge/paypal-donate-yellow.svg" alt="PayPal donate button" /></a></span>

How to Compile
-----------

* [Linux](#linux)
* [FreeBSD](#freebsd)
* [OpenBSD](#openbsd)
* [Windows](#windows)

## Linux
#### Dependencies
* [Qt5](https://www.qt.io) with OpenGL support (qtbase, qtsvg and qttools)
* [nvidia-cg](https://developer.nvidia.com/cg-toolkit)
* [alsa](https://www.alsa-project.org)
* [cmake](https://cmake.org)
* (optional) [ffmpeg libraries >= 4.0](https://ffmpeg.org) if you want video and audio recording support (libavcodec, libavformat, libavutil, libswresample and libswscale). See [notes](#ffmpeg).
#### Compilation of puNES
```bash
git clone https://github.com/punesemu/punes
cd punes
./autogen.sh
./configure
make
```
the executable `punes` is in the `src` directory.
#### Linux Debug version
If you need the debug version then you need to replace the `./configure` command of the previous examples with the following:
```bash
CFLAGS="-g -DDEBUG" CXXFLAGS="-g -DDEBUG" ./configure --disable-release [...]
```
where `[...]` are the other necessary options.
#### Example on how to compile on Ubuntu 20.10
```bash
sudo apt-get install git cmake autotools-dev autoconf build-essential pkg-config libasound2-dev qtbase5-dev qttools5-dev-tools libqt5svg5-dev nvidia-cg-toolkit
sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswresample-dev libswscale-dev
git clone https://github.com/punesemu/puNES
cd puNES
./autogen.sh
./configure
make
```
to start the emulator
```bash
./src/punes
```
## FreeBSD
#### Dependencies
* [Qt5](https://www.qt.io) with OpenGL support (qtbase, qtsvg and qttools)
* [sndio](http://www.sndio.org)
* [cmake](https://cmake.org)
* (optional) [ffmpeg libraries >= 4.0](https://ffmpeg.org) if you want video and audio recording support (libavcodec, libavformat, libavutil, libswresample and libswscale). See [notes](#ffmpeg).
#### Compilation of puNES
```bash
git clone https://github.com/punesemu/punes
cd punes
./autogen.sh
CC=cc CXX=c++ ./configure
make
```
the executable `punes` is in the `src` directory.
#### FreeBSD Debug version
If you need the debug version then you need to replace the `./configure` command of the previous examples with the following:
```bash
CFLAGS="-g -DDEBUG" CXXFLAGS="-g -DDEBUG" CC=cc CXX=c++ ./configure --disable-release [...]
```
where `[...]` are the other necessary options.
## OpenBSD
#### Dependencies
* [Qt5](https://www.qt.io) with OpenGL support (qtbase, qtsvg and qttools)
* [sndio](http://www.sndio.org)
* [cmake](https://cmake.org)
* (optional) [ffmpeg libraries >= 4.0](https://ffmpeg.org) if you want video and audio recording support (libavcodec, libavformat, libavutil, libswresample and libswscale). See [notes](#ffmpeg).
#### Compilation of puNES
```bash
git clone https://github.com/punesemu/punes
cd punes
./autogen.sh
CC=cc CXX=c++ ./configure
make
```
the executable `punes` is in the `src` directory.
#### OpenBSD Debug version
If you need the debug version then you need to replace the `./configure` command of the previous examples with the following:
```bash
CFLAGS="-g -DDEBUG" CXXFLAGS="-g -DDEBUG" CC=cc CXX=c++ ./configure --disable-release [...]
```
where `[...]` are the other necessary options.
## Windows
#### Dependencies
* [Qt5](https://www.qt.io) with OpenGL support (5.6.3 is the last if you want the support for Windows XP)
#### Development Environment installation
1. install [MSYS2](https://www.msys2.org/)
2. open "MSYS2 MinGW 64-bit" shell (or 32 bit if you want compile the 32 bit version of puNES)
```bash
pacman -Syu
```
3. close the MSYS2 window and run it again from Start menu
```bash
pacman -Su
pacman -S base-devel git wget p7zip unzip mingw-w64-i686-cmake mingw-w64-x86_64-cmake
pacman -S perl ruby python2 mingw-w64-i686-toolchain mingw-w64-x86_64-toolchain
exit
```
4. open a new MSYS2 shell and build the necessary libraries
#### Compilation of the Qt5 libraries
5. download and unzip the sources
```bash
wget http://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtbase-everywhere-src-5.15.0.zip
unzip qtbase-everywhere-src-5.15.0.zip
mv qtbase-everywhere-src-5.15.0 qt5
```
the renaming of the directory is necessary to not generate a compile-time error caused by the 255 characters maximum path length limitation on Windows, This is the typical error message you might encounter:
```code
"../../../../include/QtEventDispatcherSupport/5.15.0/QtEventDispatcherSupport/private/qwindowsguieventdispatcher_p.h:1:10: fatal error: ../../../../../src/platformsupport/eventdispatchers/qwindowsguieventdispatcher_p.h: No such file or directory"
```
6. compile the libraries
```bash
cd qt5
echo -e "QMAKE_LFLAGS += -static -static-libgcc\nDEFINES += QT_STATIC_BUILD\n" >> mkspecs/win32-g++/qmake.conf
./configure.bat -prefix $MINGW_PREFIX -extprefix $MINGW_PREFIX -bindir $MINGW_PREFIX/lib/qt5/bin -headerdir $MINGW_PREFIX/include/qt5 -libdir $MINGW_PREFIX/lib/qt5 -archdatadir $MINGW_PREFIX/lib/qt5 -plugindir $MINGW_PREFIX/lib/qt5/plugins -libexecdir $MINGW_PREFIX/lib/qt5/bin -datadir $MINGW_PREFIX/share/qt5 -docdir $MINGW_PREFIX/share/doc/qt5 -translationdir $MINGW_PREFIX/share/qt5/translations -sysconfdir $MINGW_PREFIX/etc/xdg -examplesdir $MINGW_PREFIX/share/qt5/examples -testsdir $MINGW_PREFIX/share/qt5/tests -platform win32-g++ -nomake examples -nomake tests -nomake tools -no-compile-examples -release -opensource -confirm-license -static -c++std c++11 -sse2 -static-runtime -make libs -no-ltcg -no-dbus -no-accessibility -no-inotify -no-iconv -no-icu -no-openssl -no-system-proxies -no-cups -no-fontconfig -opengl desktop -no-angle -gif -ico -qt-libpng -qt-libjpeg -qt-pcre -qt-zlib -qt-freetype
make
```
7. and finally install them
```bash
make install
sed -i -e s,Qt5OpenGLd,Qt5OpenGL,g -e s,Qt5OpenGLExtensionsd,Qt5OpenGLExtensions,g -e s,Qt5Concurrentd,Qt5Concurrent,g -e s,Qt5Cored,Qt5Core,g -e s,Qt5Guid,Qt5Gui,g -e s,Qt5Networkd,Qt5Network,g -e s,Qt5PrintSupportd,Qt5PrintSupport,g -e s,Qt5Sqld,Qt5Sql,g -e s,Qt5Testd,Qt5Test,g -e s,Qt5Widgetsd,Qt5Widgets,g -e s,Qt5Xmld,Qt5Xml,g -e s,libqtpcre2d,libqtpcre2,g -e s,libqtlibpngd,libqtlibpng,g -e s,libqtharfbuzzd,libqtharfbuzz,g $MINGW_PREFIX/lib/qt5/pkgconfig/*
cp -v $MINGW_PREFIX/lib/qt5/pkgconfig/* $MINGW_PREFIX/lib/pkgconfig/.
cd ..
```
8. now it's time for the SVG module...
```bash
wget http://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtsvg-everywhere-src-5.15.0.zip
unzip qtsvg-everywhere-src-5.15.0.zip
mv qtsvg-everywhere-src-5.15.0 qt5svg
cd qt5svg
$MINGW_PREFIX/lib/qt5/bin/qmake
make
make install
sed -i -e s,Qt5Svgd,Qt5Svg,g -e s,Qt5Cored,Qt5Core,g -e s,Qt5Guid,Qt5Gui,g -e s,Qt5Widgetsd,Qt5Widgets,g $MINGW_PREFIX/lib/qt5/pkgconfig/*
cp -v $MINGW_PREFIX/lib/qt5/pkgconfig/* $MINGW_PREFIX/lib/pkgconfig/.
cd ..
```
9. ...and for the tools
```bash
wget http://download.qt.io/archive/qt/5.15/5.15.0/submodules/qttools-everywhere-src-5.15.0.zip
unzip qttools-everywhere-src-5.15.0.zip
mv qttools-everywhere-src-5.15.0 qt5tools
cd qt5tools
$MINGW_PREFIX/lib/qt5/bin/qmake
make
make install
cd ..
```
#### Compilation of puNES
10. Now you have everything you need to compile correctly puNES
```bash
git clone https://github.com/punesemu/punes
cd punes
./autogen.sh
```
if you want D3D9 version :
```bash
./configure --with-d3d9
make
```
otherwise :
```bash
./configure --with-opengl
make
```
The executable `punes.exe` is in the `src` directory but in order to run it you need the following dlls:
* 7z.dl
* avcodec-58.dll
* avformat-58.dll
* avutil-56.dll
* cg.dll
* cgD3D9.dll (only for D3D9 version)
* cgGL.dll (only for OpenGL version)
* libwinpthread-1.dll
* swresample-3.dll
* swscale-5.dll

that you can download here : [`64bit`](https://www.dropbox.com/s/7e01vzhmc4k5ead/puNES_x86_64_dlls.zip?dl=1) version or [`32bit`](https://www.dropbox.com/s/xaspmjmczkg1zjt/puNES_i686_dlls.zip?dl=1) version.
#### Windows Debug version
If you need the debug version then you need to replace the `./configure` command of the previous examples with the following:
```bash
CFLAGS="-g -DDEBUG" CXXFLAGS="-g -DDEBUG" ./configure --disable-release [...]
```
where `[...]` are the other necessary options.

-----------

#### FFmpeg
It is always possible to disable audio/video recording support by specifying the `configure` parameter `--without-ffmpeg`.
If the installed version is lower than 4.0 the support will be disabled automatically.

Supported audio recording formats:
* WAV Audio
* MP3 Audio ([lame](https://xiph.org/vorbis/)) (*)
* AAC Audio
* Flac Audio
* Ogg Audio ([vorbis](https://xiph.org/vorbis/)) (*)

Supported video recording formats:
* MPEG 1 Video
* MPEG 2 Video
* MPEG 4 Video
* MPEG H264 Video ([libx264](https://www.videolan.org/developers/x264.html)) (*)
* High Efficiency Video Codec ([libx265](https://www.videolan.org/developers/x265.html)) (*)
* WebM Video ([libvpx](https://www.webmproject.org/code)) (*)
* Windows Media Video
* AVI FF Video
* AVI Video

(*) if compiled in FFmpeg.
