#if !defined(AFX_PAGEEXEC_H__B766C16B_1F58_4533_80E9_7219D0523FFA__INCLUDED_)
#define AFX_PAGEEXEC_H__B766C16B_1F58_4533_80E9_7219D0523FFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageExec.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageExec dialog


/****************************************************************************/
/*  类  名: CPageExec						 								*/
/*  描  述:  该类对应于电机运动模式设置中的S曲线运动模式页面	            */
/*  功  能:														     		*/
/*            1.S曲线运动模式参数设置										*/
/*  版  本:  1.0											                */
/*  修改日期:  2006-11-20                                                   */
/*  修订人: Long TY                                                         */
/****************************************************************************/
class CPageExec : public CDialog
{
// Construction
public:
	CPageExec(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageSMode)
	enum { IDD = IDD_PAGE_EXEC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	double LB0_ActuralValueX;
	double LB0_ActuralValueY;
	double LB1_ActuralValueX;
	double LB1_ActuralValueY;
	double LB0_OriginalValueX;
	double LB0_OriginalValueY;
	double LB1_OriginalValueX;
	double LB1_OriginalValueY;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageExec)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageExec)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedButtonAutoRun();
	afx_msg void OnBnClickedViewTest();
	afx_msg int GetActualMark(int MarkNum, double *MarkBX, double*MarkBY);
	afx_msg void GetActualPoint(PCB_CUT_POINT_S *pstCutPoint, PCB_CUT_POINT_S *pstActualPoint);
	afx_msg short  GetCenter(CIRCUL_POINT_S &p1,CIRCUL_POINT_S &p2, CIRCUL_POINT_S &p3, CIRCUL_POINT_S &center);
	afx_msg int GetDir(CIRCUL_POINT_S &center, CIRCUL_POINT_S &p1, CIRCUL_POINT_S &p2);

	afx_msg void OnBnClickedButtonExec();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESMODE_H__B766C16B_1F58_4533_80E9_7219D0523FFA__INCLUDED_)
