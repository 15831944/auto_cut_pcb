#if !defined(AFX_FUNCTIONBAR_H__DFE50302_2CF7_4716_80C8_58EA831D234C__INCLUDED_)
#define AFX_FUNCTIONBAR_H__DFE50302_2CF7_4716_80C8_58EA831D234C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FunctionBar.h : header file
//
#include "resource.h"
#include "BTNST.h"

/****************************************************************************/
/*  类  名: CFunctionBar													*/
/*  描  述: 功能按钮对话条类								                */
/*  说	明:														     		*/
/*            1. 各实验模块按钮集成在此对话条						    	*/            				 
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CFunctionBar dialog

class CFunctionBar : public CDialogBar
{
// Construction
public:
	bool InitDialog();
	CFunctionBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFunctionBar)
	enum { IDD = IDD_FUNCTIONBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFunctionBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFunctionBar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetBtnState(CButtonST & ActivedBtn);
	bool AddBtnToBar(CButtonST & btn,struct BtnPara btnPara);
	CButtonST m_btnDCMotor,m_btnACMotor,m_btnInterpolation,m_btnGCode,m_btnParaSet,m_btnExit;
	CButtonST * m_pBtnDebug;
	//CListCtrl m_LineList;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCTIONBAR_H__DFE50302_2CF7_4716_80C8_58EA831D234C__INCLUDED_)





















