#if !defined(AFX_ModeSelTabCtrl_H__F3E8650F_019C_479F_9E0F_60FE1181F49F__INCLUDED_)
#define AFX_ModeSelTabCtrl_H__F3E8650F_019C_479F_9E0F_60FE1181F49F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeSelTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeSelTabCtrl window
/****************************************************************************/
/*  类  名: CModeSelTabCtrl													*/
/*  描  述: 单轴实验模块中运动模式选择页面类				                */
/*  说	明:														     		*/
/*            1.内含S曲线\T曲线\VEL模式\电子齿轮四个页面对象		    	*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
class CModeSelTabCtrl : public CTabCtrl
{
	// Construction
public:
	CModeSelTabCtrl();
	CDialog *m_tabPages[5];
	int m_tabCurrent;
	int m_nNumberOfPages;
	
	// Attributes
public:
//	CViewSelTabCtrl	m_tabViewSel;
	// Operations
public:
	void InitTabPage();
	void SetRectangle();
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeSelTabCtrl)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CModeSelTabCtrl();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CModeSelTabCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__F3E8650F_019C_479F_9E0F_60FE1181F49F__INCLUDED_)
