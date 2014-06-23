#pragma once
//���ټ���4�����.
#define aclAlt			FALT
#define aclShift		FSHIFT
#define aclControl		FCONTROL
#define aclVirtKey		FVIRTKEY
namespace cs{
/*	1. �����Ӧ����ϼ��Ѿ�������߳�(������������)ע��,����������� 0.�����ڷ�����ʱ�ᱻ�滻��Ҳ������������ע��ͬһ�ȼ�������ID��ͬ��hWndΪ0��
	��ʱ���������򶼻�ע��ɹ������ǽ�����Ϣ�������һ��ע��Ľ��̣��������������˳���ԭ��ע����ȼ��Ե�һ��������Ȼ��Ч����
	2. ͬһ�߳�ע����ͬ����ϼ�,ǰһ���ᱻ�滻.
	3. ͬһ���߳�ע��Ĳ�ͬ��ϼ�,��ID��ͬ,hWnd ��ͬ,��������0.ֻҪһ����ͬ�Ϳ���ע��ɹ�.
	4. ��ϼ�ʵ���Ͽ�����4��������ͬʱ����Shift,Alt,Ctrl,Win����Win ��û���κ����⡣
	5. �ȼ������ǵ����ĸ�������������� 0 ֵ�����ߵ������������
	
	ÿһ���ȼ��������Ӧһ�� Hotkey ʵ�������ʵ�������٣�ע����ȼ����Զ����.��һ���Ѿ� Register() �ɹ���ʵ���������� Register() ����ֱ�ӷ��� 0.
	WM_HOTKEY�ǽ�����صģ�ͬһ���̣��������Ǹ��߳���ע�ᣬ���е���Ϣѭ���ﶼ���Խ��յ��ȼ����µ���Ϣ
	
	�ص������������̵� MsgLoop�����û��ʹ��MsgLoop���ص����ᱻ����

	MSDN����ID˵�����£�Specifies the identifier of the hot key. No other hot key in 
	the calling thread should have the same identifier. An application must specify a
	value in the range 0x0000 through 0xBFFF. A shared DLL must specify a value in 
	the range 0xC000 through 0xFFFF (the range returned by the GlobalAddAtom function).
	To avoid conflicts with hot-key identifiers defined by other shared DLLs,
	a DLL should use the GlobalAddAtom function to obtain the hot-key identifier.
	
	Hotkey�� id=0 ������������⴦����������� id=0��ע�᲻��ɹ���
	*/
	class CONCISE_API  Hotkey : public _class
	{
	protected:
		WORD _Identity;
		bool _CallBack(MSG* msg);
	public:
		Function<void,int /*id*/> OnPress;	//�����Ա����������Ӧ��Ϣ����
		////////////////////////////////////////////
		Hotkey();
		~Hotkey();
		//ע���ȼ�.
		bool Register(WORD id,int key,bool shift,bool ctrl = 0,bool alt = 0,bool win = 0);
		//ȡ���ȼ�ע��.
		bool UnRegister();
	};
	struct Accel : public _class
	{
		//���һ�����ټ�,key�������,һ��Ϊ���̶�Ӧ��ĸ�Ĵ�д.id��ϢID,ctrl�Ƿ����CTRL��,alt�Ƿ����ALT��,shift�Ƿ����
		//SHIFT��,����������Զ����FVIRTKEY���.��������Ѿ����ڵ���ϼ�(����false),���ǲ�ͬ��ϼ���������ͬID.
		bool Add(short key,short id,bool ctrl,bool alt,bool shit);
		//���һ����ϼ�.����������ֶ����FVIRTKEY���.
		bool Add(short key,short id,char flag);
		//ɾ��һ���Ѿ���ӵ���ϼ�.
		bool Delete(short key,short id,char flag);
		//ɾ��ָ����������ϼ�.
		bool Delete(int index);
		//��ϼ�����
		int Count(){return _AccelList.Count();}
		//��ȡ��ϼ�����ľ��(����CreateAcceleratorTable�����Ĳ���).
		inline ACCEL* GetHandle(){return (LPACCEL)_AccelList.GetHandle();}
		//������Ϳ���ֱ����ΪLPACCEL���͵Ĳ���.
		operator ACCEL* (){return (LPACCEL)_AccelList.GetHandle();}
	private:
		List<ACCEL> _AccelList;
	};
	class CONCISE_API  AccelTable : public HandleType<HACCEL>
	{
	public:
		//���ɼ��ټ���.���������غ������Զ�̬����.
		bool Create(LPACCEL pAcl,int count);
		bool Create(Accel& acl);
		//����Դ����.
		bool LoadResource(ResID res);

		//������һ��������Ϣѭ���õ��ĵ���.
		bool Start();
		//�͵���Startʱ������ͬһ������.
		bool Stop();
	private:
		bool _CallBack(MSG* msg);
	};
}
