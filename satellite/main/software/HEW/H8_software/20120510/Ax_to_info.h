void Ax_to_info(unsigned char *,int );
void crcbit (unsigned char);/*FCS計算用関数*/
void precrc_bit(unsigned char);/*１６進数を２進数に変換する関数*/
unsigned char  fcslo, fcshi;/*FCS計算する変数*/
int StrLen(const char *);/*配列の長さを取得する関数*/
int info_length;/*infoの文字列の長さ*/
static unsigned char *info;/*infoの文字列がこの変数列に記録される*/
