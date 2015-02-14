#pragma once
#include "afxwin.h"
#include "afxlinkctrl.h"
#include "TextButton.h"


// CAboutPage 对话框

class CAboutPage : public CDialog
{
	DECLARE_DYNAMIC(CAboutPage)

public:
	CAboutPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAboutPage();

// 对话框数据
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStnClickedStatic1();

	CEdit m_edit;
	CTextButton m_link;
};
