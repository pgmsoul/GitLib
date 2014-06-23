
#include "stdafx.h"
#include "v8base.h"
#include "jsbase.h"
#include "jswnd.h"

#pragma usexpstyle
namespace v8{
	/*@function import(dll,[modleList])
	@text ��һ�� DLL(��̬���ӿ�), ���ص� js ������, �����̬���ӿ������ר��Ϊ js ��д��, ��һ����ͨ��dllִ���������û���κ�Ч��.
	@param dll -- string
	@text Ҫ���ص� dll �ļ�����, �������ļ��� ��1��shell ���������Ŀ¼����2��ϵͳ�������� path ָ����Ŀ¼�
	��3��shell ����Ŀ¼�� jsdll ��Ŀ¼������Բ�ʹ��ȫ·����
	@param [modleList] -- string ��ѡ
	@text һ��dll�����ж��ģ��, Ϊ�˼ӿ��ٶ�, ����ѡ���Ե�ֻ������Ҫ��ģ��, ����������ö��ŷָ���һϵ��ģ������. ����һ��dll֧����Щģ��, ��Ҫ�������dll��˵���ĵ�. �����ָ���������(ע�ⲻ����������մ�������), dllĬ�ϼ���ȫ��ģ��. ��Ȼ, dll�������Լ���Ĭ������, ���������Ҫ�ĵ�˵��.
	@return bool, ��������Ƿ�ɹ�.*/
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
//����ָ�������ļ��Ĵ򿪳���,ext:��׺��".",app:����,icon:�ļ�ͼ����app�е�����
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
	//�������л�����App���ƺ͹���Ŀ¼������Ŀ¼Ĭ������Ϊ js �ű�����Ŀ¼��
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
		msg.Format(L"�ļ� %s û���ҵ�",file);
		::MessageBox(0,msg,L"jsuser",MB_ICONWARNING);
		return 0;
	}
	HandleScope store;
	//����JavaScript������
	//HandleScope store;
	v8::EnterJs();
	cs::String js;
	if(!ReadJsFile(js,file,-1)) 0;
	initAppC(file);
	LoadJsCode(js,file);
	v8::ExitJs();

	return 0;
}