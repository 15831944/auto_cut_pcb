// ViewSelTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MotorControlBench.h"
#include "ViewSelTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSelTabCtrl

CViewSelTabCtrl::CViewSelTabCtrl()
{
    ::ZeroMemory(&m_tabPages, sizeof(m_tabPages));
    m_tabPages[0]=new CPagePlaneSel;
    m_tabPages[1]=new CPageZoomOperate;
    m_tabPages[2]=new CPageJogOperate;
    m_tabPages[3]=new CPageFeedSetting;

    m_nNumberOfPages=4;
}

CViewSelTabCtrl::~CViewSelTabCtrl()
{

	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
	

}


BEGIN_MESSAGE_MAP(CViewSelTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CViewSelTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSelTabCtrl message handlers

void CViewSelTabCtrl::InitTabPage()
{
		m_tabCurrent=0;
		
		m_tabPages[0]->Create(IDD_PAGE_PLANESEL, this);
		m_tabPages[1]->Create(IDD_PAGE_ZOOMSEL, this);
		m_tabPages[2]->Create(IDD_PAGE_JOG, this);
		m_tabPages[3]->Create(IDD_PAGE_FSETTING,this);

		m_tabPages[0]->ShowWindow(SW_SHOW);
		m_tabPages[1]->ShowWindow(SW_HIDE);
		m_tabPages[2]->ShowWindow(SW_HIDE);
		m_tabPages[3]->ShowWindow(SW_HIDE);
		SetRectangle();

}

void CViewSelTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_tabPages[1]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	m_tabPages[2]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
    m_tabPages[3]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
}

void CViewSelTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus()){
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}
	CTabCtrl::OnLButtonDown(nFlags, point);
}






































