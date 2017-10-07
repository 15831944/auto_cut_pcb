#if !defined(AFX_INTERPEXPFRAME_H__0BC1F711_A2D0_486E_8FAC_A5B873E4F955__INCLUDED_)
#define AFX_INTERPEXPFRAME_H__0BC1F711_A2D0_486E_8FAC_A5B873E4F955__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterpExpFrame.h : header file
//
#include "FixedSplitterWnd.h"
#include "InterpExpDrawView.h"
#include "InterpExpCtrlWnd.h"
#include "POSITION.h"

/////////////////////////////////////////////////////////////////////////////
// CInterpExpFrame frame

class CInterpExpFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CInterpExpFrame)
public:
	CInterpExpFrame();       
	virtual ~CInterpExpFrame();
protected: 
	
	// Attributes
public:
  
// Operations
public:
	void GetWndPointer();
	CInterpExpCtrlWnd * m_pInterpCtrlWnd;
	CInterpExpDrawView * m_pInterpDrawView;
	CFixedSplitterWnd m_InterpExpSplitterWnd;
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterpExpFrame)
	public:
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CInterpExpFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERPEXPFRAME_H__0BC1F711_A2D0_486E_8FAC_A5B873E4F955__INCLUDED_)
