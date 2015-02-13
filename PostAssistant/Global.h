#pragma once
#include <oleacc.h>
#include <mshtml.h>

// 配置文件路径
extern CString PROFILE_PATH;
const TCHAR TEXT_SIGNS_PATH[] = _T("text.list");
const TCHAR RICH_TEXT_SIGNS_PATH[] = _T("richtext.list");

// 热键
static const int HOTKEY_POST = 0;
static const int HOTKEY_UNICODE_POST = 1;
static const int HOTKEY_HTML_POST = 2;


// 字符串操作
void SliptString(CStringArray& dst, const CString& src, LPCTSTR slipt);
CString ReplaceString(const CString& src, LPCTSTR oldString, LPCTSTR newString);

// 网页操作
BOOL GetIEDocument(CComPtr<IHTMLDocument2>& spDoc);
HRESULT EvalJS(CComPtr<IHTMLDocument2>& document, LPCTSTR script, VARIANT* ret = NULL);

// 取[0, max - 1]随机数
inline int rand(int max)
{
	return (int)((float)rand() / (RAND_MAX + 1) * max);
}

// 不重复取随机成员
inline CString GetUnrepeatedElement(CStringArray& arr, int& rest)
{
	if (rest == 0)
		rest = arr.GetSize();
	int rnd = rand(rest);

	CString tmp = arr[rnd];
	arr[rnd] = arr[rest - 1];
	arr[rest - 1] = tmp;
	rest--;

	return tmp;
}

// 转义、编码
void EscapeString(CString& src);

// 网络操作
BOOL GetInternetImageSize(LPCTSTR URL, int& width, int& height);
void HTTPGet(LPCTSTR URL, BYTE** buffer = NULL, ULONG* size = NULL);
