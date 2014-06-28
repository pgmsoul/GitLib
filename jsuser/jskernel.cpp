#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"ker",
	//	"text":"内存，定时器。",
	//	"remark":[
	//		""
	//	],
	//	"member":[
	//{
	//	"type":"class",
	//	"name":"CMemory",
	//	"text":"内存是一个非常重要的概念，因为任何数据，包括数字和字串，都占用一定的内存空间，也就是说，任何数据都是内存，所以内存的操作非常重要。",
	//	"member":[//*
	class JSMemory : public JsHandleObject<cs::Memory<char>,char*,JSMemory>{
	public:
		//*{
		//	"type":"function",
		//	"name":"bind(pointer,size)",
		//	"text":"把指针绑定到内存对象（从而可以对它进行内存操作）",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"pointer",
		//		"text":"一个内存指针（C++任何对象包括数据的指针都是内存指针）"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"size",
		//		"text":"这个内存指针的大小，比如整数指针的大小是 4 ，double 指针的大小是 8，具体大小由具体对象来决定。"
		//	}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"指示绑定是否成功，如果当前对象的内存对象已经释放，返回 undefined 。"
		//	},
		//	"remark":[
		//		"绑定类型的内存不能调整大小，所有需要调整大小的函数都会失败。解除绑定使用 close 函数"
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
		//	"text":"释放内存空间或者解除绑定的内存指针。",
		//	"return":{
		//		"type":"boolean",
		//		"text":"此函数总是返回 true，除非对象内部的内存对象已经释放（dispose），此时返回 undefined。"
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
		//	"text":"把内存数据置 0 ",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"offset",
		//		"text":"要置零的内存位置，缺省是 0 。"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"length",
		//		"text":"要置零的内存大小，缺省是 -1，也就是到内存尾部。"
		//	}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"返回实际被置零的内存长度，如果内存对象已经disposed，返回 undefined 。"
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
		//	"text":"把指定位置和长度的内存用 base64 编码字串表示，返回这个字串。",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"start",
		//		"text":"要转换的内存位置，缺省是 0 。"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"length",
		//		"text":"要转换的内存大小，缺省是 -1，也就是到内存尾部。"
		//	}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"返回一个字串，如果内存对象已经disposed，返回 undefined 。"
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
		//	"text":"把 base64 字串转换为二进制数据，并且填充到指定位置，内存可能被扩充。",
		//	"param":[
		//	{
		//		"type":"string",
		//		"name":"base64",
		//		"text":"base64 字串。"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"[start]",
		//		"text":"数据被填充的位置，缺省是 0 ，如果源数据比较大，超出内存的现有容量，会自动扩充"
		//	}
		//	],
		//	"return":{
		//		"type":"number",
		//		"text":"返回转换的二进制数据长度。"
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
		//	"text":"把指定位置和长度的内存用16进制字串表示，返回这个字串。",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"start",
		//		"text":"要转换的内存位置，缺省是 0 。"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"length",
		//		"text":"要转换的内存大小，缺省是 -1，也就是到内存尾部。"
		//	}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"返回一个16进制字串，如果内存对象已经disposed，返回 undefined 。"
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
		//	"text":"把16进制字串转换为二进制数据，并且填充到指定位置，内存可能被扩充。",
		//	"param":[
		//	{
		//		"type":"string",
		//		"name":"hex",
		//		"text":"16进制字串(不能包含0x开头)，如果字串含有非16进制字符，会被剔除，转换仍然会成功。"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"start",
		//		"text":"数据被填充的位置，缺省是 0 ，如果源数据比较大，超出内存的现有容量，会自动扩充"
		//	}
		//	],
		//	"return":{
		//		"type":"number",
		//		"text":"返回转换的二进制数据长度。"
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
		//	"text":"把内存容量双倍，但是不管原来的容量是多少，不管minSize设置为多少 ，双倍操作的容量不会小于 0x20",
		//	"param":[
		//	{
		//		"type":"integer",
		//		"name":"minSize",
		//		"text":"最少的容量，如果内存容量双倍后，不能达到这个容量，则会被扩充为这个容量。"
		//	}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"返回操作是否成功，如果系统内存不够，操作可能会失败，失败返回 undefined，成功返回 true"
		//	}
		//}//*
		static Handle<Value> doub(const Arguments& args){
			HandleScope store;
			cs::Memory<char>* mem;
			if(!GetCHandle(mem,args.This())) return Undefined();
			uint msize = GET_ARGS_INT(0,0);
			if(!mem->Double(msize)){
				ThrowException(NEW_STR("CMemory double() 失败"));
				return Undefined();
			}
			return True();
		}
		//*,{
		//	"type":"function",
		//	"name":"getNumber(type,[sign],[start])",
		//	"text":"C语言常用的数字类型有 char、short、int32、float、double、int64 以及它们对应的无符号类型，但是 JavaScript 只有一种数字类型，所以需要在内存中保存对应的数字类型。这个函数获取一段内存表示对应类型的数字。",
		//	"param":[
		//	{
		//		"type":"integer|string",
		//		"name":"type",
		//		"text":"内存数据的类型，有以下 12 种：char（1字节，用 0 表示）、short（2字节，用 1 表示）、int32（4字节，用 2 表示）、float（4字节，用 3 表示）、double（8字节，用 4 表示）、int64（8字节，用 5 表示） 以及它们对应的无符号类型（代码和有符号类型相同）。"
		//	},
		//	{
		//		"type":"boolean",
		//		"name":"sign",
		//		"text":"把内存看成有符号数字，还是无符号数字，缺省表示有符号数字。比如-127和129（-1和255）这些单字节数字在内存中实际上是相同的。这个参数对 double 和 float 类型无效，因为它们总是有符号的。"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"type",
		//		"text":"读取的内存位置偏移"
		//	}
		//	],
		//	"return":{
		//		"type":"number",
		//		"text":"返回转换后的数字，如果起始位置到内存的结尾长度不够一个指定类型的大小，返回 undefined 。"
		//	},
		//	"remark":[
		//		"JavaScript 并不能表示64位整数的最大值, JavaScript的有效范围是 正负0x100000000000000之间."
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
		//	"text":"C语言常用的数字类型有 char、short、int32、float、double、int64 以及它们对应的无符号类型，但是 JavaScript 只有一种数字类型，所以需要在内存中保存对应的数字类型。这个函数设置一段内存表示对应类型的数字。",
		//	"param":[
		//	{
		//		"type":"number",
		//		"name":"number",
		//		"text":"要设置的数值"
		//	},
		//	{
		//		"type":"integer|string",
		//		"name":"type",
		//		"text":"内存数据的类型，有以下 12 种：char（1字节，用 0 表示）、short（2字节，用 1 表示）、int32（4字节，用 2 表示）、float（4字节，用 3 表示）、double（8字节，用 4 表示）、int64（8字节，用 5 表示） 以及它们对应的无符号类型（代码和有符号类型相同）。"
		//	},
		//	{
		//		"type":"integer",
		//		"name":"start",
		//		"text":"保存位置"
		//	}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，如果内存分配失败、类型参数错误、对象为空，返回 undefined。"
		//	},
		//	"remark":[
		//		"JavaScript 并不能表示64位整数的最大值, JavaScript的有效范围是 正负0x100000000000000之间.",
		//		"如果内存不够，会自动调整大小",
		//		"有符号和无符号数字在内存中的数据是相同的，所以不需要指定数字的符号。"
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
		//	"text":"把内存转换为字串",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"start",
		//			"text":"内存数据的起始位置，缺省是 0 。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"内存数据的长度，缺省是 -1，表示到内存的结尾，但是如果有 NULL 字符，字串会截断。"
		//		},
		//		{
		//			"type":"integer|string",
		//			"name":"code",
		//			"text":"字符串编码，可以是数字或者字串名称，如果这个参数和数据的编码不符，会产生乱码。如果这个参数缺省或者为 0 （或者1200），表示 Unicode 编码，如果为 3 ，表示 ansi 编码，65001获取“utf-8”都表示utf8编码。函数使用数字指定编码比使用字串要快。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"返回一个字串"
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
		//	"text":"把字串按指定编码保存到内存的指定位置，内存可能会被扩充。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要保存的字串"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"start",
		//			"text":"要保存的内存位置，缺省是 0 。"
		//		},
		//		{
		//			"type":"integer|string",
		//			"name":"code",
		//			"text":"字符串编码，可以是数字或者字串名称，如果这个参数缺省或者为 0 （或者1200），表示 Unicode 编码，如果为 3 ，表示 ansi 编码，65001或者“utf-8”都表示utf8编码。函数使用数字指定编码比使用字串要快。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"失败返回 undefined，成功返回数据的字节长度，字串保存到内存总是包含一个 NULL 结尾字符，所以即使空字串，返回值也不会是 0 。"
		//	},
		//	"remark":[
		//		"保存字串总是包含一个结尾的 NULL 字符。"
		//	],
		//	"example":[
		//		"var mem = new CMemory();",
		//		"//设置一个utf-8编码的字串值。",
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
		//	"text":"把字串按指定编码保存到内存的指定位置，内存可能会被扩充。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要保存的字串"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"start",
		//			"text":"要保存的内存位置，缺省是 0 。"
		//		},
		//		{
		//			"type":"integer|string",
		//			"name":"code",
		//			"text":"字符串编码，可以是数字或者字串名称，如果这个参数缺省或者为 0 （或者1200），表示 Unicode 编码，如果为 3 ，表示 ansi 编码，65001或者“utf-8”都表示utf8编码。函数使用数字指定编码比使用字串要快。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"失败返回 undefined，成功返回数据的字节长度，字串保存到内存总是包含一个 NULL 结尾字符，所以即使空字串，返回值也不会是 0 。"
		//	},
		//	"remark":[
		//		"保存字串总是包含一个结尾的 NULL 字符。"
		//	],
		//	"example":[
		//		"var mem = new CMemory();",
		//		"//设置一个utf-8编码的字串值。",
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
		//	"text":"在当前内存移动拷贝数据，可能会扩充内存大小。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"src",
		//			"text":"源内存位置"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"dst",
		//			"text":"目的内存位置"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"拷贝数据的大小，这个值不能大于当前内存的有效大小（末尾到源位置长度）。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"失败返回 undefined，成功返回实际移动内存的长度"
		//	},
		//	"remark":[
		//		"移动内存和拷贝内存不同，拷贝内存不能重叠，如果源内存和目的内存有重叠，可能导致数据错误，这种时候需要使用移动内存方式。"
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
		//	"text":"把数据从当前内存的 offset 位置拷贝到目的内存 mem",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"pointer",
		//			"text":"目的内存"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"要拷贝的长度，此长度不能大于有效长度（偏移位置到末尾的距离）"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"offset",
		//			"text":"当前内存的拷贝位置，缺省是 0 ，开头位置"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"失败返回 undefined，成功返回实际拷贝的内存长度"
		//	},
		//	"remark":[
		//		"内存操作使用的是MoveMemory，所以源内存和目的内存可以重叠。"
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
		//	"text":"把数据从 mem 拷贝到当前内存的 offset 位置",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"pointer",
		//			"text":"源内存"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"要拷贝的长度，此长度不能大于有效长度（偏移位置到末尾的距离）"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"offset",
		//			"text":"当前内存的拷贝位置，缺省是 0 ，开头位置"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"失败返回 undefined，成功返回实际拷贝的内存长度"
		//	},
		//	"remark":[
		//		"内存操作使用的是MoveMemory，所以源内存和目的内存可以重叠。"
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
		//	"text":"比较 pointer 指向的内存和当前内存的数据是否相同。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"pointer",
		//			"text":"一个指向内存的指针。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"要比较的数据字节长度，如果这个长度越界，则返回 false。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[offset]",
		//			"text":"当前内存的便宜位置，不一定从开头进行比较，这个值指定比较的位置。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"如果比较的内存数据相同，返回 true，否则返回 false，函数失败（可能由错误的参数导致）返回 undefined。"
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
		//	"text":"获取设置内存的大小，对于绑定类型的内存，设置无效"
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
			//func上的属性和函数, 是对象函数(此处是CMemory)的prototype上的.
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			//inst上的属性和函数是实例本身的, 所以func上效率要高一些(不用生成实例的时候生成这些属性), 特别是函数, 没有必要在inst上设置.
			//inst会上的属性优先于func上的. 特别的, inst上设置了Accessor也会屏蔽func上的同名属性.
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			//Accessor会覆盖同名的属性, 不管是先设置Accessor还是先设置属性.
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
	//	"text":"定时器相关函数。",
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
	//	"text":"设置一个周期性调用的计时器，callback 是一个函数，这个函数每 ms 毫秒被呼叫一次。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"ms",
	//			"text":"计时器的周期，单位是毫秒。"
	//		},
	//		{
	//			"type":"function",
	//			"name":"callback",
	//			"text":"计时器回调函数。"
	//		}
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"失败返回 undefined，成功返回一个标识这个计时器的整数id，每个计时器都有自己独立的id，这个id可以用于删除定时器的参数。"
	//	},
	//	"remark":[
	//		"如果需要周期性调用的函数有参数，可以定义一个新函数，设置为 callback，然后在新函数里调用具有参数的函数。",
	//		"这个函数只能用于窗口消息循环程序，即 startMsgLoop(false) 的情况，如果使用了 startMsgLoop(true)，计时器无法正常工作。"
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
	//	"text":"停止一个计时器。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"id",
	//			"text":"要删除的计时器 id，这个值是 setTimer 函数返回的。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"失败返回 undefined，成功返回 true。"
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