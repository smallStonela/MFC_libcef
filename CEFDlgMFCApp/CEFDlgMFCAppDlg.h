
// CEFDlgMFCAppDlg.h: 头文件
//

#pragma once
#include "CefManager/SimpleClient.h"
#include "afxwin.h"
#include <algorithm>
#include <afxinet.h>
#include "../DeviceManagerDlg.h"

// CCEFDlgMFCAppDlg 对话框
class CCEFDlgMFCAppDlg : public CDialogEx, public IDataObserver
{
// 构造
public:
	CCEFDlgMFCAppDlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~CCEFDlgMFCAppDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CEFDLGMFCAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void global_GJavascriptToCplus(LPCTSTR strMessage);
public:
	CefRefPtr<CSimpleClient> m_SimpleClient;
	CefBrowserSettings m_BrowserSettings;
	CefWindowInfo m_WinInfo;
	CRect m_MainRect;
	HICON m_hIcon;
// 实现
protected:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	DeviceManagerDlg *m_DeviceDlg;

public:
	afx_msg void OnBnClickedOk();
};
