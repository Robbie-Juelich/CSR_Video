#ifndef SOCKETPORTS_H
#define SOCKETPORTS_H

#define REMOTE_PORT   11000   // port for communicating with shangweiji machine
#define REGISTER_PORT  11001
#define HEART_BEAT_PORT  50005  // port for receving hear beat msg and also for sending audio
#define AUDIO_OUT_PORT  50003  // ununsed ??
#define VIDEO_OUT_PORT  50004  // port for sending video rtcp
#define PORT_BASE 5004     // port for audio and video

//#define CAPS_PORT 50002
#define VCAPS_PORT 51002
#define ACAPS_PORT 51003

#endif // SOCKETPORTS_H
