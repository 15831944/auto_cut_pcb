#if !defined(AFX_PAGEMARK_H__B09B1F2C_9BA5_46EB_B167_47974FAA2543__INCLUDED_)
#define AFX_PAGEMARK_H__B09B1F2C_9BA5_46EB_B167_47974FAA2543__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPageMark.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageMark dialog


/****************************************************************************/
/*  ��  ��: CPageMark						 								*/
/*  ��  ��:  �����Ӧ�ڵ���˶�ģʽ�����е�T�����˶�ģʽҳ��	            */
/*  ��  ��:														     		*/
/*            1.T�����˶�ģʽ��������								    	*/
/*  ��  ��:  1.0											                */
/*  �޸�����:  2006-11-20                                                   */
/*  �޶���: Long TY                                                         */
/****************************************************************************/
class CPageMark : public CDialog
{
// Construction
public:
	CPageMark(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageMark)
	enum { IDD = IDD_PAGE_MARK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CCcdSelTabCtrl	m_tabViewSel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageMark)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageMark)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMarkModify();
	afx_msg void OnDeltaposSpinZCompensate(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMARK_H__B09B1F2C_9BA5_46EB_B167_47974FAA2543__INCLUDED_)
