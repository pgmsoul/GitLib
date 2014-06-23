
#pragma once

/*class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* , BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};*/

class CPropertiesWnd : public CDockablePane
{
// ππ‘Ï
public:
	CPropertiesWnd();

	void AdjustLayout();

//  Ù–‘
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	void Refresh(cs::Layout* lay);

protected:
	CFont m_fntPropList;
	cs::Layout* lay;
	CMFCPropertyGridCtrl m_wndPropList;
	cs::Json	winConst,*styleConst,*exConst,*ctrlConst;

	CMFCPropertyGridProperty* propName,*propMode,*propLength,*propWeight,*propLeft,
		*propTop,*propRight,*propBottom,*propCtrl,*propId,*propText,
		*propStyle,*propExStyle,*propCtrlStyle;

	CMFCPropertyGridProperty* initLayoutGroup();
	void initControlGroup();
	void setCtrlText(cs::ICtrl* ctrl);
	void setCtrlId(cs::ICtrl* ctrl);
	void setCtrlStyle(cs::ICtrl* ctrl);
	void setCtrlExStyle(cs::ICtrl* ctrl);
	void changeLayoutCtrl(bool reCreateControl = 1);
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

extern CPropertiesWnd* glbProp;