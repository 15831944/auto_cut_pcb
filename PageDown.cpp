// PageSMode.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "TechSelTabCtrl.h"
#include "PageTech.h"
#include "PageDown.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDown dialog


CPageDown::CPageDown(CWnd* pParent /*=NULL*/)
	: CDialog(CPageDown::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageDown)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_CutSpeed = 30;

}


void CPageDown::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDown)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_CUT_SPEED, m_CutSpeed);
}


BEGIN_MESSAGE_MAP(CPageDown, CDialog)
	//{{AFX_MSG_MAP(CPageDown)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_CUT_SPEED, &CPageDown::OnEnChangeEdit2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDown message handlers

BOOL CPageDown::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	#if 0
	GetDlgItem(IDC_SMODE_JERK)->SetWindowText("0.0001");
	GetDlgItem(IDC_SMODE_ACC)->SetWindowText("0.2");
	GetDlgItem(IDC_SMODE_VEL)->SetWindowText("10");
	GetDlgItem(IDC_SMODE_POS)->SetWindowText("30000");
      #endif

     // 	CGCodeCtrlWnd * pGCodeCtrlWnd;
//	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));	

      	CPageTech * pPageTechWnd;
	pPageTechWnd=((CPageTech *)((CTechSelTabCtrl*)GetParent()->GetParent()));	

      pPageTechWnd->m_CutSpeedEdit = m_CutSpeed;
        
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageDown::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

      CPageTech * pPageTechWnd;
	pPageTechWnd=((CPageTech *)((CTechSelTabCtrl*)GetParent()->GetParent()));	

      pPageTechWnd->m_CutSpeedEdit = m_CutSpeed;

	
//	((CPageTech *)GetParent())->m_CutSpeedEdit = m_CutSpeed;
      
}

