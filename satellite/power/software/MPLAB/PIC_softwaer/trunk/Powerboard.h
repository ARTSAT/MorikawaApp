#ifndef _POWERBOARD_H_
#define _POWERBOARD_H_  
#define _DEBUGf_
//#define _DEBUG_
//#define _ANTEXPNDTEST_

//�����[�X���̓R�����g�A�E�g�Ŏ��O���܂��傤
#include "16F877.h"

#device ADC=8
#include "stdint.h"
#include "string.h"

#fuses HS,WDT,NOBROWNOUT
#use delay(CLOCK=20000000)
#use rs232(baud = 19200, xmit = PIN_C6, rcv = PIN_C7,ERRORS)

#ROM 0x2100={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
//�A���e�i�W�J�����߂����Ȃ�0x7f��擪��3��������
#ROM 0x2110={0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x2120={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x2130={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x2140={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x2150={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x2160={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
//60��0x7f����荞�ނ��Ƃ�ADC��,61�ɕ��荞�ނ��Ƃɂ����UART�����ꂼ��g�p�ł��Ȃ��Ȃ�
#ROM 0x2170={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x2180={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x2190={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x21A0={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x21B0={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x21C0={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x21D0={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x21E0={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#ROM 0x21F0={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}

#include "PinAssign.h"
#include "TaskControll.h"
#include "watchCurrent.h"
#include "ADC.h"
#include "controlCW.h"
#include "controlBatteryHeater.h"
#include "LiveSign.h"
#include "ResetWorks.h"
#include "PowerOperating.h"
#include "expandAntenna.h"
#include "Communication.h"
#include "getPowerData.h"
#include "watchBatteryVOL.h"

//���[�v�p
int8_t i;//CW,�d���Ď��Ŏg�p
int8_t j;//CW


#endif