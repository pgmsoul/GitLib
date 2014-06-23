// jsbase.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "v8base.h"
#include "jsbase.h"

namespace v8{
	LPCWSTR RequireBase(LPCTSTR modeList){
		HandleScope store;
		Handle<Object> glb = v8::GetGlobal();

		cs::StringList sl;
		sl.SplitString(modeList,L",");
		for(uint i=0;i<sl.Count();i++){
			sl[i].Trim();
		}

		loadComponent(sl,modeList,L"gdi",glb,L"CRgn",&LoadGDI);
		loadComponent(sl,modeList,L"file",glb,L"CFile",&LoadFile);
		loadComponent(sl,modeList,L"net",glb,L"CHttpRequest",&LoadNet);
		loadComponent(sl,modeList,L"platform",glb,L"CRegistry",&LoadRegistry);
		loadComponent(sl,modeList,L"ker",glb,L"CMemory",&LoadKernel);
		return L"gdi,file,net,platform,ker";
	}
};