
#ifndef _TW2828_REG_H_
#define _TW2828_REG_H_

#define TW2828_CHIP_ID      0x2828

//Define MemBase
#define DISPLAYBASE	40 //Line Unit
#define RECBASE		(DISPLAYBASE+1080)
#define SPOTBASE	(RECBASE + 1152) 

// Memory
#define DRAM_ADDR_L		0x000
#define DRAM_ADDR_M		0x001
#define DRAM_ADDR_H		0x002
#define DRAM_RW_CTRL	0x003
#define DRAM_DATA		0x004
#define DRAM_STATUS		0x044
#define DRAM_BIGENDIAN_EN			0x045

// Simple OSD for Main
#define OSD_EN_CTRL       0x080
#define OSD_FONT_COLOR_R1	0x093
               
#define OSD_RAM_ADDR_L		0x0a8
#define OSD_RAM_ADDR_H		0x0a9
#define OSD_RAM_DATA_0		0x0aa
#define OSD_RAM_DATA_1		0x0ab
#define OSD_RAM_DATA_2		0x0ac
#define OSD_RAM_DATA_3		0x0ad
#define OSD_RAM_DATA_4		0x0ae
#define OSD_RAM_DATA_5		0x0af

#define OSD_DISP_RAM_ADDR_L	0x0b0
#define OSD_DISP_RAM_ADDR_H	0x0b1
#define OSD_DISP_RAM_DATA	  0x0b2

#define DCM_FIFO_CNT						0x1fd

#define VPLLMR			0x205
#define VPLLMR_H		0x206
#define VPLLNR			0x207
#define VPLLCTRL		0x22d
#define VPLLPHASE   0x22e

#define IN_RATE				0x21b
#define DGTBLK1RESET		0x20b
#define DGTBLK2RESET		0x20c
#define DGTBLK3RESET		0x20d
#define DGTBLK4RESET		0x20e
#define DGTBLK5RESET		0x20f

#define DISPMEMPITCH		0x22b
#define DISPMEMPITCH_H	0x22c

#define SYSTEM_SEL 0x3C6

#define MSEBASE_L		0x47a
#define MSEBASE_M		0x47b
#define MSEBASE_H		0x47c
#define MN_MSEIMGCTRL		0x490 // bit [0] 1: mse load buf update en or BUSY
#define MN_MSEIMGSEL		0x47d
#define MN_MSE_REG_UPDATE	0x54e

#define SYS_CTRL		0x480
  #define MDBOX_POS_SEL		0x40

#define MN_MSE0_HPOS_L		0x53c
#define MN_MSE0_HPOS_H		0x53d
#define MN_MSE0_VPOS_L		0x53e
#define MN_MSE0_VPOS_H		0x53f
#define MN_MSE1_HPOS_L		0x540
#define MN_MSE1_HPOS_H		0x541
#define MN_MSE1_VPOS_L		0x542
#define MN_MSE1_VPOS_H		0x543

#define MN_MOUSECTRL		0x544
#define MouseImgLoc			0x54c
#define MouseImgData		0x54d
#define MouseWriteEn		0x54f

#define BND_LV_CH_EN_0		0x4ec
#define BND_LV_CH_EN_1		0x4ed
#define BND_PB_CH_EN_2		0x4ee
#define BND_PB_CH_EN_3		0x4ef
#define BND_PB_CH_EN_4		0x4c2

#define SBOX0_CTRL		0x50c
#define SBOX0_HL_L		0x513
#define SBOX0_HL_H		0x514
#define SBOX0_HR_L		0x51b
#define SBOX0_HR_H		0x51c
#define SBOX0_VT_L		0x523
#define SBOX0_VT_H		0x524
#define SBOX0_VB_L		0x52b
#define SBOX0_VB_H		0x52c
#define SBOX_BCOLOR_R	0x533
#define SBOX_BCOLOR_G	0x534
#define SBOX_BCOLOR_B	0x535

#define MDBOXCTRL16		0x418   //*4
#define MDBOX16_HL_L	0x41e
#define MDBOX16_HL_H	0x41f
#define MDBOX16_VT_L	0x426
#define MDBOX16_VT_H	0x427
#define MDBOX16_HS_L		0x42e	//*4
#define MDBOX16_HS_H		0x42f
#define MDBOX16_VS_L		0x436	//*4
#define MDBOX16_VS_H		0x437
#define MDBOX16_HCELL	0x43e	//*4  Cell size
#define MDBOX16_VCELL	0x442	//*4  Cell size
#define MDCUR16_HPOS		0x446	//*4
#define MDCUR16_VPOS		0x44a	//*4
#define MDHD_CH_SEL		0x4ff
#define MDHD_DET_EN		0x772
#define MDHD_MODE			0x773
#define MDHD_CTRL			0x774
#define MDHDADJ_H0		0x710 
#define MDHDADJ_V0		0x718 
#define MDDETECT_CTRL_0	0x818
		
#define MDBOXCTRL0		0x550  
#define MDBOX0_HL_L		0x568	
#define MDBOX0_HL_H		0x569
#define MDBOX0_VT_L		0x588	
#define MDBOX0_VT_H		0x589
#define MDBOX0_HS_L		0x5a8	
#define MDBOX0_HS_H		0x5a9
#define MDBOX0_VS_L		0x5c8	
#define MDBOX0_VS_H		0x5c9
#define MDCUR0_HPOS		0x5f8	
#define MDCUR0_VPOS		0x48b	

#define MD_MASK_0_L	0x800
#define MD_MASK_0_H	0x801
#define MD_LEVEL_SENS_0	0x81a
#define MD_SPEED_0	0x81b
#define MD_TEMPO_0	0x81e
#define MD_DET_CTRL6_0	0x81f

#define XUPS_L		0x406
#define XUPS_H		0x407
#define YUPS_L		0x408
#define YUPS_H		0x409
#define XUPS_WIDTH_L		0x40e
#define XUPS_WIDTH_H		0x40f
#define YUPS_HEIGHT_L		0x410
#define YUPS_HEIGHT_H		0x411

#define POS_XUPS_L		0x491
#define POS_XUPS_H		0x492
#define POS_YUPS_L		0x493
#define POS_YUPS_H		0x494
#define POS_UPS_CTRL	0x495
#define LCDMISCCTRL		0x4cf
#define SEL_2D			0x4fe
#define DI_HEIGHT_L		0x400
#define DI_HEIGHT_H		0x401
#define DI_WIDTH_L		0x402
#define DI_WIDTH_H		0x403
#define WEAVE_WIDTH_L		0x4e1
#define WEAVE_WIDTH_H		0x4e2
#define WEAVE_HEIGHT_L	0x4e3
#define WEAVE_HEIGHT_H	0x4e4

#define MNHSTART_L	0x4ac
#define MNHSTART_H	0x4ad
#define MNVSTART_L	0x4ae
#define MNVSTART_H	0x4af

#define HTT_L			0x500
#define HTT_H			0x501
#define VTT_L			0x502
#define VTT_H			0x503
#define HDE_L			0x504
#define HDE_H			0x505
#define VDE_L			0x506
#define VDE_H			0x507
#define HFPORCH			0x508
#define VFPORCH			0x509
#define HSYNCW			0x50a
#define VSYNCW			0x50b

#define DI_ENABLE		0x54b

#define WIN0_CTRL	0x600
#define WIN0_VPOSI_L	0x614
#define WIN0_VPOSI_H	0x615
#define WIN0_VSIZE_L	0x63c
#define WIN0_VSIZE_H	0x63d
#define WIN0_HPOSI_L	0x664
#define WIN0_HPOSI_H	0x665
#define WIN0_HSIZE_L	0x68c
#define WIN0_HSIZE_H	0x68d

#define PB_MODE		0x6b4	
#define RGB_MODE	0x6b6

#define FREEZE_PB_L		0x6bb
#define FREEZE_PB_H		0x6bc

#define RGB_XSTART_0		0x6c0
#define RGB_YSTART_0		0x6d4

#define PB_FRC_CTRL_0		0x6f0
#define PB_FRC_CTRL_1		0x6f1
#define PB_FRC_CTRL_2		0x6bf

#define FIFO_LIMIT_CTRL  0x6fb

#define SYSTEM_MISC_CTRL 0x6fc
#define PB_CH_EN0		 0x6fd
#define PB_CH_EN1		 0x6fe
#define CAS_CH_EN		 0x6be
#define CAS_WIN_EN		 0x6bd

#define WINCHSEL_1_0	0x3d2
#define WIN0_XDNS_L		0x300
#define WIN0_XDNS_H		0x301
#define WIN0_YDNS_L		0x320
#define WIN0_YDNS_H		0x321
#define PB0_SR_XDNS_L		0x36a
#define PB01_SR_XDNS_H		0x36b
#define PB1_SR_XDNS_L		0x36c
#define PB2_SR_XDNS_L		0x36d
#define PB23_SR_XDNS_H		0x36e
#define PB3_SR_XDNS_L		0x36f
#define PB0_TG_XDNS_L		0x364
#define PB01_TG_XDNS_H		0x365
#define PB1_TG_XDNS_L		0x366
#define PB2_TG_XDNS_L		0x367
#define PB23_TG_XDNS_H		0x368
#define PB3_TG_XDNS_L		0x369
#define PB4_SR_XDNS_L		0x3a7
#define PB4_SRTG_XDNS_H		0x3a8
#define PB4_TG_XDNS_L		0x3a9
#define REC_DNS_CIF_0		0x353
#define REC_DNS_CIF_1		0x352
#define REC_DNS_CIF_2		0x351
#define REC_DNS_CIF_3		0x350
#define PB0_SR_YDNS_L		0x35e
#define PB01_SR_YDNS_H		0x35f
#define PB1_SR_YDNS_L		0x360
#define PB2_SR_YDNS_L		0x361
#define PB23_SR_YDNS_H		0x362
#define PB3_SR_YDNS_L		0x363
#define PB0_TG_YDNS_L		0x358
#define PB01_TG_YDNS_H		0x359
#define PB1_TG_YDNS_L		0x35a
#define PB2_TG_YDNS_L		0x35b
#define PB23_TG_YDNS_H		0x35c
#define PB3_TG_YDNS_L		0x35d
#define PB4_SR_YDNS_L		0x3aa
#define PB4_SRTG_YDNS_H		0x3ab
#define PB4_TG_YDNS_L		0x3ac
#define PB_CTRL_MODE	0x370
#define PB_CTRL_MISC	0x371
#define PB_CTRL_FLD		0x372
#define PB_CTRL_MISC1	0x356
#define PB5_CTRL		0x3ad
#define PB_CHID_INSERT	0x262
#define PB0_CHID_DEC	0x3de
#define PB0_VOS			0x3df
#define PB0_FOS			0x3e0
#define PB0_HOS			0x3e1
#define PB0_MID			0x3e2
#define PB_FLD_CTRL		0x3f2
#define PB_FLD_SEL		0x3fc
#define PB10_VSIZE		0x3fd
#define PB32_VSIZE		0x3fe

#define PB_NSTD_LIMIT_UPPER_L		0x3f8
#define PB_NSTD_LIMIT_UPPER_H		0x3f9
#define PB_NSTD_LIMIT_LOWER_L		0x3fa
#define PB_NSTD_LIMIT_LOWER_H		0x3fb

#define WIN37_VPOSI_L	0x47e
#define WIN37_HPOSI_L	0x47f
#define WIN37_POSI_H		0x482
#define WIN37_VSIZE_L	0x480
#define WIN37_HSIZE_L	0x481
#define WIN37_SIZE_H	0x483
#define WIN37_CTRL		0x484

#define SBOX_CTRL_3_0		0x50c
#define SBOX_HL_1_0			0x514
#define SBOX_HL_3_2			0x518
#define SBOX_HR_1_0			0x51c
#define SBOX_HR_3_2			0x520
#define SBOX_VT_1_0			0x524
#define SBOX_VT_3_2			0x528
#define SBOX_VB_1_0			0x524
#define SBOX_VB_3_2			0x528
#define SBOX_BCOLOR			0x538

#define RECBUFCTRL1_A	0xc00
#define RECBUFCTRL1_B	0xc10

#define RECPORTCTRL1	0xc20
#define RECPORTTBSEL1_A	0xc21		
#define RECPORTTBSEL1_B	0xc22
#define RECPORT_HDE1		0xc23
#define RECPORT_VDE1		0xc24

#define RECPORTCTRL5	0xc34
#define RECPORTTBSEL5_ADDR	0xc35		
#define RECPORTTBSEL5_DATA	0xc36
#define RECPORT_TB_MAX5		0xc37		
#define RECPORT_HDE5		0xc38
#define RECPORT_VDE5		0xc39

#define RECPORT_TB_MAX1		0xc4c

#define RECPINCTL1			0xc4e

#define RECNETPORTCTL		0xc52
#define RECNETPORTTBSEL_ADDR		0xc53
#define RECNETPORTTBSEL_DATA     0xc54
#define RECNETPORT_TB_MAX	0xc55

#define REC_MISC_CTRL		0xc58

#define REC_QCIF_OSD_L  0xc5d
#define REC_QCIF_OSD_H  0xc60
#define REC_SPOT_STATUS_ON	0xc61

#define REC_CLKMODE			0xc68	
#define REC_IFRAME			0xc69

#define REC16BITMODECRTL	0xcf0

#define RECPINPORTSEL1		0xcf5
#define RECPINPORTSEL2		0xcf6

#define RECFIELDSWITCH1		0xcf1
#define RECFIELDSWITCH2		0xcf2

#define RECNEW_FLI_SW_MD	0xc77
#define RECNEW_FLI_RD			0xc79
#define RECNEW_FLI_SW_EN	0xc7a

#define REC_FORMATTER			0xe46

#define SPT_QCIF_ON_L			0xc8c
#define SPT_QCIF_ON_H			0xc8d
#define SPT_9CH_ON_L			0xc8e
#define SPT_9CH_ON_H			0xc8f



// freeze spot/ rec
#define FREEZE_REC_L		0xc64
#define FREEZE_REC_H		0xc65

#define FREEZE_SPOT_L		0xc66
#define FREEZE_SPOT_H		0xc67

#define REC_WB_FRSC			0xc6a
#define REC_RP_FRSC			0xc6b
#define BUFMAP_CTRL			0xcfd
#define BUFMAP_EN				0xcfe
#define RECPB_FRSC			0xcff

// Channel ID Enc
#define REC1_CHID_CTRL		0xd01
#define REC1_HOS		    0xd02
#define REC1_MAX_LEVEL		0xd03
#define REC1_MIN_LEVEL		0xd04
#define REC1_VOS		    0xd05

#define REC1_FOS		    0xd90

#define REC_OSD_BNDRY_EN_0				0xe04 //RECSPOT  
#define REC_OSD_BNDRY_EN_1				0xe44

#define REC_OSD_RAM_ADDR_L		0xee0
#define REC_OSD_RAM_ADDR_H		0xee1
#define REC_OSD_RAM_DATA_0		0xee2
#define REC_OSD_RAM_DATA_1		0xee3
#define REC_OSD_RAM_DATA_2		0xee4
#define REC_OSD_RAM_DATA_3		0xee5
#define REC_OSD_RAM_DATA_4		0xee6
#define REC_OSD_RAM_DATA_5		0xee7

#define REC_OSD_DISP_RAM_ADDR_L	0xee8
#define REC_OSD_DISP_RAM_ADDR_H	0xee9
#define REC_OSD_DISP_RAM_DATA	0xeea

#define REC_OSD_FONT_COLOR_Y	0xe2d
#define REC_OSD_FONT_COLOR_CB	0xe2e
#define REC_OSD_FONT_COLOR_CR	0xe2f

#define SPOT_VSYNC_DECT				0x24f
#define SPT1_BUFCTRL_A	0xc90
#define SPT1_BUFCTRL_B	0xca0

#define SPT1_HTT_L		0xf00
#define SPT1_HTT_H		0xf01
#define SPT1_HA_L		0xf02
#define SPT1_HA_H		0xf03
#define SPT1_VA_L		0xf04
#define SPT1_VA_H		0xf05
#define SPT_FRC_0		0xf07
#define SPT1_PVSYNC		0xf08
#define SPT1_VTT_L		0xf09
#define SPT1_VTT_H		0xf0a
#define SPT_FRC		0xf0b

#define SPT1CTRL		0xf0f
#define SPT1_MEM_HS_L	0xf10
#define SPT1_MEM_VS_L	0xf11
#define SPT1_MEM_HVS_H	0xf12

#define SPT1_DNS_H		0xf0d
#define SPT1_DNS_H1		0xf0e
#define SPT1_DNS_SW		0xf13
#define SPT1_DNS_SH		0xf14
#define SPT1_DNS_TW		0xf15
#define SPT1_DNS_TH		0xf16

#define SPT1_ENC		0xf17
#define SPT1_ENC_CTRL   0xf18

#define SPTROTFREQ		0xf29

#define SPT2_HTT_L		0xf60
#define SPT2_HTT_H		0xf61
#define SPT2_HA_L		0xf62
#define SPT2_HA_H		0xf63
#define SPT2_VA_L		0xf64
#define SPT2_VA_H		0xf65
#define SPT2_FRC_0		0xf67
#define SPT2_VTT_L		0xf69
#define SPT2_VTT_H		0xf6a
#define SPT2_FRC		0xf6b

#define SPT2CTRL		0xf6f
#define SPT2_MEM_HS_L	0xf70
#define SPT2_MEM_VS_L	0xf71
#define SPT2_MEM_HVS_H	0xf72
#define SPT2_DNS_H		0xf6d
#define SPT2_DNS_H1		0xf6e
#define SPT2_DNS_SW		0xf73
#define SPT2_DNS_SH		0xf74
#define SPT2_DNS_TW		0xf75
#define SPT2_DNS_TH		0xf76
#define SPT2_ENC		0xf77
#define SPT2_ENC_CTRL	0xf78

#define SPT3CTRL		0xf9f
#define SPT_DISP_CH1	0xff0
#define SPT_PWR_ON	0xf9b
#define SPT12_SRC			0xf9c

#define IRQ_NV_1_EN		0x2b0
#define IRQ_NV_2_EN		0x2b1
#define IRQ_NV_3_EN		0x2b2
#define IRQ_MD_1_EN		0x2b3
#define IRQ_MD_2_EN		0x2b4
#define IRQ_MD_3_EN		0x2b5
#define IRQ_BD_1_EN		0x2b6
#define IRQ_BD_2_EN		0x2b7
#define IRQ_BD_3_EN		0x2b8
#define IRQ_ND_1_EN		0x2b9
#define IRQ_ND_2_EN		0x2ba
#define IRQ_ND_3_EN		0x2bb
#define IRQ_NV_1_STATUS	0x2bc
#define IRQ_NV_2_STATUS	0x2bd
#define IRQ_NV_3_STATUS	0x2be
#define IRQ_MD_1_STATUS	0x2bf
#define IRQ_MD_2_STATUS	0x2c0
#define IRQ_MD_3_STATUS	0x2c1
#define IRQ_BD_1_STATUS	0x2c2
#define IRQ_BD_2_STATUS	0x2c3
#define IRQ_BD_3_STATUS	0x2c4
#define IRQ_ND_1_STATUS	0x2c5
#define IRQ_ND_2_STATUS	0x2c6
#define IRQ_ND_3_STATUS	0x2c7
#define IRQ_CTRL			0x2c8
#define IRQ_NOVID_CLEAR		0x2c2
#define IRQ_MD_CLEAR		  0x2c3
#define IRQ_BLIND_CLEAR		0x2c4
#define IRQ_NIGHT_CLEAR		0x2c5

#ifdef _TW2882_FPGA_V6_
#define PLLCTRL				0x2f0
#define PLLMR				0x2f1
#define PLLPHASE			0x2f2
#define PLLDIVCLK			0x2f3
#define PLLCLKOUT0_DIV	    0x2f4
#define PLLCLKOUT0_PHASE	0x2f5
#define PLLCLKOUT1_DIV	    0x2f6
#define PLLCLKOUT1_PHASE	0x2f7
#define PLLCLKOUT2_DIV	    0x2f8
#define PLLCLKOUT2_PHASE	0x2f9
#define PLLCLKOUT3_DIV	    0x2fa
#define PLLCLKOUT3_PHASE	0x2fb
#endif

#endif 
