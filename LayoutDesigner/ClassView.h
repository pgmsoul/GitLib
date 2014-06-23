
#pragma once

#include "ViewTree.h"


class CClassView : public CDockablePane
{
public:
	CClassView();
	virtual ~CClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	void EnableSaveButton(BOOL enable);

protected:
	cs::View pad;
	cs::TopLayout topLay;
	cs::ImageList	butImageList[12];
	cs::Button*	opbut[12];
	cs::ToolTip	butToolTip;

	void onButCommand(WORD id,WORD type,cs::IWnd*);

	void FillClassView();

	// ÖØÐ´
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

extern CClassView* glbOpt;