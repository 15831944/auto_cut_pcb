// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C15293C1_00C3_40CA_8135_31E77BAAF299__INCLUDED_)
#define AFX_MAINFRM_H__C15293C1_00C3_40CA_8135_31E77BAAF299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FunctionBar.h"
#include "DCMotrCtrlWnd.h"
#include "ACMotorCtrlWnd.h"
#include "GCodeFrame.h"
#include "InterpExpFrame.h"
#include "DebugDlg.h"
#include "ParaSetDlg.h"
#include "PasWordDlg.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bCardInit;
	bool m_bIsInterpViewInit;
	bool m_bIsGCodeViewInit;
//	int m_nAxisCount;
//	CParaSetDlg m_ParaSetDlg;
	CInterpExpFrame * m_pInterpFrame;
	CDCMotrCtrlWnd * m_pDCMotorWnd;
	CGCodeFrame * m_pGCodeFrame;
	CSplitterWnd m_spWndDCMotor;
	bool SwitchToView(int nView);
	CFunctionBar * m_pWndFunctionBar;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
//	CGCodeFrame m_pGCodeFrame;
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!	
	//}}AFX_MSG
	afx_msg void OnDCMotorExperiment();
	afx_msg void OnACMotorExperiment();
	afx_msg void OnInterpolationExperiment();
	afx_msg void OnGCodeExperiment();
	afx_msg void OnDebug();
	afx_msg void OnParaSet();
	afx_msg void OnExit();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C15293C1_00C3_40CA_8135_31E77BAAF299__INCLUDED_)





















