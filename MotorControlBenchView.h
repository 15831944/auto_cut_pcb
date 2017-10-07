// MotorControlBenchView.h : interface of the CMotorControlBenchView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTORCONTROLBENCHVIEW_H__20F09D39_32A0_49AB_99E7_E86088B0CE53__INCLUDED_)
#define AFX_MOTORCONTROLBENCHVIEW_H__20F09D39_32A0_49AB_99E7_E86088B0CE53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMotorControlBenchView : public CFormView
{
protected: // create from serialization only
	CMotorControlBenchView();
	DECLARE_DYNCREATE(CMotorControlBenchView)

// Attributes
public:
	CMotorControlBenchDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotorControlBenchView)
	enum{IDD=IDD_MOTORCONTROLBENCH_FORM	};
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMotorControlBenchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMotorControlBenchView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MotorControlBenchView.cpp
inline CMotorControlBenchDoc* CMotorControlBenchView::GetDocument()
   { return (CMotorControlBenchDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTORCONTROLBENCHVIEW_H__20F09D39_32A0_49AB_99E7_E86088B0CE53__INCLUDED_)
