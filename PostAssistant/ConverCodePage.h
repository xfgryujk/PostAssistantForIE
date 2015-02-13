#pragma once
#include "afxwin.h"


// CConverCodePage 对话框

class CConverCodePage : public CDialog
{
	DECLARE_DYNAMIC(CConverCodePage)

public:
	CConverCodePage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConverCodePage();

// 对话框数据
	enum { IDD = IDD_CONVERTCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();


	CButton m_hexUnicodeButton;
	CButton m_decimalUnicodeButton;
	CButton m_URL_GBKButton;
	CEdit m_srcEdit;
	CEdit m_resultEdit;
	afx_msg void OnEnSetfocusEdit2();
};
