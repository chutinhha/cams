/***************************************************************************
 ** File            :   fsDEsCli.c
 ** 
 ** Description     :   command line utility functions
 **
 ** History
 **     Created     :   Nov 5  2012
 **     Modified    :	Nov 15 2012
 **
 ** Notes           :
 **
 ** ToDo            :
 **
 ***************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsDEsLibum.h"

#ifdef FSEDS
#include "fsDEsPLatformSpDfns.h"
#endif

#define FULL_NAME "Fast & Secure Decryption Encryption System"
#define SHORT_NAME "fsDEscli"
#define FULL_VERSION "1.0.0"

#define SPEED_TEST 0

#define I_FLAG 1
#define K_FLAG 2
#define P_FLAG 3
#define FSIZE 4

#ifdef FSEDS
//-----------------------------------------------------------------------------------
// Function     :   print_header
// Description  :   display the product information
// Arguments    :   none
//              :   
// return       :   void
// notes        :   
//-----------------------------------------------------------------------------------
void print_header()
{
    CLEAR
    printf ("###############################################################\n");
    printf ("######### Fast & Secure Decryption Encryption System ##########\n");
    printf ("###############################################################\n");
    printf ("\n");
}	
#endif

void print_version()
{
#ifdef FSEDS 
#else
	printf("%s %s\n", FULL_NAME, FULL_VERSION);
#endif
}	

void print_license()
{
	printf("No License Information Available\n");
}

float tofloat(size_t a){
	float tmp=a;
	return tmp;
}
void exit_proc(int code){
	switch(code){
		case I_FLAG:
			printf("No -i flag!\n");break;
		case K_FLAG:
			printf("No -k flag!\n");break;
		case P_FLAG:
			printf("No -p flag!\n");break;
		case FSIZE:
			printf("Input file has zero length!\n");break;
	}
	exit(code);
}

//-----------------------------------------------------------------------------------
// Function     :   print_version
// Description  :   simple help menu for users
// Arguments    :   none
//              :   
// return       :   none
// notes        :   
//-----------------------------------------------------------------------------------
void print_help(){
	print_version();
#ifdef FSEDS
	printf("%s [[-e]|-d] -i in.file [-o out.file] -p pass [-k key.file]\n", SHORT_NAME);
#else
    printf("%s [[-e]|-d] -i in.file [-o out.file] -p pass [-k key.file|-t X-X-X-X]\n", SHORT_NAME);
#endif
	printf("%s -g key.file seed\n", SHORT_NAME);
	printf("%s [-h|--help]\n", SHORT_NAME);
	printf("%s [-v|--version]\n", SHORT_NAME);
	printf("\t-e Encrypt file(default)\n");
	printf("\t-d Decrypt file\n");
	printf("\t-i Input file\n");
	printf("\t-o Output file\n");
	printf("\t-p Password\n");
	printf("\t-k Key file (file size>=65536 bytes)\n");
#ifdef FSEDS
#else
	printf("\t-t XXXXXXXX-XXXXXXXX-XXXXXXXX-XXXXXXXX Token in hex format\n");
#endif
	printf("\t-g Generate key file\n");
	printf("\t-h Show this Help\n");
	printf("\t-v View License(BSD)\n");
    printf ("\n###############################################################\n");
	exit(0);
}

//-----------------------------------------------------------------------------------
// Function     :   main
// Description  :   start up function
// Arguments    :   @ argc  -   no of arguments
//              :   @ argv  -   argument array of characters
// return       :   0/1     -   failure/success
// notes        :   
//-----------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	struct fs_cipher_context fsDEs_c;
	char action=0; //Encryption default
	char in_num=-1, out_num=-1, kfile_flag=0, p_num=0, tok_flag=0;
	char out_name[255];
	uint32_t i, tok[4], seed=784332;
	uint8_t *key;
	size_t fs;
	FILE *in;

#ifdef SPEED_TEST
	time_t start;
	start=time(NULL);
#endif

#ifdef FSEDS
    print_header ();
#endif

    if(argc==1)
    {
        print_help();
    }
	
    // parse the command line argument and store the values provided by the user
	for(i=1;i<argc;i++)
    { 
		if(strcmp(argv[i], "-h")==0 || strcmp(argv[i], "--help")==0 )
        {
			print_version();
			print_help();
			exit(0);
		}
		if(strcmp(argv[i], "-d")==0)
        {
			action=1;
        }
		if(strcmp(argv[i], "-i")==0)
        {
			in_num=i+1;
		}
		if(strcmp(argv[i], "-o")==0)
        {
            out_num=i+1;
        }
		if(strcmp(argv[i], "-p")==0)
        {
			p_num=i+1;
		}
		if(strcmp(argv[i], "-k")==0)
        {
			kfile_flag=i+1;
		}
		if(strcmp(argv[i], "-t")==0)
        {
			sscanf(argv[i+1], "%x-%x-%x-%x", &(tok[0]),&(tok[1]),&(tok[2]),&(tok[3]));
			tok_flag=1;
		}
		if(strcmp(argv[i], "-v")==0 || strcmp(argv[i], "--version")==0 )
        {
			print_version();
			print_license();
            printf ("###############################################################\n");
			exit(0);
		}
		if(strcmp(argv[i], "-g")==0)
        {
			if(argc == 4)
            {
				seed=atoi(argv[i+2]);
            }
			fs_gen_key_file(argv[i+1], seed);
			exit(0);
		}
	}

	in = fopen(argv[in_num], "rb");
	if (in == NULL)
	{
        exit(1);
    }
	fs=fsize(in);
	fclose(in);
	if(in_num==-1)
    {
        exit_proc(I_FLAG);
    }
	if(fs==0)
    {
        exit_proc(FSIZE);
    }
	if(out_num==-1)
    {
		strncpy(out_name, argv[in_num], strlen(argv[in_num])+1);
        if(action==0)
        {
            strncat(out_name, ".pf2", 4);
        }
		else 
        {
            strncat(out_name, ".out", 4);
        }
	}
	else
    {
        strncpy(out_name, argv[out_num], strlen(argv[out_num])+1);
    }
	if(kfile_flag==0 && tok_flag==0)
    {
        exit_proc(K_FLAG);
    }
	if(p_num==0)
    {
        exit_proc(P_FLAG);
    }

	if(tok_flag==0)
    {
        fs_cipher_init_key_file(4096, argv[p_num], argv[kfile_flag], &fsDEs_c);
    }
	else 
    {
        fs_cipher_init_token(4096, argv[p_num], tok, &fsDEs_c);
    }

	if(action==0)
    {
		fs_cipher_file_encipher(argv[in_num], out_name, &fsDEs_c);
	}
	else
    {
        fs_cipher_file_decipher(argv[in_num], out_name, &fsDEs_c);
    }
#ifdef SPEED_TEST
	printf("speed: %.1f Mb/sec\n", (tofloat(fs)/tofloat((time(NULL)-start)))/1048576);
#endif	
	exit(0);
}
