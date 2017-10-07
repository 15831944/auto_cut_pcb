#if !defined(AFX_PARASETDLG_H__DD0511A3_3195_4B23_BA4F_40CA826CD440__INCLUDED_)
#define AFX_PARASETDLG_H__DD0511A3_3195_4B23_BA4F_40CA826CD440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaSetDlg.h : header file
//
#include "TabSheet.h"
#include "PageBasePara.h"
#include"PageSysPara.h"

/////////////////////////////////////////////////////////////////////////////
// CParaSetDlg dialog


/****************************************************************************/
/*  类  名: CParaSetDlg					 									*/
/*  描  述: 该类对应参数设置对话框								            */
/*  功  能:														     		*/
/*            1.对系统参数进行设置									    	*/
/*  版  本:  1.0											                */
/*  修改日期:  2006-11-20                                                   */
/*  修订人: Long TY                                                         */
/****************************************************************************/
class CParaSetDlg : public CDialog
{
// Construction
public:
	virtual	~CParaSetDlg();
	CPageBasePara * m_PageBasePara;
	CPageSysPara * m_pageSysPara;
	CParaSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParaSetDlg)
	enum { IDD = IDD_DLG_PARASET };
	CTabSheet	m_tabParaSet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParaSetDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARASETDLG_H__DD0511A3_3195_4B23_BA4F_40CA826CD440__INCLUDED_)
