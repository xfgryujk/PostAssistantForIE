#pragma once
#include "afxwin.h"


// CInputDlg 对话框

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // 标准构造函数
	CInputDlg(const CString& staticCapital, CString* content, CWnd* pParent = NULL);
	virtual ~CInputDlg();

// 对话框数据
	enum { IDD = IDD_INPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();


	CEdit m_edit;
	CStatic m_static;
protected:
	CString m_staticCapital;
	CString* m_content;
};
