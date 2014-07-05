#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"file",
	//	"text":"文件相关功能。",
	//	"member":[//*
	//*{
	//	"type":"class",
	//	"name":"CFile",
	//	"text":"对文件内容进行读写操作。",
	//	"member":[//*
	class JSFile : public JsHandleObject<cs::File,HANDLE,JSFile,TEMPLATE_ID_FILE>{
		//*{
		//	"type":"function",
		//	"name":"create(name,[exist],[readonly],[share],[attribute])",
		//	"text":"打开一个文件, 如果文件不存在, 尝试生成它. 如果只打开一个已经存在的文件(文件不存在返回失败), 使用 open 函数，这个函数可以重复调用, 来打开一个新文件, 原来的文件会自动关闭, 无需显示调用 close 函数。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"要打开的文件名。如果没有指定文件完整的路径，文件在当前目录下。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[exist]",
		//			"text":"是否只打开一个已经存在的文件，当这个参数为 true 时，如果指定的文件名不存在，函数失败。缺省值：false。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[readonly]",
		//			"text":"是否以只读模式打开文件，一个只读模式打开的文件，只能读取，不能对文件进行改写，包括写入内容，改变长度，设置属性。缺省值是 false。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[share]",
		//			"text":"共享模式，有 3 个值，CFile.SHARE_READ，CFile.SHARE_WRITE，CFile.SHARE_DELETE，表示其它同时打开这个文件允许的打开模式。比如如果以 SHARE_READ 打开了文件，同时其它文件函数也打开这个文件的话，只能以只读模式，如果以读写模式打开文件会失败，也不能删除。如果以 SHARE_WRITE|SHARE_READ|SHARE_DELETE 模式打开文件，则这个文件可以被其它文件函数随意读取写入和删除。缺省模式是读写但是不能删除，即 SHARE_WRITE|SHARE_READ。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[attribute]",
		//			"text":"设置文件的属性，这个参数只有文件被创建的时候有效，如果文件已经存在，这个参数被忽略。一些文件属性值如下:<br/><b>CFile.READONLY</b>: 文件具有只读属性. 某些程序会针对这个属性来决定文件是否可写, 但是 CFile 本身不受此属性影响.<br/><b>CFile.HIDDEN</b>: 文件具有隐藏属性.<br/><b>CFile.SYSTEM</b>: 文件具有系统属性.<br/><b>CFile.NORMAL</b>: 文件具有普通属性. 这是缺省值.<br/><b>CFile.TEMPORARY</b>: 文件具有临时属性. 如果以此属性生成文件, 当文件关闭时, 文件会被系统自动删除.<br/>要使用更多的属性, 直接设置属性对应的数值，可以查阅 MSDN 文档，获取对应的值。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"关闭一个文件, 把缓存里的数据写入文件, 释放文件的系统资源, 但是不释放对象的 C++ 资源, 要释放 C++ 资源使用 dispose 函数. 执行了 close 的CFile对象可以再次调用 create 打开一个文件进行操作。这个函数在大多数情况下是无需调用的，如果需要打开一个新文件，直接调用 create 就可以，close 会在内部被调用。对象 dispose 的时候，close 也会被自动调用。close 函数一般在明确需要释放一个文件的时候被调用，因为有时候，文件可能无法被共享打开，某些操作需要独占文件。",
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"把文件缓存刷新到文件。当我们执行了写入操作时，可能内容并没有真正的写入到文件，这个时候如果应用程序被强行终止，那么写入的数据可能会丢失。flush 函数一般也无需显示调用，因为 close 和 dispose 都会把内容变化刷新到文件。只有在为了保险起见，防止意外引起的数据丢失时，才需要使用这个函数。注意，频繁调用 flash 函数会耗费大量资源，因为每次flash意味着数据被强制写入硬盘等物理存储器。",
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"把文件的二进制数据读入一个 CMemory 对象, 这个内存对象可能被扩充内存来存储读取的数据。",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"memory",
		//			"text":"用于存储数据的内存对象，此内存可能更加需要被扩大，此时会被强制转化为使用系统内存。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[readLen]",
		//			"text":"要读取的字节数, 如果不设置此参数（或者此参数足够大）, 函数会试着读取当前位置到文件结尾的全部数据, 但是如果数据长度超过 16MB 字节, 函数返回 false. 也就是说如果要读取超过16MB字节的数据, 必须设置此参数. 但是如果此参数太大, 导致内存不足, 函数也可能失败, 一般不要一次读取太多数据, 读取大文件, 可以分多次读取。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[memoryPos]",
		//			"text":"数据存储到 CMemory 对象的起始位置。CMemory 可能还存有其他数据，为了不影响已经存储的数据，函数支持把数据写入到 CMemory 对象的指定位置。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功时返回读取的字节数, 即使此值为 0，可能说明没有数据可读，但是函数不会认为操作失败，失败返回 undefined。"
		//	},
		//	"remark":[
		//		"当需要扩大内存来保存数据的时候，如果数据大于64K，内存对象会被自动转换为使用系统内存。"
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
					if(!m->SetLength(requireLen)) break;//L"内存分配失败"
				}
				len = f->Read(m->Handle()+memPos,readLen);
				return store.Close(Uint32::New((int32_t)len));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"write(memory,[writeLen],[memoryPos])",
		//	"text":"把二进制数据写入文件。",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"memory",
		//			"text":"用于存储数据的内存对象。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[writeLen]",
		//			"text":"要写入的长度，此长度如果大于内存的可用长度，会被缩短，默认是从起始到结尾的全部数据。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[memoryPos]",
		//			"text":"要写入数据在CMemory的位置，默认是 0 。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"成功时返回写入的字节数, 即使此值为 0，可能说明没有数据被写入，但是函数不会认为操作失败，失败返回 undefined。"
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
		//	"text":"从当前位置读取到文件结尾, 把数据作为字串返回, 参数指定编码, 如果缺省（或者是 undefined）会检测是否是 utf-8编码，如果不是按 ANSI 编码处理。",
		//	"param":[
		//		{
		//			"type":"string|integer",
		//			"name":"[code]",
		//			"text":"文件的编码, 可以是数字或者字串, 比如 “utf-8”, 缺省值是 3, 代表操作系统当前代码页的 ANSI 编码。数字 0 和 1200 都表示 unicode 编码。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功时返回读取的字串，失败返回 undefined。"
		//	},
		//	"remark":[
		//		"常用编码，“utf-8”或 65001。数字比字串要快一些。",
		//		"函数总是一次从开头读取整个文件，如果文件长度大于 1Mb（0x100000字节），函数会抛出异常。"
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
					err.Format(L"文件太大 length = %d bytes",readLen);
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
		//	"text":"从当前位置读取到文件结尾, 把数据作为字串返回, 参数指定编码, 默认是ANSI编码。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"str",
		//			"text":"要写入的字串。"
		//		},
		//		{
		//			"type":"string|integer",
		//			"name":"[code]",
		//			"text":"文件的编码, 可以是数字或者字串, 比如 “utf-8”, 缺省值是 3, 代表操作系统当前代码页的 ANSI 编码。数字 0 和 1200 都表示 unicode 编码。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功时返回写入的长度，失败返回 undefined。"
		//	},
		//	"remark":[
		//		"常用编码，“utf-8”或 65001。数字比字串要快一些。"
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
		//	"text":"返回或设置文件的长度. 此值没有上限, 依赖于平台文件支持的最大值. 如果设置的长度小于文件原有长度, 后面的数据会被清空, 设置为 0 , 可以清空全部数据。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"pointer",
		//	"objtype":"integer",
		//	"text":"返回或设置文件的指针位置. 此值没有上限, 依赖于平台文件支持的最大值. 文件指针是指文件读写操作的位置, 此值可以大于文件大小. 设为 -1 表示调整指针到文件末尾"
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
	//	"text":"此常数对象提供文件系统的相关操作，如粘贴、复制、删除等。",
	//	"member":[//*
	class JSFileSystem{
		//*{
		//	"type":"function",
		//	"name":"isFolder(filename)",
		//	"text":"判断指定的路径是否是一个文件夹。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"filename",
		//			"text":"要检测的路径"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"如果是一个文件夹返回 true，否则返回 false。"
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
		//	"text":"判断指定的文件或文件夹是否存在。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"要检测的字串"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"如果存在返回 true，否则返回 false。"
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
		//	"text":"判断指定的字串是否是一个文件。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"filename",
		//			"text":"要检测的字串"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"如果是一个文件返回 true，否则返回 false。"
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
		//	"text":"删除一个文件或文件夹。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"要移除的文件（夹）路径。"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onFileProgress(param)]",
		//			"text":"设置一个回调函数，用于获取和设置操作进行时的一些信息和选项，具体说明参考 onFileProgress 函数。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"文件操作过程中出现任何系统错误，函数都会返回 false，但是如果用户通过回调函数终止了操作，函数仍然返回 true。函数返回 false 时，删除操作可能已经执行了一部分。以下一些情况可能引起函数失败：文件目录不存在、文件没有删除权限等。"
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
		//	"text":"复制一个文件或文件夹，源文件与目的文件不能相同。此操作可以续传，函数会检测上次复制的进度，接着上次的进度来进行数据拷贝。但是，某些极为特殊的情况，当目的文件夹的数据被其它程序改写，那么续传并不检测这些改动，从而造成源文件夹和目的文件夹的数据不同。如果不能肯定目的文件夹是否被改动，最好先删除目的同名文件（夹）。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"源文件（夹）路径。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"目的文件（夹）路径。"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onFileProgress(param)]",
		//			"text":"设置一个回调函数，用于获取和设置操作进行时的一些信息和选项，具体说明参考 onFileProgress 函数。"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onDataProgress(offset,total)]",
		//			"text":"设置一个回调函数，用于获取一个文件的拷贝的具体进度。因为此操作具体说明参考 onDataProgress 函数。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"文件操作过程中出现任何系统错误，函数都会返回 false，但是如果用户通过回调函数终止了操作，函数仍然返回 true。函数返回 false 时，复制操作可能已经执行了一部分。以下一些情况可能引起函数失败：源文件目录不存在、源文件和目的文件相同、源文件没有读取权限、目的文件没有写入权限等。"
		//	},
		//	"remark":[
		//		"注意：这个函数会覆盖目的目录的同名文件，而且没有任何警告。当不能确定目的目录没有同名文件的时候，不要使用这个函数，比如在复制大量照片到同一个文件夹的时候，不保证没有同名的照片文件。对于非大文件，非大量文件，使用 sh 开头的函数（shCopy，shMove，shDelete）。"
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
		//	"text":"移动一个文件或文件夹，源文件与目的文件不能相同。此操作可以续传，函数会检测上次复制的进度，接着上次的进度来进行数据拷贝。但是，某些极为特殊的情况，当目的文件夹的数据被其它程序改写，那么续传并不检测这些改动，从而造成源文件夹和目的文件夹的数据不同。如果不能肯定目的文件夹是否被改动，最好先删除目的同名文件（夹）。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"源文件（夹）路径。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"目的文件（夹）路径。"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onFileProgress(param)]",
		//			"text":"设置一个回调函数，用于获取和设置操作进行时的一些信息和选项，具体说明参考 onFileProgress 函数。"
		//		},
		//		{
		//			"type":"function",
		//			"name":"[onDataProgress(offset,total)]",
		//			"text":"设置一个回调函数，用于获取一个文件的拷贝的具体进度。因为此操作具体说明参考 onDataProgress 函数。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"文件操作过程中出现任何系统错误，函数都会返回 false，但是如果用户通过回调函数终止了操作，函数仍然返回 true。函数返回 false 时，复制操作可能已经执行了一部分。以下一些情况可能引起函数失败：源文件目录不存在、源文件和目的文件相同、源文件没有读取权限、目的文件没有写入权限等。"
		//	},
		//	"remark":[
		//		"移动操作可以用于重命名文件和文件夹。",
		//		"移动操作在同盘符下无需拷贝数据，本质上是改名操作。",
		//		"注意：这个函数会覆盖目的目录的同名文件，而且没有任何警告。当不能确定目的目录没有同名文件的时候，不要使用这个函数，比如在移动大量照片到同一个文件夹的时候，不保证没有同名的照片文件。对于非大文件，非大量文件，使用 sh 开头的函数（shCopy，shMove，shDelete）。"
		//	]
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onFileProgress(param)",
		//	"text":"这是一个回调函数，它不是 FileSystem 的成员函数，用于 copy，move，remove 3个函数的第三个参数。每当函数要操作一个文件和文件夹的时候，这个函数被回调两次，第一次是操作将要进行，此时可以设置跳过和终止选项；第二次是操作已经完成，此时设置跳过选项无效（当前文件的操作已经完成），但是终止选项会终止后面的整个操作。",
		//	"param":[
		//		{
		//			"type":"FileSystemParam",
		//			"name":"param",
		//			"text":"用于获取和设置当前操作的信息和选项，具体说明参见 FileSystemParam。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"这个函数无需返回值。"
		//	}
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onDataProgress(param)",
		//	"text":"这是一个回调函数，它不是 FileSystem 的成员函数，用于 copy，move 函数的第四个参数，监视文件数据拷贝的进度。回调函数每拷贝 1 MB（1048576 bytes） 字节呼叫一次，文件拷贝完成时呼叫一次。",
		//	"param":[
		//		{
		//			"type":"FileDataParam",
		//			"name":"param",
		//			"text":"这个变量有 3 个属性，分别是 offset,total,cancel，分别表示已经拷贝的数据、总数据、是否取消这个文件的拷贝操作，具体说明参见 FileDataParam。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"这个函数无需返回值。"
		//	}
		//}//*

		//*,{
		//	"type":"variant",
		//	"name":"FileSystemParam",
		//	"objtype":"object",
		//	"text":"此类型变量在 onFileProgress 回调函数里被使用，用于获取操作信息或者设置控制选项。每个子文件（夹）在处理的时候，这个函数被呼叫两次，第一次是要执行操作前，此时可以设置 skip 属性取消当前文件（夹）的操作；第二次呼叫是操作执行后。",
		//	"property":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"当前文件的源目录，这个值不包含当前文件名，只是源文件夹名称。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"当前文件的目的目录，这个值不包含当前文件名，只是源文件夹名称。注意：当调用 delete 函数的时候，dst 属性没有设置。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"isFolder",
		//			"text":"当前正在处理的项目，是否是一个文件夹。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"file",
		//			"text":"当前处理的文件或文件夹的名称，不包括父目录。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"stop",
		//			"text":"是否终止后面的操作。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"skip",
		//			"text":"是否跳过当前文件（夹）的操作，这个值只有在操作执行前的一次回调里设置才起作用，因为第二次呼叫的时候操作已经完成。如果一个文件夹被跳过，它的所有子项文件和文件夹都会被跳过。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"error",
		//			"text":"错误码，如果一切正常，这个值是 0 ，要获取错误描述，使用 FileSystem 的 getErrorMsg 函数，并且把这个错误码作为参数。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"preOrAfter",
		//			"text":"此次操作上操作前的回调还是操作完成后的呼叫，true 表示操作前。"
		//		}
		//	]
		//}//*
		//*,{
		//	"type":"variant",
		//	"name":"FileDataParam",
		//	"objtype":"object",
		//	"text":"此类型变量在 onDataProgress 回调函数里被使用，用于获取文件拷贝的当前进度，和控制是否取消操作。回调函数每拷贝 1 MB（1048576 bytes） 字节呼叫一次。",
		//	"property":[
		//		{
		//			"type":"integer",
		//			"name":"offset",
		//			"text":"已经拷贝的数据长度，注意：因为续传的存在，这个值并不是从 0 开始的，实际上除了文件的大小为 0 的情况，这个参数永远不会是 0。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"length",
		//			"text":"将要拷贝的数据总长度，它就是源文件的大小。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"cancel",
		//			"text":"如果设置这个值为 true，当前文件的拷贝会被终止，但是在文件夹操作中，其它文件的拷贝不会被取消。"
		//		}
		//	]
		//}//*

		//*,{
		//	"type":"function",
		//	"name":"getErrorMsg(code)",
		//	"text":"获取错误码的文本描述。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"code",
		//			"text":"onFileProgress 回调函数返回的错误码，0 表示成功，其它值表示出现错误。"
		//		}
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"返回错误的文本描述。"
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
		//	"text":"生成一个文件夹，如果第二个参数为 true，生成 path 指定路径的父文件夹，这个用法方便创建文件的父目录，无需拆分文件路径获取父文件夹字串。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"要生成的文件夹的路径，如果第二参数为true，生成的是此路径的父文件夹。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[parent]",
		//			"text":"如果这个参数设为 true，生成第一个参数指定路径的的父文件夹。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"如果函数成功返回 true，注意：如果文件夹已经存在，函数返回 true ，失败返回 undefined。"
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
		//	"text":"生成一个文件夹，如果第二个参数为 true，生成 path 指定路径的父文件夹，这个用法方便创建文件的父目录，无需拆分文件路径获取父文件夹字串。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"要设置的文件（夹）的路径。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"attribute",
		//			"text":"要设置的属性，这个值的意义依赖第三个参数，如果没有设置第三个参数，文件会被设为这个值指定的属性。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[option]",
		//			"text":"如果设置文件属性，0：设置文件属性为第二个参数指定的值，1：添加第二个参数指定的属性，2：移除第二个参数指定的属性。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"获取文件（夹）的文件属性。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"文件（夹）的路径。"
		//		}
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"如果函数成功返回一个代表文件属性的数值，失败返回 undefined。"
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
		//	"text":"复制 src 文件（夹），包括子文件夹和它的文件，dst 是新名字。如果目的位置有同名文件，默认会弹出确认对话框，可以把 noAskExist 设为 true 强制覆盖目的文件。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"源文件（夹）的路径。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"目的文件（夹）的新名称。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noAsk]",
		//			"text":"如果出现需要询问用户选择的情况，比如复制的时候目标文件已经存在，是否不弹出选择框，而是按默认行为自动处理，参数默认值是 false，即询问用户。如果设为 true，则不会询问用户，比如复制时目的文件夹已经存在，会强制覆盖。"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noUndo]",
		//			"text":"这个复制操作默认是可以在 Windows 资源管理器里撤销的，此参数设为 true，关闭可撤销选项。"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noError]",
		//			"text":"当出现错误时，是否不弹出错误提示对话框，默认值 false，出现错误时会弹出一个对话框提示错误信息，和让用户做出处理确认。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"移动 src 文件（夹），包括子文件夹和它的文件，dst 是新名字。如果目的位置有同名文件，默认会弹出确认对话框，可以把 noAskExist 设为 true 强制覆盖目的文件。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"源文件（夹）的路径。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"如果这个参数指定的文件夹不存在，这个参数就是新文件夹的路径；如果这个参数指定的文件夹已经存在，则移动到这个文件夹内，成为它的子文件夹，名称和源文件夹相同。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noAsk]",
		//			"text":"如果出现需要询问用户选择的情况，比如复制的时候目标文件已经存在，是否不弹出选择框，而是按默认行为自动处理，参数默认值是 false，即询问用户。如果设为 true，则不会询问用户，比如复制时目的文件夹已经存在，会强制覆盖。"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noUndo]",
		//			"text":"这个移动操作默认是可以在 Windows 资源管理器里撤销的，此参数设为 true，关闭可撤销选项。"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noError]",
		//			"text":"当出现错误时，是否不弹出错误提示对话框，默认值 false，出现错误时会弹出一个对话框提示错误信息，和让用户做出处理确认。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"重命名文件（夹）。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"src",
		//			"text":"源文件（夹）的路径。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"dst",
		//			"text":"新文件（夹）的名称。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noAsk]",
		//			"text":"如果出现需要询问用户选择的情况，比如重命名文件时目标文件已经存在，是否不弹出选择框，而是按默认行为自动处理，参数默认值是 false，即询问用户。如果设为 true，则不会询问用户，比如重命名时目的文件夹已经存在，而是自动选择一个新文件名。"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noUndo]",
		//			"text":"这个重命名操作默认是可以在 Windows 资源管理器里撤销的，此参数设为 true，关闭可撤销选项。"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noError]",
		//			"text":"当出现错误时，是否不弹出错误提示对话框，默认值 false，出现错误时会弹出一个对话框提示错误信息，和让用户做出处理确认。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
		//	"text":"删除文件（夹），包括子项。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"文件（夹）的路径。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[noAsk]",
		//			"text":"如果出现需要询问用户选择的情况，比如复制的时候目标文件已经存在，是否不弹出选择框，而是按默认行为自动处理，参数默认值是 false，即询问用户。如果设为 true，则不会询问用户，比如复制时目的文件夹已经存在，会强制覆盖。"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noUndo]",
		//			"text":"删除操作默认是删除到回收站，如果想直接删除文件（夹），设置此参数为 true。"
		//		},
		//		{
		//			"type":"boolelan",
		//			"name":"[noError]",
		//			"text":"当出现错误时，是否不弹出错误提示对话框，默认值 false，出现错误时会弹出一个对话框提示错误信息，和让用户做出处理确认。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
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
	//	"text":"列出一个文件夹和它的子文件夹的文件。",
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
		//	"text":"这是一个回调函数，设置这个数据来获取信息。",
		//	"param":[
		//		{
		//			"type":"FileSearchData",
		//			"name":"param",
		//			"text":"包含搜索到的文件（夹）的信息，或者设置搜索选项。具体细节参考 FileSearchData 条目。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"这个函数无需返回值。"
		//	}
		//}//*
		//*,{
		//	"type":"variant",
		//	"name":"FileSearchData",
		//	"objtype":"object",
		//	"text":"onList 函数的参数，包含文件信息和搜索选项。",
		//	"property":[
		//		{
		//			"type":"string",
		//			"name":"name",
		//			"text":"文件名，不包含路径。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"sub",
		//			"text":"文件的子路径，这个路径加上 FileSearch 对象的 path 属性，才是全路径，如果文件就在当前的搜索路径下，这个值是空串。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"flag",
		//			"text":"如果当前搜索结果是文件, 此值为 0 ,如果搜索结果是文件夹且第一次呼叫(文件夹可能被呼叫两次,依赖搜索选项)此值为 1, 第二次呼叫, 此值为 2。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"stop",
		//			"text":"是否终止搜索。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"skip",
		//			"text":"是否跳过当前项，只有当前项是一个文件夹且是第一次回调（flag = 1）的情况下设置此值为 true 才有意义。"
		//		}
		//	]
		//}//*
		static void __stdcall _onList(cs::ListData* ld){
			FTINF* fi = (FTINF*)ld->ExtraData;
			if(fi->onList->IsUndefined()) return;
			Handle<Value> args[1];
			fi->data->Set(String::New("skip"),Boolean::New(ld->Skip));
			fi->data->Set(String::New("stop"),Boolean::New(ld->Stop));
			fi->data->Set(String::New("flag"),Int32::New(ld->Flag));//0 文件, 1 pre, 2 after.
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
		//	"text":"搜索指定路径的文件夹, 文件夹内的文件(或文件夹)在回调函数onList中获取, 每搜索到一个符号条件的文件(夹), 函数被呼叫一次。如果指定的路径（path）是一个文件，这个函数仍然会成功，回调函数返回这个文件。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"path",
		//			"text":"要搜索的路径，“.”表示当前路径（js脚本所在的路径），“..”表示当前路径的上一级目录。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"这个函数无需返回值。"
		//	}
		//}//*

		//*,{
		//	"type":"property",
		//	"name":"child",
		//	"objtype":"boolean",
		//	"text":"设置 search 函数是否搜索子文件夹，此值缺省是 false。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"before",
		//	"objtype":"boolean",
		//	"text":"设置是否每搜索到一个文件夹是否先回调一次，此时 onList 的参数的 flag 属性设为 1。文件总是只回调一次，且 flag 设为 0 。缺省值 true。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"after",
		//	"objtype":"boolean",
		//	"text":"设置是否每搜索到一个文件夹在完成了所有子项的回调后，回调一次，此时 onList 的参数的 flag 属性设置为 2。缺省值 true。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"path",
		//	"objtype":"string",
		//	"text":"如果 search 函数调用成功，FileSearch 会设置 path 属性为 search 的 path 参数的值。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"filter",
		//	"objtype":"array",
		//	"text":"用于搜索过滤的一个数组，如果在 FileSearch 的 filter 属性中添加了一个元素，那么只有这个后缀的文件会被搜索（文件夹不受影响）。比如添加 “jpg”，那么只有 jpg 文件会被搜索到。"
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
	//	"text":"添加文件类型过滤, 一旦添加了一个文件类型, 只有指定类型的文件会被搜索. 使用这个函数是为了方便。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filter",
	//			"text":"一个由逗号“,”分隔的文件后缀字串, 不要包括星号和点, 比如正确的格式: “jpg,gif”, 不要写成: “*.jpg,*.gif,.bmp”。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//}//*

	//*,{
	//	"type":"function",
	//	"name":"delFilter(filter)",
	//	"text":"移除文件过滤类型, 一旦添加了一个文件类型, 只有指定类型的文件会被搜索. 使用这个函数是为了方便。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filter",
	//			"text":"一个由逗号“,”分隔的文件后缀字串, 不要包括星号和点, 比如正确的格式: “jpg,gif”, 不要写成: “*.jpg,*.gif,.bmp”。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//}//*

	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\filelist.jsuser"}//*

	//*,{
	//	"type":"const",
	//	"name":"FilePath",
	//	"text":"此常数对象提供文件路径的操作。所有函数支持“/”或“\\”分隔的路径字串。",
	//	"member":[
	//	{
	//		"type":"function",
	//		"name":"getExt(path)",
	//		"text":"返回路径的后缀(文件类型), 不含\".\", 如果没有后缀返回空串。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"文件路径。"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"返回一个不包含“.”的后缀名，如果路径不包含一个后缀，返回空串。"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"getTitle(path)",
	//		"text":"返回文件名称, 不含路径和后缀。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"文件路径。"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"返回不包含后缀和文件夹的文件名字串。"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"getParent(path)",
	//		"text":"返回当前目录的父目录, 或者空串(如果没有父目录)。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"文件路径。"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"返回一个字串。"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"toSlash(path)",
	//		"text":"把路径里的反斜杠换成斜杠, 并且删除多余的连续斜杠, 包括末尾的斜杠。这是 Linux 风格的路径。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"文件路径。"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"返回一个字串。"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"toBackslash(path)",
	//		"text":"把路径里的斜杠换成反斜杠, 并且删除多余的连续斜杠, 包括末尾的斜杠。这是 Windows 风格的路径。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"文件路径。"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"返回一个字串。"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"linkPath(path,...)",
	//		"text":"用反斜杠连接参数作为一个路径, 这个函数可以自动处理头尾已经有斜杠或反斜杠的情况。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"需要连接的一系列路径字串。"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"返回一个字串。"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"linkExt(path,ext)",
	//		"text":"路径允许多个 \".\" 连续, 这个函数只处理ext的前面是否有一个 \".\", 如果有多个点, 不继续处理。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"文件路径。"
	//			}
	//			,{
	//				"type":"string",
	//				"name":"ext",
	//				"text":"后缀名，可以包含“.”，也可以不包含“.”。"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"返回一个字串。"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"getFullPath(path)",
	//		"text":"获取 path 的完整路径。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path",
	//				"text":"文件路径。"
	//			}
	//		],
	//		"return":{
	//			"type":"string",
	//			"text":"返回一个字串。"
	//		}
	//	}
	//	,{
	//		"type":"function",
	//		"name":"isSame(path1,path2)",
	//		"text":"windows系统路径不区分大小写, 不区分正反斜杠, 这个函数检测两个路径是否是同一个。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"path1",
	//				"text":"文件路径。"
	//			}
	//			,{
	//				"type":"string",
	//				"name":"path2",
	//				"text":"文件路径。"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"返回两个路径是否相同。"
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