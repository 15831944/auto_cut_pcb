#if !defined(AFX_PAGEZOOMOPERATE_H__B295E54B_2C91_422A_B825_1E5785A88677__INCLUDED_)
#define AFX_PAGEZOOMOPERATE_H__B295E54B_2C91_422A_B825_1E5785A88677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageZoomOperate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageZoomOperate dialog


/****************************************************************************/
/*  ��  ��: CPageZoomOperate					 							*/
/*  ��  ��: �����ӦG����ʵ��ģʽ�е�ͼ�����Ų���ҳ��			            */
/*  ��  ��:														     		*/
/*            1.��G������ʾ��ͼ�ν������Ų���						    	*/
/*  ��  ��:  1.0											                */
/*  �޸�����:  2006-11-20                                                   */
/*  �޶���: Long TY                                                         */
/****************************************************************************/
class CPageZoomOperate : public CDialog
{
// Construction
public:
	float m_ZoomScale;
	CPageZoomOperate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageZoomOperate)
	enum { IDD = IDD_CCD_LIGHT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageZoomOperate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageZoomOperate)
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnZoomstd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEZOOMOPERATE_H__B295E54B_2C91_422A_B825_1E5785A88677__INCLUDED_)
