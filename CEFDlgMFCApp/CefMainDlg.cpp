#include "CefMainDlg.h"



CefMainDlg::CefMainDlg(CWnd* pParent ): CDialogEx(IDD_DIALOG1, pParent)
{

}


CefMainDlg::~CefMainDlg()
{
}

 BOOL CefMainDlg::OnInitDialog()
{
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect m_Rect;
	m_Rect.left = 0;
	m_Rect.top = 0;
	m_Rect.right = m_Rect.left + cx;
	m_Rect.bottom = m_Rect.top + cy + 25;
	SetRect(m_Rect);
	//MoveWindow(0, 0, m_MainRect.Width(), m_MainRect.Height());
}

 void CefMainDlg::SetRect(CRect m_Rect)
 {
	 m_MainRect = m_Rect;
 }

 CRect CefMainDlg::GetRect() 
 {
	 return m_MainRect;
 }