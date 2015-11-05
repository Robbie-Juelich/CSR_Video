#include <stdio.h>
#include "haikang.h"

int main(int argc, char **argv)
{
    int ret;
    int i;
    unsigned char mil[8] = {0, 2, 3, 5, '.', 1, 4, 6};
    unsigned char speed[3] = {0, 9, 0};
    unsigned char gpsx[11] = {1, 1 , 0, 0xa7, 4, 9, '\'', 6, 4, '"', 'N'};
    unsigned char gpsy[11] = {0, 4 , 9, 0xa7, 2, 5, '\'', 7, 8, '"', 'E'};

	ret = init_device();
	if (ret < 0) return -1;
	ret = enable_device();
	if (ret < 0) return -1;

    for (i = 0; i < 100; ++i) {
        gpsx[2] = i %10;
        gpsy[4] = i %10;
        ret = send_gpsxy(gpsx, gpsy);
		if (ret < 0) return -1;

        mil[3] = i % 10;
        speed[2] = i % 10;
        ret = send_mil_speed(mil, speed);
		if (ret < 0) return -1;
        sleep(1);
    }

    sleep(5);
    disable_device();

    close_device();
    return 0;
}


