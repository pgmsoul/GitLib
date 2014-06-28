#pragma once


#ifndef CONCISEUI_EXPORTS
	//�ⲿ����
	#ifdef USE_DYNAMIC_CONCISEUI
		#define CONCISEUI_API __declspec(dllimport)
		#define USE_DYNAMIC_BASE	//ʹ�ö�̬base��
	#else
		#define CONCISEUI_API
	#endif
#else
	//�����̵���
	#ifdef CONCISEUI_STATIC
		#define CONCISEUI_API
	#else
		#define USE_DYNAMIC_BASE	//ʹ�ö�̬base��
		#define CONCISEUI_API __declspec(dllexport)
	#endif
#endif
#include <concise/concise.h>

#include "conciseui/window.h"
#include "conciseui/logwnd.h"
#include "conciseui/usermenu.h"
#include "conciseui/comdlg.h"
#include "conciseui/dlg.h"
#include "conciseui/ictrl.h"
#include "conciseui/ctrl.h"

#include "conciseui/Layout.h"
#include "conciseui/notifyicon.h"
#include "conciseui/wndPos.h"
//���ݰ�װĿ¼������·��.
#ifndef CONCISEUI_EXPORTS
	#ifdef _DEBUG
		#ifdef USE_DYNAMIC_CONCISEUI
			#pragma comment(lib,"conciseuiD.lib")
		#else
			#pragma comment(lib,"conciseuiSD.lib")
		#endif
	#else
		#ifdef USE_DYNAMIC_CONCISEUI
			#pragma comment(lib,"conciseui.lib")
		#else
			#pragma comment(lib,"conciseuiS.lib")
		#endif
	#endif
#endif