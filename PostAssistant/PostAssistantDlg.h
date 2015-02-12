
// PostAssistantDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "SettingPage.h"
#include "TextSignPage.h"
#include "RichTextSignPage.h"
#include "ConverCodePage.h"
#include "AboutPage.h"
#include "Global.h"


// CPostAssistantDlg 对话框
class CPostAssistantDlg : public CDialog
{
// 构造
public:
	CPostAssistantDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_POSTASSISTANT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnTray(WPARAM wParam, LPARAM lParam);
	LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);


protected:
	void Post();
	void UnicodePost();
	void HTMLPost();
	
	void AddSign(CString& script);
	static inline BOOL IsTieba(CComPtr<IHTMLDocument2>& document);


public:
	CTabCtrl m_tab;
	CSettingPage m_settingPage;
	CTextSignPage m_textSignPage;
	CRichTextSignPage m_richTextSignPage;
	CConverCodePage m_converCodePage;
	CAboutPage m_aboutPage;
protected:
	CWnd* m_pages[5];

	NOTIFYICONDATA m_nfData;
};
