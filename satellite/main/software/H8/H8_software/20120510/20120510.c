/***********************************************************************/
/*                                                                     */
/*  FILE        :20120510.c                                            */
/*  DATE        :Wed, May 09, 2012                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :H8/3069R                                              */
/*                                                                     */
/*  This file is generated by KPIT GNU Project Generator.              */
/*                                                                     */
/***********************************************************************/

/*************************************************************************************
* ファイル名　：　20120510.c
* タイトル　　：　INVADERメインプログラム
* 製作者　　　：　03-110317 木村元紀
* 所属　　　　：　東京大学工学部航空宇宙工学科
* 製作日　　　：　2012.5.10
* 言語　　　　：　C
**************************************************************************************
* NOTE *
* 使用CPUをH8/3048FからH8/3069Fへと変更。
* 
*************************************************************************************/         
#include "iodefine.h"
#include "intrinsic.h"
#include "global.h"
#include "init.h"
#include "sci.h"
#include "itu1.h"
#include "message.h"
#include "adconv.h"
#include "pwr.h"
#include "mission.h"
#include "teleme.h"
#include "cdh_pindefine.h"
#include "Rx_protcol.h"
#include "fm_tx.h"

#ifdef CPPAPP
//Initialize global constructors
extern "C" void __main()
{
  static int initialized;
  if (! initialized)
    {
      typedef void (*pfunc) ();
      extern pfunc __ctors[];
      extern pfunc __ctors_end[];
      pfunc *p;

      initialized = 1;
      for (p = __ctors_end; p > __ctors; )
	(*--p) ();

    }
}
#endif 

void main(void)
{
	unsigned char infunc = 0;			// 内部関数初期化フラグ
	unsigned char bus = 0;				// 内部レジスタ初期化フラグ
	unsigned char peripherals = 0;		// デバイス初期化フラグ
	
	set_imask_ccr(1);			// 割り込み禁止 
	set_imask_ccr(0);			// 割り込み許可
	  
	infunc = Init_MyFunction();
	bus = Init_Bus();
	peripherals = Init_Peripherals();
	
	debug_puts((unsigned char*)("\n\n"));
	debug_puts("================================================\n");
	debug_puts((unsigned char*)("	Hello, this is INVADER FM... Ver.20140118 \n"));
	debug_puts("================================================");
	debug_puts((unsigned char*)("\n\n"));
	
	// 初期化状況の出力
	debug_puts((unsigned char*)("**********  status of initialization  **********\n"));
	debug_puts((unsigned char*)("flag_infunc:"));
	debug_puthex(infunc);
	debug_puts((unsigned char*)("\n"));
	debug_puts((unsigned char*)("flag_bus:"));
	debug_puthex(bus);
	debug_puts((unsigned char*)("\n"));
	debug_puts((unsigned char*)("flag_peripherals:"));
	debug_puthex(peripherals);
	debug_puts((unsigned char*)("\n"));
	
	status.init[0] = infunc;
	status.init[1] = bus;
	status.init[2] = peripherals; 
    
    Read_Initial();
//	rx_init();
//	smr_time=0;		//ミッションCPU起動(タイマ予約型)のための変数。どっかの関数に入れたい。
	
	while(1)
	{
		// 自己WDT初期化(ITU1内でインクリメントされる)
		my_wdt = 0;
		
		//生存信号
		if(CheckFlags(ena_alive) && CheckFlags(flag_alive)){
			APLS = ~APLS;
			ClearFlags(flag_alive);
		}
		
		// 受信する
		if(CheckFlags(flag_rx)){
			//itu0_init();
			//itu0_start();
			RX();
			ClearFlags(flag_rx);
			//itu0_stop();
			//itu0_display();
		}
		
		// SCI0,SCI1,SCI2からコマンドを探す
		if(CheckFlags(flag_scirx)){
			//itu0_init();
			//itu0_start();
			FindCommand();
			ClearFlags(flag_scirx);
			//itu0_stop();
			//itu0_display();
		}
		
		//コマンド実行
		if(CheckFlags(flag_exe)) {
			//itu0_init();
			//itu0_start();
			ExecuteCommand();
			ClearFlags(flag_exe);
			//itu0_stop();
			//itu0_display();
		}
		
		// 電源情報要求（to PIC）
		if(CheckFlags(ena_pwrt) && CheckFlags(flag_pwrt)){
			//itu0_init();
			//itu0_start();
			RequestPowerTelemetry();
			//debug_puts("Request Power.\n");
			ClearFlags(flag_pwrt);
			//itu0_stop();
			//itu0_display();
		}
		
		// センサ情報送信（to Arduino）
		if(CheckFlags(ena_misst) && CheckFlags(flag_misst)){
			//itu0_init();
			//itu0_start();
			SendMissionTelemetry();
			ClearFlags(flag_misst);
			//itu0_stop();
			//itu0_display();
		}
		
		// AD変換
		if(CheckFlags(ena_ad) && CheckFlags(flag_ad)){
			//itu0_init();
			//itu0_start();
			get_sensor();
			ClearFlags(flag_ad);
			//itu0_stop();
			//itu0_display();
		}
		
		// AD変換結果表示
		if(CheckFlags(ena_dispAD) && CheckFlags(flag_dispAD)){
			//itu0_init();
			//itu0_start();
			disp_AD();
			ClearFlags(flag_dispAD);
			//itu0_stop();
			//itu0_display();
		}
		
		
		//粗履歴
		if(CheckFlags(ena_rteleme) && CheckFlags(flag_rteleme)){
			//itu0_init();
			//itu0_start();
			SetTelemetry_Rough();
			ClearFlags(flag_rteleme);
			//itu0_stop();
			//itu0_display();
		}
		
		//詳細履歴
		if(CheckFlags(ena_dteleme) && CheckFlags(flag_dteleme)){
			//itu0_init();
			//itu0_start();
			SetTelemetry_Detail(1);
			ClearFlags(flag_dteleme);
			//itu0_stop();
			//itu0_display();
		}
		
		//API配信
		if(CheckFlags(ena_ateleme) && CheckFlags(flag_ateleme)){
			//itu0_init();
			//itu0_start();
			SetTelemetry_API();
			ClearFlags(flag_ateleme);
			//itu0_stop();
			//itu0_display();
		}
		
		//渦電流検知
		if(CheckFlags(ena_overa) && CheckFlags(flag_overa)){
			//itu0_init();
			//itu0_start();
			check_overAmpare();
			ClearFlags(flag_overa);
			//itu0_stop();
			//itu0_display();
		}
		
		//初期化情報書き込み
		if(CheckFlags(ena_iteleme) && CheckFlags(flag_iteleme)){
			//itu0_init();
			//itu0_start();
			SetTelemetry_Initial();
			ClearFlags(flag_iteleme);
			//itu0_stop();
			//itu0_display();
		}
		
		//試験用テレメトリ出力
		if(CheckFlags(ena_oteleme) && CheckFlags(flag_oteleme)){
			//itu0_init();
			//itu0_start();
			debug_puts("Test Telemetry Output.\n");
			DebugPut_Status();
			ClearFlags(flag_oteleme);
			//itu0_stop();
			//itu0_display();
		}
		//パワーから返事が返って来ているかの確認
		if(CheckFlags(flag_cpr)){
			//itu0_init();
			//itu0_start();
			CheckPowerReply();
			ClearFlags(flag_cpr);
			//itu0_stop();
			//itu0_display();
		}
			
		// ミッションCPU起動(タイマ予約型、タイマ予約&作動時間指定型)
		if(CheckFlags(flag_smr)){
			//itu0_init();
			//itu0_start();
			RequestMissionReboot();
		
			smr_time=0;
			ClearFlags(flag_smr);
			//itu0_stop();
			//itu0_display();
						
			//debug_puts("\n itu1_cnt time: ");
			//debug_putdec(itu1_cnt);
			//debug_puts("\n smr_time time: ");
			//debug_putdec(smr_time);
			//debug_puts("\n");
		}
		// ミッションCPUシャットダウン(タイマ予約&作動時間指定型)
		if(CheckFlags(flag_sms)){
			//itu0_init();
			//itu0_start();
			
			//sci1_puts_rx("c-c-c-nsd \r\n",12);
			PronounceMissionAbort();
			
			sms_time=0;
			ClearFlags(flag_sms);
			
			//itu0_stop();
			//itu0_display();
						
			//debug_puts("\n itu1_cnt time: ");
			//debug_putdec(itu1_cnt);
			//debug_puts("\n sms_time time: ");
			//debug_putdec(sms_time);
			//debug_puts("\n");
		}
		//FM送信とそれに関するバッファーチェック
		if(CheckFlags(flag_tx)){				
			//itu0_init();
			//itu0_start();
			TX();
			ClearFlags(flag_tx);
			//itu0_stop();
			//itu0_display();			
		}
		//PICオン要求
		if(CheckFlags(flag_pic)){				
			//itu0_init();
			//itu0_start();
			RequestPowerReboot();
			ClearFlags(flag_pic);
			//itu0_stop();
			//itu0_display();			
		}
    }
}
