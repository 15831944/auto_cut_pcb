// InterpExpCtrlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "InterpExpCtrlWnd.h"
#include "MainFrm.h"
#include "MotorControlBench.h"

#include "DigitalDisp.h"
#include "GT400.h"
#include "GTCard.h"

#include "Global.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//class CGTCard;
extern CGTCard gtcard;

/////////////////////////////////////////////////////////////////////////////
CPOSITIONList g_PointList;

/////////////////////////////////////////////////////////////////////////////
// CInterpExpCtrlWnd

//int func(int i);
IMPLEMENT_DYNCREATE(CInterpExpCtrlWnd, CFormView)

CInterpExpCtrlWnd::CInterpExpCtrlWnd()
	: CFormView(CInterpExpCtrlWnd::IDD)
{
	//{{AFX_DATA_INIT(CInterpExpCtrlWnd)
	m_acc = 0.1;
	m_fAngle = 0.0;
	m_nDirect = 0;
	m_nEndX = 30000;
	m_nEndY = 40000;
	m_fRadius = 0.0;
	m_fVel = 8.0;
	m_fXCenter = 0.0;
	m_fYCenter = 0.0;
	m_nSizeOfRegister = 0;
	//}}AFX_DATA_INIT
   m_bMapError=true;
   m_bAxisMap = false;
   m_bServOn=false;
   InitCard();
}

CInterpExpCtrlWnd::~CInterpExpCtrlWnd()
{
}

void CInterpExpCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterpExpCtrlWnd)
	DDX_Control(pDX, IDC_MAP_BUTTON, m_btnMap);
	DDX_Control(pDX, IDC_YMAX_COMBO, m_cmbYMax);
	DDX_Control(pDX, IDC_YMAP_COMBO, m_cmbYMap);
	DDX_Control(pDX, IDC_XMAX_COMBO, m_cmbXMax);
	DDX_Control(pDX, IDC_XMAP_COMBO, m_cmbXMap);
	DDX_Control(pDX, IDC_SEL_COMBO, m_cmbInterpMode);
	DDX_Text(pDX, IDC_ACC_EDIT, m_acc);
	DDX_Text(pDX, IDC_ANGLE_EDIT, m_fAngle);
	DDX_Radio(pDX, IDC_DIRECTION_RADIO, m_nDirect);
	DDX_Text(pDX, IDC_ENDX_EDIT, m_nEndX);
	DDX_Text(pDX, IDC_ENDY_EDIT, m_nEndY);
	DDX_Text(pDX, IDC_R_EDIT, m_fRadius);
	DDX_Text(pDX, IDC_VEL_EDIT, m_fVel);
	DDX_Text(pDX, IDC_XCIR_EDIT, m_fXCenter);
	DDX_Text(pDX, IDC_YCIR_EDIT, m_fYCenter);
	DDX_Text(pDX, IDC_SIZEOFREGISTER, m_nSizeOfRegister);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInterpExpCtrlWnd, CFormView)
	//{{AFX_MSG_MAP(CInterpExpCtrlWnd)
	ON_BN_CLICKED(IDC_MAP_BUTTON, OnMapButton)
	ON_CBN_SELCHANGE(IDC_SEL_COMBO, OnSelchangeSelCombo)
	ON_BN_CLICKED(IDC_SERVO_ON, OnServoOn)
	ON_BN_CLICKED(IDC_RUN_BUTTON, OnRunButton)
	ON_BN_CLICKED(IDC_SERVO_OFF, OnServoOff)
	ON_BN_CLICKED(IDC_STOP_BUTTON, OnStopButton)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_XMAP_COMBO, OnSelchangeXmapCombo)
	ON_CBN_SELCHANGE(IDC_XMAX_COMBO, OnSelchangeXmaxCombo)
	ON_CBN_SELCHANGE(IDC_YMAX_COMBO, OnSelchangeYmaxCombo)
	ON_EN_UPDATE(IDC_ENDX_EDIT, OnUpdateEndxEdit)
	ON_EN_CHANGE(IDC_ENDY_EDIT, OnChangeEndyEdit)
	ON_BN_CLICKED(IDC_BTN_PEN_ON, OnBtnPenOn)
	ON_BN_CLICKED(IDC_BTN_PEN_OFF, OnBtnPenOff)
	ON_BN_CLICKED(IDC_BTN_AXISX_HOME, OnBtnAxisxHome)
	ON_BN_CLICKED(IDC_BTN_AXISY_HOME, OnBtnAxisyHome)
	ON_EN_KILLFOCUS(IDC_ACC_EDIT, OnKillfocusAccEdit)
	ON_EN_KILLFOCUS(IDC_VEL_EDIT, OnKillfocusVelEdit)
	ON_CBN_SELCHANGE(IDC_YMAP_COMBO, OnSelchangeYmapCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterpExpCtrlWnd diagnostics

#ifdef _DEBUG
void CInterpExpCtrlWnd::AssertValid() const
{
	CFormView::AssertValid();
}

void CInterpExpCtrlWnd::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInterpExpCtrlWnd message handlers

void CInterpExpCtrlWnd::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();	
	GetWndPoint();
	
	int nAxisCount;
	nAxisCount=((CMotorControlBenchApp *)AfxGetApp())->m_nAxisCount;
	CString strAxis[4]={"1轴","2轴","3轴","4轴"};
	
	m_cmbXMap.ResetContent();
	m_cmbYMap.ResetContent();
	
	for(int i=0;i<nAxisCount;i++)
	{		
		m_cmbXMap.InsertString(i,strAxis[i]);		
		m_cmbYMap.InsertString(i,strAxis[i]);
	}
	m_cmbXMap.SetCurSel(0);
	m_cmbYMap.SetCurSel(1);
	
	m_cmbXMax.SetCurSel(3);
	m_cmbYMax.SetCurSel(3);
	m_cmbInterpMode.SetCurSel(0);
	
	GetDlgItem(IDC_XCIR_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_YCIR_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_R_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_ANGLE_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DIRECTION_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_DIRECTION_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CCW)->EnableWindow(FALSE);
	GetDlgItem(IDC_SIZEOFREGISTER)->EnableWindow(FALSE);
	
	m_nXMap = 1;
	m_nYMap = 2;
	m_nDirect=0;
	
	UpdateData(FALSE);
	
}

void CInterpExpCtrlWnd::OnMapButton() 
{
	m_nXMap=m_cmbXMap.GetCurSel()+1;
	m_nYMap=m_cmbYMap.GetCurSel()+1;
	
	if(m_nXMap!=m_nYMap)
	{		
		m_bMapError=false;
		
	}
	else{
		MessageBox("请选择不同轴号！","轴映射错误",MB_ICONERROR);
		m_bMapError=true;
	}	
	m_bAxisMap=true;
	MessageBox("下一步请点击“开启轴”按钮","XX科技",MB_ICONINFORMATION);
}

void CInterpExpCtrlWnd::OnSelchangeSelCombo() 
{
	GetDlgItem(IDC_ENDX_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_ENDY_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_XCIR_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_YCIR_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_R_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_ANGLE_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DIRECTION_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CCW)->EnableWindow(FALSE);
	GetDlgItem(IDC_SIZEOFREGISTER)->EnableWindow(FALSE);
	

	int nInterpMode;
	nInterpMode=m_cmbInterpMode.GetCurSel();

	switch(nInterpMode) {
	case 0://直线插补
		GetDlgItem(IDC_ENDX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENDY_EDIT)->EnableWindow(TRUE);
		break;
	case 1://圆弧插补（圆心\角度)
		GetDlgItem(IDC_XCIR_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_YCIR_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANGLE_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIUS_OR_STEP)->SetWindowText("角   度:");
		GetDlgItem(IDC_CENX_OR_SRTX)->SetWindowText("圆  心(X):");
		GetDlgItem(IDC_CENY_OR_SRTY)->SetWindowText("圆  心(Y):");
		m_fXCenter=-40000;
		m_fYCenter=0;
		m_fAngle=300;
		break;
	case 2://圆弧插补（终点\半径)
		GetDlgItem(IDC_ENDX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENDY_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MULTI)->SetWindowText("半  径:");
		GetDlgItem(IDC_R_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DIRECTION_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CCW)->EnableWindow(TRUE);
		m_fRadius=50000;
		m_nEndX=40000;
		m_nEndY=30000;
		m_nDirect=0;
		break;
	case 3://逐点比较法直线插补
		GetDlgItem(IDC_ENDX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENDY_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MULTI)->SetWindowText("步  长:");
		GetDlgItem(IDC_R_EDIT)->EnableWindow(TRUE);
		m_fRadius=1000;
		break;
	case 4://逐点比较法圆弧插补
		GetDlgItem(IDC_ENDX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENDY_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MULTI)->SetWindowText("半  径:");
		GetDlgItem(IDC_R_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DIRECTION_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CCW)->EnableWindow(TRUE);	
		GetDlgItem(IDC_RADIUS_OR_STEP)->SetWindowText("步  长:");
		GetDlgItem(IDC_XCIR_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_YCIR_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CENX_OR_SRTX)->SetWindowText("起  点(X):");
		GetDlgItem(IDC_CENY_OR_SRTY)->SetWindowText("起  点(Y):");
		GetDlgItem(IDC_ANGLE_EDIT)->EnableWindow(TRUE);	
		m_fXCenter=50000;
		m_fYCenter=0;
		m_nEndX=-40000;
		m_nEndY=-30000;
		m_fRadius=50000;
		m_fAngle=1000;
	//	m_pInterpDrawView->m_nAxisFlag=0;
		break;
	case 5://数字积分法直线插补
		GetDlgItem(IDC_ENDX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENDY_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SIZEOFREGISTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MULTI)->SetWindowText("步  长:");	
		GetDlgItem(IDC_RADIUS_OR_STEP)->SetWindowText("角   度:");
		GetDlgItem(IDC_ANGLE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_R_EDIT)->EnableWindow(TRUE);
		m_nEndX=-40000;
		m_nEndY=-30000;
		m_fRadius=1000;
		m_nSizeOfRegister=6;
		break;
	case 6://数字积分法圆弧插补
		GetDlgItem(IDC_ENDX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENDY_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MULTI)->SetWindowText("半  径:");
		GetDlgItem(IDC_R_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DIRECTION_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CCW)->EnableWindow(TRUE);	
		GetDlgItem(IDC_RADIUS_OR_STEP)->SetWindowText("步  长:");
		GetDlgItem(IDC_XCIR_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_YCIR_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CENX_OR_SRTX)->SetWindowText("起  点(X):");
		GetDlgItem(IDC_CENY_OR_SRTY)->SetWindowText("起  点(Y):");
		GetDlgItem(IDC_ANGLE_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SIZEOFREGISTER)->EnableWindow(true);
		m_fXCenter=0;
		m_fYCenter=50000;
		m_nEndX=50000;
		m_nEndY=0;
		m_fRadius=50000;
		m_nSizeOfRegister=3;
		m_fAngle=10000;
		break;		
	}
	UpdateData(FALSE);
}


void CInterpExpCtrlWnd::GetWndPoint()
{
	CMainFrame * pMainFrm;	
	pMainFrm=(CMainFrame *) AfxGetApp()->m_pMainWnd;
	m_pInterpFrm=pMainFrm->m_pInterpFrame;
	m_pInterpDrawView=m_pInterpFrm->m_pInterpDrawView;
}

void CInterpExpCtrlWnd::OnServoOn() 
{
	CString str;
	
	if(m_bMapError)
	{
		MessageBox("尚未正确设置轴映射关系！\n请点击“坐标映射生效”按钮","XX科技",MB_ICONWARNING|MB_OK);
		return;
	}

	if(!m_bAxisMap)
	{
		int ret=MessageBox("请先点击“轴映射生效”按钮","",MB_OK|MB_ICONQUESTION);	
	    return;		
	}
	
	InitCard();

	GT_Axis(m_nXMap);
	GT_AxisOn();

	GT_Axis(m_nYMap);
	GT_AxisOn();

	m_bServOn=true;
	MessageBox("轴已经开启！\n请根据需要点击“运行”或“回零”按钮","XX科技",MB_ICONINFORMATION|MB_OK);

}

void CInterpExpCtrlWnd::OnRunButton() 
{

	short rtn;
	CString str;
	int i;

    
	double nCNT = 1;
	double cnt1[5] = {nCNT,0,0,0,0};
	double cnt2[5] = {0,nCNT,0,0,0};
	
	UpdateData(TRUE);
	
	if(gtcard.IsCardMove())
	{	
		MessageBox("有轴在运动中,请先停止轴运动再点击“运行”按钮","警告",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(!m_bServOn)
	{	
		MessageBox("请先点击“开启轴”按钮！","警告",MB_OK|MB_ICONWARNING);
		return;
	}
	
   	if(m_bMapError)
	{
		MessageBox("X、Y轴映射成同一个轴，请修改后再运行！","轴映射错误",MB_ICONERROR|MB_OK);
		return;
	}
	
	if(!m_bAxisMap)
	{//default axis mapping
		int ret=MessageBox("您新设置的轴映射尚未生效\n请点击“轴映射生效”按钮","",MB_OK|MB_ICONQUESTION);	
		if(ret==IDNO)
		{			
			rtn = GT_MapAxis(1,cnt1);   //  Error(rtn,"GT_MapAxis(1)");
			rtn = GT_MapAxis(2,cnt2);   //  Error(rtn,"GT_MapAxis(2)");	
		}
		else return;		
	}
	else
	{	
        rtn = GT_MapAxis(m_nXMap,cnt1);   //  Error(rtn,"GT_MapAxis(xmap)");
        rtn = GT_MapAxis(m_nYMap,cnt2);   //  Error(rtn,"GT_MapAxis(ymap)");	
	}

   //检查参数范围
   if(	CheckParaLimit()==false) 
	   return;	  
   //位置清零	
	for(i=1;i<5;i++)
	{
		rtn=GT_Axis(i);   //   Error(rtn,"GT_Axis");
		rtn=GT_ZeroPos();   // Error(rtn,"GT_ZeroPos");
	}
	
	g_PointList.RemoveAll();

	CDC * pDC;
      pDC=m_pInterpDrawView->GetDC();
	CRect rectAxis;
	rectAxis=m_pInterpDrawView->rectAxis;
	
	int nXMax,nYMax;
	nXMax=m_pInterpDrawView->m_nXMax;
	nYMax=m_pInterpDrawView->m_nYMax;

	int sel=m_cmbInterpMode.GetCurSel();
	switch(sel)
	{
	case 0:
		LnXY();//执行两维直线插补算法         
		m_pInterpDrawView->m_nCurveType=0;
		break;
	case 1:
		ArcXY();//X-Y平面圆弧插补（以圆心位置和角度为输入参数）
		m_pInterpDrawView->m_nCurveType=0;
		break;
	case 2:
		ArcXYP();//X-Y平面圆弧插补（以终点位置和半径为输入参数）
		m_pInterpDrawView->m_nCurveType=0;
		break;   
	case 3:  //执行两维逐点比较法直线插补
		m_Interp.LineInterp_MIT_Buffer((int)m_fRadius,m_nEndX,m_nEndY);
		m_pInterpDrawView->m_lXEnd=m_nEndX;
		m_pInterpDrawView->m_lYEnd=m_nEndY;
		m_pInterpDrawView->m_nCurveType=1;	
		break;
	case 4:  //执行两维逐点比较法圆弧插补
		m_Interp.CirInterp_MIT_Buffer((int)m_fAngle,(int)m_fXCenter,(int)m_fYCenter,m_nEndX,m_nEndY,m_fRadius,!m_nDirect);
		m_pInterpDrawView->m_lXEnd=m_nEndX;
		m_pInterpDrawView->m_lYEnd=m_nEndY;
		m_pInterpDrawView->m_lXSt=m_fXCenter;
		m_pInterpDrawView->m_lYst=m_fYCenter;
		m_pInterpDrawView->m_lRadius=(long)m_fRadius;
		m_pInterpDrawView->m_nCurveType=2;	
		break;
	case 5://执行两维DDA直线插补
		m_Interp.LineInterp_DDA_Buffer((int)m_nSizeOfRegister,(long)m_fRadius,m_nEndX,m_nEndY,0);
		m_pInterpDrawView->m_lXEnd=m_nEndX;
		m_pInterpDrawView->m_lYEnd=m_nEndY;
		m_pInterpDrawView->m_nCurveType=1;
		break;
	case 6://执行两维DDA圆弧插补
		m_Interp.CircleInterp_DDA_Buffer(m_nSizeOfRegister,(int)m_fAngle,(int)m_fXCenter,(int)m_fYCenter,m_nEndX,m_nEndY,m_fRadius,!m_nDirect);
		m_pInterpDrawView->m_lXEnd=m_nEndX;
		m_pInterpDrawView->m_lYEnd=m_nEndY;
		m_pInterpDrawView->m_lXSt=m_fXCenter;
		m_pInterpDrawView->m_lYst=m_fYCenter;
		m_pInterpDrawView->m_lRadius=(long)m_fRadius;
		m_pInterpDrawView->m_nCurveType=2;
		break;
	} 
	
	int m_timer=SetTimer(1,50,NULL);
	ASSERT(m_timer!=0);
	Sleep(100);
	m_pInterpDrawView->m_bDrawFlag=true;

}

void CInterpExpCtrlWnd::OnServoOff() 
{	
	CString strInfo;
    
	GT_Axis(m_nXMap);
	if(GT_AxisOff())
	{
		strInfo.Format("%d轴关闭失败！",m_nXMap);
		MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
		return;
	}
	GT_Axis(m_nYMap);
	if(GT_AxisOff())
	{
		strInfo.Format("%d轴关闭失败！",m_nYMap);
		MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
		return;
	}	
	m_bServOn=false;
	MessageBox("轴已经关闭！\n 要进行实验前请先点击“开启轴”","XX科技",MB_OK|MB_ICONINFORMATION);
}

void CInterpExpCtrlWnd::OnStopButton() 
{
	short rtn;

    #if 0
    	CString strInfo;
	double pnt[4];


    	gtcard.GetPrflPnt(pnt);	//确定轴当前点位置 
	strInfo.Format("position:%f, %f, %f, %f", pnt[0],pnt[1],pnt[2], pnt[3]);
       MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

      rtn = GT_SetSynAcc(m_acc);    //Error(rtn,"GT_SetSynAcc");
	rtn = GT_SetSynVel(m_fVel);    //Error(rtn,"GT_SetSynVel");
	rtn=GT_LnXY(m_nEndX+10000,m_nEndY+15000);  // Error(rtn,"GT_LnXY");

        strInfo.Format("wait");
        MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

    	gtcard.GetPrflPnt(pnt);	//确定轴当前点位置 
	strInfo.Format("position:%f, %f, %f", pnt[0],pnt[1],pnt[2], pnt[3]);
       MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
    #endif
    
    #if 0
    	CString strInfo;
	long pos=0, Atlpos=0;

    	 GT_Axis(1);
        GT_GetPos(&pos);
        GT_GetAtlPos(&Atlpos);
        strInfo.Format("pos = %d, Atlpos = %d",pos, Atlpos);
        MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

    	 GT_Axis(2);
        GT_GetPos(&pos);
        GT_GetAtlPos(&Atlpos);
        strInfo.Format("pos = %d, Atlpos = %d",pos, Atlpos);
        MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

      rtn = GT_SetSynAcc(m_acc);    //Error(rtn,"GT_SetSynAcc");
	rtn = GT_SetSynVel(m_fVel);    //Error(rtn,"GT_SetSynVel");
	rtn=GT_LnXY(m_nEndX+10000,m_nEndY+15000);  // Error(rtn,"GT_LnXY");

        strInfo.Format("wait");
        MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
      
        Atlpos = 0;
    	 GT_Axis(1);
        GT_GetPos(&pos);
        GT_GetAtlPos(&Atlpos);
        strInfo.Format("pos = %d, Atlpos = %d",pos, Atlpos);
        MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

    	 GT_Axis(2);
        GT_GetPos(&pos);
        GT_GetAtlPos(&Atlpos);
        strInfo.Format("pos = %d, Atlpos = %d",pos, Atlpos);
        MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
#endif

	rtn = GT_StpMtn();
	
	KillTimer(1);
	m_pInterpDrawView->m_bDrawFlag=false;
	MessageBox("轴已经停止运动！","XX科技",MB_OK|MB_ICONINFORMATION);
	
}

void CInterpExpCtrlWnd::LnXY()
{
    short rtn;
    rtn = GT_SetSynAcc(m_acc);    //Error(rtn,"GT_SetSynAcc");
    rtn = GT_SetSynVel(m_fVel);    //Error(rtn,"GT_SetSynVel");
    rtn=GT_LnXY(m_nEndX,m_nEndY);  // Error(rtn,"GT_LnXY");
    unsigned short mode;
    rtn = GT_GetMode(&mode);
}

void CInterpExpCtrlWnd::ArcXY()
{
	short rtn;
	rtn = GT_SetSynAcc(m_acc);    //Error(rtn,"GT_SetSynAcc");
	rtn = GT_SetSynVel(m_fVel);    //Error(rtn,"GT_SetSynVel");
	Sleep(10);
	rtn = GT_ArcXY(m_fXCenter,m_fYCenter,m_fAngle);   // Error(rtn,"GT_ArcXY");
}

void CInterpExpCtrlWnd::ArcXYP()
{
	short rtn;
	short dir;
	rtn=GT_StrtList();
    rtn=GT_MvXY(0,0,m_fVel,m_acc);
	if(m_nDirect==0)
		dir=1;
	else
		dir=-1;
	rtn=GT_ArcXYP(m_nEndX,m_nEndY,m_fRadius,dir);   //  Error(rtn,"GT_ArcXYP");
	rtn=GT_EndList();
	rtn=GT_StrtMtn();

}

void CInterpExpCtrlWnd::OnTimer(UINT nIDEvent) 
{
	int i=0;
	short rtn;
	unsigned short sts[3];
	long actl_pos[4];

	//采样当前位置
	for(i=0;i<4;i++)
	{
		rtn=GT_Axis(i+1);
		rtn=GT_ClrSts();
		rtn=GT_GetAtlPos(&actl_pos[i]);
	}

	//将当前采集数据放入链表中
	CPOSITION *pos=new CPOSITION;
      pos->x=actl_pos[m_nXMap - 1];
	pos->y=actl_pos[m_nYMap - 1];
	pos->z=0;
	UpdateLed(IDC_INTERP_XPOSITION,pos->x);
	UpdateLed(IDC_INTERP_YPOSITION,pos->y);	
	g_PointList.AddTail(pos);

	//在图形显示区绘图
	m_pInterpDrawView->DrawChart();	
	
	//获取电机状态
	for( i=0;i<4;i++)
	{
		rtn = GT_Axis(i+1);
		rtn = GT_GetSts(&sts[i]);
	}	
	
	//若电机静止，则停止采样
	if((!(sts[0]&0x400))&&(!(sts[1]&0x400))&&(!(sts[2]&0x400)))
	{
		KillTimer(1);
	}
	CFormView::OnTimer(nIDEvent);
}

void CInterpExpCtrlWnd::UpdateLed(UINT nID, double dispValue)
{
	CDigitalDisp * pLed=(CDigitalDisp *) GetDlgItem(nID);	
	pLed->SetNumToDisp(dispValue);
}


void CInterpExpCtrlWnd::OnSelchangeXmapCombo() 
{
	m_bAxisMap=false;
	m_bServOn=false;
}

void CInterpExpCtrlWnd::OnSelchangeXmaxCombo() 
{
	int sel=m_cmbXMax.GetCurSel();
	m_pInterpDrawView->m_nXMax=10*SetScale(sel);
	m_pInterpDrawView->DrawChart();
}

void CInterpExpCtrlWnd::OnSelchangeYmaxCombo() 
{
    int sel=m_cmbYMax.GetCurSel();
	m_pInterpDrawView->m_nYMax=10*SetScale(sel);
	m_pInterpDrawView->DrawChart();
}


void CInterpExpCtrlWnd::OnUpdateEndxEdit() 
{
	UpdateData(TRUE);	
}

void CInterpExpCtrlWnd::OnChangeEndyEdit() 
{
	UpdateData(TRUE);	
}

int CInterpExpCtrlWnd::SetScale(int nSel)
{
	int data;
	switch(nSel)
	{
	case 0:
		data=10;
		break;
	case 1:
		data=100;
		break;
	case 2:
		data=1000;
		break;
	case 3:
		data=10000;
		break;
	case 4:
		data=100000;
		break;
	case 5:
		data=1000000;
		break;
	case 6:
		data=10000000;
		break;
	}
	return data;

}

void CInterpExpCtrlWnd::InitCard()
{
    gtcard.InitCard(); 

    int i;
    int nCtrlMode[4];
    int nDirPulse[4];
    short nKp[4],nKi[4],nKd[4];		

    for(i = 0; i < 4; i ++)
    {
        if(((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i] == SERVO_MOTOR) //如果轴为伺服电机
        {
        	nCtrlMode[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i];
        	nKp[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKp[i];
        	nKi[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKi[i];
        	nKd[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKd[i];
        }

        else if(((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i] == STEP_MOTOR)//如果轴为步进电机
        {
        	nCtrlMode[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i];
        	nDirPulse[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nDirPls[i];
        }
    }	


    for(i = 0; i < 4; i ++)
    {
        gtcard.AxisInitial(nCtrlMode[i],nDirPulse[i],nKp[i],nKi[i],nKd[i],i+1);
    }

}

void CInterpExpCtrlWnd::OnBtnPenOn() 
{
	gtcard.OpenPort(1,true);
	MessageBox("笔架落下","XX科技",MB_OK|MB_ICONINFORMATION);	
}



void CInterpExpCtrlWnd::OnBtnPenOff() 
{

	gtcard.OpenPort(1,false);
	MessageBox("笔架已经抬起","XX科技",MB_OK|MB_ICONINFORMATION);
}

void CInterpExpCtrlWnd::OnBtnAxisxHome() 
{
	UpdateData(true);
	
	if(!m_bServOn)
	{
		MessageBox("请先点击“开启轴”按钮！","XX科技",MB_ICONINFORMATION|MB_OK);
		m_bAxisMap=false;
		return;
	}	
	if (gtcard.IsCardMove()) {
		MessageBox("请等轴停止运动后再回零","",MB_OK);
		return;
	}
    gtcard.Home(m_nXMap,HOME_TWOLIMITS);
   
}

void CInterpExpCtrlWnd::OnBtnAxisyHome() 
{
	UpdateData(true);

	
	if(!m_bServOn)
	{
		MessageBox("Y轴伺服尚未开启！","XX科技",MB_ICONINFORMATION|MB_OK);
		m_bAxisMap=false;
		return;
	}
	if (gtcard.IsCardMove()) {
		MessageBox("请等轴停止运动后再回零","",MB_OK);
		return;
	}
	 gtcard.Home(m_nYMap,HOME_TWOLIMITS);
}

void CInterpExpCtrlWnd::OnKillfocusAccEdit() 
{
	  UpdateData(TRUE);
	if((m_acc>20)||(m_acc<0))
	{
		MessageBox("加速度范围为0～20pluse/st^2","XX科技",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_VEL_EDIT)->SetFocus();
		return;
	}
 
}

void CInterpExpCtrlWnd::OnKillfocusVelEdit() 
{
	UpdateData(TRUE);
	if((m_fVel>500)||(m_fVel<0))
	{
		MessageBox("速度范围为0～500pluse/st","XX科技",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_ACC_EDIT)->SetFocus();
		return;
	}
}

void CInterpExpCtrlWnd::OnSelchangeYmapCombo() 
{
	m_bAxisMap=false;
	m_bServOn=false;
}

bool CInterpExpCtrlWnd::CheckParaLimit()
{
	double temp1,temp2;

	UpdateData(TRUE);
	//加速度检验//
	if((m_acc>10)||(m_acc<=0))
	{
		MessageBox("加速度值不合理！请重新设置\n加速度范围为0～10pluse/st^2","XX科技",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_VEL_EDIT)->SetFocus();
		return false;
	}
   //速度检验//
	if((m_fVel>100)||(m_fVel<1))
	{
		MessageBox("速度值不合理！请重新设置\n速度范围为1～100pluse/st","XX科技",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_ACC_EDIT)->SetFocus();
		return false;
	}
	if ((m_nEndX<=-1000000)||(m_nEndX>=1000000) ){
		MessageBox("终点X坐标值设置范围为-100000~1000000","XX科技",MB_OK|MB_ICONWARNING);
		return false;
	}
	if ((m_nEndY<=-1000000)||(m_nEndY>=1000000) ){
		MessageBox("终点Y坐标值设置范围为-100000~1000000","XX科技",MB_OK|MB_ICONWARNING);
		return false;
	}
 //插补参数检验//
	int nInterpMode;
	nInterpMode=m_cmbInterpMode.GetCurSel();
	
	switch(nInterpMode) {
	case 0://XY直线插补
	    if ((m_nEndX==0)&&(m_nEndY==0) ){
			MessageBox("直线插补时，终点坐标不能设置为（0，0）！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
	    }
		
		break;
	case 1://XY圆弧插补（圆心/角度）
		if ((m_fXCenter==0)&&(m_fYCenter==0) ){
			MessageBox("圆弧插补（圆心角度型）时，圆心坐标不能设置为（0，0）！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if ((m_fXCenter<-1000000)||(m_fXCenter>1000000) ){
			MessageBox("圆弧插补（圆心角度型）时，圆心X坐标值超出范围！","XX科技",MB_OK|MB_ICONWARNING);
		    return false;
		}
        if ((m_fYCenter<-1000000)||(m_fYCenter>1000000) ){
			MessageBox("圆弧插补（圆心角度型）时，圆心Y坐标值超出范围！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
			
		if (m_fAngle==0 ){
			MessageBox("圆弧插补（圆心角度型）时，插补角度不能为0！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}

		if ((m_fAngle<-360)||(m_fAngle>360) ){
			MessageBox("圆弧插补（圆心角度型）时，插补角度设置范围是-360~360度！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		
	   
		break;
	case 2://XY圆弧插补（终点/半径）
	       if ((m_nEndX==0)&&(m_nEndY==0) ){
			   MessageBox("圆弧插补（终点半径型）时，终点坐标不能设置为（0，0）！","XX科技",MB_OK|MB_ICONWARNING);
			   return false;
		   }
		   if ((m_fRadius<=0)||(m_fRadius>1000000) ){
			   MessageBox("圆弧插补（终点半径型）时，半径值超出范围！","XX科技",MB_OK|MB_ICONWARNING);
			   return false;
		   }
		 
		   temp1= 4*m_fRadius*m_fRadius/10000;
		   temp2=(double) (m_nEndX/100)*(m_nEndX/100)+(m_nEndY/100)*(m_nEndY/100); //此处必须转为double型，long型会溢出 /1000也是此目的
		   if(temp1<temp2){
			   MessageBox("圆弧插补（终点半径型）半径值设置错误，不能正确构成圆弧！","XX科技",MB_OK|MB_ICONWARNING);
			   return false;
		   }
		   break;
	case 3://逐点比较法直线插补
		if ((m_nEndX==0)&&(m_nEndY==0) ){
			MessageBox("逐点比较法直线插补时，终点坐标不能设置为（0，0）！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if ((m_fRadius<=0)||(m_fRadius>10000) ){
			MessageBox("逐点比较法直线插补时，步长值超出范围！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}	
		break;
	case 4://逐点比较法圆弧插补
		if ((m_nEndX==0)&&(m_nEndY==0) ){
			MessageBox("逐点比较法圆弧插补时，终点坐标不能设置为（0，0）！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if (((int)m_fXCenter==0)&&((int)m_fYCenter==0) ){
			MessageBox("逐点比较法圆弧插补时，起点坐标不能设置为（0，0）！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if ((m_fRadius<=0)||(m_fRadius>1000000) ){
			MessageBox("逐点比较法圆弧插补时，半径值超出范围！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
	   
		if ((m_fAngle<=0)||(m_fAngle>10000) ){
			MessageBox("逐点比较法圆弧插补时，步长值超出范围！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}	
	
		temp1=m_fXCenter*m_fXCenter+m_fYCenter*m_fYCenter;
		temp2=m_nEndX*m_nEndX+m_nEndY*m_nEndY;
		if (abs(temp1-m_fRadius*m_fRadius)>=1) {
			MessageBox("逐点比较法圆弧插补时起点值与半径值不匹配，不能正确构成圆弧！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if (abs(temp2-m_fRadius*m_fRadius)>=1) {
			MessageBox("逐点比较法圆弧插补时终点值与半径值不匹配，不能正确构成圆弧！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		break;
	case 5://数字积分法直线插补
		int step,countOfBit;
		long sizeOfResigister;
		long Bigger;
		
		step=(int)m_fRadius;
		countOfBit=(int)m_nSizeOfRegister;
		sizeOfResigister=pow((double)2, (int)countOfBit);


		if (abs(m_nEndX)>=abs(m_nEndY)) {
			Bigger=abs(m_nEndX);
		}
		else{
			Bigger=abs(m_nEndY);
		}
		if (sizeOfResigister<(int)(Bigger/m_fRadius)) {
			MessageBox("DDA直线插补时寄存器容量、终点位置和步长设置不匹配！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}

		if (step<=0) {
			MessageBox("DDA直线插补时步长设置错误！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
   

		break;
	case 6:
		step=(int)m_fRadius;
		countOfBit=(int)m_nSizeOfRegister;
		sizeOfResigister=pow((double)2, (int)countOfBit);
		
		if ((m_nEndX==0)&&(m_nEndY==0) ){
			MessageBox("数字积分法圆弧插补时，终点坐标不能设置为（0，0）！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}

		if (((int)m_fXCenter==0)&&((int)m_fYCenter==0) ){
			MessageBox("数字积分法圆弧插补时，起点坐标不能设置为（0，0）！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		
		if (abs(m_nEndX)>=abs(m_nEndY)) {
			Bigger=abs(m_nEndX);
		}
		else{
			Bigger=abs(m_nEndY);
		}
		if (sizeOfResigister<(int)(Bigger/m_fRadius)) {
			MessageBox("DDA圆弧插补时寄存器容量、终点位置和步长设置不匹配！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}		

		if ((m_fAngle<=0)||(m_fAngle>10000) ){
			MessageBox("DDA圆弧插补时，步长值设置错误或超出范围！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}	
		
		temp1=m_fXCenter*m_fXCenter+m_fYCenter*m_fYCenter;
		temp2=m_nEndX*m_nEndX+m_nEndY*m_nEndY;
		if (abs(temp1-m_fRadius*m_fRadius)>=1) {
			MessageBox("DDA圆弧插补时起点值与半径值不匹配，不能正确构成圆弧！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if (abs(temp2-m_fRadius*m_fRadius)>=1) {
			MessageBox("DDA圆弧插补时终点值与半径值不匹配，不能正确构成圆弧！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if(!((m_fXCenter>=0)&&(m_fYCenter>=0))) {
			MessageBox("本版本只提供第一象限DDA圆弧插补！起点不在第一象限内。","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if(!((m_nEndX>=0)&&(m_nEndY>=0))) {
			MessageBox("本版本只提供第一象限DDA圆弧插补！终点不在第一象限内。","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if ((m_fXCenter>m_nEndX)&&(m_nDirect==0)) {
			MessageBox("本版本只提供第一象限DDA圆弧插补，顺圆插补起点和终点有误！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		if ((m_fXCenter<m_nEndX)&&(m_nDirect==1)) {
			MessageBox("本版本只提供第一象限DDA圆弧插补，逆圆插补起点和终点有误！","XX科技",MB_OK|MB_ICONWARNING);
			return false;
		}
		break;

	}
	return true;
}
