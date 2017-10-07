// PasWordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PasWordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasWordDlg dialog


CPasWordDlg::CPasWordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasWordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasWordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPasWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasWordDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasWordDlg, CDialog)
	//{{AFX_MSG_MAP(CPasWordDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasWordDlg message handlers

void CPasWordDlg::OnOK() 
{
	static int i=3;
	CString str;	

	((CEdit *) GetDlgItem(IDC_EDIT_PASSWORD))->GetWindowText(str);
	if(i==0)
	{
		MessageBox("您已连续三次输入错误密码，无法再进行参数设置\n 请关闭程序","XX科技",MB_ICONWARNING|MB_OK);
		return;
	}
	if(str=="123456")
	{
		i=3;
		CDialog::OnOK();
	}
    else
	{
		str.Format("密码输入错误！\n你还可以输入%d次",i-1);
		MessageBox(str,"XX科技",MB_OK|MB_ICONWARNING);
		i--;
		((CEdit *) GetDlgItem(IDC_EDIT_PASSWORD))->SetFocus();
		return;
	}
	
}
