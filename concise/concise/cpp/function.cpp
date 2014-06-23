#include "../stdafx.h"
#include "../../concise.h"

namespace cs{
#pragma pack(push, 1)
	//��������ڰ����Ա����ת��Ϊ__stdcall����.��64λƽֱ̨�Ӳ���������������DEP����.Ӧ��ʹ��Delegate.
	class CProcToStdcall : public _class{
	private:
		BYTE    _mov;          // mov ecx, %pThis
		DWORD   _this;         //
		BYTE    _jmp;          // jmp func
		DWORD   _relproc;      // relative jmp
	public:
		template<typename T>void Bind(void* object,T proc,int n = 0){
			if(proc){
				_mov = 0xB9;
				_jmp = 0xE9;
				_this = (DWORD)(INT_PTR)object;
				_relproc = SuperCast<DWORD,T>(proc) - (DWORD)(INT_PTR)this - sizeof(CProcToStdcall);
			}else{
				if(n){
					_mov = 0xc2;
					_this = n;
				}else{
					_mov = 0xc3;
				}
			}
			FlushInstructionCache(GetCurrentProcess(),this,sizeof(CProcToStdcall));
		}
		inline bool IsStdcall(){
			return _mov==0xBA;
		}
		inline bool IsThiscall(){
			return _mov==0xB9;
		}
		template<typename T>inline void BindStd(T proc,int n = 0){
			Bind(0,proc,n);
			/*if(proc){
				_mov = 0xBA;
				_jmp = 0xE9;
				_this = (DWORD)(INT_PTR)0;
				_relproc = SuperCast<DWORD,T>(proc) - (DWORD)(INT_PTR)this - sizeof(CProcToStdcall);
			}else{
				if(n){
					_mov = 0xc2;
					_this = n;
				}else{
					_mov = 0xc3;
				}
			}
			FlushInstructionCache(GetCurrentProcess(),this,sizeof(CProcToStdcall));*/
		}
		void* GetObject(){
			return (void*)(INT_PTR)_this;
		}
		void* GetFunction(){
			return (void*)((char*)this + _relproc + sizeof(CProcToStdcall));
		}
		void operator = (CProcToStdcall& cp){
			memcpy(this,&cp,sizeof(CProcToStdcall));
			_relproc += SuperCast<DWORD,CProcToStdcall*>(&cp) - SuperCast<DWORD,CProcToStdcall*>(this);
			FlushInstructionCache(GetCurrentProcess(),this,sizeof(CProcToStdcall));
		}
		template<typename T>T Procedure(){
			return (T)(INT_PTR)this;
		}
	};
#pragma pack(pop)
	void* _CProcedure(void* p){
		//static CodeMemory cm;
		if(p){
			GetGlobalExecMemory()->Free((CProcToStdcall*)p);
			return 0;
		}
		else
			return GetGlobalExecMemory()->Alloc(10);
	}
	void _InitCProcdure(void* obj,void* func,void* pointer,int n){
		((CProcToStdcall*)pointer)->Bind(obj,func,n);
	}
	void _InitStdProcdure(void* func,void* pointer,int n){
		((CProcToStdcall*)pointer)->BindStd(func,n);
	}
	_StdClassProc::~_StdClassProc(){
		if(_Handle) _CProcedure(_Handle);
	}
	void _StdClassProc::Release(){
		if(_Handle) _CProcedure(_Handle);
	}
	bool _StdClassProc::IsEmpty(){
		if(_Handle==NULL) return 0;
		return ((*(char*)_Handle)==(char)0xc2)||((*(char*)_Handle)==(char)0xc3);
	}
	bool _StdClassProc::IsThiscall(){
		if(_Handle==NULL) return 0;
		return ((CProcToStdcall*)_Handle)->IsThiscall();
	}
	bool _StdClassProc::IsStdcall(){
		if(_Handle==NULL) return 0;
		return ((CProcToStdcall*)_Handle)->IsStdcall();
	}
	void _StdClassProc::operator = (_StdClassProc& scp)
	{
		if(scp._Handle==0)
		{
			if(_Handle!=0)
			{
				_CProcedure(_Handle);
				_Handle = 0;
			}
			return;
		}
		if(_Handle==0) _Handle = _CProcedure();
		*(CProcToStdcall*)_Handle = *(CProcToStdcall*)scp._Handle;
		FlushInstructionCache(GetCurrentProcess(),this,sizeof(CProcToStdcall));
	}
	void* _StdClassProc::GetObject()
	{
		return ((CProcToStdcall*)_Handle)->GetObject();
	}
	void* _StdClassProc::GetFunction()
	{
		return ((CProcToStdcall*)_Handle)->GetFunction();
	}
	bool _StdClassProc::operator == (_StdClassProc& scp)
	{
		if(GetObject()!=scp.GetObject()) return false;
		if(GetFunction()!=scp.GetFunction()) return false;
		return true;
	}
	//class _Delegate
	bool _delegate::Delete(uint index)
	{
		if(index>=_Ptrs.Count()) return false;
		_CProcedure(_Ptrs[index]);
		return _Ptrs.Delete(index)!=0;
	}
	void* _delegate::GetObject(uint index)
	{
		if(index>=_Ptrs.Count()) return 0;
		return ((CProcToStdcall*)_Ptrs[index])->GetObject();
	}
	void* _delegate::GetFunction(uint index)
	{
		if(index>=_Ptrs.Count()) return 0;
		return ((CProcToStdcall*)_Ptrs[index])->GetFunction();
	}
	void _delegate::operator = (_delegate& dt)
	{
		if(&dt==this) return;
		for(uint i=0;i<_Ptrs.Count();i++)
		{
			_CProcedure(_Ptrs[i]);
		}
		_Ptrs.Clear();
		_Ptrs.Expand(dt.Count());
		for(uint i=0;i<dt.Count();i++)
		{
			void* p = _CProcedure();
			_InitCProcdure(dt.GetObject(i),dt.GetFunction(i),p);
			_Ptrs[i] = p;
		}
	}
	_delegate::~_delegate()
	{
		for(uint i=0;i<_Ptrs.Count();i++)
		{
			_CProcedure(_Ptrs[i]);
		}
	}
	APIHook::APIHook():_orgFunc(0){
	}
	APIHook::~APIHook(){
		if(_orgFunc) GetGlobalExecMemory()->Free(_orgFunc);
	}
	bool APIHook::SetHook(void* api,void* hook,int pSize){
		if(_orgFunc==NULL){
			_orgFunc = (uchar*)GetGlobalExecMemory()->Alloc(16);
			if(_orgFunc==NULL) return 0;
		}
		HANDLE hProcess = GetCurrentProcess();
		DWORD dwOldProtect;
		//��ȡԭ��������10���ֽڣ�
		if(!VirtualProtect((void*)api,10, PAGE_EXECUTE_WRITECOPY, &dwOldProtect)) return 0;
		DWORD size = 0;
		//�� API �������� 10 ���ֽ�д�� _orgFunc����������ԭ������_orgFunc�ṹ����Ч
		//��С��15�ֽڣ���5���ֽ����һ����ת������ԭ�����ĵ�11���ֽڡ��������� _orgFunc
		//�ͺ���ԭ�������ڻ�������һ���ģ�ֻ�ǵ�11���ֽڶ���һ����ת�����ڰ�������
		//���������������ط�ִ�С�
		ReadProcessMemory(hProcess,api,_orgFunc,10,&size);
		if(size==0) return 0;
		//����_StdClassProc��ԭ������ǰ10���ֽڸ�ΪFunction�ṹ����������ԭ������api������
		//����ת���󶨵ĺ�����hook����
		_StdClassProc scp;
		scp.Attach(api);
		scp.BindStd(hook,pSize);
		scp.Detach();
		//����ԭ�������ã�ԭ����Ǻ��������ַ������ת�� API �� 15�ֽ�֮�� 
		*(_orgFunc+10) = 0xe9;
		//10��ԭ�����������ڵ�ƫ�ƣ�15�Ǳ��ṹ�Ĵ�С(���Ƿ�����ڴ��С�����ṹ������16�ֽ��ڴ棬����ֻʹ����15�ֽڣ�
		DWORD addr = *(DWORD*)(_orgFunc+11) = (DWORD)(UINT_PTR)api + 10 - 15 - (UINT_PTR)_orgFunc;
		FlushInstructionCache(GetCurrentProcess(),_orgFunc,16);
		FlushInstructionCache(GetCurrentProcess(),api,10);
		return 1;
	}
	bool APIHook::UnHook(){
		if(!_orgFunc) return 0;
		//����ԭ��ַ
		uchar* api = (uchar*)(*(DWORD*)(_orgFunc+11) - 10 + 15 + (UINT_PTR)_orgFunc);
		//�ָ�ǰ10���ֽ�����
		HANDLE hProcess = GetCurrentProcess();
		DWORD dwOldProtect;
		//��ȡԭ��������10���ֽڣ�
		if(!VirtualProtect((void*)api,10, PAGE_EXECUTE_WRITECOPY, &dwOldProtect)) return 0;
		DWORD size = 0;
		WriteProcessMemory(hProcess,api,_orgFunc,10,&size);
		GetGlobalExecMemory()->Free(_orgFunc);
		_orgFunc = 0;
		FlushInstructionCache(GetCurrentProcess(),api,10);
		return 1;
	}
}