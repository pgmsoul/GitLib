//����ļ����ⲿ�������ÿ��ͷ�ļ�
#pragma once

#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0700	// Change this to the appropriate value to target other versions of IE.
#endif

#ifdef MFC		//�����MFC������Ҫ����MFC��
#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#else
#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#endif

#if defined(_UNICODE)||defined(UNICODE)
#else
	#error ���̱�������Ϊ Unicode �ַ���
#endif
#ifdef CONCISE_EXPORTS
	//�⹤��
	#if defined(CONCISE_STATIC)
		//��̬�ⲻ��Ҫ��������
		#define CONCISE_API	
		#define CONCISETL_API
	#else
		//��̬����Ҫ��������
		#define CONCISE_API __declspec(dllexport)
		#define CONCISETL_API __declspec(dllexport)
	#endif
#else
	//���������Ĺ���
	#ifdef USE_DYNAMIC_CONCISE
		#define CONCISE_API __declspec(dllimport)		//������󵼳�����
	#else
		#define CONCISE_API __declspec(dllexport)		//������󵼳�����
	#endif
	#define CONCISETL_API __declspec(dllexport)			//ģ����󵼳�����, ģ���಻��ʹ�õ������.
#endif

#include "concise/basic.h"
#include "concise/struct.h"
#include "concise/mem.h"
#include "concise/list.h"
#include "concise/function.h"
#include "concise/text.h"
#include "concise/console.h"
#include "concise/sys.h"
#include "concise/debugoutput.h"
#include "concise/kernel.h"
#include "concise/json.h"
#include "concise/io.h"
#include "concise/filemanage.h"

#include "concise/WinHttp.h"
#include "concise/gzip.h"
#include "concise/xyssl.h"
#include "concise/ecc.h"
#include "concise/registry.h"
#include "concise/msgloop.h"
#include "concise/user.h"
#include "concise/res.h"
#include "concise/gdi.h"
#include "concise/img.h"
#include "concise/key.h"
#include "concise/web.h"

#include "concise/datatree.h"
#include "concise/precitimer.h"

//���ݰ�װĿ¼������·��.
#ifndef CONCISE_EXPORTS
	#ifdef _DEBUG
		#if defined(USE_DYNAMIC_CONCISE)
			#pragma comment(lib,"conciseD.lib")
		#else
			#pragma comment(lib,"conciseSD.lib")
		#endif
	#else
		#if defined(USE_DYNAMIC_CONCISE)
			#pragma comment(lib,"concise.lib")
		#else
			#pragma comment(lib,"conciseS.lib")
		#endif
	#endif
#endif
