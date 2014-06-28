#include "stdafx.h"
#include "v8base.h"
#include "resource.h"

namespace v8{
	LPCWSTR NULL_STR = L"";
	//*{
	//	"type":"group",
	//	"name":"base",
	//	"text":"jsuser 内置基本功能，这些函数和对象无需手动加载。",
	//	"member":[//*
	//*{
	//	"type":"const",
	//	"name":"Global",
	//	"text":"Global 就一个全局变量, 所有顶级对象都是它的属性. 或者说 Global 是唯一的最顶级的变量, 所有全局变量都是它的属性成员, 特别的, Global 自己也是自己的成员。",
	//	"remark":[
	//		"JavaScript 的函数实际上分为两类用途，一类像普通函数一样，直接调用；而另一类，用于 new SomeThing() 这种方式来创建对象。准确的说，这两类函数并没有本质区别，只不过是函数的两种用法。但是实际当中，这两种用法的函数在设计上是完全不同的，也就是说，完全可以称为两类函数。",
	//		"我们一般称直接调用的函数为函数，它们以小写字母开头。而用于创建对象的函数，我们也把它们称为类，以大写字母开头。JavaScript 内置的这种类函数，即可以用 new 来调用，也可以直接调用，结果是一样的。但是 <span style=\"color:red;font-weight:bold;\">jsuser 库的类函数不能直接调用，必须用 new 的方式来返回一个对象</span>。"
	//	]
	//}//*
	//*,{
	//	"type":"function",
	//	"name":"alert([text],[title],[iconAndtype],[parent])",
	//	"text":"弹出一个系统对话框, 显示指定的信息文本。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"[text]",
	//			"text":"要显示的信息, 这个参数可以传任何类型, 但是最终显示的是这个对象 toString 函数的返回值。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[title]",
	//			"text":"对话框标题, 缺省是应用程序名称（这个名称可以通过 App.name 设置），缺省值是js文件的名称。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[iconAndtype]",
	//			"text":"对话框的图标和按钮类型, 最多有3组可选, 每组可以选一个值:<br/>第一组, 表示图标:<br/>alert.ERROR = 0x10;（缺省值，即 0 或没有这个参数的时候，默认的值。）<br/>一个通常表示出现错误的图标<br/>alert.ASK = 0x20;<br/>一个问号图标<br/>alert.WARN = 0x30;<br/>一个感叹号图标<br/>alert.INFO = 0x40;<br/>一个信息图标<br/><br/>第二组, 表示按钮类型:<br/>alert.OK_CANCEL = 0x1;。<br/>2 个按钮, 确定, 取消.<br/>alert.ABORT_RETRY_IGNORE = 0x2;<br/>3 个按钮, 终止,重试,忽略<br/>alert.YES_NO_CANCEL = 0x3;<br/>3 个按钮, 是,否,取消<br/>alert.YES_NO = 0x4;<br/>2 个按钮, 是, 否<br/>alert.RETRY_CANCEL = 0x5;<br/>2 个按钮, 重试, 取消<br/>alert.CANCEL_TRY_CONTINUE = 0x6;<br/>3 个按钮, 取消, 重试, 继续<br/><br/>第三组, 设置缺省按钮:<br/>alert.DEF_BUTTON1 = 0x000;（缺省值）<br/>第一个按钮缺省 <br/>alert.DEF_BUTTON2 = 0x100;<br/>第二个按钮缺省<br/>alert.DEF_BUTTON3 = 0x200;<br/>第三个按钮缺省"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[parent]",
	//			"text":"父窗口的句柄, alert本质是一个模态对话框, 关闭前, 无法切换到父窗口操作。"
	//		}
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"函数总是返回一个整数。根据不同的类型，返回值的意义如下：<br/>alert.OK = 1;（点击了确定按钮）<br/>alert.CANCEL = 2;（点击了取消按钮）<br/>alert.ABORT = 3;（点击了终止按钮）<br/>alert.RETRY = 4;（点击了重试按钮(ABORT_RETRY_IGNORE的第二个按钮)）<br/>alert.IGNORE = 5;（点击了忽略按钮）<br/>alert.YES = 6;（点击了是按钮）<br/>alert.NO = 7;（点击了否按钮）<br/>alert.CLOSE = 8;（点击了标题栏关闭按钮）<br/>alert.HELP = 9;（点击了标题栏帮助按钮）<br/>alert.TRYAGAIN = 10;（点击了重试按钮(CANCEL_TRY_CONTINUE的第二个按钮)）<br/>alert.CONTINUE = 11;（点击了继续按钮）"
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
	//	"text":"获取指定编码的代码值，某些函数需要这个值。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"[codename]",
	//			"text":"编码名称，比如 utf-8(不要误写为utf8),ansi,gb2312 等等，如果参数为空, 或者是一个无效的名称, 返回 ANSI 编码 也就是 3。"
	//		}
	//	],
	//	"return":{
	//		"type":"integer",
	//		"text":"返回指定编码名称的数字代码。"
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
	//	"text":"加载一段js代码字串, 并且返回执行结果. 这个函数类似于 PHP 的 require 函数, 但是参数不是文件名, 而是 js 代码字串。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"js",
	//			"text":"Javascript 脚本字串。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[name]",
	//			"text":"这个名称用于调试时候显示的文件名。"
	//		}
	//	],
	//	"return":{
	//		"type":"any",
	//		"text":"返回脚本执行结果。"
	//	},
	//	"example":[
	//		"run('new Function(a,b){return a+b;}','test');",
	//		"//运行结果返回一个函数。"
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
	//	"text":"此函数和 run 的功能相同, 只不过参数是一个js文件名, 而不是js代码. 加载这个js文件, 并且返回执行结果. 这个函数和 PHP 的 require 函数相同。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"filename",
	//			"text":"Javascript 脚本文件的文件名。"
	//		},
	//		{
	//			"type":"string|integer",
	//			"name":"[codepage]",
	//			"text":"文件使用的编码，可以是编码的数字代码或者字串名称，缺省或 undefined 时自动识别是否是 ansi（3）或 utf-8（65001） 编码，其它编码无法自动识别。"
	//		}
	//	],
	//	"return":{
	//		"type":"any",
	//		"text":"返回脚本执行结果。"
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
				msg.Format(L"load(filename) 没有找到文件: %s",file.Handle());
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
	//	"text":"格式化字串，这个函数可以用字串的加法取代，但是 format 函数更方便直观。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"fms",
	//			"text":"用 %s 转义的格式化字串，例如 'x=%s,y=%s'，注意：这个函数只支持 %s 格式化，不支持其它转义比如 %d 等等，含有非 %s 的转义符，程序会报错。要表示 “%” 字符使用连续两个 “%%”。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[var...]",
	//			"text":"和 fms 参数对应的 n 个变量，n 是 fms 含有的 %s 转义个数，参数个数可以大于等于 n ，但是不能小于 n。"
	//		}
	//	],
	//	"return":{
	//		"type":"string",
	//		"text":"一个字串。"
	//	},
	//	"example":[
	//		"var str = format('x=%s,y=%s',3,5);",
	//		"alert(str);//显示“x=3,y=5”。"
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
	//	"text":"启动消息循环。startMsgLoop 函数执行后会一直运行，而不会执行它后面的语句，直到运行了 exitMsgLoop 函数。窗口程序必须运行 startMsgLoop 函数，否则程序会立即退出，且无法响应用户操作。",
	//	"param":[
	//		{
	//			"type":"boolean",
	//			"name":"[qMsg]",
	//			"text":"启动消息循环的种类，是否是非窗口消息循环，默认是 false。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	},
	//	"remark":[
	//		"startMsgLoop 内部启动两种消息循环，窗口类和非窗口类。非窗口类的消息循环效率更高，但是非窗口类消息循环必须没有使用任何窗口的时候调用，而且需要启用了对应的机制，否则函数失败立即返回。",
	//		"当一个应用程序含有窗口的时候，关闭主窗口会自动调用 exitMsgLoop 机制，从而退出应用程序。如果应用程序没有窗口，则必须自己处理在哪里调用 exitMsgLoop，否则程序被挂起，永远不会退出。"
	//	],
	//	"example":[
	//		"setTimer(function(){",
	//		"	//这里每 3 秒调用一次，一般 alert 会阻止程序继续运行直到用户关闭了对话框，但是它不会阻止 Timer 的调用，对话框会持续的弹出。",
	//		"	alert(\"click to call exitMsgLoop\");",
	//		"	//退出消息循环，startMsgLoop 返回，程序退出。",
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
	//	"text":"退出消息循环，执行这个函数会让 startMsgLoop 函数返回。",
	//	"param":[
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	}
	//}//*
	Handle<Value> exitMsgLoop(const Arguments& args){
		_env->queue.Close();
		return Undefined();
	}
	//*,{
	//	"type":"function",
	//	"name":"Class(proc,global)",
	//	"text":"Class 是一个函数，不是一个对象或常数，但是因为 class 已经作为保留字，所以改为 Class。Class 封装一个函数，这个函数自动操作 Stack，Stack 用于函数退出时执行栈内对象的 dispose 函数。",
	//	"param":[
	//		{
	//			"type":"function",
	//			"name":"func",
	//			"text":"要包装的函数。"
	//		},
	//		{
	//			"type":"boolean",
	//			"name":"[global]",
	//			"text":"这个函数是否全局可见，如果这个函数是作为某个对象的成员函数，可以传 false，默认值是 false。"
	//		}
	//	],
	//	"return":{
	//		"type":"function",
	//		"text":"返回一个包装后的函数。"
	//	},
	//	"remark":[
	//		"C 开头的 jsuser 对象内部绑定了一个 C++ 对象，虽然 JavaScript 的垃圾收集器能够在对象被清理的时候自动清理这个 C++ 对象，但是 C++ 对象往往使用了系统资源，这些资源可能是非常有限的，而且种类众多，应该尽快的释放这些资源。但是 Javascript 没有强制释放对象的机制，所以使用 Class 来模拟这个功能，当变量离开函数的范围之后，dispose 被自动调用，对 C 开头的 jsuser 对象来说，dispose 被执行，意味着内部的 C++ 对象被释放了，对象本身成为一个纯粹的 Javascript 对象，由垃圾收集器自动管理。一个绑定了内部资源的 Jsuser 对象（名称以 C 开头）总是在它 new 的位置所属的有 Stack 的函数范围内有效，而且必须有 Stack 来管理它。"
	//	],
	//	"example":[
	//		"//示例1：",
	//		"Class(function test(){",
	//		"	var file = new CFile();//file 的 dispose 函数在 test 执行完成后会被自动执行。",
	//		"	var obj = {};",
	//		"	obj.dispose = function(){",
	//		"		alert(\"obj.dispose called\");",
	//		"	}",
	//		"	Stack.push(obj);//obj 的 dispose 函数在函数执行结束后会被自动呼叫，非 C 开头的 Jsuser 对象如果使用 Stack 机制需要手动添加进 Stack，但是一般无需这么做，因为他们不需要清理资源，但是如果需要，它们也可以使用 Stack 机制。",
	//		"},true);//全局可见。",
	//		"test();",
	//		"//示例2：",
	//		"function main(){",
	//		"	var file = new CFile();//执行这一句的时候会出错，因为 main 函数或调用它的函数没有生成 Stack。",
	//		"}",
	//		"main();",
	//		"//示例3：",
	//		"Class(function main(){",
	//		"	func();",
	//		"},true)",
	//		"function func(){",
	//		"	var file = new CFile();//不会出错，因为 main 函数已经生成 Stack，但是直到 main 函数结束，file 才会被清理，达不到即时清理的目的。",
	//		"}",
	//		"main();"
	//	]
	//}//*

	//*,{
	//	"type":"function",
	//	"name":"require(lib,[compnent])",
	//	"text":"加载库的组件。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"lib",
	//			"text":"库的名称，比如 base，wnd 等。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[compnent]",
	//			"text":"库对应的组件，可以用逗号分隔，加载多个组件。如果此参数缺省，函数加载库的全部组件。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	},
	//	"remark":[
	//		"加载库的全部组件即占用系统内存资源，又占用 CPU 资源，导致加载时间增加，所以出于优化的考虑，可不加载没有用到的组件。",
	//		"可以通过库文档，查看一个库有哪些组件。"
	//	],
	//	"example":[
	//		"require('base','file');//加载文件组件，其它组件将不会加载。",
	//		"require('base');//base 库的全部组件都会被加载。"
	//	]
	//}//*
	//*,{
	//	"type":"const",
	//	"name":"FileSystem",
	//	"text":"此常数对象提供文件系统的相关操作，如粘贴、复制、删除等。",
	//	"member":[//*
	class JsApp : public JsConst<JsApp>{
	public:
		//*{
		//	"type":"property",
		//	"name":"name",
		//	"objtype":"string",
		//	"text":"应用程序的名称，默认是脚本文件名，不包括后缀。这个名称会被很多内部函数使用，所以谨慎修改它。比如配置文件的默认文件名称，而很多函数使用了配置文件，如果动态的改变了这个名称，可能造成加载不同的配置文件。"
		//}//*
		//*,{
		//	"type":"property",
		//	"name":"folder",
		//	"objtype":"string",
		//	"text":"应用程序的默认目录，它初始被设置为脚本所在的目录，谨慎改变它的值。"
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
	//	"text":"读取或设置配置文件，配置文件是一个 Json 格式的文本文件，可以是 utf-8 获取 ansi 编码。",
	//	"param":[
	//		{
	//			"type":"function",
	//			"name":"callback(cfg)",
	//			"text":"setConfig 函数如果成功，会触发这个回调函数，cfg 是一个对象，它的属性就是配置内容，对这个对象的任何更改最后都会作为 Json 格式保存到配置文件。"
	//		},
	//		{
	//			"type":"string",
	//			"name":"[filename]",
	//			"text":"配置文件名，缺省是和脚本同路径同名的一个后缀为“.json”的文本文件，如果指定了文件名，则读取和保存对应的文件。"
	//		}
	//	],
	//	"return":{
	//		"type":"boolean",
	//		"text":"如果成功打开了配置文件，函数返回 true，否则返回 undefined。"
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
	//	"text":"Api类可以呼叫用 C 编写的动态链接库的导出函数. Windows系统的Api函数绝大多数有2个版本, \"A\"和\"W\", 比如获取窗口文本的函数 GetWindowText, 本质上是GetWindowTextA和GetWindowTextW两个函数, C++程序依靠全局设置, 即是Unicode还是ANSI来确定使用那个版本的函数. 所以 C++ 程序中的 Api 函数一般只需要写函数名, 不需要带上 A 或 W 后缀. 但是实际上并没有 GetWindowText 这个函数, 所以在使用 Api 类的时候, 必须指定使用那个版本的函数. 当然, 基本上不要使用 A 版本的函数, 因为 A 版本函数的字串参数不能直接传 JavaScript 字串, 需要用 CMemory 对象来模拟. <b>不是所有的函数都有 A 和 W 两个版本</b>, 极个别的函数只有 W 版本或者只有 A 版本, 具体情况需要查阅 MSDN 文档。",
	//	"remark":[
	//		"使用 Api 类呼叫系统 Api 的时候, 参数只有3种: 整数, 字串, CMemory. 凡是 Api 函数需要指针和整数数字的参数, 全部传整数; 凡是参数是字串的情况, 传 JavaScript 字串; 其它情况使用 CMemory 模拟. CMemory 对象本质上能模拟任何参数, 因为它是内存对象, 包括整数和字串。",
	//		"Api类只能处理最多10个参数的Api, 事实上超过10参数的Api极少. 呼叫的时候, 必须使用和参数个对应的call, 错误的呼叫会导致程序崩溃。",
	//		"指针本质上就是整数, 所以所以指针类型的参数都可以用整数来传递。但是指针的指针，或者更多重的指针，需要用 CMemory 来模拟，因为指针的指针可能会被函数写入指针位置的数据。",
	//		"Api对象不是C开头的类, 不需要调用 dispose 释放 C++ 资源。",
	//		"调用 Api 对象的返回值就是 C 函数的返回值，但是它总是被认为是一个整数，如果 C 函数返回值不转换为一个 32 位整数，则返回值不可用。"
	//	],
	//	"example":[
	//		"//加载 user32.dll。",
	//		"var user32 = Api.loadModule(\"user32.dll\");",
	//		"if(!user32) throw \"load user32.dll error\";",
	//		"//获取 MessageBox API 函数。",
	//		"var MessageBox = new Api();",
	//		"if(!MessageBox.create(user32,\"MessageBoxW\")) ",
	//		"	throw \"get MessageBox error\";",
	//		"//调用 MessageBox API 函数，4 个参数必须都传，不能缺省。",
	//		"MessageBox(0,\"Hello!\",\"OK\",0x40);",
	//		"//卸载 user32.dll。",
	//		"Api.freeModule(user32);"
	//	],
	//	"member":[
	//	{
	//		"type":"function",
	//		"name":"loadModule(dll)",
	//		"text":"加载一个 dll，注意，这个函数是 Api 的静态函数，而不是成员函数，需要 Api.loadModule(dll) 这样，而不是用 Api 类对象来呼叫。",
	//		"param":[
	//			{
	//				"type":"string",
	//				"name":"dll",
	//				"text":"要加载的 dll，这个 dll 如果在当前目录或者系统目录，可以直接使用 dll 的文件名，否则必须传完整路径或相对路径，PATH 环境变量里的 dll 是不被自动识别的。"
	//			}
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"成功返回一个 dll 模块句柄，失败返回 undefined。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"freeModule(module)",
	//		"text":"卸载一个已经加载的 dll 模块，注意，这个函数是 Api 的静态函数，而不是成员函数，需要 Api.freeModule(dll) 这样，而不是用 Api 类对象来呼叫。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"module",
	//				"text":"loadModule 函数成功加载 dll 后的返回值，一个模块句柄。"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"成功返回 true，失败返回 false。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"create(module,funcname)",
	//		"text":"加载指定模块的函数。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"module",
	//				"text":"loadModule 函数成功加载 dll 后的返回值，一个模块句柄。"
	//			},
	//			{
	//				"type":"string",
	//				"name":"funcname",
	//				"text":"函数名称。"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"成功返回 true，失败返回 false。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isEmpty()",
	//		"text":"对象是否为空，即是否已经成功调用 create 函数。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"成功返回 true，失败返回 false。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"reset()",
	//		"text":"重置对象为空，即未调用 create 时的状态。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	}
	//]}//*


	//*,{
	//	"type":"const",
	//	"name":"Console",
	//	"text":"控制台相关操作。",
	//	"remark":[
	//		"控制台虽然是窗口，但是使用控制台不算窗口程序，它是一个特殊的窗口。"
	//	],
	//	"member":[
	//{
	//	"type":"function",
	//	"name":"show()",
	//	"text":"显示控制台窗口。",
	//	"return":{
	//		"type":"boolean",
	//		"text":"成功返回 true，失败返回 undefined。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"close()",
	//	"text":"关闭控制台窗口。",
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"write(text)",
	//	"text":"把文本输出到控制台窗口。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"text",
	//			"text":"输出内容"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"log(text)",
	//	"text":"把文本输出到控制台窗口，并且换行。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"text",
	//			"text":"输出内容"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"setColor(color)",
	//	"text":"设置输出窗口文本的颜色。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"color",
	//			"text":"颜色值，只支持 8(0--7) 种颜色：<br/> 0——色；<br/> 1——蓝色；<br/> 2——绿色；<br/> 3——青色(1|2)；<br/> 4——红色；<br/> 5——藕荷色(1|4)；<br/> 6——黄色(2|4)；<br/> 7——白色(1|2|4)；<br/>它只影响后面的输出，前面输出的颜色不会改变。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"readln()",
	//	"text":"读取控制台窗口的输入，调用这个函数以后，程序挂起，控制台等待用户输入，用户输入回车键后，函数返回。",
	//	"return":{
	//		"type":"string",
	//		"text":"返回用户输入的文本。"
	//	}
	//}
	//],"source":"D:\\SoftProject\\GitLib\\jsuser\\example\\console.jsuser"}//*

	//*,{
	//	"type":"function",
	//	"name":"log([text],[color])",
	//	"text":"把文本输出到日志窗口。",
	//	"param":[
	//		{
	//			"type":"string",
	//			"name":"[text]",
	//			"text":"要输出的文本，如果此参数缺省，输出一个空行。"
	//		},
	//		{
	//			"type":"integer",
	//			"name":"[color]",
	//			"text":"文本的颜色，缺省是白色。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"函数没有返回值。"
	//	},
	//	"remark":[
	//		"输出窗口是一个用于显示日志消息的窗口，不能响应用户的输入，但是可以控制字体大小、字体颜色、背景颜色。日志窗口有自己的消息循环，与主城市是否是窗口程序无关。"
	//	],
	//	"member":[
	//{
	//	"type":"function",
	//	"name":"show([exit])",
	//	"text":"显示日志窗口。",
	//	"param":[
	//		{
	//			"type":"boolean",
	//			"name":"exit",
	//			"text":"当用户关闭日志窗口时，是否退出程序，缺省是 true。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"没有返回值。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"close()",
	//	"text":"隐藏日志窗口，但是日志不会被清空。",
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setMaxLine([count])",
	//	"text":"设置日志窗口最大的文本行数，如果日志达到这个行数，前面的日志会被舍弃。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"[count]",
	//			"text":"可能的最大行数，缺省是 1000"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//}
	//,{
	//	"type":"function",
	//	"name":"setFontSize(size)",
	//	"text":"设置输出文本的字体大小，这个设置是会影响所有文本。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"size",
	//			"text":"字体的尺寸，缺省是 16."
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"setBkColor(color)",
	//	"text":"设置输出窗口的背景色，缺省是黑色。",
	//	"param":[
	//		{
	//			"type":"integer",
	//			"name":"color",
	//			"text":"颜色值。"
	//		}
	//	],
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
	//	}
	//},
	//{
	//	"type":"function",
	//	"name":"clear()",
	//	"text":"清空所有日志。",
	//	"return":{
	//		"type":"void",
	//		"text":"这个函数没有返回值。"
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
	//	"text":"Point 对象是一个纯 Javascript 对象，包含两个数字属性。Size 和 Point 是同一个对象的两个名称。",
	//	"member":[
	//	{
	//		"type":"property",
	//		"name":"x",
	//		"objtype":"integer",
	//		"text":"x 属性，一般表示横坐标位置或宽度。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"y",
	//		"objtype":"integer",
	//		"text":"y 属性，一般表示纵坐标位置或高度。"
	//	},
	//	{
	//		"type":"function",
	//		"name":"move(dx,dy)",
	//		"text":"调整 x 和 y 属性的值。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"dx",
	//				"text":"x 将会加上这个值。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"dy",
	//				"text":"y 将会加上这个值。"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	}
	//	]
	//}
	//,{
	//	"type":"class",
	//	"name":"Rect",
	//	"text":"Rect 对象是一个纯 Javascript 对象，包含四个数字属性。",
	//	"member":[
	//	{
	//		"type":"property",
	//		"name":"left",
	//		"objtype":"integer",
	//		"text":"左边界。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"top",
	//		"objtype":"integer",
	//		"text":"上边界。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"right",
	//		"objtype":"integer",
	//		"text":"右边界。"
	//	},
	//	{
	//		"type":"property",
	//		"name":"bottom",
	//		"objtype":"integer",
	//		"text":"下边界。"
	//	},
	//	{
	//		"type":"function",
	//		"name":"ptInRect(x,[y])",
	//		"text":"(x,y)所代表的点是否在这个矩形内，如果不传 y 参数，表示不考虑 y，只考虑 x。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"x",
	//				"text":"点的 x 坐标。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[y]",
	//				"text":"点的 y 坐标。"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"如果点在矩形内返回 true，否则返回 false。"
	//		},
	//		"remark":[
	//			"矩形的左边界和上边界上的点算矩形内部，但是右边界和下边界不算矩形内部。"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"getLocation()",
	//		"text":"获取矩形的位置，也就是左上角的坐标。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"Point",
	//			"text":"返回一个 Point 对象。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setLocation([x],[y])",
	//		"text":"设置矩形的位置，这个函数会改变矩形 4 个属性的值，但是保持大小不变。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"[x]",
	//				"text":"新位置的 x 坐标，如果此值为 undefined，矩形的 left 和 right 不变。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[y]",
	//				"text":"新位置的 y 坐标，如果此值为 undefined，矩形的 top 和 bottom 不变。"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setSize",
	//		"text":"设置矩形的大小，这个函数通过调整 right 和 bottom 来调整矩形的大小。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"[width]",
	//				"text":"矩形新的宽度，如果此值为 undefined，矩形的宽度不变。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"[height]",
	//				"text":"矩形新的高度，如果此值为 undefined，矩形的高度不变。"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"这个函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"getSize",
	//		"text":"获取矩形的大小。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"Size",
	//			"text":"返回一个 Size 对象。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"positive",
	//		"text":"通过交换 left 和 right 或者 top 和 bottom 的值来保证，left 总是小于等于 right，top 总是小于等于 bottom。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"这个函数没有返回值。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"setValue(left,top,right,bottom)",
	//		"text":"设置矩形的 4 个属性，这个函数直接设置，不做任何检测。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"left",
	//				"text":"要设置的 left 属性值。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"top",
	//				"text":"要设置的 top 属性值。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"right",
	//				"text":"要设置的 right 属性值。"
	//			},
	//			{
	//				"type":"integer",
	//				"name":"bottom",
	//				"text":"要设置的 bottom 属性值。"
	//			}
	//		],
	//		"return":{
	//			"type":"void",
	//			"text":"这个函数没有返回值。"
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
		//以下空间大小依实际情况调整
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
		//给全局添加stack只读变量.
		glb->Set(String::New("Stack"),Object::New(),ReadOnly);
		//加载基本的JS库文件
		cs::ResID rid(IDR_JS_BASE);
		LoadJsRes(rid,L"IDR_JS_BASE");
		//加载JS库里面的纯js类或函数, 供C++使用.
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
		v8::String::Value v8s(msg);//Value不转换编码, 必须用Unicode初始化字串.
		return MessageBox(0,(LPCWSTR)*v8s,L"info",mt);
	}
	int Alert(LPCWSTR msg,MSG_TYPE mt){
		return MessageBox(0,msg,L"info",mt);
	}
	//设置一个只读属性为JS函数, func 是Js函数体代码, 不包括function(){这里的代码};
	void SetJsFunc(Handle<ObjectTemplate> obj,LPCWSTR name,LPCWSTR func){
		cs::String str;
		str.Format(L"new Function(\"%s\");",func);
		obj->Set(String::New((uint16_t*)name),LoadJsCode(str),ReadOnly);
	}
	//C++版本的loadJSCode
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
			if(err.HasCaught()) break;//这两种判断方式可能是等价的。
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
	//在当前目录和libpath里面查找file文件。
	/*bool findFileInLibPath(cs::String& file){
		Local<Object> app = GetGlobal()->Get(String::New("App"))->ToObject();
		//当前目录里查找
		Local<Value> cur = app->Get(String::New("directory"));
		cs::String path;
		if(checkFileIsExist(cur,path,file)){
			file = path;
			return 1;
		}
		//libpath里查找。
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
			inf.Format(L"无法读取文件: %s",file);
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
	//加载一个js文件, 并且执行它的main函数, 如果有的话.
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
			ThrowException(String::New("可用内存耗尽"));
			return false;
		}
		::WideCharToMultiByte(charset,0,(LPCWSTR)*str,len,buf.Handle(),cbSize,0,0);
		return true;
	}
	bool GetMBString(Handle<Value>& val,cs::Memory<char>& buf){
		String::AsciiValue mbs(val);
		int len = strlen(*mbs) + 1;
		if(!buf.SetLength(len)){
			ThrowException(String::New("可用内存耗尽"));
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
	//	"text":"所有 C 类的基类，不需要调用 new CObject(); 来生成一个 CObject 对象，实际上根本没有纯粹的 CObject 对象，但是 CObject 的成员是任何 C 类对象的原型（prototype）的成员。",
	//	"remark":[
	//		"除非特别指明, 凡是C开头的类(构造函数)都绑定了一个内部 C++ 对象, CObject是它们的基类",
	//		"C 类对象有两种，自有和绑定，isOwned() 函数返回是否是自有类型。自有类型的对象负责管理内部的 C++ 资源，当对象执行 dispose 的时候，C++ 资源被释放。而绑定型的 C 类对象，不负责内部 C++ 资源的释放，事实上这种类型的对象执行 dispose 没有任何效果，即它无须也不能通过调用 dispose 来释放内部资源。",
	//		"C 类对象不能直接调用构造函数，必须使用 new 来生成对象。C 类对象的构造函数有 3 种方式：<br/>1. new CSomeThing(); 这种方式生成对象的同时，生成内部的 C++ 资源。<br/>2. new CSomethine(obj); 这种方式不生成新的 C++ 资源，而是把 obj 视为 C++ 资源，obj 必须是一个对应类型的有效 C++ 资源句柄，debind 函数返回的就是这样的句柄。<br/>3. new CSomethine(obj,true); 这种方式生成一个绑定型对象，对象不负责 obj 的释放。<br/> 2.和 3. 方式的 obj 可以是 0 ，对象将成为一个空对象，事实上 dispose 被执行后就是一个空对象。",
	//		"C 类对象自动在 Stack 管理之下，离开生成它的 Stack 范围，会自动调用 dispose。"
	//	],
	//	"member":[
	//	{
	//		"type":"function",
	//		"name":"bind(cobj,[bindType])",
	//		"text":"把一个 C++ 资源绑定到当前对象，通常情况下，obj 和 debind 函数返回的就是这个资源句柄。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"cobj",
	//				"text":"内部 C++ 资源句柄。"
	//			},
	//			{
	//				"type":"boolean",
	//				"name":"bindType",
	//				"text":"是否是绑定类型，如果生成一个绑定类型的对象，此对象不负责 C++ 资源的释放，缺省值 false。"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"绑定成功返回 true，如果 cobj 不是一个指针（指针的值总是大于 0xffff），且不是 0 ，函数会失败，失败返回 undefined。"
	//		},
	//		"remark":[
	//			"绑定操作会自动销毁之前的内部资源，相当于执行了一次 dispose。",
	//			"bind(0) 或者 bind(0,true) 相当于 dispose();"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"debind()",
	//		"text":"解除当前对象对内部 C++ 资源的管理，并且返回这个对象，当前对象变为一个空对象，isDisposed 返回 true。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"返回当前对象绑定的内部 C++ 资源指针，如果对象为空，返回 0。"
	//		},
	//		"remark":[
	//			"这个操作意味着内部 C++ 资源不受任何管理，所以必须把返回值使用 bind 函数绑定到一个对应类型的 C 类对象，否则会造成内存泄露。"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"obj()",
	//		"text":"返回当前对象的内部 C++ 资源指针。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"返回当前对象绑定的内部 C++ 资源指针，如果对象为空，返回 0。"
	//		},
	//		"remark":[
	//			"可以使用 bind 函数的 bind(cobj,true) 把这个指针绑定到一个对应类型的 C 类对象上，这样可以使用新对象进行任何操作，但是不能销毁镍板资源。不要使用自有方式把一个 C++ 资源绑定到 2 个以上的 C 类对象，否则释放时会出错。"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"isOwned()",
	//		"text":"获取当前对象的绑定类型。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"如果是自有类型返回 true，否则返回 false。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isDisposed()",
	//		"text":"获取当前对象是否为空。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"如果内部 C++ 资源为空，返回 true，否则返回 false。"
	//		},
	//		"remark":[
	//			"dispose(),bind(0),debind() 都可以使内部的 C++ 资源为 0。"
	//		]
	//	}
	//	]
	//}
	//,{
	//	"type":"class",
	//	"name":"CHandleObject",
	//	"text":"CHandleObject 继承自 CObject，添加了 4 个函数。有时候 C 类对象内部的 C++ 资源会绑定一个系统资源，这些函数操作这些系统资源句柄。",
	//	"member":[
	//	{
	//		"type":"function",
	//		"name":"attach(handle)",
	//		"text":"把系统资源 handle 绑定到当前对象的内部 C++ 资源上。",
	//		"param":[
	//			{
	//				"type":"integer",
	//				"name":"handle",
	//				"text":"系统资源句柄。handle() 函数返回这个句柄。"
	//			}
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"绑定成功返回 true，如果 handle 不是一个有效的系统资源句柄函数失败，失败返回 undefined。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"detach()",
	//		"text":"解除当前对象内部 C++ 资源对系统资源的管理，并且返回这个系统资源句柄。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"返回当前对象内部 C++ 资源绑定的系统资源指针。"
	//		},
	//		"remark":[
	//			"这个操作意味着资源不受管理，所以必须把返回值使用 attach 函数绑定到一个对应类型的 C 类对象上，否则会造成系统资源泄露。"
	//		]
	//	},
	//	{
	//		"type":"function",
	//		"name":"handle()",
	//		"text":"返回当前对象的内部 C++ 资源绑定的系统资源句柄。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"integer",
	//			"text":"当前对象的内部 C++ 资源绑定的系统资源句柄。"
	//		}
	//	},
	//	{
	//		"type":"function",
	//		"name":"isNull()",
	//		"text":"当前对象的系统资源是否为空。",
	//		"param":[
	//		],
	//		"return":{
	//			"type":"boolean",
	//			"text":"如果为空返回 true，此时调用 handle 函数会返回 0，否则返回 false。"
	//		}
	//	}
	//	],
	//	"remark":[
	//		"你并不需要生成 CHandleObject 对象，事实上，根本没有这样一个类，但是他是其它对象的基类，很多 C 类对象继承自它。"
	//	]
	//}//*

//*,{
//	"type":"const",
//	"name":"Stack",
//	"text":"Stack 提供了一个函数退出时执行清理操作的机制。如果使用 Class 封装函数，无需手动处理 Stack 操作。",
//	"member":[//*
//*{
//	"type":"function",
//	"name":"create()",
//	"text":"生成函数当前的 Stack，即使用户没有调用任何 Stack.create 函数，系统已经生成了一个根 Stack。",
//	"param":[
//	],
//	"return":{
//		"type":"void",
//		"text":"函数没有返回值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"close()",
//	"text":"调用 Stack 内对象的 dispose 函数，并且清理 Stack。",
//	"param":[
//	],
//	"return":{
//		"type":"void",
//		"text":"函数没有返回值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"push(obj)",
//	"text":"把对象 push 进 Stack，在函数退出时，对象的 dispose 函数被调用，后添加对象的 dispose 先于先添加对象的 dispose 被调用。",
//	"param":[
//	{
//		"type":"object",
//		"name":"obj",
//		"text":"一个对象变量。"
//	}
//	],
//	"return":{
//		"type":"void",
//		"text":"函数没有返回值。"
//	}
//}//*
//*,{
//	"type":"function",
//	"name":"pop(obj)",
//	"text":"把对象 pop 出 当前 Stack，并且 push 进上级 Stack。这个函数是 push 函数的逆函数。Stack 对象的 push 和 pop 顺序无关紧要，用户很少需要调用这个函数。",
//	"param":[
//	{
//		"type":"object",
//		"name":"obj",
//		"text":"一个对象变量。"
//	}
//	],
//	"return":{
//		"type":"boolean",
//		"text":"成功返回 true，失败返回 false。顶级 Stack 不能 pop，根 Stack pop 一定会失败。"
//	}
//}],//*
//*	"example":[
//		"function testStack(){",
//		"	//在函数开头初始化，之后必须调用 Stack.close()，否则 Stack 机制会被破坏。",
//		"	Stack.create();",
//		"	//任何一个对象，只要被 push 进 Stack，函数退出时就会被调用 dispose 函数，所以对象的任何清理操作写在 dispose 函数中即可。",
//		"	var obj = {};",
//		"	obj.dispose = function(){",
//		"		alert(\"dispose() called\");",
//		"	}",
//		"	//把 obj push 进 Stack。",
//		"	Stack.push(obj);",
//		"	//CFile 类对象自动 push 无需显示调用 Stack.push()，而且 C 开头的对象内部绑定了 C++ 对象，凡是使用 C 类对象的程序，必须初始化一个 Stack。",
//		"	var file = new CFile();",
//		"	//在函数退出时调用，清理 push 进 Stack 内的对象，如果函数有多个出口，必须在每个出口处调用 close。",
//		"	Stack.close();",
//		"}",
//		"testStack();",
//		"//如果使用 Class 封装函数，无需显示调用 Stack。;",
//		"Class(function testStack(){",
//		"	//任何一个对象，只要被 push 进 Stack，函数退出时就会被调用 dispose 函数，所以对象的任何清理操作写在 dispose 函数中即可。",
//		"	var obj = {};",
//		"	obj.dispose = function(){",
//		"		alert(\"dispose() called\");",
//		"	}",
//		"	//把 obj push 进 Stack。",
//		"	Stack.push(obj);",
//		"	//CFile 类对象自动 push 无需显示调用 Stack.push()，而且 C 开头的对象内部绑定了 C++ 对象，凡是使用 C 类对象的程序，必须初始化一个 Stack。",
//		"	var file = new CFile();",
//		"},true)",
//		"testStack();"
//	]
//}//*

	//*]}//*
void loadComponent(cs::StringList& sl,LPCWSTR modeList,LPCWSTR component,Handle<Object>& glb,LPCWSTR checkName,LOADJSCOMPONENT loadc){
		if(strIsNull(modeList)||sl.IndexOf(component)!=-1){
			//检测此dll是否已经加载过了.
			Local<Value> vp = glb->Get(String::New((uint16_t*)checkName));
			if(vp->IsUndefined())
				loadc(glb);
		}
	}
}