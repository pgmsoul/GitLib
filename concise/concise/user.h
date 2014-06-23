#pragma once
namespace cs{
	//�����������XP���ؼ���Manifest�ļ�,��һ���Ϳ�ִ���ļ�ͬ����Manifest.
	CONCISE_API bool CreateXPStyleManifest();
	//�رռ����,
	enum PowerMode
	{
		Power_Logoff = 0,//ע��
		Power_ReBoot = 1,//��������
		Power_ShutDown = 2,//�رյ�Դ.
		Power_Sleep = 3,//����
		Power_Suspend = 4//����
	};
	CONCISE_API bool SetPower(PowerMode mode,bool force = 0);
	//��������߳�������Timer���޷������趨�Ļص�������
	class CONCISE_API  Timer : public HandleType<UINT_PTR>
	{
	protected:
		void _TimerProc(HWND hWnd,UINT uMsg,UINT_PTR idEvent,DWORD);
		Function<void,HWND,UINT,UINT_PTR,DWORD> _Timer;
	public:
		//���������Խ��ջص��ĺ���,UINT_PTR��Timer�ľ��.
		Function<void,uint,void*> OnTimer;
		
		Timer();
		//����һ������״̬��Timer,������Stop��������Start.
		bool Start(int ms);
		//ֹͣTimer
		void Stop();
		~Timer();
	};
	//����������Ļ�ļ���ʽ,���ȡ���߼�����ʽ,�ο�ChangeDisplaySettings������ĵ�.
	class CONCISE_API  Screen : public _class
	{
	public:
		//��ȡ��ǰ�豸�����гֵ�ģʽ,�ظ��ĺ����������,����ÿ��index��1,ֱ������ֵΪ0.
		bool GetScreen(int index = ENUM_CURRENT_SETTINGS);//ENUM_REGISTRY_SETTINGS(-2)for save in regisry;
		//������Ļģʽ,�����ǻ�ȡ����ֵ,����������ֵ��֧��.test���������Ƿ����֧��Ҫ���õ�ģʽ,�����ı䵱ǰ��ʾģʽ.
		LONG SetScreen(bool test = 0);
		int Width,			//���ؿ��.
			Height,			//���ظ߶�.
			Color,			//��ɫ���.
			Frequency;		//ˢ��Ƶ��.
	};
	//���ó��򿪻�������keyName�����ƣ�����ָ������һ�����ƣ���������ظ��Ḳ��ԭ������������Ĭ��0�����ļ�����������׺)Ϊ���֡�
	//file�����������ĳ��򣬿����ǿ�ִ���ļ���Ҳ�����������ļ���Ĭ��0ָ���ó�������
	//arg��������alluser���Ƿ�������û������Ϊ0����ֻ�е�ǰ�û���Ч��
	CONCISE_API bool SetProgramAutoRun(bool autorun,LPCWSTR keyName = 0,LPCWSTR file = 0,LPCWSTR arg = 0,bool alluser = 1);
	CONCISE_API bool GetProgramAutoRun(LPCWSTR keyName = 0,LPCWSTR file = 0);
	//�����ķ���ȫ��Ϊ��̬����
	class CONCISE_API App{
	public:
		//����Ӧ�ó����ʶ����, Ӧ�ó���ͳһ������, ����һЩĬ����������.
		static void SetAppName(LPCWSTR name);
		//��ȡӦ�ó����ʶ���ƣ�Ĭ��ֵ�ǿ�ִ���ļ���.
		static LPCWSTR GetAppName();
		//����Ӧ�ó���Ŀ¼���˺����� API ���� SetCurrentDirectory û���κι�ϵ��GetCurrentDirecory Ҳ���������Ŀ¼��Ĭ��ֵ�ǿ�ִ���ļ�����Ŀ¼.
		static void SetAppDirectory(LPCWSTR folder);
		//��ȡӦ�ó���Ŀ¼���˺����� API ���� GetCurrentDirecoryû���κι�ϵ��Ĭ���ǿ�ִ���ļ�����Ŀ¼��
		static LPCWSTR GetAppDirectory();
	};
}