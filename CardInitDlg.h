#if !defined(AFX_CARDINITDLG_H__FC3A32BD_7509_4829_B1B6_95510EAFD0A5__INCLUDED_)
#define AFX_CARDINITDLG_H__FC3A32BD_7509_4829_B1B6_95510EAFD0A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CardInitDlg.h : header file
//

#include "BtnST.h"

/****************************************************************************/
/*  �ļ���: CardInitDlg.h /CardInitDlg.cpp									*/
/*  ��  ��: �忨���öԻ�����								                */
/*  ��  ��:														     		*/
/*            1. ���������ļ����ð忨�ͺ�							    	*/	 
/*            2. �����õİ忨�ͺű����������ļ�								*/	
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
/****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CCardInitDlg dialog

class CCardInitDlg : public CDialog
{
// Construction
public:
	int m_nCardType;
	CCardInitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCardInitDlg)
	enum { IDD = IDD_DLG_CARDTYPE };
	CButtonST	m_btnOK;
	CButtonST	m_btnCancel;
	CComboBox	m_cmbCardType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCardInitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCardInitDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARDINITDLG_H__FC3A32BD_7509_4829_B1B6_95510EAFD0A5__INCLUDED_)
