#ifndef CONFIG_H
#define CONFIG_H

#include<qglobal.h>

#define GST_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#define GST_VERSION   GST_VERSION_CHECK(1, 8, 2)
//#define GST_VERSION   GST_VERSION_CHECK(1, 2, 4)
//#define GST_VERSION   GST_VERSION_CHECK(0, 1, 0)

#define HAS_QTVIDEOSINK 1

#if  (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#define HEART_BEAT_USE_Q3
#endif


#endif // CONFIG_H
