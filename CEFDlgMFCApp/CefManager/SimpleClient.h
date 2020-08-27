#pragma once

#include "include/cef_client.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_resource_manager.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include "include/wrapper/cef_byte_read_handler.h"

#include <fstream>
#include "./Encryption/AES/AES_API.h" //AES加解密
#include "./Encryption/AES/openssl/evp.h"//AES加解密
//AES加密
#pragma comment(lib, "./Encryption/AES/lib/ssleay32.lib")
#pragma comment(lib, "./Encryption/AES/lib/libeay32.lib")
class IDataObserver
{
public:
	virtual void global_GJavascriptToCplus(LPCTSTR strMessage) = 0;
};

class CSimpleClient :
	public CefDownloadHandler,
	public CefKeyboardHandler,
	public CefContextMenuHandler,
	public CefBrowserProcessHandler,
	public CefApp,
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefRequestHandler
{
public:
	CSimpleClient();
	~CSimpleClient();
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
	{
		return this;
	}
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE
	{
		return this;
	}

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE
	{
		return this;
	}

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message);// OVERRIDE;

	CefRefPtr<CefBrowser> GetBrowser()
	{
		return m_cefBrowser;
	}

	/*virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) override;*/

		//download
	virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() { return this; }

	virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
		const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;

	virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

	//F5刷新功能
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() //实现
	{
		return this;
	}

	virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
		const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut) OVERRIDE;

	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
		const CefKeyEvent& event,
		CefEventHandle os_event) OVERRIDE;


	//添加右键菜单
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() //实现
	{
		return this;
	}

	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) OVERRIDE;

	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		EventFlags event_flags) OVERRIDE;

#if 1
	virtual cef_return_value_t OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request) OVERRIDE;

#endif
	//show dev tools
	void ShowDevelopTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at);
	void CloseDevelopTools(CefRefPtr<CefBrowser> browser);
	virtual bool DoClose(CefRefPtr<CefBrowser> browser);
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser);

private:
	CefRefPtr<CefBrowser> m_cefBrowser;
	CefWindowHandle m_BrowserHwnd;
	HWND hWnd_; //接收消息的句柄

	IMPLEMENT_REFCOUNTING(CSimpleClient);

private:
	IDataObserver* m_pDataObserver;
	bool m_bShowContextMenu;

	typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	BrowserList browser_list_;

public:
	CefRefPtr<CefResourceManager> resource_manager_;

	void SetDataObserver(IDataObserver* pDataObserver)
	{
		m_pDataObserver = pDataObserver;
	}

	//zj+20200113控制界面是否显示右键菜单
	void ControlShowContextMenu(bool bShowContextMenu)
	{
		m_bShowContextMenu = bShowContextMenu;
	}
};


#define AES_KEY "201809261500@bnc";
class RequestDumpResourceProvider : public CefResourceManager::Provider
{
public:
	explicit RequestDumpResourceProvider(const std::string& url) : url_(url)
	{

	}
	virtual ~RequestDumpResourceProvider()
	{

	}

	bool OnRequest(scoped_refptr<CefResourceManager::Request> request) OVERRIDE
	{
		CEF_REQUIRE_IO_THREAD();

		int nRet = 0;
		std::string& url = request->url();
		//codec
		std::string ss = ".codec";
		url += ss;
		string subStr = url.substr(8, url.length() - 1);
		ifstream _file(subStr, ios::binary);

		if (!_file)
		{
			//if ((request->url().length() - request->url().find(".pdf")) == 4) 
			//{
			//	request->Continue(NULL);
			//	return false;
			//}
			request->Continue(NULL);
		}
		else
		{
			//char *buffer;
			string mime_type_ = request->mime_type_resolver().Run(url);
			unsigned char g_EncryKey[] = "201809261500@bnc";
			bool bEncryptPwd = true;

			_file.seekg(0, std::ios::end);		// go to the end  
			long long length = _file.tellg();   // report location (this is the length)  
			_file.seekg(0, std::ios::beg);		// go back to the beginning  

			char *buffer = new char[length];			// allocate memory for a buffer of appropriate dimension  
			memset(buffer, 0, length);
			_file.read(buffer, length);			// read the whole file into the buffer  

			//codec
			unsigned char* deOutPutPwd = new unsigned char[length];
			memset(deOutPutPwd, 0, length);

			if (bEncryptPwd)//Web端支持AES加密，组件按照AES方式解密
			{
				long long encode_str_size = length;
				nRet = AES_decrypt((unsigned char*)buffer, length, deOutPutPwd, g_EncryKey);//对Base64解密后的串再进行AES解密		
			}
			CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForData(static_cast<void*>((deOutPutPwd)), nRet);

			DCHECK(stream.get());
			request->Continue(new CefStreamResourceHandler(mime_type_, stream));
			//OutputDebugStringA(mime_type_.c_str());
			delete[] buffer;
			delete[] deOutPutPwd;

			_file.close();                    // close file handle  
		}
		return true;
	}

private:
	std::string url_;
	DISALLOW_COPY_AND_ASSIGN(RequestDumpResourceProvider);

};