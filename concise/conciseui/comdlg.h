#pragma once
#include <shlobj.h>//this is use BIF_ const.

namespace cs{
	class CONCISEUI_API  FolderDialog : public _class
	{
	protected:
		Memory<wchar_t> _Path;
		Function<int,HWND,UINT,LPARAM,LPARAM> _bcp;
		int _BrowseCallbackProc(HWND hwnd,UINT msg,LPARAM lp,LPARAM pData);
	public:
		FolderDialog(void);
		bool Show(HWND owner = 0,LPCWSTR title = 0,LPCWSTR init = 0,DWORD flags = BIF_USENEWUI);
		LPCWSTR GetFolder(){return _Path;}
	};
	class CONCISEUI_API  FileDialog : public HandleType<HWND>
	{
	protected:
		//�������ָ���Ի���Ļص�����,������������������ָ��һ���Զ��庯��,����������±����Լ���_Handle��ֵ.
		Function<UINT_PTR,HWND,UINT,WPARAM,LPARAM> mDlgProc;
		UINT_PTR OFNHookProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
		//�ļ�������.
		VirtualAddress mFileNameBuffer;
		//�ļ����͹����б��ѡ����ļ��б�.
		StringMemList mFilter,mSelectFiles;
		//�Ի��򴰿ھ��(ONFHookProc�����е�hDlg�ǶԻ���ؼ��Ĵ��ھ��,������ǲ�ͬ��).������������CDM_����Ϣ�ĺ���.
		uint mFilterIndex;
	public:
		String Title					//�Ի������.
			,DefaultExt						//ȱʡ�ļ�����(��������ļ���������׺),���ֵ����Ҫ�ӵ㣬ֱ�����ú�׺��.
			,DefaultFile					//ȱʡ�ļ���.
			,InitDir;						//��ʼ��·��.
		DWORD Flags;						//�Ի�����.
		bool FlagsEx;						//�Ƿ��ж���ķ��(�������).

		//���OK��ťʱ����,����true�Ի��򽫱��ִ�,������ر�.
		Function<bool,LPOPENFILENAME,FileDialog*> OnFileOk;
		//����ɸѡ��仯ʱ����.������ɸѡ����.
		Function<void,int,FileDialog*>OnTypeChange;
		//ѡ����仯ʱ����.������Ŀ¼���ļ���������Ŀ¼��������Ϣ��bug��
		//Function<void,LPCWSTR,LPCWSTR>OnSelChange;
		//��ѡ����ļ��б仯ʱ.
		Function<void,LPCWSTR,FileDialog*>OnFolderChange;

		FileDialog();
		//��ʾ���ļ��Ի���.
		bool ShowOpen(HWND owner = 0);
		//��ʾ�����ļ��Ի���.
		bool ShowSave(HWND owner = 0);
		//����Ի����ǵ�ѡ(������ѡ,����ֻѡ��һ���ļ�),index������0,���ص���ȫ·��,����Ƕ�ѡ,0����·��,���������ļ���(������·��).
		LPCWSTR GetFileName(uint index = 0);
		//ÿ����һ�����������������ļ�����ѡ����Ͽ������һ��.
		//��ʽ:showText�����������ַ���,filter�����µĸ�ʽ "*.txt;*.ini;*.cpp"
		void AddFilter(LPCWSTR showText,LPCWSTR filter,uint index = -1);
		//����ȱʡ���ļ�������(����).
		bool SetFilterIndex(uint index);		
		//ɾ������ӵ��ļ�������.
		bool DeleteFilter(uint index);		
		//�Ի��������ڵľ��.
		HWND Handle(){return _Handle;}
		//ѡ����ļ�����(���ļ�ʱ).
		uint GetSelectCount();	
		//���ع�����������.
		uint GetFilterIndex(){return mFilterIndex;}
	};
	//��ɫ�Ի���.
	class CONCISEUI_API  ColorDialog : public _class
	{
	public:
		enum ColorFlags{ColorDlgNormal=0,	//��ͨ��ʽ
			ColorDlgBasic=4,				//ֻ��ʾ������ɫ
			ColorDlgFullOpen=2};			//չ����ɫѡ��.
		ColorDialog();
		//CC_SOLIDCOLOR(ֻ��ʾ������ɫ),CC_PREVENTFULLOPEN(��ѡȫ���򿪰���),CC_FULLOPEN(ȫ����)
		bool Show(HWND hWnd = 0,ColorFlags flags = ColorDlgNormal);
		//���úͻ�ȡ16���Զ�����ɫ.
		static DWORD CustomColor[16];
		DWORD Color;
	};
	//��ʾһ������Ի���,��ΪmCf��mLf��Աʵ�����ǿ��ŵ�,�������Ը��,���Կ���ChooseFont�������κ�ѡ��.
	class CONCISEUI_API  FontDialog : public _class
	{
	protected:
		wchar_t mStyle[4];

		Function<UINT_PTR,HWND,UINT,WPARAM,LPARAM> mDlgProc;
		UINT_PTR CFHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam);
	public:
		FontDialog();
		//��ӦApply��ť�Ļص���������LogFont��Աȡ��ѡ����������.
		Function<void,LPLOGFONT,FontDialog*> OnApply;
		//ʵ������Ȼ�Ի������mLogFont��Ա����ʼ����ʾ��ֵ,����ֻ������ĳ�Ա�Ż�����ʹ��.����һ����ɫ������mChooseFont������.
		void Initialize(int size,wchar_t* name,Color c = 0,bool bold = 0,bool italic = 0,bool underline = 0,bool strike = 0);
		//��ʾ����Ի���.apply�����Ƿ���Ӧ�ð�ť,effects���ھ����Ƿ���ʾЧ��ѡ��.LogFont��Ա��ȡ���ص�ֵ.
		bool Show(HWND hWnd = 0,bool apply = 0,bool effects = 0);
		//΢������Ƶ�ʱ��Ū�÷ǳ��ң����ò��������ṹ��ʼ����ȡ�ز��������������ṹ�ĳ�Ա�ֺܶ඼�ò��ϣ�����û��Ч����
		Object<LOGFONT> LogFont; //�����Ա�����ڳ�ʼ���Ի���Ĳ�����Ҳ����ȡ��ѡ������������
		Object<CHOOSEFONT> ChooseFont;//�����Ա�����ڳ�ʼ���Ի���Ĳ�����Ҳ����ȡ��ѡ����������.
	};
}