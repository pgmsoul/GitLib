#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	LPCWSTR NULL_STR = L"";
	//*{
	//	"type":"group",
	//	"name":"base",
	//	"text":"jsuser ���û������ܣ���Щ�����Ͷ��������ֶ����ء�",
	//	"member":[//*
	//*{
	//	"type":"const",
	//	"name":"Global",
	//	"text":"Global ��һ��ȫ�ֱ���, ���ж�����������������. ����˵ Global ��Ψһ������ı���, ����ȫ�ֱ��������������Գ�Ա, �ر��, Global �Լ�Ҳ���Լ��ĳ�Ա��",
	//	"remark":[
	//		"JavaScript �ĺ���ʵ���Ϸ�Ϊ������;��һ������ͨ����һ����ֱ�ӵ��ã�����һ�࣬���� new SomeThing() ���ַ�ʽ����������׼ȷ��˵�������ຯ����û�б�������ֻ�����Ǻ����������÷�������ʵ�ʵ��У��������÷��ĺ��������������ȫ��ͬ�ģ�Ҳ����˵����ȫ���Գ�Ϊ���ຯ����",
	//		"����һ���ֱ�ӵ��õĺ���Ϊ������������Сд��ĸ��ͷ�������ڴ�������ĺ���������Ҳ�����ǳ�Ϊ�࣬�Դ�д��ĸ��ͷ��JavaScript ���õ������ຯ������������ new �����ã�Ҳ����ֱ�ӵ��ã������һ���ġ����� <span style=\"color:red;font-weight:bold;\">jsuser ����ຯ������ֱ�ӵ��ã������� new �ķ�ʽ������һ������</span>��"
	//	]
	//}//*
	//*,{
	//	"type":"function",
	//	"name":"alert([text],[title],[iconAndtype],[parent])",
	//	"text":"����һ��ϵͳ�Ի���, ��ʾָ������Ϣ�ı���",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"[text]",
	//			"text":"Ҫ��ʾ����Ϣ, ����������Դ��κ�����, ����������ʾ����������� toString �����ķ���ֵ��"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[title]",
	//			"text":"�Ի������, ȱʡ��Ӧ�ó������ƣ�������ƿ���ͨ�� App.name ���ã���ȱʡֵ��js�ļ������ơ�"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[iconAndtype]",
	//			"text":"�Ի����ͼ��Ͱ�ť����, �����3���ѡ, ÿ�����ѡһ��ֵ:<br/>��һ��, ��ʾͼ��:<br/>alert.ERROR = 0x10;��ȱʡֵ���� 0 ��û�����������ʱ��Ĭ�ϵ�ֵ����<br/>һ��ͨ����ʾ���ִ����ͼ��<br/>alert.ASK = 0x20;<br/>һ���ʺ�ͼ��<br/>alert.WARN = 0x30;<br/>һ����̾��ͼ��<br/>alert.INFO = 0x40;<br/>һ����Ϣͼ��<br/><br/>�ڶ���, ��ʾ��ť����:<br/>alert.OK_CANCEL = 0x1;��<br/>2 ����ť, ȷ��, ȡ��.<br/>alert.ABORT_RETRY_IGNORE = 0x2;<br/>3 ����ť, ��ֹ,����,����<br/>alert.YES_NO_CANCEL = 0x3;<br/>3 ����ť, ��,��,ȡ��<br/>alert.YES_NO = 0x4;<br/>2 ����ť, ��, ��<br/>alert.RETRY_CANCEL = 0x5;<br/>2 ����ť, ����, ȡ��<br/>alert.CANCEL_TRY_CONTINUE = 0x6;<br/>3 ����ť, ȡ��, ����, ����<br/><br/>������, ����ȱʡ��ť:<br/>alert.DEF_BUTTON1 = 0x000;��ȱʡֵ��<br/>��һ����ťȱʡ <br/>alert.DEF_BUTTON2 = 0x100;<br/>�ڶ�����ťȱʡ<br/>alert.DEF_BUTTON3 = 0x200;<br/>��������ťȱʡ"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[parent]",
	//			"text":"�����ڵľ��, alert������һ��ģ̬�Ի���, �ر�ǰ, �޷��л��������ڲ�����"
	//		}
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"�������Ƿ���һ�����������ݲ�ͬ�����ͣ�����ֵ���������£�<br/>alert.OK = 1;�������ȷ����ť��<br/>alert.CANCEL = 2;�������ȡ����ť��<br/>alert.ABORT = 3;���������ֹ��ť��<br/>alert.RETRY = 4;����������԰�ť(ABORT_RETRY_IGNORE�ĵڶ�����ť)��<br/>alert.IGNORE = 5;������˺��԰�ť��<br/>alert.YES = 6;��������ǰ�ť��<br/>alert.NO = 7;������˷�ť��<br/>alert.CLOSE = 8;������˱������رհ�ť��<br/>alert.HELP = 9;������˱�����������ť��<br/>alert.TRYAGAIN = 10;����������԰�ť(CANCEL_TRY_CONTINUE�ĵڶ�����ť)��<br/>alert.CONTINUE = 11;������˼�����ť��"
	//	}
	//}//*
	Handle<Value> alert(const v8::Arguments& args){
		HandleScope store;
		cs::String msg,title;
		UINT type;
		HWND parent;
		GET_ARGS_STR(0,msg);
		if(args.Length()>1){
			if(args[1]->IsUint32()){
				type = GET_ARGS_INT(1,0);
				parent = GET_ARGS_INT2(2,0,HWND);
				title = cs::App::GetAppName();
			}else{
				GET_ARGS_STR(1,title);
				type = GET_ARGS_INT(2,0);
				parent = GET_ARGS_INT2(3,0,HWND);
			}
		}else{
			title = cs::App::GetAppName();
			type = 0;
			parent = 0;
		}
		int r = ::MessageBox(parent,msg,title,type);
		RET_UINT(r);
	}
	//*,{
	//	"type":"function",
	//	"name":"getPageCode(codename)",
	//	"text":"��ȡָ������Ĵ���ֵ��ĳЩ������Ҫ���ֵ��",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"[codename]",
	//			"text":"�������ƣ����� utf-8(��Ҫ��дΪutf8),ansi,gb2312 �ȵȣ��������Ϊ��, ������һ����Ч������, ���� ANSI ���� Ҳ���� 3��"
	//		}
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"����ָ���������Ƶ����ִ��롣"
	//	}
	//}//*
	Handle<Value> getPageCode(const Arguments& args){
		HandleScope store;
		DWORD code;
		if(args.Length()>0){
			cs::String codeName;
			GetString(args[0],codeName);
			code = cs::GetCodePage(codeName);
		}else code = 3;
		return store.Close(Uint32::New(code));
	}
	//*,{
	//	"type":"function",
	//	"name":"run(js,[name])",
	//	"text":"����һ��js�����ִ�, ���ҷ���ִ�н��. ������������� PHP �� require ����, ���ǲ��������ļ���, ���� js �����ִ���",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"js",
	//			"text":"Javascript �ű��ִ���"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[name]",
	//			"text":"����������ڵ���ʱ����ʾ���ļ�����"
	//		}
	//	],
	//	"return":{
	//		"type":"any",
	//		"text":"���ؽű�ִ�н����"
	//	},
	//	"example":[
	//		"run('new Function(a,b){return a+b;}','test');",
	//		"//���н������һ��������"
	//	]
	//}//*
	Handle<Value> loadJsCode(const Arguments& args){
		HandleScope store;
		TryCatch err;

		Handle<String> js;
		Handle<Value> name;
		if(args.Length()>0)
			js = args[0]->ToString();
		if(args.Length()>1)
			name = args[1];
		Local<Script> script = Script::Compile(args[0]->ToString(),name);
		while(!script.IsEmpty()){
			Local<Value> result = script->Run(); 
			if(result.IsEmpty()) break;
			return store.Close(result);
		}
		ReportError(err);
		return Undefined();
	}
	//*,{
	//	"type":"function",
	//	"name":"load(filename,[code])",
	//	"text":"�˺����� run �Ĺ�����ͬ, ֻ����������һ��js�ļ���, ������js����. �������js�ļ�, ���ҷ���ִ�н��. ��������� PHP �� require ������ͬ��",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filename",
	//			"text":"Javascript �ű��ļ����ļ�����"
	//		},
	//		{
	//			"type":"string|integer",
	//			"name":"[codepage]",
	//			"text":"�ļ�ʹ�õı��룬�����Ǳ�������ִ�������ִ����ƣ�ȱʡ�� undefined ʱ�Զ�ʶ���Ƿ��� ansi��3���� utf-8��65001�� ���룬���������޷��Զ�ʶ��"
	//		}
	//	],
	//	"return":{
	//		"type":"any",
	//		"text":"���ؽű�ִ�н����"
	//	}
	//}//*

	Handle<Value> loadJsFile(const Arguments& args){
		HandleScope store;
		while(true){
			if(args.Length()<1) break;
			cs::String file,js,cp;
			GetString(args[0],file);
			DWORD code = -1;
			if(args.Length()>1){
				if(args[1]->IsInt32())
					code = args[1]->Uint32Value();
				else{
					GetString(args[1],cp);
					code = cs::GetCodePage(cp);
				}
			}
			if(!ReadJsFile(js,file,code)){
				cs::String msg;
				msg.Format(L"load(filename) û���ҵ��ļ�: %s",file.Handle());
				ThrowException(NEW_WSTR(msg.Handle()));
				break;
			}
			cs::String cmd;
			if(args.Length()>2)
				GetString(args[2],cmd);
			return LoadJsCode(js,file);
		}
		return Undefined();
	}
	bool _format(cs::String& buf,LPCWSTR fms,...){
		va_list vaList;
		va_start(vaList,fms);
		int bufSize = _vscwprintf(fms,vaList);
		if(bufSize==-1){
			va_end(vaList);
			return 0;
		}
		buf.SetCubage(bufSize+3);
		int n = vswprintf_s(buf,bufSize+1,fms,vaList);
		va_end(vaList);
		return 1;
	}
	//*,{
	//	"type":"function",
	//	"name":"format(fms,[var...])",
	//	"text":"��ʽ���ִ�����������������ִ��ļӷ�ȡ�������� format ����������ֱ�ۡ�",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"fms",
	//			"text":"�� %s ת��ĸ�ʽ���ִ������� 'x=%s,y=%s'��ע�⣺�������ֻ֧�� %s ��ʽ������֧������ת����� %d �ȵȣ����з� %s ��ת���������ᱨ��Ҫ��ʾ ��%�� �ַ�ʹ���������� ��%%����"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[var...]",
	//			"text":"�� fms ������Ӧ�� n ��������n �� fms ���е� %s ת������������������Դ��ڵ��� n �����ǲ���С�� n��"
	//		}
	//	],
	//	"return":{
	//		"type":"string",
	//		"text":"һ���ִ���"
	//	},
	//	"example":[
	//		"var str = format('x=%s,y=%s',3,5);",
	//		"alert(str);//��ʾ��x=3,y=5����"
	//	]
	//}//*
	Handle<Value> format(const Arguments& args){
		HandleScope store;
		cs::String str[10],buf;

		while(true){
			if(args.Length()==0) return Undefined();
			if(args.Length()>=1&&!args[0]->IsString()) return Undefined();
			cs::String fms;
			GET_ARGS_STR(0,fms);
			for(int i=1;i<args.Length();i++){
				GET_ARGS_STR(i,str[i-1]);
			}
			if(args.Length()==1){
				buf = fms;break;
			}else if(args.Length()==2){
				_format(buf,fms,str[0].Handle());break;
			}else if(args.Length()==3){
				_format(buf,fms,str[0].Handle(),str[1].Handle());break;
			}else if(args.Length()==4){
				_format(buf,fms,str[0].Handle(),str[1].Handle(),str[2].Handle());break;
			}else if(args.Length()==5){
				_format(buf,fms,str[0].Handle(),str[1].Handle(),str[2].Handle(),str[3].Handle());break;
			}else if(args.Length()==6){
				_format(buf,fms,str[0].Handle(),str[1].Handle(),str[2].Handle(),str[3].Handle(),str[4].Handle());break;
			}else if(args.Length()==7){
				_format(buf,fms,str[0].Handle(),str[1].Handle(),str[2].Handle(),str[3].Handle(),str[4].Handle(),str[5].Handle());break;
			}else if(args.Length()==8){
				_format(buf,fms,str[0].Handle(),str[1].Handle(),str[2].Handle(),str[3].Handle(),str[4].Handle(),str[5].Handle(),str[6].Handle());break;
			}else if(args.Length()==9){
				_format(buf,fms,str[0].Handle(),str[1].Handle(),str[2].Handle(),str[3].Handle(),str[4].Handle(),str[5].Handle(),str[6].Handle(),str[7].Handle());break;
			}else if(args.Length()==10){
				_format(buf,fms,str[0].Handle(),str[1].Handle(),str[2].Handle(),str[3].Handle(),str[4].Handle(),str[5].Handle(),str[6].Handle(),str[7].Handle(),str[8].Handle());break;
			}else if(args.Length()>=11){
				_format(buf,fms,str[0].Handle(),str[1].Handle(),str[2].Handle(),str[3].Handle(),str[4].Handle(),str[5].Handle(),str[6].Handle(),str[7].Handle(),str[8].Handle(),str[9].Handle());break;
			}
			break;
		}
		return store.Close(String::New((uint16_t*)buf.Handle()));
	}
	Handle<Value> debug(const Arguments& args){
		Handle<Value> str = format(args);
		cs::String s;
		GetString(str,s);
		cs::Print(L"%s",s);
		return Undefined();
	}
	//*,{
	//	"type":"function",
	//	"name":"startMsgLoop([qMsg])",
	//	"text":"������Ϣѭ����startMsgLoop ����ִ�к��һֱ���У�������ִ�����������䣬ֱ�������� exitMsgLoop ���������ڳ���������� startMsgLoop �������������������˳������޷���Ӧ�û�������",
	//	"param":[
	//		{
	//			"type":"boolean",
	//			"name":"[qMsg]",
	//			"text":"������Ϣѭ�������࣬�Ƿ��ǷǴ�����Ϣѭ����Ĭ���� false��"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	},
	//	"remark":[
	//		"startMsgLoop �ڲ�����������Ϣѭ����������ͷǴ����ࡣ�Ǵ��������Ϣѭ��Ч�ʸ��ߣ����ǷǴ�������Ϣѭ������û��ʹ���κδ��ڵ�ʱ����ã�������Ҫ�����˶�Ӧ�Ļ��ƣ�������ʧ���������ء�",
	//		"��һ��Ӧ�ó����д��ڵ�ʱ�򣬹ر������ڻ��Զ����� exitMsgLoop ���ƣ��Ӷ��˳�Ӧ�ó������Ӧ�ó���û�д��ڣ�������Լ�������������� exitMsgLoop��������򱻹�����Զ�����˳���"
	//	],
	//	"example":[
	//		"setTimer(function(){",
	//		"	//����ÿ 3 �����һ�Σ�һ�� alert ����ֹ�����������ֱ���û��ر��˶Ի��򣬵�����������ֹ Timer �ĵ��ã��Ի��������ĵ�����",
	//		"	alert(\"click to call exitMsgLoop\");",
	//		"	//�˳���Ϣѭ����startMsgLoop ���أ������˳���",
	//		"	exitMsgLoop();",
	//		"},3000);",
	//		"startMsgLoop();"
	//	]
	//}//*
	Handle<Value> startMsgLoop(const Arguments& args){
		bool qMsg = GET_ARGS_VALUE(0,false,Boolean);
		_env->queue.Start(qMsg);
		return Undefined();
	}
	//*,{
	//	"type":"function",
	//	"name":"exitMsgLoop()",
	//	"text":"�˳���Ϣѭ����ִ������������� startMsgLoop �������ء�",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	}
	//}//*
	Handle<Value> exitMsgLoop(const Arguments& args){
		_env->queue.Close();
		return Undefined();
	}
	//*,{
	//	"type":"function",
	//	"name":"Class(proc,global)",
	//	"text":"Class ��һ������������һ�����������������Ϊ class �Ѿ���Ϊ�����֣����Ը�Ϊ Class��Class ��װһ����������������Զ����� Stack��Stack ���ں����˳�ʱִ��ջ�ڶ���� dispose ������",
	//	"param":[
	//		{
	//			"type":"function",
	//			"name":"func",
	//			"text":"Ҫ��װ�ĺ�����"
	//		},
	//		{
	//			"type":"boolean",
	//			"name":"[global]",
	//			"text":"��������Ƿ�ȫ�ֿɼ�����������������Ϊĳ������ĳ�Ա���������Դ� false��Ĭ��ֵ�� false��"
	//		}
	//	],
	//	"return":{
	//		"type":"function",
	//		"text":"����һ����װ��ĺ�����"
	//	},
	//	"remark":[
	//		"C ��ͷ�� jsuser �����ڲ�����һ�� C++ ������Ȼ JavaScript �������ռ����ܹ��ڶ��������ʱ���Զ�������� C++ ���󣬵��� C++ ��������ʹ����ϵͳ��Դ����Щ��Դ�����Ƿǳ����޵ģ����������ڶ࣬Ӧ�þ�����ͷ���Щ��Դ������ Javascript û��ǿ���ͷŶ���Ļ��ƣ�����ʹ�� Class ��ģ��������ܣ��������뿪�����ķ�Χ֮��dispose ���Զ����ã��� C ��ͷ�� jsuser ������˵��dispose ��ִ�У���ζ���ڲ��� C++ �����ͷ��ˣ��������Ϊһ������� Javascript �����������ռ����Զ�����һ�������ڲ���Դ�� Jsuser ���������� C ��ͷ���������� new ��λ���������� Stack �ĺ�����Χ����Ч�����ұ����� Stack ����������"
	//	],
	//	"example":[
	//		"//ʾ��1��",
	//		"Class(function test(){",
	//		"	var file = new CFile();//file �� dispose ������ test ִ����ɺ�ᱻ�Զ�ִ�С�",
	//		"	var obj = {};",
	//		"	obj.dispose = function(){",
	//		"		alert(\"obj.dispose called\");",
	//		"	}",
	//		"	Stack.push(obj);//obj �� dispose �����ں���ִ�н�����ᱻ�Զ����У��� C ��ͷ�� Jsuser �������ʹ�� Stack ������Ҫ�ֶ���ӽ� Stack������һ��������ô������Ϊ���ǲ���Ҫ������Դ�����������Ҫ������Ҳ����ʹ�� Stack ���ơ�",
	//		"},true);//ȫ�ֿɼ���",
	//		"test();",
	//		"//ʾ��2��",
	//		"function main(){",
	//		"	var file = new CFile();//ִ����һ���ʱ��������Ϊ main ������������ĺ���û������ Stack��",
	//		"}",
	//		"main();",
	//		"//ʾ��3��",
	//		"Class(function main(){",
	//		"	func();",
	//		"},true)",
	//		"function func(){",
	//		"	var file = new CFile();//���������Ϊ main �����Ѿ����� Stack������ֱ�� main ����������file �Żᱻ�����ﲻ����ʱ�����Ŀ�ġ�",
	//		"}",
	//		"main();"
	//	]
	//}//*

	//*,{
	//	"type":"function",
	//	"name":"require(lib,[compnent])",
	//	"text":"���ؿ�������",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"lib",
	//			"text":"������ƣ����� base��wnd �ȡ�"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[compnent]",
	//			"text":"���Ӧ������������ö��ŷָ������ض�����������˲���ȱʡ���������ؿ��ȫ�������"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	},
	//	"remark":[
	//		"���ؿ��ȫ�������ռ��ϵͳ�ڴ���Դ����ռ�� CPU ��Դ�����¼���ʱ�����ӣ����Գ����Ż��Ŀ��ǣ��ɲ�����û���õ��������",
	//		"����ͨ�����ĵ����鿴һ��������Щ�����"
	//	],
	//	"example":[
	//		"require('base','file');//�����ļ���������������������ء�",
	//		"require('base');//base ���ȫ��������ᱻ���ء�"
	//	]
	//}//*
	//*,{
	//	"type":"const",
	//	"name":"FileSystem",
	//	"text":"�˳��������ṩ�ļ�ϵͳ����ز�������ճ�������ơ�ɾ���ȡ�",
	//	"member":[//*
	class JsApp : public JsConst<JsApp>{
	public:
		//*{
		//	"type":"property",
		//	"name":"name",
		//	"objtype":"string",
		//	"text":"Ӧ�ó�������ƣ�Ĭ���ǽű��ļ�������������׺��������ƻᱻ�ܶ��ڲ�����ʹ�ã����Խ����޸��������������ļ���Ĭ���ļ����ƣ����ܶຯ��ʹ���������ļ��������̬�ĸı���������ƣ�������ɼ��ز�ͬ�������ļ���"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"folder",
		//	"objtype":"string",
		//	"text":"Ӧ�ó����Ĭ��Ŀ¼������ʼ������Ϊ�ű����ڵ�Ŀ¼�������ı�����ֵ��"
		//}//*
		static inline void set(cs::String& name,Local<Value>& value,Local<Object>& self){
			if(name==L"name"){
				cs::String str;
				GetString(value,str);
				cs::App::SetAppName(str);
			}else if(name==L"folder"){
				cs::String str;
				GetString(value,str);
				if(SetCurrentDirectory(str)){
					cs::App::SetAppDirectory(str);
				}
			}
		}
		static inline Handle<Value> get(cs::String& name,Local<Object>& self){
			if(name==L"name"){
				return NEW_WSTR(cs::App::GetAppName());
			}else if(name==L"folder"){
				return NEW_WSTR(cs::App::GetAppDirectory());
			}
			return Undefined();
		}
		static inline void init(Handle<Object>& obj){
			SET_OBJ_ACCESSOR(obj,name);
			SET_OBJ_ACCESSOR(obj,folder);
		}
	};
	//*]}//*

	//*,{
	//	"type":"function",
	//	"name":"setConfig(callback,[filename])",
	//	"text":"��ȡ�����������ļ��������ļ���һ�� Json ��ʽ���ı��ļ��������� utf-8 ��ȡ ansi ���롣",
	//	"param":[
	//		{
	//			"type":"function",
	//			"name":"callback(cfg)",
	//			"text":"setConfig ��������ɹ����ᴥ������ص�������cfg ��һ�������������Ծ����������ݣ������������κθ�����󶼻���Ϊ Json ��ʽ���浽�����ļ���"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[filename]",
	//			"text":"�����ļ�����ȱʡ�Ǻͽű�ͬ·��ͬ����һ����׺Ϊ��.json�����ı��ļ������ָ�����ļ��������ȡ�ͱ����Ӧ���ļ���"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"����ɹ����������ļ����������� true�����򷵻� undefined��"
	//	}
	//}//*
	Handle<Value> setConfig(const Arguments& args){
		HandleScope stack;
		while(true){
			if(args.Length()<1) break;
			if(!args[0]->IsFunction()) break;
			cs::String file;
			if(args.Length()>1)
				GetString(args[1],file);
			cs::Config cfg(file);
			cs::Json* json = cfg.Lock();
			if(!json) break;

			json->ToString(file,false);
			Handle<Object> glb = GetGlobal();
			Handle<Object> JSON = glb->Get(NEW_STR(JSON))->ToObject();
			Handle<Function> parse = Handle<Function>::Cast(JSON->Get(NEW_STR(parse)));
			Handle<Function> stringify = Handle<Function>::Cast(JSON->Get(NEW_STR(stringify)));
			Handle<Function> callback = Handle<Function>::Cast(args[0]);
			Handle<Value> argv[3];
			argv[0] = NEW_WSTR(file.Handle());
			Handle<Value> v = parse->Call(JSON,1,argv);
			if(v.IsEmpty()||!v->IsObject()) v = Object::New();
			argv[0] = v;
			CallFunc(glb,callback,1,argv);
			v = stringify->Call(JSON,1,argv);
			GetString(v,file);
			json->Parse(file);
			return True();
		}
		return Undefined();
	}
	void LoadBase(){
		HandleScope store;

		Handle<Object> obj = GetGlobal();
		SET_OBJ_FUNC_RO(obj,alert,alert);
		SET_OBJ_FUNC_RO(obj,getPageCode,getPageCode);
		SET_OBJ_FUNC_RO(obj,require,require);
		SET_OBJ_FUNC_RO(obj,run,loadJsCode);
		SET_OBJ_FUNC_RO(obj,load,loadJsFile);
		SET_OBJ_FUNC_RO(obj,format,format);
		//SET_OBJ_FUNC_RO(obj,log,debug);
		SET_OBJ_FUNC_RO(obj,startMsgLoop,startMsgLoop);
		SET_OBJ_FUNC_RO(obj,exitMsgLoop,exitMsgLoop);
		SET_OBJ_FUNC_RO(obj,setConfig,setConfig);

		JsApp::Load(obj,L"App");

		obj->Set(String::New("Global"),obj,ReadOnly);
	}
	//*,{
	//	"type":"class",
	//	"name":"Api",
	//	"text":"Api����Ժ����� C ��д�Ķ�̬���ӿ�ĵ�������. Windowsϵͳ��Api�������������2���汾, \"A\"��\"W\", �����ȡ�����ı��ĺ��� GetWindowText, ��������GetWindowTextA��GetWindowTextW��������, C++��������ȫ������, ����Unicode����ANSI��ȷ��ʹ���Ǹ��汾�ĺ���. ���� C++ �����е� Api ����һ��ֻ��Ҫд������, ����Ҫ���� A �� W ��׺. ����ʵ���ϲ�û�� GetWindowText �������, ������ʹ�� Api ���ʱ��, ����ָ��ʹ���Ǹ��汾�ĺ���. ��Ȼ, �����ϲ�Ҫʹ�� A �汾�ĺ���, ��Ϊ A �汾�������ִ���������ֱ�Ӵ� JavaScript �ִ�, ��Ҫ�� CMemory ������ģ��. <b>�������еĺ������� A �� W �����汾</b>, ������ĺ���ֻ�� W �汾����ֻ�� A �汾, ���������Ҫ���� MSDN �ĵ���",
	//	"remark":[
	//		"ʹ�� Api �����ϵͳ Api ��ʱ��, ����ֻ��3��: ����, �ִ�, CMemory. ���� Api ������Ҫָ����������ֵĲ���, ȫ��������; ���ǲ������ִ������, �� JavaScript �ִ�; �������ʹ�� CMemory ģ��. CMemory ����������ģ���κβ���, ��Ϊ�����ڴ����, �����������ִ���",
	//		"Api��ֻ�ܴ������10��������Api, ��ʵ�ϳ���10������Api����. ���е�ʱ��, ����ʹ�úͲ�������Ӧ��call, ����ĺ��лᵼ�³��������",
	//		"ָ�뱾���Ͼ�������, ��������ָ�����͵Ĳ��������������������ݡ�����ָ���ָ�룬���߸����ص�ָ�룬��Ҫ�� CMemory ��ģ�⣬��Ϊָ���ָ����ܻᱻ����д��ָ��λ�õ����ݡ�",
	//		"Api������C��ͷ����, ����Ҫ���� dispose �ͷ� C++ ��Դ��",
	//		"���� Api ����ķ���ֵ���� C �����ķ���ֵ�����������Ǳ���Ϊ��һ����������� C ��������ֵ��ת��Ϊһ�� 32 λ�������򷵻�ֵ�����á�"
	//	],
	//	"example":[
	//		"//���� user32.dll��",
	//		"var user32 = Api.loadModule(\"user32.dll\");",
	//		"if(!user32) throw \"load user32.dll error\";",
	//		"//��ȡ MessageBox API ������",
	//		"var MessageBox = new Api();",
	//		"if(!MessageBox.create(user32,\"MessageBoxW\")) ",
	//		"	throw \"get MessageBox error\";",
	//		"//���� MessageBox API ������4 ���������붼��������ȱʡ��",
	//		"MessageBox(0,\"Hello!\",\"OK\",0x40);",
	//		"//ж�� user32.dll��",
	//		"Api.freeModule(user32);"
	//	],
	//	"member":[
	//	{
	//		"type":"function",
	//		"name":"loadModule(dll)",
	//		"text":"����һ�� dll��ע�⣬��������� Api �ľ�̬�����������ǳ�Ա��������Ҫ Api.loadModule(dll) �������������� Api ����������С�",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"dll",
	//				"text":"Ҫ���ص� dll����� dll ����ڵ�ǰĿ¼����ϵͳĿ¼������ֱ��ʹ�� dll ���ļ�����������봫����·�������·����PATH ����������� dll �ǲ����Զ�ʶ��ġ�"
	//			}
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"�ɹ�����һ�� dll ģ������ʧ�ܷ��� undefined��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"freeModule(module)",
	//		"text":"ж��һ���Ѿ����ص� dll ģ�飬ע�⣬��������� Api �ľ�̬�����������ǳ�Ա��������Ҫ Api.freeModule(dll) �������������� Api ����������С�",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"module",
	//				"text":"loadModule �����ɹ����� dll ��ķ���ֵ��һ��ģ������"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"�ɹ����� true��ʧ�ܷ��� false��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"create(module,funcname)",
	//		"text":"����ָ��ģ��ĺ�����",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"module",
	//				"text":"loadModule �����ɹ����� dll ��ķ���ֵ��һ��ģ������"
	//			},
	//			{
	//				"type":"string",
	//				"name":"funcname",
	//				"text":"�������ơ�"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"�ɹ����� true��ʧ�ܷ��� false��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isEmpty()",
	//		"text":"�����Ƿ�Ϊ�գ����Ƿ��Ѿ��ɹ����� create ������",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"�ɹ����� true��ʧ�ܷ��� false��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"reset()",
	//		"text":"���ö���Ϊ�գ���δ���� create ʱ��״̬��",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	}
	//]}//*


	//*,{
	//	"type":"const",
	//	"name":"Console",
	//	"text":"����̨��ز�����",
	//	"remark":[
	//		"����̨��Ȼ�Ǵ��ڣ�����ʹ�ÿ���̨���㴰�ڳ�������һ������Ĵ��ڡ�"
	//	],
	//	"member":[
	//{
	//	"type":"function",
	//	"name":"show()",
	//	"text":"��ʾ����̨���ڡ�",
	//	"return":{
	//		"type":"boolean",
	//		"text":"�ɹ����� true��ʧ�ܷ��� undefined��"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"close()",
	//	"text":"�رտ���̨���ڡ�",
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"write(text)",
	//	"text":"���ı����������̨���ڡ�",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"text",
	//			"text":"�������"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"log(text)",
	//	"text":"���ı����������̨���ڣ����һ��С�",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"text",
	//			"text":"�������"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"setColor(color)",
	//	"text":"������������ı�����ɫ��",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"color",
	//			"text":"��ɫֵ��ֻ֧�� 8(0--7) ����ɫ��<br/> 0����ɫ��<br/> 1������ɫ��<br/> 2������ɫ��<br/> 3������ɫ(1|2)��<br/> 4������ɫ��<br/> 5����ź��ɫ(1|4)��<br/> 6������ɫ(2|4)��<br/> 7������ɫ(1|2|4)��<br/>��ֻӰ�����������ǰ���������ɫ����ı䡣"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"readln()",
	//	"text":"��ȡ����̨���ڵ����룬������������Ժ󣬳�����𣬿���̨�ȴ��û����룬�û�����س����󣬺������ء�",
	//	"return":{
	//		"type":"string",
	//		"text":"�����û�������ı���"
	//	}
	//}
	//],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\console.jsuser"}//*

	//*,{
	//	"type":"function",
	//	"name":"log([text],[color])",
	//	"text":"���ı��������־���ڡ�",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"[text]",
	//			"text":"Ҫ������ı�������˲���ȱʡ�����һ�����С�"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[color]",
	//			"text":"�ı�����ɫ��ȱʡ�ǰ�ɫ��"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"����û�з���ֵ��"
	//	},
	//	"remark":[
	//		"���������һ��������ʾ��־��Ϣ�Ĵ��ڣ�������Ӧ�û������룬���ǿ��Կ��������С��������ɫ��������ɫ����־�������Լ�����Ϣѭ�������������Ƿ��Ǵ��ڳ����޹ء�"
	//	],
	//	"member":[
	//{
	//	"type":"function",
	//	"name":"show([exit])",
	//	"text":"��ʾ��־���ڡ�",
	//	"param":[
	//		{
	//			"type":"boolean",
	//			"name":"exit",
	//			"text":"���û��ر���־����ʱ���Ƿ��˳�����ȱʡ�� true��"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"û�з���ֵ��"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"close()",
	//	"text":"������־���ڣ�������־���ᱻ��ա�",
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setMaxLine([count])",
	//	"text":"������־���������ı������������־�ﵽ���������ǰ�����־�ᱻ������",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"[count]",
	//			"text":"���ܵ����������ȱʡ�� 1000"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setFontSize(size)",
	//	"text":"��������ı��������С����������ǻ�Ӱ�������ı���",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"size",
	//			"text":"����ĳߴ磬ȱʡ�� 16."
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"setBkColor(color)",
	//	"text":"����������ڵı���ɫ��ȱʡ�Ǻ�ɫ��",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"color",
	//			"text":"��ɫֵ��"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"clear()",
	//	"text":"���������־��",
	//	"return":{
	//		"type":"void",
	//		"text":"�������û�з���ֵ��"
	//	}
	//}
	//],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\logwnd.jsuser"}//*
	void RectJsFromC(v8::Handle<Value> value,cs::Rect* r){
		HandleScope store;
		if(!value->IsObject()) return;
		Local<Object> vObj = value->ToObject();
		vObj->Set(String::New("left"),Int32::New(r->left));
		vObj->Set(String::New("top"),Int32::New(r->top));
		vObj->Set(String::New("right"),Int32::New(r->right));
		vObj->Set(String::New("bottom"),Int32::New(r->bottom));
	}
	//*,{
	//	"type":"class",
	//	"name":"Point",
	//	"text":"Point ������һ���� Javascript ���󣬰��������������ԡ�Size �� Point ��ͬһ��������������ơ�",
	//	"member":[
	//	{
	//		"type":"property",
	//		"name":"x",
	//		"objtype":"integer",
	//		"text":"x ���ԣ�һ���ʾ������λ�û��ȡ�"
	//	},
	//	{
	//		"type":"property",
	//		"name":"y",
	//		"objtype":"integer",
	//		"text":"y ���ԣ�һ���ʾ������λ�û�߶ȡ�"
	//	},
	//	{
	//		"type":"function",
	//		"name":"move(dx,dy)",
	//		"text":"���� x �� y ���Ե�ֵ��",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"dx",
	//				"text":"x ����������ֵ��"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"dy",
	//				"text":"y ����������ֵ��"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	}
	//	]
	//}
	//,{
	//	"type":"class",
	//	"name":"Rect",
	//	"text":"Rect ������һ���� Javascript ���󣬰����ĸ��������ԡ�",
	//	"member":[
	//	{
	//		"type":"property",
	//		"name":"left",
	//		"objtype":"integer",
	//		"text":"��߽硣"
	//	},
	//	{
	//		"type":"property",
	//		"name":"top",
	//		"objtype":"integer",
	//		"text":"�ϱ߽硣"
	//	},
	//	{
	//		"type":"property",
	//		"name":"right",
	//		"objtype":"integer",
	//		"text":"�ұ߽硣"
	//	},
	//	{
	//		"type":"property",
	//		"name":"bottom",
	//		"objtype":"integer",
	//		"text":"�±߽硣"
	//	},
	//	{
	//		"type":"function",
	//		"name":"ptInRect(x,[y])",
	//		"text":"(x,y)������ĵ��Ƿ�����������ڣ�������� y ��������ʾ������ y��ֻ���� x��",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"x",
	//				"text":"��� x ���ꡣ"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[y]",
	//				"text":"��� y ���ꡣ"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"������ھ����ڷ��� true�����򷵻� false��"
	//		},
	//		"remark":[
	//			"���ε���߽���ϱ߽��ϵĵ�������ڲ��������ұ߽���±߽粻������ڲ���"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"getLocation()",
	//		"text":"��ȡ���ε�λ�ã�Ҳ�������Ͻǵ����ꡣ",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"Point",
	//			"text":"����һ�� Point ����"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setLocation([x],[y])",
	//		"text":"���þ��ε�λ�ã����������ı���� 4 �����Ե�ֵ�����Ǳ��ִ�С���䡣",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"[x]",
	//				"text":"��λ�õ� x ���꣬�����ֵΪ undefined�����ε� left �� right ���䡣"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[y]",
	//				"text":"��λ�õ� y ���꣬�����ֵΪ undefined�����ε� top �� bottom ���䡣"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"����û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setSize",
	//		"text":"���þ��εĴ�С���������ͨ������ right �� bottom ���������εĴ�С��",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"[width]",
	//				"text":"�����µĿ�ȣ������ֵΪ undefined�����εĿ�Ȳ��䡣"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[height]",
	//				"text":"�����µĸ߶ȣ������ֵΪ undefined�����εĸ߶Ȳ��䡣"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"�������û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"getSize",
	//		"text":"��ȡ���εĴ�С��",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"Size",
	//			"text":"����һ�� Size ����"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"positive",
	//		"text":"ͨ������ left �� right ���� top �� bottom ��ֵ����֤��left ����С�ڵ��� right��top ����С�ڵ��� bottom��",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"�������û�з���ֵ��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setValue(left,top,right,bottom)",
	//		"text":"���þ��ε� 4 �����ԣ��������ֱ�����ã������κμ�⡣",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"left",
	//				"text":"Ҫ���õ� left ����ֵ��"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"top",
	//				"text":"Ҫ���õ� top ����ֵ��"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"right",
	//				"text":"Ҫ���õ� right ����ֵ��"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"bottom",
	//				"text":"Ҫ���õ� bottom ����ֵ��"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"�������û�з���ֵ��"
	//		}
	//	}
	//	]
	//}//*
	bool RectJsToC(v8::Handle<Value> value,cs::Rect* r){
		HandleScope store;
		if(!value->IsObject()) return false;
		Local<Object> vObj = value->ToObject();
		Local<Value> vl = vObj->Get(String::New("left"));
		if(vl->IsUndefined()) return false;
		Local<Value> vt = vObj->Get(String::New("top"));
		if(vt->IsUndefined()) return false;
		Local<Value> vr = vObj->Get(String::New("right"));
		if(vr->IsUndefined()) return false;
		Local<Value> vb = vObj->Get(String::New("bottom"));
		if(vb->IsUndefined()) return false;
		r->left = vl->Int32Value();
		r->top = vt->Int32Value();
		r->right = vr->Int32Value();
		r->bottom = vb->Int32Value();
		return true;
	}
	//Twin
	void PointJsFromC(v8::Handle<Value> value,cs::Twin* r){
		HandleScope store;
		if(!value->IsObject()) return;
		Local<Object> vObj = value->ToObject();
		vObj->Set(String::New("x"),Int32::New(r->x));
		vObj->Set(String::New("y"),Int32::New(r->y));
	}
	bool PointJsToC(v8::Handle<Value> value,cs::Twin* r){
		HandleScope store;
		if(!value->IsObject()) return false;
		Local<Object> vObj = value->ToObject();
		Local<Value> vx = vObj->Get(String::New("x"));
		if(vx->IsUndefined()) return false;
		Local<Value> vy = vObj->Get(String::New("y"));
		if(vy->IsUndefined()) return false;
		r->x = vx->Int32Value();
		r->y = vy->Int32Value();
		return true;
	}
	Handle<Value> NewPoint(int x,int y){
		HandleScope store;
		Handle<Value> args[2];
		args[0] = Int32::New(x);
		args[1] = Int32::New(y);
		Handle<Value> r = _env->GetFunc(FUNC_ID_POINT)->CallAsConstructor(2,args);
		return store.Close(r);
	}
	Handle<Value> NewSize(int x,int y){
		HandleScope store;
		Handle<Value> args[2];
		args[0] = Int32::New(x);
		args[1] = Int32::New(y);
		Handle<Value> r = _env->GetFunc(FUNC_ID_SIZE)->CallAsConstructor(2,args);
		return store.Close(r);
	}
	Handle<Value> NewColor(int red,int green,int blue,int alpha){
		HandleScope store;
		Handle<Value> args[4];
		args[0] = Int32::New(red);
		args[1] = Int32::New(green);
		args[2] = Int32::New(blue);
		args[3] = Int32::New(alpha);
		Handle<Value> r = _env->GetFunc(FUNC_ID_COLOR)->CallAsConstructor(4,args);
		return store.Close(r);
	}
	Handle<Value> NewRect(int left,int top,int right,int bottom){
		HandleScope store;
		Handle<Value> args[4];
		args[0] = Int32::New(left);
		args[1] = Int32::New(top);
		args[2] = Int32::New(right);
		args[3] = Int32::New(bottom);
		Handle<Value> r = _env->GetFunc(FUNC_ID_RECT)->CallAsConstructor(4,args);
		return store.Close(r);
	}
	void LoadLibStruct(Handle<Object>& glb){
		HandleScope store;

		Handle<Function> point = GetObjProperty<Function>(glb,L"Point");
		_env->SetFunc(point,FUNC_ID_POINT);

		Handle<Function> size = GetObjProperty<Function>(glb,L"Size");
		_env->SetFunc(size,FUNC_ID_SIZE);

		Handle<Function> rect = GetObjProperty<Function>(glb,L"Rect");
		_env->SetFunc(rect,FUNC_ID_RECT);

		Handle<Function> color = GetObjProperty<Function>(glb,L"Color");
		_env->SetFunc(color,FUNC_ID_COLOR);

		Handle<Object> Stack = GetObjProperty<Object>(glb,L"Stack");

		Handle<Function> stack_push = GetObjProperty<Function>(Stack,L"push");
		_env->SetFunc(stack_push,FUNC_ID_STACK_PUSH);

		Handle<Function> stack_pop = GetObjProperty<Function>(Stack,L"pop");
		_env->SetFunc(stack_pop,FUNC_ID_STACK_POP);
	}
	////////////////////////////////////////////////
	JsEnv* _env = 0;
	JsEnv::JsEnv(){
		cont = new Persistent<Context>();
		//���¿ռ��С��ʵ���������
		FuncList.Expand(FUNC_ID_MAX);
		TemplateList.Expand(TEMPLATE_ID_MAX);
	}
	JsEnv::~JsEnv(){
		for(uint i=0;i<FuncList.Count();i++){
			FuncList[i].Dispose();
		}
		for(uint i=0;i<TemplateList.Count();i++){
			TemplateList[i].Dispose();
		}
		delete cont;
	}
	void EnterJs(){
		HandleScope store;
		int rt = -1;
		if(_env!=0) return;
		_env = new JsEnv;
		*_env->cont = Context::New(NULL,ObjectTemplate::New());
		(*_env->cont)->Enter();
		Handle<Object> glb = GetGlobal();

		LoadBase();
		LoadConsole(glb);
		LoadAPICall(glb);
		//��ȫ�����stackֻ������.
		glb->Set(String::New("Stack"),Object::New(),ReadOnly);
		//���ػ�����JS���ļ�
		cs::ResID rid(IDR_JS_BASE);
		LoadJsRes(rid,L"IDR_JS_BASE");
		//����JS������Ĵ�js�����, ��C++ʹ��.
		LoadLibStruct(glb);
	}
	void ExitJs(){
		(*_env->cont)->Exit();
		(*_env->cont).Dispose();
		delete _env;
		_env = 0;
	}
	bool Queue::PostCallback(QECALL proc,void* param){
		if(!_wait.IsNull()) _wait.Wait();
		if(_handle&&_handle!=INVALID_HANDLE_VALUE){
			PostQueuedCompletionStatus(_handle,(DWORD)(UINT_PTR)proc,(ULONG_PTR)param,(LPOVERLAPPED)0);
		}else{
			PostThreadMessage(_thid,WM_QUEUE,(WPARAM)(UINT_PTR)proc,(LPARAM)param);
		}
		return true;
	}
	BOOL __stdcall EnumThreadWndProc(HWND hwnd,LPARAM lParam){
		return 0;
	}
	void Queue::Start(bool qMsg){
		//BOOL b = EnumThreadWindows(_thid,&EnumThreadWndProc,0);
		if(!qMsg){
			_thid = ::GetCurrentThreadId();
			_wait.Signal();
			_wait.Close();
			MSG msg;
			while(::GetMessage(&msg,0,0,0)){
				HandleScope store;
				TryCatch err;
				if(msg.message==WM_QUEUE){
					QECALL proc = (QECALL)(UINT_PTR)msg.wParam;
					void* param = (void*)msg.lParam;
					proc(param);
				}else{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if(err.HasCaught())
					ReportError(err);
			}
			return;
		}

		if(_handle==0||_handle==INVALID_HANDLE_VALUE){
			_handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,1);
			_wait.Signal();
			_wait.Close();
		}
		DWORD id;
		ULONG_PTR data;
		LPOVERLAPPED ol;
		while(GetQueuedCompletionStatus(_handle,&id,(PULONG_PTR)&data,&ol,INFINITE)){
			if(data==0) break;
			HandleScope store;
			TryCatch err;
			QECALL proc = (QECALL)(UINT_PTR)id;
			void* param = (void*)data;
			proc(param);
			if(err.HasCaught())
				ReportError(err);
		}
		::CloseHandle(_handle);
		_handle = 0;
	}

	int ReportError(TryCatch& try_catch){
		cs::String str;
		HandleScope handle_scope;
		v8::String::Value exception(try_catch.Exception());
		const wchar_t* exception_string = ToCString(exception);
		Handle<v8::Message> message = try_catch.Message();
		if (message.IsEmpty()) {
			str.Format(L"%s\n",exception_string);
		} else {
			cs::String buf;
			v8::String::Value filename(message->GetScriptResourceName());
			const wchar_t* filename_string = ToCString(filename);
			int linenum = message->GetLineNumber();
			buf.Format(L"file:\t%s\r\nline:\t%i\r\n%s\r\n\r\n", filename_string, linenum, exception_string);
			str += buf;
			v8::String::Value sourceline(message->GetSourceLine());
			const wchar_t* sourceline_string = ToCString(sourceline);
			buf.Format(L"%s", sourceline_string);
			int start = message->GetStartColumn();
			for (int i = 0; i < start; i++) {
				//str += L" ";
			}
			buf.Insert('^',start);
			buf.Trim();
			str += buf;
			int end = message->GetEndColumn();
			for (int i = start; i < end; i++) {
				//str += L"^";
			}
			/*str += L"\n";
			v8::String::Value stack_trace(try_catch.StackTrace());
			if (stack_trace.length() > 0) {
				const wchar_t* stack_trace_string = ToCString(stack_trace);
				buf.Format(L"%s\n", stack_trace_string);
				str += buf;
			}*/
		}
		return MessageBox(0,str,L"Error",MB_ICONERROR);
	}

	int Alert(Handle<Value> msg,MSG_TYPE mt){
		HandleScope store;
		v8::String::Value v8s(msg);//Value��ת������, ������Unicode��ʼ���ִ�.
		return MessageBox(0,(LPCWSTR)*v8s,L"info",mt);
	}
	int Alert(LPCWSTR msg,MSG_TYPE mt){
		return MessageBox(0,msg,L"info",mt);
	}
	//����һ��ֻ������ΪJS����, func ��Js���������, ������function(){����Ĵ���};
	void SetJsFunc(Handle<ObjectTemplate> obj,LPCWSTR name,LPCWSTR func){
		cs::String str;
		str.Format(L"new Function(\"%s\");",func);
		obj->Set(String::New((uint16_t*)name),LoadJsCode(str),ReadOnly);
	}
	//C++�汾��loadJSCode
	Handle<Value> LoadJsCode(LPCWSTR cjs,LPCWSTR fn){
		if(cjs==0) return Undefined();
		if(fn==0) fn = NULL_STR;
		HandleScope store;
		Handle<String> js = String::New((uint16_t*)cjs);
		Handle<Value> name = String::New((uint16_t*)fn);
		TryCatch err;

		Local<Script> script = Script::Compile(js,name);
		while(!script.IsEmpty()){
			Local<Value> result = script->Run(); 
			if(err.HasCaught()) break;//�������жϷ�ʽ�����ǵȼ۵ġ�
			//if(result.IsEmpty()) break;
			return store.Close(result);
		}
		ReportError(err);
		return Undefined();
	}
	Handle<Value> CallFunc(Handle<Object>& self,Handle<Function>& func,int argc,Handle<Value>* argv){
		TryCatch err;
		Handle<Value> ret = func->Call(self,argc,argv);
		if(err.HasCaught()){
			ReportError(err);
			return Undefined();
		}
		return ret;
	}
	//�ڵ�ǰĿ¼��libpath�������file�ļ���
	/*bool findFileInLibPath(cs::String& file){
		Local<Object> app = GetGlobal()->Get(String::New("App"))->ToObject();
		//��ǰĿ¼�����
		Local<Value> cur = app->Get(String::New("directory"));
		cs::String path;
		if(checkFileIsExist(cur,path,file)){
			file = path;
			return 1;
		}
		//libpath����ҡ�
		Local<Array> libpath = Local<Array>::Cast(app->Get(String::New("libpath")));
		uint count = libpath->Length();
		for(uint i=0;i<count;i++){
			if(checkFileIsExist(libpath->Get(i),path,file)){
				file = path;
				return 1;
			}
		}
		return 0;
	}*/
	bool ReadJsFile(cs::String& js,LPCWSTR file,DWORD cp){
		cs::String nfile;
		if(1!=cs::GetFileType(file)){
			return 0;
		}
		cs::FileStream fs;
		if(!fs.Create(file,OPEN_EXISTING)){
#ifdef _DEBUG
			cs::String inf;
			inf.Format(L"�޷���ȡ�ļ�: %s",file);
			Alert(inf.Handle(),MT_ERROR);
#endif
			return false;
		}
		fs.ReadString(js,0,cp);
		return 1;
	}
	inline bool ReadJsFile(cs::String& js,LPCWSTR file,LPCWSTR cp){
		DWORD code = cs::GetCodePage(cp); 
		return ReadJsFile(js,file,code);
	}
	//����һ��js�ļ�, ����ִ������main����, ����еĻ�.
	Handle<Value> LoadJsFile(LPCWSTR file,LPCWSTR cp){
		HandleScope store;
		cs::String js;
		if(!ReadJsFile(js,file,cp)) return Undefined();

		return LoadJsCode(js,file);
	}
	Handle<Value> LoadJsRes(cs::ResID jr,LPCWSTR name){
		DWORD sz;
		void* res = cs::GetResource(jr,L"RT_JS",&sz);
		if(res==0) return Undefined();
		cs::String js;
		js.FromMultiByte((LPCSTR)res,sz,CP_UTF8);
		return LoadJsCode(js,name);
	}
	bool IsInheritTemplate(Handle<Value> val,uint tid){
		if(!val->IsObject()) return false;
		Local<Function> func = GetEnv()->GetTemplate(tid)->GetFunction();
		Local<Object> obj = val->ToObject()->GetPrototype()->ToObject();
		while(true){
			if(obj->GetPrototype()->IsNull()) return false;
			if(obj->GetConstructorName()==func->GetName()->ToString()) return true;
			obj = obj->GetPrototype()->ToObject();
		}
	}
	Local<Object> TLNew(uint id,bool bind){
		Handle<Value> argv[1];
		int argc;
		if(bind){
			argv[0] = Boolean::New(true);
			argc = 1;
		}else{
			argc = 0;
		}
		return GetEnv()->GetTemplate(id)->GetFunction()->NewInstance(argc,argv);
	}
	Local<Object> TLNew(uint id,void* cobj){
		Handle<Value> argv[1];
		argv[0] = Uint32::New((UINT_PTR)cobj);
		Handle<FunctionTemplate> ft = GetEnv()->GetTemplate(id);
		return ft->GetFunction()->NewInstance(1,argv);
	}
	void setPersistentFunc(Persistent<Function>& pf,Handle<Value>& v){
		if(v->IsFunction()){
			if(!pf.IsEmpty()) pf.Dispose();
			pf = Persistent<Function>::New(Handle<Function>::Cast(v));
		}else if(v->IsNull()||v->IsUndefined()){
			if(pf.IsEmpty()) return;
			pf.Dispose();
			pf.Clear();
		}
	}
	bool GetMBString(Handle<Value>& val,cs::Memory<char>& buf,DWORD charset){
		String::Value str(val);
		uint len = cs::WcsLength((LPCWSTR)*str);
		if(len==0){
			buf.SetLength(0);
			return true;
		}
		int cbSize = ::WideCharToMultiByte(charset,0,(LPCWSTR)*str,len,0,0,0,0);
		if(!buf.SetLength(cbSize)){
			ThrowException(String::New("�����ڴ�ľ�"));
			return false;
		}
		::WideCharToMultiByte(charset,0,(LPCWSTR)*str,len,buf.Handle(),cbSize,0,0);
		return true;
	}
	bool GetMBString(Handle<Value>& val,cs::Memory<char>& buf){
		String::AsciiValue mbs(val);
		int len = strlen(*mbs) + 1;
		if(!buf.SetLength(len)){
			ThrowException(String::New("�����ڴ�ľ�"));
			return false;
		}
		buf.CopyFrom(*mbs,len);
		return true;
	}
	inline bool strIsNull(LPCWSTR str){
		return str==0||str[0]==0;
	}

	//*,{
	//	"type":"class",
	//	"name":"CObject",
	//	"text":"���� C ��Ļ��࣬����Ҫ���� new CObject(); ������һ�� CObject ����ʵ���ϸ���û�д���� CObject ���󣬵��� CObject �ĳ�Ա���κ� C ������ԭ�ͣ�prototype���ĳ�Ա��",
	//	"remark":[
	//		"�����ر�ָ��, ����C��ͷ����(���캯��)������һ���ڲ� C++ ����, CObject�����ǵĻ���",
	//		"C ����������֣����кͰ󶨣�isOwned() ���������Ƿ����������͡��������͵Ķ���������ڲ��� C++ ��Դ��������ִ�� dispose ��ʱ��C++ ��Դ���ͷš������͵� C ����󣬲������ڲ� C++ ��Դ���ͷţ���ʵ���������͵Ķ���ִ�� dispose û���κ�Ч������������Ҳ����ͨ������ dispose ���ͷ��ڲ���Դ��",
	//		"C �������ֱ�ӵ��ù��캯��������ʹ�� new �����ɶ���C �����Ĺ��캯���� 3 �ַ�ʽ��<br/>1. new CSomeThing(); ���ַ�ʽ���ɶ����ͬʱ�������ڲ��� C++ ��Դ��<br/>2. new CSomethine(obj); ���ַ�ʽ�������µ� C++ ��Դ�����ǰ� obj ��Ϊ C++ ��Դ��obj ������һ����Ӧ���͵���Ч C++ ��Դ�����debind �������صľ��������ľ����<br/>3. new CSomethine(obj,true); ���ַ�ʽ����һ�����Ͷ��󣬶��󲻸��� obj ���ͷš�<br/> 2.�� 3. ��ʽ�� obj ������ 0 �����󽫳�Ϊһ���ն�����ʵ�� dispose ��ִ�к����һ���ն���",
	//		"C ������Զ��� Stack ����֮�£��뿪�������� Stack ��Χ�����Զ����� dispose��"
	//	],
	//	"member":[
	//	{
	//		"type":"function",
	//		"name":"bind(cobj,[bindType])",
	//		"text":"��һ�� C++ ��Դ�󶨵���ǰ����ͨ������£�obj �� debind �������صľ��������Դ�����",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"cobj",
	//				"text":"�ڲ� C++ ��Դ�����"
	//			},
	//			{
	//				"type":"boolean",
	//				"name":"bindType",
	//				"text":"�Ƿ��ǰ����ͣ��������һ�������͵Ķ��󣬴˶��󲻸��� C++ ��Դ���ͷţ�ȱʡֵ false��"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"�󶨳ɹ����� true����� cobj ����һ��ָ�루ָ���ֵ���Ǵ��� 0xffff�����Ҳ��� 0 ��������ʧ�ܣ�ʧ�ܷ��� undefined��"
	//		},
	//		"remark":[
	//			"�󶨲������Զ�����֮ǰ���ڲ���Դ���൱��ִ����һ�� dispose��",
	//			"bind(0) ���� bind(0,true) �൱�� dispose();"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"debind()",
	//		"text":"�����ǰ������ڲ� C++ ��Դ�Ĺ������ҷ���������󣬵�ǰ�����Ϊһ���ն���isDisposed ���� true��",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"���ص�ǰ����󶨵��ڲ� C++ ��Դָ�룬�������Ϊ�գ����� 0��"
	//		},
	//		"remark":[
	//			"���������ζ���ڲ� C++ ��Դ�����κι������Ա���ѷ���ֵʹ�� bind �����󶨵�һ����Ӧ���͵� C ����󣬷��������ڴ�й¶��"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"obj()",
	//		"text":"���ص�ǰ������ڲ� C++ ��Դָ�롣",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"���ص�ǰ����󶨵��ڲ� C++ ��Դָ�룬�������Ϊ�գ����� 0��"
	//		},
	//		"remark":[
	//			"����ʹ�� bind ������ bind(cobj,true) �����ָ��󶨵�һ����Ӧ���͵� C ������ϣ���������ʹ���¶�������κβ��������ǲ�������������Դ����Ҫʹ�����з�ʽ��һ�� C++ ��Դ�󶨵� 2 �����ϵ� C ����󣬷����ͷ�ʱ�����"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"isOwned()",
	//		"text":"��ȡ��ǰ����İ����͡�",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"������������ͷ��� true�����򷵻� false��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isDisposed()",
	//		"text":"��ȡ��ǰ�����Ƿ�Ϊ�ա�",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"����ڲ� C++ ��ԴΪ�գ����� true�����򷵻� false��"
	//		},
	//		"remark":[
	//			"dispose(),bind(0),debind() ������ʹ�ڲ��� C++ ��ԴΪ 0��"
	//		]
	//	}
	//	]
	//}
	//,{
	//	"type":"class",
	//	"name":"CHandleObject",
	//	"text":"CHandleObject �̳��� CObject������� 4 ����������ʱ�� C ������ڲ��� C++ ��Դ���һ��ϵͳ��Դ����Щ����������Щϵͳ��Դ�����",
	//	"member":[
	//	{
	//		"type":"function",
	//		"name":"attach(handle)",
	//		"text":"��ϵͳ��Դ handle �󶨵���ǰ������ڲ� C++ ��Դ�ϡ�",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"handle",
	//				"text":"ϵͳ��Դ�����handle() ����������������"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"�󶨳ɹ����� true����� handle ����һ����Ч��ϵͳ��Դ�������ʧ�ܣ�ʧ�ܷ��� undefined��"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"detach()",
	//		"text":"�����ǰ�����ڲ� C++ ��Դ��ϵͳ��Դ�Ĺ������ҷ������ϵͳ��Դ�����",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"���ص�ǰ�����ڲ� C++ ��Դ�󶨵�ϵͳ��Դָ�롣"
	//		},
	//		"remark":[
	//			"���������ζ����Դ���ܹ������Ա���ѷ���ֵʹ�� attach �����󶨵�һ����Ӧ���͵� C ������ϣ���������ϵͳ��Դй¶��"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"handle()",
	//		"text":"���ص�ǰ������ڲ� C++ ��Դ�󶨵�ϵͳ��Դ�����",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"��ǰ������ڲ� C++ ��Դ�󶨵�ϵͳ��Դ�����"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isNull()",
	//		"text":"��ǰ�����ϵͳ��Դ�Ƿ�Ϊ�ա�",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"���Ϊ�շ��� true����ʱ���� handle �����᷵�� 0�����򷵻� false��"
	//		}
	//	}
	//	],
	//	"remark":[
	//		"�㲢����Ҫ���� CHandleObject ������ʵ�ϣ�����û������һ���࣬����������������Ļ��࣬�ܶ� C �����̳�������"
	//	]
	//}//*

//*,{
//	"type":"const",
//	"name":"Stack",
//	"text":"Stack �ṩ��һ�������˳�ʱִ����������Ļ��ơ����ʹ�� Class ��װ�����������ֶ����� Stack ������",
//	"member":[//*
//*{
//	"type":"function",
//	"name":"create()",
//	"text":"���ɺ�����ǰ�� Stack����ʹ�û�û�е����κ� Stack.create ������ϵͳ�Ѿ�������һ���� Stack��",
//	"param":[
//	],
//	"return":{
//		"type":"void",
//		"text":"����û�з���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"close()",
//	"text":"���� Stack �ڶ���� dispose �������������� Stack��",
//	"param":[
//	],
//	"return":{
//		"type":"void",
//		"text":"����û�з���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"push(obj)",
//	"text":"�Ѷ��� push �� Stack���ں����˳�ʱ������� dispose ���������ã�����Ӷ���� dispose ��������Ӷ���� dispose �����á�",
//	"param":[
//	{
//		"type":"object",
//		"name":"obj",
//		"text":"һ�����������"
//	}
//	],
//	"return":{
//		"type":"void",
//		"text":"����û�з���ֵ��"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"pop(obj)",
//	"text":"�Ѷ��� pop �� ��ǰ Stack������ push ���ϼ� Stack����������� push �������溯����Stack ����� push �� pop ˳���޹ؽ�Ҫ���û�������Ҫ�������������",
//	"param":[
//	{
//		"type":"object",
//		"name":"obj",
//		"text":"һ�����������"
//	}
//	],
//	"return":{
//		"type":"boolean",
//		"text":"�ɹ����� true��ʧ�ܷ��� false������ Stack ���� pop���� Stack pop һ����ʧ�ܡ�"
//	}
//}],//*
//*	"example":[
//		"function testStack(){",
//		"	//�ں�����ͷ��ʼ����֮�������� Stack.close()������ Stack ���ƻᱻ�ƻ���",
//		"	Stack.create();",
//		"	//�κ�һ������ֻҪ�� push �� Stack�������˳�ʱ�ͻᱻ���� dispose ���������Զ�����κ��������д�� dispose �����м��ɡ�",
//		"	var obj = {};",
//		"	obj.dispose = function(){",
//		"		alert(\"dispose() called\");",
//		"	}",
//		"	//�� obj push �� Stack��",
//		"	Stack.push(obj);",
//		"	//CFile ������Զ� push ������ʾ���� Stack.push()������ C ��ͷ�Ķ����ڲ����� C++ ���󣬷���ʹ�� C �����ĳ��򣬱����ʼ��һ�� Stack��",
//		"	var file = new CFile();",
//		"	//�ں����˳�ʱ���ã����� push �� Stack �ڵĶ�����������ж�����ڣ�������ÿ�����ڴ����� close��",
//		"	Stack.close();",
//		"}",
//		"testStack();",
//		"//���ʹ�� Class ��װ������������ʾ���� Stack��;",
//		"Class(function testStack(){",
//		"	//�κ�һ������ֻҪ�� push �� Stack�������˳�ʱ�ͻᱻ���� dispose ���������Զ�����κ��������д�� dispose �����м��ɡ�",
//		"	var obj = {};",
//		"	obj.dispose = function(){",
//		"		alert(\"dispose() called\");",
//		"	}",
//		"	//�� obj push �� Stack��",
//		"	Stack.push(obj);",
//		"	//CFile ������Զ� push ������ʾ���� Stack.push()������ C ��ͷ�Ķ����ڲ����� C++ ���󣬷���ʹ�� C �����ĳ��򣬱����ʼ��һ�� Stack��",
//		"	var file = new CFile();",
//		"},true)",
//		"testStack();"
//	]
//}//*

	//*]}//*
void loadComponent(cs::StringList& sl,LPCWSTR modeList,LPCWSTR component,Handle<Object>& glb,LPCWSTR checkName,LOADJSCOMPONENT loadc){
		if(strIsNull(modeList)||sl.IndexOf(component)!=-1){
			//����dll�Ƿ��Ѿ����ع���.
			Local<Value> vp = glb->Get(String::New((uint16_t*)checkName));
			if(vp->IsUndefined())
				loadc(glb);
		}
	}
}