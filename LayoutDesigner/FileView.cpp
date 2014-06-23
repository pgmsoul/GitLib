
#include "stdafx.h"
#include "mainfrm.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView
CFileView* glbTree = 0;
CFileView::CFileView()
{
	glbTree = this;
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_INSERT, OnEditInsert)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_LEFT, OnEditLeft)
	ON_COMMAND(ID_EDIT_RIGHT, OnEditRight)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_UP, OnEditUp)
	ON_COMMAND(ID_EDIT_DOWN, OnEditDown)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像:
	m_FileViewImages.Create(24,24,0,0,0);
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	OnChangeVisualStyle();

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView(){
	m_hRoot = m_wndFileView.InsertItem(_T("Root"), 0,1);
	m_wndFileView.SetItemData(m_hRoot,(DWORD_PTR)&glbDoc->topLay);
	m_wndFileView.SelectItem(m_hRoot);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL) return;

	CRect rectClient;
	GetClientRect(rectClient);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 0 + 1, rectClient.Width() - 2, rectClient.Height() - 0 - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

bool checkNameUniq(cs::Layout* lay,LPCWSTR name){
	if(cs::WcsEqual(lay->Name,name)) return false;
	for(uint i=0;i<lay->Count();i++){
		if(!checkNameUniq(lay->Element(i),name)) return false;
	}
	return true;
}
void getUniqName(cs::Layout* lay,cs::String& name){
	name = L"layout";
	int i = 1;
	while(!checkNameUniq(lay,name)){
		name.Format(L"layout%d",i);
		i++;
	}
}

//移动item 到 parent 分支下，pos 是 parent 之下的一个子项，item 放在 pos 的下面。
void CFileView::moveTreeItem(HTREEITEM item,HTREEITEM parent,HTREEITEM pos){

	cs::Layout* lay = (cs::Layout*)m_wndFileView.GetItemData(item);
	if(lay==NULL){
		CString text = m_wndFileView.GetItemText(item);
		cs::String msg = L"项绑定的数据为空: ";
		msg += text;
		showErrorMessage(msg);
		ASSERT(0);
		return;
	}
	//在 parent 下新建一个和 item 及其子项的副本，包括绑定的数据。
	HTREEITEM newItem = copyTreeItem(item,parent,pos);

	cs::Layout* oldParent = lay->GetParent();
	cs::Layout* newParent = (cs::Layout*)m_wndFileView.GetItemData(parent);
	if(newParent==NULL){
		showErrorMessage(L"项绑定的数据为空");
		ASSERT(0);
		return;
	}
	oldParent->RemoveOut(lay);
	HTREEITEM sItem = m_wndFileView.GetChildItem(parent);
	int index = 0;
	while(sItem!=newItem){
		sItem = m_wndFileView.GetNextItem(sItem,TVGN_NEXT);
		index++;
	}
	newParent->AddIn(lay,index);

	m_wndFileView.SelectItem(newItem);
	m_wndFileView.DeleteItem(item);
	m_wndFileView.Expand(newItem,TVE_EXPAND);
}
void CFileView::copyChildItem(HTREEITEM item,HTREEITEM newItem){
	HTREEITEM child = m_wndFileView.GetChildItem(item);
	while(child){
		HTREEITEM cItem = copyTreeItem(child,newItem,TVI_LAST);
		child = m_wndFileView.GetNextItem(child,TVGN_NEXT);
	}
}
HTREEITEM CFileView::copyTreeItem(HTREEITEM item,HTREEITEM parent,HTREEITEM pos){
	CString str = m_wndFileView.GetItemText(item);
	HTREEITEM newItem = m_wndFileView.InsertItem(str,0,1,parent,pos);
	cs::Layout* lay = (cs::Layout*)m_wndFileView.GetItemData(item);
	if(lay==NULL){
		ASSERT(0);
		return 0;
	}
	m_wndFileView.SetItemData(newItem,(DWORD_PTR)lay);
	copyChildItem(item,newItem);

	return newItem;
}
HTREEITEM CFileView::getSelectItem(){
	HTREEITEM item = m_wndFileView.GetSelectedItem();
	if(item==0){
		showErrorMessage(L"需要选定插入点或操做项");
		return 0;
	}
	return item;
}
void CFileView::insertLayout(cs::Layout* lay,HTREEITEM node){
	node = m_wndFileView.InsertItem(lay->Name,0,1,node);
	ASSERT(lay);
	m_wndFileView.SetItemData(node,(DWORD_PTR)lay);
	for(int i=0;i<(int)lay->Count();i++){
		insertLayout(lay->Element(i),node);
	}
}
void CFileView::Reset(){
	m_wndFileView.DeleteItem(m_hRoot);
	FillFileView();
}

void CFileView::RefreshSelf(){
	HTREEITEM sel = m_wndFileView.GetSelectedItem();
	ASSERT(sel);
	if(!sel) return;
	cs::Layout* lay = (cs::Layout*)m_wndFileView.GetItemData(sel);
	ASSERT(lay);
	if(!lay) return;
	m_wndFileView.SetItemText(sel,lay->Name);
}
void CFileView::OnEditInsert(){
	HTREEITEM cur =  getSelectItem();
	if(!cur) return;
	HTREEITEM newItem;
	cs::String name;
	getUniqName(&glbDoc->topLay,name);
	HTREEITEM parent;
	int index = 0;
	cs::Layout* lay;
	if(cur==m_hRoot){
		newItem = m_wndFileView.InsertItem(name,0,1,cur);
		parent = cur;
		lay = NULL;
	}else{
		parent = m_wndFileView.GetParentItem(cur);
		newItem = m_wndFileView.InsertItem(name,0,1,parent,cur);
		lay = (cs::Layout*)m_wndFileView.GetItemData(cur);
		ASSERT(lay);
	}
	cs::Layout* pLay = (cs::Layout*)m_wndFileView.GetItemData(parent);
	if(lay){
		index = pLay->IndexOf(lay);
		if(index==-1){
			showErrorMessage(L"指定的布局 lay 不是 pLay 的子项。");
			ASSERT(0);
		}
		index++;
	}else{
		index = -1;
	}
	lay = pLay->Add(index);
	lay->Name = name;
	m_wndFileView.SetItemData(newItem,(DWORD_PTR)lay);
	m_wndFileView.SetItemText(newItem,name);
	m_wndFileView.SelectItem(newItem);
	glbOpt->EnableSaveButton(1);
}

void CFileView::OnEditCopy(){
	HTREEITEM item = getSelectItem();
	if(item==0) return;
	HTREEITEM parent = m_wndFileView.GetParentItem(item);

	cs::Layout* lay = (cs::Layout*)m_wndFileView.GetItemData(item);
	if(lay==0){
		showErrorMessage(L"节点绑定的 layout 数据为空");
		return;
	}
	if(!SetLayoutToClipboard(lay)){
		showErrorMessage(L"无法把数据放在剪贴板");
		return;
	}
}

void CFileView::OnEditPaste(){
	HTREEITEM item = getSelectItem();
	if(!item) return;

	cs::Layout* lay = new cs::Layout();
	if(!GetLayoutFromClipboard(lay)){
		delete lay;
		return;
	}
	cs::Layout* parent = (cs::Layout*)m_wndFileView.GetItemData(item);
	if(parent==0){
		showErrorMessage(L"节点绑定的 layout 数据为空");
		return;
	}
	parent->AddIn(lay);
	insertLayout(lay,item);
	glbView->Refresh(lay);
	m_wndFileView.Expand(item,TVE_EXPAND);
	glbOpt->EnableSaveButton(1);
}
void CFileView::OnEditCut(){
	HTREEITEM item = getSelectItem();
	if(item==0) return;
	HTREEITEM parent = m_wndFileView.GetParentItem(item);
	if(parent==0){
		showErrorMessage(L"不能剪切根节点");
		return;
	}

	cs::Layout* lay = (cs::Layout*)m_wndFileView.GetItemData(item);
	if(lay==0){
		showErrorMessage(L"节点绑定的 layout 数据为空");
		return;
	}
	if(!SetLayoutToClipboard(lay)){
		showErrorMessage(L"无法把数据放在剪贴板");
		return;
	}
	cs::Layout* parentLay = lay->GetParent();

	m_wndFileView.DeleteItem(item);
	parentLay->Delete(lay);
	glbOpt->EnableSaveButton(1);
	glbView->Refresh();
}

void CFileView::OnEditLeft(){
	HTREEITEM item = getSelectItem();
	if(!item) return;

	HTREEITEM parent = m_wndFileView.GetParentItem(item);
	if(parent==0) return;
	HTREEITEM grand = m_wndFileView.GetParentItem(parent);
	if(grand==0){
		showErrorMessage(L"顶级只能有一个布局");
		return;
	}
	moveTreeItem(item,grand,parent);
	glbOpt->EnableSaveButton(1);
}

void CFileView::OnEditUp(){
	HTREEITEM item = getSelectItem();
	if(!item) return;

	HTREEITEM prev = m_wndFileView.GetNextItem(item,TVGN_PREVIOUS);
	HTREEITEM insert = m_wndFileView.GetNextItem(prev,TVGN_PREVIOUS);
	if(insert==0) insert = TVI_FIRST;
	if(prev!=0)
		moveTreeItem(item,m_wndFileView.GetParentItem(prev),insert);
	glbOpt->EnableSaveButton(1);
}
void CFileView::OnEditRight(){
	HTREEITEM item = getSelectItem();
	if(!item) return;

	HTREEITEM prev = m_wndFileView.GetNextItem(item,TVGN_PREVIOUS);
	if(prev!=0)
		moveTreeItem(item,prev,TVI_LAST);
	glbOpt->EnableSaveButton(1);
}
void CFileView::OnEditDown(){
	HTREEITEM item = getSelectItem();
	if(!item) return;

	HTREEITEM next = m_wndFileView.GetNextItem(item,TVGN_NEXT);
	if(next!=0)
		moveTreeItem(item,m_wndFileView.GetParentItem(next),next);
	glbOpt->EnableSaveButton(1);
}
void CFileView::onEditCommand(WORD id,WORD type,cs::IWnd*){
	if(id==ID_EDIT_INSERT){
		OnEditInsert();
	}else if(id==ID_EDIT_COPY){
		OnEditCopy();
	}else if(id==ID_EDIT_PASTE){
		OnEditPaste();
	}else if(id==ID_EDIT_CUT){
		OnEditCut();
	}else if(id==ID_EDIT_LEFT){
		OnEditLeft();
	}else if(id==ID_EDIT_RIGHT){
		OnEditRight();
	}else if(id==ID_EDIT_UP){
		OnEditUp();
	}else if(id==ID_EDIT_DOWN){
		OnEditDown();
	}
}
void CFileView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}
HTREEITEM CFileView::FromLayout(){
	m_wndFileView.DeleteItem(m_hRoot);
	glbDoc->topLay.Refresh();
	m_hRoot = fromLayout(&glbDoc->topLay,TVI_ROOT);
	m_wndFileView.Expand(m_hRoot,TVE_EXPAND);
	m_wndFileView.SelectItem(m_hRoot);
	return m_hRoot;
}
HTREEITEM CFileView::fromLayout(cs::Layout* lay,HTREEITEM node){
	HTREEITEM child = m_wndFileView.InsertItem(lay->Name,0,1,node);
	m_wndFileView.SetItemData(child,(DWORD_PTR)lay);
	for(uint i=0;i<lay->Count();i++){
		fromLayout(lay->Element(i),child);
	}
	return child;
}
void CFileView::OnChangeVisualStyle(){
	m_FileViewImages.DeleteImageList();

	m_FileViewImages.Create(24,24,ILC_COLOR32,0,0);
	cs::Icon icon;
	icon.LoadIcon(IDI_TREE_NOR,24,24);
	m_FileViewImages.Add(icon);
	icon.LoadIcon(IDI_TREE_SEL,24,24);
	m_FileViewImages.Add(icon);

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}
