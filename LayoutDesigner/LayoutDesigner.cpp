
// LayoutDesigner.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLayoutApp

BEGIN_MESSAGE_MAP(CLayoutApp, CWinAppEx)
	ON_COMMAND(ID_APP_HELP, &CLayoutApp::OnAppHelp)
	ON_COMMAND(ID_APP_ABOUT, &CLayoutApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CLayoutApp::OnFileOpen)
END_MESSAGE_MAP()

void CLayoutApp::OnAppHelp(){
	::ShellExecute(0,_T("open"),L"http://jsuser.sinaapp.com",NULL,NULL,SW_SHOWNORMAL);
}

void CLayoutApp::OnFileOpen(){
	fileIsZero = 1;
	CWinAppEx::OnFileOpen();
	if(fileIsZero){
		CString str = glbDoc->GetPathName();
		cs::String msg;
		msg.Format(L"文件长度为 0 ！\r\n%s",str.GetBuffer());
		showErrorMessage(msg);
	}
}
// CLayoutApp 构造

CLayoutApp::CLayoutApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CLayoutApp 对象

CLayoutApp theApp;


// CLayoutApp 初始化

BOOL CLayoutApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Jsuser"));
	LoadStdProfileSettings(10);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLayoutDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CLayoutView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslinkHome(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_HOME, &CAboutDlg::OnNMClickSyslinkHome)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CLayoutApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CLayoutApp 自定义加载/保存方法

void CLayoutApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CLayoutApp::LoadCustomState()
{
}

void CLayoutApp::SaveCustomState()
{
}

// CLayoutApp 消息处理程序




void CAboutDlg::OnNMClickSyslinkHome(NMHDR *pNMHDR, LRESULT *pResult){
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	::ShellExecute(0,_T("open"),pNMLink->item.szUrl,NULL,NULL,SW_SHOWNORMAL);
	*pResult = 0;
}
