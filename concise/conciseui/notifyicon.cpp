#include "stdafx.h"
#include "../conciseui.h"

namespace cs{
	NotifyIcon::NotifyIcon(){
		Param.New();
		Param.Zero();
		Param->cbSize = Param.Size();
	}
	//һ��ʵ�������Ψһ�Ĵ��ڣ���Ҫ�ٴΰ��µĴ��ڣ�����Close����������ʵ�����ɵ�ͼ��ᱻ�ͷ�
	bool NotifyIcon::Create(HWND hWnd)
	{
		if(Param->hWnd) return 0;
		Param->hWnd = hWnd;
		return 1;
	}
	//�ظ���ʼ״̬
	void NotifyIcon::Close()
	{
		for(uint i=0;i<_nis.Count();i++)
			Delete(_nis[i]);
		Param->hWnd = 0;
	}
	bool NotifyIcon::Add(UINT id,LPCWSTR name,DWORD msgId,ResID ri)
	{
		Icon ico;
		ico.LoadIcon(ri);
		return Add(id,name,msgId,ico);
	}
	//hIcon��Ҫ�ֶ�����
	bool NotifyIcon::Add(UINT id,LPCWSTR name,DWORD msgId,HICON hIcon)
	{
		Param->uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
		Param->uCallbackMessage = msgId;
		Param->uID = id;
		if(hIcon) Param->hIcon = hIcon;
		if(name) WcsCopy(Param->szTip,name);
		else Param->szTip[0] = 0;
		if(!Shell_NotifyIcon(NIM_ADD,Param.Handle())) return 0;
		_nis.Add(id);
		return 1;
	}
	bool NotifyIcon::ShowBallon(UINT id,LPCWSTR tip,LPCWSTR title,DWORD niif)
	{
		Param->uFlags = NIF_INFO;
		Param->uID = id;
		Param->dwInfoFlags = niif;
		if(tip)
		{
			Param->szInfo[255] = 0;
			WcsCopy(Param->szInfo,tip,255);
		}
		else Param->szInfo[0] = 0;
		if(title)
		{
			Param->szInfoTitle[63] = 0;
			WcsCopy(Param->szInfoTitle,title,63);
		}
		else Param->szInfoTitle[0] = 0;
		if(!Shell_NotifyIcon(NIM_MODIFY,Param.Handle())) return 0;
		return 1;
	}
	//�ֶ�����Param�󣬵���Shell_NotifyIcon��
	bool NotifyIcon::ShellNotify(DWORD nim)
	{
		return 0!=Shell_NotifyIcon(nim,Param.Handle());
	}
	bool NotifyIcon::Delete(UINT id)
	{
		Param->uID = id;
		if(!Shell_NotifyIcon(NIM_DELETE,Param.Handle()))
		{
			PrintLastError();
			return 0;
		}
		_nis.RemoveValue(id);
		return 1;
	}
	NotifyIcon::~NotifyIcon()
	{
		Close();
	}
}