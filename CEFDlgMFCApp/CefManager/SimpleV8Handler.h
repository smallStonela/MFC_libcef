#pragma once

#include "include/cef_v8.h"

class CSimpleV8Handler : public CefV8Handler
{
public:
	CSimpleV8Handler();
	~CSimpleV8Handler();

public:
	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;

	void SendData(std::wstring strData);
private:
	IMPLEMENT_REFCOUNTING(CSimpleV8Handler);
};