// GCodeFrame.cpp : implementation file
//

#include "stdafx.h"
#include "MotorControlBench.h"
#include "GCodeFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGCodeFrame

IMPLEMENT_DYNCREATE(CGCodeFrame, CFrameWnd)

CGCodeFrame::CGCodeFrame()
{
}

CGCodeFrame::~CGCodeFrame()
{
}


BEGIN_MESSAGE_MAP(CGCodeFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CGCodeFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCodeFrame message handlers

BOOL CGCodeFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	//	return CFrameWnd::OnCreateClient(lpcs, pContext);
	int cx=GetSystemMetrics(SM_CXSCREEN);
	int cy=GetSystemMetrics(SM_CYSCREEN);
	m_GCodeSplitterWnd.CreateStatic(this,1,2);
	m_GCodeSplitterWnd.CreateView(0,0,RUNTIME_CLASS(CGCodeDrawView),CSize(cx*69.5/100,0),pContext);
	m_GCodeSplitterWnd.CreateView(0,1,RUNTIME_CLASS(CGCodeCtrlWnd),CSize(0,0),pContext);
	return true;
	
}

BOOL CGCodeFrame::PreCreateWindow(CREATESTRUCT& cs) 
{	
	return CFrameWnd::PreCreateWindow(cs);
}

int CGCodeFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	
	GetWndPointer();
	return 0;
}

/****************************************************************************/
/*  函数名: GetWndPointer      												*/
/*  功  能：获取G代码显示窗口和控制窗口的指针								*/
/*  参  数：CDC *pDC  绘图窗口DC											*/
/*  出  口：																*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeFrame::GetWndPointer()
{
	CWnd * pWnd;
	pWnd=this->m_GCodeSplitterWnd.GetPane(0,0);
	this->m_pGCodeDrawView=DYNAMIC_DOWNCAST(CGCodeDrawView,pWnd);
    
	pWnd=this->m_GCodeSplitterWnd.GetPane(0,1);
	this->m_pGCodeCtrlWnd=DYNAMIC_DOWNCAST(CGCodeCtrlWnd,pWnd);
}
