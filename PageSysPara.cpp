// PageSysPara.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageSysPara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSysPara dialog


CPageSysPara::CPageSysPara(CWnd* pParent /*=NULL*/)
	: CDialog(CPageSysPara::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSysPara)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageSysPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSysPara)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSysPara, CDialog)
	//{{AFX_MSG_MAP(CPageSysPara)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSysPara message handlers
