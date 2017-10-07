#if !defined(AFX_PAGESYSPARA_H__097C76D8_94AA_45E7_A87A_8F43513D30BA__INCLUDED_)
#define AFX_PAGESYSPARA_H__097C76D8_94AA_45E7_A87A_8F43513D30BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSysPara.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSysPara dialog

class CPageSysPara : public CDialog
{
// Construction
public:
	CPageSysPara(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageSysPara)
	enum { IDD = IDD_PAGE_SYSTEM_PARA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSysPara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageSysPara)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESYSPARA_H__097C76D8_94AA_45E7_A87A_8F43513D30BA__INCLUDED_)
