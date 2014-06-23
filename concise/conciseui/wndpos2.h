#pragma once

namespace base{
	class BASEUI_API  WndPosRestore : public _class
	{
		IntegerList<IWnd*> wndList;
		StringMemList	wndName;
		void _OnCreate(Message*);
		void _OnDestroy(Message*);
	public:
		WndPosRestore();
		~WndPosRestore();
		//wnd����Ҫ��λӛ䛵Ĵ��ڣ�name��ָ��һ�����Q�����ӛ���Ҫ���@�����Q��^�ִ��ڡ�
		bool AddWnd(IWnd* wnd,LPCWSTR name);
		//���WndPosRestore��������ӵĴ����N��������{���@�������Ƴ������ĳɆT�����Ļ��{��
		bool RemoveWnd(IWnd* wnd);
		//����û��HOOK WM_CRAET��Ϣ�Ĵ����ں��ʵ�ʱ������������ģ��
		void OnCreate(HWND hwnd);
		//����û��HOOK WM_DESTROY��Ϣ�Ĵ����ں��ʵ�ʱ������������ģ��
		void OnDestroy(HWND hwnd);
	};
	//ע��һ��wnd�������������ܹ�����WM_CREATE��WM_DESTROY��Ϣ������������Ϣ��ֱ����úͼ�¼���ڵ�λ�ô�С������
	void BASEUI_API WPRRegisterWnd(IWnd* wnd,LPCWSTR name);
	//���һ��wnd������Զ�λ�ô�С���ܡ�
	void BASEUI_API WPRUnRegisterWnd(IWnd* wnd);
}