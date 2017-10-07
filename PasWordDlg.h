#if !defined(AFX_PASWORDDLG_H__449DA636_A961_46D3_A724_6438DB176258__INCLUDED_)
#define AFX_PASWORDDLG_H__449DA636_A961_46D3_A724_6438DB176258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasWordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPasWordDlg dialog

/****************************************************************************/
/*  类  名: CPasWordDlg					 									*/
/*  描  述: 该类对应参数设置的密码输入对话框					            */
/*  功  能:														     		*/
/*            1.验证密码											    	*/
/*  版  本:  1.0											                */
/*  修改日期:  2006-11-20                                                   */
/*  修订人: Long TY                                                         */
/****************************************************************************/
class CPasWordDlg : public CDialog
{
// Construction
public:
	CPasWordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPasWordDlg)
	enum { IDD = IDD_DIALOG_PASSWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasWordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasWordDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASWORDDLG_H__449DA636_A961_46D3_A724_6438DB176258__INCLUDED_)
