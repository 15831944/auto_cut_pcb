#if !defined(AFX_FIXEDSPLITTERWND_H__861AF38A_23A5_4A04_B9E4_9AFB39E0FF1B__INCLUDED_)
#define AFX_FIXEDSPLITTERWND_H__861AF38A_23A5_4A04_B9E4_9AFB39E0FF1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FixedSplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFixedSplitterWnd window

class CFixedSplitterWnd : public CSplitterWnd
{
// Construction
public:
	CFixedSplitterWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFixedSplitterWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFixedSplitterWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFixedSplitterWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIXEDSPLITTERWND_H__861AF38A_23A5_4A04_B9E4_9AFB39E0FF1B__INCLUDED_)
