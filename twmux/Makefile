
obj-m += twmux.o
twmux-objs :=  tw2828_drv.o tw2828_config.o tw2828_hostio.o tw2828_init.o \
				tw2828_disp.o tw2828_memory.o tw2828_lcd.o tw2828_pb.o \
				tw2828_rec.o tw2828_spot.o tw2828_osd.o tw2828_mse.o \
				tw2828_freeze.o  tw2828_system_init.o  tw2828_testpattern.o \
				tw2828_v4l2.o tw2828_fpga.o tw2828_logo.o \
				test_memory.o test_disp.o test_spot.o \
				test_rec.o test_osd.o \
				tw2968_init.o 

PWD=`pwd`

EXTRA_CFLAGS += -I$(PWD)/include/ -DVIDEO_2D1 #-DLOGO_LATE
KERNEL_DIR = /opt/freescale/kernel/linux-3.0.35
#KERNEL_DIR = /home/jie/GmbH/MX6Q/dev/elevator/linux-3.0.35

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules 
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
