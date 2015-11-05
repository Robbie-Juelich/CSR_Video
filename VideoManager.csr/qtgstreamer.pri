
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += QtGStreamer-0.10 QtGStreamerUi-0.10
    #PKGCONFIG += Qt5GStreamer-1.0 Qt5GStreamerUi-1.0
}

win32 {
    INCLUDEPATH += D:\boost_1_58_0 \
        D:\QtGstreamer\1.2.0\include \
        D:\gstreamer\1.0\x86\include\gstreamer-1.0 \
        D:\gstreamer\1.0\x86\include\glib-2.0 \
        D:\gstreamer\1.0\x86\lib\glib-2.0\include \
        D:\gstreamer\1.0\x86\include\libxml2

    CONFIG(debug,debug|release){
        LIBS += -LD:\QtGstreamer\1.2.0\lib\debug
    } else {
        LIBS += -LD:\QtGstreamer\1.2.0\lib\release
    }

    LIBS += -lQt5GStreamerUi-1.0 -lQt5GStreamer-1.0 -lQt5GLib-2.0
}

