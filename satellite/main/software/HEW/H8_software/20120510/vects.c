/***********************************************************************/
/*                                                                     */
/*  FILE        :vects.c                                               */
/*  DATE        :Wed, May 09, 2012                                     */
/*  DESCRIPTION :Vector Table                                          */
/*  CPU TYPE    :H8/3069R                                              */
/*                                                                     */
/*  This file is generated by KPIT GNU Project Generator.              */
/*                                                                     */
/***********************************************************************/
     



#include    "inthandler.h"
typedef  void (*fp) (void);
extern void start(void);
extern void stack (void);

#define VECT_SECT          __attribute__ ((section (".vects")))
const void *HardwareVectors[] VECT_SECT  = {
//;<<VECTOR DATA START (POWER ON RESET)>>
//;0 Power On Reset PC
start,
//;<<VECTOR DATA END (POWER ON RESET)>>
//  vector 1 Reserved
(fp)0,
//  vector 2 Reserved
(fp)0,
//  vector 3 Reserved
(fp)0,
//  vector 4 Reserved
(fp)0,
//  vector 5 Reserved
(fp)0,
//  vector 6 Reserved
(fp)0,
//  vector 7 NMI
 INT_NMI,
//  vector 8 TRAP
 INT_TRAP1,
//  vector 9 TRAP
 INT_TRAP2,
//  vector 10 TRAP
 INT_TRAP3,
//  vector 11 TRAP
 INT_TRAP4,
//  vector 12 IRQ0
 INT_IRQ0,
//  vector 13 IRQ1
 INT_IRQ1,
//  vector 14 IRQ2
 INT_IRQ2,
//  vector 15 IRQ3
 INT_IRQ3,
//  vector 16 IRQ4
 INT_IRQ4,
//  vector 17 IRQ5
 INT_IRQ5,
//  vector 18 Reserved
(fp)0,
//  vector 19 Reserved
(fp)0,
//  vector 20 WOVI
 INT_WOVI,
//  vector 21 CMI
 INT_CMI,
//  vector 22 Reserved
(fp)0,
//  vector 23 ADI
 INT_ADI,
//  vector 24 IMIA0
 INT_IMIA0,
//  vector 25 IMIB0
 INT_IMIB0,
//  vector 26 OVI0
 INT_OVI0,
//  vector 27 Reserved
(fp)0,
//  vector 28 IMIA1
 INT_IMIA1,
//  vector 29 IMIB1
 INT_IMIB1,
//  vector 30 OVI1
 INT_OVI1,
//  vector 31 Reserved
(fp)0,
//  vector 32 IMIA2
 INT_IMIA2,
//  vector 33 IMIB2
 INT_IMIB2,
//  vector 34 OVI2
 INT_OVI2,
//  vector 35 Reserved
(fp)0,
//  vector 36 CMIA0
 INT_CMIA0,
//  vector 37 CMIB0
 INT_CMIB0,
//  vector 38 CMIA1B1
 INT_CMIA1B1,
//  vector 39 TOVI0_1
 INT_TOVI0_1,
//  vector 40 CMIA2
 INT_CMIA2,
//  vector 41 CMIB2
 INT_CMIB2,
//  vector 42 CMIA3B3
 INT_CMIA3B3,
//  vector 43 TOVI2_3
 INT_TOVI2_3,
//  vector 44 DEND0A
 INT_DEND0A,
//  vector 45 DEND0B
 INT_DEND0B,
//  vector 46 DEND1A
 INT_DEND1A,
//  vector 47 DEND1B
 INT_DEND1B,
//  vector 48 Reserved
(fp)0,
//  vector 49 Reserved
(fp)0,
//  vector 50 Reserved
(fp)0,
//  vector 51 Reserved
(fp)0,
//  vector 52 ERI0
 INT_ERI0,
//  vector 53 RXI0
 INT_RXI0,
//  vector 54 TXI0
 INT_TXI0,
//  vector 55 TEI0
 INT_TEI0,
//  vector 56 ERI1
 INT_ERI1,
//  vector 57 RXI1
 INT_RXI1,
//  vector 58 TXI1
 INT_TXI1,
//  vector 59 TEI1
 INT_TEI1,
//  vector 60 ERI2
 INT_ERI2,
//  vector 61 RXI2
 INT_RXI2,
//  vector 62 TXI2
 INT_TXI2,
//  vector 63 TEI2
 INT_TEI2,


};