/*************************************************************************************
* ファイル名　：　itu0-tasktime.c
* タイトル　　：　INVADERタイマ(CH0)プログラム
* 製作者　　　：　03-120495 西山万里
* 所属　　　　：　東京大学工学部電気電子工学科
* 製作日　　　：　2012.6.4
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* H8内部タイマ(ITU-CH0)
* intimia1(): 10msec周期(100Hz)で割り込み処理を発生。
* H8外部クロックとしては25MHzを前提。
* 
*************************************************************************************/
#include "itu0-tasktime.h"
#include "iodefine.h"
#include "inthandler.h"


static volatile unsigned long itu0_cnt;    // 10usecの割り込み毎にインクリメント
static volatile unsigned long my_wdt; 	          // 10usecの割り込み毎にインクリメント mainloopを一回終えるとリセット

/*************************************************
            ITU0初期化関数
--------------------------------------------------
* H8外部クロック25MHz
* GRAとのコンペアマッチでカウントクリア
*
*************************************************/
unsigned char itu0_init(void)
{

	ITU.TSTR.BIT.STR0 = 0;                // カウント停止
	
	ITU0.TCR.BIT.CCLR = 1;                // GRAのコンペアマッチでカウントクリア
	ITU0.TCR.BIT.TPSC = 0;                // タイマプリスケーラ 25MHz/1=25MHz T=0.04[usec]
	ITU0.GRA = 50-1;                   // 周期2us

	itu0_cnt = 0;
	my_wdt = 0;
	
	ITU.TISRA.BIT.IMFA0 = 0;                // 割り込みステータスフラグクリア
	ITU.TISRA.BIT.IMIEA0 = 1;              // タイマ割り込み許可
	
	return 1;
}

/*************************************************
            ITU0スタート関数
*************************************************/
unsigned char itu0_start(void)
{
	
	ITU0.TCNT = 0;                        // カウントクリア
	ITU.TSTR.BIT.STR0 = 1;                // カウント開始
	
	return 1;
}

/*************************************************
            ITU0ストップ関数
*************************************************/
unsigned char itu0_stop(void)
{
	ITU.TSTR.BIT.STR0 = 0;				//カウント終了
	
	return 1;
}

/*************************************************
            ITU0同期割り込み関数
*************************************************/
void INT_IMIA0(void)
{
	itu0_cnt++;
	ITU.TISRA.BIT.IMFA0 = 0;                // 割り込みステータスフラグクリア
}

/*************************************************
            ITU0結果表示関数
*************************************************/
unsigned char itu0_display(void)
{
	int time = itu0_cnt*2;
	debug_puts("time = ");
	debug_putdec(time);
	debug_puts("us\n");
	itu0_cnt = 0;					//カウントクリア
	
	return 1;
}