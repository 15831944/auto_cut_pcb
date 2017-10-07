// PageSMode.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageUp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageExec dialog


CPageUp::CPageUp(CWnd* pParent /*=NULL*/)
	: CDialog(CPageUp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageExec)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageExec)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageUp, CDialog)
	//{{AFX_MSG_MAP(CPageExec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageExec message handlers

BOOL CPageUp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	#if 0
	GetDlgItem(IDC_SMODE_JERK)->SetWindowText("0.0001");
	GetDlgItem(IDC_SMODE_ACC)->SetWindowText("0.2");
	GetDlgItem(IDC_SMODE_VEL)->SetWindowText("10");
	GetDlgItem(IDC_SMODE_POS)->SetWindowText("30000");
       #endif
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
