
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "LayoutDesigner.h"


//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////
CClassView* glbOpt = 0;
CClassView::CClassView()
{
	glbOpt = this;
	//m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CClassView::~CClassView()
{
}

BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void CClassView::EnableSaveButton(BOOL enable){
	opbut[2]->Enable(enable!=0);
	glbDoc->SetModifiedFlag(enable);
}
WORD getButId(int index){
	if(index==0) return ID_FILE_NEW;
	else if(index==1) return ID_FILE_OPEN;
	else if(index==2) return ID_FILE_SAVE;
	return 0;
}
void createButtonIgl(cs::ImageList& igl,cs::ResID res){
	cs::Icon icon;
	igl.Create(32,32,ILC_COLOR32);
	if(icon.LoadIcon(res,32,32)){
		igl.AddIcon(icon);
	}
}
int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	createButtonIgl(butImageList[0],IDI_BUTTON_NEW);
	createButtonIgl(butImageList[1],IDI_BUTTON_OPEN);
	createButtonIgl(butImageList[2],IDI_BUTTON_SAVE);
	createButtonIgl(butImageList[3],IDI_BUTTON_SAVEAS);
	createButtonIgl(butImageList[4],IDI_BUTTON_ADD);
	createButtonIgl(butImageList[5],IDI_BUTTON_COPY);
	createButtonIgl(butImageList[6],IDI_BUTTON_PASTE);
	createButtonIgl(butImageList[7],IDI_BUTTON_CUT);
	createButtonIgl(butImageList[8],IDI_BUTTON_LEFT);
	createButtonIgl(butImageList[9],IDI_BUTTON_RIGHT);
	createButtonIgl(butImageList[10],IDI_BUTTON_UP);
	createButtonIgl(butImageList[11],IDI_BUTTON_DOWN);

	pad.Param->Location.SetValue(0,0);
	pad.BkColor.SetValue(255,255,255);
	pad.Create(m_hWnd);
	topLay.Param.SetMargin(3,3,3,3);
	topLay.SetContainer(&pad);

	topLay.Param.SetVertical();
	for(uint i=0;i<3;i++){
		cs::Layout* line = topLay.Add();
		line->Param.SetHorizontal();
		for(uint j=0;j<4;j++){
			cs::Layout* unit = line->Add();
			int index = i*4+j;
			unit->Param.SetMargin(3,3,3,3);
			unit->SetControl(L"Button",0,0);
			opbut[index] = (cs::Button*)unit->GetControl();
		}
	}
	opbut[0]->Param->Identity = ID_FILE_NEW;
	opbut[1]->Param->Identity = ID_FILE_OPEN;
	opbut[2]->Param->Identity = ID_FILE_SAVE;
	opbut[3]->Param->Identity = ID_FILE_SAVE_AS;
	opbut[4]->Param->Identity = ID_EDIT_INSERT;
	opbut[5]->Param->Identity = ID_EDIT_COPY;
	opbut[6]->Param->Identity = ID_EDIT_PASTE;
	opbut[7]->Param->Identity = ID_EDIT_CUT;
	opbut[8]->Param->Identity = ID_EDIT_LEFT;
	opbut[9]->Param->Identity = ID_EDIT_RIGHT;
	opbut[10]->Param->Identity = ID_EDIT_UP;
	opbut[11]->Param->Identity = ID_EDIT_DOWN;

	topLay.Refresh();

	for(int i=0;i<12;i++){
		opbut[i]->SetImageList(butImageList[i],BUTTON_IMAGELIST_ALIGN_CENTER);
		if(i>3)
			opbut[i]->OnCommand.Add(&glbMainFrm->m_wndFileView,&CFileView::onEditCommand);
		else
			opbut[i]->OnCommand.Add(this,&CClassView::onButCommand);
	}
	opbut[2]->Enable(0);

	butToolTip.Create(m_hWnd);
	butToolTip.AddTip(*opbut[0],L"新建 Layout 文件",0);
	butToolTip.AddTip(*opbut[1],L"打开 Layout 文件",1);
	butToolTip.AddTip(*opbut[2],L"保存",2);
	butToolTip.AddTip(*opbut[3],L"另存为",3);
	butToolTip.AddTip(*opbut[4],L"插入新 Layout",4);
	butToolTip.AddTip(*opbut[5],L"复制",5);
	butToolTip.AddTip(*opbut[6],L"粘贴",6);
	butToolTip.AddTip(*opbut[7],L"剪切",7);
	butToolTip.AddTip(*opbut[8],L"向左移动",8);
	butToolTip.AddTip(*opbut[9],L"向右移动",9);
	butToolTip.AddTip(*opbut[10],L"向上移动",10);
	butToolTip.AddTip(*opbut[11],L"向下移动",11);

	OnChangeVisualStyle();

	return 0;
}
void CClassView::onButCommand(WORD id,WORD type,cs::IWnd* iw){
	HWND hp = ::GetParent(m_hWnd);
	while(true){
		HWND hpp = ::GetParent(hp);
		if(hpp==NULL){
			break;
		}
		hp = hpp;
	}
	::SendMessage(hp,WM_COMMAND,MAKEWPARAM(id,type),(LPARAM)iw->Handle());
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
	pad.SetSize(cx,cy);
}

void CClassView::FillClassView()
{
}

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}


void CClassView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	::GetWindowRect(pad,rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
}

void CClassView::OnChangeVisualStyle(){
}
