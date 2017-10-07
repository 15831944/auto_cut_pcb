// FunctionBar.cpp : implementation file
//

#include "stdafx.h"
#include "MotorControlBench.h"
#include "FunctionBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FUNC_BTN_COUNT 7

/////////////////////////////////////////////////////////////////////////////
// CFunctionBar dialog
struct BtnPara  {
    LPCTSTR name;
	RECT btnRect;
	UINT btnID;
	COLORREF crInactiveBack;
	COLORREF crInactiveFront;
	COLORREF crActiveFront;
    COLORREF crActiveBack;
	CWnd * pParentWnd;
	UINT btnIconID;
	
};

CFunctionBar::CFunctionBar(CWnd* pParent /*=NULL*/)
//	: CDialog(CFunctionBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFunctionBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBtnDebug=new CButtonST;		
}

void CFunctionBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFunctionBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFunctionBar, CDialogBar)
	//{{AFX_MSG_MAP(CFunctionBar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFunctionBar message handlers

bool CFunctionBar::InitDialog()
{	
	int nBtnCount=FUNC_BTN_COUNT-1;
   // 	RECT LineListRect;
	int nBtnWidth,nBtnHeight;
	CRect rectFunctionBar;
	AfxGetMainWnd()->GetWindowRect(rectFunctionBar);
	nBtnWidth=870/nBtnCount;
	nBtnHeight=55; 		

    #if 1
	BtnPara btnPara;	
	btnPara.crInactiveBack=RGB(151, 219, 244);
	btnPara.crInactiveFront=RGB(255, 255, 244);
	btnPara.crActiveFront=RGB(1, 147, 247);
	btnPara.pParentWnd=this;
	
    btnPara.name="自动切割";
	btnPara.btnRect=CRect(0,5,nBtnWidth,nBtnHeight);	
	btnPara.btnID=IDC_BTN_DCMOTR;	
	btnPara.btnIconID=IDI_DCMOTOR;
	
	AddBtnToBar(m_btnDCMotor,btnPara);
	
	btnPara.name="程序编辑";
	btnPara.btnRect=CRect(nBtnWidth,5,2*nBtnWidth,nBtnHeight);	
	btnPara.btnID=IDC_BTN_ACMOTR;	
	btnPara.btnIconID=IDI_ICON2;
	AddBtnToBar(m_btnACMotor,btnPara);
	
	btnPara.name="机器参数";
	btnPara.btnRect=CRect(2*nBtnWidth,5,3*nBtnWidth,nBtnHeight);	
	btnPara.btnID=IDC_BTN_INTERPOLATION;	
	btnPara.btnIconID=IDI_INTERP;
	AddBtnToBar(m_btnInterpolation,btnPara);
	
	btnPara.name="代码实验";
	btnPara.btnRect=CRect(3*nBtnWidth,5,4*nBtnWidth,nBtnHeight);	
	btnPara.btnID=IDC_BTN_GCODE;	
	btnPara.btnIconID=IDI_GCODE;
	AddBtnToBar(m_btnGCode,btnPara);
	
	btnPara.name="系统测试";
	btnPara.btnRect=CRect(4*nBtnWidth,5,5*nBtnWidth,nBtnHeight);	
	btnPara.btnID=IDC_BTN_DEBUG;	
	btnPara.btnIconID=IDI_DEBUG;
	AddBtnToBar(*m_pBtnDebug,btnPara);
	
	btnPara.name="参数设置";
	btnPara.btnRect=CRect(5*nBtnWidth,5,6*nBtnWidth,nBtnHeight);	
	btnPara.btnID=IDC_BTN_PARASET;	
	btnPara.btnIconID=IDI_PARASET;
	AddBtnToBar(m_btnParaSet,btnPara);
	
    btnPara.name="退出系统";
	btnPara.btnRect=CRect(6*nBtnWidth,5,7*nBtnWidth,nBtnHeight);	
	btnPara.btnID=IDC_BTN_EXIT;	
	btnPara.btnIconID=IDI_EXIT;
	AddBtnToBar(m_btnExit,btnPara);
#endif

#if 0
    LineListRect=CRect(0,0,800,90);	

    m_LineList.Create(LVS_EX_GRIDLINES | LVS_REPORT | LVS_EDITLABELS | WS_CHILD | WS_VISIBLE, LineListRect, this, IDC_LINE_LIST);
    m_LineList.InsertColumn(0, "Column 1", LVCFMT_LEFT, nBtnWidth, nBtnHeight);
    m_LineList.InsertItem( 0, "aaa");
#endif
    
	return  true;	
}

bool CFunctionBar::AddBtnToBar(CButtonST & btn,BtnPara btnPara)
{
	btn.Create(btnPara.name,WS_CHILD | WS_VISIBLE,btnPara.btnRect,btnPara.pParentWnd,btnPara.btnID);
    btn.SetFlat(FALSE);
	btn.SetInactiveBgColor(btnPara.crInactiveBack,1);
	btn.SetInactiveFgColor(btnPara.crInactiveFront,1);
	btn.SetActiveBgColor(btnPara.crActiveFront,1);
	btn.SetIcon(btnPara.btnIconID);
	return 0;
}

void CFunctionBar::SetBtnState(CButtonST &ActivedBtn)
{
	m_btnACMotor.m_bIsActive=false;
	m_btnDCMotor.m_bIsActive=false;
	m_btnInterpolation.m_bIsActive=false;
	m_btnGCode.m_bIsActive=false;
	m_btnExit.m_bIsActive=false;
	ActivedBtn.m_bIsActive=true;

}
