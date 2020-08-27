// DeviceManagerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "CEFDlgMFCApp.h"
#include "DeviceManagerDlg.h"

// DeviceManagerDlg 对话框

IMPLEMENT_DYNAMIC(DeviceManagerDlg, CDialogEx)

DeviceManagerDlg::DeviceManagerDlg(CWnd* pParent /*=nullptr*/): CDialogEx(IDD_DLG_DEVICEMANAGER, pParent)
{
}

DeviceManagerDlg::~DeviceManagerDlg()
{
}


BOOL DeviceManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyle(0, WS_SYSMENU | WS_MAXIMIZEBOX);

	// TODO:  在此添加额外的初始化
	CefRefPtr<CSimpleClient> client(new CSimpleClient());
	m_SimpleClient = client;

	CefSettings settings;
	CefSettingsTraits::init(&settings);
	//settings.multi_threaded_message_loop = true;
	
	CefMainArgs mainArgs;
	CefRefPtr<CefApp> cefApp;
	CefInitialize(mainArgs, settings, cefApp, NULL);
	MoveWindow(m_rtMid.left, m_rtMid.top, m_rtMid.right - m_rtMid.left, m_rtMid.bottom - m_rtMid.top);
	CefWindowInfo winInfo;
	winInfo.SetAsChild(GetSafeHwnd(), m_rtMid);

	CefBrowserSettings browserSettings;
	//m_strBrowserURL = "D://gsoft//GiMAP//BFICPM//app//help//help.pdf";
	m_strBrowserURL = "http://www.baidu.com";
	CefBrowserHost::CreateBrowser(winInfo, m_SimpleClient, CefString(m_strBrowserURL), browserSettings, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void DeviceManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void DeviceManagerDlg::OnClose()
{
	//CefQuitMessageLoop();	
	//CefShutdown();
	//m_SimpleClient.get()->GetBrowser()->StopLoad();
	//m_SimpleClient.get()->GetBrowser()->GetHost()->CloseBrowser(false);
	CDialogEx::OnClose();
}

void DeviceManagerDlg::SetRect(CRect rect)
{
	m_rtMid = rect;
}



BEGIN_MESSAGE_MAP(DeviceManagerDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// DeviceManagerDlg 消息处理程序
