#include <concise/conciseui.h>
#include <time.h>
#include <stdio.h>
#include "../concise/xyssl/aes.h"
#pragma usexpstyle

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
//DT_WORDBREAK		���У����ǲ����жϵ��ʣ����һ�зŲ���������ʣ����ʻᱻ���У�������Ȼ�����С����Ҳ������������壬������ο�ȷŲ���һ���ַ�����һ���ַ�Ҳ��������������»���������޷���ȫ��ʾ�ĵ��ʡ�
//DT_EDITCONTROL	ģ��Edit�ؼ������DT_WORDBREAK���������£��޷�����д�µĵ��ʣ�������һ�м����������������У�������ʾ�����ı���
//DT_NOCLIP			������������Ӿ��εĸ߶ȣ������ı����ᱻ�����
//DT_END_ELLIPSIS	�޷���ʾ��β���ַ���������ű�ʾ��������Բ��ܺ� DT_WORDBREAK һ���ã�������Ч����
//DT_EXPANDTABS		��ʾtab�ַ���Ĭ��tab�ַ��ǲ�����ģ�ÿ��tab�ַ�ռ8����ͨ�ַ���4�������ַ����Ŀ�ȡ�
//DT_NOPREFIX		�ر�ǰ׺�ַ��Ĵ���ͨ��DrawText��������ǰ׺�ַ���&Ϊ�������ַ����»��ߣ�����&&Ϊ��ʾ����&��ָ��DT_NOPREFIX�����ִ����رա��»��߿��Ա���ӵ��ո��tab�������档
//DT_TABSTOP		����tab�ַ�������ȣ���DRAWTEXTPARAMS������iTabLength�����ã�������ֵ��0������ΪĬ�� 8 ��
//DT_SINGLELINE		���ӻ��з���������һ�������Ĭ�����ۻس����ǻ��з������ỻ�У�����������\r\nֵ����һ�Σ�DT_SINGLELINE �ᵼ�� uiLengthDrawn ʧЧ�����Ƿ����ִ��ĳ��ȡ�
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

class Main : public cs::Frame{
	LPCWSTR str;
	uint count;
	cs::Button	debugBut;
	DRAWTEXTPARAMS	dtp; 
	void  onDraw(cs::DC* dc,cs::View*){
		cs::Rect r(20,20,210,216);
		dc->SetBkTransparent();
		::Rectangle(*dc,r.left,r.top,r.right,r.bottom);
		cs::Font* font = cs::GetDefaultFont();
		font->Select(*dc);
		DWORD format = DT_WORDBREAK|DT_EXPANDTABS|DT_TABSTOP|DT_EDITCONTROL|DT_SINGLELINE;
		int n = DrawTextEx(*dc,(LPWSTR)str,count,r,format,&dtp);
		font->Select(*dc);
	}
	void onCreate(cs::IWnd*){
		debugBut.Create(_Handle);
	}
	void onDebug(WORD id,WORD type,cs::IWnd*){
		cs::LogShowWindow(); 
		cs::Log(L"����ɽ���Ǵ�̴��ɽ����ۣ�Ȼ���ٸĳ�һ�й�ɳ���ص���ɽ�ؽ��Ǹۡ����������й�����󣬴����ر����ѳ˿ͣ����ϴ����̲���˰������������ǧ��Ҫ�Ϲ��Ͼ������ĵȺ���������鷳��");
	}
public:
	Main(){
		debugBut.Param->Text = L"debug";
		debugBut.Param->SetRect(480,0,80,30);
		debugBut.OnCommand.Add(this,&Main::onDebug);
		OnCreate.Add(this,&Main::onCreate);
		cs::Font* font = cs::GetDefaultFont();
		font->SetSize(12);
		font->Create();
		::ZeroMemory(&dtp,sizeof(dtp));
		dtp.cbSize = sizeof(dtp);
		dtp.iTabLength = 8;
		str = L"�ѹ�\t�\r��\n����ǰ��\t����dgdsfgdsfgsdfgdsfgdsΪС��·�����������з�����ĵ���ҳ�������漼�����ѹ�Ԥȡ���桱��Ŀǰ��һ�²�������ߡ�\r\n\
			   �ݹٷ����ܣ����ѹ�Ԥȡ���桱�ܾ�׼Ԥ����һ��������ҳ����ǰ���أ�����ҳ���ٿ�һ����С��·�����û���������һ�¡�";
		count = cs::WcsLength(str);
		OnDoubleDraw.Add(this,&Main::onDraw);
		cs::SetWndRememberPos(this,L"wnd");
	}
};
int decrypt(){
	LPCWSTR src = L"F:\\user\\Dropbox\\Dropbox\\Private\\pw2.enc";
	cs::File file;
	if(!file.Create(src)) return 0;
	cs::Memory<char> decData,encData;
	int srclen = file.Read(&encData);

	decData.SetLength(96);
	cs::Aes aes;
	LPCSTR key = "��������һҹ������·���Ź���";
	LPCSTR iv = "�����м�˿�Ʋ���Ȼ�Ǻ�����";
	aes.SetDecKey(key,strlen(key),cs::aes_256);
	aes.SetIV(iv);
	aes.DecryptCbc(decData.Handle(),encData.Handle(),srclen);
	::OutputDebugStringA(decData);
	int r = 0;
	return r;
}
int encrypt(){
	LPCWSTR src = L"F:\\user\\Dropbox\\Dropbox\\Private\\pw.txt";
	cs::File file;
	if(!file.Create(src)) return 0;
	cs::Memory<char> decData,encData;
	int srclen = file.Read(&decData);
	cs::Aes aes;
	LPCSTR key = "��������һҹ������·���Ź���";
	LPCSTR iv = "�����м�˿�Ʋ���Ȼ�Ǻ�����";
	aes.SetEncKey(key,strlen(key),cs::aes_256);
	aes.SetIV(iv);
	int enclen = 96;
	encData.SetLength(enclen);
	aes.EncryptCbc(encData.Handle(),decData.Handle(),srclen);

	LPCWSTR encfn = L"F:\\user\\Dropbox\\Dropbox\\Private\\pw2.enc";
	file.Create(encfn);
	file.SetLength(0);
	file.Write(encData.Handle(),enclen);
	return 0;
}
WINMAIN{
	//encrypt();
	decrypt();
	return 0;
};