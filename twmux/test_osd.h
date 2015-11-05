
#ifndef __OSD_H__
#define __OSD_H__


#ifdef __cplusplus
extern "C" {
#endif

#define FONT_8x10		0
#define FONT_22x22	1

int OsdTest(int argc, char **argv);

void InitOSD(u8 no, int i);
void InitRecOSD(u8 no, int i);


#ifdef __cplusplus
}
#endif


#endif


