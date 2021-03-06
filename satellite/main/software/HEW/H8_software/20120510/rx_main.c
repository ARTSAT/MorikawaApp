/***********************************************************************/
/*                                                                     */
/*  FILE        :Rx_3069.c                                             */
/*  DATE        :Mon, May 27, 2013                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :H8/3069R                                              */
/*                                                                     */
/*  This file is generated by KPIT GNU Project Generator.              */
/*                                                                     */
/*                                                                     */
/*  RX単体のテストの時にだけ用いるようにする                           */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
         
#include"Rx_protcol.h"
#include "iodefine.h"
//#include"rx_pll_inv.c"
//#include"rx_protcol_inv.c"
#include "intrinsic.h"
#include"sci.h"



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


int main(void)												//メイン関数開始　み
{
															//  TODO: add application code here
    int errint;												//データ受信＆デコード時のエラー検出に使う。　み
//    int oldrxd = 0;
//    int oldtcnt= 0;
 //   int newtcnt= 0;

    set_imask_ccr(1);										//割込処理を禁止。intrinsic.hで定義 み	//これをしないとなぜか不具合がでる
    debug_sci_init();										//sci1_initの初期化（19200bps,8bit,non parity, 1 stop bit）。sci_util.cで定義　み
    set_imask_ccr(0);										//割込処理を許可。intrinsic.hで定義　み
    debug_puts("Hello. fine? Start of Rx test Program\r\n");		//()内の文字列を送信。sci_util.c(debug_puts関数)とsci1.c(sci1_puts関数)で定義　み
    delay_ms(300);											//300ms待つ
    rx_init();
	P5DDR = 0xff;											//初期化関数。rx_protocol_inv.cで定義　み
//      ITU0.TCR.BYTE=0x03;
//      ITU.TSTR.BIT.STR0=1;

//	while(1){
//		if(!FX_RXD)debug_puts("GND\n");
//	}
    while (1) {												//無限ループ　み
		debug_puts("*");
		if(FX_DET != 0){
			errint = get_data();								/*データの受信とデコードを行う。errint = 0ならば正常受信、1ならばプリアンブル検出エラー。　み*/
															/*rx_protocol_inv.cで定義																   み*/
    
			//while(FX_RXD==oldrxd);
			//oldrxd=FX_RXD;
			//newtcnt=ITU0.TCNT;
			//debug_putdec(newtcnt-oldtcnt);
			//debug_putc(0x20);
			//oldtcnt=newtcnt;

			if(errint==0){										//正常にデータが取れた場合　み
				debug_puts("Getting Data is Succeeded.\r\n");	//（）内の文字列を送信　み
				delay_ms(300);									//300ms待つ	み
				debug_puts(uplink_rx_buff);						//配列uplink_rx_buffに格納された文字列を送信。配列は147で1パケット、comm_buffer.cで定義　み
				free(uplink_rx_buff);							//uplink_rx_buffで確保した領域を離す　み
      			debug_puts("\r\n");
      			debug_putdec(ITU0.GRB);
				return 0;										//正常時の処理おしまい　み
			}
			else{												//エラー検出の場合　み
				//char errstr[12];								//エラー表示用配列定義　み
				//sprintf(errstr,"Error Code:%d\r\n",errint);		//エラーコードerrintの値をerrstrに格納　み
				//debug_puts("Getting Data is Failed.\r\n");		//（）内の文字列を送信　み
				//debug_puts(errstr);								//格納されたエラーコードerrintの値とエラーメッセージを送信　み
				free(uplink_rx_buff);							//uplink_rx_buffで確保した領域を離す　み
			}													//エラー検出時の処理おしまい　み
    
			//delay_ms(900);										//900ms待つ み
    	}
	}
  	return 0;
}															//メイン関数おしまい　み


