#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"ker",
	//	"text":"�ڴ棬��ʱ����",
	//	"remark":[
	//		""
	//	],
	//	"member":[
	//{
	//	"type":"class",
	//	"name":"CMemory",
	//	"text":"�ڴ���һ���ǳ���Ҫ�ĸ����Ϊ�κ����ݣ��������ֺ��ִ�����ռ��һ�����ڴ�ռ䣬Ҳ����˵���κ����ݶ����ڴ棬�����ڴ�Ĳ����ǳ���Ҫ��",
	//	"member":[//*
	class JSMemory : public JsHandleObject<cs::Memory<char>,char*,JSMemory>{
	public:
		//*{
		//	"type":"function",
		//	"name":"bind(pointer,size)",
		//	"text":"��ָ��󶨵��ڴ���󣨴Ӷ����Զ��������ڴ������",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"pointer",
		//		"text":"һ���ڴ�ָ�루C++�κζ���������ݵ�ָ�붼���ڴ�ָ�룩"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"size",
		//		"text":"����ڴ�ָ��Ĵ�С����������ָ��Ĵ�С�� 4 ��double ָ��Ĵ�С�� 8�������С�ɾ��������������"
		//	}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"ָʾ���Ƿ�ɹ��������ǰ������ڴ�����Ѿ��ͷţ����� undefined ��"
		//	},
		//	"remark":[
		//		"�����͵��ڴ治�ܵ�����С��������Ҫ������С�ĺ�������ʧ�ܡ������ʹ�� close ����"
		//	]
		//}//*
		static Handle<Value> bind(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;
				if(!args[0]->IsUint32()||!args[1]->IsUint32()) break;
				char* p = (char*)(UINT_PTR)args[0]->Uint32Value();
				int size = args[1]->Uint32Value();
				mem->Bind(p,size);
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"close()",
		//	"text":"�ͷ��ڴ�ռ���߽���󶨵��ڴ�ָ�롣",
		//	"return":{
		//		"type":"boolean",
		//		"text":"�˺������Ƿ��� true�����Ƕ����ڲ����ڴ�����Ѿ��ͷţ�dispose������ʱ���� undefined��"
		//	}
		//}//*
		static Handle<Value> close(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;
				mem->Free();
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"zero([offset],[length])",
		//	"text":"���ڴ������� 0 ",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"offset",
		//		"text":"Ҫ������ڴ�λ�ã�ȱʡ�� 0 ��"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"length",
		//		"text":"Ҫ������ڴ��С��ȱʡ�� -1��Ҳ���ǵ��ڴ�β����"
		//	}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"����ʵ�ʱ�������ڴ泤�ȣ�����ڴ�����Ѿ�disposed������ undefined ��"
		//	}
		//}//*
		static Handle<Value> zero(const Arguments& args){
			HandleScope store;
			cs::Memory<char>* mem;
			while(true){
				if(!GetCHandle(mem,args.This())) break;
				uint offset = GET_ARGS_INT(0,0);
				if(offset>mem->Length()) return store.Close(Uint32::New(0));
				uint length = GET_ARGS_INT(1,-1);
				if(length>(mem->Length()-offset)) length = mem->Length() - offset;
				memset(mem->Handle()+offset,0,length);
				return store.Close(Uint32::New(length));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"toBase64([start],[length])",
		//	"text":"��ָ��λ�úͳ��ȵ��ڴ��� base64 �����ִ���ʾ����������ִ���",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"start",
		//		"text":"Ҫת�����ڴ�λ�ã�ȱʡ�� 0 ��"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"length",
		//		"text":"Ҫת�����ڴ��С��ȱʡ�� -1��Ҳ���ǵ��ڴ�β����"
		//	}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"����һ���ִ�������ڴ�����Ѿ�disposed������ undefined ��"
		//	}
		//}//*
		static Handle<Value> toBase64(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;
				uint start = GET_ARGS_INT(0,0);
				if(start>mem->Length()) break;
				uint length = GET_ARGS_INT(1,-1);
				uint max_len = mem->Length() - start;
				if(length>max_len) length = max_len;
				cs::Memory<char> code;
				uint codelen = cs::Base64::Encode(code,mem->Handle()+start,length);
				cs::String str;
				str.FromMultiByte(code,codelen);
				return String::New((uint16_t*)str.Handle());
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"fromBase64(base64,[start])",
		//	"text":"�� base64 �ִ�ת��Ϊ���������ݣ�������䵽ָ��λ�ã��ڴ���ܱ����䡣",
		//	"param":[
		//	{
		//		"type":"string",
		//		"name":"base64",
		//		"text":"base64 �ִ���"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"[start]",
		//		"text":"���ݱ�����λ�ã�ȱʡ�� 0 �����Դ���ݱȽϴ󣬳����ڴ���������������Զ�����"
		//	}
		//	],
		//	"return":{
		//		"type":"number",
		//		"text":"����ת���Ķ��������ݳ��ȡ�"
		//	}
		//}//*
		static Handle<Value> fromBase64(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;

				cs::String base64;
				GetString(args[0],base64);
				int pos = GET_ARGS_INT(1,0);
				if(pos<0) break;

				cs::Memory<char> char64;
				int len = base64.ToMultiByte(&char64);
				if(len==0) break;
				int codelen = 0;
				cs::Base64::Decode(0,&codelen,char64,len);
				
				bool resetlen = false;
				if(pos+(int)mem->Length()<codelen){
					if(!mem->Expand(codelen-pos-mem->Length())) break;
					resetlen = true;
				}
				len = cs::Base64::Decode(mem->Handle()+pos,&codelen,char64,len);
				if(len!=0) break;
				if(resetlen&&codelen+pos<(int)mem->Length()){
					mem->SetLength(codelen+pos);
				}
				return store.Close(Uint32::New(codelen));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"toHex([start],[length])",
		//	"text":"��ָ��λ�úͳ��ȵ��ڴ���16�����ִ���ʾ����������ִ���",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"start",
		//		"text":"Ҫת�����ڴ�λ�ã�ȱʡ�� 0 ��"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"length",
		//		"text":"Ҫת�����ڴ��С��ȱʡ�� -1��Ҳ���ǵ��ڴ�β����"
		//	}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"����һ��16�����ִ�������ڴ�����Ѿ�disposed������ undefined ��"
		//	}
		//}//*
		static Handle<Value> toHex(const Arguments& args){
			HandleScope store;
			cs::Memory<char>* mem;
			if(!GetCHandle(mem,args.This())) return Undefined();
			uint offset = GET_ARGS_INT(0,0);
			if(offset>mem->Length()) offset = mem->Length();
			uint length = GET_ARGS_INT(1,-1);
			if(length>(mem->Length()-offset)) length = mem->Length() - offset;
			cs::String hex;
			hex.HexBinary(mem->Handle()+offset,length);
			return String::New((uint16_t*)hex.Handle());
		}
		//*,{
		//	"type":"function",
		//	"name":"fromHex(hex,[start])",
		//	"text":"��16�����ִ�ת��Ϊ���������ݣ�������䵽ָ��λ�ã��ڴ���ܱ����䡣",
		//	"param":[
		//	{
		//		"type":"string",
		//		"name":"hex",
		//		"text":"16�����ִ�(���ܰ���0x��ͷ)������ִ����з�16�����ַ����ᱻ�޳���ת����Ȼ��ɹ���"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"start",
		//		"text":"���ݱ�����λ�ã�ȱʡ�� 0 �����Դ���ݱȽϴ󣬳����ڴ���������������Զ�����"
		//	}
		//	],
		//	"return":{
		//		"type":"number",
		//		"text":"����ת���Ķ��������ݳ��ȡ�"
		//	}
		//}//*
		static Handle<Value> fromHex(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::Memory<byte>* mem;
				if(!GetCHandle(mem,args.This())) break;

				cs::String hex;
				GetString(args[0],hex);
				int pos = GET_ARGS_INT(1,0);
				int len = hex.ToBinary(*mem,pos);
				return store.Close(Uint32::New(len));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"double([minSize])",
		//	"text":"���ڴ�����˫�������ǲ���ԭ���������Ƕ��٣�����minSize����Ϊ���� ��˫����������������С�� 0x20",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"minSize",
		//		"text":"���ٵ�����������ڴ�����˫���󣬲��ܴﵽ�����������ᱻ����Ϊ���������"
		//	}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"���ز����Ƿ�ɹ������ϵͳ�ڴ治�����������ܻ�ʧ�ܣ�ʧ�ܷ��� undefined���ɹ����� true"
		//	}
		//}//*
		static Handle<Value> doub(const Arguments& args){
			HandleScope store;
			cs::Memory<char>* mem;
			if(!GetCHandle(mem,args.This())) return Undefined();
			uint msize = GET_ARGS_INT(0,0);
			if(!mem->Double(msize)){
				ThrowException(NEW_STR("CMemory double() ʧ��"));
				return Undefined();
			}
			return True();
		}
		//*,{
		//	"type":"function",
		//	"name":"getNumber(type,[sign],[start])",
		//	"text":"C���Գ��õ����������� char��short��int32��float��double��int64 �Լ����Ƕ�Ӧ���޷������ͣ����� JavaScript ֻ��һ���������ͣ�������Ҫ���ڴ��б����Ӧ���������͡����������ȡһ���ڴ��ʾ��Ӧ���͵����֡�",
		//	"param":[
		//	{
		//		"type":"integer|string",
		//		"name":"type",
		//		"text":"�ڴ����ݵ����ͣ������� 12 �֣�char��1�ֽڣ��� 0 ��ʾ����short��2�ֽڣ��� 1 ��ʾ����int32��4�ֽڣ��� 2 ��ʾ����float��4�ֽڣ��� 3 ��ʾ����double��8�ֽڣ��� 4 ��ʾ����int64��8�ֽڣ��� 5 ��ʾ�� �Լ����Ƕ�Ӧ���޷������ͣ�������з���������ͬ����"
		//	},
		//	{
		//		"type":"boolean",
		//		"name":"sign",
		//		"text":"���ڴ濴���з������֣������޷������֣�ȱʡ��ʾ�з������֡�����-127��129��-1��255����Щ���ֽ��������ڴ���ʵ��������ͬ�ġ���������� double �� float ������Ч����Ϊ���������з��ŵġ�"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"type",
		//		"text":"��ȡ���ڴ�λ��ƫ��"
		//	}
		//	],
		//	"return":{
		//		"type":"number",
		//		"text":"����ת��������֣������ʼλ�õ��ڴ�Ľ�β���Ȳ���һ��ָ�����͵Ĵ�С������ undefined ��"
		//	},
		//	"remark":[
		//		"JavaScript �����ܱ�ʾ64λ���������ֵ, JavaScript����Ч��Χ�� ����0x100000000000000֮��."
		//	]
		//}//*
		static Handle<Value> getNumber(const Arguments& args){
#define NUM_TYPE_CHAR	0
#define NUM_TYPE_SHORT	1
#define NUM_TYPE_INT	2
#define NUM_TYPE_FLOAT	3
#define NUM_TYPE_DOUBLE	4
#define NUM_TYPE_INT64	5
			HandleScope store;
			cs::Memory<char>* mem;
			if(!GetCHandle(mem,args.This())) return Undefined();
			uint start = GET_ARGS_INT(2,0);
			uint type;
			if(args[0]->IsInt32()){
				type = args[0]->Int32Value();
			}else{
				cs::String tn;
				GetString(args[1],tn);
				if(cs::WcsEqualNoCase(tn,L"char")){
					type = NUM_TYPE_CHAR;
				}else if(cs::WcsEqualNoCase(tn,L"short")){
					type = NUM_TYPE_SHORT;
				}else if(cs::WcsEqualNoCase(tn,L"int32")){
					type = NUM_TYPE_INT;
				}else if(cs::WcsEqualNoCase(tn,L"float")){
					type = NUM_TYPE_FLOAT;
				}else if(cs::WcsEqualNoCase(tn,L"double")){
					type = NUM_TYPE_DOUBLE;
				}else if(cs::WcsEqualNoCase(tn,L"int64")){
					type = NUM_TYPE_INT64;
				}else return Undefined();
			}
			bool sign = true;
			if(args.Length()>1&&args[1]->IsBoolean()){
				sign = args[1]->BooleanValue();
			}
			char* p = (char*)mem->Handle();
			switch(type){
				case NUM_TYPE_CHAR:
					{
						if(start+1>mem->Length()) return Undefined();
						int v;
						if(sign)
							v = *(char*)(p+start);
						else
							v = *(byte*)(p+start);
						return store.Close(Int32::New(v));
					}
				case NUM_TYPE_SHORT:
					{
						if(start+2>mem->Length()) return Undefined();
						int v;
						if(sign)
							v = *(short*)(p+start);
						else
							v = *(WORD*)(p+start);
						return store.Close(Int32::New(v));
					}
				case NUM_TYPE_INT:
					{
						if(start+4>mem->Length()) return Undefined();
						if(sign){
							return store.Close(Int32::New(*(int*)(p+start)));
						}else{
							return store.Close(Uint32::New(*(uint*)(p+start)));
						}
					}
				case NUM_TYPE_FLOAT:
					{
						if(start+sizeof(float)>mem->Length()) return Undefined();
						return store.Close(Number::New(*(float*)(p+start)));
					}
				case NUM_TYPE_DOUBLE:
					{
						if(start+sizeof(double)>mem->Length()) return Undefined();
						return store.Close(Number::New(*(double*)(p+start)));
					}
				case NUM_TYPE_INT64:
					{
						if(start+8>mem->Length()) return Undefined();
						if(sign){
							return store.Close(Number::New((double)*(int64*)(p+start)));
						}else{
							return store.Close(Number::New((double)*(uint64*)(p+start)));
						}
					}
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setNumber(number,type,[start])",
		//	"text":"C���Գ��õ����������� char��short��int32��float��double��int64 �Լ����Ƕ�Ӧ���޷������ͣ����� JavaScript ֻ��һ���������ͣ�������Ҫ���ڴ��б����Ӧ���������͡������������һ���ڴ��ʾ��Ӧ���͵����֡�",
		//	"param":[
		//	{
		//		"type":"number",
		//		"name":"number",
		//		"text":"Ҫ���õ���ֵ"
		//	},
		//	{
		//		"type":"integer|string",
		//		"name":"type",
		//		"text":"�ڴ����ݵ����ͣ������� 12 �֣�char��1�ֽڣ��� 0 ��ʾ����short��2�ֽڣ��� 1 ��ʾ����int32��4�ֽڣ��� 2 ��ʾ����float��4�ֽڣ��� 3 ��ʾ����double��8�ֽڣ��� 4 ��ʾ����int64��8�ֽڣ��� 5 ��ʾ�� �Լ����Ƕ�Ӧ���޷������ͣ�������з���������ͬ����"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"start",
		//		"text":"����λ��"
		//	}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true������ڴ����ʧ�ܡ����Ͳ������󡢶���Ϊ�գ����� undefined��"
		//	},
		//	"remark":[
		//		"JavaScript �����ܱ�ʾ64λ���������ֵ, JavaScript����Ч��Χ�� ����0x100000000000000֮��.",
		//		"����ڴ治�������Զ�������С",
		//		"�з��ź��޷����������ڴ��е���������ͬ�ģ����Բ���Ҫָ�����ֵķ��š�"
		//	]
		//}//*
		static Handle<Value> setNumber(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;
				uint start = GET_ARGS_INT(2,0);
				uint type;
				if(args[1]->IsInt32()){
					type = args[1]->Int32Value();
				}else{
					cs::String tn;
					GetString(args[1],tn);
					if(cs::WcsEqualNoCase(tn,L"char")){
						type = NUM_TYPE_CHAR;
					}else if(cs::WcsEqualNoCase(tn,L"short")){
						type = NUM_TYPE_SHORT;
					}else if(cs::WcsEqualNoCase(tn,L"int32")){
						type = NUM_TYPE_INT;
					}else if(cs::WcsEqualNoCase(tn,L"float")){
						type = NUM_TYPE_FLOAT;
					}else if(cs::WcsEqualNoCase(tn,L"double")){
						type = NUM_TYPE_DOUBLE;
					}else if(cs::WcsEqualNoCase(tn,L"int64")){
						type = NUM_TYPE_INT64;
					}else break;
				}
				switch(type){
				case NUM_TYPE_CHAR:
					{
						if(start+1>mem->Length()){
							if(!mem->Double(start+1)) return Undefined();
						}
						*(char*)(mem->Handle()+start) = 0xFF&args[0]->Int32Value();
						break;
					}
				case NUM_TYPE_SHORT:
					{
						if(start+2>mem->Length()){
							if(!mem->Double(start+2)) return Undefined();
						}
						*(short*)(mem->Handle()+start) = 0xFFFF&args[0]->Int32Value();
						break;
					}
				case NUM_TYPE_INT:
					{
						if(start+4>mem->Length()){
							if(!mem->Double(start+4)) return Undefined();
						}
						*(int*)(mem->Handle()+start) = 0xFFFFFFFF&args[0]->IntegerValue();
						break;
					}
				case NUM_TYPE_FLOAT:
					{
						if(start+sizeof(float)>mem->Length()){
							if(!mem->Double(start+sizeof(float))) return Undefined();
						}
						*(float*)(mem->Handle()+start) = (float)args[0]->NumberValue();
						break;
					}
				case NUM_TYPE_DOUBLE:
					{
						if(start+sizeof(double)>mem->Length()){
							if(!mem->Double(start+sizeof(double))) return Undefined();
						}
						*(double*)(mem->Handle()+start) = (double)args[0]->NumberValue();
						break;
					}
				case NUM_TYPE_INT64:
					{
						if(start+8>mem->Length()){
							if(!mem->Double(start+8)) return Undefined();
						}
						*(int64*)(mem->Handle()+start) = args[0]->IntegerValue();
						break;
					}
				default:
					return Undefined();
				}
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getString([start],[length],[code])",
		//	"text":"���ڴ�ת��Ϊ�ִ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"start",
		//			"text":"�ڴ����ݵ���ʼλ�ã�ȱʡ�� 0 ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"�ڴ����ݵĳ��ȣ�ȱʡ�� -1����ʾ���ڴ�Ľ�β����������� NULL �ַ����ִ���ضϡ�"
		//		},
		//		{
		//			"type":"integer|string",
		//			"name":"code",
		//			"text":"�ַ������룬���������ֻ����ִ����ƣ����������������ݵı��벻������������롣����������ȱʡ����Ϊ 0 ������1200������ʾ Unicode ���룬���Ϊ 3 ����ʾ ansi ���룬65001��ȡ��utf-8������ʾutf8���롣����ʹ������ָ�������ʹ���ִ�Ҫ�졣"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"����һ���ִ�"
		//	}
		//}//*
		static Handle<Value> getString(const Arguments& args){
			HandleScope store;
			while(true){
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;
				int pos = GET_ARGS_INT(0,0);
				if(pos<0) break;
				int len = GET_ARGS_INT(1,0);
				int cp = 3;
				cs::String str;
				if(args.Length()>2){
					if(args[2]->IsInt32())
						cp = args[2]->Int32Value();
					else{
						cs::String code;
						GetString(args[2],code);
						cp = cs::GetCodePage(code);
					}
				}
				if(len<=0||len>(int)mem->Length()-pos) len = mem->Length() - pos;
				str.FromMultiByte((LPCSTR)(mem->Handle()+pos),len,cp);

				return store.Close(String::New((uint16_t*)str.Handle()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setString(str,[start],[code])",
		//	"text":"���ִ���ָ�����뱣�浽�ڴ��ָ��λ�ã��ڴ���ܻᱻ���䡣",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ������ִ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"start",
		//			"text":"Ҫ������ڴ�λ�ã�ȱʡ�� 0 ��"
		//		},
		//		{
		//			"type":"integer|string",
		//			"name":"code",
		//			"text":"�ַ������룬���������ֻ����ִ����ƣ�����������ȱʡ����Ϊ 0 ������1200������ʾ Unicode ���룬���Ϊ 3 ����ʾ ansi ���룬65001���ߡ�utf-8������ʾutf8���롣����ʹ������ָ�������ʹ���ִ�Ҫ�졣"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"ʧ�ܷ��� undefined���ɹ��������ݵ��ֽڳ��ȣ��ִ����浽�ڴ����ǰ���һ�� NULL ��β�ַ������Լ�ʹ���ִ�������ֵҲ������ 0 ��"
		//	},
		//	"remark":[
		//		"�����ִ����ǰ���һ����β�� NULL �ַ���"
		//	],
		//	"example":[
		//		"var mem = new CMemory();",
		//		"//����һ��utf-8������ִ�ֵ��",
		//		"mem.setString(\"string value\",65001);"
		//	]
		//}
		//*
		static Handle<Value> setString(const Arguments& args){
			HandleScope store;
			int rt = 0;
			while(1){
				if(args.Length()<1) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;

				cs::String str;
				GetString(args[0],str);

				int pos = GET_ARGS_INT(1,0);
				if(pos<0) break;

				int cp = 3;
				if(args.Length()>2){
					if(args[2]->IsInt32())
						cp = args[2]->Int32Value();
					else{
						cs::String code;
						GetString(args[2],code);
						cp = cs::GetCodePage(code);
					}
				}
				int len = str.ToMultiByte(0,0,cp);
				/*if(cp==0||cp==1200) len += 2;
				else len += 1;*/
				if(len+pos>(int)mem->Length()){
					if(!mem->SetLength(len+pos)) break;
				}
				char* h = mem->Handle()+pos;
				rt = str.ToMultiByte((LPSTR)h,len,cp);
				return store.Close(Int32::New(rt));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setString(str,[start],[code])",
		//	"text":"���ִ���ָ�����뱣�浽�ڴ��ָ��λ�ã��ڴ���ܻᱻ���䡣",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫ������ִ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"start",
		//			"text":"Ҫ������ڴ�λ�ã�ȱʡ�� 0 ��"
		//		},
		//		{
		//			"type":"integer|string",
		//			"name":"code",
		//			"text":"�ַ������룬���������ֻ����ִ����ƣ�����������ȱʡ����Ϊ 0 ������1200������ʾ Unicode ���룬���Ϊ 3 ����ʾ ansi ���룬65001���ߡ�utf-8������ʾutf8���롣����ʹ������ָ�������ʹ���ִ�Ҫ�졣"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"ʧ�ܷ��� undefined���ɹ��������ݵ��ֽڳ��ȣ��ִ����浽�ڴ����ǰ���һ�� NULL ��β�ַ������Լ�ʹ���ִ�������ֵҲ������ 0 ��"
		//	},
		//	"remark":[
		//		"�����ִ����ǰ���һ����β�� NULL �ַ���"
		//	],
		//	"example":[
		//		"var mem = new CMemory();",
		//		"//����һ��utf-8������ִ�ֵ��",
		//		"mem.setString(\"string value\",65001);"
		//	]
		//}
		//*
		static Handle<Value> setNullEndString(const Arguments& args){
			HandleScope store;
			int rt = 0;
			while(1){
				if(args.Length()<1) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;

				cs::String str;
				GetString(args[0],str);

				int pos = GET_ARGS_INT(1,0);
				if(pos<0) break;

				int cp = 3;
				if(args.Length()>2){
					if(args[2]->IsInt32())
						cp = args[2]->Int32Value();
					else{
						cs::String code;
						GetString(args[2],code);
						cp = cs::GetCodePage(code);
					}
				}
				int len = str.ToMultiByte(0,0,cp);
				if(cp==0||cp==1200) len += 2;
				else len += 1;
				if(len+pos>(int)mem->Length()){
					if(!mem->Double(len+pos)) break;
				}
				char* h = mem->Handle()+pos;
				rt = str.ToMultiByte((LPSTR)h,len,cp);
				if(cp==0||cp==1200){
					rt += 2;
					h[rt] = h[rt+1] = 0;
				}else{
					h[rt] = 0;
					rt += 1;
				}
				return store.Close(Int32::New(rt));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"move(src,dst,length)",
		//	"text":"�ڵ�ǰ�ڴ��ƶ��������ݣ����ܻ������ڴ��С��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"src",
		//			"text":"Դ�ڴ�λ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"dst",
		//			"text":"Ŀ���ڴ�λ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"�������ݵĴ�С�����ֵ���ܴ��ڵ�ǰ�ڴ����Ч��С��ĩβ��Դλ�ó��ȣ���"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"ʧ�ܷ��� undefined���ɹ�����ʵ���ƶ��ڴ�ĳ���"
		//	},
		//	"remark":[
		//		"�ƶ��ڴ�Ϳ����ڴ治ͬ�������ڴ治���ص������Դ�ڴ��Ŀ���ڴ����ص������ܵ������ݴ�������ʱ����Ҫʹ���ƶ��ڴ淽ʽ��"
		//	]
		//}
		//*
		static Handle<Value> move(const Arguments& args){
			HandleScope store;
			cs::Memory<char>* mem;
			if(!GetCHandle(mem,args.This())) return store.Close(Uint32::New(0));

			uint dst,src,len;
			dst = GET_ARGS_INT(0,0);
			src = GET_ARGS_INT(1,0);
			len = GET_ARGS_INT(2,0);
			return store.Close(Uint32::New(mem->Move(dst,src,len)));
		}
		//*,{
		//	"type":"function",
		//	"name":"copyTo(pointer,length,[offset])",
		//	"text":"�����ݴӵ�ǰ�ڴ�� offset λ�ÿ�����Ŀ���ڴ� mem",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"pointer",
		//			"text":"Ŀ���ڴ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"Ҫ�����ĳ��ȣ��˳��Ȳ��ܴ�����Ч���ȣ�ƫ��λ�õ�ĩβ�ľ��룩"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"offset",
		//			"text":"��ǰ�ڴ�Ŀ���λ�ã�ȱʡ�� 0 ����ͷλ��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"ʧ�ܷ��� undefined���ɹ�����ʵ�ʿ������ڴ泤��"
		//	},
		//	"remark":[
		//		"�ڴ����ʹ�õ���MoveMemory������Դ�ڴ��Ŀ���ڴ�����ص���"
		//	]
		//}
		//*
		static Handle<Value> copyTo(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;
				if(!args[0]->IsUint32()||args[1]->IsUint32()) break;
				LPVOID dst = (LPVOID)(UINT_PTR)args[0]->Uint32Value();
				uint len = args[1]->Uint32Value();
				uint offset = GET_ARGS_INT(2,0);
				return store.Close(Uint32::New(mem->CopyTo((char*)dst,len,offset)));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"copyFrom(pointer,length,[offset])",
		//	"text":"�����ݴ� mem ��������ǰ�ڴ�� offset λ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"pointer",
		//			"text":"Դ�ڴ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"Ҫ�����ĳ��ȣ��˳��Ȳ��ܴ�����Ч���ȣ�ƫ��λ�õ�ĩβ�ľ��룩"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"offset",
		//			"text":"��ǰ�ڴ�Ŀ���λ�ã�ȱʡ�� 0 ����ͷλ��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"ʧ�ܷ��� undefined���ɹ�����ʵ�ʿ������ڴ泤��"
		//	},
		//	"remark":[
		//		"�ڴ����ʹ�õ���MoveMemory������Դ�ڴ��Ŀ���ڴ�����ص���"
		//	]
		//}//*
		static Handle<Value> copyFrom(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::Memory<char>* mem;
				if(!GetCHandle(mem,args.This())) break;
				if(!args[0]->IsUint32()||!args[1]->IsUint32()) break;
				LPVOID dst = (LPVOID)(UINT_PTR)args[0]->Uint32Value();
				uint len = args[1]->Uint32Value();
				uint offset = GET_ARGS_INT(2,0);
				return store.Close(Uint32::New(mem->CopyFrom((char*)dst,len,offset)));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"equal(pointer,length,[offset])",
		//	"text":"�Ƚ� pointer ָ����ڴ�͵�ǰ�ڴ�������Ƿ���ͬ��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"pointer",
		//			"text":"һ��ָ���ڴ��ָ�롣"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"Ҫ�Ƚϵ������ֽڳ��ȣ�����������Խ�磬�򷵻� false��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[offset]",
		//			"text":"��ǰ�ڴ�ı���λ�ã���һ���ӿ�ͷ���бȽϣ����ֵָ���Ƚϵ�λ�á�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"����Ƚϵ��ڴ�������ͬ������ true�����򷵻� false������ʧ�ܣ������ɴ���Ĳ������£����� undefined��"
		//	}
		//}//*
		static Handle<Value> equal(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::Memory<char>* cobj;
				if(!GetCHandle(cobj,args.This())) break;
				if(!args[0]->IsUint32()||!args[1]->IsUint32()) break;
				LPVOID dst = (LPVOID)(UINT_PTR)args[0]->Uint32Value();
				uint len = GET_ARGS_INT(1,0);
				uint offset = GET_ARGS_INT(2,0);
				if(offset>cobj->Length()) break;
				if(len>cobj->Length()-offset) break;
				if(len==0) return True();
				bool r = memcmp(cobj->Handle()+offset,dst,len)==0;
				return Bool(r);
			}
			return Undefined();
		}
		//*,{
		//	"type":"property",
		//	"name":"length",
		//	"objtype":"integer",
		//	"text":"��ȡ�����ڴ�Ĵ�С�����ڰ����͵��ڴ棬������Ч"
		//}//*
		static Handle<Value> get(cs::String& name,cs::Memory<char>* cobj,Local<Object>& self){
			if(name==L"length"){
				return Uint32::New(cobj->Length());
			}
			return Undefined();
		}
		static void set(cs::String& name,cs::Memory<char>* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"length"){
				int len = value->Uint32Value();
				if(!cobj->SetLength(len)){
					ThrowException(NEW_STR("CMemory set length failed"));
					return;
				}
			}
		}
	public:
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			//func�ϵ����Ժͺ���, �Ƕ�����(�˴���CMemory)��prototype�ϵ�.
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			//inst�ϵ����Ժͺ�����ʵ�������, ����func��Ч��Ҫ��һЩ(��������ʵ����ʱ��������Щ����), �ر��Ǻ���, û�б�Ҫ��inst������.
			//inst���ϵ�����������func�ϵ�. �ر��, inst��������AccessorҲ������func�ϵ�ͬ������.
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			//Accessor�Ḳ��ͬ��������, ������������Accessor��������������.
			SET_CLA_ACCESSOR(length);
			SET_CLA_FUNC(bind);
			SET_CLA_FUNC(zero);
			func->Set(String::New("double"),FunctionTemplate::New(&doub)->GetFunction());
			SET_CLA_FUNC(close);
			SET_CLA_FUNC(fromHex);
			SET_CLA_FUNC(toHex);
			SET_CLA_FUNC(fromBase64);
			SET_CLA_FUNC(toBase64);
			SET_CLA_FUNC(copyTo);
			SET_CLA_FUNC(copyFrom);
			SET_CLA_FUNC(move);
			SET_CLA_FUNC(equal);
			SET_CLA_FUNC(setString);
			SET_CLA_FUNC(getString);
			SET_CLA_FUNC(setNumber);
			SET_CLA_FUNC(getNumber);
		}
	};
	//*]}//*
	//*,{
	//	"type":"group",
	//	"name":"Timer",
	//	"text":"��ʱ����غ�����",
	//	"member":[//*
	class _Timer : public cs::Timer{
	public:
		Persistent<Object> self;
		Persistent<Function> cbTimer;
		~_Timer(){
			self.Dispose();
			cbTimer.Dispose();
		}
	};
	//*{
	//	"type":"function",
	//	"name":"setTimer(ms,callback)",
	//	"text":"����һ�������Ե��õļ�ʱ����callback ��һ���������������ÿ ms ���뱻����һ�Ρ�",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"ms",
	//			"text":"��ʱ�������ڣ���λ�Ǻ��롣"
	//		},
	//		{
	//			"type":"function",
	//			"name":"callback",
	//			"text":"��ʱ���ص�������"
	//		}
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"ʧ�ܷ��� undefined���ɹ�����һ����ʶ�����ʱ��������id��ÿ����ʱ�������Լ�������id�����id��������ɾ����ʱ���Ĳ�����"
	//	},
	//	"remark":[
	//		"�����Ҫ�����Ե��õĺ����в��������Զ���һ���º���������Ϊ callback��Ȼ�����º�������þ��в����ĺ�����",
	//		"�������ֻ�����ڴ�����Ϣѭ�����򣬼� startMsgLoop(false) ����������ʹ���� startMsgLoop(true)����ʱ���޷�����������"
	//	]
	//}//*

	typedef struct TIMERSTRUCT: public cs::_struct{
		UINT id;
		Persistent<Function> cb;
	}TIMERSTRUCT;
	cs::ObjectLink<TIMERSTRUCT>& getTimerStructLink(){
		static cs::ObjectLink<TIMERSTRUCT> _timerStructLink;
		return _timerStructLink;
	}
	void __stdcall _timerProc(HWND hwnd,UINT msg,UINT idEvent,DWORD dwTime){
		cs::ObjectLink<TIMERSTRUCT>& tsl = getTimerStructLink();
		for(tsl.First();;tsl.Move(1)){
			TIMERSTRUCT* ts = tsl.Element();
			if(ts==0) break;
			if(ts->id!=idEvent) continue;
			Handle<Value> argv[1];
			CallFunc(GetGlobal(),ts->cb,0,argv);
			break;
		}
	}
	Handle<Value> setTimer(const Arguments& args){
		HandleScope store;
		while(true){
			if(args.Length()<2) break;
			UINT ms = GET_ARGS_INT(0,0);
			if(!args[1]->IsFunction()) break;
			UINT id = SetTimer(0,0,ms,&_timerProc);
			if(id==0) break;
			TIMERSTRUCT* ts = getTimerStructLink().Add();
			if(ts==0) break;
			ts->id = id;
			ts->cb = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
			return store.Close(Uint32::New(id));
		}
		return Undefined();
	}
	//*,{
	//	"type":"function",
	//	"name":"killTimer(id)",
	//	"text":"ֹͣһ����ʱ����",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"id",
	//			"text":"Ҫɾ���ļ�ʱ�� id�����ֵ�� setTimer �������صġ�"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"ʧ�ܷ��� undefined���ɹ����� true��"
	//	}
	//}//*
	//*]}//*
	//*]}//*
	Handle<Value> killTimer(const Arguments& args){
		HandleScope store;
		while(true){
			if(args.Length()<1) break;
			uint id = GET_ARGS_INT(0,0);
			if(!::KillTimer(0,id)) break;
			cs::ObjectLink<TIMERSTRUCT>& tsl = getTimerStructLink();
			for(tsl.First();;tsl.Move(1)){
				TIMERSTRUCT* ts = tsl.Element();
				if(ts==0) break;
				if(ts->id!=id) continue;
				ts->cb.Dispose();
				tsl.Delete();
				break;
			}
			return True();
		}
		return Undefined();
	}
	void LoadKernel(Handle<Object>& glb){
		JSMemory::Load(glb,L"CMemory",TEMPLATE_ID_MEMORY);
		LoadJsRes(IDR_JS_MEMORY_AFTER,L"memory_after.js");
		SET_OBJ_FUNC_RO(glb,setTimer,setTimer);
		SET_OBJ_FUNC_RO(glb,killTimer,killTimer);
	}
}