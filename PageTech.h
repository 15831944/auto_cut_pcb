#if !defined(AFX_PAGETECH_H__22D7AEE4_C598_4213_B178_790FF3B00E44__INCLUDED_)
#define AFX_PAGETECH_H__22D7AEE4_C598_4213_B178_790FF3B00E44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageVelMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageTech dialog


/****************************************************************************/
/*  ��  ��: CPageTech					 								*/
/*  ��  ��:  �����Ӧ�ڵ���˶�ģʽ�����е��ٶ��˶�ģʽҳ��				    */
/*  ��  ��:														     		*/
/*            1.�ٶ��˶�ģʽ��������										*/
/*  ��  ��:  1.0											                */
/*  �޸�����:  2006-11-20                                                   */
/*  �޶���: Long TY                                                         */
/****************************************************************************/
class CPageTech : public CDialog
{
// Construction
public:
	CPageTech(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageVelMode)
	enum { IDD = IDD_PAGE_TECH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CTechSelTabCtrl	m_tabTechSel;
       long m_CutSpeedEdit;
	   
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageVelMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageVelMode)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonNewpoint();
	afx_msg void OnBnClickedTechInsert();
	afx_msg void OnBnClickedTechModify();
	afx_msg void OnBnClickedTechDelete();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEVELMODE_H__22D7AEE4_C598_4213_B178_790FF3B00E44__INCLUDED_)
