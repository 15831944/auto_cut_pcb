#if !defined(AFX_GCODECTRLWND_H__639335E7_154F_4AFC_9331_3E44657F7C72__INCLUDED_)
#define AFX_GCODECTRLWND_H__639335E7_154F_4AFC_9331_3E44657F7C72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GCodeCtrlWnd.h : header file
//
#include "ViewSelTabCtrl.h"
#include "CNCData.h"
#include "BufPtr.h"
#include "Position.h"

/****************************************************************************/
/*  类  名: CGCodeCtrlWnd													*/
/*  描  述: G代码控制操作视图类								                */
/*  说	明:														     		*/
/*            1.实现G代码编辑、编译、运行控制等						    	*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CGCodeCtrlWnd form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define LINE_DATA 1000
#define NOVALIDDATA 1000000

class CGCodeCtrlWnd : public CFormView
{
public:
	CGCodeCtrlWnd();  
	virtual ~CGCodeCtrlWnd();
protected:
	         // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGCodeCtrlWnd)

// Form Data
public:
	//{{AFX_DATA(CGCodeCtrlWnd)
	enum { IDD = IDD_CTRLWND_GCODE };
	CViewSelTabCtrl	m_tabViewSel;
	CListBox	m_ctrlNCList;
	CEdit	m_GCodeEdit;
	CComboBox	m_cmbAxisZ;
	CComboBox	m_cmbAxisY;
	CComboBox	m_cmbAxisX;
	//}}AFX_DATA

// Attributes
public:
	double oldzz;
	double oldxx;
	double oldyy;
	bool bIsSaved,bShouldSave;
	bool bIsModified;

	BOOL	m_bFIsValid;

	bool bIsAxisMapped;
	bool m_bIsAxisOn;
	bool m_bIsCardInit;
	int StartPosition;
	bool SingleRun;
	long SendSeg;
	ULONG SendLine;
	CPOSITION * pos;
	double override;
	bool MotionStarted;
	ULONG m_StartLine;
	double m_dOptiAngle[2];
	unsigned short line;
	int EndPosition;
	double CurrentOverride;
	bool bPause;
	CBufPtr BakGy;
	double MaxCoord[6];
	bool bFileLoaded;
	bool StartUserSet;
	BOOL NeedRestoreMtn;
	double dPausePos[4];
	long CurrentCutNo;
	int CrdPlan;
	bool bStop;
	bool bAutoStarted;
	bool bAutoCuting;
	bool Paused;//标记系统是否已经暂停	
	ULONG CncLength;
	int xMap,yMap,zMap;

protected:
	bool bSending;
	bool bSendEnd;

// Operations
public:
	bool CheckIsZero();
	bool m_bYHomed;
	bool m_bXHomed;
	bool CheckG92();
	bool ModifySynVel(double vel);
	int m_nAxisCount;
	void MotionPause();
	void UpdateLed(UINT nID,double dispValue);
	void GetMotionNum();
	void EndSent();
	void ErrorMessage(CString str);
	void FreeMem();
    bool IsLengZero(double xs, double xe, double ys, double ye, double zs, double ze);
	bool AxisGo();
	void LoopSend();
	void SentCnc();
	void GeneralRun();
	void PauseRun();
	bool WaitBufferEmpty();
	bool HandleMCode(int code);
	void InitCard();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCodeCtrlWnd)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCNCData senddata;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGCodeCtrlWnd)
	afx_msg void OnBtnNewNccoed();
	afx_msg void OnBtnOpenNccode();
	afx_msg void OnBtnLoadNccode();
	afx_msg void OnBtnControlRun();
	afx_msg void OnBtnControlPause();
	afx_msg void OnBtnControlStop();
	afx_msg void OnBtnCloseaxis();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnSaveNccode();
	afx_msg void OnBtnCoordEffect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditGcode();
	afx_msg void OnBtnControlXhome();
	afx_msg void OnBtnControlYhome();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCODECTRLWND_H__639335E7_154F_4AFC_9331_3E44657F7C72__INCLUDED_)





















