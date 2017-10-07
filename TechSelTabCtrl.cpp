// ModeSelTabCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////
// This class is provided as is and Ben Hill takes no
// responsibility for any loss of any kind in connection
// to this code.
/////////////////////////////////////////////////////
// Is is meant purely as a educational tool and may
// contain bugs.
/////////////////////////////////////////////////////
// ben@shido.fsnet.co.uk
// http://www.shido.fsnet.co.uk
/////////////////////////////////////////////////////
// Thanks to a mystery poster in the C++ forum on 
// www.codeguru.com I can't find your name to say thanks
// for your Control drawing code. If you are that person 
// thank you very much. I have been able to use some of 
// you ideas to produce this sample application.
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"

#include "ViewSelTabCtrl.h"
#include "TechSelTabCtrl.h"

#include "PageDown.h"
#include "PageUp.h"
#include "MotorControlBench.h"
#include "ACMotorCtrlWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModeSelTabCtrl

CTechSelTabCtrl::CTechSelTabCtrl()
{
	::ZeroMemory(&m_tabPages, sizeof(m_tabPages));
      m_tabPages[0]=new CPageDown;
	m_tabPages[1]=new CPageUp(this);    
	m_nNumberOfPages=2;
}

CTechSelTabCtrl::~CTechSelTabCtrl()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
}

void CTechSelTabCtrl::InitTabPage()
{
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	
	m_tabCurrent=0;

	m_tabPages[0]->Create(IDD_PAGE_DOWN, this);
	m_tabPages[1]->Create(IDD_PAGE_UP, this);
    
	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);

	
//	SetRectangle();
}

void CTechSelTabCtrl::SetRectangle()
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
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++){
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
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

}

BEGIN_MESSAGE_MAP(CTechSelTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CModeSelTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSelTabCtrl message handlers

void CTechSelTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus()){

             #if 0
        if(GetCurFocus() == LABAL_TAB)
        {        	
             // 插入列表控件纵列
             ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.InsertColumn(0, "No", LVCFMT_LEFT, 110);        	
             ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.InsertColumn(1, "X um", LVCFMT_LEFT, 230);
        	((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.InsertColumn(2, "Y um", LVCFMT_LEFT, 230);
        	// 设置列表控件扩展样式为整行高亮
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

        }
       #endif

		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}


}
























