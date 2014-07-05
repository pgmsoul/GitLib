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

#define ALERT_FASTMEM_FAILED

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
