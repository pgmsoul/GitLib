#pragma once
#include "mmsystem.h"

namespace cs{
	//���ɺ��뼶���ȵĶ�ʱ��������������һ���������̡߳����Ƕ��ʵ������ֻ����һ���̡߳�
	class CONCISE_API  PreciTimer : public _class
	{
	protected:
		MMRESULT _Handle;
		Function<void,UINT,UINT,DWORD,DWORD,DWORD> _eventCB;
		void _cbProc(UINT,UINT,DWORD,DWORD,DWORD);
	public:
		Function<void,UINT,LPARAM> OnTimer;//��ʱ��id�����û�����

		PreciTimer();
		bool Create(int ms,void* user = 0);
		void Close();
		UINT Handle(){return _Handle;}
		operator UINT (){return _Handle;}
		~PreciTimer();
	};
}
