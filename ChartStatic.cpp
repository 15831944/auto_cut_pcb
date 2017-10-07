// ChartStatic.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "ChartStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartStatic

CChartStatic::CChartStatic()
{
}

CChartStatic::~CChartStatic()
{
}


BEGIN_MESSAGE_MAP(CChartStatic, CStatic)
	//{{AFX_MSG_MAP(CChartStatic)
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartStatic message handlers

int CChartStatic::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	return CStatic::Create(lpszText,dwStyle,rect,pParentWnd,nID);
}

void CChartStatic::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
//	CDC * pDC=CStatic::FromHandle(lpDrawItemStruct->hDC);
	
	CStatic::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
