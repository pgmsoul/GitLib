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
	//����һ��Point����
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
		//�������Ͷ�ݵ���Ϣ����ص����Լ�ָ���ĺ������¼�ѭ������Ҫ����Ĳ������������������һ����ͬ���̵߳��ã�������ܻ�������
		bool PostCallback(QECALL proc,void* param);
	};
	class JsEnv : public cs::_class{
	public:
		Persistent<Context>* cont;
		Queue	queue;
		//Js����ĺ�������
		cs::ObjectList<Persistent<Function> > FuncList;
		inline void SetFunc(Handle<Function>& func,int id){
			FuncList[id] = Persistent<Function>::New(func);
		}
		inline Persistent<Function>& GetFunc(int id){
			return FuncList[id];
		}
		//C++����ĺ���ģ��
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
	//��ȡȫ�ֶ���.
	inline Handle<Object> GetGlobal(){
		return (*_env->cont)->Global();
	}

	int Alert(Handle<Value> msg,MSG_TYPE mt = MT_INFO);
	int Alert(LPCWSTR msg,MSG_TYPE mt);
	//����һ��Js����, ���ҷ��ؽ��.
	Handle<Value> LoadJsCode(LPCWSTR js,LPCWSTR name = 0);
	//����Js�ļ�, ���ҷ��ؽ��.
	Handle<Value> LoadJsFile(LPCWSTR file,LPCWSTR code = 0);
	//������UTF-8���뱣���JS�����ı���Դ.
	Handle<Value> LoadJsRes(cs::ResID jr,LPCWSTR name);
	//��ȡһ��js�ļ�,cp���ļ��ı�����.
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
	//ֻҪ tid �� val �̳����ϵ��κ�һ����������������� true��
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
	//����js����, main�������ص�, ����������дC++����. js���ᱻִ��, ���Ǵ���main����.
	void EnterJs();
	//ֱ������js����, cmd�Ǵ��� js ���� main �����Ĳ���.
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
