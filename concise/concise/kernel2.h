#pragma once
#include "function.h"
#define threadvar __declspec(thread) //�̱߳��ش洢����
namespace concise{
	//ʹ�� MessageBox ��ʾһ��ϵͳ�� GetLastError,WSAGetLastError ��������ֵ�������ı��Ի���
	CONCISE_API int SystemMessageBox(HWND hWnd = 0,LPCWSTR title = L"��Ϣ",DWORD style = MB_ICONINFORMATION,DWORD code = GetLastError());
	//ȡ��ϵͳ�� GetLastError,WSAGetLastError �Ⱥ������ı�����. code �����Ϻ������صĴ���.
	CONCISE_API int GetSystemMessage(String& str,DWORD code = GetLastError());
	/*����һ��������Event���ڵ���Wait����֮ǰ����Signal��֪ͨ״̬��Ȼ��ı�
	����̵߳ȴ����Event��ʱ��Signalʱ�����̵߳���Wait���Ⱥ�˳��֪ͨ*/
	class CONCISE_API Event : public HandleType<HANDLE>{
	public:
		Event();
		~Event();
		HANDLE Attach(HANDLE ent){return HandleType<HANDLE>::Attach(ent);}
		HANDLE Detach(){return HandleType<HANDLE>::Detach();}
		//autosinaled������Wait�������ú�Event ״̬�Ǳ���Ϊδ֪ͨ״̬��autoblock=1������ά��֪ͨ״̬��autoblock=0����signaled����ʼ״̬��
		bool Create(bool autoblock = 1,bool signal = 1,LPCWSTR name = 0);
		//��һ�����ڵ�Event��
		bool Open(LPCWSTR name);
		//���� Event ״̬��signaled = 1��Wait�����᷵�ء�
		void Signal(bool signaled = 1);
		//��������̡߳�
		DWORD Wait(int ms = -1);
		//�رն���
		void Close();
	};
	/*CriticalSection�����ڶ��̵߳���ʱ��Lock��Unlock�����м�Ĵ���ͬʱֻ��
	һ���߳���ִ�С����ǣ�ע�⣺Lock���������������̵߳Ķ�ε��ã����̶߳��
	����Lockֻ������ CRITICAL_SECTION LockCount��Ա��ֵ��
	*/
	class CONCISE_API CriticalSection : public _class{
	protected:
		CRITICAL_SECTION _Handle;
	public:
		inline CriticalSection(){
			InitializeCriticalSection(&_Handle);
		}
		inline ~CriticalSection(){
			DeleteCriticalSection(&_Handle);
		}
		//��ʼ�������룬���ú�������߳�û�е���Unlock֮ǰ�������̶߳�Lock�ĵ��ö������
		inline void Lock(){
			EnterCriticalSection(&_Handle);
		}
		//���������������߳������һ�������������
		inline void Unlock(){
			LeaveCriticalSection(&_Handle);
		}
		inline bool TryLock(){
			return 0!=TryEnterCriticalSection(&_Handle);
		}
	};
	//ʹ��LocalCriticalSection������������Lock��Unlock�������Զ��������������Ĵ��롣
	//����ֻ�ں�������Ϊ�ֲ�����ʹ�á�
	class CONCISE_API LocalCriticalSection : public _class{
	protected:
		CriticalSection* _handle;
	public:
		inline LocalCriticalSection(CriticalSection& cs){
			_handle = &cs;
			_handle->Lock();
		}
		inline ~LocalCriticalSection(){
			_handle->Unlock();
		}
	};
	template<typename R>DWORD __stdcall _ThreadCall(void* p){
		struct PARAM : public _struct{
			Function<R> OnCall;
		};
		PARAM* pp = (PARAM*)p;
		pp->OnCall();
		delete pp;
		return 1;
	}
	template<typename R,typename P1>DWORD __stdcall _ThreadCall(void* p){
		struct PARAM : public _struct{
			P1 p1;
			Function<R,P1> OnCall;
		};
		PARAM* pp = (PARAM*)p;
		pp->OnCall(pp->p1);
		delete pp;
		return 1;
	}
	template<typename R,typename P1,typename P2>DWORD __stdcall _ThreadCall(void* p){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			Function<R,P1,P2> OnCall;
		};
		PARAM* pp = (PARAM*)p;
		pp->OnCall(pp->p1,pp->p2);
		delete pp;
		return 1;
	}
	template<typename R,typename P1,typename P2,typename P3>DWORD __stdcall _ThreadCall(void* p){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			Function<R,P1,P2,P3> OnCall;
		};
		PARAM* pp = (PARAM*)p;
		pp->OnCall(pp->p1,pp->p2,pp->p3);
		delete pp;
		return 1;
	}
	template<typename R,typename P1,typename P2,typename P3,typename P4>DWORD __stdcall _ThreadCall(void* p){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			P4 p4;
			Function<R,P1,P2,P3,P4> OnCall;
		};
		PARAM* pp = (PARAM*)p;
		pp->OnCall(pp->p1,pp->p2,pp->p3,pp->p4);
		delete pp;
		return 1;
	}
	template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5>DWORD __stdcall _ThreadCall(void* p){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			P4 p4;
			P5 p5;
			Function<R,P1,P2,P3,P4,P5> OnCall;
		};
		PARAM* pp = (PARAM*)p;
		pp->OnCall(pp->p1,pp->p2,pp->p3,pp->p4,pp->p5);
		delete pp;
		return 1;
	}
	//0������
	template<typename R,typename PROC>bool AsynCallStd(PROC proc){
		struct PARAM : public _struct{
			Function<R> OnCall;
		};
		PARAM* p = new PARAM;
		p->OnCall.Bind(proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R>,&p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//1������
	template<typename R,typename P1,typename PROC>bool AsynCallStd(PROC proc,P1 p1){
		struct PARAM : public _struct{
			P1 p1;
			Function<R,P1> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->OnCall.Bind(proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//2������
	template<typename R,typename P1,typename P2,typename PROC>bool AsynCallStd(PROC proc,P1 p1,P2 p2){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			Function<R,P1,P2> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->p2 = p2;
		p->OnCall.Bind(proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//3������
	template<typename R,typename P1,typename P2,typename P3,typename PROC>bool AsynCallStd(PROC proc,P1 p1,P2 p2,P3 p3){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			Function<R,P1,P2,P3> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->p2 = p2;
		p->p3 = p3;
		p->OnCall.Bind(proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//4������
	template<typename R,typename P1,typename P2,typename P3,typename P4,typename PROC>bool AsynCallStd(PROC proc,P1 p1,P2 p2,P3 p3,P4 p4){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			P4 p4;
			Function<R,P1,P2,P3> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->p2 = p2;
		p->p3 = p3;
		p->p4 = p4;
		p->OnCall.Bind(proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3,P4>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//5������
	template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5,typename PROC>bool AsynCallStd(PROC proc,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			P4 p4;
			P5 p5;
			Function<R,P1,P2,P3,P4,P5> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->p2 = p2;
		p->p3 = p3;
		p->p4 = p4;
		p->p5 = p5;
		p->OnCall.Bind(proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3,P4,P5>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//0������
	template<typename R,typename OBJ,typename PROC>bool AsynCall(OBJ* obj,PROC proc){
		struct PARAM : public _struct{
			Function<R> OnCall;
		};
		PARAM* p = new PARAM;
		p->OnCall.Bind(obj,proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//1������
	template<typename R,typename P1,typename OBJ,typename PROC>bool AsynCall(OBJ* obj,PROC proc,P1 p1){
		struct PARAM : public _struct{
			P1 p1;
			Function<R,P1> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->OnCall.Bind(obj,proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//2������
	template<typename R,typename P1,typename P2,typename OBJ,typename PROC>bool AsynCall(OBJ* obj,PROC proc,P1 p1,P2 p2){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			Function<R,P1,P2> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->p2 = p2;
		p->OnCall.Bind(obj,proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//3������
	template<typename R,typename P1,typename P2,typename P3,typename OBJ,typename PROC>bool AsynCall(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			Function<R,P1,P2,P3> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->p2 = p2;
		p->p3 = p3;
		p->OnCall.Bind(obj,proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//4������
	template<typename R,typename P1,typename P2,typename P3,typename P4,typename OBJ,typename PROC>bool AsynCall(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3,P4 p4){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			P4 p4;
			Function<R,P1,P2,P3> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->p2 = p2;
		p->p3 = p3;
		p->p4 = p4;
		p->OnCall.Bind(obj,proc);
		HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3,P4>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//5������
	template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5,typename OBJ,typename PROC>bool AsynCall(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
		struct PARAM : public _struct{
			P1 p1;
			P2 p2;
			P3 p3;
			P4 p4;
			P5 p5;
			Function<R,P1,P2,P3,P4,P5> OnCall;
		};
		PARAM* p = new PARAM;
		p->p1 = p1;
		p->p2 = p2;
		p->p3 = p3;
		p->p4 = p4;
		p->p5 = p5;
		p->OnCall.Bind(obj,proc);
		HANDLE HANDLE _Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3,P4,P5>,p,0,0);
		if(!_Handle){
			delete p;
			return 0;
		}
		CloseHandle(_Handle);
		return 1;
	}
	//�̵߳Ĺ����ں˼����� 1���˼���Ϊ 0 ʱ�ָ̻߳����У�����ֵ�ǵ���֮ǰ���ں˼���������ʧ�ܷ���-1��
	//�̵߳Ĺ����ں˼����� 1���˼���Ϊ 0 ʱ�ָ̻߳����У�����ֵ�ǵ���֮ǰ���ں˼�����
	//����һ���ں˼���Ϊ 0 ���̣߳��������ټ�С��Ҳ����˵��������Ϊ����������ʧ�ܷ���-1��
	//���ǵ����� Close ����������ʵ������������һ���̡߳����߳�����֮��ʵ���Ͽ�������Threadʵ�����̵߳�ִ�в����ܵ��κ�Ӱ�졣
	//��������غ�������Ҫ�����е�ģ�������д���������Ǻ������ͣ����Ǻ��鷳�ģ����Ծ�����ʹ�����غ�����Ϊ������������ֻ��Ҫ�������ķ���������Ϊģ�庯���ĵ�һ��ģ������Ϳ����ˡ�
	//��Ϊ�̺߳��������첽���õģ����Դ��ݵĲ������������߳������ڼ���Ч�ģ�����ֲ�ʵ����ָ�벻����Ϊ�������ݡ�
	class CONCISE_API Thread : public HandleType<HANDLE>{
	protected:
		DWORD _Id;
	public:
		//0������
		template<typename R,typename PROC>bool CallStd(PROC proc){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				Function<R> OnCall;
			};
			PARAM* p = new PARAM;
			p->OnCall.BindStd(proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//1������
		template<typename R,typename P1,typename PROC>bool CallStd(PROC proc,P1 p1){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				Function<R,P1> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->OnCall.BindStd(proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//2������
		template<typename R,typename P1,typename P2,typename PROC>bool CallStd(PROC proc,P1 p1,P2 p2){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				P2 p2;
				Function<R,P1,P2> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->p2 = p2;
			p->OnCall.BindStd(proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//3������
		template<typename R,typename P1,typename P2,typename P3,typename PROC>bool CallStd(PROC proc,P1 p1,P2 p2,P3 p3){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				P2 p2;
				P3 p3;
				Function<R,P1,P2,P3> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->p2 = p2;
			p->p3 = p3;
			p->OnCall.BindStd(proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//4������
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename PROC>bool CallStd(PROC proc,P1 p1,P2 p2,P3 p3,P4 p4){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
				Function<R,P1,P2,P3> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->p2 = p2;
			p->p3 = p3;
			p->p4 = p4;
			p->OnCall.BindStd(proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3,P4>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//5������
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5,typename PROC>bool CallStd(PROC proc,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
				P5 p5;
				Function<R,P1,P2,P3,P4,P5> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->p2 = p2;
			p->p3 = p3;
			p->p4 = p4;
			p->p5 = p5;
			p->OnCall.BindStd(proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3,P4,P5>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//0������
		template<typename R,typename OBJ,typename PROC>bool Call(OBJ* obj,PROC proc){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				Function<R> OnCall;
			};
			PARAM* p = new PARAM;
			p->OnCall.Bind(obj,proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//1������
		template<typename R,typename P1,typename OBJ,typename PROC>bool Call(OBJ* obj,PROC proc,P1 p1){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				Function<R,P1> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->OnCall.Bind(obj,proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//2������
		template<typename R,typename P1,typename P2,typename OBJ,typename PROC>bool Call(OBJ* obj,PROC proc,P1 p1,P2 p2){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				P2 p2;
				Function<R,P1,P2> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->p2 = p2;
			p->OnCall.Bind(obj,proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//3������
		template<typename R,typename P1,typename P2,typename P3,typename OBJ,typename PROC>bool Call(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				P2 p2;
				P3 p3;
				Function<R,P1,P2,P3> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->p2 = p2;
			p->p3 = p3;
			p->OnCall.Bind(obj,proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//4������
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename OBJ,typename PROC>bool Call(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3,P4 p4){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
				Function<R,P1,P2,P3> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->p2 = p2;
			p->p3 = p3;
			p->p4 = p4;
			p->OnCall.Bind(obj,proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3,P4>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		//5������
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5,typename OBJ,typename PROC>bool Call(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
			if(_Handle) return 0;
			struct PARAM : public _struct{
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
				P5 p5;
				Function<R,P1,P2,P3,P4,P5> OnCall;
			};
			PARAM* p = new PARAM;
			p->p1 = p1;
			p->p2 = p2;
			p->p3 = p3;
			p->p4 = p4;
			p->p5 = p5;
			p->OnCall.Bind(obj,proc);
			_Handle = ::CreateThread(0,0,&_ThreadCall<R,P1,P2,P3,P4,P5>,p,0,&_Id);
			if(!_Handle){
				delete p;
				return 0;
			}
			return 1;
		}
		Thread():_Id(0){}
		//�ȴ��߳̽�����
		DWORD Wait(uint ms = -1){
			return ::WaitForSingleObject(_Handle,ms);
		}
		//�̵߳Ĺ����ں˼����� 1���˼���Ϊ 0 ʱ�ָ̻߳����У�����ֵ�ǵ���֮ǰ���ں˼�����
		int Pause(){
			return ::SuspendThread(_Handle);
		}
		//�̵߳Ĺ����ں˼����� 1���˼���Ϊ 0 ʱ�ָ̻߳����У�����ֵ�ǵ���֮ǰ���ں˼���������һ���ں˼���Ϊ 0 ���̣߳��������ټ�С��Ҳ����˵��������Ϊ������
		int Resume(){
			return ::ResumeThread(_Handle);
		}
		//�ر��߳̾���������߳̽����κβ��������ǵȴ������᷵�ء�
		bool Close(){
			if(!_Handle) return 0;
			if(!CloseHandle(_Handle)){
				_ASSERT(0);
			}
			_Handle = 0;
			_Id = 0;
			return 1;
		}
		//ǿ�ƽ����߳����У������̵߳Ķ�ջ���������
		bool Stop(DWORD code = 0){
			if(!_Handle) return 0;
			::TerminateThread(_Handle,code);
			::CloseHandle(_Handle);
			_Handle = 0;
			return 1;
		}
		//�߳��Ƿ�������״̬��
		inline DWORD GetID(){
			return _Id;
		}
		bool IsRunning(){
			if(!_Handle) return 0;
			DWORD code = -1;
			GetExitCodeThread(_Handle,&code);
			return code==STILL_ACTIVE;
		}
		~Thread(){
			Close();
		}
	};
	//#define THREAD_STATUS_NULL		0	//�߳�Ϊ�գ�������Ҫ�˳�
#define THREAD_STATUS_SUSPEND	1	//�߳̾�������������
#define THREAD_STATUS_RUNNING	2	//����״̬

#define THREAD_FLAG_SUSPEND		0
#define THREAD_FLAG_CONTINUE	1
#define THREAD_FLAG_EXIT		2	//�����߳�
	struct _StaticThreadParam : public _struct{
		DWORD Id;		//�߳�ID
		LONG Status;	//THREAD_STATUS_NULL,THREAD_STATUS_READY,THREAD_STATUS_RUN
		LONG Flag;		//
		HANDLE Handle;	//�߳̾��
		Event* exit;
		LPTHREAD_START_ROUTINE routine;
		_StaticThreadParam():Status(THREAD_STATUS_SUSPEND),Flag(THREAD_FLAG_SUSPEND){}
	};
	template<typename PARAM> bool checkState(PARAM pp){
		//��ʱFlag��3�ֿ��ܣ�Suspend��Continue��Exit�����״̬ΪExit���˳�ѭ��
		if(THREAD_FLAG_EXIT==::InterlockedCompareExchange(&pp->Flag,THREAD_FLAG_EXIT,THREAD_FLAG_EXIT)){
			return true;
		}
		//��ʱ״ֻ̬����ΪContinue����Suspend�����״̬ΪSuspend����ִ�й����̣߳����������
		if(THREAD_FLAG_SUSPEND==::InterlockedCompareExchange(&pp->Flag,THREAD_FLAG_SUSPEND,THREAD_FLAG_CONTINUE)){
			//��״̬��ΪSuspend��
			::InterlockedCompareExchange(&pp->Status,THREAD_STATUS_SUSPEND,pp->Status);
			//�����ʱ������Start��״̬����ΪRunning�������Running�������̡߳�
			if(THREAD_STATUS_RUNNING!=::InterlockedCompareExchange(&pp->Status,pp->Status,THREAD_STATUS_RUNNING)){
				::SuspendThread(pp->Handle);
			}
		}
		//�����Suspend�ָ�����Ҫ����Ƿ��������˳���־��
		if(THREAD_FLAG_EXIT==::InterlockedCompareExchange(&pp->Flag,THREAD_FLAG_EXIT,THREAD_FLAG_EXIT)){
			return true;
		}
		return false;
	}
	template<typename R>DWORD __stdcall _StaticThreadCall(void* p){
		struct PARAM : public _StaticThreadParam{
			Function<R> OnCall;
		};
		PARAM* pp = (PARAM*)p;
		while(1){
			pp->OnCall();
			pp->exit->Signal();
			if(checkState(pp)) break;
		}
		//delete pp;
		return 1;
	}
	template<typename R,typename P1>DWORD __stdcall _StaticThreadCall(void* p){
		struct PARAM : public _StaticThreadParam{
			Function<R,P1> OnCall;
			P1 p1;
		};
		PARAM* pp = (PARAM*)p;
		while(1){
			pp->OnCall(pp->p1);
			pp->exit->Signal();
			if(checkState(pp)) break;
		}
		return 1;
	}
	template<typename R,typename P1,typename P2>DWORD __stdcall _StaticThreadCall(void* p){
		struct PARAM : public _StaticThreadParam{
			Function<R,P1,P2> OnCall;
			P1 p1;
			P2 p2;
		};
		PARAM* pp = (PARAM*)p;
		while(1){
			pp->OnCall(pp->p1,pp->p2);
			pp->exit->Signal();
			if(checkState(pp)) break;
		}
		//delete pp;
		return 1;
	}
	template<typename R,typename P1,typename P2,typename P3>DWORD __stdcall _StaticThreadCall(void* p){
		struct PARAM : public _StaticThreadParam{
			Function<R,P1,P2,P3> OnCall;
			P1 p1;
			P2 p2;
			P3 p3;
		};
		PARAM* pp = (PARAM*)p;
		while(1){
			pp->OnCall(pp->p1,pp->p2,pp->p3);
			pp->exit->Signal();
			if(checkState(pp)) break;
		}
		//delete pp;
		return 1;
	}
	template<typename R,typename P1,typename P2,typename P3,typename P4>DWORD __stdcall _StaticThreadCall(void* p){
		struct PARAM : public _StaticThreadParam{
			Function<R,P1,P2,P3,P4> OnCall;
			P1 p1;
			P2 p2;
			P3 p3;
			P4 p4;
		};
		PARAM* pp = (PARAM*)p;
		while(1){
			pp->OnCall(pp->p1,pp->p2,pp->p3,pp->p4);
			pp->exit->Signal();
			if(checkState(pp)) break;
		}
		//delete pp;
		return 1;
	}
	template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5>DWORD __stdcall _StaticThreadCall(void* p){
		struct PARAM : public _StaticThreadParam{
			Function<R,P1,P2,P3,P4,P5> OnCall;
			P1 p1;
			P2 p2;
			P3 p3;
			P4 p4;
			P5 p5;
		};
		PARAM* pp = (PARAM*)p;
		while(1){
			pp->OnCall(pp->p1,pp->p2,pp->p3,pp->p4,pp->p5);
			pp->exit->Signal();
			if(checkState(pp)) break;
		}
		//delete pp;
		return 1;
	}
	class CONCISE_API _StaticThread : public HandleType<_StaticThreadParam*> {
	protected:
		CriticalSection _Cs;
		Event		_evExit;
	public:
		_StaticThread();
		//�ȴ��߳̽������������������̣߳�����SetClose���������̲߳������̺߳������ض�������
		DWORD Wait(uint ms = -1);
		//�����߳��˳���־����������������У�������������߳��˳�����������Ѿ��������߳������˳���
		void SetClose();
		//��������Ѿ��������˳��̣߳����ҹرվ�����������û�н������ȴ�������������˳��̣߳��رվ����
		//�ȴ��߳��˳������ָ����ʱ�����߳�û���˳���ǿ�ƽ����߳�
		void Close(uint timeOut = -1);
		//��������
		bool Pause();
		//�����̺߳����������ʱû�����ûص����������ߵ�����Close������û���������ûص�������
		//���߻ص���������ִ�У��᷵��0��wait����ָ�����߳��Ѿ�����������£�����µĺ���û��������
		//�Ƿ�ȴ�ǰ��ĺ��н������Ӷ��ɹ�����.
		//����Ѿ�������SetClose������������� 0��
		//�ɹ�����һ��Eventָ�룬���Event��ָ���ĺ���ִ�����ᱻSigned��
		Event* Start(bool wait = 0);
		//�ص��������ڱ�ִ��
		bool IsRunning();
		//�������Ӧ���ڻص������ﱻ���У������ʧ�ܣ�����ָʾ������ʼ��һ�λص����������Start������
		inline void SetContinue(bool cont = true);
		//ǿ�ƽ����߳����У������̵߳Ķ�ջ���������
		bool Stop(DWORD code = 0);
		//�����̵߳�ID������߳�û�г�ʼ�������� -1 ��
		DWORD GetID();
		~_StaticThread();
	};
	/*StaticThread��Thread���÷�������ͬ���������ڻص��ĺ������غ󲢲��˳��̣߳����ǵȴ���һ��
	Start���ã�����ִ��ָ�������񣨻ص�����������Ψһ�����þ�������ҪƵ�������˳��̵߳ĳ��ϣ�
	����CPU�����ִ����˳��̵߳Ĺ�����

	���StaticThread��SetCall��SetCallStd���õĺ���������ͬһ�����ͣ�����ֻ�е�һ�γɹ���
	��һ�ε���ȷ������ص������ĸ�ʽ��Ҫ�ı��ʽ����Ҫ��Close����

	StaticThreadֻ��2��״̬��Suspend��Running������ͨ��IsRunning����ȡ������״̬�����������״
	̬�µ��� SetCall �� SetCallStd �����������������1���ص��Ѿ������У���ʱ������
	�Ĳ���ֱ���´κ��в������ã�2.�ص���δ�����У���ʱִ�������õĲ�����ִ����Start�󣬻ص���һ��
	�Ѿ���ִ�У�*/
	class CONCISETL_API StaticThread : public _StaticThread{
	public:
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5,typename PROC> void SetCallStd(PROC proc,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3,P4,P5> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
				P5 p5;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 5;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1,P2,P3,P4,P5>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=5){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
			((PARAM*)_Handle)->p4 = p4;
			((PARAM*)_Handle)->p5 = p5;
		}
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5,typename OBJ,typename PROC> void SetCall(OBJ obj,PROC proc,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3,P4,P5> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
				P5 p5;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 5;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1,P2,P3,P4,P5>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=5){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
			((PARAM*)_Handle)->p4 = p4;
			((PARAM*)_Handle)->p5 = p5;
		}
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename PROC> void SetCallStd(PROC proc,P1 p1,P2 p2,P3 p3,P4 p4){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3,P4> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
			};

			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 4;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1,P2,P3,P4>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=4){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
			((PARAM*)_Handle)->p4 = p4;
		}
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename OBJ,typename PROC> void SetCall(OBJ obj,PROC proc,P1 p1,P2 p2,P3 p3,P4 p4){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3,P4> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
			};

			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 4;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1,P2,P3,P4>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=4){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
			((PARAM*)_Handle)->p4 = p4;
		}
		template<typename R,typename P1,typename P2,typename P3,typename PROC> void SetCallStd(PROC proc,P1 p1,P2 p2,P3 p3){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
			};

			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 3;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1,P2,P3>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=3){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
		}
		template<typename R,typename P1,typename P2,typename P3,typename OBJ,typename PROC> void SetCall(OBJ obj,PROC proc,P1 p1,P2 p2,P3 p3){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
			};

			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 3;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1,P2,P3>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=3){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
		}
		template<typename R,typename P1,typename P2,typename PROC> void SetCallStd(PROC proc,P1 p1,P2 p2){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2> OnCall;
				P1 p1;
				P2 p2;
			};

			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 2;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1,P2>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=2){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
		}
		template<typename R,typename P1,typename P2,typename OBJ,typename PROC> void SetCall(OBJ obj,PROC proc,P1 p1,P2 p2){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2> OnCall;
				P1 p1;
				P2 p2;
			};

			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 2;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1,P2>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=2){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
		}
		template<typename R,typename P1,typename PROC> void SetCallStd(PROC proc,P1 p1){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1> OnCall;
				P1 p1;
			};

			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
		}
		template<typename R,typename P1,typename OBJ,typename PROC> void SetCall(OBJ obj,PROC proc,P1 p1){	
			struct PARAM : public _StaticThreadParam{
				Function<R,P1> OnCall;
				P1 p1;
			};

			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->ParamCount = 1;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R,P1>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			if(_Handle->ParamCount!=1){
				_ASSERT(0);
				return;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
		}
		template<typename R,typename PROC> void SetCallStd(PROC proc){	
			struct PARAM : public _StaticThreadParam{
				Function<R> OnCall;
			};
			LocalCriticalSection lcs(_Cs);
			static bool firstcall = 1;
			if(firstcall){
				firstcall = 0;
				_Handle = new PARAM;
				_Handle->ParamCount = 0;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			/*if(_Handle->ParamCount!=0){
			_ASSERT(0);
			return;
			}*/
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
		}
		template<typename R,typename OBJ,typename PROC> void SetCall(OBJ obj,PROC proc){	
			struct PARAM : public _StaticThreadParam{
				Function<R> OnCall;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->Handle = ::CreateThread(0,0,&_StaticThreadCall<R>,_Handle,CREATE_SUSPENDED,&_Handle->Id);
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
		}
	};
	template<typename R,typename P1 = NullType,typename P2 = NullType,typename P3 = NullType,typename P4 = NullType,typename P5 = NullType> class CONCISETL_API StaticThread2 : public _StaticThread{
	public:
		template<typename PROC> void SetCallStd(PROC proc,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3,P4,P5> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
				P5 p5;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1,P2,P3,P4,P5>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
			((PARAM*)_Handle)->p4 = p4;
			((PARAM*)_Handle)->p5 = p5;
		}
		template<typename OBJ,typename PROC> void SetCall(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3,P4,P5> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
				P5 p5;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1,P2,P3,P4,P5>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
			((PARAM*)_Handle)->p4 = p4;
			((PARAM*)_Handle)->p5 = p5;
		}
	};
	template<typename R,typename P1,typename P2,typename P3,typename P4> class CONCISETL_API StaticThread2<R,P1,P2,P3,P4> : public _StaticThread{
	public:
		template<typename PROC> void SetCallStd(PROC proc,P1 p1,P2 p2,P3 p3,P4 p4){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3,P4> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1,P2,P3,P4>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
			((PARAM*)_Handle)->p4 = p4;
		}
		template<typename OBJ,typename PROC> void SetCall(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3,P4 p4){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3,P4> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
				P4 p4;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1,P2,P3,P4>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
			((PARAM*)_Handle)->p4 = p4;
		}
	};
	template<typename R,typename P1,typename P2,typename P3> class CONCISETL_API StaticThread2<R,P1,P2,P3> : public _StaticThread{
	public:
		template<typename PROC> void SetCallStd(PROC proc,P1 p1,P2 p2,P3 p3){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1,P2,P3>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
		}
		template<typename OBJ,typename PROC> void SetCall(OBJ* obj,PROC proc,P1 p1,P2 p2,P3 p3){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2,P3> OnCall;
				P1 p1;
				P2 p2;
				P3 p3;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1,P2,P3>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
			((PARAM*)_Handle)->p3 = p3;
		}
	};
	template<typename R,typename P1,typename P2> class CONCISETL_API StaticThread2<R,P1,P2> : public _StaticThread{
	public:
		template<typename PROC> void SetCallStd(PROC proc,P1 p1,P2 p2){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2> OnCall;
				P1 p1;
				P2 p2;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1,P2>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
		}
		template<typename OBJ,typename PROC> void SetCall(OBJ* obj,PROC proc,P1 p1,P2 p2){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1,P2> OnCall;
				P1 p1;
				P2 p2;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1,P2>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
			((PARAM*)_Handle)->p2 = p2;
		}
	};
	template<typename R,typename P1> class CONCISETL_API StaticThread2<R,P1> : public _StaticThread{
	public:
		template<typename PROC> void SetCallStd(PROC proc,P1 p1){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1> OnCall;
				P1 p1;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
			((PARAM*)_Handle)->p1 = p1;
		}
		template<typename OBJ,typename PROC> void SetCall(OBJ* obj,PROC proc,P1 p1){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1> OnCall;
				P1 p1;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R,P1>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
			((PARAM*)_Handle)->p1 = p1;
		}
	};
	template<typename R> class CONCISETL_API StaticThread2<R> : public _StaticThread{
	public:
		template<typename PROC> void SetCallStd(PROC proc){
			struct PARAM : public _StaticThreadParam{
				Function<R> OnCall;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.BindStd(proc);
		}
		template<typename OBJ,typename PROC> void SetCall(OBJ* obj,PROC proc){
			struct PARAM : public _StaticThreadParam{
				Function<R,P1> OnCall;
			};
			LocalCriticalSection lcs(_Cs);
			if(_Handle==0){
				_Handle = new PARAM;
				_Handle->routine = &_StaticThreadCall<R>;
			}
			_Handle->exit = &_evExit;
			((PARAM*)_Handle)->OnCall.Bind(obj,proc);
		}
	};
	class CONCISETL_API _TaskPool : public _class{
	protected:
		CriticalSection		_cs;
		uint				_cleanPeroid;		//0.1SΪ��λ
		uint				_maxThreadCount;
		Thread				_cleanThread;
		bool				_exitCleanThread;
		ObjectList<StaticThread>	_threadList;

		void _onClean();
		StaticThread* _getThread();
	public:
		_TaskPool();
		~_TaskPool();
		//���������߳�
		void StartCleanThread(uint period);
		//�������������߳���
		void SetMaxThread(int maxCount);
		//���ص�ǰ���е��߳���
		int GetTaskCount();
		//���ص�ǰ���е��߳���
		int GetIdleCount();
		//���ؼ�����߳������������߳���+�����߳���
		int GetThreadCount();
		//�ر�����̳߳ض������������ȴ������߳��˳��Ż᷵�ء�
		//���ָ����ʱ���ڣ��߳�û�н�����ǿ�ƽ����̡߳�
		void Close(uint timeOut = -1);
	};
	/* TaskPool ������ʱ��ȴ����е��߳��˳�������������һ����ʹ������ʱ��Ҫ��֤TaskPool������
	���е�ʵ�����٣����ڳ�Ա�б�������棩�����򡣿����߳���Ȼ��ִ�У���Щ�������ٺ�ͻ�ʹ
	������������˳����̳߳���ʱ����������Ҳ�����ж��̳߳�����Ҫ�����һ�����⡪������
	�˳�ʱ���������˳�����⡣һ�����ȵ����˳��̵߳ĺ�����TaskPool�ȴ����˳������󣬿�ʼ������������

	����������ʱ����Ҫ�����κ����ݣ�����ʹ��ExitProcessѸ�ٽ������̡�
	*/
	class CONCISETL_API TaskPool : public _TaskPool{
	public:
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5,typename PROC> Event* StartStdTask(PROC cb,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCallStd<R>(cb,p1,p2,p3,p4,p5);
			return pst->Start();
		}
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename P5,typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCall<R>(obj,cb,p1,p2,p3,p4,p5);
			return pst->Start();
		}
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename PROC> Event* StartStdTask(PROC cb,P1 p1,P2 p2,P3 p3,P4 p4){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCallStd<R>(cb,p1,p2,p3,p4);
			return pst->Start();
		}
		template<typename R,typename P1,typename P2,typename P3,typename P4,typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1,P2 p2,P3 p3,P4 p4){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCall<R>(obj,cb,p1,p2,p3,p4);
			return pst->Start();
		}
		template<typename R,typename P1,typename P2,typename P3,typename PROC> Event* StartStdTask(PROC cb,P1 p1,P2 p2,P3 p3){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCallStd<R>(cb,p1,p2,p3);
			return pst->Start();
		}
		template<typename R,typename P1,typename P2,typename P3,typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1,P2 p2,P3 p3){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCall<R>(obj,cb,p1,p2,p3);
			return pst->Start();
		}
		template<typename R,typename P1,typename P2,typename PROC> Event* StartStdTask(PROC cb,P1 p1,P2 p2){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCallStd<R>(cb,p1,p2);
			return pst->Start();
		}
		template<typename R,typename P1,typename P2,typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1,P2 p2){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCall<R>(obj,cb,p1,p2);
			return pst->Start();
		}
		template<typename R,typename P1,typename PROC> Event* StartStdTask(PROC cb,P1 p1){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCallStd<R>(cb,p1);
			return pst->Start();
		}
		template<typename R,typename P1,typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCall<R>(obj,cb,p1);
			return pst->Start();
		}
		template<typename R,typename PROC> Event* StartStdTask(PROC cb){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCallStd<R>(cb);
			return pst->Start();
		}
		template<typename R,typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread* pst = _getThread();
			pst->SetCall<R>(obj,cb);
			return pst->Start();
		}
	};
	class CONCISETL_API _TaskPool2 : public _class{
	protected:
		CriticalSection		_cs;
		uint				_cleanPeroid;		//0.1SΪ��λ
		uint				_maxThreadCount;
		Thread				_cleanThread;
		bool				_exitCleanThread;
		ObjectList<_StaticThread>	_threadList;

		void _onClean();
		_StaticThread* _getThread();
	public:
		_TaskPool2();
		~_TaskPool2();
		//���������߳�
		void StartCleanThread(uint period);
		//�������������߳���
		void SetMaxThread(int maxCount);
		//���ص�ǰ���е��߳���
		int GetTaskCount();
		//���ص�ǰ���е��߳���
		int GetIdleCount();
		//���ؼ�����߳������������߳���+�����߳���
		int GetThreadCount();
		//�ر�����̳߳ض������������ȴ������߳��˳��Ż᷵�ء�
		//���ָ����ʱ���ڣ��߳�û�н�����ǿ�ƽ����̡߳�
		void Close(uint timeOut = -1);
	};
	template<typename R,typename P1 = NullType,typename P2 = NullType,typename P3 = NullType,typename P4 = NullType,typename P5 = NullType> class CONCISETL_API TaskPool2 : public _TaskPool2{
	public:
		template<typename PROC> Event* StartStdTask(PROC cb,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1,P2,P3,P4,P5>* pst = (StaticThread2<R,P1,P2,P3,P4,P5>*)_getThread();
			pst->SetCallStd(cb,p1,p2,p3,p4,p5);
			return pst->Start();
		}
		template<typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1,P2,P3,P4,P5>* pst = (StaticThread2<R,P1,P2,P3,P4,P5>*)_getThread();
			pst->SetCall(obj,cb,p1,p2,p3,p4,p5);
			return pst->Start();
		}
	};
	template<typename R,typename P1,typename P2,typename P3,typename P4> class CONCISETL_API TaskPool2<R,P1,P2,P3,P4> : public _TaskPool2{
	public:
		template<typename PROC> Event* StartStdTask(PROC cb,P1 p1,P2 p2,P3 p3,P4 p4){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1,P2,P3,P4>* pst = (StaticThread2<R,P1,P2,P3,P4>*)_getThread();
			pst->SetCallStd(cb,p1,p2,p3,p4);
			return pst->Start();
		}
		template<typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1,P2 p2,P3 p3,P4 p4){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1,P2,P3,P4>* pst = (StaticThread2<R,P1,P2,P3,P4>*)_getThread();
			pst->SetCall(obj,cb,p1,p2,p3,p4);
			return pst->Start();
		}
	};
	template<typename R,typename P1,typename P2,typename P3> class CONCISETL_API TaskPool2<R,P1,P2,P3> : public _TaskPool2{
	public:
		template<typename PROC> Event* StartStdTask(PROC cb,P1 p1,P2 p2,P3 p3){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1,P2,P3>* pst = (StaticThread2<R,P1,P2,P3>*)_getThread();
			pst->SetCallStd(cb,p1,p2,p3);
			return pst->Start();
		}
		template<typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1,P2 p2,P3 p3){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1,P2,P3>* pst = (StaticThread2<R,P1,P2,P3>*)_getThread();
			pst->SetCall(obj,cb,p1,p2,p3);
			return pst->Start();
		}
	};
	template<typename R,typename P1,typename P2> class CONCISETL_API TaskPool2<R,P1,P2> : public _TaskPool2{
	public:
		template<typename PROC> Event* StartStdTask(PROC cb,P1 p1,P2 p2){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1,P2>* pst = (StaticThread2<R,P1,P2>*)_getThread();
			pst->SetCallStd(cb,p1,p2);
			return pst->Start();
		}
		template<typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1,P2 p2){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1,P2>* pst = (StaticThread2<R,P1,P2>*)_getThread();
			pst->SetCall(obj,cb,p1,p2);
			return pst->Start();
		}
	};
	template<typename R,typename P1> class CONCISETL_API TaskPool2<R,P1> : public _TaskPool2{
	public:
		template<typename PROC> Event* StartStdTask(PROC cb,P1 p1){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1>* pst = (StaticThread2<R,P1>*)_getThread();
			pst->SetCallStd(cb,p1);
			return pst->Start();
		}
		template<typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb,P1 p1){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R,P1>* pst = (StaticThread2<R,P1>*)_getThread();
			pst->SetCall(obj,cb,p1);
			return pst->Start();
		}
	};
	template<typename R> class CONCISETL_API TaskPool2<R> : public _TaskPool2{
	public:
		template<typename PROC> Event* StartStdTask(PROC cb){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R>* pst = (StaticThread2<R>*)_getThread();
			pst->SetCallStd(cb);
			return pst->Start();
		}
		template<typename OBJ,typename PROC> Event* StartTask(OBJ obj,PROC cb){
			LocalCriticalSection lcs(_cs);
			if(_threadList.Count()>=_maxThreadCount) return 0;
			StaticThread2<R>* pst = (StaticThread2<R>*)_getThread();
			pst->SetCall(obj,cb);
			return pst->Start();
		}
	};
	//����������̵�һ��ΨһTaskPool����һ������ʵ���ϲ�����Ҫ�ܶ��TaskPool����ʹ��һ��Ψһ��TaskPool���������̣߳�Ч�ʸ��ߡ�
	CONCISE_API TaskPool* ProcessTaskPool(bool release = false);	
	//��ȡ��ǰ�̵߳ľ������GetCurrentThread��ͬ������������صľ������α��������Ա������߳�ʹ�á�
	CONCISE_API HANDLE DuplicateCurrentThread();
	//���ص�ǰ��ִ���ļ���ȫ·������.
	CONCISE_API int GetCurrentExeFileName(String& str);
	typedef struct CONCISE_API Guid : public _struct,public GUID{
		//�Զ�����һ��GUID
		bool Create();
		//���һ����׼GUID�ִ���
		void ToString(String& str);
		//��һ����׼GUID�ִ���ʼ��
		void FromString(LPCWSTR str);
		//���һ���ֽ��ִ���
		void ToByteString(String& str);
		//��һ��32���ֽ��ַ�����ʼ����
		void FromByteString(LPCWSTR str);
		//ȡ���ֽ����ݣ�buf����Ϊ16���ֽڡ�
		void ToByte(unsigned char* buf){memcpy(buf,this,16);}
		//�����ֽ����ݣ�buf����Ϊ16���ֽڡ�
		inline void FromByte(unsigned char* buf){memcpy(this,buf,16);}
		//�ж�����GUID�Ƿ���ͬ��
		inline bool operator == (Guid& guid){return memcmp(this,&guid,16)==0;}
	}Guid;
}
