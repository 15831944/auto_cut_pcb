#if !defined(AFX_GCODEFRAME_H__203210A6_2A32_4185_9444_AF7E369DA39E__INCLUDED_)
#define AFX_GCODEFRAME_H__203210A6_2A32_4185_9444_AF7E369DA39E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GCodeFrame.h : header file
//

#include "GCodeDrawView.h"
#include "GCodeCtrlWnd.h"
#include "FixedSplitterWnd.h"



/****************************************************************************/
/*  类  名: CGCodeFrame														*/
/*  描  述: G代码模块框架类									                */
/*  说	明:														     		*/
/*            1.m_pGCodCtrlWnd 对应G代码控制界面窗口				    	*/
/*            1.m_pGCodDrawView 对应G代码显示界面窗口				    	*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CGCodeFrame frame


class CGCodeFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CGCodeFrame)
public:
	CGCodeFrame();           // protected constructor used by dynamic creation
    virtual ~CGCodeFrame();
// Attributes
public:
	CFixedSplitterWnd m_GCodeSplitterWnd;

// Operations
public:
	CGCodeCtrlWnd * m_pGCodeCtrlWnd;
	CGCodeDrawView * m_pGCodeDrawView;
	void GetWndPointer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCodeFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	

	// Generated message map functions
	//{{AFX_MSG(CGCodeFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCODEFRAME_H__203210A6_2A32_4185_9444_AF7E369DA39E__INCLUDED_)





















