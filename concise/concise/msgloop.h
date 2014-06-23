//��Ϣѭ��
#pragma once
namespace cs{
	//MsgLoop�������̵߳���Ϣ�ã�һ���߳�ֻ��ʵ����һ��ʵ������ͼʵ����ͬһ�߳��µĵڶ��� MsgLoop ʵ�������ᵼ�³����˳���
	class CONCISE_API MsgLoop : public _class{
	protected:
		HWND	_Dlg;	//�Ի�������
		HACCEL	_Accel;	//���ټ���
		//��������ǽػ���Ϣ�ĺ�������,���ĳ����������TRUE,������ĺ��������ٱ�����,ֱ�ӽ�����һ����Ϣ.
		ObjectList<Function<bool,MSG*> > _MsgHook;
	public:
		MsgLoop();
		//���õ�ǰ�߳��жԻ���ľ������ʹ���ܹ���Ӧ Tab ����
		void _SetDialog(HWND hDlg);
		//���õ�ǰ�̵߳ļ��ټ���
		void _SetAccelerator(HACCEL hacl);
		//��ʼ�̵߳�����Ϣѭ��.
		int _Start();
		//��ӵ�ǰ�̵߳���Ϣ(uMsg)��ȡ����,index����λ�á�����ֵ��һ����������ָ��,���԰���Ҫ������Ϣ�ĺ���.
		Function<bool,MSG*>* _AddMsgHook(int index = -1);
		//�Ƴ��߳��Ѿ��󶨵Ĺ��ӣ���Ӻ��Ƴ��������뱻�ɶԵĵ���.
		bool _DelMsgHook(Function<bool,MSG*> func);
		~MsgLoop();
		//���õ�ǰ�߳��жԻ���ľ������ʹ���ܹ���Ӧ Tab ����
		static int Start();
		//���õ�ǰ�߳��жԻ���ľ������ʹ���ܹ���Ӧ Tab ����
		static bool SetDialog(HWND hDlg);
		//���õ�ǰ�̵߳ļ��ټ���
		static bool SetAccelerator(HACCEL hacl);
		//��ӵ�ǰ�̵߳���Ϣ(uMsg)��ȡ����,index����λ�á�
		static bool AddMsgHook(Function<bool,MSG*> func,int index = -1);
		//�Ƴ��߳��Ѿ��󶨵Ĺ��ӣ���Ӻ��Ƴ��������뱻�ɶԵĵ���.
		static bool DelMsgHook(Function<bool,MSG*> func);
	};
	//��û�е�ǰ�̵߳� MsgLoop ����ָ��ʱ��ʹ�����º�����

	//���ص�ǰʵ���ľ��,�����ڼ��ص�ǰģ�����Դ.
	CONCISE_API inline HINSTANCE GetInstance(){return (HINSTANCE)::GetModuleHandle(0);}
}
