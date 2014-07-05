#include "stdafx.h"
#include "v8base.h"
//操作Windows注册表
namespace v8{
	//*{
	//	"type":"group",
	//	"name":"platform",
	//	"text":"平台相关。",
	//	"member":[//*
	//*{
	//	"type":"class",
	//	"name":"CRegistry",
	//	"text":"操作 Windows 注册表的类.",
	//	"member":[//*
	class JSRegistry : public JsHandleObject<cs::Registry,HKEY,JSRegistry,TEMPLATE_ID_REGISTRY>{
	public:
		//*{
		//	"type":"function",
		//	"name":"create(name,[key],[create],[noMulti])",
		//	"text":"打开或创建一个注册表项, 可以用斜杠或反斜杠分隔多级键名.",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"注册表项的名称, 此名称可以是一个用斜杠或反斜杠分隔的多级键名，如: HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[key]",
		//			"text":"注册表项的父键, 如果此参数缺省或为 0 且键名没有指定根目录的时候，会默认为 HKEY_CURRENT_USER, 如果此参数设置了一个有效的键值, 则键名不能包含根目录名（否则会被认为是子项，但是可能打开失败）.使用 getHandle() 函数返回一个 integer 来获取这个句柄。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[create]",
		//			"text":"如果此注册表项不存在, 是否创建它."
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noMulti]",
		//			"text":"如果此参数设置为 true, 则键名即使包含斜杠或反斜杠也不看成多级键名, 而是直接当作键名。Windows一般使用斜杠来分隔一个多级注册表键，但是实际上斜杠和反斜杠是可以作为键名的一部分的，这个参数用来强制规定不要分割键名，而是看成一个整体的键名，事实很少这样用。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回 1 (表示此键被创建)或 2 (表示此键已经存在), 打开失败返回 undefined ."
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
		//	"text":"打开一个注册表键的子健，这个函数和 create 只差了一个参数，功能也完全相同，唯一不同的是，它会把自身打开的键作为 create 的第二个参数。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"子健的名称, 此名称可以是一个用斜杠或反斜杠分隔的多级键名，如: SOFTWARE\\Classes。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[create]",
		//			"text":"如果此注册表项不存在, 是否创建它."
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noMulti]",
		//			"text":"如果此参数设置为 true, 则键名即使包含斜杠或反斜杠也不看成多级键名, 而是直接当作键名。Windows一般使用斜杠来分隔一个多级注册表键，但是实际上斜杠和反斜杠是可以作为键名的一部分的，这个参数用来强制规定不要分割键名，而是看成一个整体的键名，事实很少这样用。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回 1 (表示此键被创建)或 2 (表示此键已经存在), 打开失败返回 undefined ."
		//	},
		//	"remark":[
		//		"如果当前 CRegistry 对象没有打开任何注册表键，调用此函数会打开 HKEY_CURRENT_USER 下的子健，相当于 create 函数 key = 0 的情况。"
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
		//	"text":"关闭当前键。",
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined ."
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
		//	"text":"获取打开注册表键的项值，注册表是一个简单的数据库，一个键可以理解为一个表，但是表下面有子表这样一个树形结构，每个键下面除了子表还有对应的数据，这个函数用于获取数据为字串的项的值。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"要获取的数据的名称，注册表的数据是一个字典结构。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功返回数据字串, 如果此名称的数据不存在或类型不是字串类型, 函数失败. 失败返回 undefined."
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
		//	"text":"获取打开注册表键的项值，注册表是一个简单的数据库，一个键可以理解为一个表，但是表下面有子表这样一个树形结构，每个键下面除了子表还有对应的数据，这个函数用于获取数据为字串的项的值。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"要获取的数据的名称，注册表的数据是一个字典结构。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功返回数据数字值, 如果此名称的数据不存在或类型不是数字, 函数失败. 失败返回 undefined."
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
		//	"text":"设置一个注册表项下面的字串数据, 注册表项下的数据有多种类型, 常用的是字串类型和数字类型。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"要设置的数据的名称，注册表的数据是一个字典结构。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"value",
		//			"text":"要设置的字串数据"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true, 失败返回 undefined."
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
		//	"text":"设置一个注册表项下面的32位整数数据, 注册表项下的数据有多种类型, 常用的是字串类型和数字类型。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"要设置的数据的名称，注册表的数据是一个字典结构。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"value",
		//			"text":"要设置的数字数据，注意必须是32位数字，超出范围高位会被舍弃。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true, 失败返回 undefined."
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
	//	"text":"获取系统 API 函数的错误码，每当调用一个 Windows API 函数后，都可以用这个函数来获取额外的错误信息，一般函数如果没有错误，返回值是 0。",
	//	"remark":[
	//		"注意：这个函数必须在每一 API 调用之后立即调用，因为每一个 API 调用都会重置它的值。如果呼叫 API 函数之后，使用了其它函数，而其它函数在内部是否呼叫了某个 API 函数是不确定的，错误码是否被重置也是不确定的。"
	//	],
	//	"param":[
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"返回一个数字，一般 0 表示没有错误发生。"
	//	}
	//}//*
	Handle<Value> getLastError(const Arguments& args){
		HandleScope stack;
		return stack.Close(Uint32::New(GetLastError()));
	}
	//*,{
	//	"type":"function",
	//	"name":"getSysMessage([code])",
	//	"text":"获取错误码的系统描述，这函数可以用来获取 API 函数 GetLastError 和 WSAGetLastError 返回的错误码的文本描述。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"[code]",
	//			"text":"要取回描述的错误码，它可以是 getLastError 和 wsaGetLastError 返回的值，如果这个值缺省，它会在内部调用 getLastError，并且返回描述字串。"
	//		}
	//	],
	//	"return":{
	//		"type":"string",
	//		"text":"成功返回一个字串, 失败返回 undefined. 除非发生极端情况，这个函数是不会失败的。"
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