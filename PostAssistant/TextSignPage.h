#pragma once
#include "afxwin.h"


// CTextSignPage 对话框

class CTextSignPage : public CDialog
{
	DECLARE_DYNAMIC(CTextSignPage)

public:
	CTextSignPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTextSignPage();

// 对话框数据
	enum { IDD = IDD_TEXTSIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void UpdateTextSigns();

	CEdit m_edit;
	CStatic m_static;

	CStringArray m_textSigns;
	int m_restTextSigns;
};
