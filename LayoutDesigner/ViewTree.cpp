
#include "stdafx.h"
#include "ViewTree.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 消息处理程序

/*BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;

	if(pNMHDR->code==TVN_SELCHANGED){
		cs::Print(L"%s: %d",pnmtv->itemNew.pszText,pnmtv->itemNew.hItem);
	}
	return bRes;
}
*/
void CViewTree::RefreshPropAndView(cs::Layout* lay){
	if(lay==NULL){
		lay = (cs::Layout*)GetItemData(GetSelectedItem());
		if(lay==NULL) return;
	}
	glbView->Refresh(lay);
	glbProp->Refresh(lay);
}
void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	cs::Layout* lay = (cs::Layout*)GetItemData(pNMTreeView->itemNew.hItem);
	RefreshPropAndView(lay);
	*pResult = 0;
}
