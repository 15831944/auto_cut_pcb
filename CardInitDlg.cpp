// CardInitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "CardInitDlg.h"
#include "GT400.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardInitDlg dialog


CCardInitDlg::CCardInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardInitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCardInitDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCardInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardInitDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CARDTYPE, m_cmbCardType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardInitDlg, CDialog)
	//{{AFX_MSG_MAP(CCardInitDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardInitDlg message handlers

void CCardInitDlg::OnOK() 
{
	int nCardType;
	
	nCardType=((CComboBox *) GetDlgItem(IDC_CARDTYPE))->GetCurSel();

	if(nCardType==0)
		((CMotorControlBenchApp *)AfxGetApp())->m_nCardType=SV_CARD;
	else
		((CMotorControlBenchApp *)AfxGetApp())->m_nCardType=SG_CARD;

/*	if (GT_Open()!=0) {	
		MessageBox("打开控制卡失败！\n请检查您的机器内是否正确安装了板卡！","系统错误",MB_OK|MB_ICONSTOP);
		exit(1);
	}*/

	CDialog::OnOK();
}

BOOL CCardInitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nCardType;
	nCardType=((CMotorControlBenchApp *)AfxGetApp())->m_nCardType;
	m_cmbCardType.SetCurSel(nCardType);
	m_btnOK.SetFlat(FALSE);
	m_btnOK.SetIcon(IDI_MAP_OK);
	
	m_btnCancel.SetFlat(FALSE);
	m_btnCancel.SetIcon(IDI_NO);
	
	return TRUE;  
}
