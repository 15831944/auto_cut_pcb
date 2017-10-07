#if !defined(AFX_PAGEFEEDSETTING_H__A4EB6831_EE6D_4CF9_B95A_E6FE8C7505D7__INCLUDED_)
#define AFX_PAGEFEEDSETTING_H__A4EB6831_EE6D_4CF9_B95A_E6FE8C7505D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageFeedSetting.h : header file
//

/****************************************************************************/
/*  ��  ��: CPageFeedSetting					 							*/
/*  ��  ��: �����ӦG����ʵ��ģʽ�е��ٶ�����ҳ��		                    */
/*  ��  ��:														     		*/
/*            1.ѡ��Fָ���Ƿ���Ч									    	*/
/*            2.���ý�������										    	*/
/*  ��  ��:  1.0											                */
/*  �޸�����:  2006-12-11                                                   */
/*  �޶���: Long TY                                                         */
/****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CPageFeedSetting dialog


class CPageFeedSetting : public CDialog
{
// Construction
public:
	CPageFeedSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageFeedSetting)
	enum { IDD = IDD_PAGE_FSETTING };
	CSliderCtrl	m_sldFeedRatio;
	BOOL	m_bFIsValid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageFeedSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageFeedSetting)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnChkFeed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEFEEDSETTING_H__A4EB6831_EE6D_4CF9_B95A_E6FE8C7505D7__INCLUDED_)
