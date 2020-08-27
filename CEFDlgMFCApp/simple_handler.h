// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include "include/wrapper/cef_resource_manager.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include <list>

class SimpleHandler : public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRequestHandler
{
public:
	explicit SimpleHandler(bool use_views);
	~SimpleHandler();

	// Provide access to the single global instance of this object.
	static SimpleHandler* GetInstance();

	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {return this;}
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {return this;}
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }
	
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this; }

	// CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,CefRefPtr<CefFrame> frame,ErrorCode errorCode,const CefString& errorText,const CefString& failedUrl) OVERRIDE;

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);

	bool IsClosing() const { return is_closing_; }
public:
	// CefRequestHandler methods:
	virtual cef_return_value_t OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) OVERRIDE;

private:
	// Platform-specific implementation.
	void PlatformTitleChange(CefRefPtr<CefBrowser> browser,const CefString& title);

	// True if the application is using the Views framework.
	const bool use_views_;

	// List of existing browser windows. Only accessed on the CEF UI thread.
	typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	BrowserList browser_list_;

	bool is_closing_;
	CefRefPtr<CefResourceManager> resource_manager_;
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(SimpleHandler);
};
class RequestDumpResourceProvider : public CefResourceManager::Provider
{
public:
	explicit RequestDumpResourceProvider(const std::string& url) : url_(url)
	{
		DCHECK(!url.empty());
	}

	bool OnRequest(scoped_refptr<CefResourceManager::Request> request) OVERRIDE
	{
		CEF_REQUIRE_IO_THREAD();

		//const std::string& url = request->url();
		//if (url != url_)
		//{
		//	// Not handled by this provider.
		//	return false;
		//}

		//std::string str = "<html><body bgcolor=\"white\"><pre>123123123123</pre></body></html>";
		//CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForData(
		//	static_cast<void*>(const_cast<char*>(str.c_str())), str.size());
		//DCHECK(stream.get());
		//request->Continue(new CefStreamResourceHandler("text/html", stream));
		request->Continue(NULL);
		return true;
	}

private:
	std::string url_;

	DISALLOW_COPY_AND_ASSIGN(RequestDumpResourceProvider);
};
#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
