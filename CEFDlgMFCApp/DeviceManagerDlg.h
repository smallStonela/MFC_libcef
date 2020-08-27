#pragma once


// DeviceManagerDlg 对话框
#include "../CefManager/SimpleClient.h"
#include "../CefManager/SimpleApp.h"

class DeviceManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DeviceManagerDlg)

public:
	DeviceManagerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DeviceManagerDlg();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DEVICEMANAGER };
#endif
public:
	CefRefPtr<CSimpleClient> m_SimpleClient;

	CefRefPtr<CSimpleApp> app;
	CefMainArgs main_args;
	
	void* sandbox_info;
	CefString m_strBrowserURL;
	CRect m_rtMid;
public:
	void SetRect(CRect rect);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
