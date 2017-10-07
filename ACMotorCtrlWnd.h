#if !defined(AFX_ACMOTORCTRLWND_H__4B3AC106_1030_427F_93C8_B7105D90F2DF__INCLUDED_)
#define AFX_ACMOTORCTRLWND_H__4B3AC106_1030_427F_93C8_B7105D90F2DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ACMotorCtrlWnd.h : header file
//
#include "global.h"
#include "ViewSelTabCtrl.h"
#include "ModeSelTabCtrl.h"

#include "cv.h"
#include "highgui.h"
// CACMotorCtrlWnd form view
/****************************************************************************/
/*  File Name: ACMotorCtrlWnd.h	\ACMotorCtrlWnd.cpp											*/
/*  Description: this class is associated with the single Axis experimental */
/*               model														*/
/*  Function:																*/
/*            1. Initialize the card and plot area					    	*/	 
/*            2. Set PID parameters for axis								*/	
/*			  3. Do experiment about S-Curve\T-Curve\Vel\E-Gear             */ 
/*			  4. plot the curve of velocity,position,acceleration           */ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define TIME_INTERVAL       100
#define WALK_THREATHROL 150

typedef enum {NO_SIDE=0,LEFT_SIDE=1,RIGHT_SIDE,MAX_SIDE_VALUE} SideValue;
//typedef enum {NO_TYPE=0,DOWN_CUT=1,DRY_RUN,MAX_POINT_TYPE} CutType;


struct CHARTPARA;
struct PCB_CUT_POINT_S 
{
    CString type; 
 //   int   CurrentNum;
    long PosX;
    long PosY;
    long PosZ; 
    long CutSpeed;
};


struct CIRCUL_POINT_S 
{
    double x;
    double y;
};

struct PCB_M_POINT_S 
{
//    CString type; 
 //   int   CurrentNum;
    long PosX;
    long PosY;
    long PosZ; 
    long Para;
};	

class CACMotorCtrlWnd : public CDialog
{
protected:
// protected constructor used by dynamic creation
	//DECLARE_DYNCREATE(CACMotorCtrlWnd)

// Form Data
public:
	CACMotorCtrlWnd(CWnd* pParent = NULL);   // standard constructor
	//{{AFX_DATA(CACMotorCtrlWnd)
	enum { IDD = IDD_CTRLWND_ACMOTOR };
      CModeSelTabCtrl	m_tabModeSel;
	int		m_nAxis;
	long	m_nKp;
	long	m_nKi;
	long	m_nKd;
      long	LB0_ValueX;
	long	LB0_ValueY;
	long	LB1_ValueX;
	long	LB1_ValueY;
      long	RB0_ValueX;
	long	RB0_ValueY;
	long	RB1_ValueX;
	long	RB1_ValueY;
	PCB_CUT_POINT_S L_Points[MAX_POINT];
	PCB_CUT_POINT_S R_Points[MAX_POINT];
	PCB_M_POINT_S L_MPoints[3];
	PCB_M_POINT_S R_MPoints[3];

	CvCapture*  m_Video; 

	CPoint stCenter;

	int PointNum;
	CString m_StepValue;
	long m_Diameter;
	long m_MillingDiameter;
//	int		m_nShowDataTYpe;
	//}}AFX_DATA 
	CListCtrl m_listCtrl;
	int IndexInFieldList;
	char	szString[100];
	long m_CurX;
	long m_CurY;
	double m_ccdDeltaX;
	double m_ccdDeltaY;
	int m_CcdWidth;
	int m_CcdHeight;
		
public:
// Operations
	//Operation about Card
	void InitCard();

	//Operation about parameter checking
	bool CheckPara();
	void Error(short rtn,char * chCommand);

	//Operation about plotting
	void DrawChart_SG();
	void DrawChart_SV();	
	void DrawWhenStop();	
	void InitChart(UINT nWndNo,CHARTPARA para);	
	void SetChartPara();	
	void SetChartScale(UINT nChartNo,UINT nAxisTitle,double fMin=0.0,double fMax=1000.0);
	inline	void SetAuxAxisNo(const char * chAxis)
	{
		((CDialog *) m_tabModeSel.m_tabPages[3])->GetDlgItem(IDC_PASSIVEAXIS)->SetWindowText(chAxis);
	}

      void DrawView(CDC *pDC);
      void OnMouseMove2(UINT nFlags, CPoint point);
      BOOL PreTranslateMessage(MSG* pMsg);

public:
	bool VelMotion(int iAxisNum);
	bool TMotion(int iAxisNum);
	bool SMotion(int iAxisNum);
	void BufferSampleInit(int dataStyle);
	void InterfaceHandleWithAxisNO(const char * AxisName);

// Attributes
	int m_nCardType;     //板卡类型
	bool m_bAxisOn;	     //轴开启标志 开启=T，关闭=F
	bool m_bGearModeSet; //电子齿轮方式设置标志  设置=T 未设置=F
	int m_nAuxAxisNo;    //电子齿轮从动轴号


	bool m_bParaIsChanged; //PID参数更改标志  更改=T  未更改=F
	bool m_bCanDrawWithBuffer;//板卡是否支持缓冲区绘图方式  能绘图=T 不能绘图=F
	bool m_bCanDraw;

	HWND m_hPE[3];      //绘图控件窗口句柄数组	m_hPE[0]：位置曲线窗口句柄，
	                    //m_hPE[1]：速度曲线窗口句柄，m_hPE[1]：加速度曲线窗口句柄	
	double m_fTargetVel; //目标速度
	int m_DrawPageCount; //绘图区显示时分屏总数 
	int m_CurDrawPage;   //绘图区当前显示屏号
	long count;
	
	double m_fIntervalOfPage; //绘图控件中
	double m_fStartTimeOfPage;//绘图控件中每页显示的起始时间值
	double m_fEndTimeOfPage;//绘图控件中每页显示的终止时间值
	int m_nRealTimeCounter;
      CString strFilePathName;
      SideValue Side;
	  	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACMotorCtrlWnd)
	public:
	//virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//virtual ~CACMotorCtrlWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CACMotorCtrlWnd)
	afx_msg void OnRadioAxis1();
	afx_msg void OnRadioAxis2();
	afx_msg void OnRadioAxis3();
	afx_msg void OnRadioAxis4();
	afx_msg void OnSelchangeAcCtrlmode();
	afx_msg void OnAcAxison();
	afx_msg void OnAcAxisoff();
	afx_msg void OnAcStop();
	afx_msg void OnAcRun();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnClear();
	afx_msg void OnBtnPrepage();
	afx_msg void OnBtnShowall();
	afx_msg void OnBtnNextpage();
	afx_msg void OnUpdateSetkp();
	afx_msg void OnUpdateSetki();
	afx_msg void OnUpdateSetkd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonXpositive();
	afx_msg void OnBnClickedButtonXreverse();
	afx_msg void OnBnClickedButtonYpositive();
	afx_msg void OnBnClickedButtonYrerverse();
	afx_msg void GetJogIncValue(double * fDist);
       afx_msg void FindCircle();

	//afx_msg void OnBnDropDownButtonXpositive(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckLeftIn();
	afx_msg void OnBnClickedCheckRightIn();

	   afx_msg void OnBnClickedButtonOpenfile();
	   afx_msg void OnBnClickedButtonNewfile();

	   afx_msg void OnEnChangeEditDiameter();
	   afx_msg void OnDeltaposSpinDiameter(NMHDR *pNMHDR, LRESULT *pResult);
	   afx_msg void OnBnClickedButtonModify();
	   afx_msg void OnBnClickedCheckLeftProgram();
	   afx_msg void OnBnClickedCheckRightProgram();
	   afx_msg void OnBnClickedButtonZpositive();
	   afx_msg void OnBnClickedButtonZrerverse();
	   afx_msg void OnBnClickedCcdLightOnoff();
	   afx_msg void OnBnClickedMainAxic();
	   afx_msg void OnBnClickedDustCollector();
	   afx_msg void OnBnClickedLeftAirDoor();
	   afx_msg void OnBnClickedRightAirDoor();
	   afx_msg void OnBnClickedZeroButton();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACMOTORCTRLWND_H__4B3AC106_1030_427F_93C8_B7105D90F2DF__INCLUDED_)
