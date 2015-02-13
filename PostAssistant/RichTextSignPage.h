#pragma once
#include "afxwin.h"


// CRichTextSignPage 对话框

class CRichTextSignPage : public CDialog
{
	DECLARE_DYNAMIC(CRichTextSignPage)

public:
	CRichTextSignPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRichTextSignPage();

// 对话框数据
	enum { IDD = IDD_RICHTEXTSIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void UpdateRichTextSigns();

	CEdit m_edit;
	CStatic m_static;
	
	CStringArray m_richTextSigns;
	int m_restRichTextSigns;
};
