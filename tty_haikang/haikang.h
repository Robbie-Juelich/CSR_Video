#ifndef _HAI_KANG_H
#define _HAI_KANG_H

#define UART_DEVICE "/dev/ttyUSB0"

int init_device();

int enable_device();

int send_mil_speed(unsigned char *mil, unsigned char *speed);

int send_gpsxy(unsigned char *x, unsigned char *y);

int disable_device();

int close_device();

//int enable_gpsxy();
//int enable_mil_speed();
//int disable_gpsxy();
//int disable_mil_speed();

#endif

