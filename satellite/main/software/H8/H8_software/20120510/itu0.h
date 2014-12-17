/*************************************************************************************
* ファイル名　：　itu1.h
* タイトル　　：　INVADERタイマ(CH1)ヘッダファイル
* 製作者　　　：　03-110351 三好賢聖　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.1.30
* 言語　　　　：　C
**************************************************************************************
* ※備考※
* H8内部タイマ(ITU-CH1)
* intimia0(): 10msec周期(100Hz)で割り込み処理を発生。
* H8外部クロックとしては25MHzを前提。
* 
*************************************************************************************/

#ifndef ITU0_H_
#define ITU0_H_

unsigned char itu0_init(void);

#endif
