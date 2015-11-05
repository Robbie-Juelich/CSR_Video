#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sysfs.h>

#include "tw2828_config.h"
#include "tw2828_test.h"

#include "tw2828_hostio.h"
#include "tw2828_twmux.h"
#include "test_evb_tbl.h"
#include "tw2828_ioctl.h"
#include "tw2828_reg.h"
#include "tw2828_logo.h"


static struct rec_channel cached_rec_ch={0,3}; // {cam_id, rec}
struct windows_cam_mode win_mode_cache;
//extern unsigned char windows_cam_id[8];

extern int  TW2828Init(int no, _twmux usrmux);
extern void ReadMemory1(u32 addr, u8 len8, u8 *buf);
extern void MnMouseDisable(u32 no);
extern void TWMux_SetRec34_Camera(u32 rec, u32 cam_id);	

extern int  tw2828_init_v4l2(struct device *dev);
extern void tw2828_remove_v4l2(void);
extern void update_logo_info(MDPS_t mode);
extern void change_logo(MDPS_t mode);
extern void DrawLogo(u8 id);

extern void do_test_pattern(void);
extern void undo_test_pattern(void);
extern void do_rec_test_pattern(void);
extern void undo_rec_test_pattern(void);
extern void enable_channel_id_2bit(void);
extern void enable_channel_id_1bit(void);
extern void SetVIChno(u8 winno, u32 ch);

extern _RecAllInfo rTb_Rec_4xD1_27M_8b_IFrame_rport5_8; 

int  tw2828_open(struct inode * inode, struct file * file);
int  tw2828_close(struct inode * inode, struct file * file);
long tw2828_ioctl(struct file *file, unsigned int cmd, unsigned long arg);


static struct file_operations tw2828_fops = {
    .owner                  = THIS_MODULE,
    .unlocked_ioctl         = tw2828_ioctl,
    .open                   = tw2828_open,
    .release                = tw2828_close
};

static struct miscdevice tw2828_dev = {
    .minor      = MISC_DYNAMIC_MINOR,
    .name       = "tw2828dev",
    .fops       = &tw2828_fops,
};

int tw2828_open(struct inode * inode, struct file * file)
{
    return 0;
}

int tw2828_close(struct inode * inode, struct file * file)
{
    return 0;
}


void init_display(MDPS_t mode);

MDPS_t modeid_from_modetype(int mode) {
	switch(mode) {
	case TW2828_4CH:
		return MODE_4CH;
	case TW2828_6CH:
		return MODE_6CH;
	case TW2828_1CH:
	case TW2828_8CH:
		return MODE_9CH;	
	}
	return MODE_9CH;
}

long tw2828_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	u32 pitch;
	struct reg_val rval;
	struct mem_val mval;
	struct mem_valxy mvalxy;
	struct channel_onoff channel;
	struct rec_channel rec_ch;
	struct info_chars ichars;
	char *buf;
	unsigned int __user *argp = (unsigned int __user *)arg;
	struct windows_cam_mode win_mode;
	int i = 0;
	
	switch (_IOC_NR(cmd)) {
	case _IOC_NR(TW2828CMD_READ_REG): 
		if (copy_from_user(&rval, argp, sizeof(rval))) 
			return -1;
		rval.val = ReadP(rval.reg);
		printk("read val 0x%04x from reg 0x%02x\n",  rval.val, rval.reg);
		if (copy_to_user(argp, &rval, sizeof(rval)))
			return -1;
		break;	
	case _IOC_NR(TW2828CMD_WRITE_REG):
		if (copy_from_user(&rval, argp, sizeof(rval))) 
			return -1;
		printk("write val 0x%02x to reg 0x%02x\n", rval.val ,rval.reg);
		WriteP(rval.reg, rval.val);
		break;
	case _IOC_NR(TW2828CMD_CHANNEL_ONOF):
		if (copy_from_user(&channel, argp, sizeof(rval))) 
			return -1;
		printk("to turn channel %d %s\n", channel.id, channel.on? "on":"off");
		if (channel.on) TWMux_ChannelWindowOn(0x0, channel.id);
		else TWMux_ChannelWindowOff(0x0, channel.id);
		break;
	case _IOC_NR(TW2828CMD_READ_MEM):
		if (copy_from_user(&mval, argp, sizeof(mval)))
			return -1;
		buf = kmalloc(sizeof(mval.size), GFP_KERNEL);
		ReadMemory1(mval.start, mval.size >> 3, buf);
		if (copy_to_user(mval.buf, buf, mval.size))
			return -1;
	case _IOC_NR(TW2828CMD_READ_MEMXY):
		if (copy_from_user(&mvalxy, argp, sizeof(mvalxy)))
			return -1;
		pitch = ReadP(DISPMEMPITCH_H) & 0x80; pitch <<= 0x1;
		pitch |= ReadP(DISPMEMPITCH);
		buf = kmalloc(mvalxy.w * mvalxy.h * pitch * 4, GFP_KERNEL);
		//ReadMemory1(mvalxy.x, mvalxy.y, mvalxy.w, mvalxy.h, buf);
		// to be continued	
		break;
	case _IOC_NR(TW2828CMD_REC_SELECT_CHANNEL):
		if (copy_from_user(&rec_ch, argp, sizeof(rec_ch)))
			return -1;
		if ((rec_ch.rec != 3) && (rec_ch.rec != 4)) {
			return -1;
		}
		if (rec_ch.cam_id >= 8) {
			return -1;
		}
		/* buffer the new settings */
		cached_rec_ch.rec = rec_ch.rec;
		cached_rec_ch.cam_id = rec_ch.cam_id;
		TWMux_SetRec34_Camera(rec_ch.rec, rec_ch.cam_id);
		break;
	case _IOC_NR(TW2828CMD_UPDATE_INFOCHARS):
		if (copy_from_user(&ichars, argp, sizeof(ichars))) {
			return -1;
		}
		printk("type %d\n" , ichars.type);
		//printk("len %d\n", strlen(ichars.array));
		printk("len %d\n", ichars.len);
		TWMux_Update_Chars(ichars.type, ichars.array, ichars.len);
		break;
	case _IOC_NR(TW2828CMD_MODE_CHNUM):
		if (copy_from_user(&win_mode, argp, sizeof(win_mode)))
			return -1;	
		printk("mode num %d is", win_mode.mode_type);
		if(win_mode.mode_type == win_mode_cache.mode_type) {
			for ( i = 0; i < 8; ++i) {
				if(win_mode_cache.cam_id[i] != win_mode.cam_id[i]) {
					break;
				}
				return 0;
			}
		}
#if 0
		if (win_mode.mode_type == TW2828_4CH){
			return -1;
		}
		else
#endif
		{
			win_mode_cache = win_mode;
			init_display(modeid_from_modetype(win_mode.mode_type));
		}
		for ( i = 0; i < 8; ++i) {
			int j = CH1 +  (win_mode.cam_id[i]/2) * 4  + win_mode.cam_id[i]%2;
			printk("[%d %d %d] ", i, win_mode.cam_id[i], j );
			SetVIChno(i, j);
		}
		break;
	default:
		printk("Error ioctl for tw2828\n");
		break;
	}
	return 0;
}

void init_windows_mode(void)
{
	unsigned char i = 0;

	win_mode_cache.mode_type = TW2828_8CH;
	for (i =0; i < 8; ++i) {
		win_mode_cache.cam_id[i] = i;
	}
}


void init_display(MDPS_t mode)
{
	int i;
	SetHostBus(TW2828_0);
	GetIOInfoData(HostBusNo);
	for (i = 0; i < 8; ++i) {
		printk("%d ", win_mode_cache.cam_id[i]);
	}

	if(win_mode_cache.mode_type == TW2828_1CH){
		TWMux_MakeFullScreen(0x0, win_mode_cache.cam_id[0]);
		mdelay(4000);
		change_logo(TW2828_1CH);
		DrawLogo(TW2828_0);
	}
	else {
		MdspAction(HostBusNo, mode);
	}
	update_logo_info((win_mode_cache.mode_type));
	WriteP(0xc4f,ReadP(0xc4f)|0x03);
	//MdspAction(HostBusNo, MODE_20CH);
	// MODE_9CH  MODE_32CH   MODE_20CH
	TWMux_BT1120_OnOff(HostBusNo, 0x1);  // turn on the BT.1120 output
}


void init_record(void)
{
	WriteP2(0x24c, 0xff);
	WriteP2(0x24d, 0xff);
	TWMux_LoadRecordDataAll(HostBusNo, rTb_Rec_4xD1_27M_8b_IFrame_rport5_8);
    /* To select the channel */
//	TWMux_SetRec34_Camera(3, 1);
//	do_rec_test_pattern();
//	undo_rec_test_pattern();
}


static ssize_t show_camera_id(struct device * dev, 
							  struct device_attribute * attr, char * buf) 
{
	return snprintf(buf, PAGE_SIZE, "%d\n", cached_rec_ch.cam_id);
}

static ssize_t store_camera_id(struct device * dev, 
				 			  struct device_attribute * attr, const char * buf, size_t count)
{
	unsigned long data;
	ssize_t len;
	char * endp;

	data = simple_strtoul(buf, &endp, 10);
	if(data < 0 || data >= 8) 
		return -EINVAL;

	len = endp - buf;
	cached_rec_ch.cam_id = data;

	//printk( KERN_DEBUG "New cam_id=%d\t len=%d\n", cached_rec_ch.cam_id, len);
	printk( KERN_ERR "New cam_id=%d\t len=%d\n", cached_rec_ch.cam_id, len);

	TWMux_SetRec34_Camera(cached_rec_ch.rec, cached_rec_ch.cam_id);
	
	return count;
}
// create   /sys/devices/platform/tw2828/camera_id
static DEVICE_ATTR(camera_id, S_IWUSR | S_IRUGO, show_camera_id, store_camera_id);


static void board_reset(void);


extern void tw2968_module_exit(void);
extern int tw2968_module_init(void);


static int tw2828_probe(struct platform_device *pdev)
{
	int ret = -1;
	int retry = 0x0;

	host_io_init();	
	int val = ReadP(0x1000);
	printk("fpga id0 %x\n", val);
	val = ReadP(0x1001);
	printk("fpga id1 %x\n", val);
	val = ReadP(0x1002);
	printk("fpga id3 %x\n", val);
	WriteP(0x1002, 0xff);
	val = ReadP(0x1002);
	printk("fpga id3 %x\n", val);

	// we have only one 2828 now
	for(;;) {
		//ret = TW2828Init(TW2828_0, Default_PAL_720_1024x768);
		ret = TW2828Init(TW2828_0, Default_PAL_720_1080i);
		//ret = TW2828Init(TW2828_0, Default_PAL_720_1080P);
		if (ret) {
			printk("init tw2828_0 error\n");
		}
//	ret = TW2828Init(TW2828_1, Default_PAL_720_1080i);
//	if (ret) printk("init tw2828_1 error\n");

		ret = DCM_EyeCalibration();
		++retry;

		if(ret == 0 || retry == 0x10) {
			break;	
		}
		board_reset();
		msleep(500);
		printk("DRAM check failed, Try another time\n");
	}
	printk("DRAM check ok\n");

	tw2968_module_init();

	init_windows_mode();
	init_logo_info(modeid_from_modetype(win_mode_cache.mode_type));
	init_display(modeid_from_modetype(win_mode_cache.mode_type));
#if defined(LOGO_LATE)
	DrawLogo(TW2828_0);
#endif
	init_record();

	ret = misc_register(&tw2828_dev);
	if (ret) {
		printk(KERN_ERR "misc_register error\n");
		return ret;
	}
	ret = tw2828_init_v4l2(&(pdev->dev)); // init v4l2 device
	if (ret) {
		printk(KERN_ERR "tw2828_init_v4l2 error\n");
		return ret;
	}

	/**
	 * register /sys/devices/xxx entries
	 */
	ret = device_create_file(&(pdev->dev), &dev_attr_camera_id);
	if (ret) {
		printk(KERN_ERR "create /sys entry error\n");
		return ret;
	}

	//TWMux_SetVIChno(0x0, 0x7, CH9);
	//TWMux_Update_Value(GPS_X, array);

	WriteP(0x348, 0xff);  // fot anti-aliasing
	return 0;
}


#define MX6Q_SABRELITE_CSR_RST  IMX_GPIO_NR(1, 4)  //GPIO04
#include <linux/gpio.h>


static void board_reset()
{
	int ret;

#if 0
	WriteP(0x217, 0xff);
	WriteP(0x20b, 0xff);
 	WriteP(0x20c, 0xff);
 	WriteP(0x20d, 0xff);
 	WriteP(0x20e, 0xff);
 	WriteP(0x20f, 0xff);
	//WriteP(0x217, 0x0);
 	delay(10);  //
#endif

	ret = gpio_request( MX6Q_SABRELITE_CSR_RST, "csr_rst" );
    if ( ret ) {
        printk( KERN_DEBUG "failed to get GPIO04: %d\n", ret );
        return;
    }

    /* output reset signal */
    gpio_direction_output( MX6Q_SABRELITE_CSR_RST, 1 );
    msleep(2);
    gpio_direction_output( MX6Q_SABRELITE_CSR_RST, 0 );
    msleep(5);
    gpio_direction_output( MX6Q_SABRELITE_CSR_RST, 1 );
	msleep(10);

    gpio_free( MX6Q_SABRELITE_CSR_RST );
}

static int tw2828_remove(struct platform_device *pdev)
{
	host_io_uninit();
	misc_deregister(&tw2828_dev);
	tw2828_remove_v4l2();
	device_remove_file(&(pdev->dev), &dev_attr_camera_id);
	free_logo_info();
	
	tw2968_module_exit();

	board_reset();
	//msleep(1000);
	return 0;
}

static struct platform_driver tw2828_driver = {
	.driver = {
        .name = "tw2828",
        .owner = THIS_MODULE,
    },
    .probe = tw2828_probe,
    .remove = tw2828_remove,
};

static int __init tw2828_init(void)
{
    return platform_driver_register(&tw2828_driver);
}

static void __exit tw2828_uninit(void)
{
    return platform_driver_unregister(&tw2828_driver);
}

module_init(tw2828_init);
module_exit(tw2828_uninit);

MODULE_AUTHOR("J. Jiang");
MODULE_DESCRIPTION("TW2828 video mux driver");
MODULE_LICENSE("GPL");

