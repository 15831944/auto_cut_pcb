// InterpExpFrame.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "InterpExpFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterpExpFrame

IMPLEMENT_DYNCREATE(CInterpExpFrame, CFrameWnd)

CInterpExpFrame::CInterpExpFrame()
{
}

CInterpExpFrame::~CInterpExpFrame()
{
}


BEGIN_MESSAGE_MAP(CInterpExpFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CInterpExpFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterpExpFrame message handlers

BOOL CInterpExpFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	int cx=GetSystemMetrics(SM_CXSCREEN);
	int cy=GetSystemMetrics(SM_CYSCREEN);
	m_InterpExpSplitterWnd.CreateStatic(this,1,2);
	m_InterpExpSplitterWnd.CreateView(0,0,RUNTIME_CLASS(CInterpExpDrawView),CSize(cx*2/3,0),pContext);
	m_InterpExpSplitterWnd.CreateView(0,1,RUNTIME_CLASS(CInterpExpCtrlWnd),CSize(0,0),pContext);
	return true;
	
//	return CFrameWnd::OnCreateClient(lpcs, pContext);
}



int CInterpExpFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	GetWndPointer();
	return 0;
}

void CInterpExpFrame::GetWndPointer()
{
	CWnd * pWnd;
	pWnd=this->m_InterpExpSplitterWnd.GetPane(0,0);
	this->m_pInterpDrawView=DYNAMIC_DOWNCAST(CInterpExpDrawView,pWnd);
    
	pWnd=this->m_InterpExpSplitterWnd.GetPane(0,1);
	this->m_pInterpCtrlWnd=DYNAMIC_DOWNCAST(CInterpExpCtrlWnd,pWnd);
}
