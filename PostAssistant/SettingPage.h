#pragma once
#include "afxwin.h"


// CSettingPage 对话框

class CSettingPage : public CDialog
{
	DECLARE_DYNAMIC(CSettingPage)

public:
	CSettingPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingPage();

// 对话框数据
	enum { IDD = IDD_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedCheck3();

	CButton m_textSignCheck;
	CButton m_richTextSignCheck;
	CButton m_keepFormatCheck;
	CComboBox m_postHotkeyCombo;
	CComboBox m_unicodePostHotkeyCombo;
	CButton m_autoRunCheck;
	afx_msg void OnBnClickedCheck4();
};
