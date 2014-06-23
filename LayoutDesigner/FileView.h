
#pragma once

#include "ViewTree.h"

class CFileView : public CDockablePane
{
	// 构造
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	HTREEITEM FromLayout();
	void onEditCommand(WORD id,WORD type,cs::IWnd*);
	void Reset();
	void RefreshSelf();
	//刷新当前选中项的文本。
	// 属性
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	HTREEITEM	m_hRoot;
	//CFileViewToolBar m_wndToolBar;
	HTREEITEM getSelectItem();
	void moveTreeItem(HTREEITEM item,HTREEITEM parent,HTREEITEM pos);
	void copyChildItem(HTREEITEM item,HTREEITEM newItem);
	HTREEITEM copyTreeItem(HTREEITEM item,HTREEITEM parent,HTREEITEM pos);
	void insertLayout(cs::Layout* lay,HTREEITEM node);
	HTREEITEM fromLayout(cs::Layout* lay,HTREEITEM node);

protected:
	void FillFileView();

	// 实现
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditDown();
	afx_msg void OnEditUp();
	afx_msg void OnEditRight();
	afx_msg void OnEditLeft();
	afx_msg void OnEditInsert();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

extern CFileView* glbTree;