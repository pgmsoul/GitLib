// jslib.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "v8base.h"
/*@class API
@text API������������ϵͳ�� API ����. Windowsϵͳ��API�������������2���汾, "A"��"W", �����ȡ�����ı��ĺ��� GetWindowText, ��������GetWindowTextA��GetWindowTextW��������, C++��������ȫ������, ����Unicode����ANSI��ȷ��ʹ���Ǹ��汾�ĺ���.
����C++�����е�API����һ��ֻ��Ҫд������, ����Ҫ����A��W��׺. ����ʵ���ϲ�û��GetWindowText�������, ������ʹ��API���ʱ��, ����ָ��ʹ���Ǹ��汾�ĺ���. ��Ȼ, �����ϲ�Ҫʹ��A�汾�ĺ���, ��ΪA�汾�������ִ���������ֱ�Ӵ�JavaScript�ִ�, ��Ҫ��
CMemory ������ģ��. <b>�������еĺ�������A��W�����汾</b>, ������ĺ���ֻ��W�汾����ֻ��A�汾, ���������Ҫ����MSDN�ĵ�.
@text ʹ��API�����ϵͳAPI��ʱ��, ����ֻ��3��: ����, �ִ�, CMemory. ����API������Ҫָ����������ֵĲ���, ȫ��������; ���ǲ������ִ������, ��JavaScript�ִ�; �������ʹ�� CMemory ģ��. CMemory����������ģ���κβ���, ��Ϊ�����ڴ����, �����������ִ�.
@text API��ֻ�ܴ������10��������API, ��ʵ�ϳ���10������API����. ���е�ʱ��, ����ʹ�úͲ�������Ӧ��call, ����ĺ��лᵼ�³������. ���� GetWindowTextW ���������3������, �ͱ���ʹ�� call3 ��������. ʹ��call(n)������API������ʱ��, ����Ҳ����ȱʡ,
ȱʡ�Ĳ���һ�ɴ����� 0 , ����������API�����Ĳ������ܴ� 0 ��ʱ��, �ͻ����.
@text ָ�뱾���Ͼ�������, ��������ָ�����͵Ĳ���������������������.
@text call(n)�ķ���ֵ����һ������, ���APIû�з���ֵ, �������ֵ������.
@text ͨ��CMemory, ���Ե��úʹ����κ�API����, ��ΪCMemory����ֱ�Ӵ����ڴ�.
@text API������C��ͷ����, ����Ҫ���� dispose �ͷ� C++ ��Դ.*/
/*@range_begin*/
namespace v8{
	typedef int (__stdcall *API0)();
	typedef int (__stdcall *API1)(INT_PTR);
	typedef int (__stdcall *API2)(INT_PTR,INT_PTR);
	typedef int (__stdcall *API3)(INT_PTR,INT_PTR,INT_PTR);
	typedef int (__stdcall *API4)(INT_PTR,INT_PTR,INT_PTR,INT_PTR);
	typedef int (__stdcall *API5)(INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR);
	typedef int (__stdcall *API6)(INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR);
	typedef int (__stdcall *API7)(INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR);
	typedef int (__stdcall *API8)(INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR);
	typedef int (__stdcall *API9)(INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR);
	typedef int (__stdcall *API10)(INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR,INT_PTR);
	/*@function integer loadModule(dll)
	@text ����һ��DLL�ļ�, ����ɹ��������DLL�ľ��, ��������������API��create����.
	@param dll -- string
	@text DLL�ļ�������, ������ڵ�ǰ��ϵͳĿ¼, ��Ҫ����·��.
	@return integer �ɹ�����DLL�ľ��, ʧ�ܷ��� false .
	@code var user32 = API.loadModule("user32.dll");*/
	static Handle<Value> loadModule(const Arguments& args){
		HandleScope store;
		if(args.Length()<1) return Undefined();
		cs::String dll;
		GetString(args[0],dll);
		HMODULE h = ::LoadLibrary(dll);
		if(h==NULL) return Undefined();
		return store.Close(Uint32::New((INT_PTR)h));
	}
	/*@function bool freeModule(dll)
	@text �ͷ�һ��DLLģ��, �ɹ�ִ�����������, ģ�齫��ж��, ������ٿ���, ��Ӧ��API����Ҳ�����ٺ���.
	@param module -- integer
	@text DLLģ��ľ��.
	@return bool ָʾж���Ƿ�ɹ�.
	@code freeModule(user32);*/
	static Handle<Value> freeModule(const Arguments& args){
		if(args.Length()<1) return False();
		HMODULE h = (HMODULE)(INT_PTR)args[0]->Uint32Value();
		bool b = 0!=FreeLibrary(h);
		return Bool(b);
	}
#define GET_MUL_ARGS(i) \
	if(len>i){\
	if(args[i]->IsString()){\
	GetString(args[i],s##i);\
	p##i = (INT_PTR)s##i.Handle();\
	}else if(IsTemplate(args[i],TEMPLATE_ID_MEMORY)){\
	cs::Memory<char>* mem;\
	if(GetCHandle<cs::Memory<char> >(mem,args[i]->ToObject())) p##i = (INT_PTR)mem->Handle();\
			else p##i = 0;\
	}else{\
	p##i = args[i]->Uint32Value();\
	}\
	}
	class API{
	public:
		static Handle<Value> Create(const Arguments& args){
			HandleScope store;
			Handle<Function> func = FunctionTemplate::New(&call)->GetFunction();
			func->SetPrototype(args.This());
			return store.Close(func);
		}
		static inline Handle<Object> getApi(const Arguments& args){
			return args.Callee()->GetPrototype()->ToObject();
		}
		static Handle<Value> call(const Arguments& args){
			switch(args.Length())
			{
			case 0:
				return call0(args);
			case 1:
				return call1(args);
			case 2:
				return call2(args);
			case 3:
				return call3(args);
			case 4:
				return call4(args);
			case 5:
				return call5(args);
			case 6:
				return call6(args);
			case 7:
				return call7(args);
			case 8:
				return call8(args);
			case 9:
				return call9(args);
			case 10:
				return call0(args);
			}
			return Undefined();
		}
		/*@function integer call{n}(arg0,arg1,...,argn);
		@text ����API����.
		@param arg0 -- argn n����ȡֵ��0��9.
		@text һ�����������ִ�����CMemory����, ��������3��. 
		@text API������4�ֽ�������ָ���ʱ��, ����������, ����1�ֽ�2�ֽں�8�ֽڵ���������ֱ�Ӵ�, ����ʹ��CMemoryģ��.
		@text API������˫�ֽ��ִ���ֻ���ڴ����ʱ��(LPCWSTR����const wchar_t*)ֱ�Ӵ�JavaScript�ִ�, ������������������ȡ���ַ��������, ����ʹ��JavaScript�ִ�, ������CMemoryģ��
		@text API�����������������ִ�ֱ�Ӵ��ݵ����, ȫ����CMemory��ģ��, ����˵����ʾ������.
		@return ����һ������, ��������ĺ����ɾ����API��������.*/
		static Handle<Value> call0(const Arguments& args){
			HandleScope store;
			API0 f = (API0)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			int len = args.Length();

			INT_PTR r = f();
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call1(const Arguments& args){
			HandleScope store;
			API1 f = (API1)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0;
			cs::String s0((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);

			INT_PTR r = f(p0);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call2(const Arguments& args){
			HandleScope store;
			API2 f = (API2)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1;
			cs::String s0((uint)0),s1((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);

			INT_PTR r = f(p0,p1);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call3(const Arguments& args){
			HandleScope store;
			API3 f = (API3)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1,p2;
			cs::String s0((uint)0),s1((uint)0),s2((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);
			GET_MUL_ARGS(2);

			INT_PTR r = f(p0,p1,p2);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call4(const Arguments& args){
			HandleScope store;
			API4 f = (API4)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1,p2,p3;
			cs::String s0((uint)0),s1((uint)0),s2((uint)0),s3((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);
			GET_MUL_ARGS(2);
			GET_MUL_ARGS(3);

			INT_PTR r = f(p0,p1,p2,p3);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call5(const Arguments& args){
			HandleScope store;
			API5 f = (API5)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1,p2,p3,p4;
			cs::String s0((uint)0),s1((uint)0),s2((uint)0),s3((uint)0),s4((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);
			GET_MUL_ARGS(2);
			GET_MUL_ARGS(3);
			GET_MUL_ARGS(4);

			INT_PTR r = f(p0,p1,p2,p3,p4);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call6(const Arguments& args){
			HandleScope store;
			API6 f = (API6)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1,p2,p3,p4,p5;
			cs::String s0((uint)0),s1((uint)0),s2((uint)0),s3((uint)0),s4((uint)0),s5((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);
			GET_MUL_ARGS(2);
			GET_MUL_ARGS(3);
			GET_MUL_ARGS(4);
			GET_MUL_ARGS(5);

			INT_PTR r = f(p0,p1,p2,p3,p4,p5);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call7(const Arguments& args){
			HandleScope store;
			API7 f = (API7)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1,p2,p3,p4,p5,p6;
			cs::String s0((uint)0),s1((uint)0),s2((uint)0),s3((uint)0),s4((uint)0),s5((uint)0),s6((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);
			GET_MUL_ARGS(2);
			GET_MUL_ARGS(3);
			GET_MUL_ARGS(4);
			GET_MUL_ARGS(5);
			GET_MUL_ARGS(6);

			INT_PTR r = f(p0,p1,p2,p3,p4,p5,p6);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call8(const Arguments& args){
			HandleScope store;
			API8 f = (API8)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1,p2,p3,p4,p5,p6,p7;
			cs::String s0((uint)0),s1((uint)0),s2((uint)0),s3((uint)0),s4((uint)0),s5((uint)0),s6((uint)0),s7((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);
			GET_MUL_ARGS(2);
			GET_MUL_ARGS(3);
			GET_MUL_ARGS(4);
			GET_MUL_ARGS(5);
			GET_MUL_ARGS(6);
			GET_MUL_ARGS(7);

			INT_PTR r = f(p0,p1,p2,p3,p4,p5,p6,p7);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call9(const Arguments& args){
			HandleScope store;
			API9 f = (API9)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1,p2,p3,p4,p5,p6,p7,p8;
			cs::String s0((uint)0),s1((uint)0),s2((uint)0),s3((uint)0),s4((uint)0),s5((uint)0),s6((uint)0),s7((uint)0),s8((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);
			GET_MUL_ARGS(2);
			GET_MUL_ARGS(3);
			GET_MUL_ARGS(4);
			GET_MUL_ARGS(5);
			GET_MUL_ARGS(6);
			GET_MUL_ARGS(7);
			GET_MUL_ARGS(8);

			INT_PTR r = f(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			return store.Close(Uint32::New(r));
		}
		static Handle<Value> call10(const Arguments& args){
			HandleScope store;
			API10 f = (API10)getInternelPointer(getApi(args),0);
			if(f==0) return Undefined();
			INT_PTR p0,p1,p2,p3,p4,p5,p6,p7,p8,p9;
			cs::String s0((uint)0),s1((uint)0),s2((uint)0),s3((uint)0),s4((uint)0),s5((uint)0),s6((uint)0),s7((uint)0),s8((uint)0),s9((uint)0);
			int len = args.Length();

			GET_MUL_ARGS(0);
			GET_MUL_ARGS(1);
			GET_MUL_ARGS(2);
			GET_MUL_ARGS(3);
			GET_MUL_ARGS(4);
			GET_MUL_ARGS(5);
			GET_MUL_ARGS(6);
			GET_MUL_ARGS(7);
			GET_MUL_ARGS(8);
			GET_MUL_ARGS(9);

			INT_PTR r = f(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			return store.Close(Uint32::New(r));
		}
		/*@function bool create(modle,funcName)
		@text ����һ��API��������, ������������ظ�����, ���ɲ�ͬ�� API ����.
		@param modle -- interger
		@text API����DLL��ģ����, ���������� API.loadModule("dll") �������ص�.
		@param funcName -- string 
		@text API����������, ע��, ��ЩAPI������Ҫ�����ƺ������"W"��"A"��׺, ����"MessageBoxW"��"MessageBoxA", �������ǲ�ͬ����������, jsuseһ��ֻʹ��W�汾�ĺ���, ���Ҫʹ��A�汾�ĺ���, �ִ���������ֱ�Ӵ���, ������ʹ�� CMemory ����������������.
		@return bool ��ʾ�����Ƿ�ɹ�.
		*/
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			if(args.Length()<2) return False();

			HMODULE h = (HMODULE)(INT_PTR)args[0]->Uint32Value();
			String::AsciiValue name(args[1]);

			FARPROC f = GetProcAddress(h,*name);
			if(f==0) return False();
			setInternelPointer(args.This()->GetPrototype()->ToObject(),0,f);
			return True();
		}
		/*@function void reset()
		@text �����������Ϊ��, ��ԭ״̬Ϊδ����create֮ǰ.
		return undefined �˺���û�з���ֵ.*/
		static Handle<Value> reset(const Arguments& args){
			setInternelPointer(args.This()->GetPrototype()->ToObject(),0,0);
			return Undefined();
		}
		/*@function bool isEmpty()
		@text ���API�����Ƿ�Ϊ��, �ɹ�����create֮��, ����ֵΪ false, ִ�� reset֮�� ����ֵΪtrue.
		return bool ָʾ�����Ƿ�Ϊ��.*/
		static Handle<Value> isEmpty(const Arguments& args){
			bool b = 0==getInternelPointer(args.This()->GetPrototype()->ToObject(),0);
			return Bool(b);
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(create);
			SET_CLA_FUNC(reset);
			SET_CLA_FUNC(isEmpty);
			/*SET_CLA_FUNC(call0);
			SET_CLA_FUNC(call1);
			SET_CLA_FUNC(call2);
			SET_CLA_FUNC(call3);
			SET_CLA_FUNC(call4);
			SET_CLA_FUNC(call5);
			SET_CLA_FUNC(call6);
			SET_CLA_FUNC(call7);
			SET_CLA_FUNC(call8);
			SET_CLA_FUNC(call8);
			SET_CLA_FUNC(call9);
			SET_CLA_FUNC(call10);*/
		}
		static void Load(Handle<Object>& glb,LPCWSTR name,int tid){
			HandleScope store;
			Handle<FunctionTemplate> ft = FunctionTemplate::New(&API::Create);
			Handle<ObjectTemplate> ot_func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> ot_inst = ft->InstanceTemplate();  
			cs::String tss;
			tss.Format(L"new Function(\"{return '%s';}\")",name);
			Handle<Value> ts = LoadJsCode(tss);
			ot_func->Set(String::New("toString"),ts,ReadOnly);
			ot_inst->SetInternalFieldCount(1);
			GetEnv()->SetTemplate(ft,tid);
			init(ft);
			Handle<Function> Api = ft->GetFunction();
			glb->Set(String::New((uint16_t*)name),Api);
			SET_OBJ_FUNC(Api,loadModule,loadModule);
			SET_OBJ_FUNC(Api,freeModule,freeModule);
		}
	};
	/*@range_end*/
	void LoadAPICall(Handle<Object>& glb){
		API::Load(glb,L"Api",TEMPLATE_ID_API);
	}
}