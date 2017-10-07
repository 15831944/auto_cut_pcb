// PageFeedSetting.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageFeedSetting.h"
#include "GCodeCtrlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageFeedSetting dialog


CPageFeedSetting::CPageFeedSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CPageFeedSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageFeedSetting)
	m_bFIsValid = FALSE;
	//}}AFX_DATA_INIT
}


void CPageFeedSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageFeedSetting)
	DDX_Control(pDX, IDC_FEEDRATIO, m_sldFeedRatio);
	DDX_Check(pDX, IDC_CHK_FEED, m_bFIsValid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageFeedSetting, CDialog)
	//{{AFX_MSG_MAP(CPageFeedSetting)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHK_FEED, OnChkFeed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageFeedSetting message handlers

void CPageFeedSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	//速度更新//
	CString str;
	double fCurFeed;
	
	str.Format("进给倍率:%d%%",10*m_sldFeedRatio.GetPos());
	GetDlgItem(IDC_FEED_TEXT)->SetWindowText(str);
	fCurFeed=double(m_sldFeedRatio.GetPos()/10.0);
	((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()))->override=fCurFeed;
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CPageFeedSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bFIsValid=true;
	m_sldFeedRatio.SetRange(1,12);
	m_sldFeedRatio.SetPos(10);	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageFeedSetting::OnChkFeed() 
{
	UpdateData(TRUE);
	((CGCodeCtrlWnd *)((CViewSelTabCtrl*)GetParent()->GetParent()))->m_bFIsValid=m_bFIsValid;
		
}
