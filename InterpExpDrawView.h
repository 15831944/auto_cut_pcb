#if !defined(AFX_INTERPEXPDRAWVIEW_H__94546D87_F996_4819_85BA_533C7355BB61__INCLUDED_)
#define AFX_INTERPEXPDRAWVIEW_H__94546D87_F996_4819_85BA_533C7355BB61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterpExpDrawView.h : header file
//
#include "BtnST.h"
#include "Position.h"
//#include "MainFrm.h"
//#include "MotorControlBenchDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CInterpExpDrawView view

class CInterpExpFrame;

/****************************************************************************/
/*  类  名: CInterpExpDrawView												*/
/*  描  述: 二维插补轨迹显示视图类							                */
/*  说	明:														     		*/
/*            1.实现二维插补轨迹实时显示,坐标显示					    	*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
class CInterpExpDrawView : public CView
{
protected:
	CInterpExpDrawView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInterpExpDrawView)

public:
// Operations
	void DrawCoord(CDC * pDC);
	void DrawXScale(CDC * pDC,int nScaleData);
	void DrawScale(CDC * pDC);
	void DrawCurve(CDC * pDC);
	void DrawChart();
	void DrawYScale(CDC * pDC,int nScaleData);
	void DrawProfileArc(CDC * pDC, long XSt,long YSt,long XEnd,long YEnd,long R,bool bIsCW);
	void DrawProfileLine(CDC * pDC,long xEnd,long yEnd);

// Attributes
	bool m_bIsCW;
	int m_nCurveType;
	long m_lXSt,m_lYst,m_lXEnd,m_lYEnd; //起点\终点坐标
	long m_lRadius;						//插补圆弧半径
	CInterpExpFrame * m_pInterpFrm;		//二维插补框架对象
	int m_nXMax;						//
	int m_nYMax;
	int m_nZMax;
	int m_nAxisFlag;
	CRect rectAxis;
    bool m_bDrawFlag;					//绘图标志
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterpExpDrawView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInterpExpDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CInterpExpDrawView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERPEXPDRAWVIEW_H__94546D87_F996_4819_85BA_533C7355BB61__INCLUDED_)
