// ConverCodePage.cpp : 实现文件
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "ConverCodePage.h"
#include "afxdialogex.h"


// CConverCodePage 对话框

IMPLEMENT_DYNAMIC(CConverCodePage, CDialogEx)

CConverCodePage::CConverCodePage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConverCodePage::IDD, pParent)
{

}

CConverCodePage::~CConverCodePage()
{
}

void CConverCodePage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_hexUnicodeButton);
	DDX_Control(pDX, IDC_BUTTON2, m_decimalUnicodeButton);
	DDX_Control(pDX, IDC_BUTTON3, m_URL_GBKButton);
	DDX_Control(pDX, IDC_EDIT1, m_srcEdit);
	DDX_Control(pDX, IDC_EDIT2, m_resultEdit);
}


BEGIN_MESSAGE_MAP(CConverCodePage, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CConverCodePage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CConverCodePage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CConverCodePage::OnBnClickedButton3)
	ON_EN_SETFOCUS(IDC_EDIT2, &CConverCodePage::OnEnSetfocusEdit2)
END_MESSAGE_MAP()


// CConverCodePage 消息处理程序

#pragma region UI
// 屏蔽Esc关闭窗口
void CConverCodePage::OnCancel()
{
}

// 屏蔽回车关闭窗口
void CConverCodePage::OnOK()
{
}

// 销毁窗口
void CConverCodePage::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}
#pragma endregion

// 十进制Unicode
void CConverCodePage::OnBnClickedButton1()
{
	CStringW src;
	CString result, tmp;
	m_srcEdit.GetWindowTextW(src);
	const int len = src.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (src[i] == _T('\r') && src[i + 1] == _T('\n'))
		{
			i++;
			result += _T("\r\n");
		}
		else
		{
			tmp.Format(_T("&#%d;"), src[i]);
			result += tmp;
		}
	}
	m_resultEdit.SetWindowText(result);
}

// 十六进制Unicode
void CConverCodePage::OnBnClickedButton2()
{
	CStringW src;
	CString result, tmp;
	m_srcEdit.GetWindowTextW(src);
	const int len = src.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (src[i] == _T('\r') && src[i + 1] == _T('\n'))
		{
			i++;
			result += _T("\r\n");
		}
		else
		{
			tmp.Format(_T("&#x%X;"), src[i]);
			result += tmp;
		}
	}
	m_resultEdit.SetWindowText(result);
}

// URL_GBK
void CConverCodePage::OnBnClickedButton3()
{
	CString _src, result, tmp;
	m_srcEdit.GetWindowText(_src);
	CStringA src(_src);
	const int len = src.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (src[i] == '\r' && src[i + 1] == '\n')
		{
			i++;
			result += _T("\r\n");
		}
		else
		{
			tmp.Format(_T("%%%02X"), src[i] & 0xFF);
			result += tmp;
		}
	}
	m_resultEdit.SetWindowText(result);
}

// 获得焦点
void CConverCodePage::OnEnSetfocusEdit2()
{
	static BOOL ignore = FALSE;
	if (ignore)
	{
		ignore = FALSE;
		return;
	}

	CString text;
	m_resultEdit.GetWindowText(text);
	if (text == _T(""))
		return;

	const int len = (text.GetLength() + 1) * sizeof(TCHAR);
	HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE, len);
	if (hmem == NULL)
		return;
	LPTSTR p = (LPTSTR)GlobalLock(hmem);
	memcpy(p, (LPCTSTR)text, len);
	GlobalUnlock(hmem);

	if (!OpenClipboard())
		return;
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hmem);
	CloseClipboard();

	ignore = TRUE;
	AfxMessageBox(_T("已复制"), MB_ICONINFORMATION);
}
