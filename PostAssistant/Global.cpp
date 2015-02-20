#include "stdafx.h"
#include "Global.h"
#include <vector>
#include <msxml2.h>


CString PROFILE_PATH;


// 分割字符串
void SplitString(CStringArray& dst, const CString& src, LPCTSTR slipt)
{
	dst.RemoveAll();
	const int len = _tcslen(slipt);

	int start = 0, end = 0;
	while ((end = src.Find(slipt, end)) != -1)
	{
		dst.Add(src.Mid(start, end - start));
		start = end += len;
	}
	dst.Add(src.Right(src.GetLength() - start));
}

// 枚举寻找Internet Explorer_Server窗口
static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	TCHAR buf[30];
	GetClassName(hwnd, buf, _countof(buf));
	if (_tcscmp(buf, _T("Internet Explorer_Server")) == 0)
	{
		*(HWND*)lParam = hwnd;
		return FALSE;
	}
	else
		return TRUE;
}

// 取鼠标位置IE网页文档接口
BOOL GetIEDocument(CComPtr<IHTMLDocument2>& spDoc)
{
	BOOL result = FALSE;

	// 取鼠标位置窗口句柄
	POINT point;
	GetCursorPos(&point);
	HWND hWnd = WindowFromPoint(point);
	// 取Internet Explorer_Server窗口句柄
	TCHAR buf[30];
	GetClassName(hWnd, buf, _countof(buf));
	if (_tcscmp(buf, _T("Internet Explorer_Server")) != 0)
	{
		hWnd = NULL;
		EnumChildWindows(hWnd, EnumChildProc, (LPARAM)&hWnd);
	}
	
	// 取document接口
	if (hWnd != NULL)
	{
		LRESULT lRes;
		UINT nMsg = RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
		SendMessageTimeout(hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes);
		result = SUCCEEDED(ObjectFromLresult(lRes, IID_IHTMLDocument2, 0, (void**)&spDoc));
	}

	return result;
}

// 执行JavaScript
HRESULT EvalJS(CComPtr<IHTMLDocument2>& document, LPCTSTR script, VARIANT* ret)
{
	HRESULT hr;

	// 取window
	CComPtr<IHTMLWindow2> window;
	if (FAILED(hr = document->get_parentWindow(&window)))
		return hr;

	// 取eval ID
	static DISPID dispID = -1;
	if (dispID == -1)
	{
		CComPtr<IDispatchEx> dispEx;
		if (FAILED(hr = window->QueryInterface(&dispEx)))
			return hr;
		if (FAILED(hr = dispEx->GetDispID(_bstr_t("eval"), fdexNameCaseSensitive, &dispID)))
			return hr;
	}

	// 调用eval
	CComDispatchDriver disp = window;
	hr = disp.Invoke1(dispID, &_variant_t(script), ret);
	
	return hr;
}

// 转义
void EscapeString(CString& src)
{
	src.Replace(_T("&"), _T("&amp;"));
	src.Replace(_T(" "), _T("&nbsp;"));
	src.Replace(_T("\""), _T("\\\""));
	src.Replace(_T("'"), _T("\\'"));
	src.Replace(_T("'"), _T("\\'"));
	src.Replace(_T("<"), _T("&lt;"));
	src.Replace(_T(">"), _T("&gt;"));
}

// 取网络图片尺寸
BOOL GetInternetImageSize(LPCTSTR URL, int& width, int& height)
{
	// 下载图片
	BYTE* buffer;
	ULONG size;
	HTTPGet(URL, &buffer, &size);
	if (buffer == NULL)
	{
		width = height = 500;
		return FALSE;
	}
	
	// 创建流
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, size);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, buffer, size);
	delete buffer;
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// 加载到CImage
	CImage image;
	image.Load(pstm);
	// 释放流
	GlobalUnlock(m_hMem);
	pstm->Release();

	width  = image.GetWidth();
	height = image.GetHeight();
	return TRUE;
}

// http get请求，注意自行delete buffer
void HTTPGet(LPCTSTR URL, BYTE** buffer, ULONG* size)
{
	if (buffer != NULL)
		*buffer = NULL;
	if (size != NULL)
		*size = 0;
	CComPtr<IServerXMLHTTPRequest> xml;
	if (FAILED(xml.CoCreateInstance(__uuidof(ServerXMLHTTP))))
		return;

	xml->open(_bstr_t(_T("GET")), _bstr_t(URL), _variant_t(false), _variant_t(), _variant_t());
	xml->setTimeouts(5000, 5000, 5000, 5000);
	xml->send(_variant_t());
	long state = 0;
	xml->get_readyState(&state);
	if (state == 4 && buffer != NULL && size != NULL)
	{
		_variant_t body;
		xml->get_responseBody(body.GetAddress());
		BYTE* p;
		if (SUCCEEDED(SafeArrayAccessData(body.parray, (void**)&p)))
		{
			*size = body.parray->rgsabound[0].cElements;
			*buffer = new BYTE[*size];
			memcpy(*buffer, p, *size);
			SafeArrayUnaccessData(body.parray);
		}
	}
}
