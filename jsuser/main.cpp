
#include "stdafx.h"
#include "v8base.h"
#include "jsbase.h"
#include "jswnd.h"

#pragma usexpstyle
namespace v8{
	/*@function import(dll,[modleList])
	@text 把一个 DLL(动态链接库), 加载到 js 环境中, 这个动态链接库必须是专门为 js 编写的, 对一个普通的dll执行这个函数没有任何效果.
	@param dll -- string
	@text 要加载的 dll 文件名称, 如果这个文件在 （1）shell 程序的所在目录，（2）系统环境变量 path 指定的目录里，
	（3）shell 所在目录的 jsdll 子目录，则可以不使用全路径。
	@param [modleList] -- string 可选
	@text 一个dll可能有多个模块, 为了加快速度, 可以选择性的只加载需要的模块, 这个参数是用逗号分隔的一系列模块名称. 具体一个dll支持那些模块, 需要查阅这个dll的说明文档. 如果不指定这个参数(注意不是这个参数空串的情形), dll默认加载全部模块. 当然, dll可以有自己的默认设置, 具体情况需要文档说明.
	@return bool, 代表加载是否成功.*/
	Handle<Value> require(const Arguments& args){
		HandleScope store;
		while(true){
			if(args.Length()<1) break;
			cs::String name,module;
			LPCWSTR cl = NULL_STR;
			GetString(args[0],name);
			if(args.Length()>1){
				GetString(args[1],module);
			}
			if(cs::WcsEqualNoCase(name,L"base")){
				cl = RequireBase(module);
			}else if(cs::WcsEqualNoCase(name,L"wnd")){
				cl = RequireWnd(module);
			}
			return store.Close(String::New((uint16_t*)cl));
		}
		return Undefined();
	}
}

void trimQuote(cs::String& file,wchar_t quote){
	if(file.Length()==0) return;
	if(file[0]!=quote) return;
	int pos = file.Find(quote,1);
	if(pos==-1) return;
	file.Head(pos);
	file.Tail(1);
}
bool getCmdFile(LPCWSTR cmd,cs::String& file){
	file = cmd;
	trimQuote(file,'\"');
	trimQuote(file,'\'');
	if(file.Length()==0){
		return false;
	}
	return true;
}
//设置指定类型文件的打开程序,ext:后缀含".",app:程序,icon:文件图标在app中的索引
bool rgFileOpenApp(LPCWSTR ext,LPCWSTR app,int icon = 0){
	cs::Registry reg,sub;
	if(cs::Registry::REG_ERROR==reg.Create(ext,HKEY_CLASSES_ROOT,1)) return false;
	cs::String name = L"jsuser";
	name += ext;
	reg.SetStrValue(0,name);
	if(cs::Registry::REG_ERROR==reg.Create(name,HKEY_CLASSES_ROOT,1)) return false;

	if(0==sub.Create(L"DefaultIcon",reg,1)) return false;
	name = app;
	name += L",";
	name.InsertNumber(icon);
	sub.SetStrValue(0,name);

	if(0==sub.MultiCreate(L"shell/open/command",reg,1)) return false;
	name = L"\"";
	name += app;
	name += L"\" \"%1\"";
	sub.SetStrValue(0,name);
	return true;
}
void setJsFileShell(){
	cs::String app;
	cs::GetCurrentExeFileName(app);
	rgFileOpenApp(L".jsuser",app,1);
}
void plainStart(){
	setJsFileShell();
}
void initAppC(LPCWSTR file){
	//设置运行环境：App名称和工作目录。工作目录默认设置为 js 脚本所在目录。
	wchar_t buf[MAX_PATH],*fn;
	DWORD len = GetFullPathName(file,MAX_PATH,buf,&fn);
	cs::String folder = buf,name = fn;
	cs::FPToParent(folder);
	cs::App::SetAppDirectory(folder);
	SetCurrentDirectory(folder);
	cs::FPToTitle(name);
	cs::App::SetAppName(name);
}
WINMAIN{
	cs::String file;
	if(!getCmdFile(lpCmdLine,file)){
		setJsFileShell();
		file = L"main.jsuser";
	}
	if(cs::GetFileType(file)!=1){
		cs::String msg;
		msg.Format(L"文件 %s 没有找到",file);
		::MessageBox(0,msg,L"jsuser",MB_ICONWARNING);
		return 0;
	}
	HandleScope store;
	//启动JavaScript环境。
	//HandleScope store;
	v8::EnterJs();
	cs::String js;
	if(!ReadJsFile(js,file,-1)) 0;
	initAppC(file);
	LoadJsCode(js,file);
	v8::ExitJs();

	return 0;
}