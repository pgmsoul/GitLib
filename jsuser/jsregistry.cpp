#include "stdafx.h"
#include "v8base.h"
//����Windowsע���
namespace v8{
	//*{
	//	"type":"group",
	//	"name":"platform",
	//	"text":"ƽ̨��ء�",
	//	"member":[//*
	//*{
	//	"type":"class",
	//	"name":"CRegistry",
	//	"text":"���� Windows ע������.",
	//	"member":[//*
	class JSRegistry : public JsHandleObject<cs::Registry,HKEY,JSRegistry,TEMPLATE_ID_REGISTRY>{
	public:
		//*{
		//	"type":"function",
		//	"name":"create(name,[key],[create],[noMulti])",
		//	"text":"�򿪻򴴽�һ��ע�����, ������б�ܻ�б�ָܷ��༶����.",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"ע����������, �����ƿ�����һ����б�ܻ�б�ָܷ��Ķ༶��������: HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[key]",
		//			"text":"ע�����ĸ���, ����˲���ȱʡ��Ϊ 0 �Ҽ���û��ָ����Ŀ¼��ʱ�򣬻�Ĭ��Ϊ HKEY_CURRENT_USER, ����˲���������һ����Ч�ļ�ֵ, ��������ܰ�����Ŀ¼��������ᱻ��Ϊ��������ǿ��ܴ�ʧ�ܣ�.ʹ�� getHandle() ��������һ�� integer ����ȡ��������"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[create]",
		//			"text":"�����ע��������, �Ƿ񴴽���."
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noMulti]",
		//			"text":"����˲�������Ϊ true, �������ʹ����б�ܻ�б��Ҳ�����ɶ༶����, ����ֱ�ӵ���������Windowsһ��ʹ��б�����ָ�һ���༶ע����������ʵ����б�ܺͷ�б���ǿ�����Ϊ������һ���ֵģ������������ǿ�ƹ涨��Ҫ�ָ���������ǿ���һ������ļ�������ʵ���������á�"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ����� 1 (��ʾ�˼�������)�� 2 (��ʾ�˼��Ѿ�����), ��ʧ�ܷ��� undefined ."
		//	},
		//	"remark":[
		//		""
		//	]
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Registry* reg;
				Local<Object> self = args.This();
				if(!GetCHandle(reg,self)) break;
				cs::String name;
				GetString(args[0],name);
				HKEY key;
				if(args.Length()>1) key = (HKEY)(UINT_PTR)args[1]->Uint32Value();
				else key = 0;
				bool bCreate;
				if(args.Length()>2){
					bCreate = args[2]->BooleanValue();
				}else bCreate = false;
				bool noMulti;
				if(args.Length()>3){
					noMulti = args[3]->BooleanValue();
				}else noMulti = false;
				int r;
				if(noMulti){
					r = reg->Create(name,key,bCreate);
				}else{
					r = reg->MultiCreate(name,key,bCreate);
				}
				if(r==0) break;
				return store.Close(Uint32::New(r));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"toSubKey(name,[create],[noMulti])",
		//	"text":"��һ��ע�������ӽ������������ create ֻ����һ������������Ҳ��ȫ��ͬ��Ψһ��ͬ���ǣ����������򿪵ļ���Ϊ create �ĵڶ���������",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"�ӽ�������, �����ƿ�����һ����б�ܻ�б�ָܷ��Ķ༶��������: SOFTWARE\\Classes��"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[create]",
		//			"text":"�����ע��������, �Ƿ񴴽���."
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noMulti]",
		//			"text":"����˲�������Ϊ true, �������ʹ����б�ܻ�б��Ҳ�����ɶ༶����, ����ֱ�ӵ���������Windowsһ��ʹ��б�����ָ�һ���༶ע����������ʵ����б�ܺͷ�б���ǿ�����Ϊ������һ���ֵģ������������ǿ�ƹ涨��Ҫ�ָ���������ǿ���һ������ļ�������ʵ���������á�"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ����� 1 (��ʾ�˼�������)�� 2 (��ʾ�˼��Ѿ�����), ��ʧ�ܷ��� undefined ."
		//	},
		//	"remark":[
		//		"�����ǰ CRegistry ����û�д��κ�ע���������ô˺������ HKEY_CURRENT_USER �µ��ӽ����൱�� create ���� key = 0 �������"
		//	]
		//}//*
		static Handle<Value> toSubKey(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Registry* reg;
				Local<Object> self = args.This();
				if(!GetCHandle(reg,self)) break;
				cs::String name;
				GetString(args[0],name);
				bool bCreate;
				if(args.Length()>1){
					bCreate = args[1]->BooleanValue();
				}else bCreate = false;
				bool noMulti;
				if(args.Length()>2){
					noMulti = args[2]->BooleanValue();
				}else noMulti = false;
				int r;
				if(noMulti){
					r = reg->ToSubKey(name,bCreate);
				}else{
					r = reg->ToMultiSubKey(name,bCreate);
				}
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"close()",
		//	"text":"�رյ�ǰ����",
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined ."
		//	}
		//}//*
		static Handle<Value> close(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Registry* reg;
				Local<Object> self = args.This();
				if(!GetCHandle(reg,self)) break;
				if(reg->Close()) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getStrValue(name)",
		//	"text":"��ȡ��ע��������ֵ��ע�����һ���򵥵����ݿ⣬һ�����������Ϊһ�������Ǳ��������ӱ�����һ�����νṹ��ÿ������������ӱ��ж�Ӧ�����ݣ�����������ڻ�ȡ����Ϊ�ִ������ֵ��",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"Ҫ��ȡ�����ݵ����ƣ�ע����������һ���ֵ�ṹ��"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ����������ִ�, ��������Ƶ����ݲ����ڻ����Ͳ����ִ�����, ����ʧ��. ʧ�ܷ��� undefined."
		//	}
		//}//*
		static Handle<Value> getStrValue(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Registry* reg;
				Local<Object> self = args.This();
				if(!GetCHandle(reg,self)) break;
				cs::String name;
				GetString(args[0],name);
				cs::String val;
				if(!reg->GetStrValue(name,val)) break;
				return store.Close(String::New((uint16_t*)val.Handle()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getIntValue(name)",
		//	"text":"��ȡ��ע��������ֵ��ע�����һ���򵥵����ݿ⣬һ�����������Ϊһ�������Ǳ��������ӱ�����һ�����νṹ��ÿ������������ӱ��ж�Ӧ�����ݣ�����������ڻ�ȡ����Ϊ�ִ������ֵ��",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"Ҫ��ȡ�����ݵ����ƣ�ע����������һ���ֵ�ṹ��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�������������ֵ, ��������Ƶ����ݲ����ڻ����Ͳ�������, ����ʧ��. ʧ�ܷ��� undefined."
		//	}
		//}//*
		static Handle<Value> getIntValue(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Registry* reg;
				Local<Object> self = args.This();
				if(!GetCHandle(reg,self)) break;
				cs::String name;
				GetString(args[0],name);
				DWORD val;
				if(!reg->GetIntValue(name,val)) break;
				return store.Close(Uint32::New(val));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setStrValue(name,value)",
		//	"text":"����һ��ע�����������ִ�����, ע������µ������ж�������, ���õ����ִ����ͺ��������͡�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"Ҫ���õ����ݵ����ƣ�ע����������һ���ֵ�ṹ��"
		//		},
		//		{
		//			"type":"string",
		//			"name":"value",
		//			"text":"Ҫ���õ��ִ�����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true, ʧ�ܷ��� undefined."
		//	}
		//}//*
		static Handle<Value> setStrValue(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::Registry* reg;
				Local<Object> self = args.This();
				if(!GetCHandle(reg,self)) break;
				cs::String name;
				GetString(args[0],name);
				cs::String val;
				GetString(args[1],val);
				if(!reg->SetStrValue(name,val)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setIntValue(name,value)",
		//	"text":"����һ��ע����������32λ��������, ע������µ������ж�������, ���õ����ִ����ͺ��������͡�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"Ҫ���õ����ݵ����ƣ�ע����������һ���ֵ�ṹ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"value",
		//			"text":"Ҫ���õ��������ݣ�ע�������32λ���֣�������Χ��λ�ᱻ������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true, ʧ�ܷ��� undefined."
		//	}
		//}//*
		static Handle<Value> setIntValue(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::Registry* reg;
				Local<Object> self = args.This();
				if(!GetCHandle(reg,self)) break;
				cs::String name;
				GetString(args[0],name);
				uint val = args[1]->Uint32Value();
				if(!reg->SetIntValue(name,val)) break;
				return True();
			}
			return Undefined();
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  

			SET_CLA_FUNC(create);
			SET_CLA_FUNC(close);
			SET_CLA_FUNC(toSubKey);
			SET_CLA_FUNC(getStrValue);
			SET_CLA_FUNC(getIntValue);
			SET_CLA_FUNC(setStrValue);
			SET_CLA_FUNC(setIntValue);

			//SET_CLA_CONST2(CLASSES_ROOT,(UINT_PTR)HKEY_CLASSES_ROOT);
			//SET_CLA_CONST2(CURRENT_USER,(UINT_PTR)HKEY_CURRENT_USER);
			//SET_CLA_CONST2(USERS,(UINT_PTR)HKEY_USERS);
			//SET_CLA_CONST2(CURRENT_CONFIG,(UINT_PTR)HKEY_CURRENT_CONFIG);
			//SET_CLA_CONST2(LOCAL_MACHINE,(UINT_PTR)HKEY_LOCAL_MACHINE);
		}
	};
	//*]}//*

	//*,{
	//	"type":"function",
	//	"name":"getLastError()",
	//	"text":"��ȡϵͳ API �����Ĵ����룬ÿ������һ�� Windows API �����󣬶������������������ȡ����Ĵ�����Ϣ��һ�㺯�����û�д��󣬷���ֵ�� 0��",
	//	"remark":[
	//		"ע�⣺�������������ÿһ API ����֮���������ã���Ϊÿһ�� API ���ö�����������ֵ��������� API ����֮��ʹ���������������������������ڲ��Ƿ������ĳ�� API �����ǲ�ȷ���ģ��������Ƿ�����Ҳ�ǲ�ȷ���ġ�"
	//	],
	//	"param":[
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"����һ�����֣�һ�� 0 ��ʾû�д�������"
	//	}
	//}//*
	Handle<Value> getLastError(const Arguments& args){
		HandleScope stack;
		return stack.Close(Uint32::New(GetLastError()));
	}
	//*,{
	//	"type":"function",
	//	"name":"getSysMessage([code])",
	//	"text":"��ȡ�������ϵͳ�������⺯������������ȡ API ���� GetLastError �� WSAGetLastError ���صĴ�������ı�������",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"[code]",
	//			"text":"Ҫȡ�������Ĵ����룬�������� getLastError �� wsaGetLastError ���ص�ֵ��������ֵȱʡ���������ڲ����� getLastError�����ҷ��������ִ���"
	//		}
	//	],
	//	"return":{
	//		"type":"string",
	//		"text":"�ɹ�����һ���ִ�, ʧ�ܷ��� undefined. ���Ƿ��������������������ǲ���ʧ�ܵġ�"
	//	}
	//}//*
	Handle<Value> getSysMessage(const Arguments& args){
		HandleScope stack;
		while(true){
			if(args.Length()<1) break;
			cs::String msg;
			DWORD code = GET_ARGS_INT(0,0);
			cs::GetSystemMessage(msg,code);
			return stack.Close(NEW_WSTR(msg.Handle()));
		}
		return Undefined();
	}
	void LoadRegistry(Handle<Object>& glb){
		SET_OBJ_FUNC_RO(glb,getLastError,getLastError);
		SET_OBJ_FUNC_RO(glb,getSysMessage,getSysMessage);
		JSRegistry::Load(glb,L"CRegistry");
	}
	//*]}//*
}