/***************************************************************************
 ** File            :   
 ** 
 ** Description     :   
 **
 ** History
 **     Created     :   
 **     Modified    :
 **
 ** Notes           :
 **
 ** ToDo            :
 **
 ***************************************************************************/

#ifndef __HASH_H__
#define __HASH_H__

#include <stdint.h>

struct fs_hash_context{
	uint8_t *table;
	uint32_t hash[8];
	uint8_t hash_len;
	uint8_t ent_num;
};

#if 0
fsDEsH->table[0]=166;
fsDEsH->table[1]=223;
fsDEsH->table[2]=187;
fsDEsH->table[3]=192;
fsDEsH->table[4]=82;
fsDEsH->table[5]=109;
fsDEsH->table[6]=150;
fsDEsH->table[7]=218;
fsDEsH->table[8]=64;
fsDEsH->table[9]=42;
fsDEsH->table[10]=8;
fsDEsH->table[11]=207;
fsDEsH->table[12]=199;
fsDEsH->table[13]=164;
fsDEsH->table[14]=180;
fsDEsH->table[15]=193;
fsDEsH->table[16]=83;
fsDEsH->table[17]=34;
fsDEsH->table[18]=17;
fsDEsH->table[19]=91;
fsDEsH->table[20]=172;
fsDEsH->table[21]=221;
fsDEsH->table[22]=203;
fsDEsH->table[23]=172;
fsDEsH->table[24]=123;
fsDEsH->table[25]=109;
fsDEsH->table[26]=103;
fsDEsH->table[27]=53;
fsDEsH->table[28]=10;
fsDEsH->table[29]=130;
fsDEsH->table[30]=39;
fsDEsH->table[31]=64;
fsDEsH->table[32]=75;
fsDEsH->table[33]=212;
fsDEsH->table[34]=45;
fsDEsH->table[35]=116;
fsDEsH->table[36]=28;
fsDEsH->table[37]=93;
fsDEsH->table[38]=226;
fsDEsH->table[39]=167;
fsDEsH->table[40]=201;
fsDEsH->table[41]=197;
fsDEsH->table[42]=153;
fsDEsH->table[43]=243;
fsDEsH->table[44]=210;
fsDEsH->table[45]=30;
fsDEsH->table[46]=112;
fsDEsH->table[47]=12;
fsDEsH->table[48]=239;
fsDEsH->table[49]=209;
fsDEsH->table[50]=119;
fsDEsH->table[51]=208;
fsDEsH->table[52]=82;
fsDEsH->table[53]=216;
fsDEsH->table[54]=21;
fsDEsH->table[55]=32;
fsDEsH->table[56]=44;
fsDEsH->table[57]=46;
fsDEsH->table[58]=167;
fsDEsH->table[59]=239;
fsDEsH->table[60]=114;
fsDEsH->table[61]=132;
fsDEsH->table[62]=105;
fsDEsH->table[63]=152;
fsDEsH->table[64]=225;
fsDEsH->table[65]=54;
fsDEsH->table[66]=154;
fsDEsH->table[67]=117;
fsDEsH->table[68]=64;
fsDEsH->table[69]=125;
fsDEsH->table[70]=219;
fsDEsH->table[71]=173;
fsDEsH->table[72]=229;
fsDEsH->table[73]=230;
fsDEsH->table[74]=217;
fsDEsH->table[75]=77;
fsDEsH->table[76]=122;
fsDEsH->table[77]=3;
fsDEsH->table[78]=43;
fsDEsH->table[79]=155;
fsDEsH->table[80]=1;
fsDEsH->table[81]=96;
fsDEsH->table[82]=126;
fsDEsH->table[83]=167;
fsDEsH->table[84]=22;
fsDEsH->table[85]=184;
fsDEsH->table[86]=239;
fsDEsH->table[87]=34;
fsDEsH->table[88]=116;
fsDEsH->table[89]=105;
fsDEsH->table[90]=184;
fsDEsH->table[91]=115;
fsDEsH->table[92]=187;
fsDEsH->table[93]=38;
fsDEsH->table[94]=15;
fsDEsH->table[95]=8;
fsDEsH->table[96]=113;
fsDEsH->table[97]=236;
fsDEsH->table[98]=75;
fsDEsH->table[99]=234;
fsDEsH->table[100]=70;
fsDEsH->table[101]=55;
fsDEsH->table[102]=73;
fsDEsH->table[103]=149;
fsDEsH->table[104]=156;
fsDEsH->table[105]=117;
fsDEsH->table[106]=14;
fsDEsH->table[107]=6;
fsDEsH->table[108]=70;
fsDEsH->table[109]=187;
fsDEsH->table[110]=173;
fsDEsH->table[111]=21;
fsDEsH->table[112]=144;
fsDEsH->table[113]=183;
fsDEsH->table[114]=107;
fsDEsH->table[115]=5;
fsDEsH->table[116]=128;
fsDEsH->table[117]=230;
fsDEsH->table[118]=34;
fsDEsH->table[119]=90;
fsDEsH->table[120]=92;
fsDEsH->table[121]=6;
fsDEsH->table[122]=227;
fsDEsH->table[123]=234;
fsDEsH->table[124]=108;
fsDEsH->table[125]=207;
fsDEsH->table[126]=223;
fsDEsH->table[127]=55;
fsDEsH->table[128]=1;
fsDEsH->table[129]=222;
fsDEsH->table[130]=136;
fsDEsH->table[131]=252;
fsDEsH->table[132]=183;
fsDEsH->table[133]=242;
fsDEsH->table[134]=243;
fsDEsH->table[135]=6;
fsDEsH->table[136]=246;
fsDEsH->table[137]=90;
fsDEsH->table[138]=129;
fsDEsH->table[139]=69;
fsDEsH->table[140]=191;
fsDEsH->table[141]=174;
fsDEsH->table[142]=190;
fsDEsH->table[143]=35;
fsDEsH->table[144]=165;
fsDEsH->table[145]=190;
fsDEsH->table[146]=137;
fsDEsH->table[147]=20;
fsDEsH->table[148]=25;
fsDEsH->table[149]=200;
fsDEsH->table[150]=118;
fsDEsH->table[151]=111;
fsDEsH->table[152]=235;
fsDEsH->table[153]=238;
fsDEsH->table[154]=112;
fsDEsH->table[155]=124;
fsDEsH->table[156]=14;
fsDEsH->table[157]=230;
fsDEsH->table[158]=163;
fsDEsH->table[159]=206;
fsDEsH->table[160]=156;
fsDEsH->table[161]=244;
fsDEsH->table[162]=153;
fsDEsH->table[163]=211;
fsDEsH->table[164]=25;
fsDEsH->table[165]=21;
fsDEsH->table[166]=162;
fsDEsH->table[167]=169;
fsDEsH->table[168]=252;
fsDEsH->table[169]=125;
fsDEsH->table[170]=122;
fsDEsH->table[171]=51;
fsDEsH->table[172]=107;
fsDEsH->table[173]=68;
fsDEsH->table[174]=38;
fsDEsH->table[175]=109;
fsDEsH->table[176]=71;
fsDEsH->table[177]=93;
fsDEsH->table[178]=31;
fsDEsH->table[179]=252;
fsDEsH->table[180]=104;
fsDEsH->table[181]=199;
fsDEsH->table[182]=178;
fsDEsH->table[183]=10;
fsDEsH->table[184]=42;
fsDEsH->table[185]=7;
fsDEsH->table[186]=167;
fsDEsH->table[187]=80;
fsDEsH->table[188]=40;
fsDEsH->table[189]=213;
fsDEsH->table[190]=33;
fsDEsH->table[191]=116;
fsDEsH->table[192]=71;
fsDEsH->table[193]=22;
fsDEsH->table[194]=197;
fsDEsH->table[195]=150;
fsDEsH->table[196]=247;
fsDEsH->table[197]=10;
fsDEsH->table[198]=30;
fsDEsH->table[199]=38;
fsDEsH->table[200]=181;
fsDEsH->table[201]=199;
fsDEsH->table[202]=64;
fsDEsH->table[203]=248;
fsDEsH->table[204]=212;
fsDEsH->table[205]=230;
fsDEsH->table[206]=172;
fsDEsH->table[207]=154;
fsDEsH->table[208]=126;
fsDEsH->table[209]=124;
fsDEsH->table[210]=116;
fsDEsH->table[211]=229;
fsDEsH->table[212]=246;
fsDEsH->table[213]=76;
fsDEsH->table[214]=160;
fsDEsH->table[215]=210;
fsDEsH->table[216]=31;
fsDEsH->table[217]=58;
fsDEsH->table[218]=207;
fsDEsH->table[219]=143;
fsDEsH->table[220]=66;
fsDEsH->table[221]=3;
fsDEsH->table[222]=34;
fsDEsH->table[223]=210;
fsDEsH->table[224]=12;
fsDEsH->table[225]=44;
fsDEsH->table[226]=85;
fsDEsH->table[227]=110;
fsDEsH->table[228]=214;
fsDEsH->table[229]=56;
fsDEsH->table[230]=47;
fsDEsH->table[231]=36;
fsDEsH->table[232]=42;
fsDEsH->table[233]=30;
fsDEsH->table[234]=29;
fsDEsH->table[235]=187;
fsDEsH->table[236]=130;
fsDEsH->table[237]=250;
fsDEsH->table[238]=26;
fsDEsH->table[239]=82;
fsDEsH->table[240]=82;
fsDEsH->table[241]=0;
fsDEsH->table[242]=209;
fsDEsH->table[243]=246;
fsDEsH->table[244]=73;
fsDEsH->table[245]=189;
fsDEsH->table[246]=6;
fsDEsH->table[247]=112;
fsDEsH->table[248]=212;
fsDEsH->table[249]=112;
fsDEsH->table[250]=50;
fsDEsH->table[251]=96;
fsDEsH->table[252]=229;
fsDEsH->table[253]=216;
fsDEsH->table[254]=109;
fsDEsH->table[255]=144;
#endif //1

void fs_hash_init (struct fs_hash_context *fsDEsH);
 
void fs_hash_step (uint32_t next, struct fs_hash_context *fsDEsH);
 
uint32_t fs_hash_modify (uint32_t a, uint32_t b, struct fs_hash_context *fsDEsH);

uint32_t fs_hash_rotate (uint32_t a);
 
void fs_hash_calc (uint32_t *data, size_t data_len, struct fs_hash_context *fsDEsH);

#endif //__HASH_H__