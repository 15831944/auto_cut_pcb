// MotorControlBench.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MotorControlBench.h"

#include "MainFrm.h"
#include "MotorControlBenchDoc.h"
#include "MotorControlBenchView.h"
#include "GCodeCtrlWnd.h"
#include "CardInitDlg.h"
//#include "global.h"
#include "GTCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CGTCard gtcard;
/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchApp

BEGIN_MESSAGE_MAP(CMotorControlBenchApp, CWinApp)
	//{{AFX_MSG_MAP(CMotorControlBenchApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchApp construction

CMotorControlBenchApp::CMotorControlBenchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMotorControlBenchApp object

CMotorControlBenchApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchApp initialization

BOOL CMotorControlBenchApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	if (!RegisterReg()) exit(1);//注册DigitalDisp.ocx控件

	m_nAxis=1;
	m_nCardType=0;
	m_bSetupUpdated=false;
	
	if (gtcard.InitCard()==2) exit(1);
	
	LoadIniFile();

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMotorControlBenchDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMotorControlBenchView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMotorControlBenchApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchApp message handlers
bool ProcessMessage()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL,0,0,PM_NOREMOVE))
	{
		if (!AfxGetApp()->PumpMessage()){
			::PostQuitMessage(0);
			AfxMessageBox("PeekMessage error,end application");
			return TRUE;
		}  
	}
	return FALSE;
}

bool CMotorControlBenchApp::LoadIniFile()
{
   	CString axis[4] = {"Axis1","Axis2","Axis3","Axis4"};

	char exeFullPath[MAX_PATH];
    CString strExeFileName="MotorControlBench.exe";
	CString strExePath;
 
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH); //获取应用程序路径	
	strExePath.Format("%s",exeFullPath); //将其格式化为字符串 	 
	exeFullPath[strExePath.GetLength()-strExeFileName.GetLength()]='\0'; //去掉应用程序的全名（15为应用程序文件全名的长度）
	strExePath.Format("%s",exeFullPath); //得到应用程序所在路径 	 
	strExePath+="default.ini";//得到配置文件的全路径

	m_inifile.Create(strExePath);

//  m_inifile.Create(DEFAULT_INI_FILE);

	m_inifile.GetVarInt("CARD","CardType",m_nCardType);
	m_inifile.GetVarInt("SYSTEM","AxisCount",m_nAxisCount);
    m_inifile.GetVarFloat("SYSTEM","G00Speed",m_fG00Spd); 
	m_inifile.GetVarFloat("SYSTEM","G01Speed",m_fG01Spd);
	m_inifile.GetVarFloat("SYSTEM","G00ACC",m_fG00Acc);
    m_inifile.GetVarFloat("SYSTEM","G01ACC",m_fG01Acc);


	for(int i = 0 ; i < 4 ; i ++)
	{
		m_inifile.GetVarInt(axis[i],"ControlMode",m_nCtrlMode[i]);
		m_inifile.GetVarInt(axis[i],"PPR",m_nAxisPPR[i]);
		m_inifile.GetVarFloat(axis[i],"HomeVel",m_fHomeVel[i]);
		m_inifile.GetVarInt(axis[i],"HomeStyle",m_nHomeStyle[i]);
		m_inifile.GetVarFloat(axis[i],"HomeAcc",m_fHomeAcc[i]);
		
		if(m_nCtrlMode[i] == STEP_MOTOR)
		{
			m_inifile.GetVarInt(axis[i],"Dir/Pulse",m_nDirPls[i]);
		}
		else if(m_nCtrlMode[i] == SERVO_MOTOR)
		{
			m_inifile.GetVarShort(axis[i],"Kp",m_nKp[i]);
			m_inifile.GetVarShort(axis[i],"Ki",m_nKi[i]);
			m_inifile.GetVarShort(axis[i],"Kd",m_nKd[i]);
		}
	}
	
	CCardInitDlg *dlg = new CCardInitDlg();
	int nRET = dlg->DoModal();
	
	switch(nRET)
	{
	case IDOK:
		m_inifile.SetVarInt("CARD","CardType",m_nCardType);
		break;
	case IDCANCEL:		
		PostQuitMessage(0);
		break;	
	}
     delete dlg;

    return TRUE;
}



bool CMotorControlBenchApp::RegisterReg()
{
	//ActiveX控件的路径及文件名
	LPCTSTR pszDllName="DigitalDisp.ocx";
	//装载ActiveX控件
	HINSTANCE hLib = LoadLibrary(pszDllName);
	if (hLib < (HINSTANCE)HINSTANCE_ERROR)
	{
		AfxMessageBox("不能载入DigitalDisp.ocx文件!",MB_OK|MB_ICONWARNING);
		return false;
	}
	//获取注册函数DllRegisterServer地址
	FARPROC lpDllEntryPoint; 
	lpDllEntryPoint = GetProcAddress
		(hLib,_T("DllRegisterServer"));
	//调用注册函数DllRegisterServer
	if(lpDllEntryPoint!=NULL)
	{
		if(FAILED((*lpDllEntryPoint)()))
		{
			AfxMessageBox("调用DllRegisterServer失败！",MB_OK);
				FreeLibrary(hLib);
			return false;
		};
		AfxMessageBox("DigitalDisp.ocx注册成功",MB_OK|MB_ICONINFORMATION);
		return true;
	}
	else
	{
		AfxMessageBox("调用DllRegisterServer失败!",MB_OK);	
		return false;
	}

}





















