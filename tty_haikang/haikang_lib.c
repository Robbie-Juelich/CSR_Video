#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#include "haikang.h"

#define BAUDRATE	B19200
#define DATABITS	0x8
#define STOPBITS	0x1
#define POLARITY   's'  // N or s both ok

#define CMD_MIL_SPEED 		0x1
#define CMD_GPSXY     		0x2

static int fd;

static struct termios oldtio;

static unsigned char cmd[32];

static int set_speed(int speed)
{
	int		status;
	struct	termios opt;

	tcgetattr(fd, &opt);
	tcflush(fd, TCIOFLUSH);
	cfsetispeed(&opt, speed);
	cfsetospeed(&opt, speed);
	status = tcsetattr(fd, TCSANOW, &opt);
	if (status != 0) {
		return -1;
	}
	tcflush(fd,TCIOFLUSH);
	return 0;
}

static int set_parity(int databits,int stopbits,int parity)
{
	struct termios options;
	if (tcgetattr(fd,&options) !=  0) {
		return -1;
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) {
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr, "[Serial Error]   Unsopported databits\n");
		return -1;
	}

	switch(parity) {
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"[Serial Error]   Unsupported parity\n");
		return -1;
	}

	switch (stopbits) {
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr,"[Serial Error] Unsupported stopbits\n");
		return -1;
	}
	if (parity != 'n') {
		options.c_iflag |= INPCK;
	}

	tcflush(fd,TCIFLUSH);
	options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/	
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
	if (tcsetattr(fd, TCSANOW, &options) != 0) {
        return -1;
	}
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag &= ~OPOST;   /*Output*/

	return 0;
}


int init_device()
{
	int ret;
	fd = open (UART_DEVICE, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "[Serial Error] Open device %s failed\n", UART_DEVICE);
		return -1;
	}

	tcflush(fd, TCIFLUSH);
	tcgetattr(fd, &oldtio); 	

	ret = set_speed(BAUDRATE);
	if (ret < 0) {
		fprintf(stderr, "[Serial Error] Set speed fail\n");
		return -1;
	}

	ret = set_parity(DATABITS, STOPBITS, POLARITY);
	if (ret < 0) {
		fprintf(stderr, "[Serial Error] Set parity fail\n");
		return -1;
	}
	
	return 0;
}

inline unsigned char DIGIT2ASCII(unsigned char x) 
{
	if (x <= 9 && x >=0) return x + '0';
	return x;
}

int entry_on_off(unsigned char idx, int on)
{
	int i = 0;
	int sum = 0x0;
	
	cmd[0] = 0x5;	
	cmd[1] = idx + 2;
	if (on) {
		cmd[2] = 0xAA;
	}
	else {
		cmd[2] = 0x00;
	}
	sum = cmd[1] + cmd[2];
	cmd[3] = sum & 0xff;
	cmd[4] = 0x17;

	sum = write(fd, cmd, 5);
	tcflush(fd, TCIFLUSH);
}

#define ON	0x1
#define OFF	0x0

int enable_device()
{
	entry_on_off(CMD_GPSXY, ON);
	entry_on_off(CMD_MIL_SPEED, ON);
	return 0;
}

int disable_device()
{
	entry_on_off(CMD_GPSXY, OFF);
	entry_on_off(CMD_MIL_SPEED, OFF);
	return 0;
}

int send_mil_speed(unsigned char* mil, unsigned char* speed)
{	
	int i = 0x0;
	int sum = 0x0;

	if (mil[4] != '.') {
		fprintf(stderr, "[Serial Error] Milcity value error\n");
		return -1;
	}
	cmd[0] = 0x05;
	cmd[1] = CMD_MIL_SPEED;
	for (i = 0; i < 8; ++ i) {
		cmd[2 + i] = DIGIT2ASCII(mil[i]);
	}
	cmd[10] = 'K';
	cmd[11] = 'm';
	cmd[12] = ' ';
	
	for (i = 0; i < 3; ++i)	{
		cmd[13 + i] = DIGIT2ASCII(speed[i]);
	}
	cmd[16] = 'K';
	cmd[17] = 'm';
	cmd[18] = '/';
	cmd[19] = 'h';
	
	for (i = 1; i <= 19; ++i) {
		sum += cmd[i];
	}
	cmd[20] = sum & 0xff;
	cmd[21] = 0x17;

	sum = write(fd, cmd, 22);
	tcflush(fd, TCIFLUSH);
}

int send_gpsxy(unsigned char *x, unsigned char *y)
{
	int i = 0x0;
	int sum = 0x0;

	if (!(x[3] == 0xA7 && x[6] == '\'' && x[9] == '"')) {
		fprintf(stderr, "[Serial Error] GPS x value error\n");
		return -1;
	}
	if (!(y[3] == 0xA7 && y[6] == '\'' && y[9] == '"')) {
		fprintf(stderr, "[Serial Error] GPS y value error\n");
		return -1;
	}

	cmd[0] = 0x05;
	cmd[1] = CMD_GPSXY;
	for (i = 0; i < 11; ++ i) {
		cmd[2 + i] = DIGIT2ASCII(x[i]);
	}
	cmd[13] = ' ';
	
	for (i = 0; i < 11; ++i)	{
		cmd[14 + i] = DIGIT2ASCII(y[i]);
	}
	
	for (i = 1; i <= 24; ++i) {
		sum += cmd[i];
	}
	cmd[25] = sum & 0xff;
	cmd[26] = 0x17;

	sum = write(fd, cmd, 27);
	tcflush(fd, TCIFLUSH);
}


int close_device()
{
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);
}

