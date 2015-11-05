
#include "tw2828_hostio.h"

#define REG_FPGA_SELECT 0x1002

void tw2828_select_bt1120(void)
{
	WriteReg(REG_FPGA_SELECT, 0x0);
}

void tw2828_select_bt656(void)
{
	WriteReg(REG_FPGA_SELECT, 0x1);
	//WriteReg(REG_FPGA_SELECT, 0x0);
}

void tw2828_select_bt656_2(void)
{
	WriteReg(REG_FPGA_SELECT, 0x2);
	//WriteReg(REG_FPGA_SELECT, 0x0);
}
