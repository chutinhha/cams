/***************************************************************************
 ** File            :   fsDEsRandom.c
 ** 
 ** Description     :   random number generation utility
 **
 ** History
 **     Created     :   Nov 5 2012
 **     Modified    :	Nov 15 2012
 **
 ** Notes           :
 **
 ** ToDo            :
 **
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "fsDEsHash.h"
#include "fsDEsRandom.h"

uint32_t fsDEsRandom_state=0xf6a90b51;

//-----------------------------------------------------------------------------------
// Function     :   fs_srandom
// Description  :   generates random seed parameter value
// Arguments    :   @ seed  -   seed value supplied by the user
//              :   
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
void fs_srandom(uint32_t seed)
 {
	fs_random_32();
	fsDEsRandom_state ^= seed;
	fs_random_32();
}

//-----------------------------------------------------------------------------------
// Function     :   fs_random_32
// Description  :   generates a 32 bit random number value
// Arguments    :   none
//              :   
// return       :   random number generated value
// notes        :   
//-----------------------------------------------------------------------------------
uint32_t fs_random_32()
{
	uint32_t i;
    struct fs_hash_context fsDEsH;
	fs_hash_init(&fsDEsH);
	//uint32_t i;
	fs_hash_step(fsDEsRandom_state, &fsDEsH);
	fs_hash_step(time(NULL)+clock(), &fsDEsH);
    
	for(i=0;i<8; i++)
		fs_hash_step((i<<20)+fsDEsRandom_state, &fsDEsH);
	
    fsDEsRandom_state=fsDEsH.hash[0];
	fs_hash_step(fsDEsRandom_state, &fsDEsH);
	
    return fsDEsH.hash[0]^fsDEsH.hash[1];
}

//-----------------------------------------------------------------------------------
// Function     :   fs_random_64
// Description  :   generates a 64 bit random number value
// Arguments    :   none
//              :   
// return       :   random number generated value
// notes        :   
//-----------------------------------------------------------------------------------
uint64_t fs_random_64()
{
	uint64_t tmp;
   	uint32_t i;
	struct fs_hash_context fsDEsH;
	fs_hash_init(&fsDEsH);
	//uint32_t i;
	fs_hash_step(fsDEsRandom_state, &fsDEsH);
	fs_hash_step(time(NULL)+clock(), &fsDEsH);
	
    for(i=1;i<9; i++)
		fs_hash_step((i<<20)+fsDEsRandom_state, &fsDEsH);
	
    fsDEsRandom_state=fsDEsH.hash[0];
	fs_hash_step(fsDEsRandom_state, &fsDEsH);
	tmp=fsDEsH.hash[0];
	tmp=tmp<<32;
	tmp+=fsDEsH.hash[1];
	
    return tmp;
}
