#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/videodev2.h>
#include <media/v4l2-int-device.h>
#include <media/v4l2-chip-ident.h>
#include "mxc_v4l2_capture.h"  

#define TW2828_MODE_MAX 2

struct sensor {
    struct v4l2_int_device *v4l2_int_device;
	struct sensor_data sen;
    v4l2_std_id std_id;
	struct device *dev;
} tw2828_v4l2_data;

typedef struct {
    int v4l2_id;        /*!< Video for linux ID. */
    char name[16];      /*!< Name (e.g., "NTSC", "PAL", etc.) */
    u16 raw_width;      /*!< Raw width. */
    u16 raw_height;     /*!< Raw height. */
    u16 active_width;   /*!< Active width. */
    u16 active_height;  /*!< Active height. */
} video_fmt_t;

enum {
	TW2828_FMT_PRIV0 =  0x0,
	TW2828_FMT_PRIV1
};

static v4l2_std_id video_idx = TW2828_FMT_PRIV0;

#define TW2828_FMT  V4L2_PIX_FMT_UYVY

static video_fmt_t video_fmts[] = {
    {
        /*! (B, G, H, I, N) PAL */
        .v4l2_id = V4L2_STD_PAL,
        .name = "PAL",
        .raw_width = 720,
        .raw_height = 625,
        .active_width = 720,
        .active_height = 576,
    },
    {
        .v4l2_id = V4L2_STD_PRIV_1080_HD,
        .name = "BT1120",
        .raw_width = 2220,
        .raw_height = 1125,
        .active_width = 1920,
        .active_height = 1080,
    }
};

static struct v4l2_queryctrl tw2828_v4l2_qctrl[] = {
    {
        .id = V4L2_CID_BRIGHTNESS,
        .type = V4L2_CTRL_TYPE_INTEGER,
        .name = "Brightness",
        .minimum = 0,
        .maximum = 255,
        .step = 1,
        .default_value = 128,
        .flags = 0,
    },
};

#define V4L2_IDENT_TW2828  (V4L2_IDENT_UPD64083 + 1)

static int ioctl_g_chip_ident(struct v4l2_int_device *s, int *id)
{
    ((struct v4l2_dbg_chip_ident *)id)->match.type = V4L2_CHIP_MATCH_HOST;
    strcpy(((struct v4l2_dbg_chip_ident *)id)->match.name, "tw2828_v4l2");
    ((struct v4l2_dbg_chip_ident *)id)->ident = V4L2_IDENT_TW2828;
    return 0;
}

static int ioctl_enum_framesizes(struct v4l2_int_device *s,
                 struct v4l2_frmsizeenum *fsize)
{
    if ( fsize->index < 0 || fsize->index >= ARRAY_SIZE(video_fmts) )
        return -EINVAL;
    fsize->discrete.width = video_fmts[fsize->index].active_width;
    fsize->discrete.height  = video_fmts[fsize->index].active_height;
    return 0;
}

static int ioctl_enum_fmt_cap(struct v4l2_int_device *s,
                  struct v4l2_fmtdesc *fmt)
{
    if (fmt->index >= TW2828_MODE_MAX)
        return -EINVAL;
    // ???
    fmt->pixelformat = tw2828_v4l2_data.sen.pix.pixelformat;

    return 0;
}

static void ioctl_g_ifparm_bt1120 (struct v4l2_int_device *s,  struct v4l2_ifparm *p )
{
    if (s == NULL) {
        printk("%s:ERROR!! no slave device set!\n", __func__);
        return;
    }
    memset(p, 0, sizeof(*p));


	p->if_type = V4L2_IF_TYPE_BT1120; //??
    //p->u.bt1120.mode = V4L2_IF_TYPE_BT1120_MODE_BT_16BIT;
    p->u.bt1120.mode = V4L2_IF_TYPE_BT1120_MODE_BT_8BIT;
		// V4L2_IF_TYPE_BT656_MODE_NOBT_16BIT,;

    p->u.bt1120.nobt_hs_inv = 0; // Do NOT invert Hsync 
    //p->u.bt1120.nobt_hs_inv = 1; // invert Hsync

    //p->u.bt1120.nobt_vs_inv = 0; // Do NOT invert Vsync
    p->u.bt1120.nobt_vs_inv = 1; // invert Vsync

    //p->u.bt1120.bt_sync_correct = 0; // use internal Vsync
    p->u.bt1120.bt_sync_correct = 1; // use external Vsync

    p->u.bt1120.clock_curr = 0; //INTERLACED_SDR
    //p->u.bt1120.clock_curr = 1; //INTERLACED_DDR
    //p->u.bt1120.clock_curr = 2; // PROGRESSIVE_SDR
    //p->u.bt1120.clock_curr = 3; // PROGRESSIVE_DDR

    p->u.bt1120.latch_clk_inv = 0; // Do NOT invert PIXEL clock
    //p->u.bt1120.latch_clk_inv = 1; //  invert PIXEL clock
}

static void ioctl_g_ifparm_bt656 (struct v4l2_int_device *s, struct v4l2_ifparm *p)
{
	if (s == NULL) {
        dev_dbg(tw2828_v4l2_data.dev,
                "%s:ERROR!! no slave device set!\n", __func__);
        return;
    }
	memset(p, 0, sizeof(*p));
	p->if_type = V4L2_IF_TYPE_BT656;
    p->u.bt656.mode = V4L2_IF_TYPE_BT656_MODE_NOBT_8BIT;
    p->u.bt656.nobt_hs_inv = 0; // Do NOT invert Hsync 
    p->u.bt656.nobt_vs_inv = 0; // Do NOT invert Vsync
    p->u.bt656.bt_sync_correct = 0; // use internal Vsync
    p->u.bt656.clock_curr = 0; //BT656_INTER
    p->u.bt656.latch_clk_inv = 0; // Do NOT invert PIXEL clock
}

static int ioctl_g_ifparm(struct v4l2_int_device *s, struct v4l2_ifparm *p)
{
	
    printk( "ioctl_g_ifparm %s\n", __func__);
	if (video_idx == 0x0) {
		ioctl_g_ifparm_bt656(s, p);
	}
	else {
		ioctl_g_ifparm_bt1120( s, p );
	}
	return 0;
}

static void tw2828_get_std(v4l2_std_id *std)
{
	int idx;

	if (video_idx == TW2828_FMT_PRIV0) {
		/* PAL */
		*std = V4L2_STD_PAL;
		idx = TW2828_FMT_PRIV0;
	} else {
		*std = V4L2_STD_PRIV_1080_HD;
		idx = TW2828_FMT_PRIV1;
	}

	if (*std != tw2828_v4l2_data.std_id) {
		video_idx = idx;
		tw2828_v4l2_data.std_id = *std;
		tw2828_v4l2_data.sen.pix.width = video_fmts[video_idx].raw_width;
		tw2828_v4l2_data.sen.pix.height = video_fmts[video_idx].raw_height;
	}
}

static int ioctl_g_fmt_cap(struct v4l2_int_device *s, struct v4l2_format *f)
{
	struct sensor_data *sensor;
	v4l2_std_id std;
	
	sensor = (struct sensor_data*)(s->priv);
	switch (f->type) {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
		f->fmt.pix = sensor->pix;
		printk(KERN_DEBUG "%s: fmt.pix.width=%d fmt.pix.height=%d\n", 
						__func__, f->fmt.pix.width, f->fmt.pix.height);
		break;

	case V4L2_BUF_TYPE_PRIVATE: 
		tw2828_get_std(&std);
		f->fmt.pix.pixelformat = (u32)std; //?? std or FMT?
		break;

	default:
		f->fmt.pix = sensor->pix;
		break;
	}

    return 0;
}

static int ioctl_queryctrl(struct v4l2_int_device *s,
               struct v4l2_queryctrl *qc)
{
    int i;

//	dev_dbg(&tw2828_v4l2_data.sen.i2c_client->dev, "%s\n", __func__);
    for (i = 0; i < ARRAY_SIZE(tw2828_v4l2_qctrl); i++)
        if (qc->id && qc->id == tw2828_v4l2_qctrl[i].id) {
            memcpy(qc, &(tw2828_v4l2_qctrl[i]), sizeof(*qc));
            return 0;
        }

    return -EINVAL;
}

static int ioctl_dev_init(struct v4l2_int_device *s)
{
    return 0;
}

static int ioctl_init(struct v4l2_int_device *s)
{
    return 0;
}

extern void tw2828_select_bt1120(void);
extern void tw2828_select_bt656(void);

static void select_output(int idx)
{
	if (idx == TW2828_FMT_PRIV0) {
		tw2828_select_bt656();
	}
	else {
		tw2828_select_bt1120();
	}
	printk("Select input %s\n", (video_idx == 0) ? "BT656": "BT1120");
}

void set_current_format(int idx)
{
	tw2828_v4l2_data.sen.pix.width = video_fmts[idx].raw_width;
	tw2828_v4l2_data.sen.pix.height = video_fmts[idx].raw_height;
	tw2828_v4l2_data.sen.pix.pixelformat = TW2828_FMT;
	// other field such as image_size 
}

static int ioctl_s_fmt_cap(struct v4l2_int_device *s, struct v4l2_format *f)
{
	int i;
	
	// actually mxc driver will not call this function
	return 0;

#if 0
	printk("To match %d * %d  format: 0x%x %c%c%c%c\n", 
			f->fmt.pix.width, f->fmt.pix.height, 
			f->fmt.pix.pixelformat,
			f->fmt.pix.pixelformat & 0xff,
			(f->fmt.pix.pixelformat >> 8)  & 0xff,
            (f->fmt.pix.pixelformat >> 16) & 0xff,
            (f->fmt.pix.pixelformat >> 24) & 0xff);
	for (i = 0x0; i < TW2828_MODE_MAX; ++i)	 {
		if (video_fmts[i].active_height == f->fmt.pix.height && 
			video_fmts[i].active_width == f->fmt.pix.width &&
			TW2828_FMT == f->fmt.pix.pixelformat) {
			break;
		}
	}
	if (video_idx == i) {
		return 0;
	}

	if (i == TW2828_MODE_MAX) {  // do not find the match mode
		video_idx = 0x0;
		printk(KERN_ERR "Error mode %d x %d, use default\n", 
				f->fmt.pix.width, f->fmt.pix.height);
		return -1;
	}

	video_idx = i;
	tw2828_v4l2_data.sen.pix.width = video_fmts[video_idx].raw_width;
	tw2828_v4l2_data.sen.pix.height = video_fmts[video_idx].raw_height;
	f->fmt.pix.width = video_fmts[video_idx].active_width;
	f->fmt.pix.height = video_fmts[video_idx].active_height;
	
	return 0;
#endif
}


static int ioctl_s_parm(struct v4l2_int_device *s, struct v4l2_streamparm *f)
{
	if (f->parm.capture.capturemode == 0x0) { // PAL
		video_idx = TW2828_FMT_PRIV0;
	}
	else { // 1080HD
		video_idx = TW2828_FMT_PRIV1;
	}

	 //?? std or FMT?
	set_current_format(video_idx);
	select_output(video_idx);

	return 0;
}

static int ioctl_g_parm(struct v4l2_int_device *s, struct v4l2_streamparm *a)
{
	struct sensor *sensor = s->priv;
	struct v4l2_captureparm *cparm = &a->parm.capture;
	
	pr_debug( "%s\n", __func__);

	switch (a->type) {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
		dev_dbg(tw2828_v4l2_data.dev,
				         "%s: type is V4L2_BUF_TYPE_VIDEO_CAPTURE\n", __func__);
		memset(a, 0, sizeof(*a));
		a->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		cparm->capability = sensor->sen.streamcap.capability;
		cparm->timeperframe = sensor->sen.streamcap.timeperframe;
		cparm->capturemode = sensor->sen.streamcap.capturemode;
		break;
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
	case V4L2_BUF_TYPE_VIDEO_OVERLAY:
	case V4L2_BUF_TYPE_VBI_CAPTURE:
	case V4L2_BUF_TYPE_VBI_OUTPUT:
	case V4L2_BUF_TYPE_SLICED_VBI_CAPTURE:
	case V4L2_BUF_TYPE_SLICED_VBI_OUTPUT:
		break;
	default:
		pr_debug("ioctl_g_parm:type is unknown %d\n", a->type);
		break;
	}
	return 0;
}

static struct v4l2_int_ioctl_desc tw2828_v4l2_ioctl_desc[] = {
    {vidioc_int_g_chip_ident_num, (v4l2_int_ioctl_func*)ioctl_g_chip_ident},
    {vidioc_int_enum_framesizes_num, (v4l2_int_ioctl_func *) ioctl_enum_framesizes},
    {vidioc_int_enum_fmt_cap_num, (v4l2_int_ioctl_func*)ioctl_enum_fmt_cap},
	{vidioc_int_dev_init_num, (v4l2_int_ioctl_func *)ioctl_dev_init},
	{vidioc_int_dev_init_num, (v4l2_int_ioctl_func*)ioctl_dev_init},
	{vidioc_int_g_ifparm_num, (v4l2_int_ioctl_func*)ioctl_g_ifparm},
	{vidioc_int_g_fmt_cap_num, (v4l2_int_ioctl_func*)ioctl_g_fmt_cap},
	{vidioc_int_s_fmt_cap_num, (v4l2_int_ioctl_func *)ioctl_s_fmt_cap},
	{vidioc_int_queryctrl_num, (v4l2_int_ioctl_func*)ioctl_queryctrl},
	{vidioc_int_init_num, (v4l2_int_ioctl_func*)ioctl_init},
    {vidioc_int_s_parm_num, (v4l2_int_ioctl_func*)ioctl_s_parm},
	{vidioc_int_g_parm_num, (v4l2_int_ioctl_func*)ioctl_g_parm},	
};

static struct v4l2_int_slave tw2828_v4l2_slave = {
	.ioctls = tw2828_v4l2_ioctl_desc,
	.num_ioctls = ARRAY_SIZE(tw2828_v4l2_ioctl_desc),
};

static struct v4l2_int_device tw2828_v4l2_int_device = {
	.module = THIS_MODULE,
	.name = "tw2828_v4l2",
	.type = v4l2_int_type_slave,
	.u = {
		.slave = &tw2828_v4l2_slave,
	},
};

static ssize_t show_mode_id(struct device * dev,
                              struct device_attribute * attr, char * buf)
{
    return snprintf(buf, PAGE_SIZE, "%d\n", (int)video_idx);
}

static ssize_t store_mode_id(struct device * dev, 
			struct device_attribute * attr, const char * buf, size_t count)
{
    unsigned long data;
    ssize_t len;
    char * endp;

    data = simple_strtoul(buf, &endp, 10);
    if(data < 0 || data >= 8)
        return -EINVAL;

    len = endp - buf;
	video_idx = data;


    printk( KERN_DEBUG "New mode_id=%d\t len=%d\n", (int)data, len);

	set_current_format(video_idx);
	select_output(video_idx);

    return count;
}
// create /sys/devices/platform/tw2828/mode_id
static DEVICE_ATTR(mode_id, S_IWUSR | S_IRUGO, show_mode_id, store_mode_id);


int tw2828_init_v4l2(struct device *pdev)
{
	int ret;

	memset(&tw2828_v4l2_data, 0, sizeof(tw2828_v4l2_data));
	tw2828_v4l2_data.sen.streamcap.capability = V4L2_MODE_HIGHQUALITY |
									V4L2_CAP_TIMEPERFRAME;
	tw2828_v4l2_data.sen.streamcap.capturemode = 0;
	tw2828_v4l2_data.sen.streamcap.timeperframe.denominator = 25; //25fps
	tw2828_v4l2_data.sen.streamcap.timeperframe.numerator = 1;
	tw2828_v4l2_data.std_id = V4L2_STD_PAL;//V4L2_DV_1080P50;  // V4L2_STD_PAL ?? 
	video_idx  = TW2828_FMT_PRIV0;  // ?? 
	tw2828_v4l2_data.sen.pix.width = video_fmts[video_idx].raw_width;
	tw2828_v4l2_data.sen.pix.height = video_fmts[video_idx].raw_height;
	tw2828_v4l2_data.sen.pix.pixelformat = TW2828_FMT;  /* YUV422 */

// 3014     /* Used to detect TV in (type 1) vs. camera (type 0)*/
// 3015     cam->device_type = cam_fmt.fmt.pix.priv;

	tw2828_v4l2_data.sen.pix.priv = 0x1;   // ??
	tw2828_v4l2_data.sen.on = true;
	tw2828_v4l2_data.sen.csi = 0x0;   // notice here, the csi number is 0

	tw2828_v4l2_data.dev = pdev;
	tw2828_v4l2_data.v4l2_int_device = &tw2828_v4l2_int_device;

	select_output(video_idx);
	set_current_format(video_idx);
 
	// device CSI reset
	tw2828_v4l2_int_device.priv = &(tw2828_v4l2_data.sen);
	printk("\ntw2828: used as a v4l2 device\n");
	ret = v4l2_int_device_register(&tw2828_v4l2_int_device);
	if (ret < 0) {
		printk(KERN_ERR "tw2828: error register v4l2 device\n");
	}

	ret = device_create_file(pdev, &dev_attr_mode_id);
	if (ret < 0) {
		printk(KERN_ERR "tw2828: error register sys mode entry\n");
	}
	return ret;
}

void tw2828_remove_v4l2(void)
{
	v4l2_int_device_unregister(&tw2828_v4l2_int_device);
	device_remove_file(tw2828_v4l2_data.dev, &dev_attr_mode_id);
}

