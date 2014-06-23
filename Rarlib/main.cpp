
#include "stdafx.h"
#include "unrar.h"
#pragma comment(lib,"unrar.lib")

WINMAIN
{
	HANDLE   hArcData; 
	int   RHCode,PFCode; 
	char   CmtBuf[262144]; 
	RARHeaderData   HeaderData; 
	RAROpenArchiveData   OpenArchiveData; 
	char   *p; 
	char   FileName[256]; 
	char   ExtraPath[256]; 

	char* RarFileName = "abc.rar";
	char* ExtractPath = ".";
	strcpy(FileName,RarFileName); 
	if(ExtractPath) 
		strcpy(ExtraPath,ExtractPath); 
	else 
	{ 
		strcpy(ExtraPath,RarFileName); 
		p=ExtraPath; 
		while(*(++p)!=0); 
		while(*(--p)!=92); 
		*p=0; 
	} 

	OpenArchiveData.ArcName=FileName; 
	OpenArchiveData.CmtBuf=CmtBuf; 
	OpenArchiveData.CmtBufSize=sizeof(CmtBuf); 
	OpenArchiveData.OpenMode=RAR_OM_EXTRACT; 

	hArcData=RAROpenArchive(&OpenArchiveData); 
	HeaderData.CmtBuf=NULL; 
	while   ((RHCode=RARReadHeader(hArcData,&HeaderData))==0) 
	{ 
		PFCode=RARProcessFile(hArcData,2,ExtraPath,NULL); 
	} 
	RARCloseArchive(hArcData); 	return 0;
};