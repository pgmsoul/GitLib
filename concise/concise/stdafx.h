
#pragma once

#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0600		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0600	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#ifdef MFC		//�����MFC������Ҫ����MFC��
#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#else
#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#endif
