/*
The MIT License (MIT)

Copyright (c) 2015 xfgryujk

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdafx.h"
#include "PostAssistant.h"
#include "PostAssistantDlg.h"
#include "afxdialogex.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 常量
static const UINT WM_TASKBARCREATED = RegisterWindowMessage(_T("TaskbarCreated"));
static const UINT WM_TRAY = WM_APP + 1;

#pragma region
static const TCHAR POST_SCRIPT1[] = _T("UE.instants.ueditorInstant0.getContent=function(){\
var d=UE.Editor.prototype.getContent.call(this);return d.replace(/&#39;/g,\"'\").replace(/&quot;/g,'\"')\
.replace(/(^(<br\\/>)+)|((<br\\/>)+$)/g,\"\")+'");
static const TCHAR NO_OLD_CONTENT_POST_SCRIPT1[] = _T("UE.instants.ueditorInstant0.getContent = function(){return '");
static const TCHAR KEEP_FORMAT_POST_SCRIPT1[] = _T("UE.instants.ueditorInstant0.getContent=function(){\
var d=UE.Editor.prototype.getContent.call(this);return d.replace(/&#39;/g,\"'\").replace(/&quot;/g,'\"')\
.replace(/(^(<br\\/>)+)|((<br\\/>)+$)/g,\"\").replace(/&nbsp;&nbsp;/g,\"　　\").replace(/<br\\/>/g,\"<br>\")+'");
static const TCHAR POST_SCRIPT2[] = _T("'};$(\".poster_submit\")[0].click();");

static const TCHAR VIDEO1[] = _T("<embed class = \"BDE_Flash\" type=\"application/x-shockwave-flash\" \
pluginspage=\"http://www.macromedia.com/go/getflashplayer\" wmode=\"transparent\" play=\"true\" \
loop=\"false\" menu=\"false\" src=\"");
static const TCHAR VIDEO2[] = _T("\" width=\"500\" height=\"450\" allowscriptaccess=\"never\" \
allowfullscreen=\"true\" scale=\"noborder\">");

static const TCHAR IMAGE_FORMAT[] = _T("<img class = \"BDE_Image\" src = \"%s\" unselectable=\"on\" \
pic_type=\"1\" height=\"%d\" width=\"%d\" / >");

static const TCHAR GET_CONTENT_SCRIPT[] = _T("document.getElementById('ueditor_replace').innerHTML");
static const TCHAR SET_CONTENT_SCRIPT1[] = _T("document.getElementById('ueditor_replace').innerHTML = '");
static const TCHAR SET_CONTENT_SCRIPT2[] = _T("';");
#pragma endregion


// 构造函数
CPostAssistantDlg::CPostAssistantDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPostAssistantDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 初始化m_pages
	m_pages[0] = &m_settingPage;
	m_pages[1] = &m_textSignPage;
	m_pages[2] = &m_richTextSignPage;
	m_pages[3] = &m_converCodePage;
	m_pages[4] = &m_aboutPage;

	// 初始化托盘图标数据
	m_nfData.cbSize				= sizeof(NOTIFYICONDATA);
	m_nfData.hIcon				= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_tcscpy_s(m_nfData.szTip, _T("发贴助手"));
	m_nfData.uCallbackMessage	= WM_TRAY;
	m_nfData.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
}

#pragma region MFC
void CPostAssistantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CPostAssistantDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CPostAssistantDlg::OnTcnSelchangeTab1)
	ON_WM_HOTKEY()
	ON_MESSAGE(WM_TRAY, OnTray)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreated)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPostAssistantDlg 消息处理程序


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPostAssistantDlg::OnPaint()
{
	static BOOL first = TRUE;
	if (first)
	{
		first = FALSE;
		CStringArray argv;
		SliptString(argv, AfxGetApp()->m_lpCmdLine, _T(" "));
		for (int i = 0; i < argv.GetSize(); i++)
			if (argv[i].MakeLower() == _T("-hide")) // 初始隐藏窗口，放在OnInitDialog无效
			{
				ShowWindow(SW_HIDE);
				Shell_NotifyIcon(NIM_ADD, &m_nfData);
				break;
			}
		return;
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPostAssistantDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#pragma endregion

// 初始化
BOOL CPostAssistantDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化托盘窗口句柄
	m_nfData.hWnd = m_hWnd;

	// 初始化m_tab
	m_tab.InsertItem(0, _T("设置"));
	m_tab.InsertItem(1, _T("文字签名"));
	m_tab.InsertItem(2, _T("富文本签名"));
	m_tab.InsertItem(3, _T("编码转换"));
	m_tab.InsertItem(4, _T("帮助&&关于"));

	// 初始化各页
	m_settingPage.Create(IDD_SETTING, &m_tab);
	m_textSignPage.Create(IDD_TEXTSIGN, &m_tab);
	m_richTextSignPage.Create(IDD_RICHTEXTSIGN, &m_tab);
	m_converCodePage.Create(IDD_CONVERTCODE, &m_tab);
	m_aboutPage.Create(IDD_ABOUT, &m_tab);

	CRect rect;
	m_tab.GetClientRect(&rect);
	rect.left += 1; rect.right -= 3; rect.top += 22; rect.bottom -= 2;
	m_settingPage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	m_textSignPage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_richTextSignPage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_converCodePage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);
	m_aboutPage.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	// 读取设置
	TCHAR tmpText[10];
	// 使用文字签名
	GetPrivateProfileString(_T("设置"), _T("text"), _T("1"), tmpText, _countof(tmpText), PROFILE_PATH);
	m_settingPage.m_textSignCheck.SetCheck(tmpText[0] == _T('1'));
	// 使用富文本签名
	GetPrivateProfileString(_T("设置"), _T("richtext"), _T("1"), tmpText, _countof(tmpText), PROFILE_PATH);
	m_settingPage.m_richTextSignCheck.SetCheck(tmpText[0] == _T('1'));
	// 保持格式
	GetPrivateProfileString(_T("设置"), _T("KeepFormat"), _T("0"), tmpText, _countof(tmpText), PROFILE_PATH);
	m_settingPage.m_keepFormatCheck.SetCheck(tmpText[0] == _T('1'));
	// 发帖热键
	m_settingPage.m_postHotkeyCombo.SetCurSel(GetPrivateProfileInt(_T("设置"), _T("ft"), 'Q' - 'A', PROFILE_PATH));
	// 发繁体字热键
	m_settingPage.m_unicodePostHotkeyCombo.SetCurSel(GetPrivateProfileInt(_T("设置"), _T("ftz"), 'W' - 'A', PROFILE_PATH));
	// 开机启动
	::CRegKey reg;
	if (reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), KEY_QUERY_VALUE) == ERROR_SUCCESS)
	{
		CString value;
		ULONG length;
		reg.QueryStringValue(_T("PostAssistant"), value.GetBuffer(MAX_PATH), &length);
		value.ReleaseBuffer();
		CString exePath;
		GetModuleFileName(NULL, exePath.GetBuffer(MAX_PATH), MAX_PATH);
		exePath.ReleaseBuffer();
		m_settingPage.m_autoRunCheck.SetCheck(value.Find(exePath) != -1);
	}
	// 文字签名
	CFile file;
	CStringA buffer;
	if (file.Open(TEXT_SIGNS_PATH, CFile::typeText | CFile::modeRead))
	{
		const ULONGLONG len = file.GetLength();
		LPSTR p = buffer.GetBuffer((int)len + 1);
		file.Read(p, (UINT)len);
		file.Close();
		p[len] = '\0';
		buffer.ReleaseBuffer();
		m_textSignPage.m_edit.SetWindowText(CString(buffer));
	}
	m_textSignPage.UpdateTextSigns();
	// 富文本签名
	if (file.Open(RICH_TEXT_SIGNS_PATH, CFile::typeText | CFile::modeRead))
	{
		const ULONGLONG len = file.GetLength();
		LPSTR p = buffer.GetBuffer((int)len + 1);
		file.Read(p, (UINT)len);
		file.Close();
		p[len] = '\0';
		buffer.ReleaseBuffer();
		m_richTextSignPage.m_edit.SetWindowText(CString(buffer));
	}
	m_richTextSignPage.UpdateRichTextSigns();

	// 注册热键
	m_settingPage.OnCbnSelchangeCombo1();
	m_settingPage.OnCbnSelchangeCombo2();
	RegisterHotKey(m_hWnd, HOTKEY_HTML_POST, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_F11);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 释放
void CPostAssistantDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// 托盘图标
	Shell_NotifyIcon(NIM_DELETE, &m_nfData);
}

#pragma region UI
// 窗口 /////////////////////////////////////////////////////////////////////////////////

// 屏蔽Esc关闭窗口
void CPostAssistantDlg::OnCancel()
{
}

// 屏蔽回车关闭窗口
void CPostAssistantDlg::OnOK()
{
}

// 销毁窗口
void CPostAssistantDlg::OnClose()
{
	DestroyWindow();

	CDialog::OnClose();
}

// 切换标签
void CPostAssistantDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int index = m_tab.GetCurSel();
	for (int i = 0; i < 5; i++)
		m_pages[i]->ShowWindow(i == index ? SW_SHOW : SW_HIDE);
}

// 限制最小尺寸
void CPostAssistantDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 384;
	lpMMI->ptMinTrackSize.y = 299;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

// 改变尺寸
void CPostAssistantDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect); // 默认368 * 261
	m_tab.SetWindowPos(NULL, 0, 0, rect.Width() - 21, rect.Height() - 21, SWP_NOMOVE | SWP_NOREDRAW);

	m_tab.GetClientRect(&rect);
	rect.left += 1; rect.right -= 3; rect.top += 22; rect.bottom -= 2;
	m_settingPage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);
	m_textSignPage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);
	m_richTextSignPage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);
	m_converCodePage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);
	m_aboutPage.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOREDRAW);

	Invalidate();
}

// 托盘 /////////////////////////////////////////////////////////////////////////////////

// 最小化
void CPostAssistantDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE) // 最小化时隐藏窗口
	{
		ShowWindow(SW_HIDE);
		Shell_NotifyIcon(NIM_ADD, &m_nfData);
		return;
	}

	CDialog::OnSysCommand(nID, lParam);
}

// 托盘消息
LRESULT CPostAssistantDlg::OnTray(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN) // 单击显示窗口
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nfData);
		ShowWindow(SW_SHOW);
	}

	return 0;
}

// 任务栏重建
LRESULT CPostAssistantDlg::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)
{
	if (!IsWindowVisible())
		Shell_NotifyIcon(NIM_ADD, &m_nfData);
	return 0;
}

// 热键 /////////////////////////////////////////////////////////////////////////////////

// 热键
void CPostAssistantDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	switch (nHotKeyId)
	{
	case HOTKEY_POST:
		Post();
		break;
	case HOTKEY_UNICODE_POST:
		UnicodePost();
		break;
	case HOTKEY_HTML_POST:
		HTMLPost();
		break;
	}

	CDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}
#pragma endregion

// 普通发帖
void CPostAssistantDlg::Post()
{
	UnregisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_POST);

	CComPtr<IHTMLDocument2> document;
	if (GetIEDocument(document) && IsTieba(document))
	{
		CString script = m_settingPage.m_keepFormatCheck.GetCheck() ? KEEP_FORMAT_POST_SCRIPT1 : POST_SCRIPT1;
		AddSign(script);
		script += POST_SCRIPT2;
		EvalJS(document, script);
	}
	
	RegisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_POST, MOD_CONTROL, 'A' + m_settingPage.m_postHotkeyCombo.GetCurSel());
}

// Unicode码
void CPostAssistantDlg::UnicodePost()
{
	// 暂时没办法
}

// 编辑源码
void CPostAssistantDlg::HTMLPost()
{
	UnregisterHotKey(AfxGetMainWnd()->m_hWnd, HOTKEY_HTML_POST);

	CComPtr<IHTMLDocument2> document;
	if (GetIEDocument(document) && IsTieba(document))
	{
		// 取原发帖框内容HTML
		_variant_t vContent;
		EvalJS(document, GET_CONTENT_SCRIPT, &vContent);
		CString content = vContent;
		CInputDlg inputDlg(_T("文本框源码"), &content);
		if (inputDlg.DoModal() == IDOK && content != _T(""))
		{
			// 转义
			content = ReplaceString(content, _T("\r\n"), _T("<br>"));
			content = ReplaceString(content, _T("\\"), _T("\\\\"));
			content = ReplaceString(content, _T("'"), _T("\\'"));

			CString script = NO_OLD_CONTENT_POST_SCRIPT1 + content;
			AddSign(script);
			script += POST_SCRIPT2;
			EvalJS(document, SET_CONTENT_SCRIPT1 + content + SET_CONTENT_SCRIPT2);
			EvalJS(document, script);
		}
	}

	RegisterHotKey(m_hWnd, HOTKEY_HTML_POST, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_F11);
}

// 杂项 /////////////////////////////////////////////////////////////////////////////////

// JS加上签名
void CPostAssistantDlg::AddSign(CString& script)
{
	BOOL hasTextSign = m_settingPage.m_textSignCheck.GetCheck() && m_textSignPage.m_edit.GetWindowTextLengthW() > 0;
	BOOL hasRichTextSign = m_settingPage.m_richTextSignCheck.GetCheck() && m_richTextSignPage.m_edit.GetWindowTextLengthW() > 0;
	if (!hasTextSign && !hasRichTextSign)
		return;

	script += _T("<br><br><br>");

	// 文字签名
	if (hasTextSign)
	{
		CString sign = GetUnrepeatedElement(m_textSignPage.m_textSigns, m_textSignPage.m_restTextSigns);
		EscapeString(sign);
		script += sign;
	}

	// 富文本签名
	if (hasRichTextSign)
	{
		if (hasTextSign)
			script += _T("<br>");
		CString sign = GetUnrepeatedElement(m_richTextSignPage.m_richTextSigns, m_richTextSignPage.m_restRichTextSigns);
		CString ext = sign.Right(4).MakeLower();
		if (!(ext == ".jpg" || ext == ".gif" || ext == ".png")) // 是视频
			script += VIDEO1 + sign + VIDEO2;
		else // 是图片
		{
			// 取图片尺寸
			TCHAR tmp[20];
			GetPrivateProfileString(_T("图片尺寸"), sign, _T(""), tmp, _countof(tmp), PROFILE_PATH);
			int width, height;
			if (_stscanf_s(tmp, _T("%dx%d"), &width, &height) != 2) // 无尺寸记录
			{
				if(!GetInternetImageSize(sign, width, height))
					AfxMessageBox(_T("╮(╯_╰)╭图片尺寸获取失败(网络问题？)，默认500x500"), MB_ICONINFORMATION);
				CString text;
				text.Format(_T("%dx%d"), width, height);
				WritePrivateProfileString(_T("图片尺寸"), sign, text, PROFILE_PATH);
			}
			// 最大尺寸560*600
			if (width > 560 || height > 600)
			{
				float scale1 = 560.0f / (float)width;
				float scale2 = 600.0f / (float)height;
				float scale = scale1 < scale2 ? scale1 : scale2;
				width  = (int)(width  * scale);
				height = (int)(height * scale);
			}

			CString image;
			image.Format(IMAGE_FORMAT, sign, height, width);
			script += image;
		}
	}
}

// 是贴吧网址
BOOL CPostAssistantDlg::IsTieba(CComPtr<IHTMLDocument2>& document)
{
	BSTR url_;
	document->get_URL(&url_);
	CString url = _bstr_t(url_, FALSE);
	return url.Find(_T("http://tieba.baidu.com/f")) != -1 || url.Find(_T("http://tieba.baidu.com/p")) != -1;
}
