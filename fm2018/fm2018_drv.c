#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/ioctl.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>

#include "fm2018_ioctl.h"
#include "parameter_data_file.h"

enum fm2018_mode {
	HANDSET, HANDFREE, BYPASS, INIT, TEST_MAIN, TEST_SUB
};

#define FM2018_DRIVER_NAME "fm2018"

static struct fm2018_data {
    struct i2c_client *client;
    wait_queue_head_t wait;
} fm2018_data;


static int fm2018_i2c_read(struct i2c_client *client, char *buf, int len);
static int fm2018_i2c_write(struct i2c_client *client, char *buf, int len);

int fm2018_write_reg(uint16_t reg,  uint16_t data)
{
	uint8_t wBuf[7];
	struct i2c_client *client = fm2018_data.client;
	
	wBuf[0]=0xfc;
	wBuf[1]=0xf3;
	wBuf[2]=0x3b;
	wBuf[3]= (reg >> 8) & 0xff;
    wBuf[4]= reg & 0xff;
    wBuf[5]= (data >> 8) & 0xff;
    wBuf[6]= data & 0xff;

	return fm2018_i2c_write(client, wBuf, 7);
}


int fm2018_read_reg(uint16_t reg, uint16_t *val)
{
	uint8_t wBuf[5];
	uint8_t rBuf[4];
	uint8_t dataH, dataL;
	int ret;

	struct i2c_client *client = fm2018_data.client;

	wBuf[0]=0xfC;
    wBuf[1]=0xf3;
    wBuf[2]=0x37;
    wBuf[3]=(reg >> 8) & 0xff;
    wBuf[4]= reg & 0xff;
	ret = fm2018_i2c_write(client, wBuf, 5);
	if(ret < 0) {
		printk("Error read i2c\n")	;
		return ret;
	}

	rBuf[0]=0xfc;
    rBuf[1]=0xf3;
    rBuf[2]=0x60;
    rBuf[3]=0x26;
	ret = fm2018_i2c_read(client, rBuf, 4);
	if(ret < 0) {
		printk("Error read i2c\n")	;
		return ret;
	}
	dataH = rBuf[0];

	rBuf[0]=0xfc;
    rBuf[1]=0xf3;
    rBuf[2]=0x60;
    rBuf[3]=0x25;
	ret = fm2018_i2c_read(client, rBuf, 4);
	if(ret < 0) {
		printk("Error read i2c\n")	;
		return ret;
	}
    dataL = rBuf[0];

	*val = ((dataH<<8)&0xff00) + (dataL&0xff);
	return 0;
}


long fm2018_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct reg_val rval;
	int ret;
	unsigned int __user *argp = (unsigned int __user *)arg;
	switch (_IOC_NR(cmd)) {
	case _IOC_NR(FM2018CMD_REG):
		if (copy_from_user(&rval, argp, sizeof(rval)))
            return -1;
		if(rval.write) {
			printk("[kernel] to write register");
        	ret = fm2018_write_reg(rval.reg, rval.val);
        	printk("to write val 0x%02x to reg 0x%02x\n", rval.val ,rval.reg);
			return ret;
		}
		else {
			printk("[kernel] to read register");
			ret = fm2018_read_reg(rval.reg, &rval.val);
			if(ret < 0) return ret;
			printk("read val 0x%04x from reg 0x%04x\n",  rval.val, rval.reg);
	
			if (copy_to_user(argp, &rval, sizeof(rval)))
				return -1;
		}
		return 0;
        break;
/*	
    case _IOC_NR(FM2018CMD_WRITE_REG):
		printk("kernel to write register");
       if (copy_from_user(&rval, argp, sizeof(rval))){
			printk("copy error");
            return -1;
		}
        printk("write val 0x%02x to reg 0x%02x\n", rval.val ,rval.reg);
        ret = fm2018_write_reg(rval.reg, rval.val);
		return ret;
        break;
*/
	default:
		break;	
	}
	return 0;
}

int fm2018_open(struct inode * inode, struct file * file)
{
    return 0;
}

int fm2018_close(struct inode * inode, struct file * file)
{
    return 0;
}

static struct file_operations fm2018_fops = {
    .owner                  = THIS_MODULE,
    .unlocked_ioctl         = fm2018_ioctl,
    .open                   = fm2018_open,
    .release                = fm2018_close
};

static struct miscdevice fm2018_dev = {
    .minor      = MISC_DYNAMIC_MINOR,
    //.minor      = 254,
    .name       = "fm2018dev",
    .fops       = &fm2018_fops,
};


static int fm2018_i2c_read(struct i2c_client *client, char *buf, int len)
{
    if(len != i2c_master_send(client, buf, len)){
        pr_err("[FMDSP] i2c_read: i2c_master_send error\n");
        return -1;
    }
	mdelay(100);
    if(1 != i2c_master_recv(client, buf, 1)){
        pr_err("[FMDSP] i2c_read: i2c_master_recv error\n");
        buf[0] = 0xff;
        return -1;
    }
    return 0;
}

static int fm2018_i2c_write(struct i2c_client *client, char *buf, int len)
{
    if(len != i2c_master_send(client, buf, len)){
        pr_err("[FMDSP] i2c_write: i2c_master_send error\n");
        return -1;
    }
	mdelay(100);
    return 0;
}

void Two_Mic_NR_Download(unsigned short mode)
{
	struct i2c_client *client = fm2018_data.client;

	switch (mode){
	case HANDSET:  // 
		fm2018_i2c_write(client, parameter_data_handset_mode, 
			sizeof(parameter_data_handset_mode));
		break;
	case HANDFREE:
		printk("set to handfree mode\n");	
		//fm2018_i2c_write(client, parameter_data_handfree_mode,
		//	sizeof(parameter_data_handfree_mode));
		fm2018_i2c_write(client, parameter_data_one_mic_handfree_mode,
			sizeof(parameter_data_one_mic_handfree_mode));
		break;
	case BYPASS:
		printk("set to bypass mode\n");
		fm2018_i2c_write(client, parameter_data_bypass_mode,
			sizeof(parameter_data_bypass_mode));
		break;
	case INIT:
		fm2018_i2c_write(client, parameter_data_Init,
			sizeof(parameter_data_Init));
		break;
	case TEST_MAIN:
		printk("set to testmain mode\n");
		fm2018_i2c_write(client, parameter_data_TEST_MAIN_MIC,
			sizeof(parameter_data_TEST_MAIN_MIC));
		break;
	case TEST_SUB:
		fm2018_i2c_write(client, parameter_data_TEST_SUB_MIC,
			sizeof(parameter_data_TEST_SUB_MIC));
		break;
	default:
		break;
	}
}

void Two_Mic_NR_chip_Init(void)
{
	// do nothing here
	// GPIO to reset
	Two_Mic_NR_Download(INIT);

}


#define MX6Q_SABRELITE_FM2018_RST  IMX_GPIO_NR(4, 29)  

void FM_chip_Reset(void)
{

	return;
/*
	int ret;
	ret = gpio_request( MX6Q_SABRELITE_FM2018_RST, "fm2018_rst" );
    if (ret) {
        printk( KERN_DEBUG "failed to get GPIO04: %d\n", ret );
        return;
    }

	gpio_direction_output( MX6Q_SABRELITE_FM2018_RST, 1 );
    msleep(2);
    gpio_direction_output( MX6Q_SABRELITE_FM2018_RST, 0 );
    msleep(5);
    gpio_direction_output( MX6Q_SABRELITE_FM2018_RST, 1 );
    msleep(10);

    gpio_free( MX6Q_SABRELITE_FM2018_RST );

	printk("Reset ok\n");
	// do nothing here
*/
}


void Two_Mic_NR_chip_Handfree_mode(void)
{
	FM_chip_Reset();
	Two_Mic_NR_Download(HANDFREE);
}

void Two_Mic_NR_chip_TEST_MAIN_mode(void)
{
	FM_chip_Reset();
	Two_Mic_NR_Download(TEST_MAIN);	
}

void Two_Mic_NR_chip_Bypass_mode(void)
{
	FM_chip_Reset();
	Two_Mic_NR_Download(BYPASS);
}

static int fm2018_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	fm2018_data.client = client;

	printk("in probe fm2018\n");
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("[fm2018] i2c_check_functionality error!\n");
        return -ENOTSUPP;
    }

//	printk("To init chip\n");
//	Two_Mic_NR_chip_Init();

#if 1
	printk("To set handfree\n");
	Two_Mic_NR_chip_Handfree_mode();
#else
//	printk("To set testmain\n");
//	Two_Mic_NR_chip_TEST_MAIN_mode();
	printk("To set bypass\n");
	Two_Mic_NR_chip_Bypass_mode();
#endif
	strlcpy(client->name, FM2018_DRIVER_NAME, I2C_NAME_SIZE);
	i2c_set_clientdata(client, &fm2018_data);	

	misc_register(&fm2018_dev);

	return 0;
}

static int fm2018_remove(struct i2c_client *client)
{	
	FM_chip_Reset();		
	misc_deregister(&fm2018_dev);
	printk("fm2018 driver removed\n");
	return 0;
}


static const struct i2c_device_id fm2018_id[] = {
    { FM2018_DRIVER_NAME, 0 },
    { }
};


static struct i2c_driver fm2018_driver = {
    .probe      = fm2018_probe,
    .remove     = fm2018_remove,
	.id_table   = fm2018_id,
    .driver     = {
    .name = FM2018_DRIVER_NAME,
    },
};

static int __init fm2018_init(void)
{
	int ret = i2c_add_driver(&fm2018_driver);
	if (ret){
		printk(KERN_ERR "[fm2018] add driver error\n");
	}
	printk("add fm2018 driver ok\n");
	return ret;
}

static void __exit fm2018_exit(void)
{
	i2c_del_driver(&fm2018_driver);
}

module_init(fm2018_init);
module_exit(fm2018_exit);

MODULE_AUTHOR("Zicheng Zhao");
MODULE_DESCRIPTION("fm2018 video driver");
MODULE_LICENSE("GPL");

