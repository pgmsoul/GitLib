#pragma once

namespace cs{
	class CONCISEUI_API  WndPosRestore : public _class{
		DWORD _flag;
		IntegerList<IWnd*> _wndList;
		StringMemList	_wndName;
		void _OnCreate(Message*,IWnd*);
		void _OnDestroy(Message*,IWnd*);
	public:
		WndPosRestore();
		~WndPosRestore();
		void SetFlag(DWORD flag){
			_flag = flag;
		}
		//wnd����Ҫ��λӛ䛵Ĵ��ڣ�name��ָ��һ�����Q�����ӛ���Ҫ���@�����Q��^�ִ��ڡ�
		bool AddWnd(IWnd* wnd,LPCWSTR name);
		//���WndPosRestore��������ӵĴ����N��������{���@�������Ƴ������ĳɆT�����Ļ��{��
		bool RemoveWnd(IWnd* wnd);
		//����û��HOOK WM_CRAET��Ϣ�Ĵ����ں��ʵ�ʱ������������ģ��
		void OnCreate(HWND hwnd);
		//����û��HOOK WM_DESTROY��Ϣ�Ĵ����ں��ʵ�ʱ������������ģ��
		void OnDestroy(HWND hwnd);
	};
#define WPR_FLAG_LEFT	1	//��¼ X λ��
#define WPR_FLAG_TOP	2	//��¼ Y λ��
#define WPR_FLAG_WIDTH	4	//��¼���
#define WPR_FLAG_HEIGHT	8	//��¼�߶�
	//ע��һ��wnd�������������ܹ�����WM_CREATE��WM_DESTROY��Ϣ������������Ϣ��ֱ����úͼ�¼���ڵ�λ�ô�С������
	void CONCISEUI_API SetWndRememberPos(IWnd* wnd,LPCWSTR name,DWORD flag = 0xf);
	//���һ��wnd������Զ�λ�ô�С���ܡ�
	//void CONCISEUI_API SetWndRememberPos(IWnd* wnd);
}