
// PostAssistant.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "PostAssistant.h"
#include "PostAssistantDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPostAssistantApp

BEGIN_MESSAGE_MAP(CPostAssistantApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPostAssistantApp 构造

CPostAssistantApp::CPostAssistantApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPostAssistantApp 对象

CPostAssistantApp theApp;


// CPostAssistantApp 初始化
#include "Global.h"
BOOL CPostAssistantApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();


	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	// 防多开
	if (OpenMutex(MUTEX_ALL_ACCESS, TRUE, _T("PostAssistant")) == NULL)
		CreateMutex(NULL, TRUE, _T("PostAssistant"));
	else
	{
		AfxMessageBox(_T("多开什么的最讨厌了！"), MB_ICONWARNING);
		return FALSE;
	}

	// 初始化随机数种子
	srand(GetTickCount());

	// 防止IE11 KB3025390补丁禁止执行JS
#pragma region
	static const LPCTSTR regPath[2] = { _T("SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_DISABLE_INTERNAL_SECURITY_MANAGER"),
		_T("SOFTWARE\\Wow6432Node\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_DISABLE_INTERNAL_SECURITY_MANAGER") };
	::CRegKey reg;
	for (int i = 0; i < 2; i++)
		if (reg.Open(HKEY_LOCAL_MACHINE, regPath[i], KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			DWORD value;
			LSTATUS status = reg.QueryDWORDValue(_T("iexplore.exe"), value);
			if ((status == ERROR_SUCCESS && value == 0) || status == ERROR_FILE_NOT_FOUND)
			{
				reg.SetDWORDValue(_T("iexplore.exe"), 1);
				AfxMessageBox(_T("请重启IE使发帖助手生效"), MB_ICONWARNING);
			}
			reg.Close();
		}
#pragma endregion

	// 初始化配置文件路径
	TCHAR cd[MAX_PATH];
	GetCurrentDirectory(_countof(cd), cd);
	PROFILE_PATH = cd;
	PROFILE_PATH += _T("\\myini.ini");

	// 初始化COM库
	CoInitialize(NULL);

	// 载入主窗口
	CPostAssistantDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// 卸载COM库
	CoUninitialize();


	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

