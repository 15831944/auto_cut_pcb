#if !defined(AFX_PAGEDOWN_H__B766C16B_1F58_4533_80E9_7219D0523FFA__INCLUDED_)
#define AFX_PAGEDOWN_H__B766C16B_1F58_4533_80E9_7219D0523FFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDown.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDown dialog


/****************************************************************************/
/*  ��  ��: CPageDown						 								*/
/*  ��  ��:  �����Ӧ�ڵ���˶�ģʽ�����е�S�����˶�ģʽҳ��	            */
/*  ��  ��:														     		*/
/*            1.S�����˶�ģʽ��������										*/
/*  ��  ��:  1.0											                */
/*  �޸�����:  2006-11-20                                                   */
/*  �޶���: Long TY                                                         */
/****************************************************************************/
class CPageDown : public CDialog
{
// Construction
public:
	CPageDown(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageDown)
	enum { IDD = IDD_PAGE_DOWN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

      int m_CutSpeed;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageDown)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageDown)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESMODE_H__B766C16B_1F58_4533_80E9_7219D0523FFA__INCLUDED_)
