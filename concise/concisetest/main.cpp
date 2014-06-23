#include <concise/conciseui.h>
#include <time.h>
#include <stdio.h>
#include "../concise/xyssl/aes.h"

void __stdcall onProgress(cs::DirData* dd,void* param){
	uint preAfter = dd->PreOrAfter;
	cs::Print(L"%s,%d",dd->FindData->cFileName,preAfter);
	cs::Print(L"%s,%s",dd->SourceDir,dd->DestDir);
}
void __stdcall onDataProgress(uint64 offset,uint64 length,bool& cancel,void* param){
	uint of = (uint)offset,len = (uint)length;
	cs::Print(L"%d,%d",of,len);
}
void __stdcall onComplete(LPCWSTR path,LPCWSTR fn,void*){
	cs::Print(L"%s,%s\r\n",path,fn);
}
void copy(){
	LPCWSTR src = L"D:\\SoftProject\\Chrome";
	LPCWSTR dst = L"D:\\SoftProject\\Chrome2";
	cs::DSParam dsp;
	dsp.Destinate = dst;
	dsp.Source= src;
	dsp.OnProgress.BindStd(&onProgress);
	dsp.OnFileProgress.BindStd(&onDataProgress);
	dsp.OnComplete.BindStd(&onComplete);
	cs::DirectorySystem::Copy(&dsp);
}
void del(){
	LPCWSTR src = L"D:\\SoftProject\\Chrome";
	LPCWSTR dst = L"D:\\SoftProject\\Chrome2";
	cs::DSParam dsp;
	dsp.Destinate = dst;
	dsp.Source= dst;
	dsp.OnProgress.BindStd(&onProgress);
	dsp.OnFileProgress.BindStd(&onDataProgress);
	dsp.OnComplete.BindStd(&onComplete);
	cs::DirectorySystem::Delete(&dsp);
}
void __stdcall onList(cs::ListData* ld){
	cs::Print(L"%s,%d",ld->FindData->cFileName,ld->Flag);
}
void list(){
	cs::FileTree ft;
	ft.SetAfterCall(1);
	ft.SetPreCall(1);
	ft.SetSearchSub(1);
	ft.OnList.BindStd(&onList);
	LPCWSTR src = L"D:\\SoftProject\\Chrome";
	ft.Search(src);
}
void __stdcall proc(WORD,WORD,void*){
	;
}
//DT_WORDBREAK		换行，但是不会切断单词，如果一行放不下这个单词，单词会被剪切，但是仍然不换行。左右不会输出半个字体，如果矩形宽度放不下一个字符，就一个字符也不输出，但是上下会输出部分无法完全显示的单词。
//DT_EDITCONTROL	模拟Edit控件，配合DT_WORDBREAK，区别如下，无法单行写下的单词，会在下一行继续输出，而不会剪切，不会显示半行文本。
//DT_NOCLIP			持续输出，无视矩形的高度，所以文本都会被输出。
//DT_END_ELLIPSIS	无法显示的尾部字符用三个点号表示，这个属性不能和 DT_WORDBREAK 一起用，否则无效果。
//DT_EXPANDTABS		显示tab字符，默认tab字符是不输出的，每个tab字符占8个普通字符（4个中文字符）的宽度。
//DT_NOPREFIX		关闭前缀字符的处理，通常DrawText解释助记前缀字符，&为给其后的字符加下划线，解释&&为显示单个&。指定DT_NOPREFIX，这种处理被关闭。下划线可以被添加到空格和tab键的下面。
//DT_TABSTOP		设置tab字符的最大宽度，在DRAWTEXTPARAMS参数的iTabLength里设置，如果这个值是0，会设为默认 8 。
//DT_SINGLELINE		无视换行符，持续在一行输出。默认无论回车还是换行符，都会换行，但是连续的\r\n值换行一次，DT_SINGLELINE 会导致 uiLengthDrawn 失效，总是返回字串的长度。
class Main : public cs::Frame{
	LPCWSTR str;
	uint count;
	DRAWTEXTPARAMS	dtp;
	void  onDraw(cs::DC* dc,cs::View* ){
		cs::Rect r(20,20,210,216);
		dc->SetBkTransparent();
		::Rectangle(*dc,r.left,r.top,r.right,r.bottom);
		cs::Font* font = cs::GetDefaultFont();
		font->Select(*dc);
		DWORD format = DT_WORDBREAK|DT_EXPANDTABS|DT_TABSTOP|DT_EDITCONTROL|DT_SINGLELINE;
		int n = DrawTextEx(*dc,(LPWSTR)str,count,r,format,&dtp);
		font->Select(*dc);
	}
public:
	Main(){
		cs::Font* font = cs::GetDefaultFont();
		font->SetSize(20);
		font->Create();
		::ZeroMemory(&dtp,sizeof(dtp));
		dtp.cbSize = sizeof(dtp);
		dtp.iTabLength = 8;
		str = L"搜狗\t浏\r览\n器日前宣\t布，dgdsfgdsfgsdfgdsfgds为小米路由器带来了研发两年的的网页加速引擎技术“搜狗预取引擎”，目前这一新插件已上线。\r\n\
			   据官方介绍，“搜狗预取引擎”能精准预测下一步访问网页并提前加载，打开网页至少快一倍。小米路由器用户可以体验一下。";
		count = cs::WcsLength(str);
		OnDraw.Add(this,&Main::onDraw);
		cs::SetWndRememberPos(this,L"wnd");
	}
};
int shfile(LPCWSTR dst,LPCWSTR src,UINT func,FILEOP_FLAGS flag){
	cs::String from = src;
	cs::FPToFullPath(from);
	from.SetCubage(from.Length()+1);
	from[from.Length()] = 0;

	cs::String to = dst;
	cs::FPToFullPath(to);
	to.SetCubage(to.Length()+1);
	to[to.Length()] = 0;

	SHFILEOPSTRUCT fos;
	::ZeroMemory(&fos,sizeof(fos));
	fos.wFunc = func;
	fos.pFrom = from;
	fos.pTo = to;
	fos.fFlags = flag;
	fos.lpszProgressTitle = L"jsuser";

	return SHFileOperation(&fos);
}
int shcopy(LPCWSTR dst,LPCWSTR src,bool noundo = false,bool noask = false){
	FILEOP_FLAGS flag = FOF_SIMPLEPROGRESS;
	if(!noundo) flag |= FOF_ALLOWUNDO;
	if(noask) flag |= FOF_NOCONFIRMATION;
	return shfile(dst,src,FO_COPY,flag);
}
int shmove(LPCWSTR dst,LPCWSTR src,bool noundo = false,bool noask = false){
	FILEOP_FLAGS flag = FOF_SIMPLEPROGRESS;
	if(!noundo) flag |= FOF_ALLOWUNDO;
	if(noask) flag |= FOF_NOCONFIRMATION;
	return shfile(dst,src,FO_MOVE,flag);
}
int shrename(LPCWSTR dst,LPCWSTR src,bool noundo = false,bool askexist = false){
	FILEOP_FLAGS flag = FOF_SIMPLEPROGRESS;
	if(!noundo) flag |= FOF_ALLOWUNDO;
	if(!askexist) flag |= FOF_RENAMEONCOLLISION;
	return shfile(dst,src,FO_RENAME,flag);
}
int shdelete(LPCWSTR path,bool noundo = false,bool noask = false){
	FILEOP_FLAGS flag = FOF_SIMPLEPROGRESS;
	if(!noundo) flag |= FOF_ALLOWUNDO;
	if(noask) flag |= FOF_NOCONFIRMATION;
	return shfile(0,path,FO_DELETE,flag);
}
int sh(){
	LPCWSTR src = L"D:\\Documents\\Downloads\\abc";
	LPCWSTR dst = L"D:\\Documents\\Downloads\\ABC3\\abc3";
	return shmove(dst,src,false,true);
}
class A{
int i;
public:
	A(){
		i = 0;
	}
	~A(){
		i = -1;
	}
};
class B{
	int j;
	A a;
public:
	B(){
		j = 0;
	}
	~B(){
		j = -1;
	}
};
WINMAIN{
	B a;
	return 0;
};