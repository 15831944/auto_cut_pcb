#if !defined(AFX_INTERPEXPCTRLWND_H__0F671A18_F2C4_4FA0_8DB2_7021138B866E__INCLUDED_)
#define AFX_INTERPEXPCTRLWND_H__0F671A18_F2C4_4FA0_8DB2_7021138B866E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterpExpCtrlWnd.h : header file

#include "BtnST.h"
//#include "MotorControlBenchDoc.h"
#include "InterpExpDrawView.h"
#include "Interpolation.h"

class CInterpExpFrame;
class CInterpExpDrawView;


/////////////////////////////////////////////////////////////////////////////
// CInterpExpCtrlWnd form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


/****************************************************************************/
/*  类  名: CInterpExpCtrlWnd												*/
/*  描  述: 二维插补模块控制操作视图类						                */
/*  说	明:														     		*/
/*            1.实现二维插补方式设置,参数设置,坐标映射,基本控制操作等	 	*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
class CInterpExpCtrlWnd : public CFormView
{
protected:
	CInterpExpCtrlWnd();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInterpExpCtrlWnd)

// Form Data
public:
	//{{AFX_DATA(CInterpExpCtrlWnd)
	enum { IDD = IDD_CTRLWND_INTERP };
	CButton	m_btnMap;
	CComboBox	m_cmbYMax;
	CComboBox	m_cmbYMap;
	CComboBox	m_cmbXMax;
	CComboBox	m_cmbXMap;
	CComboBox	m_cmbInterpMode;
	double	m_acc;
	double	m_fAngle;
	int		m_nDirect;
	long	m_nEndX;
	long	m_nEndY;
	double	m_fRadius;
	double	m_fVel;
	double	m_fXCenter;
	double	m_fYCenter;
	int		m_nSizeOfRegister;
	//}}AFX_DATA

// Attributes
public:
  
	int m_nCtrlMode[4];
	int m_nDirPulse[4];
	short m_nKp[4],m_nKi[4],m_nKd[4];
	bool m_bAxisMap;
	bool m_bMapError;
	bool m_bServOn;
	int m_nXMap;
	int m_nYMap;
//	CMotorControlBenchDoc * pDoc;
// Operations
public:
	CInterpolation m_Interp;
	bool CheckParaLimit();
	void InitCard();
	int SetScale(int nSel);
	void UpdateLed(UINT nID, double dispValue);
	void ArcXYP();
	void ArcXY();
	void LnXY();
	void GetWndPoint();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterpExpCtrlWnd)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInterpExpCtrlWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CInterpExpCtrlWnd)
	afx_msg void OnMapButton();
	afx_msg void OnSelchangeSelCombo();
	afx_msg void OnServoOn();
	afx_msg void OnRunButton();
	afx_msg void OnServoOff();
	afx_msg void OnStopButton();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeXmapCombo();
	afx_msg void OnSelchangeXmaxCombo();
	afx_msg void OnSelchangeYmaxCombo();
	afx_msg void OnUpdateEndxEdit();
	afx_msg void OnChangeEndyEdit();
	afx_msg void OnBtnPenOn();
	afx_msg void OnBtnPenOff();
	afx_msg void OnBtnAxisxHome();
	afx_msg void OnBtnAxisyHome();
	afx_msg void OnKillfocusAccEdit();
	afx_msg void OnKillfocusVelEdit();
	afx_msg void OnSelchangeYmapCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CInterpExpDrawView * m_pInterpDrawView;
	CInterpExpFrame * m_pInterpFrm;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERPEXPCTRLWND_H__0F671A18_F2C4_4FA0_8DB2_7021138B866E__INCLUDED_)





















