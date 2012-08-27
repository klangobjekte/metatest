#-------------------------------------------------
#
# Project created by QtCreator 2011-10-01T08:59:12
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = metatest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp



macx {
    INCLUDEPATH += . \
	/usr/local/include
	#/usr/local/include/AAF_1.1.2

    # /Developer/SDKs/AAF-src-1.1.3/AAFPPCDarwinSDK/g++/include \
    QMAKE_LFLAGS += -F/System/Library/Frameworks \
	-F//Developer/Library/PrivateFrameworks
    QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
    CONFIG += x86
    CONFIG += ppc
    LIBS += -L/usr/local/lib \
	-lsqlite3.0 \
	-lmeta

    #LIBS += -L/usr/local/lib \
    #        -lsndfile
    #LIBS += -L/usr/local/lib \
    #        -laafiid \
    #        -laaflib
    LIBS += -framework \
	Carbon \
	-framework \
	CoreAudio \
	-framework \
	CoreServices
}

win32 {
    # TARGET = libqwave2
    #sources.path = "C:\Qt_Projekte\ProSoundFinder2-build-desktop\release"
    INCLUDEPATH += \
        "C:\Program Files\Mega-Nerd\libsndfile\include" \
        "C:\Program Files\Microsoft DirectX SDK (June 2008)\Include" \
        "C:/Qt_Projekte/meta_mp"



    LIBS += "C:\Program Files\Microsoft DirectX SDK (June 2008)\Lib\x86\dsound.lib" \
        "C:\Program Files\Mega-Nerd\libsndfile\lib\libsndfile-1.lib" \
        #"C:/Qt_Projekte/meta_mp/debug/libmeta.a"
        #"C:\Qt_Projekte\meta-build-desktop-Qt_aus_PATH_Debug\debug\libmeta.a"
C:\Qt_Projekte\meta-build-desktop-Qt_4_8_1_for_Desktop_-_MSVC2008__Qt_SDK__Debug\debug\libmeta.a

        #DEFINES \
        #+= \
        #QWAVE_MAKE_DLL

    # If you compile with QtCreator/gcc:
    #win32-g++:LIBS += /path/to/your/libMyLib.a

    #IF you compile with MSVC: */
    #win32-msvc:LIBS += /path/to/your/libMyLib.lib
}
unix:linux-g++ {
    INCLUDEPATH += /ldc/include
    LIBS += -L/ldc/lib
}
