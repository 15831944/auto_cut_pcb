// PageSMode.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "ACMotorCtrlWnd.h"
#include "CcdSelTabCtrl.h"
#include "PageSystem.h"
#include <math.h>
#include "GTCard.h"
#include "Global.h"
#include "GT400.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageExec dialog


CPageSystem::CPageSystem(CWnd* pParent /*=NULL*/)
	: CDialog(CPageSystem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageExec)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageExec)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSystem, CDialog)
	//{{AFX_MSG_MAP(CPageExec)
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CPageSystem::OnBnClickedButtonOpenfile)
	//ON_BN_CLICKED(IDC_BUTTON_AUTO_RUN, &CPageSystem::OnBnClickedButtonAutoRun)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageExec message handlers

BOOL CPageSystem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageSystem::OnBnClickedButtonOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
}


#if 0
void CPageExec::LnXY(long x, long y, double acc, double fVel)
{
	short rtn;
       rtn = GT_SetSynAcc(acc);    //Error(rtn,"GT_SetSynAcc");
	rtn = GT_SetSynVel(fVel);    //Error(rtn,"GT_SetSynVel");
	rtn=GT_LnXY(x, y);  // Error(rtn,"GT_LnXY");
	unsigned short mode;
	rtn = GT_GetMode(&mode);
}


void CPageExec::GetActMark(&MarkAX, &MarkAY, &MarkBX, &MarkBY);
{
    /* 获取像素坐标*/
    /* 转换成运动坐标*/
}
#endif


