#if !defined(AFX_VIEWSELTABCTRL_H__14B34548_4462_40EA_86E6_E423B9F9037B__INCLUDED_)
#define AFX_VIEWSELTABCTRL_H__14B34548_4462_40EA_86E6_E423B9F9037B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSelTabCtrl.h : header file
//
#include"PagePlaneSel.h"
#include"PageZoomOperate.h"
#include "PageJogOperate.h"
#include "PageFeedSetting.h"

/////////////////////////////////////////////////////////////////////////////
// CViewSelTabCtrl window

class CViewSelTabCtrl : public CTabCtrl
{
// Construction
public:
	CViewSelTabCtrl();

// Attributes
public:

	CDialog *m_tabPages[4];
	int m_tabCurrent;
	int m_nNumberOfPages;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSelTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRectangle();
	void InitTabPage();
	virtual ~CViewSelTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewSelTabCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSELTABCTRL_H__14B34548_4462_40EA_86E6_E423B9F9037B__INCLUDED_)





















