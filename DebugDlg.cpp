// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "DebugDlg.h"
#include"DigitalDisp.h"

#include "GTCard.h"
#include "GT400.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CGTCard gtcard;
/////////////////////////////////////////////////////////////////////////////
// CDebugDlg dialog
DWORD WINAPI HomeThread(LPVOID pParam)
{
 // gtcard.Home(1,HOME_TWOLIMITS);
	short rtn;
	unsigned short status;
	long lPosLimPos,lNegLimPos,lZeroPos;
	double fHomeVel(3),fHomeAcc(0.01);
	
	int nHomeMode=HOME_TWOLIMITS;
	int nAxis(1);
//	fHomeVel=pDoc->m_machPara.HomeVel[nAxis-1]/**1000*0.0002/60*/;
//	fHomeAcc=pDoc->m_machPara.HomeAcc[nAxis-1]/**1000*0.0002/60*0.0002/60*/;
	fHomeVel=5;
	fHomeAcc=0.2;
	

		//轴正向转动//
		rtn=GT_Axis(nAxis);
		rtn=GT_ClrSts();
		rtn=GT_PrflT();
		rtn=GT_SetVel(fHomeVel);
		rtn=GT_SetAcc(fHomeAcc);
		rtn=GT_ZeroPos();
		rtn=GT_SetPos(500000);
		rtn=GT_Update();
		rtn=GT_GetSts(&status);		
		//检测正限位信号//
		while ((status&0x20)!=0x20) {		
			if(status&0x01==0x01) 
			{
				AfxMessageBox("运动完成，未检测到正限位信号",MB_OK);
				return 0;
			}
			Sleep(20);
			rtn=GT_Axis(nAxis);
			rtn=GT_GetSts(&status);	
			Sleep(20);
		}
		rtn=GT_Axis(nAxis);
		rtn=GT_GetAtlPos(&lPosLimPos);     
		rtn=GT_SetPos(-500000);		
		rtn=GT_Update();
		
		rtn=GT_Axis(nAxis);
		rtn=GT_ClrSts();
		//检测负限位信号//
		Sleep(20);
		rtn=GT_GetSts(&status);
		Sleep(20);
		while ((status&0x40)!=0x40) {
			if(status&0x01==0x01) 
			{
				AfxMessageBox("运动完成，未检测到负限位信号",MB_OK);
				return 0;
			}			
			Sleep(20);
			rtn=GT_Axis(nAxis);
			rtn=GT_GetSts(&status);	
			Sleep(20);
		}	
		rtn=GT_Axis(nAxis);
        rtn=GT_GetAtlPos(&lNegLimPos);
        lZeroPos=(long) abs(lPosLimPos-lNegLimPos)/2;
		rtn=GT_ClrSts();
		Sleep(20);
	//	rtn=GT_SetPos(lNegLimPos+lZeroPos);	
		rtn=GT_Axis(nAxis);
		rtn=GT_SetPos(60000);
		Sleep(20);
		rtn=GT_Update(); 		
	
		//检测回零是否完成//
		rtn=GT_Axis(nAxis);
		rtn=GT_GetSts(&status);	
		while((status&0x01)!=0x01){
			Sleep(20);
			rtn=GT_Axis(nAxis);
			rtn=GT_GetSts(&status);
			Sleep(20);
		}	
		AfxMessageBox("回零完成！",MB_OK);
		rtn=GT_ZeroPos();

  ExitThread(1);
  return 1;
}

CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugDlg)
	//}}AFX_DATA_INIT
	m_bServOn=false;
	m_bCardInit=false;

}


void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugDlg)
	DDX_Control(pDX, IDC_GOOGOL_AVI, m_ctrlAVI);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	//{{AFX_MSG_MAP(CDebugDlg)
	ON_BN_CLICKED(IDC_BTN_MANUAL_XPLUS, OnBtnManualXplus)
	ON_BN_CLICKED(IDC_BTN_MANUAL_XMINUS, OnBtnManualXminus)
	ON_BN_CLICKED(IDC_BTN_MANUAL_SERVON, OnBtnManualServon)
	ON_BN_CLICKED(IDC_BTN_MANUAL_YPLUS, OnBtnManualYplus)
	ON_BN_CLICKED(IDC_BTN_MANUAL_YMINUS, OnBtnManualYminus)
	ON_BN_CLICKED(IDC_BTN_MANUAL_ZMINUS, OnBtnManualZminus)
	ON_BN_CLICKED(IDC_BTN_MANUAL_AMINUS, OnBtnManualAminus)
	ON_BN_CLICKED(IDC_BTN_MANUAL_ZPLUS, OnBtnManualZplus)
	ON_BN_CLICKED(IDC_BTN_MANUAL_APLUS, OnBtnManualAplus)
	ON_BN_CLICKED(IDC_BTN_MANUAL_HOME, OnBtnManualHome)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OUT_BIT0, OnOutBit0)
	ON_BN_CLICKED(IDC_OUT_BIT1, OnOutBit1)
	ON_BN_CLICKED(IDC_OUT_BIT2, OnOutBit2)
	ON_BN_CLICKED(IDC_OUT_BIT3, OnOutBit3)
	ON_BN_CLICKED(IDC_OUT_BIT4, OnOutBit4)
	ON_BN_CLICKED(IDC_OUT_BIT5, OnOutBit5)
	ON_BN_CLICKED(IDC_OUT_BIT6, OnOutBit6)
	ON_BN_CLICKED(IDC_OUT_BIT7, OnOutBit7)
	ON_BN_CLICKED(IDC_OUT_BIT8, OnOutBit8)
	ON_BN_CLICKED(IDC_OUT_BIT9, OnOutBit9)
	ON_BN_CLICKED(IDC_OUT_BIT10, OnOutBit10)
	ON_BN_CLICKED(IDC_OUT_BIT11, OnOutBit11)
	ON_BN_CLICKED(IDC_OUT_BIT12, OnOutBit12)
	ON_BN_CLICKED(IDC_OUT_BIT13, OnOutBit13)
	ON_BN_CLICKED(IDC_OUT_BIT14, OnOutBit14)
	ON_BN_CLICKED(IDC_OUT_BIT15, OnOutBit15)
	ON_BN_CLICKED(IDC_BTN_MANUAL_SERVOFF, OnBtnManualServoff)
	ON_BN_CLICKED(IDC_BTN_MANUAL_RESET, OnBtnManualReset)
	ON_BN_CLICKED(IDC_BTN_MANUAL_CARDINIT, OnBtnManualCardinit)
	ON_BN_CLICKED(IDC_BTN_AXIS1_HOME, OnBtnAxis1Home)
	ON_BN_CLICKED(IDC_BTN_AXIS2_HOME, OnBtnAxisHome)
	ON_BN_CLICKED(IDC_BTN_AXIS3_HOME, OnBtnAxis3Home)
	ON_BN_CLICKED(IDC_BTN_AXIS4_HOME, OnBtnAxis4Home)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg message handlers

BOOL CDebugDlg::OnInitDialog() 
{
	int i=1;
	CDialog::OnInitDialog();
    int nAxisCount;

//	m_ctrlAVI.Open(IDR_AVI_GOOGOL);
//	m_ctrlAVI.Play(0,-1,-1);

	m_bCardInit=((CMainFrame*)GetParent())->m_bCardInit;
	CMotorControlBenchApp* pApp=(CMotorControlBenchApp*)AfxGetApp();
	nAxisCount=pApp->m_nAxisCount;
   

	long IDC_JOG_PLUS[4]={IDC_BTN_MANUAL_XPLUS,IDC_BTN_MANUAL_YPLUS,
		                 IDC_BTN_MANUAL_ZPLUS,IDC_BTN_MANUAL_APLUS};
	long IDC_JOG_MINUS[4]={IDC_BTN_MANUAL_XMINUS,IDC_BTN_MANUAL_YMINUS,
	                  	IDC_BTN_MANUAL_ZMINUS,IDC_BTN_MANUAL_AMINUS};
	long IDC_Home[4]={IDC_BTN_AXIS1_HOME,IDC_BTN_AXIS2_HOME,IDC_BTN_AXIS3_HOME,
		              IDC_BTN_AXIS4_HOME};
	
	CString strAxisName[2]={"X轴","Y轴"};    

	for(i=1;i<=4-nAxisCount;i++)
	{
		GetDlgItem(IDC_JOG_PLUS[4-i])->EnableWindow(false);
		GetDlgItem(IDC_JOG_MINUS[4-i])->EnableWindow(false);
		GetDlgItem(IDC_Home[4-i])->EnableWindow(false);
	}
    CString str;
	for(i=1;i<=nAxisCount;i++)
	{
		str.Format("%d轴+",i);
        GetDlgItem(IDC_JOG_PLUS[i-1])->SetWindowText(str);
		str.Format("%d轴-",i);
		GetDlgItem(IDC_JOG_MINUS[i-1])->SetWindowText(str);
		if (nAxisCount==2) {
			strAxisName[0]="X轴+";
			strAxisName[1]="Y轴+";
			GetDlgItem(IDC_JOG_PLUS[i-1])->SetWindowText(strAxisName[i-1]);
			strAxisName[0]="X轴-";
			strAxisName[1]="Y轴-";		
			GetDlgItem(IDC_JOG_MINUS[i-1])->SetWindowText(strAxisName[i-1]);
			
		}
	}

		if (nAxisCount==2) 
		{
			GetDlgItem(IDC_AXIS1_DEBUG)->SetWindowText("X    轴:");
			GetDlgItem(IDC_AXIS2_DEBUG)->SetWindowText("Y    轴:");
			GetDlgItem(IDC_AXIS3_DEBUG)->EnableWindow(FALSE);
			GetDlgItem(IDC_AXIS4_DEBUG)->EnableWindow(FALSE);
			GetDlgItem(IDC_AXIS5_DEBUG)->SetWindowText("X轴光栅:");
			GetDlgItem(IDC_AXIS6_DEBUG)->SetWindowText("Y轴光栅:");
			
		}

	((CComboBox *) GetDlgItem(IDC_CMB_INC_VALUE))->SetCurSel(4);
	((CComboBox *) 	GetDlgItem(IDC_CMB_VEL_VALUE))->SetCurSel(1);
	CButtonST * btnXMinus=(CButtonST *) GetDlgItem(IDC_BTN_MANUAL_XMINUS);

	SetTimer(1,200,NULL);
	return TRUE;  
}

void CDebugDlg::OnBtnManualXplus() 
{
	CString str,strVel;
	double dist;
	double vel;
	long lPosLimPos,lNegLimPos,lZeroPos;
       CString strInfo;
	long pos=0;

	if(m_bServOn==false)
	{
		MessageBox("轴尚未开启！请先点击“轴开启”按钮","",MB_ICONWARNING|MB_OK);
		return;
	}

	GetDlgItem(IDC_CMB_INC_VALUE)->GetWindowText(str);
	GetDlgItem(IDC_CMB_VEL_VALUE)->GetWindowText(strVel);
	dist=atof(str);
	vel=atof(strVel);

	gtcard.Jog(AXIS_1,dist,vel,POSITIVE_DIRCT);

#if 0
       CString strInfo;
	double pnt[4];

        strInfo.Format("wait");
        MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

    	gtcard.GetPrflPnt(pnt);	//确定轴当前点位置 
	strInfo.Format("position:%f, %f, %f", pnt[0],pnt[1],pnt[2]);
       MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
#endif

	while(gtcard.IsCardMove())
	{
            Sleep(100);
	}


    //  GT_Axis(0);
    //  GT_GetAtlPos(&lNegLimPos);
    //  strInfo.Format("position:%d", lNegLimPos);
    //  MessageBox(strInfo,"ERROR",MB_OK|MB_ICONERROR);

}

void CDebugDlg::OnBtnManualXminus() 
{
	CString str,strVel;
	double dist,vel;

	if(m_bServOn==false)
	{
		MessageBox("轴尚未开启！请先点击“轴开启”按钮","",MB_ICONWARNING|MB_OK);
		return;
	}

	GetDlgItem(IDC_CMB_INC_VALUE)->GetWindowText(str);
	GetDlgItem(IDC_CMB_VEL_VALUE)->GetWindowText(strVel);
	dist=atof(str);
	vel=atof(strVel);
	gtcard.Jog(AXIS_1,dist,vel,NEGTIVE_DIRCT);


}

void CDebugDlg::OnBtnManualServon() 
{
	short rtn;
	bool  bInit;
	bInit=((CMainFrame*)GetParent())->m_bCardInit|m_bCardInit;

	if (m_bCardInit) {		
		for (int i=0;i<4;i++)
		{
			rtn=GT_Axis(i);
			rtn=GT_AxisOn();
		}
		m_bServOn=true;
	}
	else
	{
		MessageBox("板卡尚未开启,请先点击“卡初始化”按钮","XX科技",MB_ICONWARNING|MB_OK);
		return;
	}
}

void CDebugDlg::OnBtnManualYplus() 
{
	CString str,strVel;
	double dist,vel;

	if(m_bServOn==false)
	{
		MessageBox("轴尚未开启！请先点击“轴开启”按钮","",MB_ICONWARNING|MB_OK);
		return;
	}
	GetDlgItem(IDC_CMB_INC_VALUE)->GetWindowText(str);
	GetDlgItem(IDC_CMB_VEL_VALUE)->GetWindowText(strVel);
	dist=atof(str);
	vel=atof(strVel);
	gtcard.Jog(AXIS_2,dist,vel,POSITIVE_DIRCT);
	
}

void CDebugDlg::OnBtnManualYminus() 
{
	CString str,strVel;
	double dist,vel;

	if(m_bServOn==false)
	{
		MessageBox("轴尚未开启！请先点击“轴开启”按钮","",MB_ICONWARNING|MB_OK);
		return;
	}
	GetDlgItem(IDC_CMB_INC_VALUE)->GetWindowText(str);
	GetDlgItem(IDC_CMB_VEL_VALUE)->GetWindowText(strVel);
	dist=atof(str);
	vel=atof(strVel);
	gtcard.Jog(AXIS_2,dist,vel,NEGTIVE_DIRCT);
}

void CDebugDlg::OnBtnManualZminus() 
{
	CString str,strVel;
	double dist,vel;

	if(m_bServOn==false)
	{
		MessageBox("轴尚未开启！请先点击“轴开启”按钮","",MB_ICONWARNING|MB_OK);
		return;
	}
	GetDlgItem(IDC_CMB_INC_VALUE)->GetWindowText(str);
	GetDlgItem(IDC_CMB_VEL_VALUE)->GetWindowText(strVel);
	dist=atof(str);
	vel=atof(strVel);
	gtcard.Jog(AXIS_3,dist,vel,NEGTIVE_DIRCT);

}

void CDebugDlg::OnBtnManualAminus() 
{
	CString str,strVel;
	double dist,vel;
   
	if(m_bServOn==false)
	{
		MessageBox("轴尚未开启！请先点击“轴开启”按钮","",MB_ICONWARNING|MB_OK);
		return;
	}
	GetDlgItem(IDC_CMB_INC_VALUE)->GetWindowText(str);
	GetDlgItem(IDC_CMB_VEL_VALUE)->GetWindowText(strVel);
	dist=atof(str);
	vel=atof(strVel);
	gtcard.Jog(AXIS_4,dist,vel,NEGTIVE_DIRCT);
}

void CDebugDlg::OnBtnManualZplus() 
{
	CString str,strVel;
	double dist,vel;

	if(m_bServOn==false)
	{
		MessageBox("轴尚未开启！请先点击“轴开启”按钮","",MB_ICONWARNING|MB_OK);
		return;
	}
	GetDlgItem(IDC_CMB_INC_VALUE)->GetWindowText(str);
	GetDlgItem(IDC_CMB_VEL_VALUE)->GetWindowText(strVel);
	dist=atof(str);
	vel=atof(strVel);
	gtcard.Jog(AXIS_3,dist,vel,POSITIVE_DIRCT);
}

void CDebugDlg::OnBtnManualAplus() 
{
	CString str,strVel;
	double dist,vel;

	if(m_bServOn==false)
	{
		MessageBox("轴尚未开启！请先点击“轴开启”按钮","",MB_ICONWARNING|MB_OK);
		return;
	}
	GetDlgItem(IDC_CMB_INC_VALUE)->GetWindowText(str);
	GetDlgItem(IDC_CMB_VEL_VALUE)->GetWindowText(strVel);
	dist=atof(str);
	vel=atof(strVel);
	gtcard.Jog(AXIS_4,dist,vel,POSITIVE_DIRCT);
}

void CDebugDlg::OnBtnManualHome() 
{

}

void CDebugDlg::OnTimer(UINT nIDEvent) 
{
	bool bIsOn;
	HICON hIcon;
	UINT  i;
	for(i=IDC_LED_IN0;i<=IDC_LED_IN15;i++)
	{
		bIsOn=gtcard.GetInptSts(i-IDC_LED_IN0);
			if (bIsOn) 
				hIcon=AfxGetApp()->LoadIcon(IDI_LEDON);			
			else
				hIcon=AfxGetApp()->LoadIcon(IDI_LEDOFF);
			((CStatic *)GetDlgItem(i))->SetIcon(hIcon);			
	}
	double actlPos[4];
	gtcard.GetEncPos(actlPos);
	UpdateLed(IDC_DEBUG_DISP_X,actlPos[0]);
	UpdateLed(IDC_DEBUG_DISP_Y,actlPos[1]);
	UpdateLed(IDC_DEBUG_DISP_Z,actlPos[2]);
	UpdateLed(IDC_DEBUG_DISP_A,actlPos[3]);

	//光栅尺读数//
	long actlpos;
	short rtn;
	rtn=GT_EncPos(1,&actlpos);
    UpdateLed(IDC_DEBUG_DISP_AXIS5,actlpos/1000);
    rtn=GT_EncPos(2,&actlpos);
    UpdateLed(IDC_DEBUG_DISP_AXIS6,-actlpos/1000);
	
	//显示限位信号//
	ShowAxisSignal();

	CDialog::OnTimer(nIDEvent);
}

void CDebugDlg::OnOutBit0() 
{
	OutPutSignal(IDC_OUT_BIT0,0);
}

void CDebugDlg::OnOutBit1() 
{
	OutPutSignal(IDC_OUT_BIT1,1);	
}

void CDebugDlg::OnOutBit2() 
{
	OutPutSignal(IDC_OUT_BIT2,2);
}

void CDebugDlg::OnOutBit3() 
{
	OutPutSignal(IDC_OUT_BIT3,3);
	
}

void CDebugDlg::OnOutBit4() 
{
	OutPutSignal(IDC_OUT_BIT4,4);
	
}

void CDebugDlg::OnOutBit5() 
{
	OutPutSignal(IDC_OUT_BIT5,5);
}

void CDebugDlg::OnOutBit6() 
{
	OutPutSignal(IDC_OUT_BIT6,6);	
}

void CDebugDlg::OnOutBit7() 
{
	OutPutSignal(IDC_OUT_BIT7,7);
	
}

void CDebugDlg::OnOutBit8() 
{
	OutPutSignal(IDC_OUT_BIT8,8);
}

void CDebugDlg::OnOutBit9() 
{
	OutPutSignal(IDC_OUT_BIT9,9);
	
}

void CDebugDlg::OnOutBit10() 
{
  OutPutSignal(IDC_OUT_BIT10,10);
	
}

void CDebugDlg::OnOutBit11() 
{
  OutPutSignal(IDC_OUT_BIT11,11);
	
}

void CDebugDlg::OnOutBit12() 
{
	OutPutSignal(IDC_OUT_BIT12,12);
}

void CDebugDlg::OnOutBit13() 
{
	OutPutSignal(IDC_OUT_BIT13,13);
}

void CDebugDlg::OnOutBit14() 
{
	OutPutSignal(IDC_OUT_BIT14,14);
}

void CDebugDlg::OnOutBit15() 
{
	OutPutSignal(IDC_OUT_BIT15,15);
}

/****************************************************************************/
/*  函数名: OutPutSignal      												*/
/*  功  能：根据界面中输出信号checkbo控件状态操作相应输出口位				*/
/*  参  数： 																*/
/*  		1.int nID  对应的checkbox控件ID									*/
/*			2.int nBit  进行操作的输出口位									*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CDebugDlg::OutPutSignal(int nID, int nBit)
{
	int sts=((CButton *) GetDlgItem(nID))->GetCheck();
	if(sts)
	{
		gtcard.OpenPort(nBit,true);
	}
	else
	{
		gtcard.OpenPort(nBit,false);
	}

}

/****************************************************************************/
/*  函数名: UpdateLed      													*/
/*  功  能：在位置显示LED控件中显示数值										*/
/*  参  数： 																*/
/*  		1.UINT nID  对应的LED控件ID号									*/
/*			2.double dispValue  显示的数值									*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CDebugDlg::UpdateLed(UINT nID, double dispValue)
{
	CDigitalDisp * pLed=(CDigitalDisp *) GetDlgItem(nID);	
	pLed->SetNumToDisp(dispValue);
}

void CDebugDlg::OnBtnManualServoff() 
{
	for(int i=0;i<4;i++)
	{
		GT_Axis(i);
		GT_AxisOff();
	}
	m_bServOn=false;
	MessageBox("各轴伺服已关闭！","XX科技",MB_ICONINFORMATION);
	
}

void CDebugDlg::OnBtnManualReset() 
{
	if(gtcard.IsCardMove())
	{
		MessageBox("有轴在运动，请等轴运动停止后再复位","XX科技",MB_ICONWARNING|MB_OK);
	    return;
	}
	for(int i=0;i<4;i++)
	{
		GT_Axis(i);
		GT_AxisOff();
	}

	GT_Reset();
	m_bCardInit=false;
}

void CDebugDlg::OnBtnManualCardinit() 
{
	InitCard();
	MessageBox("板卡已初始化","XX科技",MB_ICONINFORMATION);
}

/****************************************************************************/
/*  函数名: InitCard	      												*/
/*  功  能：板卡及轴初始化设置												*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CDebugDlg::InitCard()
{
   	gtcard.InitCard(); 
	m_bCardInit=true;
	
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

/****************************************************************************/
/*  函数名: ShowAxisSignal      											*/
/*  功  能：显示各轴专用信号状态											*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明：IDB_GREEN（绿色）对应无信号，IDB_RED(红色）对应有信号			*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CDebugDlg::ShowAxisSignal()
{
	CRect rect;
	UINT res;
	CWnd *pWnd;
	CBitmap pBitmap;
	unsigned short sts;
	long IDC[16]={IDC_AXIS1_POS,IDC_AXIS1_NEG,IDC_AXIS1_HOME,IDC_AXIS1_ALARM,
		IDC_AXIS2_POS,IDC_AXIS2_NEG,IDC_AXIS2_HOME,IDC_AXIS2_ALARM,
		IDC_AXIS3_POS,IDC_AXIS3_NEG,IDC_AXIS3_HOME,IDC_AXIS3_ALARM,
		IDC_AXIS4_POS,IDC_AXIS4_NEG,IDC_AXIS4_HOME,IDC_AXIS4_ALARM};
				
	for (int nAxis=0;nAxis<=3;nAxis++)
	{
		
		for(int nSignalType=0;nSignalType<=3;nSignalType++)
		{
			CDC *pDC;
			CDC SourceDC;
			pWnd=GetDlgItem(IDC[nAxis*4+nSignalType]);
			GT_Axis(nAxis+1);
			GT_ClrSts();
			Sleep(10);
			GT_GetSts(&sts);
			
			switch(nSignalType)
			{
			case 0:  //正限位检查					 
				sts=sts&0x20;
				sts==0x20 ?res=IDB_RED:res=IDB_GREEN;
				break;
			case 1:	//正限位检查			     
				sts=sts&0x40;
				sts==0x40 ?res=IDB_RED:res=IDB_GREEN;				
				break;
			case 2:	//正限位检查			
				sts=sts&0x08;
				sts==0x08 ?res=IDB_RED:res=IDB_GREEN;
				break;
			case 3:	//正限位检查			
				sts=sts&0x02;
				sts==0x02 ?res=IDB_RED:res=IDB_GREEN;
				break;
			}			
			pBitmap.LoadBitmap(res);
			pWnd->GetClientRect(&rect);
			pDC=pWnd->GetDC();
			pDC->SetBkMode(TRANSPARENT);	
			//将当前位图放入兼容CDC
			SourceDC.CreateCompatibleDC(NULL);//生成兼容的CDC
			CBitmap* pOldBitmap = SourceDC.SelectObject(&pBitmap);//选择位图
			pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
				&SourceDC, 0, 0, SRCCOPY);//BitBlt
			SourceDC.SelectObject(pOldBitmap);//删除CDC中所选择的位图
			pBitmap.DeleteObject();//删除位图
			ReleaseDC(pDC);
		}
	}

}

void CDebugDlg::OnOK() 
{
//	m_ctrlAVI.Stop();
//	m_ctrlAVI.Close();
	CDialog::OnOK();
}

void CDebugDlg::OnBtnAxis1Home() 
{
	if (!m_bServOn) {
		MessageBox("请先点击“开启轴”按钮。","XX科技",MB_ICONINFORMATION);
		return;
	}
/*	LPVOID lpParam;
	unsigned long ThreadID;
	CreateThread(NULL,0,HomeThread,lpParam,0,&ThreadID);*/
	gtcard.Home(1,HOME_TWOLIMITS);
}

void CDebugDlg::OnBtnAxisHome() 
{
	if (!m_bServOn) {
		MessageBox("请先点击“开启轴”按钮。","XX科技",MB_ICONINFORMATION);
		return;
	}
	gtcard.Home(2,HOME_TWOLIMITS);
}

void CDebugDlg::OnBtnAxis3Home() 
{
	if (!m_bServOn) {
		MessageBox("请先点击“开启轴”按钮。","XX科技",MB_ICONINFORMATION);
		return;
	}
    gtcard.Home(3,HOME_TWOLIMITS);	
}

void CDebugDlg::OnBtnAxis4Home() 
{
	if (!m_bServOn) {
		MessageBox("请先点击“开启轴”按钮。","XX科技",MB_ICONINFORMATION);
		return;
	}
	gtcard.Home(4,HOME_TWOLIMITS);	
}


