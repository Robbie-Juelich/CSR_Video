#ifndef SOCKETPORTS_H
#define SOCKETPORTS_H

/*
 * Port for testing
*/
#define REMOTE_PORT   11000

/*
 * On server
 * Computer on the train send message to this port to register and to communicate
*/
#define REGISTER_PORT  11001

/*
 * On server
 * Port for receving hear beat msg
 * IPCAM will send hearbeat msg to both HEART_BEAT_PORT and BIND_OUT_PORT
 * while BIND_OUT_PORT is also used for sending video rtcp and also for sending audio
*/
#define HEART_BEAT_PORT  50005  //
#define BIND_OUT_PORT  50004  // port for sending video rtcp and also for sending audio

/*
 * Not used now
*/
#define AUDIO_OUT_PORT  50003


/*
 * On server
 * used for receiving video/audio
*/
#define PORT_BASE 5004     // port for audio and video

/*
 * On server
 * for receiving caps
*/
#define VCAPS_PORT 51002
#define ACAPS_PORT 51003

#endif // SOCKETPORTS_H
