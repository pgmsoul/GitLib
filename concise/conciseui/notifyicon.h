#pragma once
namespace cs{
	/*�����ڽ����趨����Ϣ��lParamΪ�����Ϣ*/
	class CONCISEUI_API  NotifyIcon : public _class
	{
	protected:
		IntegerDiction<DWORD> _nis;
	public:
		Object<NOTIFYICONDATA> Param;

		NotifyIcon();
		//һ��ʵ�������Ψһ�Ĵ��ڣ���Ҫ�ٴΰ��µĴ��ڣ�����Close����������ʵ�����ɵ�ͼ��ᱻ�ͷ�
		bool Create(HWND hWnd);
		//�ظ���ʼ״̬
		void Close();
		bool Add(UINT id,LPCWSTR name,DWORD msgId,ResID ri);
		//hIcon��Ҫ�ֶ�����
		bool Add(UINT id,LPCWSTR name,DWORD msgId,HICON hIcon);
		bool ShowBallon(UINT id,LPCWSTR tip,LPCWSTR title,DWORD niif);
		//�ֶ�����Param�󣬵���Shell_NotifyIcon��
		bool ShellNotify(DWORD nim);
		bool Delete(UINT id);
		~NotifyIcon();
	};
}