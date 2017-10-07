#if !defined(AFX_PAGECCDRANGE_H__532C4ADA_DD0A_4009_9C4A_43AA77CD63BF__INCLUDED_)
#define AFX_PAGECCDRANGE_H__532C4ADA_DD0A_4009_9C4A_43AA77CD63BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PagePlaneSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPagePlaneSel dialog

/****************************************************************************/
/*  ��  ��: CPagePlaneSel					 								*/
/*  ��  ��: �����ӦG����ʵ��ģʽ�е���ʾƽ��ѡ��ҳ��		                */
/*  ��  ��:														     		*/
/*            1.ѡ��G������ʾ������ʾƽ��							    	*/
/*  ��  ��:  1.0											                */
/*  �޸�����:  2006-11-20                                                   */
/*  �޶���: Long TY                                                         */
/****************************************************************************/
class CPageCcdRange : public CDialog
{
// Construction
public:
	CPageCcdRange(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPagePlaneSel)
	enum { IDD = IDD_CCD_RANGE };
	int		m_nWidth;
	int         m_nHigh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPagePlaneSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPagePlaneSel)
	afx_msg void OnXyplane();
	afx_msg void OnXzplane();
	afx_msg void OnYzplane();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
       afx_msg void OnDeltaposSpinCcdWeidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCcdHeight(NMHDR *pNMHDR, LRESULT *pResult);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEPLANESEL_H__532C4ADA_DD0A_4009_9C4A_43AA77CD63BF__INCLUDED_)
