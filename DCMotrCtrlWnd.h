#if !defined(AFX_DCMOTRCTRLWND_H__F3013371_0EC6_4954_BEF7_863DC8F0C471__INCLUDED_)
#define AFX_DCMOTRCTRLWND_H__F3013371_0EC6_4954_BEF7_863DC8F0C471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCMotrCtrlWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCMotrCtrlWnd form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "cv.h"
#include "highgui.h"

#include "ChartStatic.h"
class CDCMotrCtrlWnd : public CFormView
{
protected:
     // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDCMotrCtrlWnd)

// Form Data
public:
	CDCMotrCtrlWnd();      
	//{{AFX_DATA(CDCMotrCtrlWnd)
	enum { IDD = IDD_CTRLWND_DCMOTOR };
	double	m_TargetVal;
	UINT	m_nKp;
	UINT	m_nKi;
	UINT	m_nKd;
	//}}AFX_DATA

// Attributes
public:
	HWND m_hPE;
	int m_nRealTimeCounter;
	double StartTime;
	double ZoomStart;
	double ZoomEnd;
	double ZoomInterval;
	short m_nAxisNo;
	
// Operations
public:
	void InitCard();
	void SetPID(UINT nKp,UINT nKi,UINT nKd);
	//void DrawToHDC(const IplImage* arr, CRect lRect, UINT ID);
	//bool GetBitmapData(HDC hDCDst, SIZE* size, int* channels, void** data );
	float m_fLastYValue;
	float m_fTr;
	float m_fMaxY;
	int m_nLineCount;
	void SetChartShowPara(int nShowType);
	int GetChartShowType();
	int m_nShowType;
	double m_fManualMaxX;
	void UpdateLed(UINT nID,double dispValue);
	void InitialChart();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCMotrCtrlWnd)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);

	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDCMotrCtrlWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDCMotrCtrlWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnChartStop();
	afx_msg void OnBtnChartStart();
	afx_msg void OnBtnChartCalculate();
	afx_msg void OnBtnChartRestore();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCMOTRCTRLWND_H__F3013371_0EC6_4954_BEF7_863DC8F0C471__INCLUDED_)
