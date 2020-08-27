#include "stdafx.h"
#include "SimpleV8Handler.h"


CSimpleV8Handler::CSimpleV8Handler()
{
}


CSimpleV8Handler::~CSimpleV8Handler()
{
}

// 3.JavaScript 调用 C++ 函数 Demo
/*
bool CSimpleV8Handler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	if (name == CefString("ShowJsInfo"))
	{
		if (arguments.size() == 1 && arguments[0]->IsString())
		{
			CefString arg = arguments[0]->GetStringValue();

			std::wstring strInfo(L"这是 JavaScript 发来的数据：");
			strInfo += arg.ToWString();

			::MessageBox(nullptr, strInfo.c_str(), L"C++ 提示框", MB_OK);

			return true;
		}
	}
	return false;
}*/

// 4.命名管道
/*
bool CSimpleV8Handler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	if (name == CefString("ShowJsInfo"))
	{
		if (arguments.size() == 1 && arguments[0]->IsString())
		{
			std::wstring wstrData;
			wstrData = arguments[0]->GetStringValue().ToWString();

			SendData(wstrData);
			return true;
		}
	}
	return false;
}*/

// 5.进程间通信
bool CSimpleV8Handler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	// send message to browser process that method began to execute
	CefRefPtr<CefProcessMessage> objMsg = CefProcessMessage::Create(CefString("msg_from_render"));
	CefRefPtr<CefListValue> argList = objMsg->GetArgumentList();
	argList->SetString(0, name);

	if (arguments.size() > 0)
	{
		for (size_t i = 0; i < arguments.size(); i++)
		{
			if (arguments[i]->IsString())
			{
				argList->SetString(i + 1, arguments[i]->GetStringValue());
			}
			else if (arguments[i]->IsInt())
			{
				argList->SetInt(i + 1, arguments[i]->GetIntValue());
			}
			else if (arguments[i]->GetDoubleValue())
			{
				argList->SetDouble(i + 1, arguments[i]->GetDoubleValue());
			}
		}
	}

	CefV8Context::GetCurrentContext()->GetBrowser()->SendProcessMessage(PID_BROWSER, objMsg);

	return true;
}

void CSimpleV8Handler::SendData(std::wstring strData)
{
	HANDLE hPipe;
	LPCTSTR lpvMessage = TEXT("Default message from client.");
	BOOL   fSuccess = FALSE;
	DWORD  cbToWrite, cbWritten, dwMode;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\cefnamedpipe");
	lpvMessage = strData.c_str();

	// Try to open a named pipe; wait for it, if necessary. 
	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

							// Break if the pipe handle is valid. 
		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			return;
		}

		// All pipe instances are busy, so wait for 20 seconds. 
		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return;
		}
	}

	// The pipe connected; change to message-read mode. 
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		return;
	}

	// Send a message to the pipe server. 
	cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);

	fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		lpvMessage,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		return;
	}

	CloseHandle(hPipe);
}