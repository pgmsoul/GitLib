
#include "stdafx.h"
#include "unrar.h"
#pragma comment(lib,"unrar.lib")

int RarCompress(LPCWSTR rar,LPCWSTR src){
	return 0;
}
int RarDecompress(LPCWSTR src,LPCWSTR dst){
	RARHeaderDataEx			hd; 

	cs::Memory<char> buf(true);
	buf.SetLength(0x100000);
	AutoStruct(RAROpenArchiveDataEx,oad);
	oad.ArcNameW = (LPWSTR)src;
	oad.CmtBufSize = buf.Length();
	oad.CmtBuf = buf.Handle();
	oad.OpenMode = RAR_OM_EXTRACT;

	HANDLE hRar = RAROpenArchiveEx(&oad);
	if(!hRar) return -1;
	hd.CmtBuf = 0;

	int rst;
	while(true){
		rst = RARReadHeaderEx(hRar,&hd);
		if(rst) break;
		int code = RARProcessFileW(hRar,2,(LPWSTR)dst,NULL); 
	}
	RARCloseArchive(hRar);
	return rst;
}
WINMAIN
{
	RarDecompress(L"abc.rar",L".");
	return 0;
};