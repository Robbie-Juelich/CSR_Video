

#define ISIL_ENDFLAG           0xffff

unsigned short tbl_ntsc_tw2968_page0[] = {
		// 00        01          02           03          04          05           06           07          08           09          0A          0B          0C          0D          0E           0F
		ISIL_ENDFLAG		
};

unsigned short tbl_ntsc_tw2968_page1[] = {
		// 00        01          02           03          04          05           06           07          08           09          0A          0B          0C          0D          0E           0F
		ISIL_ENDFLAG
};

unsigned short tbl_pal_tw2968_page0[] = {
		// 00        01          02           03          04          05           06           07          08           09          0A          0B          0C          0D          0E           0F
		ISIL_ENDFLAG		
};

unsigned short tbl_pal_tw2968_page1[] = { ISIL_ENDFLAG,
		// 00        01          02           03          04          05           06           07          08           09          0A          0B          0C          0D          0E           0F
		ISIL_ENDFLAG
};

unsigned short tbl_tw2968_dump_108m[] = {
		//addr,	data 
		0x40,   0x00,  // page 0 
		0x6f,   0x00,  // Video data buses are 27MHz ( Port 1 ~ 4 )
		0xcc,   0x00,  // Video decoder uses 27MHz D1 mode ( Channel 1 ~ 8 )
		0xcd,   0x00,  // Video decoder output uses 27MHz video data ( Channel 1 ~ 8 )
		0xf9,   0x40,  // Video Clock pins ( CLKNO, CLKPO ) are 27/54/108MHz
#if 0
		0xe7,	0xaa,  // Video Data Port 1 ~ 4 are single output ( 108MHz )
		0xfa,	0x4a,  // Video Clock frequency is 27/36Mhz output 
#endif
		0xe7,	0x55,  // Video Data Port 1 ~ 4 are single output ( 108MHz )
		0xfa,	0x45,  // Video Clock frequency is 27/36Mhz output 


		0x9e,	0x72,	// channel ID enable

		// record
		0xe8,	0x10,
		0xe9,	0x32,
		0xea,	0x54,
		0xeb,	0x76,
		// live		
		0xec,	0x10,
		0xed,	0x32,
		0xee,	0x54,
		0xef,	0x76,	
		
		// depends on a board
		//0xfb,	0x2f,  // polarity
		0x9f,   0x00,  // Video clock phase delay ( optional )
		
		0x40,   0x00,  // page 0 
		0x0e,   0x00,
		0x1e,   0x00,
		0x2e,   0x00,
		0x3e,   0x00,
		0x40,   0x01,  // page 1 
		0x0e,   0x00,
		0x1e,   0x00,
		0x2e,   0x00,
		0x3e,   0x00,
				
		0x40,   0x00,  // page 0 

		ISIL_ENDFLAG
}; 		

/*
unsigned short tbl_tw2968_dump_144m[] = {
		//addr,	data 
		0x40,   0x00,  // page 0 
		0x6f,   0xf0,  // Video data buses are 27MHz ( Port 1 ~ 4 )
		0xcc,   0xff,  // Video decoder uses 27MHz D1 mode ( Channel 1 ~ 8 )
		0xcd,   0xff,  // Video decoder output uses 27MHz video data ( Channel 1 ~ 8 )
		0xf9,   0x03,  // Video Clock pins ( CLKNO, CLKPO ) are 27/54/108MHz

		0xe7,	0xaa,  // Video Data Port 1 ~ 4 are single output ( 108MHz )
		
		0x9e,	0x72,	// channel ID enable

		// record
		0xe8,	0x10,
		0xe9,	0x32,
		0xea,	0x54,
		0xeb,	0x76,
		// live			
		0xec,	0x10,
		0xed,	0x32,
		0xee,	0x54,
		0xef,	0x76,				

		0xfa,	0x4a,  // Video Clock frequency is 27/36Mhz output 
		
		// depends on a board
		//0xfb,	0x2f,  // polarity
		0x9f,   0x00,  // Video clock phase delay ( optional )

		0x40,   0x00,  // page 0 

		ISIL_ENDFLAG
};
*/
#if 0
vdec.table_ntsc[0] = tbl_ntsc_tw2968_page0;
vdec.table_ntsc[1] = tbl_ntsc_tw2968_page1;
vdec.table_pal[0] = tbl_pal_tw2968_page0;
vdec.table_pal[1] = tbl_pal_tw2968_page1;
vdec.pdump_ntsc = tbl_tw2968_dump_108m;
vdec.pdump_pal  = tbl_tw2968_dump_108m;
vdec.page_register = 0x40;
vdec.max_page = 2;
#endif
