#include <stdio.h>
#include "tw2828_ioctl.h"

static int tw2828_check_info_val(itype type, float val)
{

	switch(type) {
	case GPS_X:
	case GPS_Y:
		if (val > 180.0 && val < 0.0) {
			fprintf(stderr, "Error value for GPS\n");
			return -1;
		}
		break;
	case VELOCITY:
		if (val > 500.0) {
			fprintf(stderr, "Error value for velocity\n");
			return -1;
		}
		break;
	case MILECITY:
		if (val > 9999.0) {
			fprintf(stderr, "Error value for milecity\n");
			return -1;
		}
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

int tw2828_update_info(const int fd, struct info_value *ival) 
{
	int i;
	int type = ival->type;
	float val = ival->fval;

	if (tw2828_check_info_val(ival->type, ival->fval)) {
		return -1;
	}

	for (i = 0x0; i < NUM_DIGITS - NUM_INT; ++i) {
		val *= 10;
		ival->array[NUM_INT + i] = ((int)val) % 10;
	}
	
	val = ival->fval;
	for (i = 0; i < NUM_INT; ++i) {
		ival->array[NUM_INT - i - 1] = ((int)val) % 10;
		val /=10;
	}

	if (ioctl(fd, TW2828CMD_UPDATE_VALUE, ival)) {
		return -1;
	}
	return 0;
}

#if 1
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

int main(int argc, char **argv)
{
	int fd;
	float gps_x = 120.81;
	float gps_y =  56.91;
	float velocity = 80.8;
	float milicity = 1999.0;
	struct info_value ival;
	
	fd = open(DEVICE, O_RDWR);
	if (fd < 0 ) {
		fprintf(stderr, "Cannot open %s\n", DEVICE);
		return -1;
	}

	srand(0);
	while (1) {
		float tmp = (rand() / RAND_MAX - 0.5) * 50;
		milicity += 10.5;
		velocity = 50 + tmp;
		gps_x += tmp;
		gps_y += tmp;

		gps_x = fabs(gps_x);
		gps_y = fabs(gps_y);
		velocity = fabs(velocity);

		ival.type = GPS_X;
		ival.fval = gps_x;
		//printf("Update GPS_X %f\n", ival.fval);
		tw2828_update_info(fd, &ival);

		ival.type = GPS_Y;
		ival.fval = gps_y;
		//printf("Update GPS_Y %f\n", ival.fval);
		tw2828_update_info(fd, &ival);

		ival.type = VELOCITY;
		ival.fval = velocity;
		//printf("Update VELOCITY %f\n", ival.fval);
		tw2828_update_info(fd, &ival);

		ival.type = MILECITY;
		ival.fval = milicity;
		//printf("Update MILECITY %f\n", ival.fval);
		tw2828_update_info(fd, &ival);
		sleep(2);
	}	
	close(fd);
	return 0;
}

#endif
