
#include "stdafx.h"

#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
CPropertiesWnd* glbProp = 0;
CPropertiesWnd::CPropertiesWnd():propName(0){
	glbProp = this;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 消息处理程序
LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM,LPARAM lParam){
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	COleVariant vt = pProp->GetValue(); //改变之后的值
	if(pProp==propName){
		lay->Name = vt.bstrVal;
		glbTree->RefreshSelf();
	}else if(pProp==propMode){
		if(cs::WcsEqual((LPCWSTR)vt.bstrVal,L"Vertical"))
			lay->Param.SetVertical();
		else
			lay->Param.SetHorizontal();
	}else if(pProp==propLength){
		lay->Param.Length = (LONG)vt.lVal;
	}else if(pProp==propWeight){
		lay->Param.Weight = (float)vt.fltVal;
	}else if(pProp==propLeft){
		lay->Param.Margin.left = (LONG)vt.lVal;
	}else if(pProp==propTop){
		lay->Param.Margin.top = (LONG)vt.lVal;
	}else if(pProp==propRight){
		lay->Param.Margin.right = (LONG)vt.lVal;
	}else if(pProp==propBottom){
		lay->Param.Margin.bottom = (LONG)vt.lVal;
	}else if(pProp==propCtrl){
		changeLayoutCtrl();
	}else if(pProp==propId){
		cs::ICtrl* ctrl = lay->GetControl();
		ctrl->Param->Identity = pProp->GetValue().uiVal;
		setCtrlId(ctrl);
	}else if(pProp==propText){
		cs::ICtrl* ctrl = lay->GetControl();
		ctrl->Param->Text = pProp->GetValue().bstrVal;
		setCtrlText(ctrl);
	}else if(propStyle==pProp->GetParent()){
		VARIANT_BOOL hasStyle = pProp->GetValue().boolVal;
		LPCWSTR name = pProp->GetName();
		DWORD style = styleConst->PropertyInt32(name);
		cs::ICtrl* ctrl = lay->GetControl();
		if(hasStyle) ctrl->Param->AddStyle(style);
		else ctrl->Param->RemoveStyle(style);
		setCtrlStyle(ctrl);
	}else if(propExStyle==pProp->GetParent()){
		VARIANT_BOOL hasStyle = pProp->GetValue().boolVal;
		LPCWSTR name = pProp->GetName();
		DWORD style = exConst->PropertyInt32(name);
		cs::ICtrl* ctrl = lay->GetControl();
		if(hasStyle) ctrl->Param->AddExStyle(style);
		else ctrl->Param->RemoveExStyle(style);
		setCtrlExStyle(ctrl);
	}else if(propCtrlStyle==pProp->GetParent()){
		cs::ICtrl* ctrl = lay->GetControl();
		VARIANT_BOOL hasStyle = pProp->GetValue().boolVal;
		LPCWSTR name = pProp->GetName();
		DWORD style = ctrlConst->GetProperty(ctrl->Param->ClassName)->PropertyInt32(name);
		if(hasStyle) ctrl->Param->AddStyle(style);
		else ctrl->Param->RemoveStyle(style);
		setCtrlStyle(ctrl);
	}
	glbView->Refresh();
	glbOpt->EnableSaveButton(1);
	return 0;
}
void CPropertiesWnd::setCtrlText(cs::ICtrl* ctrl){
	ctrl->SetText(ctrl->Param->Text);
}
void CPropertiesWnd::setCtrlId(cs::ICtrl* ctrl){
	ctrl->SetWindowParam(GWL_ID,(LONG_PTR)ctrl->Param->Identity);
}
void CPropertiesWnd::setCtrlStyle(cs::ICtrl* ctrl){
	ctrl->SetWindowParam(GWL_STYLE,(LONG_PTR)ctrl->Param->Style);
}
void CPropertiesWnd::setCtrlExStyle(cs::ICtrl* ctrl){
	ctrl->SetWindowParam(GWL_EXSTYLE,(LONG_PTR)ctrl->Param->ExStyle);
}
bool hasStyle(cs::Json* json,DWORD style,LPCWSTR key){
	int v = json->PropertyInt32(key);
	if(v==0) return 0;
	return (v&style)==v;
}
void CPropertiesWnd::changeLayoutCtrl(bool reCreateControl){
	COleVariant vCtrl = propCtrl->GetValue();
	if(cs::WcsEqual(L"None",vCtrl.bstrVal)){
		lay->SetControl(0,0,0);
		propCtrlStyle->Show(0);
		propStyle->Show(0);
		propExStyle->Show(0);
		propText->Enable(0);
		propId->Enable(0);
	}else{
		propCtrlStyle->Expand(0);
		propStyle->Expand(0);
		propExStyle->Expand(0);
		COleVariant vStr = propText->GetValue();
		COleVariant vWord = propId->GetValue();
		if(reCreateControl)
			lay->SetControl(vCtrl.bstrVal,vWord.uiVal,vStr.bstrVal);
		cs::ICtrl* ctrl = lay->GetControl();
		ASSERT(ctrl);
		if(!ctrl) return;

		cs::Json* clist = ctrlConst->GetProperty(vCtrl.bstrVal);
		ASSERT(clist);
		if(!clist) return;

		int newCount = clist->Count();
		int oldCount = propCtrlStyle->GetSubItemsCount();
		for(int i=oldCount;i<newCount;i++){
			LPCWSTR key;
			clist->GetPropertyByIndex(i,&key);
			CMFCPropertyGridProperty* p = new CMFCPropertyGridProperty(key,(_variant_t)false,L"");
			propCtrlStyle->AddSubItem(p);
		}
		for(int i=oldCount-1;i>=newCount;i--){
			CMFCPropertyGridProperty* prop = propCtrlStyle->GetSubItem(i);
			propCtrlStyle->RemoveSubItem(prop);
		}

		for(int i=0;i<newCount;i++){
			LPCWSTR key;
			clist->GetPropertyByIndex(i,&key);
			propCtrlStyle->GetSubItem(i)->SetName(key,0);
			bool has = hasStyle(clist,ctrl->Param->Style,key);
			propCtrlStyle->GetSubItem(i)->SetValue((_variant_t)has);
		}

		int styleCount = styleConst->Count();
		for(int i=0;i<styleCount;i++){
			LPCWSTR key;
			styleConst->GetPropertyByIndex(i,&key);
			bool has = hasStyle(styleConst,ctrl->Param->Style,key);
			propStyle->GetSubItem(i)->SetValue((_variant_t)has);
		}

		int exCount = exConst->Count();
		for(int i=0;i<exCount;i++){
			LPCWSTR key;
			exConst->GetPropertyByIndex(i,&key);
			bool has = hasStyle(exConst,ctrl->Param->ExStyle,key);
			propExStyle->GetSubItem(i)->SetValue((_variant_t)has);
		}

		propCtrlStyle->Show(1);
		propStyle->Show(1);
		propExStyle->Show(1);
		propId->Enable(1);
		propText->Enable(1);

		m_wndPropList.AdjustLayout();
		m_wndPropList.RedrawWindow();
	}
}
void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);


	//int cyCmb = rectCombo.Size().cy;
	//int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + 0 + 0, rectClient.Width(), rectClient.Height() -(0+0), SWP_NOACTIVATE | SWP_NOZORDER);
}
void CPropertiesWnd::Refresh(cs::Layout* lay){
	this->lay = lay;
	if(!propName) return;//这个函数可能会先于初始化之前被呼叫一次，过滤掉。
	propName->SetValue(lay->Name.Handle());
	LPCWSTR mode = lay->Param.IsVertical()?L"Vertical":L"Horizontal";
	propMode->SetValue(mode);

	propLength->SetValue((LONG)lay->Param.Length);
	propWeight->SetValue((float)lay->Param.Weight);
	propLeft->SetValue((LONG)lay->Param.Margin.left);
	propTop->SetValue((LONG)lay->Param.Margin.top);
	propRight->SetValue((LONG)lay->Param.Margin.right);
	propBottom->SetValue((LONG)lay->Param.Margin.bottom);

	cs::ICtrl* ctrl = lay->GetControl();
	if(ctrl){
		propCtrl->SetValue(ctrl->Param->ClassName.Handle());
		propText->SetValue(ctrl->Param->Text.Handle());
		propId->SetValue((_variant_t)ctrl->Param->Identity);
	}else{
		propCtrl->SetValue(L"None");
		propText->SetValue(L"");
		propId->SetValue((_variant_t)(USHORT)0);
	}
	changeLayoutCtrl(0);
}
int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	InitPropList();

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}
CMFCPropertyGridProperty* CPropertiesWnd::initLayoutGroup(){
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("布局属性"));

	propName = new CMFCPropertyGridProperty(_T("名称"), (_variant_t) _T("Layout1"), _T("指定窗口标题栏中显示的文本"));
	pGroup->AddSubItem(propName);

	propMode = new CMFCPropertyGridProperty(_T("方向"), _T("Horizontal"), _T("指示子 Layout 的排列方向，可选：水平排列（缺省），垂直排列。"));
	propMode->AddOption(_T("Horizontal"));
	propMode->AddOption(_T("Vertical"));
	propMode->AllowEdit(FALSE);
	pGroup->AddSubItem(propMode);

	propLength = new CMFCPropertyGridProperty(_T("长度"), (_variant_t) 0l, _T("指示布局的最小长度"));
	pGroup->AddSubItem(propLength);

	propWeight = new CMFCPropertyGridProperty(_T("权重"), (_variant_t) 1.0f, _T("指示布局获得父布局剩余尺寸的权重，如果这个值为 0 ，布局的长度固定为 length。"));
	pGroup->AddSubItem(propWeight);

	propLeft = new CMFCPropertyGridProperty(_T("左边距"), (_variant_t) 10l, _T("左边框与父 Layout 左边框的距离"));
	pGroup->AddSubItem(propLeft);

	propTop = new CMFCPropertyGridProperty( _T("上边距"), (_variant_t) 10l, _T("上边框与父 Layout 上边框的距离"));
	pGroup->AddSubItem(propTop);

	propRight = new CMFCPropertyGridProperty( _T("右边距"), (_variant_t) 10l, _T("右边框与父 Layout 右边框的距离"));
	pGroup->AddSubItem(propRight);

	propBottom = new CMFCPropertyGridProperty( _T("下边距"), (_variant_t) 10l, _T("下边框与父 Layout 下边框的距离"));
	pGroup->AddSubItem(propBottom);

	return pGroup;
}
void initStyleGroup(CMFCPropertyGridProperty* group,cs::Json* json){
	for(uint i=0;i<json->Count();i++){
		LPCWSTR key;
		json->GetPropertyByIndex(i,&key);
		CMFCPropertyGridProperty* p = new CMFCPropertyGridProperty(key,(_variant_t)false,L"");
		group->AddSubItem(p);
	}
	group->Show(0);
	group->Expand(0);
}
void CPropertiesWnd::initControlGroup(){
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("控件属性"));
	m_wndPropList.AddProperty(pGroup);

	propCtrl = new CMFCPropertyGridProperty(_T("控件类型"), (_variant_t) _T("None"), _T("当前 Layout 绑定的控件类型"));

	LPCWSTR ctrlNameList = L"None|Static|Button|Edit|ComboBoxEx32|ComboBox|ListBox|SysDateTimePick32|SysIPAddress32|SysLink|SysHeader32|msctls_updown32|msctls_trackbar32msctls_statusbar32|SysListView32|msctls_hotkey32|SysTabControl32|msctls_progress32|ToolbarWindow32|SysTreeView32|SysMonthCal32";
	cs::StringList sl;
	sl.SplitString(ctrlNameList,L"|");
	for(uint i=0;i<sl.Count();i++){
		propCtrl->AddOption(sl[i]);
	}
	propCtrl->AllowEdit(FALSE);
	pGroup->AddSubItem(propCtrl);

	propText = new CMFCPropertyGridProperty(_T("控件文本"), (_variant_t) _T(""), _T("控件显示的文本"));
	pGroup->AddSubItem(propText);

	_variant_t vt;
	vt.vt = VT_UI2;
	vt.iVal = 0;
	propId = new CMFCPropertyGridProperty(_T("控件ID"), vt, _T("控件的ID，WORD 类型"));
	pGroup->AddSubItem(propId);

	if(!winConst.LoadFromFile(L"control_const.json")){
		DWORD sz = 0;
		void* res = cs::GetResource(IDJ_CONST,L"RT_JSON",&sz);
		if(res==NULL){
			showErrorMessage(L"读取常数资源出错!");
			ExitProcess(0);
		}
		cs::File f;
		if(!f.Create(L"control_const.json")){
			showErrorMessage(L"无法生成 control_const.json 文件!");
			ExitProcess(0);
		}
		f.SetLength(0);
		f.Write(res,sz);
		f.Close();
		if(!winConst.LoadFromFile(L"control_const.json")){
			showErrorMessage(L"读取 control_const.json 出错!");
			ExitProcess(0);
		}
	}
	cs::Json* cj = winConst.GetProperty(L"Const");
	styleConst = cj->GetProperty(L"WindowStyle");
	exConst = cj->GetProperty(L"WindowExStyle");
	ctrlConst = cj->GetProperty(L"ControlStyle");

	propCtrlStyle = new CMFCPropertyGridProperty(_T("控件风格"));
	m_wndPropList.AddProperty(propCtrlStyle);
	propCtrlStyle->Show(0);

	propStyle = new CMFCPropertyGridProperty(_T("窗口风格"));
	initStyleGroup(propStyle,styleConst);
	m_wndPropList.AddProperty(propStyle);

	propExStyle = new CMFCPropertyGridProperty(_T("窗口额外风格"));
	initStyleGroup(propExStyle,exConst);
	m_wndPropList.AddProperty(propExStyle);
}

void CPropertiesWnd::InitPropList(){
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroupLayout = initLayoutGroup();
	m_wndPropList.AddProperty(pGroupLayout);

	initControlGroup();
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}
