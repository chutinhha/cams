#ifndef __IDDELIUTILITY_H__
#define __IDDELIUTILITY__H__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "IdPLatformSpDfns.h"
using namespace std;

typedef vector<string> Fields;

#define PKT_DELI  '$'
#define FLD_DELI '%'
#define EVT_DELI  '&'
#define FLD_SIZ   100
#define NUM_FLDS  10
#define NUM_PKT_FLDS  10
#define NUM_EVT_FLDS   18

using namespace std;



class CIdDeliUtility
{
public:
	CIdDeliUtility(){}


string GetUnDeliFld(string& szStr ,char tdeli1 ,char  tdeli2 )
	{
	char *pTmp=new char[szStr.length()+1];
	char *pStr = pTmp;

	strcpy(pStr,szStr.c_str());
	int found=0;

	char *szBuffer=new char[2*szStr.length()+1];  //Double the size of szStr
	memset(szBuffer,'\0',2*szStr.length()+1);

	//cout<<szBuffer;
	int index = 0;
	while(1)
	{

		if(*pStr == '/')
		{
			szBuffer[index++]='/';
			szBuffer[index++]='/';
		}
		if(*pStr == tdeli1)
		{
			szBuffer[index++]='/';
			szBuffer[index++]=tdeli1;
		}
		if(*pStr == tdeli2)
		{
			szBuffer[index++]='/';
			szBuffer[index++]=tdeli2;
		}
		if(*pStr == '\0')
		{
			found =1;
		}

		if((*pStr != '/') && (*pStr != tdeli1 )&&(*pStr != tdeli2 )&&( *pStr != '\0'))
		{
			szBuffer[index++]=*pStr;
		}


	/*	-----------------------------------------------------------------------------*/
		if(found == 1)
			break;
		pStr++;

	}
	//cout<<szBuffer;

	string strDeliString = szBuffer;

	delete [] pTmp;
	delete [] szBuffer;

	return strDeliString;
}

string GetUnDeliFld(unsigned int uValue)
{

		char szTempPort[10]={0};
		string strBuff="";
		SNPRINTF(szTempPort,sizeof(szTempPort),"%u",uValue);
		strBuff.append(szTempPort);
		return strBuff;
}

string AddBegDeli(string& strDeliString ,char tChar)
{
	strDeliString.insert(strDeliString.begin(),tChar);
	return strDeliString;
}

string AddEndDeli(string& strDeliString ,char tChar)
{
	strDeliString.insert(strDeliString.end(),1,tChar);
	return strDeliString;
}



	//string DeliString(string ptr) ;
Fields EscFields(string strUnEscString ,unsigned int uSize,unsigned int uFilds,char chFldDeli,char chPktDeli)
{
	char *pUnEscString;
	char *deletePtr;
	size_t uLength = strUnEscString.size()+1;
	deletePtr = pUnEscString = new char [uLength];

	memset(pUnEscString,'\0',uLength);
	strcpy (pUnEscString, strUnEscString.c_str());

	char *pStartFld=NULL, *pEndFld=NULL;
	char *escStr = new char[uLength];
	//char **unEscStr;
	//unEscStr=new char * [uFilds];
	Fields tFields;
	/*for(unsigned int Count=0;Count<uFilds;Count++)
		{
			unEscStr[Count]=new char[uSize];
			memset(unEscStr[Count],'\0',uSize);
		}*/
//	int  uCount=0;
	while( GetField(pUnEscString,&pStartFld,&pEndFld,chFldDeli,chPktDeli) )
		{
		//	Get the first field from input
			memset(escStr,'\0',uLength);
			int nBytes = pEndFld-pStartFld-1;
			if((nBytes) == 0)
			{
				memset(escStr,'\0',uLength);
			}
			else
			{
				strncpy(escStr,pStartFld+1,nBytes);
			}
			pUnEscString=pEndFld;
		//	unEscStr[uCount++]= GetUnEscapedString(escStr,chFldDeli,chPktDeli);
			string str = GetUnEscapedString(escStr,chFldDeli,chPktDeli);
			//cout<<"UNESC-STRING :"<<str<<endl;
			tFields.push_back(str);//(GetUnEscapedString(escStr,chFldDeli,chPktDeli));

		}
	delete []escStr;
	delete []deletePtr;
	return tFields;
}


string GetUnEscapedString(char* pUnEscString,char chFldDeli ,char chPktDeli)
	{
	size_t uSize = strlen(pUnEscString)+1;
	char *pBuff = new char[uSize];
	memset(pBuff,'\0',uSize);
	int uCount=0;
	while(*pUnEscString)
		{

			if( *pUnEscString == '/' )
				{
				/*---------------------------------------*/
			/*	switch( *(pUnEscString+1) )
						{
							case '/':
							case FLD_DELI:
							case PKT_DELI:
							pBuff[uCount] = *(pUnEscString+1);
							uCount++;
							pUnEscString+=2;
							break;
						//	error if EOS || /a
							case '\0':
							default:
							return NULL;
							//break;
						}*/
				/*-----------------------------------------*/

					if((*(pUnEscString+1)=='/') || (*(pUnEscString+1)==chFldDeli) ||(*(pUnEscString+1)==chPktDeli))
						{
							pBuff[uCount] = *(pUnEscString+1);
							uCount++;
							pUnEscString+=2;

						}
					else
					{
						delete []pBuff;
						return string ("");
					}
					/*------------------------------------------*/
				}
			else
				{

				pBuff[uCount] = *pUnEscString;
				uCount++; pUnEscString++;
				}
		}//endwhile

	pBuff[uCount] = '\0';
	string strRetval = pBuff;
	delete []pBuff;
	return strRetval;

 }


char* GetField(char *strUnEscString, char **pStartFld, char ** pEndFld,char chFldDeli,char chPktDeli)
	{

	/**Gets string and eachtime find the string bounded by FLD_DELI, Finds pStartFld and pEndFld. Return a Field,*/
	char *pUnEscString=strUnEscString ;



	int uFoundEsc = 0;

	//skip to leading 'FLD_DELI'
	while( *pUnEscString != FLD_DELI	/* && *(pUnEscString-1) != '/'*/	)
		{
			pUnEscString++;
		}


	*pStartFld = pUnEscString; //pStartFld points to leading 'FLD_DELI'
	pUnEscString++;
	while(1)
		{

			 //get pos of ending 'FLD_DELI'
			if( *pUnEscString == '\0')
				{
					return NULL;
				}
			else if( *pUnEscString == '/' && ( *(pUnEscString+1) == chFldDeli || *(pUnEscString+1) == '/'|| *(pUnEscString+1) == chPktDeli))
				{
					//skip //, /'FLD_DELI'and /PKT_DELI
					pUnEscString+=2;
				}
				else if( *pUnEscString != '/' && *pUnEscString != chFldDeli && *pUnEscString != chPktDeli)
					{
						//skip normal chars
						 pUnEscString++;
					}
					else if( *pUnEscString == chFldDeli)
							{
								//found ending 'FLD_DELI' of field
								uFoundEsc = 1;
								*pEndFld = pUnEscString;
								break;
							}
						else
							{
								//	cerr<<"Read Error or Format Error in One of the Field"<<endl;
								return NULL ;
							}
		}
	if( uFoundEsc != 1 )
		return NULL;
	else
		return pUnEscString;

	}

};




#endif
