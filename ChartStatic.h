#if !defined(AFX_CHARTSTATIC_H__9370B014_279E_401A_9C44_5BC85EA4DFC5__INCLUDED_)
#define AFX_CHARTSTATIC_H__9370B014_279E_401A_9C44_5BC85EA4DFC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChartStatic window

class CChartStatic : public CStatic
{
// Construction
public:
	CChartStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	int Create(LPCTSTR lpszText,DWORD dwStyle,const RECT &rect,CWnd * pParentWnd,UINT nID=0xffff);
	virtual ~CChartStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartStatic)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTSTATIC_H__9370B014_279E_401A_9C44_5BC85EA4DFC5__INCLUDED_)
