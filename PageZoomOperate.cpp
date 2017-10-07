// PageZoomOperate.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageZoomOperate.h"
#include "GTCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageZoomOperate dialog
extern CGTCard gtcard;

CPageZoomOperate::CPageZoomOperate(CWnd* pParent /*=NULL*/)
	: CDialog(CPageZoomOperate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageZoomOperate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ZoomScale=1.0;
}


void CPageZoomOperate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageZoomOperate)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageZoomOperate, CDialog)
	//{{AFX_MSG_MAP(CPageZoomOperate)
//	ON_BN_CLICKED(IDC_BTN_ZOOMIN, OnZoomin)
//	ON_BN_CLICKED(IDC_BTN_ZOOMOUT, OnZoomout)
//	ON_BN_CLICKED(IDC_BTN_STAND, OnZoomstd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageZoomOperate message handlers

void CPageZoomOperate::OnZoomin() 
{
#if 0
	gtcard.pDrawView->bReDraw=true;	
	if(gtcard.pDrawView->m_scale>20)
	{
		gtcard.pDrawView->m_scale=20;
		GetDlgItem(IDC_BTN_ZOOMIN)->EnableWindow(FALSE);
	}
	else
	{
		m_ZoomScale=1.2*m_ZoomScale;
		gtcard.pDrawView->m_scale=m_ZoomScale;
		GetDlgItem(IDC_BTN_ZOOMOUT)->EnableWindow(TRUE);
		double temp=gtcard.pDrawView->m_scale;
	}
	gtcard.pDrawView->Invalidate();
#endif
}

void CPageZoomOperate::OnZoomout() 
{
#if 0
	gtcard.pDrawView->bReDraw=true;	
	if(gtcard.pDrawView->m_scale<0.05)
	{
		gtcard.pDrawView->m_scale=0.05;
		GetDlgItem(IDC_BTN_ZOOMOUT)->EnableWindow(FALSE);
	}
	else
	{
		m_ZoomScale=0.8*m_ZoomScale;
		gtcard.pDrawView->m_scale=m_ZoomScale;
		GetDlgItem(IDC_BTN_ZOOMIN)->EnableWindow(TRUE);
		double temp=gtcard.pDrawView->m_scale;
	}
	gtcard.pDrawView->Invalidate();
#endif
}

void CPageZoomOperate::OnZoomstd() 
{
#if 0
    gtcard.pDrawView->bReDraw=true;	
	m_ZoomScale=1;
	gtcard.pDrawView->m_scale=m_ZoomScale;
	GetDlgItem(IDC_BTN_ZOOMIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ZOOMOUT)->EnableWindow(TRUE);	
	gtcard.pDrawView->Invalidate();
#endif
}








































