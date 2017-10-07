// ParaSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "ParaSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParaSetDlg dialog


CParaSetDlg::CParaSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParaSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParaSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_PageBasePara=new CPageBasePara;
	m_pageSysPara=new CPageSysPara;
}


void CParaSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParaSetDlg)
	DDX_Control(pDX, IDC_PARASET_SHEET, m_tabParaSet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParaSetDlg, CDialog)
	//{{AFX_MSG_MAP(CParaSetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParaSetDlg message handlers

BOOL CParaSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_tabParaSet.AddPage("基本参数设置",m_PageBasePara,IDD_PAGE_BASEPARA);
//	m_tabParaSet.AddPage("系统参数设置",m_pageSysPara,IDD_PAGE_SYSTEM_PARA);
	m_tabParaSet.Show();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//DEL CParaSetDlg::~CParaSetDlg()
//DEL {
//DEL   
//DEL }

CParaSetDlg::~CParaSetDlg()
{
	delete m_PageBasePara;
	delete m_pageSysPara;
}
