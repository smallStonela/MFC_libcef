#include "stdafx.h"
#include "SimpleClient.h"
enum {
	ID_CMD_REFRESH = 0,
	ID_SHOW_DEV_TOOLS
};


namespace {
	CSimpleClient* g_instance = NULL;
}

CSimpleClient::~CSimpleClient()
{
}


void CSimpleClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (!m_cefBrowser.get())
	{
		m_cefBrowser = browser;
		if (m_cefBrowser != NULL) 
		{
			m_BrowserHwnd = browser->GetHost()->GetWindowHandle();
		}		
	}

	browser_list_.push_back(browser);
}


bool CSimpleClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	if (message->GetName() == CefString("msg_from_render"))
	{
		CefRefPtr<CefListValue> argList = message->GetArgumentList();
		CefString strFunctionName = argList->GetString(0);
		if (strFunctionName == CefString("global_GJavascriptToCplus"))
		{
			CefString sCefMessage;
			if (argList->GetSize() >= 1)
			{
				sCefMessage = argList->GetString(1);
				std::wstring wstrMessage;
				wstrMessage = sCefMessage.ToWString();
				if (m_pDataObserver != NULL)
					m_pDataObserver->global_GJavascriptToCplus(wstrMessage.c_str());
			}
		}

		return true;
	}
	return false;   // has handle for the message
}

//
//bool CSimpleClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
//	CefRefPtr<CefFrame> frame,
//	const CefString& target_url,
//	const CefString& target_frame_name,
//	WindowOpenDisposition target_disposition,
//	bool user_gesture,
//	const CefPopupFeatures& popupFeatures,
//	CefWindowInfo& windowInfo,
//	CefRefPtr<CefClient>& client,
//	CefBrowserSettings& settings,
//	bool* no_javascript_access)
//{
//	switch (target_disposition)
//	{
//	case WOD_NEW_FOREGROUND_TAB:
//	case WOD_NEW_BACKGROUND_TAB:
//	case WOD_NEW_POPUP:
//	case WOD_NEW_WINDOW:
//		browser->GetMainFrame()->LoadURL(target_url);
//		return true; //cancel create
//	}
//
//	return false;
//}

void CSimpleClient::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback)
{
	callback->Continue(download_item->GetURL(), true);
}

void CSimpleClient::OnDownloadUpdated(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback)
{
	if (download_item->IsComplete())
	{
		//MessageBox.Show("下载成功");
		OutputDebugString(L"下载成功");
		if (browser->IsPopup() && !browser->HasDocument())
		{
			//browser->GetHost()->ParentWindowWillClose();
			//browser->GetHost()->CloseBrowser(true);
		}
	}
	else
	{
		//如果取消应该跳转到一个网页
		//browser->GoBack();
	}
}

bool CSimpleClient::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
	const CefKeyEvent& event,
	CefEventHandle os_event,
	bool* is_keyboard_shortcut)
{
	if (event.type != KEYEVENT_RAWKEYDOWN)
		return false;

	if (event.windows_key_code == 116)//F5刷新
	{
		//zj marked2020-1-7屏蔽F5
		//browser->Reload();
	}
	else if (event.windows_key_code == VK_F12)
	{
		if (m_bShowContextMenu)
		{
			ShowDevelopTools(browser, CefPoint(0, 0));
		}
	}
	return false;
}

bool CSimpleClient::OnKeyEvent(CefRefPtr<CefBrowser> browser,
	const CefKeyEvent& event,
	CefEventHandle os_event) {
	return false;
}

//右键菜单
void CSimpleClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model)
{
	if (model->GetCount() > 0) // 刷新菜单
	{
		if (m_bShowContextMenu)
		{
			model->AddSeparator();
			model->AddItem(ID_SHOW_DEV_TOOLS, __TEXT("DevTools"));
		}
		else
		{
			model->Clear();//清除右键菜单
			return;
		}
	}
}

bool CSimpleClient::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id, EventFlags event_flags)
{
	switch (command_id)
	{
	case ID_CMD_REFRESH:
		browser->Reload();
		break;
	case ID_SHOW_DEV_TOOLS:
	{
		ShowDevelopTools(browser, CefPoint(0, 0));
		break;
	}
	default:
		break;
	}
	return false;
}

//开发选项
void CSimpleClient::ShowDevelopTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at)
{
	CefWindowInfo windowInfo;
	CefBrowserSettings settings;
	windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
	browser->GetHost()->ShowDevTools(windowInfo, this, settings, CefPoint());
}

void CSimpleClient::CloseDevelopTools(CefRefPtr<CefBrowser> browser)
{
	browser->GetHost()->CloseDevTools();
}

bool CSimpleClient::DoClose(CefRefPtr<CefBrowser> browser)
{
	////同一个browser可能有多个浏览器窗口(在新的子窗口打开链接,而不是在当前窗口跳转时会出现.),此时返回值可能会被用来做一些特别处理.想了解请参考官方demo.
	//if (CefCurrentlyOn(TID_UI))
	//{
	//	return true;
	//}
	return false;	//返回true取消关闭
}
void CSimpleClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if (browser.get())
	{
		browser->GetHost()->CloseBrowser(false);
	}

	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) 
	{
		if ((*bit)->IsSame(browser)) 
		{
			browser_list_.erase(bit);
			break;
		}		
	}
	if (m_BrowserHwnd == browser->GetHost()->GetWindowHandle())
	{// 浏览器窗口被销毁的话,browser指针置为NULL.
		m_cefBrowser = NULL;
	}
}

cef_return_value_t CSimpleClient::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
{
	return resource_manager_->OnBeforeResourceLoad(browser, frame, request, callback);
}

CefRefPtr<CefResourceHandler> CSimpleClient::GetResourceHandler(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request)
{

	return resource_manager_->GetResourceHandler(browser, frame, request);

}

void SetupResourceManager(CefRefPtr<CefResourceManager> resource_manager)
{
	if (!CefCurrentlyOn(TID_IO))
	{
		// Execute on the browser IO thread.
		CefPostTask(TID_IO, base::Bind(SetupResourceManager, resource_manager));
		return;
	}
	// Add provider for bundled resource files.	
	resource_manager->AddProvider(new RequestDumpResourceProvider("http://www.baidu.com/"), 100, std::string());
}

CSimpleClient::CSimpleClient()
{
	//安装资源管理器
	DCHECK(!g_instance);
	g_instance = this;

	hWnd_ = NULL;
	m_bShowContextMenu = false;
	resource_manager_ = new CefResourceManager();
	SetupResourceManager(resource_manager_);
}
