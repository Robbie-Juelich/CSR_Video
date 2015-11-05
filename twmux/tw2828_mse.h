
#ifndef _MSE_H_
#define _MSE_H_

#ifdef __cplusplus
 extern "C" {
#endif 

//==================================================================================
//						Mouse function description
//==================================================================================
extern u16 InitMse(u8 id);
extern void MnMouseEnable(u32);
extern void MnMouseDisable(u32);
extern u8 ChangeMouseNumber(u8 no);

void MnChangeMseImg(u8 no, u8 index);

int MseTest(int argc, char **argv);

#ifdef __cplusplus
 }
#endif 

#endif
