#if !defined(AFX_DEBUGDLG_H__D412156B_A934_4339_8B29_FE5B06FC8E82__INCLUDED_)
#define AFX_DEBUGDLG_H__D412156B_A934_4339_8B29_FE5B06FC8E82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugDlg.h : header file
//
#include "BtnST.h"

/****************************************************************************/
/*  类  名: CDebugDlg														*/
/*  描  述: 系统测试对话框类								                */
/*  功  能:														     		*/
/*            1. 系统测试对话框操作									    	*/	 
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
// CDebugDlg dialog

class CDebugDlg : public CDialog
{
// Construction
public:
//	CAnimateCtrl * m_pctrlAVI;
	void ShowAxisSignal();
	bool m_bCardInit;
	void InitCard();
	void UpdateLed(UINT nID,double dispValue);
	void OutPutSignal(int nID,int nBit);
	CDebugDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDebugDlg)
	enum { IDD = IDD_DLG_DEBUG };
	CAnimateCtrl	m_ctrlAVI;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebugDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnManualXplus();
	afx_msg void OnBtnManualXminus();
	afx_msg void OnBtnManualServon();
	afx_msg void OnBtnManualYplus();
	afx_msg void OnBtnManualYminus();
	afx_msg void OnBtnManualZminus();
	afx_msg void OnBtnManualAminus();
	afx_msg void OnBtnManualZplus();
	afx_msg void OnBtnManualAplus();
	afx_msg void OnBtnManualHome();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOutBit0();
	afx_msg void OnOutBit1();
	afx_msg void OnOutBit2();
	afx_msg void OnOutBit3();
	afx_msg void OnOutBit4();
	afx_msg void OnOutBit5();
	afx_msg void OnOutBit6();
	afx_msg void OnOutBit7();
	afx_msg void OnOutBit8();
	afx_msg void OnOutBit9();
	afx_msg void OnOutBit10();
	afx_msg void OnOutBit11();
	afx_msg void OnOutBit12();
	afx_msg void OnOutBit13();
	afx_msg void OnOutBit14();
	afx_msg void OnOutBit15();
	afx_msg void OnBtnManualServoff();
	afx_msg void OnBtnManualReset();
	afx_msg void OnBtnManualCardinit();
	virtual void OnOK();
	afx_msg void OnBtnAxis1Home();
	afx_msg void OnBtnAxisHome();
	afx_msg void OnBtnAxis3Home();
	afx_msg void OnBtnAxis4Home();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_bServOn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGDLG_H__D412156B_A934_4339_8B29_FE5B06FC8E82__INCLUDED_)
