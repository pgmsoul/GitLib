#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"file",
	//	"text":"�ļ���ع��ܡ�",
	//	"member":[//*
	//*{
	//	"type":"class",
	//	"name":"CFile",
	//	"text":"���ļ����ݽ��ж�д������",
	//	"member":[//*
	class JSFile : public JsHandleObject<cs::File,HANDLE,JSFile,TEMPLATE_ID_FILE>{
		//*{
		//	"type":"function",
		//	"name":"create(name,[exist],[readonly],[share],[attribute])",
		//	"text":"��һ���ļ�, ����ļ�������, ����������. ���ֻ��һ���Ѿ����ڵ��ļ�(�ļ������ڷ���ʧ��), ʹ�� open ������������������ظ�����, ����һ�����ļ�, ԭ�����ļ����Զ��ر�, ������ʾ���� close ������",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"Ҫ�򿪵��ļ��������û��ָ���ļ�������·�����ļ��ڵ�ǰĿ¼�¡�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[exist]",
		//			"text":"�Ƿ�ֻ��һ���Ѿ����ڵ��ļ������������Ϊ true ʱ�����ָ�����ļ��������ڣ�����ʧ�ܡ�ȱʡֵ��false��"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[readonly]",
		//			"text":"�Ƿ���ֻ��ģʽ���ļ���һ��ֻ��ģʽ�򿪵��ļ���ֻ�ܶ�ȡ�����ܶ��ļ����и�д������д�����ݣ��ı䳤�ȣ��������ԡ�ȱʡֵ�� false��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[share]",
		//			"text":"����ģʽ���� 3 ��ֵ��CFile.SHARE_READ��CFile.SHARE_WRITE��CFile.SHARE_DELETE����ʾ����ͬʱ������ļ�����Ĵ�ģʽ����������� SHARE_READ �����ļ���ͬʱ�����ļ�����Ҳ������ļ��Ļ���ֻ����ֻ��ģʽ������Զ�дģʽ���ļ���ʧ�ܣ�Ҳ����ɾ��������� SHARE_WRITE|SHARE_READ|SHARE_DELETE ģʽ���ļ���������ļ����Ա������ļ����������ȡд���ɾ����ȱʡģʽ�Ƕ�д���ǲ���ɾ������ SHARE_WRITE|SHARE_READ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[attribute]",
		//			"text":"�����ļ������ԣ��������ֻ���ļ���������ʱ����Ч������ļ��Ѿ����ڣ�������������ԡ�һЩ�ļ�����ֵ����:<br/><b>CFile.READONLY</b>: �ļ�����ֻ������. ĳЩ����������������������ļ��Ƿ��д, ���� CFile �����ܴ�����Ӱ��.<br/><b>CFile.HIDDEN</b>: �ļ�������������.<br/><b>CFile.SYSTEM</b>: �ļ�����ϵͳ����.<br/><b>CFile.NORMAL</b>: �ļ�������ͨ����. ����ȱʡֵ.<br/><b>CFile.TEMPORARY</b>: �ļ�������ʱ����. ����Դ����������ļ�, ���ļ��ر�ʱ, �ļ��ᱻϵͳ�Զ�ɾ��.<br/>Ҫʹ�ø��������, ֱ���������Զ�Ӧ����ֵ�����Բ��� MSDN �ĵ�����ȡ��Ӧ��ֵ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		""
		//	]
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope store;
			while(1){
				if(args.Length()<1) break;
				cs::File* f;
				if(!GetCHandle(f,args.This())) break;

				cs::String name;
				GetString(args[0],name);

				DWORD open = GET_ARGS_VALUE(1,false,Boolean)?OPEN_EXISTING:OPEN_ALWAYS;
				DWORD rw = GET_ARGS_VALUE(2,false,Boolean)?FILE_GENERIC_READ:FILE_GENERIC_READ|FILE_GENERIC_WRITE;
				DWORD share = GET_ARGS_INT2(3,FILE_SHARE_WRITE|FILE_SHARE_READ,DWORD);
				DWORD attr = GET_ARGS_INT2(4,FILE_ATTRIBUTE_NORMAL,DWORD);

				f->Close();
				bool r = f->Create(name,open,share,rw,attr);
				if(!r) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"close()",
		//	"text":"�ر�һ���ļ�, �ѻ����������д���ļ�, �ͷ��ļ���ϵͳ��Դ, ���ǲ��ͷŶ���� C++ ��Դ, Ҫ�ͷ� C++ ��Դʹ�� dispose ����. ִ���� close ��CFile��������ٴε��� create ��һ���ļ����в�������������ڴ�����������������õģ������Ҫ��һ�����ļ���ֱ�ӵ��� create �Ϳ��ԣ�close �����ڲ������á����� dispose ��ʱ��close Ҳ�ᱻ�Զ����á�close ����һ������ȷ��Ҫ�ͷ�һ���ļ���ʱ�򱻵��ã���Ϊ��ʱ���ļ������޷�������򿪣�ĳЩ������Ҫ��ռ�ļ���",
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		""
		//	]
		//}//*
		static Handle<Value> close(const Arguments& args){
			cs::File* f;
			if(!GetCHandle(f,args.This())) False();
			return Bool(f->Close());
		}
		//*,{
		//	"type":"function",
		//	"name":"flush()",
		//	"text":"���ļ�����ˢ�µ��ļ���������ִ����д�����ʱ���������ݲ�û��������д�뵽�ļ������ʱ�����Ӧ�ó���ǿ����ֹ����ôд������ݿ��ܻᶪʧ��flush ����һ��Ҳ������ʾ���ã���Ϊ close �� dispose ��������ݱ仯ˢ�µ��ļ���ֻ����Ϊ�˱����������ֹ������������ݶ�ʧʱ������Ҫʹ�����������ע�⣬Ƶ������ flash ������ķѴ�����Դ����Ϊÿ��flash��ζ�����ݱ�ǿ��д��Ӳ�̵�����洢����",
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		""
		//	]
		//}//*
		static Handle<Value> flush(const Arguments& args){
			cs::File* f;
			if(!GetCHandle(f,args.This())) False();
			return Bool(f->Flush());
		}
		//*,{
		//	"type":"function",
		//	"name":"read(memory,[readLen],[memoryPos])",
		//	"text":"���ļ��Ķ��������ݶ���һ�� CMemory ����, ����ڴ������ܱ������ڴ����洢��ȡ�����ݡ�",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"memory",
		//			"text":"���ڴ洢���ݵ��ڴ���󣬴��ڴ���ܸ�����Ҫ�����󣬴�ʱ�ᱻǿ��ת��Ϊʹ��ϵͳ�ڴ档"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[readLen]",
		//			"text":"Ҫ��ȡ���ֽ���, ��������ô˲��������ߴ˲����㹻��, ���������Ŷ�ȡ��ǰλ�õ��ļ���β��ȫ������, ����������ݳ��ȳ��� 16MB �ֽ�, �������� false. Ҳ����˵���Ҫ��ȡ����16MB�ֽڵ�����, �������ô˲���. ��������˲���̫��, �����ڴ治��, ����Ҳ����ʧ��, һ�㲻Ҫһ�ζ�ȡ̫������, ��ȡ���ļ�, ���Էֶ�ζ�ȡ��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[memoryPos]",
		//			"text":"���ݴ洢�� CMemory �������ʼλ�á�CMemory ���ܻ������������ݣ�Ϊ�˲�Ӱ���Ѿ��洢�����ݣ�����֧�ְ�����д�뵽 CMemory �����ָ��λ�á�"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�ʱ���ض�ȡ���ֽ���, ��ʹ��ֵΪ 0������˵��û�����ݿɶ������Ǻ���������Ϊ����ʧ�ܣ�ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		"����Ҫ�����ڴ����������ݵ�ʱ��������ݴ���64K���ڴ����ᱻ�Զ�ת��Ϊʹ��ϵͳ�ڴ档"
		//	]
		//}//*
		static Handle<Value> read(const Arguments& args){
			HandleScope store;
			while(1){
				if(args.Length()<1||!IsTemplate(args[0],TEMPLATE_ID_MEMORY)) break;
				cs::Memory<char>* m;
				if(!GetCHandle<cs::Memory<char> >(m,args[0]->ToObject())) break;
				cs::File* f;
				if(!GetCHandle(f,args.This())) break;
				if(f->IsNull()) break;
				uint readLen,memPos;
				readLen = GET_ARGS_INT(1,0x1000000);
				uint64 pos = f->GetPointer();
				uint64 len = f->GetLength();
				if(len-pos<readLen){
					readLen = (uint)(len - pos);
				}
				memPos = GET_ARGS_INT(2,0);

				uint requireLen = memPos + readLen;
				if(requireLen>m->Length()){
					if(!m->GetUseSysMemory()&&requireLen>0x10000){
						m->Free();
						m->SetUseSysMemory(1);
					}
					if(!m->SetLength(requireLen)) break;//L"�ڴ����ʧ��"
				}
				len = f->Read(m->Handle()+memPos,readLen);
				return store.Close(Uint32::New((int32_t)len));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"write(memory,[writeLen],[memoryPos])",
		//	"text":"�Ѷ���������д���ļ���",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"memory",
		//			"text":"���ڴ洢���ݵ��ڴ����"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[writeLen]",
		//			"text":"Ҫд��ĳ��ȣ��˳�����������ڴ�Ŀ��ó��ȣ��ᱻ���̣�Ĭ���Ǵ���ʼ����β��ȫ�����ݡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[memoryPos]",
		//			"text":"Ҫд��������CMemory��λ�ã�Ĭ���� 0 ��"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�ʱ����д����ֽ���, ��ʹ��ֵΪ 0������˵��û�����ݱ�д�룬���Ǻ���������Ϊ����ʧ�ܣ�ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		""
		//	]
		//}//*
		static Handle<Value> write(const Arguments& args){
			HandleScope store;
			while(1){
				if(args.Length()<1||!IsTemplate(args[0],TEMPLATE_ID_MEMORY)) break;
				cs::Memory<char>* m;
				if(!GetCHandle<cs::Memory<char> >(m,args[0]->ToObject())) break;
				cs::File* f;
				if(!GetCHandle(f,args.This())) break;
				if(f->IsNull()) break;

				uint writeLen,memPos;
				writeLen = GET_ARGS_INT(1,-1);
				memPos = GET_ARGS_INT(2,0);
				if(memPos>m->Length()) memPos = m->Length();
				if(writeLen>m->Length()-memPos) writeLen = m->Length() - memPos;

				int len = f->Write(m->Handle()+memPos,writeLen);
				return store.Close(Uint32::New(len));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"readString([code])",
		//	"text":"�ӵ�ǰλ�ö�ȡ���ļ���β, ��������Ϊ�ִ�����, ����ָ������, ���ȱʡ�������� undefined�������Ƿ��� utf-8���룬������ǰ� ANSI ���봦��",
		//	"param":[
		//		{
		//			"type":"string|integer",
		//			"name":"[code]",
		//			"text":"�ļ��ı���, ���������ֻ����ִ�, ���� ��utf-8��, ȱʡֵ�� 3, �������ϵͳ��ǰ����ҳ�� ANSI ���롣���� 0 �� 1200 ����ʾ unicode ���롣"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�ʱ���ض�ȡ���ִ���ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		"���ñ��룬��utf-8���� 65001�����ֱ��ִ�Ҫ��һЩ��",
		//		"��������һ�δӿ�ͷ��ȡ�����ļ�������ļ����ȴ��� 1Mb��0x100000�ֽڣ����������׳��쳣��"
		//	]
		//}//*
		static Handle<Value> readString(const Arguments& args){
			HandleScope store;

			while(1){
				cs::File* f;
				if(!GetCHandle(f,args.This())) break;
				if(f->IsNull()) break;

				int readLen = (uint)f->GetLength();
				if((uint)readLen>0x100000){
					cs::String err;
					err.Format(L"�ļ�̫�� length = %d bytes",readLen);
					ThrowException(Exception::Error(String::New((uint16_t*)err.Handle())));
					break;
				}
				cs::Memory<char> m;
				m.SetUseSysMemory(true);
				f->SetPointer(0);
				readLen = f->Read<char>(&m);
				if(readLen<=0) return store.Close(String::New(""));

				DWORD cp = -1;
				if(args.Length()>0&&!args[0]->IsUndefined()){
					if(args[0]->IsInt32())
						cp = args[0]->Int32Value();
					else{
						cs::String code;
						GetString(args[0],code);
						cp = cs::GetCodePage(code);
					}
				}
				if(cp==-1){
					if(cs::MbsIsUtf8(m.Handle(),readLen)) cp = 65001;
					else cp = 3;
				}
				cs::String str;
				str.FromMultiByte((LPCSTR)m.Handle(),readLen,cp);
				return store.Close(String::New((uint16_t*)str.Handle()));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"writeString(str,[code])",
		//	"text":"�ӵ�ǰλ�ö�ȡ���ļ���β, ��������Ϊ�ִ�����, ����ָ������, Ĭ����ANSI���롣",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"Ҫд����ִ���"
		//		},
		//		{
		//			"type":"string|integer",
		//			"name":"[code]",
		//			"text":"�ļ��ı���, ���������ֻ����ִ�, ���� ��utf-8��, ȱʡֵ�� 3, �������ϵͳ��ǰ����ҳ�� ANSI ���롣���� 0 �� 1200 ����ʾ unicode ���롣"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�ʱ����д��ĳ��ȣ�ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		"���ñ��룬��utf-8���� 65001�����ֱ��ִ�Ҫ��һЩ��"
		//	]
		//}//*
		static Handle<Value> writeString(const Arguments& args){
			HandleScope store;
			while(1){
				int len = 0;
				if(args.Length()<1) break;

				cs::File* f;
				if(!GetCHandle(f,args.This())) break;
				if(f->IsNull()) break;

				cs::String str;
				GetString(args[0],str);

				DWORD cp = 3;
				if(args.Length()>1){
					if(args[1]->IsInt32())
						cp = args[1]->Int32Value();
					else{
						cs::String code;
						GetString(args[1],code);
						cp = cs::GetCodePage(code);
					}
				}

				cs::Memory<char> buf;
				uint wlen = str.ToMultiByte(&buf,cp);
				wlen = f->Write(buf.Handle(),wlen);
				return store.Close(Uint32::New(wlen));
			}
			return Undefined();
		}
	public:
		//*,{
		//	"type":"property",
		//	"name":"length",
		//	"objtype":"integer",
		//	"text":"���ػ������ļ��ĳ���. ��ֵû������, ������ƽ̨�ļ�֧�ֵ����ֵ. ������õĳ���С���ļ�ԭ�г���, ��������ݻᱻ���, ����Ϊ 0 , �������ȫ�����ݡ�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"pointer",
		//	"objtype":"integer",
		//	"text":"���ػ������ļ���ָ��λ��. ��ֵû������, ������ƽ̨�ļ�֧�ֵ����ֵ. �ļ�ָ����ָ�ļ���д������λ��, ��ֵ���Դ����ļ���С. ��Ϊ -1 ��ʾ����ָ�뵽�ļ�ĩβ"
		//}//*
		static Handle<Value> get(cs::String& name,cs::File* cobj,Local<Object>& self){
			if(name==L"length"){
				if(cobj->IsNull()) return Int32::New(-1);
				return Number::New((double)cobj->GetLength());
			}else if(name==L"pointer"){
				if(cobj->IsNull()) return Int32::New(-1);
				return Number::New((double)cobj->GetPointer());
			}
			return Undefined();
		}
		static void set(cs::String& name,cs::File* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"length"){
				uint64 len = (uint64)value->NumberValue();
				if(len>=0x8000000000000000) return;
				cobj->SetLength(len);
			}else if(name==L"pointer"){
				uint64 poi = (uint64)value->NumberValue();
				if(poi>=0x8000000000000000)
					cobj->SetPointer(0,FILE_END);
				else
					cobj->SetPointer(poi);
			}
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			SET_CLA_ACCESSOR(length);
			SET_CLA_ACCESSOR(pointer);
			SET_CLA_FUNC(create);
			SET_CLA_FUNC(close);
			SET_CLA_FUNC(read);
			SET_CLA_FUNC(readString);
			SET_CLA_FUNC(flush);
			SET_CLA_FUNC(write);
			SET_CLA_FUNC(writeString);

			SET_CLA_CONST(READONLY,FILE_ATTRIBUTE);
			SET_CLA_CONST(HIDDEN,FILE_ATTRIBUTE);
			SET_CLA_CONST(SYSTEM,FILE_ATTRIBUTE);
			SET_CLA_CONST(NORMAL,FILE_ATTRIBUTE);
			SET_CLA_CONST(TEMPORARY,FILE_ATTRIBUTE);

			SET_CLA_CONST2(SHARE_READ,FILE_SHARE_READ);
			SET_CLA_CONST2(SHARE_WRITE,FILE_SHARE_WRITE);
			SET_CLA_CONST2(SHARE_DELETE,FILE_SHARE_DELETE);
		}
	};
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\file.jsuser"}//*

	//*,{
	//	"type":"const",
	//	"name":"FileSystem",
	//	"text":"�˳��������ṩ�ļ�ϵͳ����ز�������ճ�������ơ�ɾ���ȡ�",
	//	"member":[//*
	class JSFileSystem{
		//*{
		//	"type":"function",
		//	"name":"isFolder(filename)",
		//	"text":"�ж�ָ����·���Ƿ���һ���ļ��С�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"filename",
		//			"text":"Ҫ����·��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�����һ���ļ��з��� true�����򷵻� false��"
		//	}
		//}//*
		static Handle<Value> isFolder(const Arguments& args){
			HandleScope store;
			if(args.Length()<1) return False();
			cs::String file;
			GetString(args[0],file);
			bool r = 2==cs::GetFileType(file);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"isExist(path)",
		//	"text":"�ж�ָ�����ļ����ļ����Ƿ���ڡ�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"Ҫ�����ִ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"������ڷ��� true�����򷵻� false��"
		//	}
		//}//*
		static Handle<Value> isExist(const Arguments& args){
			HandleScope store;
			if(args.Length()<1) return False();
			cs::String file;
			GetString(args[0],file);
			bool r = 0!=cs::GetFileType(file);
			return Bool(r);
		}
		//*,{
		//	"type":"function",
		//	"name":"isFile(filename)",
		//	"text":"�ж�ָ�����ִ��Ƿ���һ���ļ���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"filename",
		//			"text":"Ҫ�����ִ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�����һ���ļ����� true�����򷵻� false��"
		//	}
		//}//*
		static Handle<Value> isFile(const Arguments& args){
			HandleScope store;
			if(args.Length()<1) return False();
			cs::String file;
			GetString(args[0],file);
			bool r = 1==cs::GetFileType(file);
			return Bool(r);
		}
		typedef struct _FSINF : public cs::_struct{
			Local<Object> recv;
			Local<Function> fileProc,folderProc;
			Local<Object> inf;
		}FSINF;
		static void __stdcall _onFileProgress(uint64 copyed,uint64 total,bool& b,void* ud){
			HandleScope store;
			FSINF* fi = (FSINF*)ud;
			Handle<Value> args[1];
			Handle<Object> obj = Object::New();
			args[0] = obj;
			obj->Set(String::New("offset"),Number::New((double)copyed));
			obj->Set(String::New("length"),Number::New((double)copyed));
			obj->Set(String::New("cancel"),False());
			CallFunc(fi->recv,fi->fileProc,1,args);
			b = obj->Get(String::New("cancel"))->BooleanValue();
		}
		static void __stdcall _onDirProgress(cs::DirData* dd,void* ud){
			HandleScope store;
			FSINF* fi = (FSINF*)ud;
			fi->inf = Object::New();
			Handle<Value> args[1];
			fi->inf->Set(String::New("src"),String::New((uint16_t*)dd->SourceDir));
			if(dd->DestDir)
				fi->inf->Set(String::New("dst"),String::New((uint16_t*)dd->DestDir));
			bool isFolder = 0!=(dd->FindData->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY);
			fi->inf->Set(String::New("isFolder"),Boolean::New(isFolder));
			if(!isFolder){
				fi->inf->Set(String::New("file"),String::New((uint16_t*)dd->FindData->cFileName));
			}
			fi->inf->Set(String::New("stop"),Boolean::New(dd->Stop));
			fi->inf->Set(String::New("skip"),Boolean::New(dd->Skip));
			fi->inf->Set(String::New("error"),Int32::New(dd->Error));
			fi->inf->Set(String::New("preOrAfter"),Boolean::New(!dd->PreOrAfter));
			args[0] = fi->inf;
			CallFunc(fi->recv,fi->folderProc,1,args);
			dd->Skip = fi->inf->Get(String::New("skip"))->BooleanValue();
			dd->Stop = fi->inf->Get(String::New("stop"))->BooleanValue();
		}
		static void _bfs(cs::DSParam& fs,FSINF& fi,const Arguments& args){
			HandleScope store;
			fi.recv = args.This();
			fs.UserData = &fi;
			if(args.Length()>2&&args[2]->IsFunction()){
				fi.folderProc = Local<Function>::Cast(args[2]);
				fs.OnProgress.BindStd(&JSFileSystem::_onDirProgress);
			}
			if(args.Length()>3&&args[3]->IsFunction()){
				fi.fileProc = Local<Function>::Cast(args[3]);
				fs.OnFileProgress.BindStd(&JSFileSystem::_onFileProgress);
			}
		}
		//*,{
		//	"type":"function",
		//	"name":"delete(src,[onFileProgress(param)])",
		//	"text":"ɾ��һ���ļ����ļ��С�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"Ҫ�Ƴ����ļ����У�·����"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onFileProgress(param)]",
		//			"text":"����һ���ص����������ڻ�ȡ�����ò�������ʱ��һЩ��Ϣ��ѡ�����˵���ο� onFileProgress ������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ļ����������г����κ�ϵͳ���󣬺������᷵�� false����������û�ͨ���ص�������ֹ�˲�����������Ȼ���� true���������� false ʱ��ɾ�����������Ѿ�ִ����һ���֡�����һЩ�������������ʧ�ܣ��ļ�Ŀ¼�����ڡ��ļ�û��ɾ��Ȩ�޵ȡ�"
		//	}
		//}//*
		static Handle<Value> _delete(const Arguments& args){
			HandleScope store;
			if(args.Length()<1) return False();
			cs::DSParam dsp;
			GetString(args[0],dsp.Source);
			FSINF fi;
			if(args.Length()>1&&args[1]->IsFunction()){
				fi.recv = args.This();
				fi.folderProc = Local<Function>::Cast(args[1]);
				dsp.UserData = &fi;
				dsp.OnProgress.BindStd(&JSFileSystem::_onDirProgress);
			}
			return Bool(cs::DirectorySystem::Delete(&dsp));
		}
		//*,{
		//	"type":"function",
		//	"name":"copy(src,dst,[onFileProgress(param)],[onDataProgress(param)])",
		//	"text":"����һ���ļ����ļ��У�Դ�ļ���Ŀ���ļ�������ͬ���˲����������������������ϴθ��ƵĽ��ȣ������ϴεĽ������������ݿ��������ǣ�ĳЩ��Ϊ������������Ŀ���ļ��е����ݱ����������д����ô�������������Щ�Ķ����Ӷ����Դ�ļ��к�Ŀ���ļ��е����ݲ�ͬ��������ܿ϶�Ŀ���ļ����Ƿ񱻸Ķ��������ɾ��Ŀ��ͬ���ļ����У���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"Դ�ļ����У�·����"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"Ŀ���ļ����У�·����"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onFileProgress(param)]",
		//			"text":"����һ���ص����������ڻ�ȡ�����ò�������ʱ��һЩ��Ϣ��ѡ�����˵���ο� onFileProgress ������"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onDataProgress(offset,total)]",
		//			"text":"����һ���ص����������ڻ�ȡһ���ļ��Ŀ����ľ�����ȡ���Ϊ�˲�������˵���ο� onDataProgress ������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ļ����������г����κ�ϵͳ���󣬺������᷵�� false����������û�ͨ���ص�������ֹ�˲�����������Ȼ���� true���������� false ʱ�����Ʋ��������Ѿ�ִ����һ���֡�����һЩ�������������ʧ�ܣ�Դ�ļ�Ŀ¼�����ڡ�Դ�ļ���Ŀ���ļ���ͬ��Դ�ļ�û�ж�ȡȨ�ޡ�Ŀ���ļ�û��д��Ȩ�޵ȡ�"
		//	},
		//	"remark":[
		//		"ע�⣺��������Ḳ��Ŀ��Ŀ¼��ͬ���ļ�������û���κξ��档������ȷ��Ŀ��Ŀ¼û��ͬ���ļ���ʱ�򣬲�Ҫʹ����������������ڸ��ƴ�����Ƭ��ͬһ���ļ��е�ʱ�򣬲���֤û��ͬ������Ƭ�ļ������ڷǴ��ļ����Ǵ����ļ���ʹ�� sh ��ͷ�ĺ�����shCopy��shMove��shDelete����"
		//	]
		//}//*
		static Handle<Value> copy(const Arguments& args){
			HandleScope store;
			if(args.Length()<2) return False();
			cs::DSParam dsp;
			GetString(args[0],dsp.Source);
			GetString(args[1],dsp.Destinate);
			FSINF fi;
			_bfs(dsp,fi,args); 
			return Bool(cs::DirectorySystem::Copy(&dsp));
		}
		//*,{
		//	"type":"function",
		//	"name":"move(src,dst,[onFileProgress(param)],[onDataProgress(param)])",
		//	"text":"�ƶ�һ���ļ����ļ��У�Դ�ļ���Ŀ���ļ�������ͬ���˲����������������������ϴθ��ƵĽ��ȣ������ϴεĽ������������ݿ��������ǣ�ĳЩ��Ϊ������������Ŀ���ļ��е����ݱ����������д����ô�������������Щ�Ķ����Ӷ����Դ�ļ��к�Ŀ���ļ��е����ݲ�ͬ��������ܿ϶�Ŀ���ļ����Ƿ񱻸Ķ��������ɾ��Ŀ��ͬ���ļ����У���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"Դ�ļ����У�·����"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"Ŀ���ļ����У�·����"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onFileProgress(param)]",
		//			"text":"����һ���ص����������ڻ�ȡ�����ò�������ʱ��һЩ��Ϣ��ѡ�����˵���ο� onFileProgress ������"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onDataProgress(offset,total)]",
		//			"text":"����һ���ص����������ڻ�ȡһ���ļ��Ŀ����ľ�����ȡ���Ϊ�˲�������˵���ο� onDataProgress ������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ļ����������г����κ�ϵͳ���󣬺������᷵�� false����������û�ͨ���ص�������ֹ�˲�����������Ȼ���� true���������� false ʱ�����Ʋ��������Ѿ�ִ����һ���֡�����һЩ�������������ʧ�ܣ�Դ�ļ�Ŀ¼�����ڡ�Դ�ļ���Ŀ���ļ���ͬ��Դ�ļ�û�ж�ȡȨ�ޡ�Ŀ���ļ�û��д��Ȩ�޵ȡ�"
		//	},
		//	"remark":[
		//		"�ƶ��������������������ļ����ļ��С�",
		//		"�ƶ�������ͬ�̷������追�����ݣ��������Ǹ���������",
		//		"ע�⣺��������Ḳ��Ŀ��Ŀ¼��ͬ���ļ�������û���κξ��档������ȷ��Ŀ��Ŀ¼û��ͬ���ļ���ʱ�򣬲�Ҫʹ������������������ƶ�������Ƭ��ͬһ���ļ��е�ʱ�򣬲���֤û��ͬ������Ƭ�ļ������ڷǴ��ļ����Ǵ����ļ���ʹ�� sh ��ͷ�ĺ�����shCopy��shMove��shDelete����"
		//	]
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onFileProgress(param)",
		//	"text":"����һ���ص������������� FileSystem �ĳ�Ա���������� copy��move��remove 3�������ĵ�����������ÿ������Ҫ����һ���ļ����ļ��е�ʱ������������ص����Σ���һ���ǲ�����Ҫ���У���ʱ����������������ֹѡ��ڶ����ǲ����Ѿ���ɣ���ʱ��������ѡ����Ч����ǰ�ļ��Ĳ����Ѿ���ɣ���������ֹѡ�����ֹ���������������",
		//	"param":[
		//		{
		//			"type":"FileSystemParam",
		//			"name":"param",
		//			"text":"���ڻ�ȡ�����õ�ǰ��������Ϣ��ѡ�����˵���μ� FileSystemParam��"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����������践��ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onDataProgress(param)",
		//	"text":"����һ���ص������������� FileSystem �ĳ�Ա���������� copy��move �����ĵ��ĸ������������ļ����ݿ����Ľ��ȡ��ص�����ÿ���� 1 MB��1048576 bytes�� �ֽں���һ�Σ��ļ��������ʱ����һ�Ρ�",
		//	"param":[
		//		{
		//			"type":"FileDataParam",
		//			"name":"param",
		//			"text":"��������� 3 �����ԣ��ֱ��� offset,total,cancel���ֱ��ʾ�Ѿ����������ݡ������ݡ��Ƿ�ȡ������ļ��Ŀ�������������˵���μ� FileDataParam��"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����������践��ֵ��"
		//	}
		//}//*

		//*,{
		//	"type":"variant",
		//	"name":"FileSystemParam",
		//	"objtype":"object",
		//	"text":"�����ͱ����� onFileProgress �ص������ﱻʹ�ã����ڻ�ȡ������Ϣ�������ÿ���ѡ�ÿ�����ļ����У��ڴ����ʱ������������������Σ���һ����Ҫִ�в���ǰ����ʱ�������� skip ����ȡ����ǰ�ļ����У��Ĳ������ڶ��κ����ǲ���ִ�к�",
		//	"property":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"��ǰ�ļ���ԴĿ¼�����ֵ��������ǰ�ļ�����ֻ��Դ�ļ������ơ�"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"��ǰ�ļ���Ŀ��Ŀ¼�����ֵ��������ǰ�ļ�����ֻ��Դ�ļ������ơ�ע�⣺������ delete ������ʱ��dst ����û�����á�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"isFolder",
		//			"text":"��ǰ���ڴ������Ŀ���Ƿ���һ���ļ��С�"
		//		},
		//		{
		//			"type":"string",
		//			"name":"file",
		//			"text":"��ǰ������ļ����ļ��е����ƣ���������Ŀ¼��"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"stop",
		//			"text":"�Ƿ���ֹ����Ĳ�����"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"skip",
		//			"text":"�Ƿ�������ǰ�ļ����У��Ĳ��������ֵֻ���ڲ���ִ��ǰ��һ�λص������ò������ã���Ϊ�ڶ��κ��е�ʱ������Ѿ���ɡ����һ���ļ��б��������������������ļ����ļ��ж��ᱻ������"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"error",
		//			"text":"�����룬���һ�����������ֵ�� 0 ��Ҫ��ȡ����������ʹ�� FileSystem �� getErrorMsg ���������Ұ������������Ϊ������"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"preOrAfter",
		//			"text":"�˴β����ϲ���ǰ�Ļص����ǲ�����ɺ�ĺ��У�true ��ʾ����ǰ��"
		//		}
		//	]
		//}//*
		//*,{
		//	"type":"variant",
		//	"name":"FileDataParam",
		//	"objtype":"object",
		//	"text":"�����ͱ����� onDataProgress �ص������ﱻʹ�ã����ڻ�ȡ�ļ������ĵ�ǰ���ȣ��Ϳ����Ƿ�ȡ���������ص�����ÿ���� 1 MB��1048576 bytes�� �ֽں���һ�Ρ�",
		//	"property":[
		//		{
		//			"type":"integer",
		//			"name":"offset",
		//			"text":"�Ѿ����������ݳ��ȣ�ע�⣺��Ϊ�����Ĵ��ڣ����ֵ�����Ǵ� 0 ��ʼ�ģ�ʵ���ϳ����ļ��Ĵ�СΪ 0 ����������������Զ������ 0��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"��Ҫ�����������ܳ��ȣ�������Դ�ļ��Ĵ�С��"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"cancel",
		//			"text":"����������ֵΪ true����ǰ�ļ��Ŀ����ᱻ��ֹ���������ļ��в����У������ļ��Ŀ������ᱻȡ����"
		//		}
		//	]
		//}//*

		//*,{
		//	"type":"function",
		//	"name":"getErrorMsg(code)",
		//	"text":"��ȡ��������ı�������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"code",
		//			"text":"onFileProgress �ص��������صĴ����룬0 ��ʾ�ɹ�������ֵ��ʾ���ִ���"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"���ش�����ı�������"
		//	}
		//}//*
		static Handle<Value> move(const Arguments& args){
			HandleScope store;
			if(args.Length()<2) return False();
			cs::DSParam dsp;
			GetString(args[1],dsp.Destinate);
			GetString(args[0],dsp.Source);
			FSINF fi;
			_bfs(dsp,fi,args);
			return Bool(cs::DirectorySystem::Move(&dsp));
		}
		//*,{
		//	"type":"function",
		//	"name":"createFolder(path,[parent])",
		//	"text":"����һ���ļ��У�����ڶ�������Ϊ true������ path ָ��·���ĸ��ļ��У�����÷����㴴���ļ��ĸ�Ŀ¼���������ļ�·����ȡ���ļ����ִ���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"Ҫ���ɵ��ļ��е�·��������ڶ�����Ϊtrue�����ɵ��Ǵ�·���ĸ��ļ��С�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[parent]",
		//			"text":"������������Ϊ true�����ɵ�һ������ָ��·���ĵĸ��ļ��С�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"��������ɹ����� true��ע�⣺����ļ����Ѿ����ڣ��������� true ��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> createFolder(const Arguments& args){
			HandleScope store;
			if(args.Length()<1) return False();
			cs::String file;
			GetString(args[0],file);
			bool create_parent = false;
			if(args.Length()>1){
				create_parent = args[1]->BooleanValue()==true;
			}
			bool r = cs::CreateFolder(file,create_parent);
			if(r) return True();
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"setAttribute(path,attribute,[option])",
		//	"text":"����һ���ļ��У�����ڶ�������Ϊ true������ path ָ��·���ĸ��ļ��У�����÷����㴴���ļ��ĸ�Ŀ¼���������ļ�·����ȡ���ļ����ִ���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"Ҫ���õ��ļ����У���·����"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"attribute",
		//			"text":"Ҫ���õ����ԣ����ֵ�������������������������û�����õ������������ļ��ᱻ��Ϊ���ֵָ�������ԡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[option]",
		//			"text":"��������ļ����ԣ�0�������ļ�����Ϊ�ڶ�������ָ����ֵ��1����ӵڶ�������ָ�������ԣ�2���Ƴ��ڶ�������ָ�������ԡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> setAttribute(const Arguments& args){
			HandleScope store;
			if(args.Length()<2) return False();
			cs::String file;
			GetString(args[0],file);
			int option = GET_ARGS_INT(2,0);
			bool r;
			if(option==0){
				r = cs::SetFileAttr(file,args[1]->Uint32Value());
			}else if(option==1){
				r = cs::SetFileAttr(file,args[1]->Uint32Value(),0);
			}else{
				r = cs::SetFileAttr(file,0,args[1]->Uint32Value());
			}
			if(r) return True();
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getAttribute(path)",
		//	"text":"��ȡ�ļ����У����ļ����ԡ�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"�ļ����У���·����"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"��������ɹ�����һ�������ļ����Ե���ֵ��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> getAttribute(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::String file;
				GetString(args[0],file);
				DWORD attr = cs::GetFileAttributes(file);
				if(attr==-1) break;
				return store.Close(Uint32::New(attr));
			}
			return Undefined();
		}
		static Handle<Value> getErrorMsg(const Arguments& args){
			HandleScope store;
			uint code = GET_ARGS_INT(0,-1);
			LPCWSTR msg = cs::FSGetErrorMsg(code);
			if(msg==0) return Undefined();
			return store.Close(String::New((uint16_t*)msg));
		}
		static int shfile(LPCWSTR dst,LPCWSTR src,UINT func,FILEOP_FLAGS flag){
			cs::String from = src;
			cs::FPToFullPath(from);
			int pos = from.Length() + 1;
			from.SetCubage(pos);
			from[pos] = 0;

			cs::String to = dst;
			cs::FPToFullPath(to);
			to.SetCubage(to.Length()+1);
			to[to.Length()+1] = 0;

			SHFILEOPSTRUCT fos;
			::ZeroMemory(&fos,sizeof(fos));
			fos.wFunc = func;
			fos.pFrom = from;
			fos.pTo = to;
			fos.fFlags = flag;
			fos.lpszProgressTitle = L"jsuser";

			return SHFileOperation(&fos);
		}
		static int shcopy(LPCWSTR dst,LPCWSTR src,bool noundo = false,bool noask = false,bool noerr = false){
			FILEOP_FLAGS flag = FOF_SIMPLEPROGRESS;
			if(!noundo) flag |= FOF_ALLOWUNDO;
			if(noask) flag |= FOF_NOCONFIRMATION;
			if(noerr) flag |= FOF_NOERRORUI;
			return shfile(dst,src,FO_COPY,flag);
		}
		static int shmove(LPCWSTR dst,LPCWSTR src,bool noundo = false,bool noask = false,bool noerr = false){
			FILEOP_FLAGS flag = FOF_SIMPLEPROGRESS;
			if(!noundo) flag |= FOF_ALLOWUNDO;
			if(noask) flag |= FOF_NOCONFIRMATION;
			if(noerr) flag |= FOF_NOERRORUI;
			return shfile(dst,src,FO_MOVE,flag);
		}
		static int shrename(LPCWSTR dst,LPCWSTR src,bool noundo = false,bool noask = false,bool noerr = false){
			FILEOP_FLAGS flag = FOF_SIMPLEPROGRESS;
			if(!noundo) flag |= FOF_ALLOWUNDO;
			if(noask) flag |= FOF_RENAMEONCOLLISION;
			if(noerr) flag |= FOF_NOERRORUI;
			return shfile(dst,src,FO_RENAME,flag);
		}
		static int shdelete(LPCWSTR path,bool noundo = false,bool noask = false,bool noerr = false){
			FILEOP_FLAGS flag = FOF_SIMPLEPROGRESS;
			if(!noundo) flag |= FOF_ALLOWUNDO;
			if(noask) flag |= FOF_NOCONFIRMATION;
			if(noerr) flag |= FOF_NOERRORUI;
			return shfile(0,path,FO_DELETE,flag);
		}
		//*,{
		//	"type":"function",
		//	"name":"shCopy(src,dst,[noAsk],[noUndo],[noError])",
		//	"text":"���� src �ļ����У����������ļ��к������ļ���dst �������֡����Ŀ��λ����ͬ���ļ���Ĭ�ϻᵯ��ȷ�϶Ի��򣬿��԰� noAskExist ��Ϊ true ǿ�Ƹ���Ŀ���ļ���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"Դ�ļ����У���·����"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"Ŀ���ļ����У��������ơ�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noAsk]",
		//			"text":"���������Ҫѯ���û�ѡ�����������縴�Ƶ�ʱ��Ŀ���ļ��Ѿ����ڣ��Ƿ񲻵���ѡ��򣬶��ǰ�Ĭ����Ϊ�Զ���������Ĭ��ֵ�� false����ѯ���û��������Ϊ true���򲻻�ѯ���û������縴��ʱĿ���ļ����Ѿ����ڣ���ǿ�Ƹ��ǡ�"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noUndo]",
		//			"text":"������Ʋ���Ĭ���ǿ����� Windows ��Դ�������ﳷ���ģ��˲�����Ϊ true���رտɳ���ѡ�"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noError]",
		//			"text":"�����ִ���ʱ���Ƿ񲻵���������ʾ�Ի���Ĭ��ֵ false�����ִ���ʱ�ᵯ��һ���Ի�����ʾ������Ϣ�������û���������ȷ�ϡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> shCopy(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::String src,dst;
				GetString(args[0],src);
				GetString(args[1],dst);
				bool noask = GET_ARGS_VALUE(2,false,Boolean);
				bool noundo = GET_ARGS_VALUE(3,false,Boolean);
				bool noerr = GET_ARGS_VALUE(4,false,Boolean);
				if(0!=shcopy(dst,src,noundo,noask,noerr)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"shMove(src,dst,[noAskExist],[noUndo],[noError])",
		//	"text":"�ƶ� src �ļ����У����������ļ��к������ļ���dst �������֡����Ŀ��λ����ͬ���ļ���Ĭ�ϻᵯ��ȷ�϶Ի��򣬿��԰� noAskExist ��Ϊ true ǿ�Ƹ���Ŀ���ļ���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"Դ�ļ����У���·����"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"����������ָ�����ļ��в����ڣ���������������ļ��е�·��������������ָ�����ļ����Ѿ����ڣ����ƶ�������ļ����ڣ���Ϊ�������ļ��У����ƺ�Դ�ļ�����ͬ��"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noAsk]",
		//			"text":"���������Ҫѯ���û�ѡ�����������縴�Ƶ�ʱ��Ŀ���ļ��Ѿ����ڣ��Ƿ񲻵���ѡ��򣬶��ǰ�Ĭ����Ϊ�Զ���������Ĭ��ֵ�� false����ѯ���û��������Ϊ true���򲻻�ѯ���û������縴��ʱĿ���ļ����Ѿ����ڣ���ǿ�Ƹ��ǡ�"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noUndo]",
		//			"text":"����ƶ�����Ĭ���ǿ����� Windows ��Դ�������ﳷ���ģ��˲�����Ϊ true���رտɳ���ѡ�"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noError]",
		//			"text":"�����ִ���ʱ���Ƿ񲻵���������ʾ�Ի���Ĭ��ֵ false�����ִ���ʱ�ᵯ��һ���Ի�����ʾ������Ϣ�������û���������ȷ�ϡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> shMove(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::String src,dst;
				GetString(args[0],src);
				GetString(args[1],dst);
				bool noask = GET_ARGS_VALUE(2,false,Boolean);
				bool noundo = GET_ARGS_VALUE(3,false,Boolean);
				bool noerr = GET_ARGS_VALUE(4,false,Boolean);
				if(0!=shmove(dst,src,noundo,noask,noerr)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"shRename(src,dst,[noAskExist],[noUndo],[noError])",
		//	"text":"�������ļ����У���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"Դ�ļ����У���·����"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"���ļ����У������ơ�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noAsk]",
		//			"text":"���������Ҫѯ���û�ѡ�������������������ļ�ʱĿ���ļ��Ѿ����ڣ��Ƿ񲻵���ѡ��򣬶��ǰ�Ĭ����Ϊ�Զ���������Ĭ��ֵ�� false����ѯ���û��������Ϊ true���򲻻�ѯ���û�������������ʱĿ���ļ����Ѿ����ڣ������Զ�ѡ��һ�����ļ�����"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noUndo]",
		//			"text":"�������������Ĭ���ǿ����� Windows ��Դ�������ﳷ���ģ��˲�����Ϊ true���رտɳ���ѡ�"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noError]",
		//			"text":"�����ִ���ʱ���Ƿ񲻵���������ʾ�Ի���Ĭ��ֵ false�����ִ���ʱ�ᵯ��һ���Ի�����ʾ������Ϣ�������û���������ȷ�ϡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> shRename(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<2) break;
				cs::String src,dst;
				GetString(args[0],src);
				GetString(args[1],dst);
				bool noask = GET_ARGS_VALUE(2,false,Boolean);
				bool noundo = GET_ARGS_VALUE(3,false,Boolean);
				bool noerr = GET_ARGS_VALUE(4,false,Boolean);
				if(0!=shrename(dst,src,noundo,noask,noerr)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"shDelete(path,[noAsk],[noUndo],[noError])",
		//	"text":"ɾ���ļ����У����������",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"�ļ����У���·����"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noAsk]",
		//			"text":"���������Ҫѯ���û�ѡ�����������縴�Ƶ�ʱ��Ŀ���ļ��Ѿ����ڣ��Ƿ񲻵���ѡ��򣬶��ǰ�Ĭ����Ϊ�Զ���������Ĭ��ֵ�� false����ѯ���û��������Ϊ true���򲻻�ѯ���û������縴��ʱĿ���ļ����Ѿ����ڣ���ǿ�Ƹ��ǡ�"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noUndo]",
		//			"text":"ɾ������Ĭ����ɾ��������վ�������ֱ��ɾ���ļ����У������ô˲���Ϊ true��"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noError]",
		//			"text":"�����ִ���ʱ���Ƿ񲻵���������ʾ�Ի���Ĭ��ֵ false�����ִ���ʱ�ᵯ��һ���Ի�����ʾ������Ϣ�������û���������ȷ�ϡ�"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> shDelete(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				cs::String path;
				GetString(args[0],path);
				bool noask = GET_ARGS_VALUE(1,false,Boolean);
				bool noundo = GET_ARGS_VALUE(2,false,Boolean);
				bool noerr = GET_ARGS_VALUE(3,false,Boolean);
				if(0!=shdelete(path,noundo,noask,noerr)) break;
				return True();
			}
			return Undefined();
		}
	public:
		static void Load(Handle<Object>& glb){
			HandleScope store;
			Local<Object> obj = Object::New();
			glb->Set(String::New("FileSystem"),obj,ReadOnly);

			SET_OBJ_FUNC(obj,shCopy,JSFileSystem::shCopy);
			SET_OBJ_FUNC(obj,shRename,JSFileSystem::shRename);
			SET_OBJ_FUNC(obj,shMove,JSFileSystem::shMove);
			SET_OBJ_FUNC(obj,shDelete,JSFileSystem::shDelete);
			SET_OBJ_FUNC(obj,isFile,JSFileSystem::isFile);
			SET_OBJ_FUNC(obj,isFolder,JSFileSystem::isFolder);
			SET_OBJ_FUNC(obj,isExist,JSFileSystem::isExist);
			SET_OBJ_FUNC(obj,delete,JSFileSystem::_delete);
			SET_OBJ_FUNC(obj,copy,JSFileSystem::copy);
			SET_OBJ_FUNC(obj,move,JSFileSystem::move);
			SET_OBJ_FUNC(obj,createFolder,JSFileSystem::createFolder);
			SET_OBJ_FUNC(obj,setAttribute,JSFileSystem::setAttribute);
			SET_OBJ_FUNC(obj,getAttribute,JSFileSystem::getAttribute);
			SET_OBJ_FUNC(obj,getErrorMsg,JSFileSystem::getErrorMsg);
		}
	};
	//*]}//*

	//*,{
	//	"type":"class",
	//	"name":"CFileList",
	//	"text":"�г�һ���ļ��к��������ļ��е��ļ���",
	//	"member":[//*
	class JSFileSearch : public JsCObject<cs::FileTree,JSFileSearch,TEMPLATE_ID_FILELIST>{
		typedef struct _FTINF : public cs::_struct{
			Handle<Object> self;
			Handle<Function> onList;
			Handle<Object> data;
		}FTINF;
		//*{
		//	"type":"function",
		//	"name":"onList(data)",
		//	"text":"����һ���ص����������������������ȡ��Ϣ��",
		//	"param":[
		//		{
		//			"type":"FileSearchData",
		//			"name":"param",
		//			"text":"�������������ļ����У�����Ϣ��������������ѡ�����ϸ�ڲο� FileSearchData ��Ŀ��"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����������践��ֵ��"
		//	}
		//}//*
		//*,{
		//	"type":"variant",
		//	"name":"FileSearchData",
		//	"objtype":"object",
		//	"text":"onList �����Ĳ����������ļ���Ϣ������ѡ�",
		//	"property":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"�ļ�����������·����"
		//		},
		//		{
		//			"type":"string",
		//			"name":"sub",
		//			"text":"�ļ�����·�������·������ FileSearch ����� path ���ԣ�����ȫ·��������ļ����ڵ�ǰ������·���£����ֵ�ǿմ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"flag",
		//			"text":"�����ǰ����������ļ�, ��ֵΪ 0 ,�������������ļ����ҵ�һ�κ���(�ļ��п��ܱ���������,��������ѡ��)��ֵΪ 1, �ڶ��κ���, ��ֵΪ 2��"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"stop",
		//			"text":"�Ƿ���ֹ������"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"skip",
		//			"text":"�Ƿ�������ǰ�ֻ�е�ǰ����һ���ļ������ǵ�һ�λص���flag = 1������������ô�ֵΪ true �������塣"
		//		}
		//	]
		//}//*
		static void __stdcall _onList(cs::ListData* ld){
			FTINF* fi = (FTINF*)ld->ExtraData;
			if(fi->onList->IsUndefined()) return;
			Handle<Value> args[1];
			fi->data->Set(String::New("skip"),Boolean::New(ld->Skip));
			fi->data->Set(String::New("stop"),Boolean::New(ld->Stop));
			fi->data->Set(String::New("flag"),Int32::New(ld->Flag));//0 �ļ�, 1 pre, 2 after.
			fi->data->Set(String::New("sub"),String::New((uint16_t*)ld->Relative));
			fi->data->Set(String::New("name"),String::New((uint16_t*)ld->FindData->cFileName));
			args[0] = fi->data;
			CallFunc(fi->self,fi->onList,1,args);
			ld->Skip = fi->data->Get(String::New("skip"))->BooleanValue();
			ld->Stop = fi->data->Get(String::New("stop"))->BooleanValue();
		}
		//*,{
		//	"type":"function",
		//	"name":"search(path)",
		//	"text":"����ָ��·�����ļ���, �ļ����ڵ��ļ�(���ļ���)�ڻص�����onList�л�ȡ, ÿ������һ�������������ļ�(��), ����������һ�Ρ����ָ����·����path����һ���ļ������������Ȼ��ɹ����ص�������������ļ���",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"Ҫ������·������.����ʾ��ǰ·����js�ű����ڵ�·��������..����ʾ��ǰ·������һ��Ŀ¼��"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����������践��ֵ��"
		//	}
		//}//*

		//*,{
		//	"type":"property",
		//	"name":"child",
		//	"objtype":"boolean",
		//	"text":"���� search �����Ƿ��������ļ��У���ֵȱʡ�� false��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"before",
		//	"objtype":"boolean",
		//	"text":"�����Ƿ�ÿ������һ���ļ����Ƿ��Ȼص�һ�Σ���ʱ onList �Ĳ����� flag ������Ϊ 1���ļ�����ֻ�ص�һ�Σ��� flag ��Ϊ 0 ��ȱʡֵ true��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"after",
		//	"objtype":"boolean",
		//	"text":"�����Ƿ�ÿ������һ���ļ������������������Ļص��󣬻ص�һ�Σ���ʱ onList �Ĳ����� flag ��������Ϊ 2��ȱʡֵ true��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"path",
		//	"objtype":"string",
		//	"text":"��� search �������óɹ���FileSearch ������ path ����Ϊ search �� path ������ֵ��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"filter",
		//	"objtype":"array",
		//	"text":"�����������˵�һ�����飬����� FileSearch �� filter �����������һ��Ԫ�أ���ôֻ�������׺���ļ��ᱻ�������ļ��в���Ӱ�죩��������� ��jpg������ôֻ�� jpg �ļ��ᱻ��������"
		//}//*

		static Handle<Value> search(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				cs::FileTree* ft;
				if(!GetCHandle(ft,self)) break;
				cs::String path;
				GetString(args[0],path);
				FTINF fi;
				fi.self = self;
				fi.onList = Handle<Function>::Cast(self->Get(String::New("onList")));
				fi.data = Object::New();
				cs::StringMemList* sml = ft->GetFilter();
				sml->Clear();
				Local<Array> filter = Local<Array>::Cast(self->Get(String::New("filter")));
				if(!filter.IsEmpty()){
					uint len = filter->Length();
					cs::String ext;
					for(uint i=0;i<len;i++){
						GetString(filter->Get(i),ext);
						sml->Add(ext);
					}
				}
				ft->SetSearchSub(self->Get(String::New("child"))->BooleanValue());
				ft->SetPreCall(self->Get(String::New("before"))->BooleanValue());
				ft->SetAfterCall(self->Get(String::New("after"))->BooleanValue());
				ft->OnList.BindStd(&_onList);
				SET_PROP(self,path,args[0]);
				if(!ft->Search(path,&fi)) break;
				return True();
			}
			return Undefined();
		}
	public:
		static void inline on_create(Handle<Object>& self,cs::FileTree* cObj){
			SET_PROP(self,child,False());
			SET_PROP(self,before,True());
			SET_PROP(self,after,False());
			SET_PROP_RO(self,filter,Array::New());
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();
			SET_CLA_FUNC(search);
		}

	};
	//*,{
	//	"type":"function",
	//	"name":"addFilter(filter)",
	//	"text":"����ļ����͹���, һ�������һ���ļ�����, ֻ��ָ�����͵��ļ��ᱻ����. ʹ�����������Ϊ�˷��㡣",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filter",
	//			"text":"һ���ɶ��š�,���ָ����ļ���׺�ִ�, ��Ҫ�����Ǻź͵�, ������ȷ�ĸ�ʽ: ��jpg,gif��, ��Ҫд��: ��*.jpg,*.gif,.bmp����"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//}//*

	//*,{
	//	"type":"function",
	//	"name":"delFilter(filter)",
	//	"text":"�Ƴ��ļ���������, һ�������һ���ļ�����, ֻ��ָ�����͵��ļ��ᱻ����. ʹ�����������Ϊ�˷��㡣",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filter",
	//			"text":"һ���ɶ��š�,���ָ����ļ���׺�ִ�, ��Ҫ�����Ǻź͵�, ������ȷ�ĸ�ʽ: ��jpg,gif��, ��Ҫд��: ��*.jpg,*.gif,.bmp����"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//}//*

	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\filelist.jsuser"}//*

	//*,{
	//	"type":"const",
	//	"name":"FilePath",
	//	"text":"�˳��������ṩ�ļ�·���Ĳ��������к���֧�֡�/����\\���ָ���·���ִ���",
	//	"member":[
	//	{
	//		"type":"function",
	//		"name":"getExt(path)",
	//		"text":"����·���ĺ�׺(�ļ�����), ����\".\", ���û�к�׺���ؿմ���",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"�ļ�·����"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"����һ����������.���ĺ�׺�������·��������һ����׺�����ؿմ���"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"getTitle(path)",
	//		"text":"�����ļ�����, ����·���ͺ�׺��",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"�ļ�·����"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"���ز�������׺���ļ��е��ļ����ִ���"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"getParent(path)",
	//		"text":"���ص�ǰĿ¼�ĸ�Ŀ¼, ���߿մ�(���û�и�Ŀ¼)��",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"�ļ�·����"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"����һ���ִ���"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"toSlash(path)",
	//		"text":"��·����ķ�б�ܻ���б��, ����ɾ�����������б��, ����ĩβ��б�ܡ����� Linux ����·����",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"�ļ�·����"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"����һ���ִ���"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"toBackslash(path)",
	//		"text":"��·�����б�ܻ��ɷ�б��, ����ɾ�����������б��, ����ĩβ��б�ܡ����� Windows ����·����",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"�ļ�·����"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"����һ���ִ���"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"linkPath(path,...)",
	//		"text":"�÷�б�����Ӳ�����Ϊһ��·��, ������������Զ�����ͷβ�Ѿ���б�ܻ�б�ܵ������",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"��Ҫ���ӵ�һϵ��·���ִ���"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"����һ���ִ���"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"linkExt(path,ext)",
	//		"text":"·�������� \".\" ����, �������ֻ����ext��ǰ���Ƿ���һ�� \".\", ����ж����, ����������",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"�ļ�·����"
	//			}
	//			,{
	//				"type":"string",
	//				"name":"ext",
	//				"text":"��׺�������԰�����.����Ҳ���Բ�������.����"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"����һ���ִ���"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"getFullPath(path)",
	//		"text":"��ȡ path ������·����",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"�ļ�·����"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"����һ���ִ���"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"isSame(path1,path2)",
	//		"text":"windowsϵͳ·�������ִ�Сд, ����������б��, ��������������·���Ƿ���ͬһ����",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path1",
	//				"text":"�ļ�·����"
	//			}
	//			,{
	//				"type":"string",
	//				"name":"path2",
	//				"text":"�ļ�·����"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"��������·���Ƿ���ͬ��"
	//		}
	//	}
	//	]
	//}//*

//*]}//* file Close

	Handle<Value> FilePath_getFullPath(const Arguments& args){
		HandleScope store;
		while(true){
			if(args.Length()<1) break;
			cs::String path;
			GetString(args[0],path);
			if(!cs::FPToFullPath(path)) break;
			return String::New((uint16_t*)path.Handle());
		}
		return Undefined();
	}
	void loadFilePath(Handle<Object>& glb){
		HandleScope store;
		Handle<Object> filepath = glb->Get(String::New("FilePath"))->ToObject();//GetObjProperty<Object>(glb,L"FilePath");
		SET_OBJ_FUNC(filepath,getFullPath,FilePath_getFullPath);
	}
	void LoadFile(Handle<Object>& glb){
		JSFile::Load(glb,L"CFile");
		JSFileSystem::Load(glb);
		JSFileSearch::Load(glb,L"CFileList");
		LoadJsRes(IDR_JS_FILE,L"file.js");
		loadFilePath(glb);
	}
}