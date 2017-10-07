#if !defined(AFX_PAGEGEARMODE_H__742AD97A_CAB7_4000_B20A_99F474CCB7BB__INCLUDED_)
#define AFX_PAGEGEARMODE_H__742AD97A_CAB7_4000_B20A_99F474CCB7BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageGearMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageGearMode dialog
/****************************************************************************/
/*  类  名: CPageGearMode					 								*/
/*  描  述: 该类对应于电机运动模式设置中的电子齿轮模式页面                  */
/*  功  能:														     		*/
/*            1. 设定电子齿轮参数									    	*/	 
/*            2. 取消先前设定的电子齿轮										*/	
/*  版  本:  1.0											                */
/*  修改日期:  2006-11-20                                                   */
/*  修订人: Long TY                                                         */
/****************************************************************************/
class CPageGearMode : public CDialog
{
// Construction
public:
	CPageGearMode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageGearMode)
	enum { IDD = IDD_PAGE_GMODE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageGearMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageGearMode)
	afx_msg void OnBtnGearOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGearCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEGEARMODE_H__742AD97A_CAB7_4000_B20A_99F474CCB7BB__INCLUDED_)
