#define ANTENNAWAITTIME		4000
#define ANTENNAEXPANDTIME	2400 //現在約2分
#define ANTENNAINTERVALTIME 4 //約4時間
#define EXPANDANTENNAON		175//本番は-5度(175)
#define SUNSHINECUR			20

int16_t antennacount;
int8_t  antennaminute;
int8_t	antennahour;

short antennaExpandflag;//shortから変更

void wait_AntennaExpand(void);
void antennaInit(void);
void interval_AntennaExpand(void);
void antennaExpander(void);
void AEerrCheck(void);