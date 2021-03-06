/* =============================================================================== */
/* 															                       */
/* 				H8 Timer Program for INVADER BBM					               */
/* 															                       */
/* 										Written by Shota Kikuchi			       */
/* 										20012.02.10                                */
/* 	                                                                               */
/* 										Based on "timer.c" for PRISM			   */
/* 															                       */
/* =============================================================================== */
#ifndef __TIMER_C__
#define __TIMER_C__

#include "iodefine.h"
#include "timer.h"
#include "inthandler.h"
#include "sci_util.h"
#include "intrinsic.h"

volatile unsigned int time;
volatile unsigned int timer_flag = 0;

void init_timer(void)
{
	ITU0.TCR.BIT.CCLR = 3;
	ITU.TSNC.BIT.SYNC0 = 0;
    ITU0.TCR.BIT.TPSC = 0x03;		//クロックを1/8 (2MHz)に設定　→　周期:0.5usec
	ITU0.TCR.BIT.CCLR = 2;			//デフォルトではGRBを用いる
    ITU0.TIOR.BYTE = 0x88;
    ITU0.TIER.BIT.IMIEA = 0x00;		//割り込み禁止
	ITU0.TIER.BIT.IMIEB = 0x00;
    ITU.TSTR.BIT.STR0 = 0;			//ITU0 TCNTカウントストップ

}


/* ------------------------------- */
/*   ITU0によりt[msec]待機         */
/* ------------------------------- */

/*
void delay_ms(unsigned int t)
{
    unsigned int i = 0;
	
	ITU0.GRA = 2000*t-1;			//t[msec]
    ITU0.TCR.BIT.CCLR = 0x01;		//GRAを用いる
	ITU0.TCNT = 0;
//	ITU0.TIER.BIT.IMIEA = 0x01;
    ITU.TSTR.BIT.STR0 = 1;			//ITU0 TCNTカウントスタート
    
    while(i < t) {
		//IMFAフラグをチェック
        if(ITU0.TSR.BIT.IMFA) {
            ITU0.TSR.BIT.IMFA = 0;		//IMFAフラグをクリア
            i++;
        }
    }
    ITU.TSTR.BIT.STR0 = 0;			//ITU0 TCNTカウントストップ
	ITU0.TIER.BIT.IMIEA = 0x00;
}
*/

void delay_ms(unsigned int t)
{	
//	timer_flag = 0;				//早めに0にしないと、割り込み関数が先に呼ばれてしまってwhileから抜けられない
	ITU0.GRA = 2000*t-1;		//t[msec]
	ITU0.TCR.BIT.CCLR = 1;		//GRAを用いる
	ITU0.TIER.BIT.IMIEA = 1;	//IMFAフラグによる割り込み許可
	ITU.TSTR.BIT.STR0 = 1;		//ITU0 TCNTカウントスタート
	
	while(!timer_flag);
	 
	timer_flag = 0;
	ITU.TSTR.BIT.STR0 = 0;		//ITU0 TCNTカウントストップ
	ITU0.TCR.BIT.CCLR = 2;		//デフォルトではGRBを用いる
}

void INT_IMIA0(void) {
	ITU0.TSR.BIT.IMFA = 0;			//割り込みステータスフラグクリア
	ITU0.TIER.BIT.IMIEA = 0x00;		//IMFAフラグによる割り込み禁止
	timer_flag = 1;					//while文から抜けるためのフラグを立てる
}	



/* ------------------------------- */
/*   ITU0によりt[usec]待機         */
/* ------------------------------- */


void delay_us(unsigned int t)
{	
	ITU0.GRB = 2*t-1;			//t[usec]
	ITU.TSTR.BIT.STR0 = 1;		//ITU0 TCNTカウントスタート
	ITU0.TIER.BIT.IMIEB = 1;	//IMFBフラグによる割り込み許可
	
	while(!timer_flag);
	 
	timer_flag = 0;
	ITU.TSTR.BIT.STR0 = 0;		//ITU0 TCNTカウントストップ
}


void INT_IMIB0(void) {
	ITU0.TSR.BIT.IMFB = 0;			//割り込みステータスフラグクリア
	ITU0.TIER.BIT.IMIEB = 0x00;		//IMFBフラグによる割り込み禁止
	timer_flag = 1;					//while文から抜けるためのフラグを立てる
}	




#endif
