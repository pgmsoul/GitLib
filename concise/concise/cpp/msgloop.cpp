#include "../stdafx.h"
#include "../../concise.h"

namespace cs{
	//���������3����;��
	// 1. �洢�̵߳�Ψһ MsgLoop �����ָ�루�ֲ���̬����������������ã���ʱ msg �������߳�Ψһ MsgLoop �����ָ�룬ʵ������ֻ�� MsgLoop �Ĺ��캯���е���һ�Ρ�
	// 2. ȡ���߳�Ψһ MsgLoop �����ָ�룬��ʱ msg ������Ĭ��ֵ 0 ��
	// 3. ���������߳�Ψһ MsgLoop �����ָ��Ϊ 0 ������ msg ���봫����ʱ��ֵ��������ǳ��򽫻��˳�����ʵ����ֻ�� MsgLoop ����������ʱ�����һ�Ρ�
	__declspec(thread) static MsgLoop* cMsgLoop = 0;
	MsgLoop* getThreadMsgLoop(bool del = false){
		if(del){
			if(cMsgLoop){
				delete cMsgLoop;
				cMsgLoop = 0;
			}
			return 0;
		}
		if(cMsgLoop==0) cMsgLoop = new MsgLoop;
		return cMsgLoop;
	}
	MsgLoop::MsgLoop():_Dlg(0),_Accel(0){
	}
	void MsgLoop::_SetDialog(HWND hDlg){
		_Dlg = hDlg;
	}
	void MsgLoop::_SetAccelerator(HACCEL hacl){
		_Accel = hacl;
	}
	int MsgLoop::_Start(){
		MSG msg;
		while(::GetMessage(&msg,0,0,0)){
			if(_Dlg){
				if(IsDialogMessage(_Dlg,&msg)) continue;
			}
			if(_Accel){
				if(TranslateAccelerator(msg.hwnd,_Accel,&msg)) continue;
			}
			uint i = 0;
			for(;i<_MsgHook.Count();i++){
				if(!_MsgHook[i].IsNull()) {
					if(_MsgHook[i].Procedure()(&msg)) break;
				}
			}
			if(i==_MsgHook.Count()){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		getThreadMsgLoop(true);
		return (int)msg.wParam;
	}
	Function<bool,MSG*>* MsgLoop::_AddMsgHook(int index){
		return _MsgHook.Add(index);
	}
	bool MsgLoop::_DelMsgHook(Function<bool,MSG*> func){
		for(uint i=0;i<_MsgHook.Count();i++){
			if(func==_MsgHook[i]){
				_MsgHook.Delete(i);
				return 1;
			}
		}
		return 0;
	}
	MsgLoop::~MsgLoop(){
	}
	int MsgLoop::Start(){
		MsgLoop* ml = getThreadMsgLoop();
		return ml->_Start();
	}
	//���õ�ǰ�߳��жԻ���ľ������ʹ���ܹ���Ӧ Tab ����
	bool MsgLoop::SetDialog(HWND hDlg){
		MsgLoop* ml = getThreadMsgLoop();
		if(ml==0) return 0;
		ml->_SetDialog(hDlg);
		return 1;
	}
	//���õ�ǰ�̵߳ļ��ټ���
	bool MsgLoop::SetAccelerator(HACCEL hacl){
		MsgLoop* ml = getThreadMsgLoop();
		if(ml==0) return 0;
		ml->_SetAccelerator(hacl);
		return 1;
	}
	//��ӵ�ǰ�̵߳���Ϣ(uMsg)��ȡ����,index����λ�á�����ֵ��һ����������ָ��,���԰���Ҫ������Ϣ�ĺ���.
	bool MsgLoop::AddMsgHook(Function<bool,MSG*> func,int index){
		MsgLoop* ml = getThreadMsgLoop();
		if(ml==0) return 0;
		*(ml->_AddMsgHook(index)) = func;
		return 1;
	}
	//�Ƴ��߳��Ѿ��󶨵Ĺ��ӣ���Ӻ��Ƴ��������뱻�ɶԵĵ���.
	bool MsgLoop::DelMsgHook(Function<bool,MSG*> func){
		MsgLoop* ml = getThreadMsgLoop();
		if(ml==0) return 0;
		return ml->_DelMsgHook(func);
	}
}
