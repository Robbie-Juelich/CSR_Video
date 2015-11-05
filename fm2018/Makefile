
#ifeq ($(PARAM_FILE), )
#		PARAM_FILE:=../../Makefile.param
#		include $(PARAM_FILE)
#endif

obj-m := fm2018.o
fm2018-objs += fm2018_drv.o  #two_mic_NR_custom_if.o

#tw2968-objs += tw2968_drv.o tw2968_i2c.o

#EXTRA_CFLAGS += -D$(HI_FPGA) 
#EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c -I$(PWD)/../gpio-i2c1
#EXTRA_CFLAGS += -I$(PWD)/../gpio_i2c_8b
PWD= `pwd`
LINUX_ROOT = /opt/freescale/kernel/linux-3.0.35

#/home/tuituji/work/linux-3.0.35

default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules
clean: 
	@make -C $(LINUX_ROOT) M=$(PWD) clean



