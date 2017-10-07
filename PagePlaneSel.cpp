// PagePlaneSel.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PagePlaneSel.h"
#include"GTCard.h"
#include "ViewSelTabCtrl.h"
#include "GCodeCtrlWnd.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*#define XY_PLANE 0
#define XZ_PLANE 1
#define YZ_PLANE 2*/

/////////////////////////////////////////////////////////////////////////////
// CPagePlaneSel dialog
extern CGTCard gtcard;

CPagePlaneSel::CPagePlaneSel(CWnd* pParent /*=NULL*/)
	: CDialog(CPagePlaneSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPagePlaneSel)
	m_nViewSel = 0;
	//}}AFX_DATA_INIT
}


void CPagePlaneSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPagePlaneSel)
//	DDX_Radio(pDX, IDC_XYPLANE, m_nViewSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPagePlaneSel, CDialog)
	//{{AFX_MSG_MAP(CPagePlaneSel)
//	ON_BN_CLICKED(IDC_XYPLANE, OnXyplane)
//	ON_BN_CLICKED(IDC_XZPLANE, OnXzplane)
//	ON_BN_CLICKED(IDC_YZPLANE, OnYzplane)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPagePlaneSel message handlers

void CPagePlaneSel::OnXyplane() 
{
	// TODO: Add your control notification handler code here
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));
	
	if (pApp->m_nAxisCount==2) {
     pGCodeCtrlWnd->m_cmbAxisX.EnableWindow(TRUE);
	 pGCodeCtrlWnd->m_cmbAxisY.EnableWindow(TRUE);
     pGCodeCtrlWnd->m_cmbAxisZ.EnableWindow(FALSE);
	}
	
	pGCodeCtrlWnd->GetDlgItem(IDC_BTN_CONTROL_XHOME)->SetWindowText("X回零");
	pGCodeCtrlWnd->GetDlgItem(IDC_BTN_CONTROL_YHOME)->SetWindowText("Y回零");
		
	
	((CMotorControlBenchApp *)AfxGetApp())->nViewPlnSel=XY_PLANE;
	gtcard.pDrawView->bReDraw=true;
	gtcard.pDrawView->Invalidate();
}

void CPagePlaneSel::OnXzplane() 
{
	// TODO: Add your control notification handler code here
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));
	
	if (pApp->m_nAxisCount==2) {
		pGCodeCtrlWnd->m_cmbAxisX.EnableWindow(TRUE);
		pGCodeCtrlWnd->m_cmbAxisY.EnableWindow(FALSE);		
		pGCodeCtrlWnd->m_cmbAxisZ.EnableWindow(TRUE);
	}
	pGCodeCtrlWnd->GetDlgItem(IDC_BTN_CONTROL_XHOME)->SetWindowText("X回零");
	pGCodeCtrlWnd->GetDlgItem(IDC_BTN_CONTROL_YHOME)->SetWindowText("Z回零");

	((CMotorControlBenchApp *)AfxGetApp())->nViewPlnSel=XZ_PLANE;
	gtcard.pDrawView->bReDraw=true;
	gtcard.pDrawView->Invalidate();
}

void CPagePlaneSel::OnYzplane() 
{
	// TODO: Add your control notification handler code here
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));
	
	if (pApp->m_nAxisCount==2) {
		pGCodeCtrlWnd->m_cmbAxisX.EnableWindow(FALSE);		
		pGCodeCtrlWnd->m_cmbAxisY.EnableWindow(TRUE);		
		pGCodeCtrlWnd->m_cmbAxisZ.EnableWindow(TRUE);
	}

	pGCodeCtrlWnd->GetDlgItem(IDC_BTN_CONTROL_XHOME)->SetWindowText("Y回零");
	pGCodeCtrlWnd->GetDlgItem(IDC_BTN_CONTROL_YHOME)->SetWindowText("Z回零");
   
	((CMotorControlBenchApp *)AfxGetApp())->nViewPlnSel=YZ_PLANE;
	gtcard.pDrawView->bReDraw=true;
	gtcard.pDrawView->Invalidate();
}


