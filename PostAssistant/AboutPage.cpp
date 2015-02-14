// AboutPage.cpp : 实现文件
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "AboutPage.h"
#include "afxdialogex.h"


// CAboutPage 对话框

IMPLEMENT_DYNAMIC(CAboutPage, CDialog)

CAboutPage::CAboutPage(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutPage::IDD, pParent)
{

}

CAboutPage::~CAboutPage()
{
}

void CAboutPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_STATIC1, m_link);
}


BEGIN_MESSAGE_MAP(CAboutPage, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_STN_CLICKED(IDC_STATIC1, &CAboutPage::OnStnClickedStatic1)
END_MESSAGE_MAP()


// CAboutPage 消息处理程序

#pragma region UI
// 屏蔽Esc关闭窗口
void CAboutPage::OnCancel()
{
}

// 屏蔽回车关闭窗口
void CAboutPage::OnOK()
{
}

// 销毁窗口
void CAboutPage::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}

void CAboutPage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect); // 默认343 * 216
	m_edit.SetWindowPos(NULL, 0, 0, rect.Width() - 19, rect.Height() - 40, SWP_NOMOVE | SWP_NOREDRAW);
	m_link.SetWindowPos(NULL, rect.Width() - 118, rect.Height() - 21, 0, 0, SWP_NOSIZE | SWP_NOREDRAW);

	Invalidate();
}
#pragma endregion

BOOL CAboutPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edit.SetWindowText(_T("设置好后用热键发贴\r\n\
~~~~~~~~~~~~~~~~~~\r\n\
\r\n\
此版本只支持IE内核的浏览器，Firefox和Chrome等非IE内核的浏览器会无效\r\n\
\r\n\
更新、Chrome版下载：http ://pan.baidu.com/s/1i3ooIFz\r\n\
\r\n\
此程序通过MIT协议开源，你可以在https://github.com/xfgryujk/PostAssistantForIE得到源码，欢迎完善程序"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CAboutPage::OnStnClickedStatic1()
{
	ShellExecute(NULL, _T("open"), _T("http://tieba.baidu.com/i/37897560"), NULL, NULL, SW_NORMAL);
}
