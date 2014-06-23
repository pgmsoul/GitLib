
// LayoutDoc.cpp : CLayoutDoc 类的实现
//

#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLayoutDoc

IMPLEMENT_DYNCREATE(CLayoutDoc, CDocument)

BEGIN_MESSAGE_MAP(CLayoutDoc, CDocument)
END_MESSAGE_MAP()


// CLayoutDoc 构造/析构
CLayoutDoc* glbDoc = NULL;
CLayoutDoc::CLayoutDoc()
{
	glbDoc = this;
}

CLayoutDoc::~CLayoutDoc()
{
}

BOOL CLayoutDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	glbView->Reset();
	glbTree->Reset();
	return TRUE;
}




// CLayoutDoc 序列化

void CLayoutDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()){
		cs::Json json;
		topLay.ToJson(&json);
		cs::String str;
		json.ToString(str);
		cs::Memory<char> buf;
		int len = str.ToMultiByte(&buf,CP_UTF8);
		ar.Write(buf.Handle(),len);
		glbOpt->EnableSaveButton(0);
	}else{
		fileIsZero = 0;
		int flen = (int)ar.GetFile()->GetLength();
		if(flen>0x100000){
			showErrorMessage(L"文件太大，超过0x100000");
			return;
		}
		cs::Memory<char> buf;
		buf.SetLength(flen);
		if(flen!=ar.Read(buf.Handle(),flen)){
			showErrorMessage(L"读取文件出错");
		}
		DWORD code = cs::MbsIsUtf8(buf.Handle(),flen)?CP_UTF8:CP_THREAD_ACP;
		cs::String str;
		str.FromMultiByte(buf.Handle(),flen,code);
		cs::Json json;
		if(!json.Parse(str)){
			showErrorMessage(L"Layout 布局文件错误");
			return;
		}
		if(!topLay.FromJson(&json)){
			showErrorMessage(L"Layout 布局文件错误");
			return;
		}
		glbOpt->EnableSaveButton(0);
		//topLay.Refresh();
		glbTree->FromLayout();
		//glbView->pad.Invalidate();
	}
}


// CLayoutDoc 诊断

#ifdef _DEBUG
void CLayoutDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLayoutDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLayoutDoc 命令
