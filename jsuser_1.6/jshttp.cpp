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
	//	"text":"������ء�",
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
	//	"text":"���� http ����",
	//	"remark":[
	//		"��������ҳ����������ת�������ָ���µ�ҳ�档"
	//	],
	//	"member":[//*
	class JsHttp : public JsCObject<V8Http,JsHttp>{
	public:
		//*{
		//	"type":"function",
		//	"name":"request([upload],[uplen],[sync])",
		//	"text":"ִ��һ��http�����������������ͬ��ִ�У�Ҳ�������첽ִ�еģ���Ҫ���ûص����� onRequest ���� onRequestMemory��",
		//	"param":[
		//		{
		//			"type":"string|CMemory|integer",
		//			"name":"[upload]",
		//			"text":"�����������Ȳ���һ�� CMemory ����Ҳ����һ���������ᱻת��Ϊ�ִ��Դ���Ҫ�ϴ����ִ����ݻ��߶��������ݣ�һ����˵�����ִ�������˵������������ҳ��ĺ������ ?param1=value1&amp;param2=value2 ��ʽ���������ϴ����������������ڷ����������ã�Ҳ���԰���Щ����ʹ�ô˲����ϴ������������������������ġ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[uplen]",
		//			"text":"��� upload �������ִ�����������ᱻ���ԣ���� upload ������һ�� CMemory ������˲���ָ�����ݵĳ��ȣ�����˲���ȱʡ��Ϊ 0 ����� CMemory ��������ݳ��ȣ�CMemory��ȫ�����ݶ����ϴ���"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[async]",
		//			"text":"�Ƿ��첽���ã�http ������һ����ʱ�����������ϣ�����򱻹��𣬱���ʹ���첽���󡣵����ڼ�Ӧ���У�ͬ�����ø����㡣���ֵ��Ĭ��ֵ�� false ����������ô˲�����������ͬ���ġ��첽����£�����Ҫ�����ϴ����ݣ���������������������� upload"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		"���ô˺���ǰ�����û�е��� parseUrl ��������Ҫ�����ҳ�棬��ô����Ӧ������������ַ���� host ���ԣ���������ʧ�ܡ�",
		//		"���첽���� request ʱ������� startMsgLoop ��������Ϊ�������첽ִ�У����û�е��� startMsgLoop���� http �������֮ǰ������Ӧ�ó����Ѿ��˳���",
		//		"����һ���Ǵ��ڳ���Ӧ���� onRequest �� onRequestMemory ����������� exitMsgLoop ����������Ӧ�ó���û���˳����ƣ���һֱ���С�",
		//		"��� request ����ʧ�ܣ���Ҫ���� startMsgLoop����Ϊ��������£�onRequest �������ᱻ�ص���"
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
		//	"text":"����Ҫ���ص� url�����ֵ�������� http ���� https ��ͷ��һ����Ч url����������Զ��������в�����������ַ host��ҳ�� page��http ���� https ģʽ���˿� port ����ΪĬ�� http �˿ڻ��� https �˿ڡ�",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"url",
		//			"text":"���ü��ص� url ��ַ�������ǰҳ����������ת������ᱻָ���µ�ҳ�档"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����/��ȡ http ��ģʽ��һ���� \"POST\" �� \"GET\"��Ĭ���� \"GET\"��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"host",
		//	"objtype":"string",
		//	"text":"����/��ȡ http ��������������������һ��������Ҳ������һ�� IP ��ַ��ʾ����www.baidu.com��ע���� www ��û�� www ����ȫ��ͬ��������ַ�����������������������������ͬ���ĵ�ַ������ĳЩ��վ����ܲ�ͬ��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"page",
		//	"objtype":"string",
		//	"text":"����/��ȡ http �����ҳ�棬�����ֵΪ�գ���ʾ����ҳ��ʾ����/index.html��ҳ���ַ�����ٰ��� http ͷ����������ַ��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"port",
		//	"objtype":"integer",
		//	"text":"����/��ȡ�������Ķ˿ڣ�һ��ʹ��Ĭ��ֵ���������á����磬������� http ������ʹ�� 80 �˿ڡ�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"https",
		//	"objtype":"bool",
		//	"text":"����/��ȡ�Ƿ�ʹ�� https ģʽ��"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"code",
		//	"objtype":"integer|string",
		//	"text":"����/��ȡҳ��ı��룬Ĭ��ֵ�����û�����ã��� 3 �� ANSI ���롣"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"header",
		//	"objtype":"array",
		//	"text":"��������ͷ��ÿ��Ԫ�ر���Ϊͷ��һ�з��͡�",
		//	"remark":[
		//		"���Ҫ�ϴ����������ݣ��������ͷ \"Content-Type\\: text/xml\"",
		//		"���Ҫʹ�� ?param1=value1&param2=value2 ���ַ�ʽ�ϴ����ݣ��������ͷ \"Content-Type\\: application/x-www-form-urlencoded\""
		//	]
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onRequest(html,header)",
		//	"text":"����һ���ص����������� CHttpRequest ����� onRequest ����Ϊ�����͵ĺ�����",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"html",
		//			"text":"ҳ��� html �ı������ҳ�治�� ANSI ������Ҫ������ȷ�ı���ֵ�������������롣ע�⣺���������Լ���ȡ html ҳ�������õı��룬��Ϊ����������������� html ���ݡ�����ʧ�ܣ����ֵ�� undefined��"
		//		},
		//		{
		//			"type":"string",
		//			"name":"header",
		//			"text":"���ص�ͷ��Ϣ�����ֵ��һ�������ı���"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"�˺������践��ֵ��"
		//	},
		//	"remark":[
		//		"�������ʧ�ܣ�html �� undefined��html Ϊ���ַ���ʱ������������ʧ�ܣ���Ϊҳ�淵�ص����ݳ��ȿ����� 0 ."
		//	]
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onRequestMemory(mem,header)",
		//	"text":"����һ���ص����������� CHttpRequest ����� onRequestMemory ����Ϊ�����͵ĺ�������ʱ������ҳ�沢���ı������Ƕ��������ݣ���������²�Ҫ���� onRequest �ص��������������� onRequestMemory ����Ϊ�˸�ʽ�ĺ�����",
		//	"param":[
		//		{
		//			"type":"CMemory",
		//			"name":"mem",
		//			"text":"���������ݱ���Ķ��󡣺���ʧ�ܣ����ֵ�� undefined��"
		//		},
		//		{
		//			"type":"string",
		//			"name":"header",
		//			"text":"���ص�ͷ��Ϣ�����ֵ��һ�������ı������߿մ���"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"�˺������践��ֵ��"
		//	},
		//	"remark":[
		//		"�������ʧ�ܣ�mem �� undefined��mem ����Ϊ 0 ʱ������������ʧ�ܣ���Ϊҳ�淵�ص����ݳ��ȿ����� 0 ."
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
#define SOCK_ERROR_CLOSED	-4	//����ȷ�Ĺر�
#define SOCK_ERROR_MEMORY	-3	//�޷������ڴ�
#define SOCK_ERROR_LENGTH	-2	//���ݳ��ȴ��� 0x100000
#define SOCK_ERROR_HEADFLAG	-1	//����ͷ��ʶ����
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
	//*,{
	//	"type":"class",
	//	"name":"CSocket",
	//	"text":"Socket ����",
	//	"remark":[
	//		"�ṩ socket ����ع��ܡ�"
	//	],
	//	"member":[//*
	class JsSocket : public JsWrapObject<AsynSocket,JsSocket>{
		//*{
		//	"type":"function",
		//	"name":"getRemoteIp()",
		//	"text":"��ȡһ�������ӵ� socket ��Զ�� ip ��ַ��socket ���봦������״̬��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�����һ�� ip ��ַ�ִ���ʧ�ܷ��� undefined��"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"getRemotePort()",
		//	"text":"����һ�������ӵ� socket ��Զ�̶˿ڡ�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�����һ���˿�ֵ��ʧ�ܷ��� undefined��"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"getLocalIp()",
		//	"text":"����һ�������� socket �ı��� ip ��ַ�������һ���������ڣ������ַ��������ַ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�����һ�� ip ��ַ�ִ���ʧ�ܷ��� undefined��"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"getLocalPort()",
		//	"text":"����һ�������� socket �ı��ض˿ڡ�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"integer",
		//		"text":"�ɹ�����һ���˿�ֵ��ʧ�ܷ��� undefined��"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"connect(ip|host)",
		//	"text":"�������ӣ����������һ���첽��������Ϊ������һ����ʱ���������ӳɹ������Ҫ�� onConnect �ص��������ȡ��",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"ip|host",
		//			"text":"Ҫ���ӵ� ip ��ַ����������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�����ɹ������������ӷ��� true�������ⲻ����ɹ����ӵ���������ʧ�ܷ��� undefined��"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"send(len,[nohead])",
		//	"text":"�������ݣ������ַ��ͷ�ʽ��һ���Ǻ�����ͷ��Ϣ�ķ�ʽ������ȱʡ��ʽ������ֻ������ CTcpServer �����������Ҫ����ͨ���ķ�������nohead ��Ϊ true��",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"len",
		//			"text":"Ҫ���͵����ݳ��ȣ�Ҫ���͵�������Ҫͨ�� socket ����� sendData ���ã�����һ�� CMemory ��������������ֵ�� 1M �ֽڣ�һ�η��ͳ��� 1M �ֽڵ����ݻ�ʧ�ܡ�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[nohead]",
		//			"text":"ʹ�ú�������Ϣͷ�ķ�ʽ������ͨ��ʽ���ͣ�ȱʡ�Ǻ�����ͷ��ʽ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	},
		//	"remark":[
		//		"send �������첽�������鿴�������ݵĽ������Ҫ���� onSend �ص�������",
		//		"��ͨ������ʽ���䲻ͬ�����ʹ�ú�����ͷ�ķ�ʽ�����ݿ���һ�ν��գ�Ҳ���� onRecv ֻҪ���ص������Ǻͷ��Ͷ� send ����������ͬ���������ݣ������Լ��������ݣ������˱�̡�"
		//	]
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"isConnected()",
		//	"text":"���ص�ǰ socket �Ƿ�������״̬��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"���ӷ��� true��δ���ӷ��� false��"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"recv()",
		//	"text":"���������̣߳�������������������socket ���޷��������ݵģ����һ�� socket ����Ҫ�������ݣ���ô�Ͳ���Ҫ���� recv �������� socket ����� onRecv �ص��������ȡ���յ����ݡ�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�����߳������ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"close()",
		//	"text":"�ر����ӣ��ͷ� socket ��Դ��������պͷ����߳���Ȼ�����У����ǻ�����̣߳����Ǿ������ʱ�������ӳٵġ�onRecv �� onSend �����᷵�ط�������롣",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
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
		//*,{
		//	"type":"property",
		//	"name":"sendData",
		//	"objtype":"CMemory",
		//	"text":"��������Ҫ���͵����ݣ�ֱ�� onSend �������أ���Щ���ݲŻ����������ͳ�ȥ��sendData �ĳ��Ȳ��ܱ�����Ϊ���� 1M������ send ������ʧ�ܡ�"
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onSend(len,error)",
		//	"text":"����һ���ص����������� CSocket ����� onSend ����Ϊ�����͵ĺ�����",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"len",
		//			"text":"�Ѿ����͵����ݵĳ��ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"error",
		//			"text":"�����룬�������û�д���error = 0����������һ���ⲿ������������һ���ڲ�ϵͳ����"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"�˺������践��ֵ��"
		//	},
		//	"remark":[
		//		"sendData ����������Ҫ���͵����ݣ�����һ�� CMemory ����ÿ�η��͵����ݲ��ܴ��� 1M �ֽڡ�"
		//	]
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"recvData",
		//	"objtype":"CMemory",
		//	"text":"����������յ����ݣ��� onSend �������ȡ�������ݵĳ��ȡ�"
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onRecv(len,error)",
		//	"text":"����һ���ص����������� CSocket ����� onRecv ����Ϊ�����͵ĺ�����",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"len",
		//			"text":"�������ݵĳ��ȡ�"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"error",
		//			"text":"�����룬�������û�д���error = 0����������һ���ⲿ������������һ���ڲ�ϵͳ����"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"�˺������践��ֵ��"
		//	},
		//	"remark":[
		//		"���ʹ��������ͷ�Ĵ��䷽ʽ���⺯�����Ƿ��� send �������͵����ݳ��ȡ����ʹ��ͨ���ķ�ʽ������ճ����ǲ�ȷ���ġ�"
		//	]
		//}//*
		//*,{
		//	"type":"function",
		//	"name":"onConnect(connected,error)",
		//	"text":"����һ���ص����������� CSocket ����� onConnect ����Ϊ�����͵ĺ�����",
		//	"param":[
		//		{
		//			"type":"boolean",
		//			"name":"connected",
		//			"text":"���������Ƿ�ɹ���"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"error",
		//			"text":"�����룬�������û�д���error = 0����������һ���ⲿ������������һ���ڲ�ϵͳ����"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"�˺������践��ֵ��һ���Ͽ��� socket ���Ա�����ʹ�á�"
		//	}
		//}//*
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
						//��������ݣ������Ƿ����˴�������ݻ��������ݡ�
						error = SOCK_ERROR_HEADFLAG;
						return 0;
					}
					dataLen = *(uint*)(buf+4);
					if(dataLen>0x100000){
						dataLen = 0;
						//��������ݣ�����̫���������Ƿ����˴�������ݻ��������ݡ�
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
			//���nohead = false��len ������ 0
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
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\socketclient.jsuser"}//*

	//*,{
	//	"type":"class",
	//	"name":"CTcpServer",
	//	"text":"�ṩ TCP ���������ܡ�",
	//	"member":[//*
	class JsTcpServer : public JsWrapObject<cs::TCPServer,JsTcpServer>{
		//*{
		//	"type":"function",
		//	"name":"startup(port)",
		//	"text":"���� tcp ��������port ָ���˿ڡ�",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"port",
		//			"text":"port ��һ��16λ������"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��ͬһ�˿ڣ�ֻ������һ��������������"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"shutdown()",
		//	"text":"�ر�����������ļ��������������������Ӱ�����е����ӣ���ֻ�ǹر��µ����ӽ�����������ر����е����ӡ�",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
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
		//*,{
		//	"type":"function",
		//	"name":"onAccept(client)",
		//	"text":"����һ���ص����������пͻ�������ʱ��onAccept ���ص���",
		//	"param":[
		//		{
		//			"type":"CSocket",
		//			"name":"client",
		//			"text":"�Ϳͻ������ӵ� socket��"
		//		}
		//	],
		//	"return":{
		//		"type":"void",
		//		"text":"����û�з���ֵ��"
		//	},
		//	"remark":[
		//		"���û������ onAccept �ص����������пͻ�����������ᱻ�����رա�����Ч�������ﵽ����ʱ��������Ҳ�ᱻ�رա�"
		//	]
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"maxClients",
		//	"objtype":"integer",
		//	"text":"��ȡ������������������ȱʡ�� 100�����������һ���������ֵ����ȱʡֵ�Դ����������ֵ������Ͽ����е����ӣ���ʹʵ���������Ѿ������趨ֵ�������½����ӻᱻ�رգ�ֱ��ʵ��������С���趨��ֵ��"
		//}//*
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
				
				Handle<Array> clients = Handle<Array>::Cast(oa->cobj->self->GetHiddenValue(NEW_STR(clients)));
				uint index = 0;
				int max_count = oa->cobj->self->Get(NEW_STR(maxClients))->Int32Value();
				if(max_count<=0) max_count = 100;
				bool canreserv = false;
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
						canreserv = true;
						break;
					}
					index++;
					if((int)index>=max_count){
						break;
					}
				}
				if(!canreserv){
					::shutdown(oa->sock,2);
					closesocket(oa->sock);
					delete oa;
					return;
				}
				Handle<Object> jssock = GetEnv()->GetTemplate(TEMPLATE_ID_SOCKET)->GetFunction()->NewInstance();
				JsSocket::CBObject* asynSock;
				GetCHandle(asynSock,jssock);
				clients->Set(index,jssock);

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
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\socketserver.jsuser"}//*
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
		UPNPUrls	_urls;// @UPNPUrls : �������ֵ��0��UPNPUrls��Ϊ��������
		IGDdatas	_igdDatas;// @IGDdatas : �������ֵ��0��IGDdatas��Ϊ��������
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
	//*,{
	//	"type":"class",
	//	"name":"CUpnp",
	//	"text":"�ṩ·�����Ķ˿�ӳ�������",
	//	"member":[//*
	class JsUpnp : public JsWrapObject<Upnp,JsUpnp>{
		//*{
		//	"type":"function",
		//	"name":"set(port,[udpOrTcp],[disabled],[innerPort],[name],[localIp])",
		//	"text":"����һ���˿�ӳ�䣬����ⲿ�˿�Ϊ port������Ϊ udpOrTcp ��ӳ���Ѿ��棬�������������Ĳ������������һ���˿�ӳ�䡣",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"port",
		//			"text":"ӳ����ⲿ�˿ڣ�Ҳ�����������ʵĶ˿ڣ�����˿�ֵͬʱҲ������˿�ӳ��ı�ʶ����ͬ�Ķ˿�ӳ�������˿ڱ��벻ͬ���˿���һ�� 16(��� 0xFFFF) λ���������� 16 λ��ֵ�ᱻ�ضϡ�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[udpOrTcp]",
		//			"text":"�˿�ӳ������ͣ�TCP ���� UDP ȱʡ�� TCP �� false��"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[disabled]",
		//			"text":"����˿�ӳ���Ƿ���Ϊ����״̬��ȱʡ������״̬ false��"
		//		},
		//		{
		//			"type":"integer",
		//			"name":"[innerPort]",
		//			"text":"ӳ����ڲ��˿ڣ����ֵȱʡ���ⲿ�˿���ͬ��һ��û�б�Ҫ����һ����ͬ�Ķ˿ڡ�"
		//		},
		//		{
		//			"type":"string",
		//			"name":"[name]",
		//			"text":"����˿�ӳ����ı�����������ȱʡʱ���Զ�ʹ��һ��Ĭ�����ơ�"
		//		},
		//		{
		//			"type":"string",
		//			"name":"[localIp]",
		//			"text":"�˿�ӳ������������������� port �˿�ʱ����ָ������� ip��ȱʡ�Ǳ�����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ���˿ں����͵�ӳ�������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"port",
		//			"text":"Ҫ��ȡ��ӳ����ⲿ�˿ڣ�port ��һ�� 16 λ��������"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[udpOrTcp]",
		//			"text":"�˿�ӳ������ͣ�udp �� true��tcp �� false��ȱʡ�� false��"
		//		}
		//	],
		//	"return":{
		//		"type":"PortMapping",
		//		"text":"�ɹ�����һ�� PortMapping ����ʧ�ܷ��� undefined��"
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
		//	"text":"����ָ�������Ķ˿�ӳ�������",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"index",
		//			"text":"Ҫ��ȡ�Ķ˿�ӳ���������"
		//		}
		//	],
		//	"return":{
		//		"type":"PortMapping",
		//		"text":"�ɹ�����һ�� PortMapping ����ʧ�ܷ��� undefined��"
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
		//	"text":"�Ƴ�ָ���˿ں����͵Ķ˿�ӳ�䡣",
		//	"param":[
		//		{
		//			"type":"integer",
		//			"name":"port",
		//			"text":"�ⲿ�˿ڡ�"
		//		},
		//		{
		//			"type":"boolean",
		//			"name":"[udpOrTcp]",
		//			"text":"���ͣ�udp -- true��tcp -- false����"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ���� ip ��ַ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�����һ�� ip ��ַ�ִ���ʧ�ܷ��� undefined��"
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
		//	"text":"��ȡ���� ip ��ַ��",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"string",
		//		"text":"�ɹ�����һ�� ip ��ַ�ִ���ʧ�ܷ��� undefined��"
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
		//	"text":"��ʼ�� Upnp ����",
		//	"param":[
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
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
	//*]}//*
	//*,{
	//	"type":"class",
	//	"name":"Host",
	//	"text":"������һ����������ϵͳ���ṩ�������ִ�����Ϊ�ض��� ip ��ַ�Լ�������Ϣ�Ĺ��ܡ�",
	//	"member":[//*
	class JsHost : public JsObject<JsHost>{
		//*{
		//	"type":"function",
		//	"name":"create([name])",
		//	"text":"�����������Ƴ�ʼ����������name ������һ��������Ҳ������һ�� ip ��ַ��ȱʡ��մ���ʾ������",
		//	"param":[
		//		{
		//			"type":"string",
		//			"name":"[name]",
		//			"text":"�������ƣ���������һ��������Ҳ������һ�� ip ��ַ��ȱʡ��մ���ʾȡ�ر�������Ϣ��"
		//		}
		//	],
		//	"return":{
		//		"type":"boolean",
		//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
		//	}
		//}//*
		static Handle<Value> create(const Arguments& args){
			HandleScope stack;
			while(true){
				Handle<Object> self = args.This();
				cs::Memory<char> name;
				if(args.Length()>0&&!args[0]->IsUndefined()){ 
					GetAscii(args[0],name);
				}
				WSADATA wsd;
				::WSAStartup(MAKEWORD(2,2),&wsd);
				hostent* host = ::gethostbyname(name);
				if(host==0) break;
				int count = 0;
				Handle<Array> aliases = Array::New();
				self->Set(NEW_STR(aliases),aliases);
				while(host->h_aliases[count]){
					aliases->Set(count,String::New(host->h_aliases[count]));
					count++;
				}

				Handle<Array> ip = Array::New();
				self->Set(NEW_STR(ip),ip);
				count = 0;
				while(host->h_addr_list[count]){
					cs::IP _ip = *((u_long*)host->h_addr_list[count]);

					ip->Set(count,String::New(_ip.ToString()));
					count++;
				}

				self->Set(NEW_STR(official),String::New(host->h_name));
				::WSACleanup();
				return True();
			}
			::WSACleanup();
			return Undefined();
		}
		//*,{
		//	"type":"property",
		//	"name":"official",
		//	"objtype":"string",
		//	"text":"ȡ�������Ĺٷ����ơ�"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"aliases",
		//	"objtype":"array",
		//	"text":"�����ı������飬һ�����������ж��������"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"ip",
		//	"objtype":"array",
		//	"text":"������ ip ���飬һ�����������ж�� ip��"
		//}//*
	public:
		static void init(Handle<FunctionTemplate>& ft){
			HandleScope store;
			Handle<ObjectTemplate> func = ft->PrototypeTemplate();
			SET_CLA_FUNC(create);
		};
	};
	//*],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\host.jsuser"}//*
	void LoadNet(Handle<Object>& glb){
		JsHttp::Load(glb,L"CHttpRequest",TEMPLATE_ID_HTTP);
		JsSocket::Load(glb,L"CSocket",TEMPLATE_ID_SOCKET);
		JsTcpServer::Load(glb,L"CTcpServer",TEMPLATE_ID_TCPSERVER);
		JsUpnp::Load(glb,L"CUpnp",TEMPLATE_ID_UPNP);
		JsHost::Load(glb,L"Host",TEMPLATE_ID_HOST);
	}
	//*]}//*
}