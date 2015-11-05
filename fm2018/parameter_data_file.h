#ifndef PARAMETER_DATA_FILE_H
#define PARAMETER_DATA_FILE_H

#define TWOMICNR_INIT_PARA_LENGTH    29

unsigned char parameter_data_ret_test[] = {	
    0xFC, 0xF3, 0x3B, 0x1E, 0x52, 0x00, 0x13,
};

unsigned char parameter_data_Init[] = {
//	0xC0,  // this is the slave address  
    0xFC, 0xF3, 0x3B, 0x1E, 0x52, 0x00, 0x13,
    //0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x70, 0x05, 0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3A, 0x00, 0x00,
//    0xFC, 0xF3, 0x3B, 0x1E, 0x3A, 0x80, 0x00  // ok ??
};

#define TWOMICNR_BYPASS_PARA_LENGTH    64

unsigned char parameter_data_bypass_mode[] = {
//    0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x34, 0x00, 0x22,
    0xFC, 0xF3, 0x3B, 0x1E, 0x36, 0x00, 0x1D,
    0xFC, 0xF3, 0x3B, 0x1E, 0x4A, 0x00, 0x22,
    0xFC, 0xF3, 0x3B, 0x1E, 0x4C, 0x00, 0x1D,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3D, 0x01, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x52, 0x00, 0x13,
    //0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x00,
    
	0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x4f, 0x00, 0x00,


    0xFC, 0xF3, 0x3B, 0x1E, 0x70, 0x05, 0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3A, 0x00, 0x00
};

#define TWOMICNR_HANDSET_PARA_LENGTH    267

unsigned char parameter_data_handset_mode[] = {
//    0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x34, 0x00, 0xDA,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3D, 0x01, 0x10,
	0xFC, 0xF3, 0x3B, 0x1E, 0x44, 0x00, 0x81,
    0xFC, 0xF3, 0x3B, 0x1E, 0x45, 0x00, 0xDE,
    0xFC, 0xF3, 0x3B, 0x1E, 0x46, 0x00, 0x14,
    0xFC, 0xF3, 0x3B, 0x1E, 0x47, 0x20, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x48, 0x0B, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x49, 0x0A, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0x4D, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x51, 0xc0, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x52, 0x00, 0x13,
    0xFC, 0xF3, 0x3B, 0x1E, 0x57, 0x7F, 0xFF,
    //0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x70, 0x05, 0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0xB3, 0x12, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xB4, 0x17, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xB5, 0x11, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xB6, 0x01, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBC, 0x50, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBD, 0x18, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBE, 0x28, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBF, 0x7F, 0xFF,
	0xFC, 0xF3, 0x3B, 0x1E, 0xD0, 0x1A, 0xCA,
	0xFC, 0xF3, 0x3B, 0x1E, 0xD1, 0x2C, 0xA6,
	0xFC, 0xF3, 0x3B, 0x1E, 0xD2, 0x51, 0x5D,
	0xFC, 0xF3, 0x3B, 0x1E, 0xD3, 0x77, 0x11,
    0xFC, 0xF3, 0x3B, 0x1E, 0xD5, 0x40, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xD6, 0x40, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xD7, 0x40, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xD8, 0x38, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xD9, 0x2E, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xDA, 0x54, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x00, 0x32, 0xF5,
    0xFC, 0xF3, 0x3B, 0x1F, 0x01, 0x28, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0B, 0x00, 0x60,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0C, 0x0D, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0D, 0x08, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3A, 0x00, 0x00
};

#define TWOMICNR_HANDFREE_PARA_LENGTH    281

unsigned char parameter_data_handfree_mode[] = {
//    0xC0,

    0xFC, 0xF3, 0x3B, 0x1E, 0x30, 0x02, 0x31,
    0xFC, 0xF3, 0x3B, 0x1E, 0x34, 0x00, 0x6B,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3D, 0x04, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x41, 0x01, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x44, 0x00, 0x81,
    0xFC, 0xF3, 0x3B, 0x1E, 0x45, 0x03, 0xCF,
    0xFC, 0xF3, 0x3B, 0x1E, 0x46, 0x00, 0x11,
    0xFC, 0xF3, 0x3B, 0x1E, 0x4D, 0x03, 0x00,
    //0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x63, 0x00, 0x03,
    0xFC, 0xF3, 0x3B, 0x1E, 0x70, 0x05, 0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x88, 0x38, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x89, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x8B, 0x00, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0x8C, 0x00, 0x10,
    0xFC, 0xF3, 0x3B, 0x1E, 0x92, 0x38, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xA0, 0x10, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBC, 0x68, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBD, 0x01, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBF, 0x70, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC0, 0x26, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC1, 0x10, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC5, 0x2B, 0x06,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC6, 0x0C, 0x1F,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC8, 0x28, 0x79,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC9, 0x65, 0xAB,
    0xFC, 0xF3, 0x3B, 0x1E, 0xCA, 0x40, 0x26,
    0xFC, 0xF3, 0x3B, 0x1E, 0xCB, 0x7F, 0xFF,
    0xFC, 0xF3, 0x3B, 0x1E, 0xCC, 0x7F, 0xFE,
    0xFC, 0xF3, 0x3B, 0x1E, 0xF8, 0x04, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xF9, 0x01, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xFF, 0x4B, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x00, 0x7F, 0xFF,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0A, 0x0A, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0C, 0x01, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0D, 0x78, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x86, 0x00, 0x07,
    0xFC, 0xF3, 0x3B, 0x1E, 0x87, 0x00, 0x03,
    0xFC, 0xF3, 0x3B, 0x1E, 0x52, 0x00, 0x13,

    //0xFC, 0xF3, 0x3B, 0x1E, 0x4A, 0x00, 0x11, // input PGA

    0xFC, 0xF3, 0x3B, 0x1E, 0x3A, 0x00, 0x00
};


unsigned char parameter_data_one_mic_handfree_mode[] = {
    0xFC, 0xF3, 0x3B, 0x1E, 0x30, 0x02, 0x31,
    //0xFC, 0xF3, 0x3B, 0x1E, 0x34, 0x00, 0x6B,
    0xFC, 0xF3, 0x3B, 0x1E, 0x34, 0x00, 0x60,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3D, 0x03, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3E, 0x01, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x41, 0x01, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x44, 0x00, 0x81,
    0xFC, 0xF3, 0x3B, 0x1E, 0x45, 0x03, 0xCF,
    0xFC, 0xF3, 0x3B, 0x1E, 0x46, 0x00, 0x11,
    0xFC, 0xF3, 0x3B, 0x1E, 0x47, 0x48, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x48, 0x10, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x49, 0x08, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0x4D, 0x02, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0x52, 0x00, 0x13,
    0xFC, 0xF3, 0x3B, 0x1E, 0x63, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x70, 0x05, 0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x86, 0x00, 0x09,
    0xFC, 0xF3, 0x3B, 0x1E, 0x87, 0x00, 0x03,
    0xFC, 0xF3, 0x3B, 0x1E, 0x88, 0x38, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x89, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x8B, 0x00, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0x8C, 0x00, 0x10,
    0xFC, 0xF3, 0x3B, 0x1E, 0x92, 0x78, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xA0, 0x10, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xA1, 0x33, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xA2, 0x32, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBC, 0x68, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBD, 0x01, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xBF, 0x70, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC0, 0x26, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC1, 0x10, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC5, 0x2B, 0x06,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC6, 0x0C, 0x1F,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC8, 0x28, 0x79,
    0xFC, 0xF3, 0x3B, 0x1E, 0xC9, 0x65, 0xAB,
    0xFC, 0xF3, 0x3B, 0x1E, 0xCA, 0x40, 0x26,
    0xFC, 0xF3, 0x3B, 0x1E, 0xCB, 0x7F, 0xFF,
    0xFC, 0xF3, 0x3B, 0x1E, 0xCC, 0x7F, 0xFE,
    0xFC, 0xF3, 0x3B, 0x1E, 0xF8, 0x04, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xF9, 0x01, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0xFF, 0x4B, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x00, 0x7F, 0xFF,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0A, 0x0A, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0C, 0x01, 0x00,
    0xFC, 0xF3, 0x3B, 0x1F, 0x0D, 0x78, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3A, 0x00, 0x00
};





#define TWOMICNR_TEST_MAIN_PARA_LENGTH    50

unsigned char parameter_data_TEST_MAIN_MIC[] = {
    0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x34, 0x00, 0xDA,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3D, 0x00, 0xA0,


    0xFC, 0xF3, 0x3B, 0x1E, 0x4F, 0x00, 0x10,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3d, 0x02, 0x80,
    0xFC, 0xF3, 0x3B, 0x1E, 0xA1, 0x66, 0x66,

	
//    0xFC, 0xF3, 0x3B, 0x1E, 0x4F, 0x00, 0x00,  // by zzc
    0xFC, 0xF3, 0x3B, 0x1E, 0x52, 0x00, 0x13,
    0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x01,  // by zzc
    0xFC, 0xF3, 0x3B, 0x1E, 0x70, 0x05, 0xC0,
	0xFC, 0xF3, 0x3B, 0x1E, 0x3A, 0x00, 0x00,
};


#define TWOMICNR_TEST_SUB_PARA_LENGTH    50

unsigned char parameter_data_TEST_SUB_MIC[] = {
    0xC0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x34, 0x00, 0xDA,
    0xFC, 0xF3, 0x3B, 0x1E, 0x3D, 0x00, 0xA0,
    0xFC, 0xF3, 0x3B, 0x1E, 0x4F, 0x00, 0x20,
    0xFC, 0xF3, 0x3B, 0x1E, 0x52, 0x00, 0x13,
    //0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x00,
    0xFC, 0xF3, 0x3B, 0x1E, 0x60, 0x00, 0x01,
    0xFC, 0xF3, 0x3B, 0x1E, 0x70, 0x05, 0xC0,
	0xFC, 0xF3, 0x3B, 0x1E, 0x3A, 0x00, 0x00,
};

#endif
