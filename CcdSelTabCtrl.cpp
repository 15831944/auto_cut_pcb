// ViewSelTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MotorControlBench.h"
#include "CcdSelTabCtrl.h"
#include "PageCcdRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSelTabCtrl

CCcdSelTabCtrl::CCcdSelTabCtrl()
{
    ::ZeroMemory(&m_tabPages, sizeof(m_tabPages));
    m_tabPages[0]=new CPageCcdRange;
    m_tabPages[1]=new CPageZoomOperate;
    m_tabPages[2]=new CPageJogOperate;
    m_tabPages[3]=new CPageFeedSetting;

    m_nNumberOfPages=4;
}

CCcdSelTabCtrl::~CCcdSelTabCtrl()
{

	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
	

}


BEGIN_MESSAGE_MAP(CCcdSelTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CViewSelTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSelTabCtrl message handlers

void CCcdSelTabCtrl::InitTabPage()
{
		m_tabCurrent=0;
		
		m_tabPages[0]->Create(IDD_CCD_RANGE, this);
		m_tabPages[1]->Create(IDD_CCD_LIGHT, this);
		m_tabPages[2]->Create(IDD_PAGE_JOG, this);
		m_tabPages[3]->Create(IDD_PAGE_FSETTING,this);

		m_tabPages[0]->ShowWindow(SW_SHOW);
		m_tabPages[1]->ShowWindow(SW_HIDE);
		m_tabPages[2]->ShowWindow(SW_HIDE);
		m_tabPages[3]->ShowWindow(SW_HIDE);
		SetRectangle();

}

void CCcdSelTabCtrl::SetRectangle()
{
#if 0
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
    #endif

    CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	//nY=itemRect.bottom+1;
	nY=1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-30;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_tabPages[1]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	m_tabPages[2]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
    m_tabPages[3]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);

}

void CCcdSelTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
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






































