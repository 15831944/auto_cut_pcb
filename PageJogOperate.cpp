// PageJogOperate.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageJogOperate.h"
#include "GTCard.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageJogOperate dialog
extern CGTCard gtcard;

CPageJogOperate::CPageJogOperate(CWnd* pParent /*=NULL*/)
	: CDialog(CPageJogOperate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageJogOperate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageJogOperate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageJogOperate)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageJogOperate, CDialog)
	//{{AFX_MSG_MAP(CPageJogOperate)
	ON_BN_CLICKED(IDC_BTN_JOG_XPLS, OnBtnJogXpls)
	ON_BN_CLICKED(IDC_BTN_JOG_XMIS, OnBtnJogXmis)
	ON_BN_CLICKED(IDC_BTN_JOG_YPLS, OnBtnJogYpls)
	ON_BN_CLICKED(IDC_BTN_JOG_YMIS, OnBtnJogYmis)
	ON_BN_CLICKED(IDC_BTN_JOG_ZPLS, OnBtnJogZpls)
	ON_BN_CLICKED(IDC_BTN_JOG_ZMIS, OnBtnJogZmis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageJogOperate message handlers

void CPageJogOperate::OnBtnJogXpls() 
{
	int AxisNO;	
	double dist;
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));	

	if (!pGCodeCtrlWnd->bIsAxisMapped) {
		MessageBox("坐标映射尚未生效，请先点击“坐标映射生效”按钮！","",MB_OK|MB_ICONINFORMATION);
		return;
	}
	GetJogIncValue(&dist);
	AxisNO=pGCodeCtrlWnd->xMap-1;
	gtcard.Jog(AxisNO,dist,1,POSITIVE_DIRCT);
}

void CPageJogOperate::OnBtnJogXmis() 
{
	int AxisNO;	
	double dist;
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));

	if (!pGCodeCtrlWnd->bIsAxisMapped) {
		MessageBox("坐标映射尚未生效，请先点击“坐标映射生效”按钮！","",MB_OK|MB_ICONINFORMATION);
		return;
	}
	GetJogIncValue(&dist);
	AxisNO=pGCodeCtrlWnd->xMap-1;
	gtcard.Jog(AxisNO,dist,1,NEGTIVE_DIRCT);
	
}

void CPageJogOperate::OnBtnJogYpls() 
{
	int AxisNO;	
	double  dist;
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));	
  
	if (!pGCodeCtrlWnd->bIsAxisMapped) {
		MessageBox("坐标映射尚未生效，请先点击“坐标映射生效”按钮！","",MB_OK|MB_ICONINFORMATION);
		return;
   }
	AxisNO=pGCodeCtrlWnd->yMap-1;
	GetJogIncValue(&dist);
	gtcard.Jog(AxisNO,dist,1,POSITIVE_DIRCT);	
	
}

void CPageJogOperate::OnBtnJogYmis() 
{
	int AxisNO;	
	double dist;
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));	
	
	if (!pGCodeCtrlWnd->bIsAxisMapped) {
		MessageBox("坐标映射尚未生效，请先点击“坐标映射生效”按钮！","",MB_OK|MB_ICONINFORMATION);
		return;
	}
	AxisNO=pGCodeCtrlWnd->yMap-1;
	GetJogIncValue(&dist);
	gtcard.Jog(AxisNO,dist,1,NEGTIVE_DIRCT);
	
}

BOOL CPageJogOperate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	((CComboBox*) GetDlgItem(IDC_CMB_JOG))->SetCurSel(4);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageJogOperate::OnBtnJogZpls() 
{
	int AxisNO;	
	double dist;
	GetJogIncValue(&dist);
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));	
	
	if (!pGCodeCtrlWnd->bIsAxisMapped) {
		MessageBox("坐标映射尚未生效，请先点击“坐标映射生效”按钮！","",MB_OK|MB_ICONINFORMATION);
		return;
	}
	AxisNO=pGCodeCtrlWnd->zMap-1;
	GetJogIncValue(&dist);
	gtcard.Jog(AxisNO,dist,1,POSITIVE_DIRCT);
	
}

void CPageJogOperate::OnBtnJogZmis() 
{
	int AxisNO;	
	double dist;
	CGCodeCtrlWnd * pGCodeCtrlWnd;
	pGCodeCtrlWnd=((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()));	
	if (!pGCodeCtrlWnd->bIsAxisMapped) {
		MessageBox("坐标映射尚未生效，请先点击“坐标映射生效”按钮！","",MB_OK|MB_ICONINFORMATION);
		return;
	}
	AxisNO=pGCodeCtrlWnd->zMap-1;
	GetJogIncValue(&dist);
	gtcard.Jog(AxisNO,dist,1,NEGTIVE_DIRCT);	
}

void CPageJogOperate::GetJogIncValue(double * fDist)
{
	CString str;
	GetDlgItem(IDC_CMB_JOG)->GetWindowText(str);
	str.Remove('m');
	*fDist=atof(str);
}























