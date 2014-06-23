#pragma once

namespace cs{
	/*�Ի���������Ԥ�ȷ���255���ؼ�,����Ŀؼ������ڳ�ʼ���Ի����ʱ���ֶ�����.

	�Ի�����Ȼ��һ�����ڣ�ʵ���϶Ի����ǳ��洰�ڵ�һ�ְ�װ����Ҫ�ı��������ط���
	1. ���ɺ���������CreateWindow��CreateWindowEx���������°�װ�õĺ�����
	DialogBoxParam��ģ̬�Ի��򣩺�CreateDialogParam����ģ̬�Ի��򣩣����Ҳ���Ҫ
	ע����������ʵ�Ի������һ���ض��Ĵ����࣬���������ÿؼ�һ��Ԥ��ע����ˡ�
	2. ͬ�����ں���Ҳ�����˰�װ���Ի���Ĵ��ں�����Ȼ��ʽ�Ϻ���ͨ������һ���ģ�
	���Ҵ����ϢҲ��δ������ͷ���������������ǶԻ��򲻷��ͺܶ���Ϣ������WM_CREATE��
	������WM_INITDIALOG���档���ҶԻ���û��һ��DefDialogProc������ȱʡ���أ�
	�Ի��򴰿ں�������ֵһ����FALSE����ô�Ի���ᰴ�ڲ���ȱʡ��ʽ������֮����
	TRUE����ô�ڲ��ͺ��������Ϣ�������κδ���

	��Щ�ض���Ϣ��Ҫ��������ֵ������WM_CTRLCOLORDLG����Ϣ������һ����ˢ�������ô
	�����ˢ���������ƶԻ���ı��������ҶԻ���Ͳ��ٷ���WM_ERASEBKGROUND��Ϣ��

	IDlg �� IWnd �ߵ���Ϣ������ȫ��ͬ������IDlg���յ���Ϣ����ʱ������DefWindowProc��
	���ǵ���_defDlgProcһ��ֱ�ӷ��� 0 �ĺ�����

	ģ̬�Ի���Ĺرհ�ť������WM_COMMAND��OnCommand�����IDCANCEL��֧����ȷ����Close
	���������򲻻��Զ��رա�*/
	class CONCISEUI_API IDlg : virtual public IWnd{
	private:
		HRESULT _defDlgProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
		Function<HRESULT,HWND,UINT,WPARAM,LPARAM> _defDlgFunc;
	protected:
		void _MsgHookProc(Message* msg,IWnd*);
	public:
		Delegate<IDlg*>	OnCreate;	//WM_CREATE��Ϣ�����������ɿؼ���ʱ������Ϣ��ʵ��������ɺ�ģ�������һ�Ρ�
		IDlg();
		HWND	GetNextGroupItem(HWND ctrl,bool pre = 0){return ::GetNextDlgGroupItem(_Handle,ctrl,pre);}
		HWND	GetNextTabItem(HWND ctrl,bool pre = 0){return ::GetNextDlgTabItem(_Handle,ctrl,pre);}
		int		GetCtrlID(HWND ctrl){return GetDlgCtrlID(ctrl);}
		HWND	GetCtrlHandle(int id){return GetDlgItem(_Handle,id);}
		int		GetCtrlInt(int id,bool sign = 1,int* bSuccess = 0){return ::GetDlgItemInt(_Handle,id,bSuccess,sign);}
		int		GetCtrlText(int id,String& str);
		bool	SetCtrlInt(int id,int value,bool sign = 0){return SetDlgItemInt(_Handle,id,value,sign)!=0;}
		bool	SetCtrlText(int id,const wchar_t* text){return SetDlgItemText(_Handle,id,text)!=0;}
	};
	//ģ̬�Ի���ģ̬�Ի������Լ�����Ϣѭ�����������ڼ䣬����Ϣѭ�����ղ�����Ϣ��
	class CONCISEUI_API ModelDialog : public IDlg
	{
	public:
		ModelDialog();
		//����һ��ģ̬�Ի���,ֱ���Ի�������֮ǰ,����������᷵��.��������µķ���ֵ�ǵ���EndDialog���õ�ֵ.�����ΪhParent
		//����������,����ֵΪ0,�������󷵻�-1.��ʹ�Ի��������Ϊ���ɼ�,�������Ҳ����ʾ�Ի���.
		INT_PTR Create(HWND hParent = 0);
		//�رնԻ���codeָ���˳�����.
		bool Close(int code = 0);
	};
	//��ģ̬�Ի���.
	class CONCISEUI_API NormalDialog : public IDlg
	{
	public:
		NormalDialog();
		//����һ����ģ̬�Ի���,���ضԻ���ľ��.
		INT_PTR Create(HWND hParent = 0);
		//�رնԻ���.
		bool Close();
	};
};