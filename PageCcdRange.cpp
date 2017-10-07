// PagePlaneSel.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageCcdRange.h"
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

CPageCcdRange::CPageCcdRange(CWnd* pParent /*=NULL*/)
	: CDialog(CPageCcdRange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPagePlaneSel)
	m_nWidth = 100;
      m_nHigh = 100;
	//}}AFX_DATA_INIT
}


void CPageCcdRange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPagePlaneSel)
//	DDX_Radio(pDX, IDC_XYPLANE, m_nViewSel);
      	DDX_Text(pDX, IDC_CCD_WIDTH, m_nWidth);
      	DDX_Text(pDX, IDC_CCD_HEIGHT, m_nHigh);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageCcdRange, CDialog)
	//{{AFX_MSG_MAP(CPagePlaneSel)
//	ON_BN_CLICKED(IDC_XYPLANE, OnXyplane)
//	ON_BN_CLICKED(IDC_XZPLANE, OnXzplane)
//	ON_BN_CLICKED(IDC_YZPLANE, OnYzplane)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CCD_WEIDTH, &CPageCcdRange::OnDeltaposSpinCcdWeidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CCD_HEIGHT, &CPageCcdRange::OnDeltaposSpinCcdHeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPagePlaneSel message handlers

void CPageCcdRange::OnXyplane() 
{
	// TODO: Add your control notification handler code here
}

void CPageCcdRange::OnXzplane() 
{
	// TODO: Add your control notification handler code here
}

void CPageCcdRange::OnYzplane() 
{
	// TODO: Add your control notification handler code here
}

void CPageCcdRange::OnDeltaposSpinCcdWeidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
    CString str;	

    ((CEdit *) GetDlgItem(IDC_CCD_WIDTH))->GetWindowText(str);

    long width = atof(str);
    
    if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
    {
        width++;
    }
    else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
    {
        width--;
    }
    
    CString ss;
    ss.Format(_T("%d "), width);
    GetDlgItem(IDC_CCD_WIDTH)->SetWindowText(ss);   //EDIT控件显示数据

    *pResult = 0;
}


void CPageCcdRange::OnDeltaposSpinCcdHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    CString str;	

    ((CEdit *) GetDlgItem(IDC_CCD_HEIGHT))->GetWindowText(str);

    long height = atof(str);

    if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
    {
        height++;
    }
    else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
    {
        height--;
    }

    CString ss;
    ss.Format(_T("%d "), height);
    GetDlgItem(IDC_CCD_HEIGHT)->SetWindowText(ss);   //EDIT控件显示数据

    *pResult = 0;
}




