#include "stdafx.h"
#include "SimpleApp.h"
#include "SimpleV8Handler.h"
#include "SimpleClient.h"
#include "../CEFDlgMFCAppDlg.h"
CSimpleApp::CSimpleApp()
{
}


CSimpleApp::~CSimpleApp()
{

}

void CSimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	// The var type can accept all object or variable
	
	//AfxMessageBox(L"OnContextCreated");
	CefRefPtr<CefV8Value> pV8 = context->GetGlobal();

	// bind function 
	CefRefPtr<CSimpleV8Handler> pJsHandler(new CSimpleV8Handler());
	
	CefRefPtr<CefV8Value> pFunc = CefV8Value::CreateFunction(CefString("global_GJavascriptToCplus"), pJsHandler);
	pV8->SetValue(CefString("global_GJavascriptToCplus"), pFunc, V8_PROPERTY_ATTRIBUTE_NONE);
}

void CSimpleApp::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	
	return;
}