#if !defined(AFX_PAGEBASEPARA_H__82E2D4B0_DC36_4D33_8BC3_93C127F2214A__INCLUDED_)
#define AFX_PAGEBASEPARA_H__82E2D4B0_DC36_4D33_8BC3_93C127F2214A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageBasePara.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageBasePara dialog
/****************************************************************************/
/*  类  名: CPageBasePara													*/
/*  描  述: 参数设置模块中轴参数设置页面类					                */
/*  说	明:														     		*/
/*            1.设置轴基本参数										    	*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
class CPageBasePara : public CDialog
{
// Construction
public:
	
	void SaveCurAxisPara(int nAxisNo);
	void GetPID();
	int m_nAxisCount;
	long m_nPPR[4];
	bool m_bIsFirstRun;
	bool SaveIniFile();
	void LoadAxisPara(short nAxisNo);
	virtual	~CPageBasePara();
	UINT m_nCardType;
	int  m_nCtrlMode[4];
	int  m_nDirPls[4];
	int m_nAxisNo;

	short  *m_nKP, *m_nKI,* m_nKD;
    double m_fG00Spd,m_fG01Spd;
    double * m_fG00Acc, * m_fG01Acc;
//	int * m_nHomeStyle;
    double  * m_fHomeVel,*m_fHomeAcc;

	CPageBasePara(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageBasePara)
	enum { IDD = IDD_PAGE_BASEPARA };
	int		m_nMotorType;
	int		m_nStepMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageBasePara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageBasePara)
	afx_msg void OnSelchangeCmbAxiscount();
	afx_msg void OnSelchangeCmbAxisnoParsetpage();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioStep();
	afx_msg void OnRadioServo();
	afx_msg void OnRadioDir();
	afx_msg void OnRadioPulse();
	afx_msg void OnUpdateEditKpParaset();
	afx_msg void OnUpdateEditKiParaset();
	afx_msg void OnUpdateEditKdParaset();
	virtual void OnOK();
	afx_msg void OnUpdateEditPpr();
	virtual void OnCancel();
	afx_msg void OnUpdateEditG00Spd();
	afx_msg void OnUpdateEditG01Spd();
	afx_msg void OnDropdownCmbAxisnoParsetpage();
	afx_msg void OnUpdateEditG01Acc();
	afx_msg void OnClose();
	afx_msg void OnUpdateEditG00Acc();
	afx_msg void OnUpdateHomeVel();
	afx_msg void OnUpdateHomeAcc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEBASEPARA_H__82E2D4B0_DC36_4D33_8BC3_93C127F2214A__INCLUDED_)





















