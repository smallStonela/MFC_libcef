#pragma once

#include "include/cef_app.h"
class CCEFDlgMFCAppDlg;
class CSimpleApp :public CefApp, public CefRenderProcessHandler, public CefBrowserProcessHandler
{
public:
	CSimpleApp();
	~CSimpleApp();
	
	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE
	{
		return this;
	}

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE
	{
		return this;
	}


	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE
	{
		command_line->AppendSwitch("--disable-web-security");
		//command_line->AppendSwitch("process-per-site");
		//command_line->AppendSwitch("--no-proxy-server");
		//command_line->AppendSwitch("--enable-media-stream");
		//command_line->AppendSwitch("--disable-browser-side-navigation");
		//command_line->AppendSwitch("--disable-gpu-shader-disk-cache");
		//command_line->AppendSwitch("--no-user-gesture-required");
	}
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;



private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CSimpleApp);
};