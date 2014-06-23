// jslib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "v8base.h"
/*@class API
@text API类可以让你呼叫系统的 API 函数. Windows系统的API函数绝大多数有2个版本, "A"和"W", 比如获取窗口文本的函数 GetWindowText, 本质上是GetWindowTextA和GetWindowTextW两个函数, C++程序依靠全局设置, 即是Unicode还是ANSI来确定使用那个版本的函数.
所以C++程序中的API函数一般只需要写函数名, 不需要带上A或W后缀. 但是实际上并没有GetWindowText这个函数, 所以在使用API类的时候, 必须指定使用那个版本的函数. 当然, 基本上不要使用A版本的函数, 因为A版本函数的字串参数不能直接传JavaScript字串, 需要用
CMemory 对象来模拟. <b>不是所有的函数都有A和W两个版本</b>, 极个别的函数只有W版本或者只有A版本, 具体情况需要查阅MSDN文档.
@text 使用API类呼叫系统API的时候, 参数只有3种: 整数, 字串, CMemory. 凡是API函数需要指针和整数数字的参数, 全部传整数; 凡是参数是字串的情况, 传JavaScript字串; 其它情况使用 CMemory 模拟. CMemory对象本质上能模拟任何参数, 因为它是内存对象, 包括整数和字串.
@text API类只能处理最多10个参数的API, 事实上超过10参数的API极少. 呼叫的时候, 必须使用和参数个对应的call, 错误的呼叫会导致程序崩溃. 比如 GetWindowTextW 这个函数有3个参数, 就必须使用 call3 来呼叫它. 使用call(n)来呼叫API函数的时候, 参数也可以缺省,
缺省的参数一律传整数 0 , 所以如果这个API函数的参数不能传 0 的时候, 就会出错.
@text 指针本质上就是整数, 所以所以指针类型的参数都可以用整数来传递.
@text call(n)的返回值总是一个整数, 如果API没有返回值, 这个返回值无意义.
@text 通过CMemory, 可以调用和处理任何API函数, 因为CMemory对象直接处理内存.
@text API对象不是C开头的类, 不需要调用 dispose 释放 C++ 资源.*/
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
	@text 加载一个DLL文件, 如果成功返回这个DLL的句柄, 这个句柄可以用于API的create函数.
	@param dll -- string
	@text DLL文件的名称, 如果不在当前和系统目录, 需要完整路径.
	@return integer 成功返回DLL的句柄, 失败返回 false .
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
	@text 释放一个DLL模块, 成功执行这个函数后, 模块将被卸载, 句柄不再可用, 对应的API函数也不能再呼叫.
	@param module -- integer
	@text DLL模块的句柄.
	@return bool 指示卸载是否成功.
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
		@text 呼叫API函数.
		@param arg0 -- argn n可以取值从0到9.
		@text 一个整数或者字串或者CMemory对象, 必须是这3种. 
		@text API参数是4字节整数和指针的时候, 传整数参数, 但是1字节2字节和8字节的整数不能直接传, 必须使用CMemory模拟.
		@text API参数是双字节字串且只用于传入的时候(LPCWSTR或者const wchar_t*)直接传JavaScript字串, 但是如果这个参数用于取回字符串的情况, 不能使用JavaScript字串, 必须用CMemory模拟
		@text API参数不能用整数和字串直接传递的情况, 全部用CMemory来模拟, 具体说明见示例代码.
		@return 返回一个整数, 这个整数的含义由具体的API函数决定.*/
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
		@text 生成一个API函数对象, 这个函数可以重复调用, 生成不同的 API 对象.
		@param modle -- interger
		@text API所在DLL的模块句柄, 这个句柄是由 API.loadModule("dll") 函数返回的.
		@param funcName -- string 
		@text API函数的名称, 注意, 有些API函数需要在名称后面添加"W"或"A"后缀, 比如"MessageBoxW"和"MessageBoxA", 本质上是不同的两个函数, jsuse一般只使用W版本的函数, 如果要使用A版本的函数, 字串参数不能直接传递, 而必须使用 CMemory 对象做参数来传递.
		@return bool 表示加载是否成功.
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
		@text 重置这个对象为空, 还原状态为未调用create之前.
		return undefined 此函数没有返回值.*/
		static Handle<Value> reset(const Arguments& args){
			setInternelPointer(args.This()->GetPrototype()->ToObject(),0,0);
			return Undefined();
		}
		/*@function bool isEmpty()
		@text 这个API对象是否为空, 成功调用create之后, 返回值为 false, 执行 reset之后 返回值为true.
		return bool 指示对象是否为空.*/
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