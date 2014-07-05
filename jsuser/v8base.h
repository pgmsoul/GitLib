#pragma once

#include "v8macro.h"
#include "v8objid.h"
#include "v8obj.h"

namespace v8{
	extern LPCWSTR NULL_STR;
	void LoadConsole(Handle<Object>& glb);
	void LoadAPICall(Handle<Object>& glb);
	void RectJsFromC(Handle<Value> vObj,cs::Rect* r);
	bool RectJsToC(Handle<Value> vObj,cs::Rect* r);
	void PointJsFromC(Handle<Value> vObj,cs::Twin* r);
	bool PointJsToC(Handle<Value> vObj,cs::Twin* r);
	//返回一个Point对象
	Handle<Value> NewPoint(int x,int y);
	Handle<Value> NewSize(int x,int y);
	Handle<Value> NewColor(int red,int green,int blue,int alpha);
	Handle<Value> NewRect(int left,int top,int right,int bottom);
	typedef void (*QECALL)(void*);
#define WM_QUEUE 0x735
	class Queue : public cs::_class{
	protected:
		HANDLE _handle;
		DWORD  _thid;
		cs::Event	_wait;
	public:
		Queue(){
			_wait.Create(1,0);
			_handle = 0;
			_thid = 0;
		}
		~Queue(){
			Close();
		}
		void Close(){
			if(_handle){
				PostQueuedCompletionStatus(_handle,(DWORD)(UINT_PTR)0,(ULONG_PTR)0,(LPOVERLAPPED)0);
			}else{
				PostQuitMessage(0);
			}
		}
		/*void Init(){
			if(_handle) return;
			_handle = INVALID_HANDLE_VALUE;
		}*/
		void Start(bool qMsg);
		//这个函数投递的消息，会回调它自己指定的函数，事件循环不需要额外的操作。这个函数必须在一个不同的线程调用，否则可能会死锁。
		bool PostCallback(QECALL proc,void* param);
	};
	class JsEnv : public cs::_class{
	public:
		Persistent<Context>* cont;
		Queue	queue;
		//Js定义的函数对象
		cs::ObjectList<Persistent<Function> > FuncList;
		inline void SetFunc(Handle<Function>& func,int id){
			FuncList[id] = Persistent<Function>::New(func);
		}
		inline Persistent<Function>& GetFunc(int id){
			return FuncList[id];
		}
		//C++定义的函数模版
		cs::ObjectList<Persistent<FunctionTemplate> > TemplateList;
		inline void SetTemplate(Handle<FunctionTemplate>& tl,int id){
			TemplateList[id] = Persistent<FunctionTemplate>::New(tl);
		}
		inline Persistent<FunctionTemplate>& GetTemplate(int id){
			return TemplateList[id];
		}
		bool IsMainThread();
		JsEnv();
		~JsEnv();
	};
	extern wchar_t bin_path[MAX_PATH];
	
	extern JsEnv* _env;
	inline JsEnv* GetEnv(){return _env;}
	int ReportError(TryCatch& err);
	enum MSG_TYPE{
		MT_ERROR = MB_ICONERROR,
		MT_WARN = MB_ICONWARNING,
		MT_INFO = MB_ICONINFORMATION
	};
	void SetAppInfo(LPCWSTR name,LPCWSTR info);
	void GetAppInfo(LPCWSTR name,cs::String& info);
	//获取全局对象.
	inline Handle<Object> GetGlobal(){
		return (*_env->cont)->Global();
	}

	int Alert(Handle<Value> msg,MSG_TYPE mt = MT_INFO);
	int Alert(LPCWSTR msg,MSG_TYPE mt);
	//运行一段Js代码, 并且返回结果.
	Handle<Value> LoadJsCode(LPCWSTR js,LPCWSTR name = 0);
	//加载Js文件, 并且返回结果.
	Handle<Value> LoadJsFile(LPCWSTR file,LPCWSTR code = 0);
	//运行以UTF-8编码保存的JS代码文本资源.
	Handle<Value> LoadJsRes(cs::ResID jr,LPCWSTR name);
	//读取一个js文件,cp是文件文本编码.
	bool ReadJsFile(cs::String& js,LPCWSTR file,DWORD cp);
	//bool IsInheritTemplate(Handle<Value> val,uint tid);
	inline bool ReadJsFile(cs::String& js,LPCWSTR file,LPCWSTR cp);
	inline const wchar_t* ToCString(const String::Value& value) {
		return *value ? (wchar_t*)*value : L"<string conversion failed>";
	}
	Handle<Value> CallFunc(Handle<Object>& self,Handle<Function>& func,int argc,Handle<Value>* argv);
	inline void GetUtf8(Handle<Value> val,cs::Memory<char>& utf8){
		v8::String::Utf8Value sv(val);
		LPCSTR str = (LPCSTR)*sv;
		utf8.CopyFrom(str,strlen(str)+1);
	}
	inline void GetAscii(Handle<Value> val,cs::Memory<char>& ascii){
		v8::String::AsciiValue sv(val);
		LPCSTR str = (LPCSTR)*sv;
		ascii.CopyFrom(str,strlen(str)+1);
	}
	inline void GetString(Handle<Value> val,cs::String& str){
		v8::String::Value sv(val);
		str = (LPCWSTR)*sv;
	}
	//只要 tid 是 val 继承链上的任何一环，这个函数都返回 true。
	inline bool IsTemplate(Handle<Value>& val,int tid){
		if(!val->IsObject()) return false;
		Persistent<FunctionTemplate>& ft = GetEnv()->GetTemplate(tid);
		if(ft.IsEmpty()) return false;
		return ft->HasInstance(val);
	}
	inline bool IsFunc(Handle<Value>& val,int fid){
		Persistent<Function>& f = GetEnv()->GetFunc(fid);
		if(f.IsEmpty()) return false;
		return f->GetName()->ToString()==val->ToObject()->GetConstructorName();
		//return GetEnv()->GetFunc(fid)==val->ToObject()->GetConstructor();
	}
	//启动js环境, main函数被回调, 可以在里面写C++代码. js不会被执行, 而是传给main函数.
	void EnterJs();
	//直接运行js代码, cmd是传给 js 里面 main 函数的参数.
	void ExitJs();
	Local<Object> TLNew(uint id,bool bind = false);
	Local<Object> TLNew(uint id,void* cobj);
	void setPersistentFunc(Persistent<Function>& pf,Handle<Value>& v);
	inline void setInternelPointer(Handle<Object>& self,int index,void* p){
		self->SetInternalField(index,External::New(p));
	}
	inline void* getInternelPointer(Handle<Object>& self,int index){
		Local<External> ext = Local<External>::Cast(self->GetInternalField(index));
		return ext->Value();
	}
	inline bool StringIs(Handle<Value> val,LPCWSTR str){
		String::Value sv(val);
		return cs::WcsEqual((LPCWSTR)*sv,str);
	}
	bool GetMBString(Handle<Value>& val,cs::Memory<char>& buf,DWORD charset);
	bool GetMBString(Handle<Value>& val,cs::Memory<char>& buf);
	template<typename T> inline Handle<T> GetObjProperty(Handle<Object>& obj,LPCWSTR name){
		return Handle<T>::Cast(obj->Get(String::New((uint16_t*)name)));
	}
	typedef void (*LOADJSCOMPONENT)(Handle<Object>&);
	void loadComponent(cs::StringList& sl,LPCWSTR modeList,LPCWSTR component,Handle<Object>& glb,LPCWSTR checkName,LOADJSCOMPONENT loadc);
	Handle<Value> require(const Arguments& args);
	inline bool IsPointer(void* p){
		return p==0||(0xffff0000&(UINT_PTR)p);
	}
}
