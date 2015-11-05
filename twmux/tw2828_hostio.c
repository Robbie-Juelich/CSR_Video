#include <linux/kernel.h>
#include <linux/io.h>
#include <mach/hardware.h>

#include <linux/delay.h>

static void __iomem *base = NULL;

void host_io_init (void)
{
	base = ioremap_nocache(WEIM_ARB_BASE_ADDR, SZ_128M);
	return;
}

void host_io_uninit(void)
{
	if (base)
		iounmap(base);
	return;
}


u8 ReadReg(u16 reg) 
{
    u8 val = 0;;
    val = readb((void * __iomem)(base + reg) );
//	mdelay(5);
    return val; 
} 

void WriteReg(u16 reg, u8 val)
{ 
    writeb(val, (void * __iomem) (base + reg) );
	udelay(10);
    return;
}

void WriteReg2(u16 reg, u8 val)
{ 
	u8 new_val;
    writeb(val, (void * __iomem) (base + reg) );
	udelay(10);
	new_val = readb((void *__iomem)(base + reg));
	if(new_val != val) {
		printk("-------------------\n"
			"write 0x%x to 0x%x error, retry\n"
			"----------------\n", val, reg);
    	writeb(val, (void * __iomem) (base + reg) );
	}
	udelay(5);
    return;
}

