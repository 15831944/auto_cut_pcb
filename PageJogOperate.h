#if !defined(AFX_PAGEJOGOPERATE_H__0BBBF05A_7D2D_45C3_A5AA_7CE6F0E030AA__INCLUDED_)
#define AFX_PAGEJOGOPERATE_H__0BBBF05A_7D2D_45C3_A5AA_7CE6F0E030AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageJogOperate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageJogOperate dialog

/****************************************************************************/
/*  ��  ��: CPageGearMode					 								*/
/*  ��  ��: �����ӦG����ʵ��ģʽ�еĵ㶯����ҳ��			                */
/*  ��  ��:														     		*/
/*            1.��X\Y\Z���е㶯����									    	*/
/*  ��  ��:  1.0											                */
/*  �޸�����:  2006-11-20                                                   */
/*  �޶���: Long TY                                                         */
/****************************************************************************/
class CPageJogOperate : public CDialog
{
// Construction
public:
	void GetJogIncValue(double *fDist);
	CPageJogOperate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageJogOperate)
	enum { IDD = IDD_PAGE_JOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageJogOperate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageJogOperate)
	afx_msg void OnBtnJogXpls();
	afx_msg void OnBtnJogXmis();
	afx_msg void OnBtnJogYpls();
	afx_msg void OnBtnJogYmis();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnJogZpls();
	afx_msg void OnBtnJogZmis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEJOGOPERATE_H__0BBBF05A_7D2D_45C3_A5AA_7CE6F0E030AA__INCLUDED_)
