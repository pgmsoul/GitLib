#include "stdafx.h"
#include "v8base.h"

#include <stdio.h>
#include <miniupnpc/miniwget.h>
#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/upnperrors.h>

#ifdef _DEBUG
	#pragma comment(lib,"../miniupnpc/msvc/debug/miniupnpc.lib")
#else
	#pragma comment(lib,"../miniupnpc/msvc/release/miniupnpc.lib")
#endif

namespace v8{
	//*{
	//	"type":"group",
	//	"name":"net",
	//	"text":"网络相关。",
	//	"remark":[
	//		""
	//	],
	//	"member":[//*
	class V8Http : public cs::HttpRequest{
	protected:
		cs::Memory<char> _html;
		uint			_dlLen;
		cs::String		_rhead;
		cs::Thread _thread;
		static void _onDownLoad(void* obj){
			V8Http* vh = (V8Http*)obj;

			HandleScope store;
			Handle<Value> argv[2];
			argv[0] = Undefined();
			argv[1] = String::New("");

			Handle<Value> prop = vh->HttpObject->Get(String::New("onRequest"));
			if(prop->IsFunction()){
				Handle<Function> onRequest = Handle<Function>::Cast(prop);
				if(vh->_dlLen!=-1){
					Handle<Value> jscode = vh->HttpObject->Get(String::New("code"));
					uint cp;
					if(jscode->IsString()){
						cs::String code;
						GetString(jscode,code);
						cp = cs::GetCodePage(code);
					}else if(jscode->IsUint32()){
						cp = jscode->Uint32Value();
					}else{
						cp = 3;
					}

					cs::String str = NULL_STR;
					str.FromMultiByte(vh->_html,vh->_dlLen,cp);

					argv[0] = String::New((uint16_t*)str.Handle());
					if(!vh->_rhead.IsNull()){
						argv[1] = String::New((uint16_t*)vh->_rhead.Handle());
					}
				}
				CallFunc(vh->HttpObject,onRequest,2,argv);
			}

			prop = vh->HttpObject->Get(String::New("onRequestMemory"));
			if(prop->IsFunction()){
				Handle<Function> onRequestMemory = Handle<Function>::Cast(prop);
				if(vh->_dlLen!=-1){
					vh->_html.SetLength(vh->_dlLen);
					argv[0] = Uint32::New((UINT_PTR)&vh->_html);
					argv[1] = True();
					argv[0] = GetEnv()->GetTemplate(TEMPLATE_ID_MEMORY)->GetFunction()->CallAsConstructor(2,argv);
				}
				if(!vh->_rhead.IsNull()){
					argv[1] = String::New((uint16_t*)vh->_rhead.Handle());
				}
				CallFunc(vh->HttpObject,onRequestMemory,2,argv);
			}
			vh->_html.Free();
			vh->_dlLen = -1;
			vh->_rhead = NULL_STR;
			vh->_thread.Close();
		}
		void _onRequest(cs::Memory<char>* mem){
			_dlLen = Request(mem->Handle(),mem->Length(),_html,&_rhead);
			delete mem;
			GetEnv()->queue.PostCallback(&_onDownLoad,this);
		}
	public:
		Persistent<Object> HttpObject;
		V8Http():_html(true),_dlLen(-1){}
		~V8Http(){
			_thread.Stop();
			HttpObject.Dispose();
		}
		bool StartRequest(void* data,uint len,bool async){
			if(async){
				cs::Memory<char>* mem = new cs::Memory<char>;
				mem->CopyFrom((char*)data,len);
				return _thread.Call<void>(this,&V8Http::_onRequest,mem);
			}
			_dlLen = Request(data,len,_html,&_rhead);
			_onDownLoad(this);
			return true;
		}
	};
	//*{
	//	"type":"class",
	//	"name":"CHttpRequest",
	//	"text":"发起 http 请求。",
	//	"remark":[
	//		"如果请求的页面设置了跳转，请求会指向新的页面。"
	//	],
	//	"member":[//*
	class JsHttp : public JsCObject<V8Http,JsHttp>{
	public:
		//*{
		//	"type":"function",
		//	"name":"request([upload],[uplen],[sync])",
		//	"text":"执行一个http请求，这个函数可以是同步执行，也可以是异步执行的，需要设置回调函数 onRequest 或者 onRequestMemory。",
		//	"param":[
		//		{
		//			"type":"string|CMemory|integer",
		//			"name":"[upload]",
		//			"text":"这个参数如果既不是一个 CMemory 对象，也不是一个整数，会被转换为字串对待。要上传的字串数据或者二进制数据，一般来说对于字串数据来说，可以在链接页面的后面添加 ?param1=value1&amp;param2=value2 格式的数据来上传参数，但是依赖于服务器的设置，也可以把这些数据使用此参数上传，但是这两种情况是有区别的。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[uplen]",
		//			"text":"如果 upload 参数是字串，这个参数会被忽略，如果 upload 参数是一个 CMemory 对象，则此参数指定数据的长度，如果此参数缺省或为 0 或大于 CMemory 对象的数据长度，CMemory的全部数据都会上传。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[async]",
		//			"text":"是否异步调用，http 请求是一个耗时操作，如果不希望程序被挂起，必须使用异步请求。但是在简单应用中，同步调用更方便。这个值的默认值是 false 即如果不设置此参数，请求是同步的。异步情况下，不需要保存上传数据，函数调用完可以立即销毁 upload"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	},
		//	"remark":[
		//		"调用此函数前，如果没有调用 parseUrl 函数设置要请求的页面，那么至少应该设置主机地址，即 host 属性，否则函数会失败。",
		//		"当异步调用 request 时必须调用 startMsgLoop 函数，因为函数是异步执行，如果没有调用 startMsgLoop，在 http 请求完成之前，可能应用程序已经退出。",
		//		"对于一个非窗口程序，应该在 onRequest 或 onRequestMemory 函数里面调用 exitMsgLoop 函数，否则应用程序没有退出机制，会一直运行。",
		//		"如果 request 函数失败，不要调用 startMsgLoop，因为这种情况下，onRequest 函数不会被回调。"
		//	],
		//	"example":[
		//		"\"use strict\";",
		//		"require(\"jsbase.dll\");",
		//		"Class(function main(){",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;var http = new CHttpRequest();",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;http.parseUrl(\"http://www.google.com.hk\");",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;http.onRequest = function(html,head){",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;//alert(html);",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;alert(head);",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;exitMsgLoop();",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;}",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;if(http.request(0,0,true))",
		//		"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;startMsgLoop();",
		//		"},true);",
		//		"main();"
		//	]
		//}//*
		static Handle<Value> request(const Arguments& args){
			HandleScope store;
			while(true){
				V8Http* http;
				Local<Object> self = args.This();
				if(!GetCHandle(http,self)) break;
				if(http==NULL) break;
				if(!http->Create()) break;
				http->Agent = L"jsuser 2.0";

				if(!http->Connect()) break;
				
				Handle<Array> header = Handle<Array>::Cast(self->Get(String::New("header")));
				uint count = header->Length();
				cs::String head;
				for(uint i=0;i<count;i++){
					GetString(header->Get(i),head);
					http->SetHeader(head);
				}

				http->HttpObject = Persistent<Object>::New(self);

				int uplen = 0;
				bool rst;
				bool async = GET_ARGS_VALUE(2,false,Boolean);
				if(args.Length()<1){
					rst = http->StartRequest(0,0,1);
				}else if(IsTemplate(args[0],TEMPLATE_ID_MEMORY)){
					cs::Memory<char>* m;
					if(!GetCHandle(m,args[0]->ToObject())) break;
					uplen = GET_ARGS_INT(1,0);
					if(uplen<=0||uplen>(int)m->Length()) uplen = m->Length();
					rst = http->StartRequest(m->Handle(),uplen,async);
				}else if(args[0]->IsUint32()){
					uint data = GET_ARGS_INT(0,0);
					uplen = GET_ARGS_INT(1,0);
					if((data&0xffff0000)==0&&uplen!=0){
						ThrowException(String::New("upload not a pointer"));
						break;
					}
					rst = http->StartRequest((void*)(UINT_PTR)data,uplen,async);
				}else{
					cs::Memory<char> upload;
					cs::String str;
					GetString(args[0],str);
					uplen = str.ToMultiByte(&upload);
					rst = http->StartRequest(upload.Handle(),uplen,async);
				}
				return Bool(rst);
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"parseUrl(url)",
		//	"text":"设置要加载的 url，这个值必须是以 http 或者 https 开头的一个有效 url。这个函数自动设置下列参数：主机地址 host，页面 page，http 还是 https 模式，端口 port 重置为默认 http 端口或者 https 端口。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"url",
		//			"text":"设置加载的 url 地址，如果当前页面设置了跳转，请求会被指向新的页面。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> parseUrl(const Arguments& args){
			HandleScope store;
			while(true){
				if(args.Length()<1) break;
				V8Http* http;
				Local<Object> self = args.This();
				if(!GetCHandle(http,self)) break;
				if(http==NULL) break;
				cs::String url;
				GetString(args[0],url);
				if(!http->ParseUrl(url)) break;
				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"property",
		//	"name":"method",
		//	"objtype":"string",
		//	"text":"设置/获取 http 的模式，一般是 \"POST\" 或 \"GET\"，默认是 \"GET\"。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"host",
		//	"objtype":"string",
		//	"text":"设置/获取 http 的主机域名，它可以是一个域名，也可以是一个 IP 地址。示例：www.baidu.com，注意有 www 和没有 www 是完全不同的主机地址，大多数主机会把这两类情况解析到同样的地址，但是某些网站则可能不同。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"page",
		//	"objtype":"string",
		//	"text":"设置/获取 http 请求的页面，如果此值为空，表示打开主页。示例：/index.html，页面地址不能再包含 http 头或者主机地址。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"port",
		//	"objtype":"integer",
		//	"text":"设置/获取服务器的端口，一般使用默认值，无需设置。比如，绝大多数 http 服务器使用 80 端口。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"https",
		//	"objtype":"bool",
		//	"text":"设置/获取是否使用 https 模式。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"code",
		//	"objtype":"integer|string",
		//	"text":"设置/获取页面的编码，默认值（如果没有设置）是 3 即 ANSI 编码。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"header",
		//	"objtype":"array",
		//	"text":"设置请求头，每个元素被作为头的一行发送。"
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onRequest(html,header)",
		//	"text":"这是一个回调函数，设置 CHttpRequest 对象的 onRequest 属性为此类型的函数。",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"html",
		//			"text":"页面的 html 文本，如果页面不是 ANSI 编码需要设置正确的编码值，否则会产生乱码。注意：函数不会自己读取 html 页面里设置的编码，因为函数并不分析具体的 html 内容。函数失败，这个值是 undefined。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"header",
		//			"text":"返回的头信息，这个值是一个多行文本。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"此函数无需返回值。"
		//	},
		//	"remark":[
		//		"如果下载失败，html 是 undefined。html 为空字符串时，不代表下载失败，因为页面返回的数据长度可能是 0 ."
		//	]
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onRequestMemory(mem,header)",
		//	"text":"这是一个回调函数，设置 CHttpRequest 对象的 onRequestMemory 属性为此类型的函数。有时候下载页面并非文本，而是二进制数据，这种情况下不要设置 onRequest 回调函数，而是设置 onRequestMemory 属性为此格式的函数。",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"mem",
		//			"text":"二进制数据保存的对象。函数失败，这个值是 undefined。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"header",
		//			"text":"返回的头信息，这个值是一个多行文本，或者空串。"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"此函数无需返回值。"
		//	},
		//	"remark":[
		//		"如果下载失败，mem 是 undefined。mem 长度为 0 时，不代表下载失败，因为页面返回的数据长度可能是 0 ."
		//	]
		//}//*

		static Handle<Value> get(cs::String& name,V8Http* cobj,Local<Object>& self){
			if(name==L"method"){
				return String::New((uint16_t*)cobj->Method.Handle());
			}else if(name==L"host"){
				return String::New((uint16_t*)cobj->Host.Handle());
			}else if(name==L"page"){
				return String::New((uint16_t*)cobj->Page.Handle());
			}else if(name==L"port"){
				return Uint32::New(cobj->Port);
			}else if(name==L"https"){
				return Boolean::New(cobj->GetHttpsMode());
			}
			return Undefined();
		}
		static void set(cs::String& name,V8Http* cobj,Local<Value>& value,Local<Object>& self){
			if(name==L"method"){
				GetString(value,cobj->Method);
				cobj->Method.Upper();
			}else if(name==L"host"){
				GetString(value,cobj->Host);
			}else if(name==L"page"){
				GetString(value,cobj->Page);
			}else if(name==L"port"){
				uint32_t port = value->ToUint32()->Uint32Value();
				if(port!=0) cobj->Port = port;
			}else if(name==L"https"){
				bool https = value->ToBoolean()->BooleanValue();
				cobj->SetHttpsMode(https);
			}
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			Handle<ObjectTemplate> inst = ft->InstanceTemplate();  
			SET_PROP_RO(func,header,Array::New());
			SET_CLA_ACCESSOR(method);
			SET_CLA_ACCESSOR(host);
			SET_CLA_ACCESSOR(page);
			SET_CLA_ACCESSOR(port);
			SET_CLA_ACCESSOR(https);
			SET_CLA_FUNC(parseUrl);
			SET_CLA_FUNC(request);
		}
	};
	//*]}//*
#define HEAD_FLAG 0x152a93bf
#define SOCK_ERROR_CLOSED	-4	//被正确的关闭
#define SOCK_ERROR_MEMORY	-3	//无法分配内存
#define SOCK_ERROR_LENGTH	-2	//数据长度大于 0x100000
#define SOCK_ERROR_HEADFLAG	-1	//数据头标识错误
#define MAX_DATA_LENGTH		0x100000
	class AsynSocket : public cs::Socket{
	protected:
		cs::Thread sendThread,recvThread;
		AsynSocket(){
			connected = false;
		}
		void WaitClose(){
			recvThread.Wait();
			sendThread.Wait();
			recvThread.Close();
			sendThread.Close();
		}
		~AsynSocket(){
			Close();
			sendThread.Stop();
			recvThread.Stop();
		}
	public:
		bool connected;
		cs::Memory<char> sendBuf,recvBuf;
		bool IsConnected(){return connected;}
		void Close(){
			Shutdown();
			cs::Socket::Close();
			connected = 0;
		}
	};
	class JsSocket : public JsWrapObject<AsynSocket,JsSocket>{
		static Handle<Value> getRemoteIp(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::Socket* cobj;
				if(!GetCHandle(cobj,self)) break;
				cs::SOCKADDR sa;
				if(!cobj->GetPeerAddr(sa)) break;
				return stack.Close(String::New(sa.GetIp()));
			}
			return Undefined();
		}
		static Handle<Value> getRemotePort(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::Socket* cobj;
				if(!GetCHandle(cobj,self)) break;
				cs::SOCKADDR sa;
				if(!cobj->GetPeerAddr(sa)) break;
				return stack.Close(Uint32::New(sa.GetPort()));
			}
			return Undefined();
		}
		static Handle<Value> getLocalIp(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::Socket* cobj;
				if(!GetCHandle(cobj,self)) break;
				cs::SOCKADDR sa;
				if(!cobj->GetSockAddr(sa)) break;
				return stack.Close(String::New(sa.GetIp()));
			}
			return Undefined();
		}
		static Handle<Value> getLocalPort(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::Socket* cobj;
				if(!GetCHandle(cobj,self)) break;
				cs::SOCKADDR sa;
				if(!cobj->GetSockAddr(sa)) break;
				return stack.Close(Uint32::New(sa.GetPort()));
			}
			return Undefined();
		}
		static Handle<Value> connect(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<2) break;
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				LPCSTR addr;
				if(!args[0]->IsString()) break;
				v8::String::Utf8Value sv(args[0]);
				addr = (LPCSTR)*sv;
				WORD port = (WORD)GET_ARGS_INT(1,0);
				cobj->Connect(addr,port);
				return True();
			}
			return Undefined();
		}
		static Handle<Value> send(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				uint len = GET_ARGS_INT(0,0);
				bool nohead = GET_ARGS_VALUE(1,false,Boolean);
				if(!cobj->Send(len,nohead)) break;
				return True();
			}
			return Undefined();
		}
		static Handle<Value> isConnected(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				return Bool(cobj->IsConnected());
			}
			return Undefined();
		}
		static Handle<Value> recv(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				bool nohead = GET_ARGS_VALUE(0,false,Boolean);
				if(!cobj->Recv(nohead)) break;
				return True();
			}
			return Undefined();
		}
		static Handle<Value> close(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,self)) break;
				if(cobj->IsNull()) break;

				cobj->Close();
				return True();
			}
			return Undefined();
		}
	public:
		class CBObject : public WrapObject{
			typedef struct ONCONNECT : public cs::_struct{
				CBObject* cobj;
				bool result;
				int error;
			};
			typedef struct ONSEND : public cs::_struct{
				CBObject* cobj;
				int		error;
				uint	length;
				bool	nohead;
			};
			typedef struct ONRECV : public cs::_struct{
				CBObject* cobj;
				uint	dataLen;
				int		error;
				cs::Event* wait;
			};
			static void OnConnect(void* p){
				ONCONNECT* oc = (ONCONNECT*)p;
				HandleScope stack;
				v8::Handle<Value> prop = oc->cobj->self->Get(NEW_STR(onConnect));
				oc->cobj->recvThread.Close();
				if(prop->IsFunction()){
					v8::Handle<Function> func = v8::Handle<Function>::Cast(prop);
					v8::Handle<Value> argv[2];
					argv[0] = Boolean::New(oc->result);
					argv[1] = Int32::New(oc->error);
					CallFunc(oc->cobj->self,func,2,argv);
				}
				delete oc;
			}
			void onConnect(LPSTR str,WORD port){
				cs::SOCKADDR sa;
				sa.SetPort(port);
				cs::IP ip;
				ip = str;
				if(ip.IsNull()){
					cs::Host host;
					if(host.Create(str)){
						for(int i=0;i<host.GetIpCount();i++){
							sa.SetIp(host.GetIp(i));
							connected = cs::Socket::Connect(sa);
							if(connected) break;
						}
					}
				}else{
					sa.SetIp(ip);
					connected = cs::Socket::Connect(sa);
				}
				ONCONNECT* oc = new ONCONNECT;
				oc->cobj = this;
				oc->error = WSAGetLastError();
				oc->result = connected;
				cs::MemoryFree(str,1);
				GetEnv()->queue.PostCallback(&OnConnect,oc);
			}
			static void OnSend(void* p){
				ONSEND* os = (ONSEND*)p;
				HandleScope stack;
				v8::Handle<Value> prop = os->cobj->self->Get(NEW_STR(onSend));
				os->cobj->sendThread.Close();
				if(prop->IsFunction()){
					v8::Handle<Function> func = v8::Handle<Function>::Cast(prop);
					v8::Handle<Value> argv[2];
					argv[0] = Uint32::New(os->length);
					argv[1] = Int32::New(os->error);
					CallFunc(os->cobj->self,func,2,argv);
				}
				delete os;
			}
			void onSend(uint len,bool nohead){
				ONSEND* os = new ONSEND;
				os->cobj = this;
				uint offset = 0;
				os->error = 0;
				if(!nohead){
					char head[8];
					*(uint*)head = HEAD_FLAG;
					*(uint*)(head+4) = len;
					while(offset<8){
						uint sendlen = cs::Socket::Send(head+offset,8-offset);
						if(sendlen==0||sendlen==-1){
							os->error = WSAGetLastError();
							if(os->error==0) os->error = SOCK_ERROR_CLOSED;
							break;
						}
						offset += sendlen;
					}
				}
				offset = 0;
				if(os->error==0){
					while(offset<len){
						uint sendlen = cs::Socket::Send(sendBuf.Handle()+offset,len-offset);
						if(sendlen==0||sendlen==-1){
							os->error = WSAGetLastError();
							break;
						}
						offset += sendlen;
					}
				}
				os->length = offset;
				if(os->error){
					connected = false;
				}
				GetEnv()->queue.PostCallback(&OnSend,os);
			}
			void postRecv(uint dataLen,cs::Event* wait,int error){
				ONRECV od;
				od.dataLen = dataLen;
				od.cobj = this;
				od.error = error;
				od.wait = wait;
				GetEnv()->queue.PostCallback(&OnRecv,&od);
				wait->Wait();
			}
			static void OnRecv(void* p){
				HandleScope stack;
				ONRECV* od = (ONRECV*)p;
				v8::Handle<Value> prop = od->cobj->self->Get(NEW_STR(onRecv));
				if(!prop->IsFunction()){
					if(od->error){
						od->cobj->connected = false;
						od->cobj->recvThread.Close();
					}
					od->wait->Signal();
					return;
				}
				v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(prop);
				v8::Handle<Value> argv[2];
				argv[0] = Uint32::New(od->dataLen);
				argv[1] = Int32::New(od->error);
				if(od->error){
					od->cobj->connected = false;
					od->cobj->recvThread.Close();
				}
				CallFunc(od->cobj->self,func,2,argv);
				od->wait->Signal();
			}
			bool recvData(cs::Event* wait,uint dataLen,char* buf){
				uint offset = 0;
				while(true){
					if(!checkRecvBuf(wait)){
						return 0;
					}
					int len = Receive(buf+offset,dataLen-offset);
					if(len==0||len==-1){
						uint error = WSAGetLastError();
						if(error==0) error = SOCK_ERROR_CLOSED;
						postRecv(dataLen,wait,error);
						return 0;
					}
					offset += len;
					if(offset<dataLen) continue;
					postRecv(dataLen,wait,0);
					return 1;
				}
			}
			bool recvHead(uint& dataLen,cs::Event* wait,uint& error){
				char buf[8];
				uint offset = 0;
				error = 0;
				while(true){
					dataLen = 0;
					int len = Receive(buf+offset,8-offset);
					if(len==0||len==-1){
						error = WSAGetLastError();
						if(error==0) error = SOCK_ERROR_CLOSED;
						return 0;
					}
					offset += len;
					if(offset<8) continue;
					static uint head_flag = HEAD_FLAG;
					if(memcmp(&head_flag,buf,4)!=0){
						//错误的数据，可能是发送了错误的数据或者有数据。
						error = SOCK_ERROR_HEADFLAG;
						return 0;
					}
					dataLen = *(uint*)(buf+4);
					if(dataLen>0x100000){
						dataLen = 0;
						//错误的数据，数据太长，可能是发送了错误的数据或者有数据。
						error = SOCK_ERROR_LENGTH;
						return 0;
					}
					if(dataLen==0){
						postRecv(dataLen,wait,0);
						offset = 0;
						continue;
					}
					return 1;
				}
			}
			bool checkRecvBuf(cs::Event* wait){
				if(recvBuf.Length()<MAX_DATA_LENGTH){
					recvBuf.SetUseSysMemory(1);
					if(!recvBuf.SetLength(MAX_DATA_LENGTH)){
						postRecv(0,wait,SOCK_ERROR_MEMORY);
						return false;
					}
				}
				return true;
			}
			void onRecv(){
				cs::Event wait;
				wait.Create(1,0);
				int recvOffset = 0;
				while(true){
					uint dataLen,error = 0;
					if(!recvHead(dataLen,&wait,error)){
						postRecv(dataLen,&wait,error);
						break;
					}
					if(!recvData(&wait,dataLen,recvBuf.Handle())){
						break;
					}
				}
			}
			void onRecvNoHead(){
				cs::Event wait;
				wait.Create(1,0);

				while(true){
					if(!checkRecvBuf(&wait)){
						postRecv(0,&wait,SOCK_ERROR_MEMORY);
						return;
					}
					int len = Receive(recvBuf.Handle(),MAX_DATA_LENGTH);
					if(len==0||len==-1){
						uint error = WSAGetLastError();
						if(error==0) error = SOCK_ERROR_CLOSED;
						postRecv(0,&wait,error);
						return;
					}
					postRecv(len,&wait,0);
				}
			}
		public:
			bool Connect(LPCSTR str,WORD port){
				if(connected||!recvThread.IsNull()||!sendThread.IsNull()) return false;
				cs::Socket::Close();
				if(!Create()) return false;
				int len = strlen(str);
				LPSTR addr = (LPSTR)cs::MemoryAlloc(len+1,1);
				memcpy(addr,str,len+1);
				bool r = recvThread.Call<void>(this,&CBObject::onConnect,addr,port);
				if(!r){
					cs::MemoryFree(addr);
				}
				return r;
			}
			bool Recv(bool nohead){
				if(!connected) return 0; 
				if(recvBuf.Length()<MAX_DATA_LENGTH){
					recvBuf.SetUseSysMemory(1);
					if(!recvBuf.SetLength(MAX_DATA_LENGTH)) return 0;
				}
				if(nohead){
					return recvThread.Call<void>(this,&CBObject::onRecvNoHead);
				}
				return recvThread.Call<void>(this,&CBObject::onRecv);
			}
			//如果nohead = false，len 可以是 0
			bool Send(uint len,bool nohead){
				if(!connected) return 0; 
				if(nohead&&len==0) return 0;
				if(sendBuf.Length()<len) return 0;
				if(!nohead&&len>MAX_DATA_LENGTH) return 0;
				return sendThread.Call<void>(this,&CBObject::onSend,len,nohead);
			}
		};
		static void on_create(Handle<Object>& self,WrapObject* cobj){
			Handle<Value> argv[2];
			argv[0] = Uint32::New((UINT_PTR)&cobj->recvBuf);
			argv[1] = True();
			Handle<Value> mem = GetEnv()->GetTemplate(TEMPLATE_ID_MEMORY)->GetFunction()->NewInstance(2,argv);
			SET_PROP_RO(self,recvData,mem);
			argv[0] = Uint32::New((UINT_PTR)&cobj->sendBuf);
			mem = GetEnv()->GetTemplate(TEMPLATE_ID_MEMORY)->GetFunction()->NewInstance(2,argv);
			cobj->sendBuf.SetUseSysMemory(1);
			SET_PROP_RO(self,sendData,mem);
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(connect);
			SET_CLA_FUNC(send);
			SET_CLA_FUNC(recv);
			SET_CLA_FUNC(close);
			SET_CLA_FUNC(isConnected);
			SET_CLA_FUNC(getRemotePort);
			SET_CLA_FUNC(getRemoteIp);
			SET_CLA_FUNC(getLocalIp);
			SET_CLA_FUNC(getLocalPort);
		}
	};
	class JsTcpServer : public JsWrapObject<cs::TCPServer,JsTcpServer>{
		static Handle<Value> startup(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				cobj->OnAccept.Bind(cobj,&CBObject::onAccept);
				WORD port = (WORD)GET_ARGS_INT(0,0);
				HRESULT rst = cobj->StartupAsyn(port);
				if(rst!=0){
					/*cs::String msg;
					GetSystemMessage(msg,rst);
					Handle<Object> err = StringObject::New(NEW_WSTR(msg.Handle()))->ToObject();
					err->Set(NEW_STR(code),Uint32::New(rst));
					ThrowException(err);*/
					break;
				}
				return True();
			}
			return Undefined();
		}
		static Handle<Value> shutdown(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				CBObject* cobj;
				if(!GetCHandle(cobj,self)) break;

				cobj->Close();
				return True();
			}
			return Undefined();
		}
	public:
		static void __stdcall clientChanged(){
			;
		}
		class CBObject : public WrapObject{
		public:
			~CBObject(){
				_Handle.Close();
				_thread.Stop();
			}
			typedef struct ONACCEPT : public cs::_struct{
				SOCKET sock;
				CBObject*	cobj;
				ONACCEPT(){
					cobj = 0;
					sock = 0;
				}
			};
			static void onJsAccept(void* p){
				HandleScope stack;
				ONACCEPT* oa = (ONACCEPT*)p;
				v8::Handle<Value> prop = oa->cobj->self->Get(NEW_STR(onAccept));
				if(!prop->IsFunction()){
					::shutdown(oa->sock,2);
					closesocket(oa->sock);
					delete oa;
					return;
				}
				Handle<Object> jssock = GetEnv()->GetTemplate(TEMPLATE_ID_SOCKET)->GetFunction()->NewInstance();
				JsSocket::CBObject* asynSock;
				GetCHandle(asynSock,jssock);
				
				Handle<Array> clients = Handle<Array>::Cast(oa->cobj->self->GetHiddenValue(NEW_STR(clients)));
				uint index = 0;
				while(true){
					Handle<Value> elm = clients->Get(index);
					bool canUse = false;
					if(elm.IsEmpty()||elm->IsUndefined()){
						canUse = true;
					}else{
						JsSocket::CBObject* as;
						if(GetCHandle(as,elm->ToObject())){
							if(!as->IsConnected()){
								canUse = true;
							}
						}else canUse = true;
					}
					if(canUse){
						clients->Set(index,jssock);
						break;
					}
					index++;
				}
				asynSock->Attach(oa->sock);
				asynSock->connected = true;

				v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(prop);
				Handle<Value> argv[1];
				argv[0] = jssock;
				CallFunc(oa->cobj->self,func,1,argv);
				delete oa;
			}
			void onAccept(SOCKET sock){
				ONACCEPT* oa = new ONACCEPT;
				oa->sock = sock;
				oa->cobj = this;

				GetEnv()->queue.PostCallback(&onJsAccept,oa);
			}
		};
		static void on_create(Handle<Object>& self,WrapObject* cobj){
			self->SetHiddenValue(NEW_STR(clients),Array::New());
		}
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(startup);
			SET_CLA_FUNC(shutdown);
		}
	};
	typedef struct PORTMAPPING : public cs::_struct{
		char ip[40];
		char port[6];
		char localPort[6];
		char protocol[4];
		char name[80];
		char enabled[6];
		PORTMAPPING(){
			Reset();
		}
		void Reset(){
			ip[0] = 0;
			port[0] = 0;
			localPort[0] = 0;
			protocol[0] = 0;
			name[0] = 0;
			enabled[0] = 0;
		}
	}PORTMAPPING,*LPPORTMAPPING;
	class Upnp : public cs::_class{
	protected:
		UPNPDev*	_upnpDevice;
		UPNPUrls	_urls;// @UPNPUrls : 如果返回值非0，UPNPUrls作为传出参数
		IGDdatas	_igdDatas;// @IGDdatas : 如果返回值非0，IGDdatas作为传出参数
		char		_localIp[64];	/* my ip address on the LAN */
	public:
		Upnp(){
			_upnpDevice = 0;
			_urls.controlURL = 0;
			_localIp[0] = 0;
			WSADATA wsd;
			::WSAStartup(MAKEWORD(2,2),&wsd);
		}
		~Upnp(){
			::WSACleanup();
			if(_urls.controlURL) FreeUPNPUrls(&_urls);
			if(_upnpDevice) freeUPNPDevlist(_upnpDevice);
		}
		bool Create(){
			const char * multicastif = 0;
			const char * minissdpdpath = 0;
			int error = 0;
			_upnpDevice = upnpDiscover(200,multicastif,minissdpdpath,0,0,&error);
			if(_upnpDevice==0) return 0;
			int r = UPNP_GetValidIGD(_upnpDevice,&_urls,&_igdDatas,_localIp,sizeof(_localIp));
			return r==1;
		}
		bool GetPortMapping(PORTMAPPING& pm,int index){
			if(!_urls.controlURL) return 0;
			char iBuf[6];
			_snprintf_s(iBuf,6,_TRUNCATE,"%d",index);
			char rHost[64] = {0};
			char duration[16] = {0};
			int r = UPNP_GetGenericPortMappingEntry(_urls.controlURL,_igdDatas.first.servicetype,iBuf,pm.port, pm.ip, pm.localPort,pm.protocol, pm.name, pm.enabled,rHost, duration);
			return r==0;
		}
		uint GetAllPortMapping(cs::ObjectList<PORTMAPPING>& pmList){
			if(!_urls.controlURL) return 0;

			int i = 0;
			char index[6];
			char rHost[64];
			char duration[16];
			while(true){
				_snprintf_s(index, 6,_TRUNCATE, "%d", i);
				PORTMAPPING* pm = new PORTMAPPING;
				rHost[0] = '\0'; 
				duration[0] = '\0';
				int r = UPNP_GetGenericPortMappingEntry(_urls.controlURL,_igdDatas.first.servicetype,index,pm->port, pm->ip, pm->localPort,pm->protocol, pm->name, pm->enabled,rHost, duration);
				if(r){
					delete pm;
					break;
				}
				pmList.AddIn(pm);
				i++;
			}
			return pmList.Count();
		}
		const char* GetIp(){return _localIp;}
		bool GetExernalIp(char* extip40){
			if(!_urls.controlURL) return 0;
			int r = UPNP_GetExternalIPAddress(_urls.controlURL,_igdDatas.first.servicetype,extip40);
			return (r==UPNPCOMMAND_SUCCESS);
		}
		bool SetPortMapping(uint eport,const char * description = 0,const char * proto = "TCP",bool enable = true,const char * iaddr = 0,uint iport = 0){
			if(!_urls.controlURL) return 0;

			if(eport==0) return 0;

			char eBuf[6],iBuf[6];
			_snprintf_s(eBuf,6,_TRUNCATE,"%d",eport);
			if(proto==0||strcmp(proto,"UDP")!=0)
				proto = "TCP";
			if(description==NULL)
				description = "jsuser network";
			if(iport==0)
				iport = eport;
			_snprintf_s(iBuf,6,_TRUNCATE,"%d",iport);
			if(iaddr==NULL)
				iaddr = _localIp;
			const char* enableBuf = enable?"1":"0";

			int r = UPNP_AddPortMapping(_urls.controlURL,_igdDatas.first.servicetype,
				eBuf, iBuf, iaddr, description,
				proto, 0, 0,enableBuf);
			return r==UPNPCOMMAND_SUCCESS;
		}
		bool GetPortMapping(PORTMAPPING& pm,int eport,bool tcpOrUdp){
			if(!_urls.controlURL) return 0;
			if(eport==0) return 0;
			pm.Reset();
			_snprintf_s(pm.port,6,_TRUNCATE,"%d",eport);
			if(tcpOrUdp) strcpy_s(pm.protocol,4,"TCP");
			else strcpy_s(pm.protocol,4,"UDP");

			int r = UPNP_GetSpecificPortMappingEntry(_urls.controlURL,_igdDatas.first.servicetype,pm.port,pm.protocol,0,pm.ip,pm.localPort,pm.name,pm.enabled,0);
			return r==UPNPCOMMAND_SUCCESS;
		}
		bool RemovePortMapping(int eport,bool tcp = true){
			if(!_urls.controlURL) return 0;
			if(eport==0) return 0;

			char port[6],*proto;
			_snprintf_s(port,6,_TRUNCATE,"%d",eport);

			if(tcp) proto = "TCP";
			else proto = "UDP";

			int r = UPNP_DeletePortMapping(_urls.controlURL,_igdDatas.first.servicetype,port, proto, 0);
			return r==UPNPCOMMAND_SUCCESS;
		}

	};
	Handle<Object> PMToObj(PORTMAPPING& pm){
		Handle<Object> obj = Object::New();
		bool enable = pm.enabled[0]=='1';
		SET_PROP(obj,enable,Boolean::New(enable));
		SET_PROP(obj,ip,String::New(pm.ip));
		SET_PROP(obj,localPort,Uint32::New(atoi(pm.localPort)));
		SET_PROP(obj,name,String::New(pm.name));
		SET_PROP(obj,port,Uint32::New(atoi(pm.port)));
		bool udpOrTcp = cs::MbsEqual("UDP",pm.protocol);
		SET_PROP(obj,udp,Boolean::New(udpOrTcp));
		return obj;
	}
	class JsUpnp : public JsWrapObject<Upnp,JsUpnp>{
		//*,{
		//	"type":"function",
		//	"name":"set(port,[udpOrTcp],[disabled],[innerPort],[name],[localIp])",
		//	"text":"设置一个端口映射，如果外部端口为 port，类型为 udpOrTcp 的映射已经存，则重新设置它的参数，否则添加一个端口映射。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"port",
		//			"text":"映射的外部端口，也就是外网访问的端口，这个端口值同时也是这个端口映射的标识，不同的端口映射外网端口必须不同。端口是一个 16(最大 0xFFFF) 位整数，大于 16 位的值会被截断。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[udpOrTcp]",
		//			"text":"端口映射的类型，TCP 还是 UDP 缺省是 TCP 即 false。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[disabled]",
		//			"text":"这个端口映射是否设为禁用状态，缺省是启用状态 false。"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[innerPort]",
		//			"text":"映射的内部端口，这个值缺省和外部端口相同，一般没有必要设置一个不同的端口。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"[name]",
		//			"text":"这个端口映射的文本描述，参数缺省时会自动使用一个默认名称。"
		//		},
		//		{
		//			"type":"string",
		//			"name":"[localIp]",
		//			"text":"端口映射的主机，即外网访问 port 端口时，被指向的主机 ip。缺省是本机。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> setPM(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				Upnp* cobj;
				if(!GetCHandle(cobj,self)) break;

				uint port = GET_ARGS_INT(0,0);
				if((WORD)port==0) break; 

				const char* proto = GET_ARGS_VALUE(1,false,Boolean)?"UDP":"TCP";

				bool enable = !GET_ARGS_VALUE(2,false,Boolean);

				uint localPort = GET_ARGS_INT(3,0);

				cs::Memory<char> name;
				if(args.Length()>4) GetUtf8(args[4],name);

				cs::Memory<char> ip;
				if(args.Length()>5) GetUtf8(args[5],ip);

				if(!cobj->SetPortMapping(port,name,proto,enable,ip,localPort)) break;

				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"get(port,[udpOrTcp])",
		//	"text":"返回指定端口和类型的映射参数。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"port",
		//			"text":"要获取的映射的外部端口，port 是一个 16 位正整数。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[udpOrTcp]",
		//			"text":"端口映射的类型，udp 传 true，tcp 传 false，缺省是 false。"
		//		}
		//	],
		//	"return":{
		//		"type":"PortMapping",
		//		"text":"成功返回一个 PortMapping 对象，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getPM(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				Upnp* cobj;
				if(!GetCHandle(cobj,self)) break;

				uint port = GET_ARGS_INT(0,0);
				bool proto = !GET_ARGS_VALUE(1,false,Boolean);
				PORTMAPPING pm;
				if(!cobj->GetPortMapping(pm,port,proto)) break;

				return stack.Close(PMToObj(pm));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"indexOf(index)",
		//	"text":"返回指定索引的端口映射参数。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"要获取的端口映射的索引。"
		//		}
		//	],
		//	"return":{
		//		"type":"PortMapping",
		//		"text":"成功返回一个 PortMapping 对象，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> indexOf(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				Upnp* cobj;
				if(!GetCHandle(cobj,self)) break;

				uint index = GET_ARGS_INT(0,0);
				PORTMAPPING pm;
				if(!cobj->GetPortMapping(pm,index)) break;

				return stack.Close(PMToObj(pm));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"remove(port,[udpOrTcp])",
		//	"text":"移除指定端口和类型的端口映射。",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"port",
		//			"text":"外部端口。"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[udpOrTcp]",
		//			"text":"类型（udp -- true，tcp -- false）。"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> remove(const Arguments& args){
			HandleScope stack;
			while(true){
				if(args.Length()<1) break;
				Handle<Object> self = args.This();
				Upnp* cobj;
				if(!GetCHandle(cobj,self)) break;

				uint port = GET_ARGS_INT(0,0);
				bool tcp = !GET_ARGS_VALUE(1,false,Boolean);
				if(!cobj->RemovePortMapping(port,tcp)) break;

				return True();
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getExtIp()",
		//	"text":"获取外网 ip 地址。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功返回一个 ip 地址字串，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getExtIp(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				Upnp* cobj;
				if(!GetCHandle(cobj,self)) break;

				uint index = GET_ARGS_INT(0,0);
				char ip[40];
				if(!cobj->GetExernalIp(ip)) break;

				return stack.Close(String::New(ip));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"getIp()",
		//	"text":"获取本机 ip 地址。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"成功返回一个 ip 地址字串，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> getIp(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				Upnp* cobj;
				if(!GetCHandle(cobj,self)) break;

				uint index = GET_ARGS_INT(0,0);
				const char* ip = cobj->GetIp();
				if(ip[0]==0) break;

				return stack.Close(String::New(ip));
			}
			return Undefined();
		}
		//*,{
		//	"type":"function",
		//	"name":"create()",
		//	"text":"初始化 Upnp 对象。",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"成功返回 true，失败返回 undefined。"
		//	}
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				Upnp* cobj;
				if(!GetCHandle(cobj,self)) break;

				if(!cobj->Create()) break;
				return True();
			}
			return Undefined();
		}
	public:
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			func->Set(NEW_STR(set),FunctionTemplate::New(&setPM),ReadOnly);
			func->Set(NEW_STR(get),FunctionTemplate::New(&getPM),ReadOnly);
			SET_CLA_FUNC(indexOf);
			SET_CLA_FUNC(remove);
			SET_CLA_FUNC(getExtIp);
			SET_CLA_FUNC(getIp);
			SET_CLA_FUNC(create);
		}
	};
	void LoadNet(Handle<Object>& glb){
		JsHttp::Load(glb,L"CHttpRequest",TEMPLATE_ID_HTTP);
		JsSocket::Load(glb,L"CSocket",TEMPLATE_ID_SOCKET);
		JsTcpServer::Load(glb,L"CTcpServer",TEMPLATE_ID_TCPSERVER);
		JsUpnp::Load(glb,L"CUpnp",TEMPLATE_ID_UPNP);
	}
	//*]}//*
}