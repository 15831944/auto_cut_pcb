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
/*  ��  ��: CPasWordDlg					 									*/
/*  ��  ��: �����Ӧ�������õ���������Ի���					            */
/*  ��  ��:														     		*/
/*            1.��֤����											    	*/
/*  ��  ��:  1.0											                */
/*  �޸�����:  2006-11-20                                                   */
/*  �޶���: Long TY                                                         */
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
