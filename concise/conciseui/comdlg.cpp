#include "stdafx.h"
#include "../conciseui.h"


namespace cs{
	//class FolderDialog	
	FolderDialog::FolderDialog(void)
	{
		::CoInitializeEx(0,COINIT_APARTMENTTHREADED);
		_Path.SetLength(MAX_PATH);
		_Path.Zero();
		_bcp.Bind(this,&FolderDialog::_BrowseCallbackProc);
	}
	int FolderDialog::_BrowseCallbackProc(HWND hwnd,UINT msg,LPARAM lp,LPARAM pData)
	{
		if(msg==BFFM_INITIALIZED)
		{
			::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)_Path.Handle());
		}
		return 0;
	}
	bool FolderDialog::Show(HWND owner,LPCWSTR title,LPCWSTR init,DWORD flags){
		if(init) WcsCopy(_Path.Handle(),init);
		BROWSEINFOW bi;
		bi.hwndOwner = owner;
		bi.pidlRoot = 0;
		bi.pszDisplayName = 0;
		bi.lpszTitle = title;
		bi.lpfn = _bcp;
		bi.ulFlags = flags;
		bi.lParam = 0;
		bi.iImage = 0;
		LPITEMIDLIST piil = ::SHBrowseForFolderW(&bi);
		bool r = 0;
		if(piil){
			r = ::SHGetPathFromIDListW(piil,_Path)!=0;
			LPMALLOC pm; 
			::SHGetMalloc(&pm);
			pm->Free(piil);
			if(!r) _Path[0] = 0;
		}
		return r;
	}
	//class FileDialog
	FileDialog::FileDialog():Flags(OFN_ENABLEHOOK|OFN_ENABLESIZING|OFN_EXPLORER|OFN_NOVALIDATE|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT),FlagsEx(true)
	{
		mDlgProc.Bind(this,&FileDialog::OFNHookProc);
		mFileNameBuffer.Reserve(0x20);	//����2M�����ڴ�.
		mFileNameBuffer.Commit(0,0);
		mSelectFiles.SetSize(MAX_PATH);
		mFilter.Add(L"");				//Ҫ����ĩβ���������ַ�.
	}
	UINT_PTR FileDialog::OFNHookProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		if(_Handle==0) _Handle = ::GetParent(hDlg);
		if(uMsg==WM_NOTIFY)
		{
			LPOFNOTIFY pon = (LPOFNOTIFY)lParam;
			switch(pon->hdr.code)
			{
			case CDN_SELCHANGE:
				{
					/*if(OnSelChange.IsNull()) break;
					Memory<wchar_t> path(MAX_PATH),file(MAX_PATH);
					int n = (int)::SendMessage(_Handle,CDM_GETFILEPATH,(WPARAM)MAX_PATH,(LPARAM)path.Handle());
					�����Ϣ���ص�ֵ����Ŀ¼���ļ�����Ŀ¼��û�����ţ��ļ����ÿո�ָ����������Ű�Χ��Ŀ¼���ļ���֮��û�пո�Ŀ¼ĩβ������б�ܡ�
					����������ʱû�У���ʱ�У�����ļ��������ո�ʵ�����޷���ȷ��ȡ�ļ��������Դ˷���ʵ���ϲ����á�
					if(n<0) path[0] = 0;
					n = (int)::SendMessage(_Handle,CDM_GETSPEC,(WPARAM)MAX_PATH,(LPARAM)file.Handle());
					�����Ϣ�����ļ���������ͬ�ϡ�
					if(n<0) path[0] = 0;
					OnSelChange(path,file);*/
					break;
				}
			case CDN_FOLDERCHANGE:
				{
					if(!OnFolderChange.IsNull()){
						Memory<wchar_t> path;
						path.SetLength(MAX_PATH);
						int n = (int)::SendMessage(_Handle,CDM_GETFOLDERPATH,(WPARAM)MAX_PATH,(LPARAM)path.Handle());
						if(n<0){
							path[0] = 0;
						}
						OnFolderChange(path,this);
					}
					break;
				}
			case CDN_FILEOK://this message is ago okid message,if return TRUE then no okid message;
				if((!OnFileOk.IsNull())&&(OnFileOk(pon->lpOFN,this)))
				{
					SetWindowLong(_Handle,DWL_MSGRESULT,1L);
					return 1;
				}
				break;
			case CDN_TYPECHANGE:
				OnTypeChange(pon->lpOFN->nFilterIndex,this);
				break;
			}
		}
		return 0;
	};
	void FileDialog::AddFilter(LPCWSTR showText,LPCWSTR filter,uint index){
		if(index>mFilter.Count()) index = mFilter.Count();
		this->mFilter.Delete(index-1);
		this->mFilter.Add(showText,index);
		index++;
		this->mFilter.Add(filter,index);
		this->mFilter.Add(L"");
	}
	bool FileDialog::SetFilterIndex(uint index)
	{
		if(mFilter.Count()<=index)
			return 0;
		mFilterIndex = index + 1;
		return 1;
	}
	bool FileDialog::DeleteFilter(uint index)
	{
		if(mFilter.Count()<=(2*index+1))
			return 0;
		bool r = this->mFilter.Delete(index*2)!=0;
		r &= this->mFilter.Delete(index*2)!=0;
		return r;
	}
	LPCWSTR FileDialog::GetFileName(uint index)
	{
		if(mSelectFiles.Count()<=index) return 0;
		return mSelectFiles[index];
	}
	uint FileDialog::GetSelectCount()
	{
		uint count = mSelectFiles.Count();
		if(count>1) count--;
		return count;
	}
	bool FileDialog::ShowOpen(HWND owner)
	{
		if(_Handle) return 0;//��ֹͬһʵ���������Ի���

		Object<OPENFILENAME> ofn(true);
		ofn.Zero();
		ofn->lStructSize = (DWORD)sizeof(OPENFILENAME);
		//�ļ�������Ͽ������.���ļ������÷ֺŸ���.ǰһ������ʾ������,�����������ַ�.��һ�����ļ����͹����ִ�,ǰ�����ַ�������"*.".
		ofn->lpstrFilter = mFilter.Handle();
		//���������һ����NULL�ַ��ֿ��Ķ��ַ���,ĩβ����������NULL�ַ�(�Դ���ȷ����β).
		ofn->lpstrFile = (LPWSTR)mFileNameBuffer.Handle();
		WcsCopy(ofn->lpstrFile,DefaultFile.Handle(),DefaultFile.Length());
		ofn->nMaxFile = 0x200000;
		ofn->lpstrTitle = Title;
		ofn->Flags = Flags;
		ofn->lpfnHook = mDlgProc.Procedure();
		ofn->lpstrDefExt = DefaultExt.Handle();
		ofn->FlagsEx = FlagsEx;//OFN_EX_NOPLACESBAR;
		ofn->lpstrInitialDir = InitDir;
		ofn->hwndOwner = owner;
		ofn->nFilterIndex = mFilterIndex;

		mSelectFiles.Clear();
		bool r = ::GetOpenFileName(ofn.Handle())==1;
		if(r)
		{
			mSelectFiles.AddFromMemory(ofn->lpstrFile,-1);
			mFileNameBuffer.Desert();
		}
		_Handle = 0;
		return r;
	}
	bool FileDialog::ShowSave(HWND owner)
	{
		if(_Handle) return 0;

		Object<OPENFILENAME> ofn(true);
		ofn.Zero();
		ofn->lStructSize = (DWORD)sizeof(OPENFILENAME);
		//�ļ�������Ͽ������.���ļ������÷ֺŸ���.ǰһ������ʾ������,�����������ַ�.��һ�����ļ����͹����ִ�,ǰ�����ַ�������"*.".
		ofn->lpstrFilter = mFilter.Handle();
		//���������һ����NULL�ַ��ֿ��Ķ��ַ���,ĩβ����������NULL�ַ�(�Դ���ȷ����β).
		ofn->lpstrFile = (LPWSTR)mFileNameBuffer.Handle();
		WcsCopy(ofn->lpstrFile,DefaultFile.Handle(),DefaultFile.Length());
		ofn->nMaxFile = 0x200000;
		ofn->lpstrTitle = Title;
		ofn->Flags = Flags;
		ofn->lpfnHook = mDlgProc.Procedure();
		ofn->lpstrDefExt = DefaultExt.Handle();
		ofn->FlagsEx = FlagsEx;//OFN_EX_NOPLACESBAR;
		ofn->lpstrInitialDir = InitDir;
		ofn->hwndOwner = owner;
		ofn->nFilterIndex = mFilterIndex;

		mSelectFiles.Clear();
		bool r = ::GetSaveFileName(ofn.Handle())==1;
		if(r){
			String fp = ofn->lpstrFile;
			String ext = ofn->lpstrFile;
			FPToExt(ext);
			if(ext.IsNull()){
				uint ifilter = (uint)ofn->nFilterIndex*2 - 1;
				if(mFilter.Count()>ifilter){
					ext = mFilter[ifilter];
					uint i = ext.Length();
					if(i>0) i--;
					for(;i>0;i--){
						if(ext[i]=='.') break;
					}
					FPLinkExt(fp,ext+i);
				}
			}
			mSelectFiles.AddFromMemory(fp,-1);
			mFileNameBuffer.Desert();
		}
		_Handle = 0;
		return r;
	}

	//class Color Dialog
	DWORD ColorDialog::CustomColor[16];
	ColorDialog::ColorDialog(){
		;
	}
	bool ColorDialog::Show(HWND hWnd,ColorFlags flags){
		CHOOSECOLOR cc;
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = hWnd;
		cc.lpCustColors = CustomColor;
		cc.rgbResult = Color;
		cc.Flags = CC_ANYCOLOR|CC_RGBINIT|flags;

		if (!ChooseColor(&cc)) return false;
		Color = cc.rgbResult; 
		return true;
	}
	//class Font Dialog
	FontDialog::FontDialog(){
		LogFont.New();
		LogFont.Zero();
		ChooseFont.New();
		ChooseFont.Zero();
		ChooseFont->lStructSize = ChooseFont.Size();
		ChooseFont->lpLogFont = LogFont.Handle();
		ChooseFont->lpszStyle = mStyle;
		ChooseFont->lpszStyle[0] = 0;
		ChooseFont->Flags = CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT|CF_USESTYLE;
		mDlgProc.Bind(this,&FontDialog::CFHookProc);
		ChooseFont->lpfnHook = mDlgProc.Procedure();
	}
	bool FontDialog::Show(HWND hWnd,bool apply,bool effects){
		if(effects) ChooseFont->Flags |= CF_EFFECTS;
		if(apply) ChooseFont->Flags |= CF_APPLY|CF_ENABLEHOOK;
		if((LogFont->lfWeight<0x200)&&(!LogFont->lfItalic)) WcsCopy(ChooseFont->lpszStyle,L"����");
		else if((LogFont->lfItalic)&&(LogFont->lfWeight<0x200)) WcsCopy(ChooseFont->lpszStyle,L"б��");
		else if((LogFont->lfItalic)&&(LogFont->lfWeight>=0x200)) WcsCopy(ChooseFont->lpszStyle,L"��б��");
		else WcsCopy(ChooseFont->lpszStyle,L"����");
		ChooseFont->hwndOwner = hWnd;
		bool r = ::ChooseFont(ChooseFont.Handle())!=0;
		if(r) OnApply(LogFont.Handle(),this);
		return r;
	}
	void FontDialog::Initialize(int size,wchar_t* name,Color color,bool bold,bool italic,bool underline,bool strike)
	{
		LogFont->lfHeight = size*3/4;
		WcsCopy(LogFont->lfFaceName,name);
		LogFont->lfItalic = italic;
		LogFont->lfUnderline = underline;
		LogFont->lfStrikeOut = strike;
		ChooseFont->rgbColors = color;
	}
	UINT_PTR FontDialog::CFHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)
	{
		if((uiMsg==WM_COMMAND)&&(HIWORD(wParam)==BN_CLICKED)&&(LOWORD(wParam)==0x402))//��رհ�ťҲ�ᴥ��ȡ��BTN���µ���Ϣ.
		{
			wchar_t name[32],shap[4],size[4];
			HWND h = GetDlgItem(hdlg,0x470);
			::GetWindowText(h,name,32);
			WcsCopy(LogFont->lfFaceName,name,32);

			h = GetDlgItem(hdlg,0x471);
			::GetWindowText(h,shap,4);
			if(WcsEqual(shap,L"б��"))LogFont->lfItalic = 1,LogFont->lfWeight = 400;
			else if(WcsEqual(shap,L"����"))LogFont->lfItalic = 0,LogFont->lfWeight = 900;
			else if(WcsEqual(shap,L"��б��"))LogFont->lfItalic = 1,LogFont->lfWeight = 900;
			else LogFont->lfItalic = 0,LogFont->lfWeight = 0;

			h = GetDlgItem(hdlg,0x472);
			::GetWindowText(h,size,4);
			if(WcsEqual(size,L"����")) LogFont->lfHeight = -56;
			else if(WcsEqual(size,L"С��")) LogFont->lfHeight = -48; 
			else if(WcsEqual(size,L"һ��")) LogFont->lfHeight = -35; 
			else if(WcsEqual(size,L"Сһ")) LogFont->lfHeight = -32; 
			else if(WcsEqual(size,L"����")) LogFont->lfHeight = -29; 
			else if(WcsEqual(size,L"С��")) LogFont->lfHeight = -24; 
			else if(WcsEqual(size,L"����")) LogFont->lfHeight = -21; 
			else if(WcsEqual(size,L"С��")) LogFont->lfHeight = -20; 
			else if(WcsEqual(size,L"�ĺ�")) LogFont->lfHeight = -19; 
			else if(WcsEqual(size,L"С��")) LogFont->lfHeight = -16; 
			else if(WcsEqual(size,L"���")) LogFont->lfHeight = -14; 
			else if(WcsEqual(size,L"С��")) LogFont->lfHeight = -12; 
			else if(WcsEqual(size,L"����")) LogFont->lfHeight = -10; 
			else if(WcsEqual(size,L"С��")) LogFont->lfHeight = -9; 
			else if(WcsEqual(size,L"�ߺ�")) LogFont->lfHeight = -7; 
			else if(WcsEqual(size,L"�˺�")) LogFont->lfHeight = -7; 
			else{
				String str(size);
				LogFont->lfHeight = str.ToInt32(10);
			}
			if(LogFont->lfHeight==0) LogFont->lfHeight = 9;

			h = GetDlgItem(hdlg,0x411);
			if(BST_CHECKED==SendMessage(h,BM_GETCHECK,0,0)) LogFont->lfUnderline = 1;
			else LogFont->lfUnderline = 0;

			h = GetDlgItem(hdlg,0x410);
			if(BST_CHECKED==SendMessage(h,BM_GETCHECK,0,0)) LogFont->lfStrikeOut = 1;
			else LogFont->lfStrikeOut = 0;

			LogFont->lfCharSet = DEFAULT_CHARSET;

			OnApply(LogFont.Handle(),this);
			return 1;
		}else if(uiMsg==WM_INITDIALOG){
			HWND h = GetDlgItem(hdlg,0x470);
			::SetWindowText(h,LogFont->lfFaceName);
		}
		return 0;
	}
}