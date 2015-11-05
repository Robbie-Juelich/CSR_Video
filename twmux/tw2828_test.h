#ifndef _TW2828_TEST_H
#define _TW2828_TEST_H

#include "tw2828_twmux.h"
#include "test_disp.h"
#include "test_spot.h"
#include "test_osd.h"


typedef struct {
    char *CmdStr;
        int (*func)(int argc, char **argv);
    char *DocLine;
} TCommand;

int TestDcmGateCalibration(int argc, char **argv);
int MainDisplayTest(int argc, char **argv);
int MseTest(int argc, char **argv);

void GetIOInfoData (int no);
void InitRecforMdsp(u8 id, u8 mode);
void InitSpot_4_HD_Rotate(u8 no, u8 p, u8 rp, u8 buf_sel);


#endif
