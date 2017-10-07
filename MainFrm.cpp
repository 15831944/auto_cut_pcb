// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MotorControlBench.h"

#include "MainFrm.h"

#include "FunctionBar.h"
#include "GCodeFrame.h"
#include "InterpExpFrame.h"
#include "MotorControlBenchDoc.h"
#include "GTCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CGTCard gtcard;

//#define AFX_IDW_PANE_FIRST IDD_GCODEFRAME
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code !
ON_WM_CREATE()
//}}AFX_MSG_MAP
ON_BN_CLICKED(IDC_BTN_DCMOTR,OnDCMotorExperiment)
ON_BN_CLICKED(IDC_BTN_ACMOTR,OnACMotorExperiment)
ON_BN_CLICKED(IDC_BTN_INTERPOLATION, OnInterpolationExperiment)
ON_BN_CLICKED(IDC_BTN_GCODE,OnGCodeExperiment)
ON_BN_CLICKED(IDC_BTN_DEBUG,OnDebug)
ON_BN_CLICKED(IDC_BTN_PARASET,OnParaSet)
ON_BN_CLICKED(IDC_BTN_EXIT,OnExit)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pWndFunctionBar=new CFunctionBar;
//	CMotorControlBenchApp * pApp=(CMotorControlBenchApp * )(AfxGetApp());
//	m_nAxisCount=pApp->m_nAxisCount;
	m_bIsGCodeViewInit=false;
	m_bIsInterpViewInit=false;
	m_bCardInit=false;
}

CMainFrame::~CMainFrame()
{
	delete m_pWndFunctionBar;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

//CBRS_SIZE_DYNAMIC
	if (!m_pWndFunctionBar->Create(this,IDD_FUNCTIONBAR,WS_VISIBLE|WS_CHILD|CBRS_BOTTOM,IDD_FUNCTIONBAR)||!m_pWndFunctionBar->InitDialog()) {
		TRACE0("Failed to create Function dialog bar\n");
		return -1;
	}
    
	m_pWndFunctionBar->EnableDocking(CBRS_ORIENT_HORZ);

	SetMenu(NULL);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame:: OnDCMotorExperiment()
{
	if (gtcard.IsCardMove()) {
		MessageBox("轴在运动中！请先停止轴运动！","XX科技",MB_ICONWARNING|MB_OK);
		return;
	}
	
	m_pWndFunctionBar->SetBtnState(m_pWndFunctionBar->m_btnDCMotor);
	Invalidate();

	SwitchToView(IDD_CTRLWND_DCMOTOR);
//	m_pDCMotorWnd->InitialChart();
}

void CMainFrame:: OnACMotorExperiment()
{
	if (gtcard.IsCardMove()) {
		MessageBox("轴在运动中！请先停止轴运动！","XX科技",MB_ICONWARNING|MB_OK);
		return;
	}

    #if 0
	m_pWndFunctionBar->SetBtnState(m_pWndFunctionBar->m_btnACMotor);
	Invalidate();	
	SwitchToView(IDD_CTRLWND_ACMOTOR);
	m_bCardInit=true;
    #endif
      CACMotorCtrlWnd debugDlg;
	debugDlg.DoModal();	
}

void CMainFrame:: OnInterpolationExperiment()
{
	if (gtcard.IsCardMove()) {
		MessageBox("轴在运动中！请先停止轴运动！","XX科技",MB_ICONWARNING|MB_OK);
		return;
	}
	
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp * )(AfxGetApp());
	int nAxisCount=pApp->m_nAxisCount;
	if(nAxisCount!=1)
	{
		m_pWndFunctionBar->SetBtnState(m_pWndFunctionBar->m_btnInterpolation);
		Invalidate();
		SwitchToView(IDD_INTERPEXP_FRAME);	
	}
	else
	{
		AfxMessageBox("该功能在单轴模式下无法使用！",MB_ICONWARNING|MB_OK);
	}

	m_bCardInit=true;

}

void CMainFrame:: OnGCodeExperiment()
{
	if (gtcard.IsCardMove()) {
		MessageBox("轴在运动中！请先停止轴运动！","XX科技",MB_ICONWARNING|MB_OK);
		return;
	}
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp * )(AfxGetApp());
	int nAxisCount=pApp->m_nAxisCount;
	if(nAxisCount!=1)
	{
		m_pWndFunctionBar->SetBtnState(m_pWndFunctionBar->m_btnGCode);
		Invalidate();
		SwitchToView(IDD_GCODE_FRAME);
	}
	else
	{
		AfxMessageBox("该功能在单轴模式下无法使用！",MB_ICONWARNING|MB_OK);
	}
	m_bCardInit=true;
}

void CMainFrame:: OnDebug()
{
	if (gtcard.IsCardMove()) {
		MessageBox("轴在运动中！请先停止轴运动！","XX科技",MB_ICONWARNING|MB_OK);
       return;
	}
	CDebugDlg debugDlg;
	debugDlg.DoModal();	
}

void CMainFrame:: OnParaSet()
{
	if(gtcard.IsCrdMove())
	{
		MessageBox("有轴在运动！请停止轴运动再退出程序！"); 
		return;
	}
	
	CPasWordDlg pswDlg;
	int nRet=pswDlg.DoModal();
	if(IDOK==nRet)
	{
		CParaSetDlg paraSetDlg;
		paraSetDlg.DoModal();
	}

}

void CMainFrame:: OnExit()
{
	if(gtcard.IsCrdMove())
	{
		MessageBox("有轴在运动！请停止轴运动再退出程序！"); 
		return;
	}
	int rtn=MessageBox("您确定要退出程序吗？","XX科技",MB_ICONQUESTION|MB_YESNO);
	if(rtn==IDYES)
	{
		gtcard.CloseCard();
		delete GetActiveView();
		if(m_bIsGCodeViewInit) delete m_pGCodeFrame;		 
		if(m_bIsInterpViewInit) delete m_pInterpFrame;
		PostQuitMessage(0);
	}
	else
	{
		return;
	}
}

bool CMainFrame::SwitchToView(int nView)
{
//	static bool m_bIsGCodeViewInit(false);
	static bool m_bIsInterpViewInit(false);
//	CDocument *pDoc=GetActiveDocument();
	CMotorControlBenchDoc *pDoc=(CMotorControlBenchDoc *) GetActiveDocument();
	CView * pOldActiveView=GetActiveView();		
    
	if(nView==IDD_INTERPEXP_FRAME){
		//选择了插补实验运行界面
		m_pInterpFrame=new CInterpExpFrame;
		m_bIsInterpViewInit=true;
		CCreateContext context;
		context.m_pNewViewClass=(CRuntimeClass *)m_pInterpFrame;	
		context.m_pCurrentDoc=pOldActiveView->GetDocument();
		
		m_pInterpFrame->Create(NULL,NULL,AFX_WS_DEFAULT_VIEW&~WS_BORDER,
			CFrameWnd::rectDefault,this,NULL,0,&context);
 
		if(m_bIsGCodeViewInit){
			//如果G代码运行界面存在，则隐藏其窗口，并销毁内存
			m_pGCodeFrame->ShowWindow(SW_HIDE);
			m_pGCodeFrame->SetDlgCtrlID(IDD_GCODE_FRAME);
			delete m_pGCodeFrame;
			m_bIsGCodeViewInit=false;
		}
		
		m_pInterpFrame->ShowWindow(SW_SHOW);
		pOldActiveView->ShowWindow(SW_HIDE);
		m_pInterpFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		
		m_pInterpFrame->m_pInterpCtrlWnd->OnInitialUpdate();
		m_pInterpFrame->m_pInterpDrawView->OnInitialUpdate();
		RecalcLayout();
		return true;
	}
	
	if(nView==IDD_GCODE_FRAME){	
		//选择了G代码运行界面
		m_pGCodeFrame=new CGCodeFrame;
		m_bIsGCodeViewInit=true;
		CCreateContext context;
		context.m_pNewViewClass=(CRuntimeClass *)m_pGCodeFrame;	
		context.m_pCurrentDoc=pOldActiveView->GetDocument();
	
		m_pGCodeFrame->Create(NULL,NULL,AFX_WS_DEFAULT_VIEW&~WS_BORDER,
			CFrameWnd::rectDefault,this,NULL,0,&context);

		if(m_bIsInterpViewInit){
			//如果插补实验运行界面存在，则隐藏其窗口，并销毁内存
			m_pInterpFrame->ShowWindow(SW_HIDE);
			m_pInterpFrame->SetDlgCtrlID(IDD_INTERPEXP_FRAME);
			delete m_pInterpFrame;
			m_bIsInterpViewInit=false;
		}
		
		m_pGCodeFrame->ShowWindow(SW_SHOW);
		pOldActiveView->ShowWindow(SW_HIDE);
		m_pGCodeFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		m_pGCodeFrame->m_pGCodeCtrlWnd->OnInitialUpdate();		
		
		RecalcLayout();
		return true;
	}   
	else{
	//选择了其他运行界面	
		CView *pNewActiveView=(CView *)GetDlgItem(nView);			
		if(pNewActiveView==NULL)
		{
			switch (nView)
			{
			case IDD_CTRLWND_DCMOTOR:                                                                                  
				pNewActiveView=(CView *) new CDCMotrCtrlWnd;
				break;
			case IDD_CTRLWND_ACMOTOR:
				pNewActiveView=(CView *) new  CACMotorCtrlWnd;
				break;                     
		
			}
			CCreateContext context;
			context.m_pCurrentDoc=pOldActiveView->GetDocument();		
			pNewActiveView->Create(NULL,NULL,AFX_WS_DEFAULT_VIEW&~WS_BORDER,rectDefault,this,nView,&context);
			pNewActiveView->OnInitialUpdate();
		}
		
		
		pNewActiveView->ShowWindow(SW_SHOW);
		pOldActiveView->ShowWindow(SW_HIDE);

//		pDoc->AddView(pNewActiveView);
//		pDoc->RemoveView(pOldActiveView);
		SetActiveView(pNewActiveView);
		
		if(m_bIsGCodeViewInit){
			//如果G代码运行界面存在，则隐藏其窗口，并销毁内存
			m_pGCodeFrame->ShowWindow(SW_HIDE);
			m_pGCodeFrame->SetDlgCtrlID(IDD_GCODE_FRAME);
			delete m_pGCodeFrame;
			m_bIsGCodeViewInit=false;
		}

		if(m_bIsInterpViewInit){
			//如果插补实验运行界面存在，则隐藏其窗口，并销毁内存
			m_pInterpFrame->ShowWindow(SW_HIDE);
			m_pInterpFrame->SetDlgCtrlID(IDD_INTERPEXP_FRAME);
			delete m_pInterpFrame;
			m_bIsInterpViewInit=false;
		}

            /* delete by hwz*/
            #if 0
		int ID;
		if(pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CDCMotrCtrlWnd))
			ID=IDD_CTRLWND_DCMOTOR;
		if(pOldActiveView->GetRuntimeClass() == RUNTIME_CLASS(CACMotorCtrlWnd))
			ID=IDD_CTRLWND_ACMOTOR;
	      #endif
          
    	      //pOldActiveView->SetDlgCtrlID(ID);	
		
		pNewActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);		
		delete pOldActiveView;		

		RecalcLayout();
		return true;
		}
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
			
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//	delete m_pGCodeFrame;
	return CFrameWnd::DestroyWindow();
}































































