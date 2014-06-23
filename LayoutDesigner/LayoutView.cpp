
// LayoutView.cpp : CLayoutView 类的实现
//

#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLayoutView* glbView = NULL;
class TextDraw : public cs::_class{
protected:
	cs::Rect rect;
	cs::String text;
public:
	TextDraw(){
		rect.SetValue(0,0,0,0);
	}
	void SetLocation(int x,int y){
		rect.SetLocation(x,y);
	}
	void SetText(LPCWSTR str){
		text = str;
	}
	void Draw(HDC hdc){
		::DrawText(hdc,text,text.Length(),rect,DT_SINGLELINE);
	}
	cs::Twin GetRequestSize(HDC hdc){
		::DrawText(hdc,text,text.Length(),rect,DT_SINGLELINE|DT_CALCRECT);
		return cs::Twin(rect.Width(),rect.Height());
	}
};
void rectangle(cs::DC* dc,int x,int y,int cx,int cy){
	dc->MoveTo(x,y);
	dc->LineTo(x+cx,y);
	dc->LineTo(x+cx,y+cy);
	dc->LineTo(x,y+cy);
	dc->LineTo(x,y);
}
void Pad::drawLayout(cs::DC* dc,cs::Layout* lay){
	cs::Twin16 lc = lay->GetLocation(),
		sz = lay->GetSize();
	if(selLay!=lay) rectangle(dc,lc.x,lc.y,sz.x,sz.y);
	if(lay->Count()==0){
		LPCWSTR name = lay->Name;
		TextDraw td;
		td.SetText(name);
		cs::Twin ds = td.GetRequestSize(*dc);
		td.SetLocation(lc.x+(sz.x-ds.x)/2,lc.y+(sz.y-ds.y)/2);
		if(selLay==lay) dc->SetTextColor(0xff);
		td.Draw(*dc);
		if(selLay==lay) dc->SetTextColor(0xff0000);
	}
	for(uint i=0;i<lay->Count();i++){
		drawLayout(dc,lay->Element(i));
	}
}
Pad::Pad():selLay(0){
	OnDraw.Add(this,&Pad::onDraw);
	outLine.Create(0xff0000,PS_DOT);
	selLine.Create(0xff,PS_DOT,3);
	font.SetSize(20);
	font.Create();
}
void Pad::onDraw(cs::DC* dc,cs::View*){
	outLine.Select(*dc);
	font.Select(*dc);
	dc->SetBkTransparent();
	dc->SetTextColor(0xff0000);
	drawLayout(dc,&glbDoc->topLay);
	if(selLay!=0){
		cs::Twin16 lc = selLay->GetLocation(),
			sz = selLay->GetSize();
		selLine.Select(*dc);
		rectangle(dc,lc.x,lc.y,sz.x,sz.y);
		selLine.Select(*dc);
	}
	font.Select(*dc);
	outLine.Select(*dc);
}

// CLayoutView

IMPLEMENT_DYNCREATE(CLayoutView, CView)

BEGIN_MESSAGE_MAP(CLayoutView, CView)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CLayoutView 构造/析构

CLayoutView::CLayoutView()
{
	glbView = this;
	glbDoc->topLay.SetContainer(&pad);
	// TODO: 在此处添加构造代码

}

CLayoutView::~CLayoutView()
{
}
void CLayoutView::Reset(){
	pad.selLay = 0;
	glbDoc->topLay.Reset();
	glbDoc->topLay.Refresh();
	pad.Invalidate();
}
BOOL CLayoutView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CLayoutView 绘制

void CLayoutView::OnDraw(CDC* /*pDC*/)
{
	CLayoutDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}
void CLayoutView::Refresh(cs::Layout* sel){
	if(sel!=NULL) pad.selLay = sel;
	glbDoc->topLay.Refresh();
	pad.Invalidate();
}
void CLayoutView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLayoutView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CLayoutView 诊断

#ifdef _DEBUG
void CLayoutView::AssertValid() const
{
	CView::AssertValid();
}

void CLayoutView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLayoutDoc* CLayoutView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLayoutDoc)));
	return (CLayoutDoc*)m_pDocument;
}
#endif //_DEBUG


// CLayoutView 消息处理程序

void CLayoutView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	pad.SetSize(cx,cy);

}

int CLayoutView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	pad.BkColor.SetValue(0xff,0xff,0xff);
	pad.Param->Location.SetValue(0,0);
	pad.Create(m_hWnd);

	return 0;
}
