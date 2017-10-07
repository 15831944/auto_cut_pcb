// ACMotorCtrlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MotorControlBench.h"
#include "ACMotorCtrlWnd.h"
#include "GTCard.h"
#include "MotorControlBenchDoc.h"
#include "CcdSelTabCtrl.h"
#include "PageMark.h"

#include "Global.h"
#include "GT400.h"

#include "Pegrpapi.h"


#include "cv.h"
#include "highgui.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	INTERVAL_VALUE 50
#define BUFFER_SIZE 20    //存放运动数据的板卡缓冲区大小
#define SAMPLE_INTERVAL 49  //采集数据时间间隔 

#if 1
CWnd *pwnd;                         //绐椾綋鍩虹被
CDC  *pDC;   
CRect lRect3;   
CStatic *pStaic;

long        Smooth_Step=5;     //滑块1的平滑滤波微调参数值，默认为3
IplImage*   src=0;             //opencv IplImage类图像指针用来存放和处理截取到的图像
CRect lRect;   
HDC Disp_hDC1 ;                 //璁惧鍙ユ焺DC 

#define INTERVAL_VALUE	5  //定义显示刷新时间间隔

#endif


//全局变量//
extern CGTCard gtcard;
CArray<double,double>  glVelArray;
double  atlPos[BUFFER_SIZE], atlVel[BUFFER_SIZE];

//绘图控件结构体
struct CHARTPARA {
	HWND  hWnd;  //绘图控件窗口句柄
	UINT nID;    //绘图控件ID号
	RECT  ChartRect;//绘图控件窗口矩形
	char * chTitle;//绘图控件标题
	char * XLabel; //X轴标题
	char *  YLabel;	//Y轴标题
	int nLineCount; //绘图控件中的曲线数
	long nPointCount;//显示点数
	double fXMax,fXMin;//X坐标最大值和最小值
	double fYMax,fYMin;//Y坐标最大值和最小值
};



//IMPLEMENT_DYNCREATE(CACMotorCtrlWnd, CFormView)
DWORD WINAPI DrawThread(LPVOID pParam)
{	
	//采集电机运行时数据
/*	do{
		rtn = GT_GetWatch(&count);
		this->m_pFr->m_pXPro->m_count += count;
		if(count != 0)
		{   
			for(int i = 0;i < count;++i)
			{
				this->m_pFr->m_pXPro->m_GraphRect1.data[this->m_pFr->m_pXPro->m_count + i] = atlVel[i];
				this->m_pFr->m_pXPro->m_GraphRect3.data[this->m_pFr->m_pXPro->m_count + i] = atlPos[i];
				
				if(i != 0)
				{
					this->m_pFr->m_pXPro->m_GraphRect2.data[this->m_pFr->m_pXPro->m_count + i] =(atlVel[i] - atlVel[i-1]);
				}
				else 
				{
					if(this->m_pFr->m_pXPro->m_count==0)
						this->m_pFr->m_pXPro->m_GraphRect2.data[this->m_pFr->m_pXPro->m_count + i]=atlVel[this->m_pFr->m_pXPro->m_count+i];
					else
						this->m_pFr->m_pXPro->m_GraphRect2.data[this->m_pFr->m_pXPro->m_count +i] =this->m_pFr->m_pXPro->m_GraphRect1.data[this->m_pFr->m_pXPro->m_count + i]-this->m_pFr->m_pXPro->m_GraphRect1.data[this->m_pFr->m_pXPro->m_count + i-1]; 
				}
				TRACE("acc:%lf\n",this->m_pFr->m_pXPro->m_GraphRect2.data[this->m_pFr->m_pXPro->m_count + i]);
			}
			
		}
		
	/*	MSG msg;
		while (::PeekMessage(&msg,this->GetSafeHwnd(),WM_MOUSEFIRST,WM_MOUSELAST,PM_NOREMOVE))
		{					
			if (!AfxGetApp()->PumpMessage()){
				::PostQuitMessage(0);
				AfxMessageBox("PeekMessage error,end application");			
			}
		}	
		
		rtn = GT_GetSts(&status);
	}while(status&0x400);//轴在运动		 
	
				for(i = 0; i < 3; ++i) //采样电机停止时刻的数据
				{
					do {
						rtn = GT_GetWatch(&count);
						this->m_pFr->m_pXPro->m_count += count;
						if(count != 0)
						{		      	 
							for(int j = 0;j < count;++j)
							{
								this->m_pFr->m_pXPro->m_GraphRect1.data[this->m_pFr->m_pXPro->m_count + j] = atlVel[j];
								this->m_pFr->m_pXPro->m_GraphRect3.data[this->m_pFr->m_pXPro->m_count + j] = atlPos[j];
								if(j != 0)
								{
									this->m_pFr->m_pXPro->m_GraphRect2.data[this->m_pFr->m_pXPro->m_count + j] =  (atlVel[j] - atlVel[j-1]);
									
								}
								else 
								{
									this->m_pFr->m_pXPro->m_GraphRect2.data[this->m_pFr->m_pXPro->m_count + j] = (this->m_pFr->m_pXPro->m_GraphRect1.data[this->m_pFr->m_pXPro->m_count+j]-this->m_pFr->m_pXPro->m_GraphRect1.data[this->m_pFr->m_pXPro->m_count+j-1]); 
									
								}
							}
						}
					} while(count == 0);	   
				}
				
				rtn = GT_StopWatch();*/					
				
	short rtn; 
	float fTime;
	float fActualY[3];				//数组,存放轴参数，位置[0],速度[1],加速度[2]
	unsigned short nCountOfBuffer;	//缓冲区大小，为0表示缓冲区空
	unsigned short status; 			//板卡状态	
	int IsReachTargetVel(0);	   //判断速度是否达到目标值，用于速度数据处理	 

	CACMotorCtrlWnd * pWnd=(CACMotorCtrlWnd *) pParam;
	
	//从缓冲区获取轴运动参数			  
	#if 0
	do {	
		rtn=GT_GetWatch(&nCountOfBuffer);						  
		if(nCountOfBuffer != 0)			//缓冲区中有数据
		{   
			for(int i = 0;i < nCountOfBuffer;++i)
			{	
				//获取轴位置和速度数据
				fActualY[0]=(float)atlPos[i];
				fActualY[1]=(float)atlVel[i];
				
				//将速度数据加入到glVelArray，用于计算加速度
				glVelArray.Add(atlVel[pWnd->count]);
				//	TRACE("Pos: No %d %lf\n",i,atlPos[i]);
				
				//速度值处理，由于板卡缓冲区返回的实际速度值可能会有+-1个脉冲的偏差，
				//导致在非加减速阶段加速度计算不正确，故有如下处理
				if(fActualY[1]==pWnd->m_fTargetVel)  IsReachTargetVel=1;		
				if(IsReachTargetVel&&(pWnd->m_nShowDataTYpe!=PROFILE_VALUE)) 
				{
					if((fActualY[1]<=pWnd->m_fTargetVel+1)&&(fActualY[1]>=pWnd->m_fTargetVel-1))
						fActualY[1]=pWnd->m_fTargetVel;
					glVelArray.SetAt(pWnd->count,fActualY[1]);
					//	fActualY[1]=(float)(glVelArray.GetAt(count-2)+glVelArray.GetAt(count-1)+fActualY[1])/3;
				}
				
				//计算时间值
				fTime=(float) pWnd->count*(SAMPLE_INTERVAL+1)*0.0002;
				
				//计算加速度
				if(pWnd->count==0) fActualY[2]=0; //初始时，加速度为0
				else
				{
					fActualY[2]=(float)((glVelArray.GetAt(pWnd->count)-glVelArray.GetAt(pWnd->count-1)))/(SAMPLE_INTERVAL+1);
					if(fActualY[2]==(float)0.0) fActualY[2]=(float)0.00001; //为0时，图形中无法显示曲线，故设为0.00001
				}
				
				//换页显示处理
				if (fTime>=pWnd->m_fEndTimeOfPage) {
					pWnd->m_DrawPageCount++;
					pWnd->m_CurDrawPage++;			
					pWnd->m_fEndTimeOfPage = pWnd->m_fEndTimeOfPage +pWnd->m_fIntervalOfPage;
					pWnd->m_fStartTimeOfPage = pWnd->m_fEndTimeOfPage - pWnd->m_fIntervalOfPage;
					//重新设置坐标刻度
					for(int i=0;i<3;i++)
					{
						PEvset(pWnd->m_hPE[i], PEP_fMANUALMINX, &pWnd->m_fStartTimeOfPage, 1);
						PEvset(pWnd->m_hPE[i], PEP_fMANUALMAXX, &pWnd->m_fEndTimeOfPage, 1);	
					}
				}
				
				//将各数据保存到相应的绘图控件数据存储单元
				//位移
				PEvsetcellEx(pWnd->m_hPE[0], PEP_faYDATA, 0, pWnd->count, &fActualY[0]);
				PEvsetcellEx(pWnd->m_hPE[0], PEP_faXDATA, 0, pWnd->count, &fTime);
				//速度
				PEvsetcellEx(pWnd->m_hPE[1], PEP_faYDATA, 0, pWnd->count, &fActualY[1]);
				PEvsetcellEx(pWnd->m_hPE[1], PEP_faXDATA, 0, pWnd->count, &fTime);	
				//加速度
				PEvsetcellEx(pWnd->m_hPE[2], PEP_faYDATA, 0, pWnd->count, &fActualY[2]);
				PEvsetcellEx(pWnd->m_hPE[2], PEP_faXDATA, 0, pWnd->count, &fTime);	
				//	TRACE("第 %d 次：%f\n",count,fActualY[2]);
				pWnd->count++;				
			}
		}
		pWnd->m_bCanDraw=true;
		rtn = GT_GetSts(&status);
	} while(status&0x400);
	
	for(int j=0;j<5;j++)
	{
		rtn=GT_GetWatch(&nCountOfBuffer);	
		if(nCountOfBuffer != 0)
		{   
			for(int i = 0;i < nCountOfBuffer;++i)
			{				
				fActualY[0]=(float)atlPos[i];
				fActualY[1]=(float)atlVel[i];
				glVelArray.Add(atlVel[i]);	
				fTime=(float) pWnd->count*(SAMPLE_INTERVAL+1)*0.0002;
				if(pWnd->count==0)
					fActualY[2]=0;
				else
				{
					fActualY[2]=(float)((glVelArray.GetAt(pWnd->count)-glVelArray.GetAt(pWnd->count-1)))/(SAMPLE_INTERVAL+1);
					if(fActualY[2]==(float) 0.0) fActualY[2]=(float) 0.00001;
				}
				
				if (fTime>=pWnd->m_fEndTimeOfPage) {
					pWnd->m_DrawPageCount++;
					pWnd->m_CurDrawPage++;
					//	CString str;
					//	str.Format("第%d页\n",m_DrawPageCount);
					//	MessageBox(str,"report",MB_OK);
					//TRACE(str);
					pWnd->m_fEndTimeOfPage = pWnd->m_fEndTimeOfPage +pWnd->m_fIntervalOfPage;
					pWnd->m_fStartTimeOfPage =pWnd->m_fEndTimeOfPage -pWnd->m_fIntervalOfPage;
					for(int i=0;i<3;i++)
					{
						PEvset(pWnd->m_hPE[i], PEP_fMANUALMINX, &pWnd->m_fStartTimeOfPage, 1);
						PEvset(pWnd->m_hPE[i], PEP_fMANUALMAXX, &pWnd->m_fEndTimeOfPage, 1);	
					}
				}
				
				PEvsetcellEx(pWnd->m_hPE[0], PEP_faYDATA, 0, pWnd->count, &fActualY[0]);
				PEvsetcellEx(pWnd->m_hPE[0], PEP_faXDATA, 0, pWnd->count, &fTime);
				PEvsetcellEx(pWnd->m_hPE[1], PEP_faYDATA, 0, pWnd->count, &fActualY[1]);
				PEvsetcellEx(pWnd->m_hPE[1], PEP_faXDATA, 0, pWnd->count, &fTime);	
				PEvsetcellEx(pWnd->m_hPE[2], PEP_faYDATA, 0, pWnd->count, &fActualY[2]);
				PEvsetcellEx(pWnd->m_hPE[2], PEP_faXDATA, 0, pWnd->count, &fTime);	
				//	TRACE("第 %d 次：%f\n",count,fActualY[2]);
				pWnd->count++;				
			}
		}
	}
	rtn = GT_StopWatch();
	pWnd->m_bCanDraw=false;
	for( int i=0;i<3;i++)
	{	
		PEreinitialize(pWnd->m_hPE[i]);
		PEresetimage(pWnd->m_hPE[i], 0, 0);  
		::InvalidateRect(pWnd->m_hPE[i], NULL, FALSE);
	}
	AfxMessageBox("ok");
      #endif
	return 1;
	
}

CACMotorCtrlWnd::CACMotorCtrlWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CACMotorCtrlWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACMotorCtrlWnd)
	m_nAxis = 0;
	m_nKp = 5;
	m_nKi = 0;
	m_nKd = 0;
//	m_nShowDataTYpe = 0;
	//}}AFX_DATA_INIT

	m_bGearModeSet=false;
	m_bAxisOn=false;
	m_bCanDrawWithBuffer=true;
	m_bAxisOn = FALSE;
	
	m_fStartTimeOfPage=0.0f;
	m_fIntervalOfPage=m_fEndTimeOfPage=5.0f;
	m_DrawPageCount=1;
	m_CurDrawPage=1;

      	LB0_ValueX = 0;
	LB0_ValueY = 0;
	LB1_ValueX = 0;
	LB1_ValueY = 0;
      RB0_ValueX = 0;
	RB0_ValueY = 0;
	RB1_ValueX = 0;
	RB1_ValueY = 0;

      m_StepValue = "10mm";
      m_Diameter = 36;
      m_MillingDiameter = 1500;
      
	for(int i=0;i<3;i++)
	{
	    m_hPE[i]=NULL;
	}	

       Side = NO_SIDE;
       PointNum = 0;
       
       for(int i=0; i<MAX_POINT; i++)
       {
           L_Points[i].type = "NULL";
           L_Points[i].PosX = 0;
           L_Points[i].PosY = 0;
           L_Points[i].PosZ = 0;
           R_Points[i].type = "NULL";
           R_Points[i].PosX = 0;
           R_Points[i].PosY = 0;
           R_Points[i].PosZ = 0;
        }

       for(int i=0; i<3; i++)
       {
 //          L_MPoints[i].type = "NULL";
           L_MPoints[i].PosX = 0;
           L_MPoints[i].PosY = 0;
           L_MPoints[i].PosZ = 0;
           L_MPoints[i].Para = 0;

 //          R_Points[i].type = "NULL";
           R_MPoints[i].PosX = 0;
           R_MPoints[i].PosY = 0;
           R_MPoints[i].PosZ = 0;
           R_MPoints[i].Para = 0;
        }    

        memset(szString, 0, 100);
}
       
#if 0
CACMotorCtrlWnd::~CACMotorCtrlWnd()
{
	//析构绘图控件
	for(int i=0;i<3;i++)
	{
		if(m_hPE[i])
		{
			PEdestroy(m_hPE[i]);
		}
	}	
}
#endif

void CACMotorCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACMotorCtrlWnd)
	DDX_Control(pDX, IDC_TAB_AC_MODESEL, m_tabModeSel);
      DDX_Control(pDX, IDC_LIST, m_listCtrl);
      	DDX_Text(pDX, IDC_EDIT_STEP, m_StepValue);
      	DDX_Text(pDX, IDC_EDIT_DIAMETER, m_Diameter);
      DDX_Text(pDX, IDC_EDIT_MILLING_DIAMETER, m_MillingDiameter);
//	DDX_Radio(pDX, IDC_RADIO_AXIS1, m_nAxis);
//	DDX_Text(pDX, IDC_SETKP, m_nKp);
//	DDX_Text(pDX, IDC_SETKI, m_nKi);
//	DDX_Text(pDX, IDC_SETKD, m_nKd);
//	DDX_Radio(pDX, IDC_RADIO_PROFILE, m_nShowDataTYpe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACMotorCtrlWnd, CDialog)
	//{{AFX_MSG_MAP(CACMotorCtrlWnd)
//	ON_BN_CLICKED(IDC_RADIO_AXIS1, OnRadioAxis1)
//	ON_BN_CLICKED(IDC_RADIO_AXIS2, OnRadioAxis2)
//	ON_BN_CLICKED(IDC_RADIO_AXIS3, OnRadioAxis3)
//	ON_BN_CLICKED(IDC_RADIO_AXIS4, OnRadioAxis4)
//	ON_CBN_SELCHANGE(IDC_AC_CTRLMODE, OnSelchangeAcCtrlmode)
	ON_BN_CLICKED(IDC_AC_AXISON, OnAcAxison)
	ON_BN_CLICKED(IDC_AC_AXISOFF, OnAcAxisoff)
	ON_BN_CLICKED(IDC_AC_STOP, OnAcStop)
	ON_BN_CLICKED(IDC_AC_RUN, OnAcRun)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_BTN_PREPAGE, OnBtnPrepage)
	ON_BN_CLICKED(IDC_BTN_SHOWALL, OnBtnShowall)
	ON_BN_CLICKED(IDC_BTN_NEXTPAGE, OnBtnNextpage)
//	ON_EN_UPDATE(IDC_SETKP, OnUpdateSetkp)
//	ON_EN_UPDATE(IDC_SETKI, OnUpdateSetki)
//	ON_EN_UPDATE(IDC_SETKD, OnUpdateSetkd)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CACMotorCtrlWnd::OnNMClickList)
	ON_BN_CLICKED(IDC_BUTTON_XPOSITIVE, &CACMotorCtrlWnd::OnBnClickedButtonXpositive)
	//ON_NOTIFY(BCN_DROPDOWN, IDC_BUTTON_XPOSITIVE, &CACMotorCtrlWnd::OnBnDropDownButtonXpositive)
//	ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDC_BUTTON_XREVERSE, &CACMotorCtrlWnd::OnBnClickedButtonXreverse)
ON_BN_CLICKED(IDC_BUTTON_YPOSITIVE, &CACMotorCtrlWnd::OnBnClickedButtonYpositive)
ON_BN_CLICKED(IDC_BUTTON_YRERVERSE, &CACMotorCtrlWnd::OnBnClickedButtonYrerverse)
ON_BN_CLICKED(IDC_CHECK_LEFT_IN, &CACMotorCtrlWnd::OnBnClickedCheckLeftIn)
ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CACMotorCtrlWnd::OnBnClickedButtonOpenfile)
ON_BN_CLICKED(IDC_BUTTON_NEWFILE, &CACMotorCtrlWnd::OnBnClickedButtonNewfile)
ON_BN_CLICKED(IDC_CHECK_RIGHT_IN, &CACMotorCtrlWnd::OnBnClickedCheckRightIn)
ON_EN_CHANGE(IDC_EDIT_DIAMETER, &CACMotorCtrlWnd::OnEnChangeEditDiameter)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIAMETER, &CACMotorCtrlWnd::OnDeltaposSpinDiameter)
ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CACMotorCtrlWnd::OnBnClickedButtonModify)
ON_BN_CLICKED(IDC_CHECK_LEFT_PROGRAM, &CACMotorCtrlWnd::OnBnClickedCheckLeftProgram)
ON_BN_CLICKED(IDC_CHECK_RIGHT_PROGRAM, &CACMotorCtrlWnd::OnBnClickedCheckRightProgram)
ON_BN_CLICKED(IDC_BUTTON_ZPOSITIVE, &CACMotorCtrlWnd::OnBnClickedButtonZpositive)
ON_BN_CLICKED(IDC_BUTTON_ZRERVERSE, &CACMotorCtrlWnd::OnBnClickedButtonZrerverse)
ON_BN_CLICKED(IDC_CCD_LIGHT_ONOFF, &CACMotorCtrlWnd::OnBnClickedCcdLightOnoff)
ON_BN_CLICKED(IDC_MAIN_AXIC, &CACMotorCtrlWnd::OnBnClickedMainAxic)
ON_BN_CLICKED(IDC_DUST_COLLECTOR, &CACMotorCtrlWnd::OnBnClickedDustCollector)
ON_BN_CLICKED(IDC_LEFT_AIR_DOOR, &CACMotorCtrlWnd::OnBnClickedLeftAirDoor)
ON_BN_CLICKED(IDC_RIGHT_AIR_DOOR, &CACMotorCtrlWnd::OnBnClickedRightAirDoor)
ON_BN_CLICKED(IDC_ZERO_BUTTON, &CACMotorCtrlWnd::OnBnClickedZeroButton)
END_MESSAGE_MAP()

BOOL CACMotorCtrlWnd::OnInitDialog() 
{
	int i=1;
	CDialog::OnInitDialog();

	//初始化板卡
	InitCard();  
      
	//设置页面初始化
	m_tabModeSel.InsertItem(0,"执行");
	m_tabModeSel.InsertItem(1,"基准");
	m_tabModeSel.InsertItem(2,"教导");
	m_tabModeSel.InsertItem(3,"编辑");
	m_tabModeSel.InsertItem(4,"系统");
	m_tabModeSel.InitTabPage();

      // 插入列表控件纵列
    	m_listCtrl.InsertColumn(0, "T0", LVCFMT_LEFT, 100);
    	m_listCtrl.InsertColumn(1, "X(um)", LVCFMT_LEFT, 120);
    	m_listCtrl.InsertColumn(2, "Y(um)", LVCFMT_LEFT, 120);
      m_listCtrl.InsertColumn(3, "Z or R(um)", LVCFMT_LEFT, 120);
    	m_listCtrl.InsertColumn(4, "Spd(mm/sec)", LVCFMT_LEFT, 120);

    	// 设置列表控件扩展样式为整行高亮
    	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);   

	return TRUE;  
}

/////////////////////////////////////////////////////////////////////////////
// CACMotorCtrlWnd diagnostics

#ifdef _DEBUG
void CACMotorCtrlWnd::AssertValid() const
{
	CDialog::AssertValid();
}

void CACMotorCtrlWnd::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CACMotorCtrlWnd message handlers

#if 0
void CACMotorCtrlWnd::OnInitialUpdate() 
{
	RECT LineListRect;
    
	CDialog::OnInitialUpdate();

      #if 0
	//获取系统总轴数
	int nAxisCount;
	CMotorControlBenchApp* pApp=(CMotorControlBenchApp*)AfxGetApp();
	nAxisCount=pApp->m_nAxisCount;

	//初始化绘图区
	SetChartPara();
	
	//初始化板卡
	InitCard();  

	//根据系统总轴数设置界面
	switch(nAxisCount) {
	case 1:
            GetDlgItem(IDC_RADIO_AXIS2)->EnableWindow(FALSE);
            GetDlgItem(IDC_RADIO_AXIS3)->EnableWindow(FALSE);
            GetDlgItem(IDC_RADIO_AXIS4)->EnableWindow(FALSE);
            break;
	case 2:
		GetDlgItem(IDC_RADIO_AXIS3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AXIS4)->EnableWindow(FALSE);
		break;
	case 3:		
		GetDlgItem(IDC_RADIO_AXIS4)->EnableWindow(FALSE);
		break;
	case 4:
		break;
	}
      #endif

	//初始化板卡
	InitCard();  
      
	//设置页面初始化
	m_tabModeSel.InsertItem(0,"执行");
	m_tabModeSel.InsertItem(1,"基准");
	m_tabModeSel.InsertItem(2,"教导");
	m_tabModeSel.InsertItem(3,"编辑");
	m_tabModeSel.InitTabPage();

      // 插入列表控件纵列
    	m_listCtrl.InsertColumn(0, "T0", LVCFMT_LEFT, 100);
    	m_listCtrl.InsertColumn(1, "X(um)", LVCFMT_LEFT, 120);
    	m_listCtrl.InsertColumn(2, "Y(um)", LVCFMT_LEFT, 120);
      m_listCtrl.InsertColumn(3, "Z or R(um)", LVCFMT_LEFT, 120);
    	m_listCtrl.InsertColumn(4, "Spd(mm/sec)", LVCFMT_LEFT, 120);

    	// 设置列表控件扩展样式为整行高亮
    	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);   

#if 0
    LineListRect=CRect(0,530,800,620);	

    m_LineList.Create(LVS_EX_GRIDLINES | LVS_REPORT | LVS_EDITLABELS | WS_CHILD | WS_VISIBLE, LineListRect, this, IDC_LINE_LIST);
    m_LineList.InsertColumn(0, "Column 1", LVCFMT_LEFT, 40, 500);
    m_LineList.InsertItem( 0, "aaa");
#endif
    
    #if 0
	//根据配置文件系统参数设置界面
	CComboBox * pModeSel;
	int nCtrlMode = pApp->m_nCtrlMode[m_nAxis];
	m_nCardType=pApp->m_nCardType;	
	pModeSel=(CComboBox *)GetDlgItem(IDC_AC_CTRLMODE);

    if(SG_CARD==m_nCardType)
	{	
		pModeSel->InsertString(0,"脉冲量");
		pModeSel->SetCurSel(0);
		GetDlgItem(IDC_SETKP)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETKI)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETKD)->EnableWindow(FALSE);		
	}
	else
	{
		pModeSel->InsertString(0,"模拟电压");
		pModeSel->InsertString(1,"脉冲量");
		pModeSel->SetCurSel(nCtrlMode);
		if(nCtrlMode == SERVO_MOTOR)
		{
			GetDlgItem(IDC_SETKP)->EnableWindow(TRUE);
			GetDlgItem(IDC_SETKI)->EnableWindow(TRUE);
			GetDlgItem(IDC_SETKD)->EnableWindow(TRUE);
			m_nKp = ((CMotorControlBenchApp*)AfxGetApp())->m_nKp[m_nAxis];
			m_nKi = ((CMotorControlBenchApp*)AfxGetApp())->m_nKi[m_nAxis];
			m_nKd = ((CMotorControlBenchApp*)AfxGetApp())->m_nKd[m_nAxis];
		}
		else if(nCtrlMode == STEP_MOTOR)
		{
			GetDlgItem(IDC_SETKP)->EnableWindow(FALSE);
			GetDlgItem(IDC_SETKI)->EnableWindow(FALSE);
			GetDlgItem(IDC_SETKD)->EnableWindow(FALSE);			
		} 
	}	
      #endif
      
	UpdateData(FALSE);	
}
#endif

#if 0
/****************************************************************************/
/*  函数名: InitChart       												*/
/*  功  能：初始化曲线绘制控件												*/
/*  参  数： 																*/
/*            1. UINT nWndNo    控件号								    	*/	 
/*            2. CHARTPARA para  控件参数结构体变量							*/	
/*  出  口：																*/ 
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::InitChart(UINT nWndNo,CHARTPARA para)
{
    HWND hPE;
	RECT rect;       //绘图窗口矩形
	UINT nID;        //创建的绘图窗口ID号
	long nPtCount;   //单屏显示点数
	int nLineCount;  //显示曲线数
	int nLineStyle;  //线型
	double fXMin,fXMax;
	double fYMin,fYMax;
	char * strXLabel,* strYLabel;	
	char * strTitle;

	//根据传入的参数初始化绘图控件
	strTitle=para.chTitle;
	nLineStyle=PELT_MEDIUMTHINSOLID;/*PELT_THINSOLID,PELT_MEDIUMSOLID*/
	rect=para.ChartRect;
	nID=para.nID;
	nPtCount=para.nPointCount;
	nLineCount=para.nLineCount;
	fXMin=para.fXMin;
	fXMax=para.fXMax;
	fYMin=para.fYMin;
	fYMax=para.fYMax;
	strXLabel=para.XLabel;
	strYLabel=para.YLabel;

  //销毁已有控件
	if(m_hPE[nWndNo])
	{
		PEdestroy(m_hPE[nWndNo]);
		Invalidate();
	}
	
  // Construct Object ,创建显示窗口//
   hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, nID); 
   
  //set subset's number,设置显示曲线数
   PEnset(hPE, PEP_nSUBSETS, nLineCount);
  
   //set point count,设置单屏显示点数
   PEnset(hPE, PEP_nPOINTS, nPtCount);
 
   // Set Manual Y scale ,设置Y轴手动刻度//   
   PEnset(hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
   PEvset(hPE, PEP_fMANUALMINY, &fYMin, 1);
   PEvset(hPE, PEP_fMANUALMAXY, &fYMax, 1);

   // Set Manual X scale,设置X轴手动刻度 //
   PEnset(hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
   PEvset(hPE, PEP_fMANUALMINX, &fXMin, 1);
   PEvset(hPE, PEP_fMANUALMAXX, &fXMax, 1);
 
   //绘图数据初始化
   float val = 0;
   for (int i=0;i<nLineCount;i++)
   {	  
	   // Clear out default data,清空原有数据 //
	   PEvsetcellEx(hPE, PEP_faXDATA, i, 0, &val);
	   PEvsetcellEx(hPE, PEP_faXDATA, i, 1, &val);
	   PEvsetcellEx(hPE, PEP_faXDATA, i, 2, &val);
	   PEvsetcellEx(hPE, PEP_faXDATA, i, 3, &val);
	   
	   PEvsetcellEx(hPE, PEP_faYDATA, i, 0, &val);
	   PEvsetcellEx(hPE, PEP_faYDATA, i, 1, &val);
	   PEvsetcellEx(hPE, PEP_faYDATA, i, 2, &val);
	   PEvsetcellEx(hPE, PEP_faYDATA, i, 3, &val);

	   //set the line type,设置显示曲线的线型//	   	   
	   PEvsetcell(hPE, PEP_naSUBSETLINETYPES, i, &nLineStyle); 
   } 
 
  //set line title,设置曲线名称//   
   char szSubsets[] = "实际位置曲线\t位置曲线\t";
   PEvset (hPE, PEP_szaSUBSETLABELS, szSubsets, nLineCount);

   // Set Various Other Properties,设置显示参数 //
   PEnset(hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
   PEnset(hPE, PEP_nQUICKSTYLE, PEQS_LIGHT_SHADOW);
   
   PEszset(hPE, PEP_szMAINTITLE, strTitle);   
   PEszset(hPE, PEP_szSUBTITLE, NULL);
   PEszset(hPE, PEP_szXAXISLABEL, strXLabel);
   PEszset(hPE, PEP_szYAXISLABEL, strYLabel);
  
   PEnset(hPE, PEP_bNORANDOMPOINTSTOEXPORT, TRUE);
   PEnset(hPE, PEP_bFOCALRECT, FALSE);
   PEnset(hPE, PEP_bALLOWBAR, FALSE);
   PEnset(hPE, PEP_bALLOWPOPUP, FALSE);
   PEnset(hPE, PEP_bPREPAREIMAGES, TRUE);
   PEnset(hPE, PEP_bCACHEBMP, TRUE);
   PEnset(hPE, PEP_bFIXEDFONTS, TRUE);
  
   //set line's color,设置显示曲线的颜色//
   DWORD col = RGB(255,0,0);
   PEvsetcell(hPE, PEP_dwaSUBSETCOLORS, 0, &col);   
   
   col = RGB(0, 0, 255);
   PEvsetcell(hPE, PEP_dwaSUBSETCOLORS, 1, &col); 
   
   //其他参数设置 
   PEnset(hPE,PEP_dwWDESKCOLOR,col);
   PEnset(hPE, PEP_nGRADIENTBARS, 8);
   PEnset(hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
   PEnset(hPE, PEP_bMAINTITLEBOLD, TRUE);//主标题为粗体
   PEnset(hPE, PEP_bSUBTITLEBOLD, TRUE);//副标题为粗体
   PEnset(hPE, PEP_bLABELBOLD, TRUE);   //坐标名为粗体  
   PEnset(hPE, PEP_nFONTSIZE, PEFS_SMALL);//设置为小字体 
  
 //根据参数重绘
   PEreinitialize(hPE);
   PEresetimage(hPE, 0, 0);  
   m_hPE[nWndNo]=hPE;
}

/****************************************************************************/
/*  函数名: SetChartPara       												*/
/*  功  能：设置各绘图控件参数												*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::SetChartPara()
{
	CHARTPARA chartPara[3];
	RECT rectClient,rectBar,rectChart[3];  
	
	CMainFrame * pMainFrm;
	long rectHeight;
	char * chYLabel[3]={"位移 (pulse)","速度 (pulse/ST)","加速度 (pulse/ST^2)"};
	char * chTitle[3]={"位移－时间曲线图","速度－时间曲线图","加速度－时间曲线图"};	
	
    //获取绘图控件窗口高度值，即视窗的1/3高度
	pMainFrm=(CMainFrame*) AfxGetApp()->m_pMainWnd;    
	pMainFrm->GetClientRect( &rectClient );
	pMainFrm->m_pWndFunctionBar->GetWindowRect(&rectBar);
	rectHeight=(rectClient.bottom-(rectBar.bottom-rectBar.top)-50)/3;	
    
	for(int i=0;i<3;i++)
	{
		//设置各绘图控件参数
		rectChart[i].left =rectClient.left+5;
		rectChart[i].right=(rectClient.right-rectClient.left)*7/10+20;	
		rectChart[i].bottom=(rectClient.bottom-(rectBar.bottom-rectBar.top)-35)*(i+1)/3;
		rectChart[i].top=rectChart[i].bottom-rectHeight;
		
		chartPara[i].chTitle=chTitle[i];
		chartPara[i].YLabel=chYLabel[i];
		chartPara[i].XLabel="时间（秒）";
		chartPara[i].ChartRect=rectChart[i];
		chartPara[i].nID=1001+i;
		
		chartPara[i].fXMin=0.0f;
		chartPara[i].fXMax=m_fIntervalOfPage;
		chartPara[i].fYMin=0.0f;
		chartPara[i].fYMax=20000.0f;
		
		chartPara[i].nLineCount=1;	
		chartPara[i].nPointCount=100*m_fIntervalOfPage*1000/INTERVAL_VALUE;
		
		//初始化控件
		InitChart(i,chartPara[i]);
	}
}
#endif

void CACMotorCtrlWnd::OnRadioAxis1() 
{
	m_nAxis=0;
	UpdateData(FALSE);
	InterfaceHandleWithAxisNO("1轴");
}

void CACMotorCtrlWnd::OnRadioAxis2() 
{
	m_nAxis=1;
	UpdateData(FALSE);
	InterfaceHandleWithAxisNO("2轴");
}

void CACMotorCtrlWnd::OnRadioAxis3() 
{
	m_nAxis=2;
	UpdateData(FALSE);

	InterfaceHandleWithAxisNO("3轴");
}

void CACMotorCtrlWnd::OnRadioAxis4() 
{
	m_nAxis=3;
	UpdateData(FALSE);

	InterfaceHandleWithAxisNO("4轴");
}

void CACMotorCtrlWnd::OnSelchangeAcCtrlmode() 
{
	UpdateData(TRUE);
	#if 0
	int ctrlmode=((CComboBox*)GetDlgItem(IDC_AC_CTRLMODE))->GetCurSel();
	
	if(ctrlmode==0)
	{//模拟电压 
		if (m_nCardType==SG_CARD)
		{
			MessageBox("您使用的是SG卡，不能设置为模拟电压方式","XX科技",MB_ICONINFORMATION);
			return;
		}
		GetDlgItem(IDC_SETKP)->EnableWindow(true);
		GetDlgItem(IDC_SETKI)->EnableWindow(true);
		GetDlgItem(IDC_SETKD)->EnableWindow(true);
		MessageBox("如果您使用的是交流伺服电机\n请确认电机驱动器已设置为速度控制模式","XX科技",MB_ICONINFORMATION);	
	}
	else
	{//脉冲
		GetDlgItem(IDC_SETKP)->EnableWindow(false);
		GetDlgItem(IDC_SETKI)->EnableWindow(false);
		GetDlgItem(IDC_SETKD)->EnableWindow(false);
		MessageBox("如果您使用的是交流伺服电机\n请确认电机驱动器已设置为位置控制模式!","XX科技",MB_ICONINFORMATION);	

	}	
       #endif
}

void CACMotorCtrlWnd::OnAcAxison() 
{
    #if 0
	short rtn;
	unsigned short  nCtrlMode;	//控制模式
	int nPulseMode;   //脉冲方式
	
    UpdateData(TRUE);
	
	nCtrlMode=((CComboBox *) (GetDlgItem(IDC_AC_CTRLMODE)))->GetCurSel();
	nPulseMode = ((CMotorControlBenchApp *)AfxGetApp())->m_nDirPls[m_nAxis];
	
	rtn=GT_Axis(m_nAxis+1);
	rtn=GT_ClrSts();	
	if (m_nCardType==SV_CARD) {	//当前为SV卡
		
		//根据控制方式设置参数
		switch(nCtrlMode)
		{
		case SERVO_MOTOR://模拟电压
			//参数检验
			if(!CheckPara()) return;			
			if (m_nKp>50) 
			{
				if(	IDNO==MessageBox("您设置的Kp大于50，有可能引起电机震颤，确定要继续吗？","PID设置",MB_ICONQUESTION|MB_YESNO))
					return;
			}				
			//设置控制方式和PID	
			rtn=GT_ClrSts();
			Sleep(20);
			rtn = GT_CtrlMode(0);  //Error(rtn,"GT_CtrlMode");			
			rtn = GT_SetKp(m_nKp); // Error(rtn,"SetKp");
			rtn = GT_SetKi(m_nKi); // Error(rtn,"SetKi");
			rtn = GT_SetKd(m_nKd); // Error(rtn,"SetKd");							
			break;
		case STEP_MOTOR: //脉冲量	
			rtn=GT_ClrSts();
			Sleep(20);
			rtn = GT_CtrlMode(1);  // Error(rtn,"GT_CtrlMode");	
			
			//设置脉冲方式
			switch(nPulseMode)
			{
			case PULSE_MODE:
				rtn = GT_StepPulse();      
				Error(rtn,"GT_StepPulse");
				break;
			case DIR_MODE:
				rtn = GT_StepDir();   
				Error(rtn,"GT_StepDir");
				break;
			default:
				AfxMessageBox("请选择正负脉冲或脉冲+方式模式");
				return;
				break;
			}	
			break;
		}			
	}
	else  //当前为SG卡
	{
		switch(nPulseMode)
		{
		case PULSE_MODE:
			rtn = GT_StepPulse();      
			Error(rtn,"GT_StepPulse");
			break;
		case DIR_MODE:
			rtn = GT_StepDir();   
			Error(rtn,"GT_StepDir");
			break;
		default:
			AfxMessageBox("请选择正负脉冲或脉冲+方式模式");
			return;
			break;
		}				
	}
	rtn=GT_Update();
	rtn=GT_AxisOn();	
	
	if(m_bGearModeSet) //电子齿轮方式
	{
		GT_Axis(m_nAuxAxisNo);//开启从动轴
		GT_AxisOn();
	//	GT_Axis(m_nAxis+1);//开启主动轴
	//	GT_AxisOn();
	}	
	m_bAxisOn=true;
	m_bParaIsChanged=false;
	MessageBox("轴已经开启，下一步点击“运行”按钮","XX科技",MB_ICONINFORMATION);
    #endif
}

void CACMotorCtrlWnd::Error(short rtn, char *chCommand)
{
	if(rtn!=0)
	{
		char *err_code=(char*)new(char*);
        char *str=(char*)new(char*);
		itoa(rtn,err_code,10);		
		strcpy(str,chCommand);
		strcat(str,"=");
        strcat(str,err_code);
		CWnd::MessageBox(str,"Error",MB_OK);
	}
}

void CACMotorCtrlWnd::OnAcAxisoff() 
{
	short rtn;
	for(int i=0;i<4;i++)
	{
	  rtn=GT_Axis(i);
	  rtn=GT_AxisOff();
	}
	m_bAxisOn=false;
	MessageBox("轴已关闭","XX科技");	
}

void CACMotorCtrlWnd::OnAcStop() 
{
	short rtn;
	for(int i=0;i<4;i++)
	{
		rtn=GT_Axis(m_nAxis+1);
		rtn=GT_SmthStp();
		rtn=GT_Update();	
	}
	KillTimer(1);
	if (m_nCardType==SV_CARD) {
		rtn = GT_StopWatch();
	}
	m_bAxisOn=false;	
}

#if 1
// returns TRUE if there is a problem such as ERROR_IO_PENDING.
static bool GetBitmapData(HDC hDCDst, SIZE* size, int* channels, void** data )
{
    BITMAP bmp;
    GdiFlush();
    HGDIOBJ h = GetCurrentObject( hDCDst, OBJ_BITMAP );
    if( size )
        size->cx = size->cy = 0;
    if( data )
        *data = 0;

    if (h == NULL)
        return true;
    if (GetObject(h, sizeof(bmp), &bmp) == 0)
        return true;

    if( size )
    {
        size->cx = abs(bmp.bmWidth);
        size->cy = abs(bmp.bmHeight);
    }

    if( channels )
        *channels = bmp.bmBitsPixel/8;

    if( data )
        *data = bmp.bmBits;

    return false;
}


static void  DrawToHDC( HDC hDCDst, const IplImage* arr, CRect lRect) 
{
    int bmp_w , bmp_h;

    SIZE size = { 0, 0 };
    int channels = 0;
    void* dst_ptr = 0;
    const int channels0 = 3;
    int origin = 0;
    CvMat stub, *image;
    bool changed_size = false; // philipg
    CWnd *pwnd;                         //绐椾綋鍩虹被


#if 0
    CDC *pDC = GetDlgItem(ID)->GetDC();
    HDC hDCDst= pDC->GetSafeHdc();
#endif

   // CvRect dst = RectToCvRect( *(RECT *)arr );

    if( CV_IS_IMAGE_HDR( arr ))
        origin = ((IplImage*)arr)->origin;

     image = cvGetMat( arr, &stub );

	bmp_w = image->width;
	bmp_h = image->height;

        #if 0
    // if there is something wrong with these system calls, we cannot display image...
    if (GetBitmapData( hDCDst, &size, &channels, &dst_ptr ))
   {
	AfxMessageBox("GetBitmapData error!");
		return;
   }
    #endif
    
      BYTE tmp[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
	BITMAPINFO *bmi = (BITMAPINFO*)tmp;
	HBITMAP hBmp;
	int i;
	memset(bmi,0,sizeof(BITMAPINFO));
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth = arr->width;
	bmi->bmiHeader.biHeight = arr->height; 
	bmi->bmiHeader.biPlanes = 1;
	bmi->bmiHeader.biBitCount = arr->nChannels * arr->depth;
	bmi->bmiHeader.biCompression = BI_RGB;
	bmi->bmiHeader.biSizeImage = 0;//if biCompression is BI_RGB,this can be 0
	bmi->bmiHeader.biClrImportant =0 ;

	switch(arr->nChannels * arr->depth) 
	{ 
	case 8 : 
		for(i=0 ; i < 256 ; i++){ 
			bmi->bmiColors[i].rgbBlue = i;
			bmi->bmiColors[i].rgbGreen= i;
			bmi->bmiColors[i].rgbRed= i;
		}
		break;
	case 32:
	case 24: 
		((DWORD*) bmi->bmiColors)[0] = 0x00FF0000;
		((DWORD*) bmi->bmiColors)[1] = 0x0000FF00;
		((DWORD*) bmi->bmiColors)[2] = 0x000000FF;
		break; 
	} 
	hBmp = ::CreateDIBSection(NULL,bmi,DIB_RGB_COLORS,NULL,0,0);
	
 
	
	SetDIBits(NULL,hBmp,0,arr->height,arr->imageData,bmi,DIB_RGB_COLORS);
	
	
	HDC menDC;
	//HBITMAP bit;

	menDC=CreateCompatibleDC(hDCDst);

//	bit=CreateCompatibleBitmap(WinDC,400,300);
	SelectObject(menDC,hBmp);
	DeleteObject(hBmp);
    
	SetStretchBltMode(hDCDst, COLORONCOLOR);
     
	StretchBlt(hDCDst,0,0,lRect3.right-lRect3.left, lRect3.bottom-lRect3.top, menDC,0,0,arr->width,arr->height, SRCCOPY);
		
	//BitBlt(hDCDst,0,0,arr->width,arr->height,menDC,0,0,SRCCOPY);

	DeleteDC(menDC);

}
#endif



void CACMotorCtrlWnd::OnAcRun() 
{
	short  rtn;
    
    	double nCNT = 1;
	double cnt1[5] = {nCNT,0,0,0,0};
	double cnt2[5] = {0,nCNT,0,0,0};

        #if 0

	int iAxisNum;			//轴号
	int nCardType;			//板卡类型
	int nCurMode;
	CString str;	
    int i=1;  

	CMotorControlBenchApp * pApp;
	pApp=(CMotorControlBenchApp *)AfxGetApp();
	nCardType=pApp->m_nCardType;
	iAxisNum=m_nAxis+1;
	m_bCanDraw=false;
	
   //运行状态检查
	if(!m_bAxisOn)
	{
		MessageBox("请先开启轴！","XX科技",MB_ICONINFORMATION);
		return;
	} 
	if (m_bParaIsChanged) {
		MessageBox("PID参数发生了变化.\n请先点击“开启轴”按钮载入新的PID参数","",MB_ICONINFORMATION);
	    return;
	}
	for(i=1;i<5;i++)
	{		
		if(gtcard.IsAxisMoving(i))
		{
			MessageBox("有轴在运动，请等运动停止后再运行！","提示",MB_ICONINFORMATION|MB_OK);
			return;
		}
	}  
	 
	//板卡缓冲区数据采集//
	UpdateData(TRUE);  	
	if (nCardType==SV_CARD) {		
		
		if(m_nShowDataTYpe==PROFILE_VALUE) //数据采集类型为规划值
		{
			rtn = GT_AddWatch(m_nAxis+1,AXIS_PROFILE_POS,atlPos);
			if (rtn==-1) {
				MessageBox("您所使用的板卡版本不支持数据缓冲采集功能，无法实现绘图功能！\n如您需要绘图功能，请与XX公司联系！","XX科技",MB_ICONINFORMATION|MB_OK);
			}
			rtn = GT_AddWatch(m_nAxis+1,AXIS_PROFILE_VEL,atlVel);
			if (rtn==-1) {
				MessageBox("您所使用的板卡版本不支持数据缓冲采集功能，无法实现绘图功能！\n如您需要绘图功能，请与XX公司联系！","XX科技",MB_ICONINFORMATION|MB_OK);
			}
			
		}
		else  //数据采集类型为实际值
		{
			rtn = GT_AddWatch(m_nAxis+1,AXIS_ACTUAL_POS,atlPos);
			rtn = GT_AddWatch(m_nAxis+1,AXIS_ACTUAL_VEL,atlVel);
		}
		
		rtn = GT_EndWatch(BUFFER_SIZE);		
		if (rtn==-1)
		{
			m_bCanDrawWithBuffer=false;
			int res=MessageBox("您所使用的板卡版本不支持数据缓冲采集功能，无法实现绘图功能！\n如您需要绘图功能，请与XX公司联系！\n 继续运行请选“YES” ","XX科技",MB_ICONINFORMATION|MB_YESNO);
			if (res==IDNO) return;
		}
	}

	
	//重绘曲线显示区//
	OnBtnClear();

	//各轴位置清零//
    for(i=1;i<5;i++)
	{
		rtn=GT_Axis(i);			
		rtn=GT_ZeroPos();
		Sleep(20);
		rtn=GT_Update();
	}     
 
	//运动参数设置
	nCurMode=m_tabModeSel.GetCurSel();
	switch(nCurMode)
	{	
	case S_MODE:	
      if (!SMotion(iAxisNum)) return;
	   Sleep(10);
	   if (m_bCanDrawWithBuffer) 
	   {
		   rtn = GT_StartWatch(SAMPLE_INTERVAL);
	   }
	//   Sleep(10);
	//   rtn=GT_Update();
	   break;
	case T_MODE:		
	    if (!TMotion(iAxisNum)) return; 
		if (m_bCanDrawWithBuffer)
		{
			rtn = GT_StartWatch(SAMPLE_INTERVAL);
		}
	//	Sleep(10);
	//	rtn=GT_Update();	
		break;
    case V_MODE: 	
		//加速度//
		if (!VelMotion(iAxisNum)) return; 
		if (m_bCanDrawWithBuffer) 
		{
			rtn = GT_StartWatch(SAMPLE_INTERVAL);
		}
	//	Sleep(10);
	//	rtn=GT_Update();		
	   break;
	case G_MODE:	
	   	MessageBox("电子齿轮设置模式下,请切换至其他模式再点击运行键","提示",MB_OK);
		return;
		break;
    }
	
	Invalidate();

	count=0;
	m_DrawPageCount=1;
	m_CurDrawPage=1;
	m_fEndTimeOfPage=5;
	glVelArray.RemoveAll();	

    #endif
/*	LPVOID lpParam;
	unsigned long ThreadID;
	lpParam=(LPVOID) this;
	CreateThread(NULL,0,DrawThread,lpParam,0,&ThreadID);
	*/
       #if 1
      static int  k=0;   //防止重复按键	

	//gtcard.Home(1,HOME_TWOLIMITS);
	//gtcard.Home(2,HOME_TWOLIMITS);
	InitCard();

	for(int i=0;i<4;i++)
	{
		GT_Axis(i);
		GT_AxisOn();
	}
    
       rtn = GT_MapAxis(1,cnt1);   //  Error(rtn,"GT_MapAxis(1)");
       rtn = GT_MapAxis(2,cnt2);   //  Error(rtn,"GT_MapAxis(2)");	
//       rtn = GT_MapAxis(3,cnt2);   //  Error(rtn,"GT_MapAxis(2)");	

   //位置清零	
	for(int i=1;i<5;i++)
	{
		rtn=GT_Axis(i);   //   Error(rtn,"GT_Axis");
		rtn=GT_ZeroPos();   // Error(rtn,"GT_ZeroPos");
	}
	
      if(k==0)
      {  
        m_Video=cvCreateCameraCapture(-1);      //打开摄像头
        if (!m_Video)
        {
            k=0;
            MessageBox("无法打开摄像头");
            return;
        }
        k=1;
    }
    else
    {
        MessageBox("摄像头已打开");
    }
    #endif

    pwnd = GetDlgItem(IDC_SHOW_IMG); 	//鏍规嵁ID鍊艰幏鍙栧璇濇涓瓙绐椾綋鎺т欢瀵硅薄鎸囬拡,杩欓噷鏄寜閽?绐椾綋

    pDC =pwnd->GetDC();                //鍙栧緱瀹㈡埛鍖烘樉绀鸿澶嘾c
    Disp_hDC1= pDC->GetSafeHdc();      //鑾峰彇瀹㈡埛鍖烘寜閽?绐椾綋鐨勮澶囨弿杩癉C缁橠isp_hDC1
    pStaic = (CStatic*)GetDlgItem(IDC_SHOW_IMG);
    pStaic->GetClientRect(&lRect3);    

    stCenter.x = (lRect3.left + lRect3.right)/2;
    stCenter.y = (lRect3.top + lRect3.bottom)/2;

        
    SetTimer(1,100,NULL);
}

void CACMotorCtrlWnd::DrawView(CDC *pDC) 
{
    #if 0
    	 src=cvQueryFrame(m_Video);             //读取摄像头一帧数据
    	cvShowImage("orig", src);
       //DrawToHDC(pDC->GetSafeHdc(), src, lRect) ;   
    #endif
}

void CACMotorCtrlWnd::OnMouseMove2(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    CRect rect_ctr;
    (this->GetDlgItem(IDC_SHOW_IMG))->GetWindowRect(&rect_ctr);//获取Picture控件相对屏幕左上角的坐标，
     //存储到rect_ctr中

    ScreenToClient(rect_ctr);//获取Picture控件相对对话框客户区左上角的坐标
    point.x -=  rect_ctr.left;//point获取的是鼠标相对对话框客户区左上角的坐标，减去rect_ctr.left和
    point.y -=  rect_ctr.top;//rect_ctr.top后，即为鼠标相对Picture控件左上角的坐标

    //获取状态栏上的文字，并将鼠标的坐标显示在状态栏上
    #if 0
    char	szString[100]={0};
    char szText = {0};

    HWND hWnd = ::GetActiveWindow();
    ::GetWindowText(hWnd, szText,200);
    memset(szText+strlen(szText),' ',20);
    #endif
    sprintf(szString, "%s", "");
    
    //如果鼠标位于Picture控件之外，在对话框状态栏上不显示其坐标
    if( point.x>(rect_ctr.right-rect_ctr.left) || point.y>(rect_ctr.bottom-rect_ctr.top )
       || point.x<0 || point.y<0 )
    {
         sprintf(szString, "%s", "");
    }
    else
    {
                sprintf(szString, "walk(%d, %d)", point.x, point.y);
#if 0
        //sprintf(szString,"(%4i,%4i)", point.x, point.y);
        if(point.x > 200 && point.x < 400 && point.y > 150 && point.y < 300)
        {
            sprintf(szString, "walk(%d, %d)", point.x, point.y);
        }

        if(((point.x > 100 && point.x <= 200) || (point.x >= 400 && point.x < 600))
            && ((point.y > 100 && point.y <= 150) || (point.y >= 300 && point.y < 450) ))
        {
             sprintf(szString, "run(%d, %d)", point.x, point.y);
        }

        if(point.x < 100 || point.x > 600 || point.y < 100 && point.y > 450)
        {
            sprintf(szString,"%s", "");
        }
  #endif      
    }
    
    //将鼠标坐标显示到对话框状态栏上
    //	sprintf(szString,"(%4i,%4i)",point.x,point.y);
    #if 0
    strcpy(szText+20,szString);
    ::SetWindowText(hWnd,(LPWSTR)szText);
    CDialog::OnMouseMove(nFlags, point);
    #endif
}

BOOL CACMotorCtrlWnd::PreTranslateMessage(MSG* pMsg)  
{
    CRect rect_ctr;
    CRect rc;
    CPoint ptCursor;
    double s32ToCenterPicX, s32ToCenterPicY;
    unsigned long u32CurR;
    double u32Vel;
    double s32PosX, s32PosY;
    static CvPoint pre_pt = {-1,-1}; 
    CvFont font;  
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);  
    short rtn;
    double nCNT = 1;
    double cnt1[5] = {nCNT,0,0,0,0};
    double cnt2[5] = {0,nCNT,0,0,0};
  long  lActPosX=0,   lActPosY = 0;
     
    GetDlgItem(IDC_SHOW_IMG)->GetClientRect(rect_ctr);
    GetDlgItem(IDC_SHOW_IMG)->ClientToScreen(rect_ctr);
            
    if  (GetDlgItem(IDC_SHOW_IMG)->GetSafeHwnd() == pMsg->hwnd )
    {
        if(pMsg->message == WM_MOUSEMOVE)
        {
            GetCursorPos(&ptCursor);
            GetDlgItem(IDC_SHOW_IMG)->GetWindowRect(&rc);    
            if(rc.PtInRect(ptCursor))
            {
                 s32ToCenterPicX = ptCursor.x-rect_ctr.left - stCenter.x;
                 s32ToCenterPicY = ptCursor.y-rect_ctr.top - stCenter.y;
                 u32CurR = sqrt(s32ToCenterPicX*s32ToCenterPicX + s32ToCenterPicY*s32ToCenterPicY);
                 m_CurX = ptCursor.x-rect_ctr.left;
                 m_CurY = ptCursor.y-rect_ctr.top;
              
                 if(u32CurR < WALK_THREATHROL)
                 {
                    sprintf(szString, "walk(%d, %d)", ptCursor.x-rect_ctr.left, ptCursor.y-rect_ctr.top);
                 }
                 else
                 {
                    sprintf(szString, "run(%d, %d)", ptCursor.x-rect_ctr.left, ptCursor.y-rect_ctr.top);
                 }
            }
            else
            {
                  sprintf(szString, "%s", " ");
            }
            
           //  OnMouseMove2(MK_MBUTTON, pMsg->pt);
            return TRUE;
        }
        if(pMsg->message == WM_LBUTTONDOWN)
        {
            GetCursorPos(&ptCursor);
            GetDlgItem(IDC_SHOW_IMG)->GetWindowRect(&rc);    
            if(rc.PtInRect(ptCursor))
            {
                 CString strInfo;

                // sprintf(szString, "walk(%d, %d)", ptCursor.x-rect_ctr.left, ptCursor.y-rect_ctr.top);
                 s32ToCenterPicX = ptCursor.x-rect_ctr.left - stCenter.x;
                 s32ToCenterPicY = ptCursor.y-rect_ctr.top - stCenter.y;
                 u32CurR = sqrt(s32ToCenterPicX*s32ToCenterPicX + s32ToCenterPicY*s32ToCenterPicY);
                 m_CurX = ptCursor.x-rect_ctr.left;
                 m_CurY = ptCursor.y-rect_ctr.top;
                 if(u32CurR < WALK_THREATHROL)
                 {
                    sprintf(szString, "walk(%d, %d)", m_CurX, m_CurY);
                 }
                 else
                 {
                    sprintf(szString, "run(%d, %d)", m_CurX, m_CurY);
                 }

                 u32Vel = u32CurR*u32CurR*0.001; /* y=x*x */

                 s32PosX = u32Vel  * TIME_INTERVAL * (s32ToCenterPicX/u32CurR) ;
                 s32PosY = u32Vel  * TIME_INTERVAL * (s32ToCenterPicY/u32CurR) ;

	          // GT_Axis(1);
                // GT_SmthStp();
                // GT_Axis(2);
                // GT_SmthStp();	
               rtn=GT_Axis(1);
                rtn=GT_GetAtlPos(&lActPosX);
              rtn=GT_Axis(2);
                rtn=GT_GetAtlPos(&lActPosY);

                
      //          rtn = GT_MapAxis(1, cnt1);   //  Error(rtn,"GT_MapAxis(xmap)");
      //          rtn = GT_MapAxis(2, cnt2);   //  Error(rtn,"GT_MapAxis(ymap)");	

                 GT_SetSynAcc(0.1);    
                 GT_SetSynVel(u32Vel);    


                // gtcard.Jog(AXIS_1,s32PosX,1,POSITIVE_DIRCT);
            	   // strInfo.Format("position:%f, %f, u32Vel=%f", s32PosX, s32PosY, u32Vel);
                // MessageBox(strInfo,"ERROR",MB_OK|MB_ICONERROR);

                 GT_LnXY((double)lActPosX+s32PosX, (double)lActPosY+s32PosY);
                 unsigned short mode;
                 rtn = GT_GetMode(&mode);
            }
            else
            {
                 sprintf(szString, "%s", " ");
            }
           //  OnMouseMove2(MK_MBUTTON, pMsg->pt);
            return TRUE;
        }

    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CACMotorCtrlWnd::FindCircle() 
{   
    static CvPoint pre_pt = {-1,-1}; 
    CvPoint center;
    int radius = 0;
    int i;
    CvFont font;  
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);  

    src=cvQueryFrame(m_Video);             //读取摄像头一帧数据

    #if 0
    IplImage* gray = cvCreateImage( cvGetSize(src), 8, 1 );

    CvMemStorage* storage = cvCreateMemStorage(0);
    cvCvtColor( src, gray, CV_BGR2GRAY );
    //cvNormalize(gray, gray,255, 0, CV_MINMAX);
    cvSmooth( gray, gray, CV_GAUSSIAN, 3, 3 ); // smooth it, otherwise a lot of false circles may be detected 

    IplConvKernel *element=0;
    element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_ELLIPSE,0);

    cvDilate(gray,gray,element,2);
    cvErode(gray,gray,element,2);

    cvReleaseStructuringElement(&element);

    CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, 25, 300, 30 , m_Diameter/2-3, m_Diameter/2+3);    
    
    //cvCvtColor( gray, src, CV_GRAY2BGR );

    for( i = 0; i < circles->total; i++ )
    {
        float* p = (float*)cvGetSeqElem( circles, i );

        //if(p[0] > (src->height/2 - 40) || p[0] > (src->height/2 + 40) 
        //     || p[1] < (src->width/2 - 40) || p[1] > (src->width/2 + 40) )
        {
            cvCircle( src, cvPoint(cvRound(p[0]),cvRound(p[1])), 2, CV_RGB(255,0,0), -1, 8, 0 );
            cvCircle( src, cvPoint(cvRound(p[0]),cvRound(p[1])), cvRound(p[2]), CV_RGB(255,0,0), 1, 8, 0 );
        }
       // cout<<"圆心坐标x= "<<cvRound(p[0])<<endl<<"圆心坐标y= "<<cvRound(p[1])<<endl;
       // cout<<"半径="<<cvRound(p[2])<<endl; 
       #if 0
        center.x = cvRound(p[0]);
        center.y = cvRound(p[1]);
        radius = cvRound(p[2]);
        cvCircle (src, center, radius, CV_RGB(255,0,0), 1, 8, 0 );
      #endif
        
    }
    #endif
    
    CvPoint pt1 = cvPoint(0, src->height/2),     //椤剁偣鍧愭爣rect.x, rect.y杞垚 CvPoint pt1 鏍煎紡锛屽悗闈㈠垝绾垮嚱鏁伴渶瑕佽繖绉嶆牸寮忥紝rect.x, rect.y鍙互鐪嬫垚鏄竴鏉″瑙掔嚎椤剁偣鍧愭爣
    pt2 = cvPoint(src->width, src->height/2); //鍦ㄨ繖閲岀畻鍑虹浉瀵逛簬椤剁偣鍧愭爣鐨勫瑙掔嚎涓婂彟涓�澶村潗鏍囧�?

    cvLine(src, pt1,  pt2, cvScalar(0,0,255), 1);     //鐢讳袱鍧愭爣闂寸殑鐩寸嚎锛屼篃灏辨槸瀵硅绾?

    pt1 = cvPoint(src->width/2, 0);  //椤剁偣鍧愭爣rect.x, rect.y杞垚 CvPoint pt1 鏍煎紡锛屽悗闈㈠垝绾垮嚱鏁伴渶瑕佽繖绉嶆牸寮忥紝rect.x, rect.y鍙互鐪嬫垚鏄竴鏉″瑙掔嚎椤剁偣鍧愭爣
    pt2 = cvPoint(src->width/2, src->height);
    cvLine(src,pt1,  pt2, cvScalar(0,0,255), 1);     //鐢讳袱鍧愭爣闂寸殑鐩寸嚎锛屼篃灏辨槸瀵硅绾?

    center.x = src->width/2;
    center.y = src->height/2;
    radius = m_Diameter/2;
    cvCircle (src, center, radius, CV_RGB(255,0,0), 1, 8, 0 );

    if(m_tabModeSel.GetCurSel() == LABAL_TAB)
    {
        CString str;
        CEdit *pedit;	

        pedit=(CEdit*)((CPageMark*)(m_tabModeSel.m_tabPages[LABAL_TAB]))->m_tabViewSel.m_tabPages[0]->GetDlgItem(IDC_CCD_WIDTH);
	  pedit->GetWindowText(str);
        m_CcdWidth = atol(str);
        pedit=(CEdit*)((CPageMark*)(m_tabModeSel.m_tabPages[LABAL_TAB]))->m_tabViewSel.m_tabPages[0]->GetDlgItem(IDC_CCD_HEIGHT);
	  pedit->GetWindowText(str);
        m_CcdHeight = atol(str);

        pt1 = cvPoint(center.x - m_CcdWidth/2, center.y - m_CcdHeight/2);  //椤剁偣鍧愭爣rect.x, rect.y杞垚 CvPoint pt1 鏍煎紡锛屽悗闈㈠垝绾垮嚱鏁伴渶瑕佽繖绉嶆牸寮忥紝rect.x, rect.y鍙互鐪嬫垚鏄竴鏉″瑙掔嚎椤剁偣鍧愭爣
        pt2 = cvPoint(center.x + m_CcdWidth/2, center.y - m_CcdHeight/2);
        cvLine(src,pt1,  pt2, cvScalar(0,255,0), 1);     //鐢讳袱鍧愭爣闂寸殑鐩寸嚎锛屼篃灏辨槸瀵硅绾?

        pt1 = cvPoint(center.x - m_CcdWidth/2, center.y - m_CcdHeight/2);  //椤剁偣鍧愭爣rect.x, rect.y杞垚 CvPoint pt1 鏍煎紡锛屽悗闈㈠垝绾垮嚱鏁伴渶瑕佽繖绉嶆牸寮忥紝rect.x, rect.y鍙互鐪嬫垚鏄竴鏉″瑙掔嚎椤剁偣鍧愭爣
        pt2 = cvPoint(center.x - m_CcdWidth/2, center.y + m_CcdHeight/2);
        cvLine(src,pt1,  pt2, cvScalar(0,255,0), 1);     //鐢讳袱鍧愭爣闂寸殑鐩寸嚎锛屼篃灏辨槸瀵硅绾?

        pt1 = cvPoint(center.x - m_CcdWidth/2, center.y + m_CcdHeight/2);  //椤剁偣鍧愭爣rect.x, rect.y杞垚 CvPoint pt1 鏍煎紡锛屽悗闈㈠垝绾垮嚱鏁伴渶瑕佽繖绉嶆牸寮忥紝rect.x, rect.y鍙互鐪嬫垚鏄竴鏉″瑙掔嚎椤剁偣鍧愭爣
        pt2 = cvPoint(center.x + m_CcdWidth/2, center.y + m_CcdHeight/2);
        cvLine(src,pt1,  pt2, cvScalar(0,255,0), 1);     //鐢讳袱鍧愭爣闂寸殑鐩寸嚎锛屼篃灏辨槸瀵硅绾?

        pt1 = cvPoint(center.x + m_CcdWidth/2, center.y - m_CcdHeight/2);  //椤剁偣鍧愭爣rect.x, rect.y杞垚 CvPoint pt1 鏍煎紡锛屽悗闈㈠垝绾垮嚱鏁伴渶瑕佽繖绉嶆牸寮忥紝rect.x, rect.y鍙互鐪嬫垚鏄竴鏉″瑙掔嚎椤剁偣鍧愭爣
        pt2 = cvPoint(center.x + m_CcdWidth/2, center.y + m_CcdHeight/2);
        cvLine(src,pt1,  pt2, cvScalar(0,255,0), 1);     //鐢讳袱鍧愭爣闂寸殑鐩寸嚎锛屼篃灏辨槸瀵硅绾?

    }

    if(strcmp(szString, ""))
    {
        pre_pt = cvPoint(m_CurX * (src->width)/(lRect3.right - lRect3.left), m_CurY * (src->height)/(lRect3.bottom - lRect3.top)); 
        cvPutText(src, szString, pre_pt, &font, cvScalar(0,255, 0));      //cvShowImage("test", src);
    }

    cvFlip(src, NULL,0);  
    DrawToHDC(Disp_hDC1, src, lRect3);

    sprintf(szString, "%s", " ");

#if 0
    cvRelease((void**)&src);
#endif

   // cvRelease((void**)&gray);
   // cvRelease((void**)&circles);

   // cvReleaseMemStorage(&storage); 
}


void CACMotorCtrlWnd::OnTimer(UINT nIDEvent) 
{	
/*	if (!m_bCanDraw) return;
	for( int i=0;i<3;i++)
	{	
		PEreinitialize(m_hPE[i]);
		PEresetimage(m_hPE[i], 0, 0);  
		::InvalidateRect(m_hPE[i], NULL, FALSE);
	}*/
     #if 0
	if (m_bCanDrawWithBuffer) 
      {
          DrawChart_SV();
	}
     #endif

     #if 0
    switch(nIDEvent)
    {
        case 2:
            if(IndexInFieldList == 0)
            {
	          CString str;
                LB0_ValueX += 10;
                m_StepValue = LB0_ValueX;
                str.Format("%d", m_StepValue);
                GetDlgItem(IDC_EDIT_STEP)->SetWindowText(str);
                KillTimer(2);
            }
            return;

        default:
            break;
    }
      #endif
      
    	#if 0
       src=cvQueryFrame(m_Video);             //读取摄像头一帧数据

        CvPoint pt1 = cvPoint(0, src->height/2),     //椤剁偣鍧愭爣rect.x, rect.y杞垚 CvPoint pt1 鏍煎紡锛屽悗闈㈠垝绾垮嚱鏁伴渶瑕佽繖绉嶆牸寮忥紝rect.x, rect.y鍙互鐪嬫垚鏄竴鏉″瑙掔嚎椤剁偣鍧愭爣
        pt2 = cvPoint(src->width, src->height/2); //鍦ㄨ繖閲岀畻鍑虹浉瀵逛簬椤剁偣鍧愭爣鐨勫瑙掔嚎涓婂彟涓�澶村潗鏍囧�?

       cvLine(src, pt1,  pt2, cvScalar(0,0,255), 1);     //鐢讳袱鍧愭爣闂寸殑鐩寸嚎锛屼篃灏辨槸瀵硅绾?

       pt1 = cvPoint(src->width/2, 0);  //椤剁偣鍧愭爣rect.x, rect.y杞垚 CvPoint pt1 鏍煎紡锛屽悗闈㈠垝绾垮嚱鏁伴渶瑕佽繖绉嶆牸寮忥紝rect.x, rect.y鍙互鐪嬫垚鏄竴鏉″瑙掔嚎椤剁偣鍧愭爣
        pt2 = cvPoint(src->width/2, src->height);
       cvLine(src,pt1,  pt2, cvScalar(0,0,255), 1);     //鐢讳袱鍧愭爣闂寸殑鐩寸嚎锛屼篃灏辨槸瀵硅绾?

        CvPoint center;
        int radius;
        center.x = src->width/2;
        center.y = src->height/2;
        radius = 30;
        cvCircle (src, center, radius, CV_RGB(255,0,0), 1, 8, 0 );


    	//cvShowImage("orig", src);
	  DrawToHDC(Disp_hDC1, src, lRect3);

    //	CClientDC dc(this); // device context for painting
	//dc.SetMapMode(MM_TEXT);
	//CMemDC  pDC(&dc);  // non flickering painting

     // this->DrawView(pDC);
    #endif

      FindCircle();
    
	CDialog::OnTimer(nIDEvent);
}

/****************************************************************************/
/*  函数名: SetChartScale      												*/
/*  功  能：设置绘图控件各轴刻度											*/
/*  参  数： 																*/
/*         1. UINT nChartNo   控件编号										*/
/*         2. UINT nAxisTitle 轴刻度设置方式  0=X轴手动刻度  1=Y轴手动刻度  */
/*											  2=X轴自动刻度  3=Y轴自动刻度  */
/*         3. double fMin     手动刻度最小值								*/
/*         4. double fMax     手动刻度最大值								*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::SetChartScale(UINT nChartNo,UINT nAxisTitle, double fMin, double fMax)
{
	if(nChartNo>2||nChartNo<0)
		return;

	switch(nAxisTitle) {
	case 0://X轴手动刻度 
		PEnset(m_hPE[nChartNo], PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
		PEvset(m_hPE[nChartNo], PEP_fMANUALMINX, &fMin, 1);
		PEvset(m_hPE[nChartNo], PEP_fMANUALMAXX, &fMax, 1);
		break;
	case 1://Y轴手动刻度 
		PEnset(m_hPE[nChartNo], PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
		PEvset(m_hPE[nChartNo], PEP_fMANUALMINY, &fMin, 1);
		PEvset(m_hPE[nChartNo], PEP_fMANUALMAXY, &fMax, 1);
		break;
	case 2://X轴自动刻度 
		PEnset(m_hPE[nChartNo], PEP_nMANUALSCALECONTROLX, PEMSC_NONE);
		break;
	case 3://Y轴自动刻度
		 PEnset(m_hPE[nChartNo], PEP_nMANUALSCALECONTROLY, PEMSC_NONE);

	}	
	PEreinitialize(m_hPE[nChartNo]);
	PEresetimage(m_hPE[nChartNo], 0, 0);  
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
void CACMotorCtrlWnd::InitCard()
{	
	int i;
	int nCtrlMode[4];
	int nDirPulse[4];
	short nKp[4],nKi[4],nKd[4];	
	
	//初始化板卡
	gtcard.InitCard();
	
	//获取轴参数
	for(i = 0; i < 4; i ++)
	{
		if(((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i] == SERVO_MOTOR) 
		{
			nCtrlMode[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i];
			nKp[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKp[i];
			nKi[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKi[i];
			nKd[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKd[i];
		}
		
		else if(((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i] == STEP_MOTOR)
		{
			nCtrlMode[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i];
			nDirPulse[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nDirPls[i];
		}
	}
   
	//初始化轴
	for(i = 0; i < 4; i ++)
	{
		gtcard.AxisInitial(nCtrlMode[i],nDirPulse[i],nKp[i],nKi[i],nKd[i],i+1);
	}

}



void CACMotorCtrlWnd::OnBtnClear() 
{
	float val = 0;
	#if 0
	SetChartPara();
	m_CurDrawPage=m_DrawPageCount=1;	
    for(int j=0;j<3;j++)
	{
		for (int i=0;i<2;i++)
		{			
			// Clear out default data,清空原有数据 //
			PEvsetcellEx(m_hPE[j], PEP_faXDATA, i, 0, &val);
			PEvsetcellEx(m_hPE[j], PEP_faXDATA, i, 1, &val);
			PEvsetcellEx(m_hPE[j], PEP_faXDATA, i, 2, &val);
			PEvsetcellEx(m_hPE[j], PEP_faXDATA, i, 3, &val);
			
			PEvsetcellEx(m_hPE[j], PEP_faYDATA, i, 0, &val);
			PEvsetcellEx(m_hPE[j], PEP_faYDATA, i, 1, &val);
			PEvsetcellEx(m_hPE[j], PEP_faYDATA, i, 2, &val);
			PEvsetcellEx(m_hPE[j], PEP_faYDATA, i, 3, &val);
		} 
		PEreinitialize(m_hPE[j]);
		PEresetimage(m_hPE[j], 0, 0);  
		::InvalidateRect(m_hPE[j], NULL, FALSE);
	}
    #endif
    
}

void CACMotorCtrlWnd::OnBtnPrepage() 
{
	GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(TRUE);
	m_CurDrawPage--;
	if(m_CurDrawPage>=1)
	{
		for(int i=0;i<3;i++)
		{
			SetChartScale(i,0,(m_CurDrawPage-1)*m_fIntervalOfPage,m_CurDrawPage*m_fIntervalOfPage);
			PEreinitialize(m_hPE[i]);
			PEresetimage(m_hPE[i], 0, 0);  
			::InvalidateRect(m_hPE[i], NULL, FALSE);
		}
	}
	if(m_CurDrawPage<=1)
		GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(FALSE);
}

void CACMotorCtrlWnd::OnBtnShowall() 
{
	for(int i=0;i<3;i++)
	{
		SetChartScale(i,0,0,m_DrawPageCount*m_fIntervalOfPage);
		PEreinitialize(m_hPE[i]);
		PEresetimage(m_hPE[i], 0, 0);  
		::InvalidateRect(m_hPE[i], NULL, FALSE);
	}    
}

void CACMotorCtrlWnd::OnBtnNextpage() 
{
	GetDlgItem(IDC_BTN_PREPAGE)->EnableWindow(TRUE);
	
	if(m_CurDrawPage>=m_DrawPageCount) 
	{
		return;
	}	
	m_CurDrawPage++;
	for(int i=0;i<3;i++)
	{
		SetChartScale(i,0,(m_CurDrawPage-1)*m_fIntervalOfPage,m_CurDrawPage*m_fIntervalOfPage);
		PEreinitialize(m_hPE[i]);
		PEresetimage(m_hPE[i], 0, 0);  
		::InvalidateRect(m_hPE[i], NULL, FALSE);
	}	
	if(m_CurDrawPage==m_DrawPageCount)
	{
		GetDlgItem(IDC_BTN_NEXTPAGE)->EnableWindow(FALSE);
	}
}

void CACMotorCtrlWnd::DrawWhenStop()
{
	int i = 0;
   	float fActualY[3];
	float fTime;
	unsigned short nCountOfBuffer;
	short rtn;
	rtn = GT_GetWatch(&nCountOfBuffer);

		if(nCountOfBuffer != 0)
		{   
			for(i = 0;i < nCountOfBuffer;++i)
			{			
				glVelArray.Add(atlVel[i]);		
				//	TRACE("Pos: No %d %lf\n",i,atlPos[i]);
				fActualY[0]=(float)atlPos[i];
				fActualY[1]=(float)atlVel[i];
				fTime=(float) count*(SAMPLE_INTERVAL+1)*0.0002*1.25;
				if(count==0)
					fActualY[2]=0;
				else
				{
					fActualY[2]=(float)((glVelArray.GetAt(count)-glVelArray.GetAt(count-1)))/(SAMPLE_INTERVAL+1);
					if(fActualY[2]==(float) 0.0) fActualY[2]=(float) 0.00001;
				}
				
				if (fTime>=m_fEndTimeOfPage) {
					m_DrawPageCount++;
					m_CurDrawPage++;
					//	CString str;
					//	str.Format("第%d页\n",m_DrawPageCount);
					//	MessageBox(str,"report",MB_OK);
					//TRACE(str);
					m_fEndTimeOfPage = m_fEndTimeOfPage + m_fIntervalOfPage;
					m_fStartTimeOfPage = m_fEndTimeOfPage - m_fIntervalOfPage;
					for(int i=0;i<3;i++)
					{
						PEvset(m_hPE[i], PEP_fMANUALMINX, &m_fStartTimeOfPage, 1);
						PEvset(m_hPE[i], PEP_fMANUALMAXX, &m_fEndTimeOfPage, 1);	
					}
				}
				
				PEvsetcellEx(m_hPE[0], PEP_faYDATA, 0, count, &fActualY[0]);
				PEvsetcellEx(m_hPE[0], PEP_faXDATA, 0, count, &fTime);
				PEvsetcellEx(m_hPE[1], PEP_faYDATA, 0, count, &fActualY[1]);
				PEvsetcellEx(m_hPE[1], PEP_faXDATA, 0, count, &fTime);	
				PEvsetcellEx(m_hPE[2], PEP_faYDATA, 0, count, &fActualY[2]);
				PEvsetcellEx(m_hPE[2], PEP_faXDATA, 0, count, &fTime);	
				//	TRACE("第 %d 次：%f\n",count,fActualY[2]);
				count++;				
			}
			
			for( i=0;i<3;i++)
			{	
				PEreinitialize(m_hPE[i]);
				PEresetimage(m_hPE[i], 0, 0);  
				::InvalidateRect(m_hPE[i], NULL, FALSE);
			}
		}
}

/****************************************************************************/
/*  函数名: DrawChart_SV      												*/
/*  功  能：绘制实时曲线	  												*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明：1.采用了缓冲区数据保存的机制，GT_GetWatch()						*/
/*			2.当板卡为SV，调用此函数										*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::DrawChart_SV()
{
    #if 0
	short rtn; 
	float fTime;
	float fActualY[3];				//数组,存放轴参数，位置[0],速度[1],加速度[2]
	unsigned short nCountOfBuffer;	//缓冲区大小，为0表示缓冲区空
	unsigned short status; 			//板卡状态
	static int IsNotFirstRun(0);	//判断是否第一次	
	static int IsReachTargetVel(0);	//判断速度是否达到目标值，用于速度数据处理		
	int i = 0;

	//从缓冲区获取轴运动参数
	rtn = GT_GetWatch(&nCountOfBuffer);
	rtn = GT_GetSts(&status);
	
	
	if(nCountOfBuffer != 0)			//缓冲区中有数据
	{   
		for(i = 0;i < nCountOfBuffer;++i)
		{	
			//获取轴位置和速度数据
			fActualY[0]=(float)atlPos[i];
			fActualY[1]=(float)atlVel[i];

			//将速度数据加入到glVelArray，用于计算加速度
			glVelArray.Add(atlVel[i]);
			//	TRACE("Pos: No %d %lf\n",i,atlPos[i]);

			//速度值处理，由于板卡缓冲区返回的实际速度值可能会有+-1个脉冲的偏差，
			//导致在非加减速阶段加速度计算不正确，故有如下处理
			if(fActualY[1]==m_fTargetVel)  IsReachTargetVel=1;		
			if(IsReachTargetVel&&(m_nShowDataTYpe!=PROFILE_VALUE)) 
			{
				if((fActualY[1]<=m_fTargetVel+1)&&(fActualY[1]>=m_fTargetVel-1))
					fActualY[1]=m_fTargetVel;
				glVelArray.SetAt(count,fActualY[1]);
				//	fActualY[1]=(float)(glVelArray.GetAt(count-2)+glVelArray.GetAt(count-1)+fActualY[1])/3;
			}
			
			//计算时间值
			fTime=(float) count*(SAMPLE_INTERVAL+1)*0.0002*1.25;

			//计算加速度
			if(count==0) fActualY[2]=0; //初始时，加速度为0
			else
			{
				fActualY[2]=(float)((glVelArray.GetAt(count)-glVelArray.GetAt(count-1)))/(SAMPLE_INTERVAL+1);
				if(fActualY[2]==(float)0.0) fActualY[2]=(float)0.00001; //为0时，图形中无法显示曲线，故设为0.00001
			}
			
			//换页显示处理
			if (fTime>=m_fEndTimeOfPage) {
				m_DrawPageCount++;
				m_CurDrawPage++;			
				m_fEndTimeOfPage = m_fEndTimeOfPage + m_fIntervalOfPage;
				m_fStartTimeOfPage = m_fEndTimeOfPage - m_fIntervalOfPage;
			 //重新设置坐标刻度
				for(int i=0;i<3;i++)
				{
					PEvset(m_hPE[i], PEP_fMANUALMINX, &m_fStartTimeOfPage, 1);
					PEvset(m_hPE[i], PEP_fMANUALMAXX, &m_fEndTimeOfPage, 1);	
				}
			}
			
			//将各数据保存到相应的绘图控件数据存储单元
			//位移
			PEvsetcellEx(m_hPE[0], PEP_faYDATA, 0, count, &fActualY[0]);
			PEvsetcellEx(m_hPE[0], PEP_faXDATA, 0, count, &fTime);
			//速度
			PEvsetcellEx(m_hPE[1], PEP_faYDATA, 0, count, &fActualY[1]);
			PEvsetcellEx(m_hPE[1], PEP_faXDATA, 0, count, &fTime);	
			//加速度
			PEvsetcellEx(m_hPE[2], PEP_faYDATA, 0, count, &fActualY[2]);
			PEvsetcellEx(m_hPE[2], PEP_faXDATA, 0, count, &fTime);	
			//	TRACE("第 %d 次：%f\n",count,fActualY[2]);
			count++;				
		}
		
		//绘图
		for( i=0;i<3;i++)
		{	
			PEreinitialize(m_hPE[i]);
			PEresetimage(m_hPE[i], 0, 0);  
			::InvalidateRect(m_hPE[i], NULL, FALSE);
		}
		IsNotFirstRun=1;
	}
	
    //轴运动停止处理
	if((status&0x01)&&IsNotFirstRun)
	{
		IsNotFirstRun=0;
		DrawWhenStop();
		rtn = GT_StopWatch();		
		KillTimer(1);
	}
    #endif
}

void CACMotorCtrlWnd::DrawChart_SG()
{
  	unsigned short status; 
	static int IsNotFirstRun(0);
	static int IsReachTargetVel(0);

	double fActualVel;
	long  lActPos;
	
	float fActualY[3];
	float fTime;  
	
	GT_Axis(m_nAxis);
	GT_GetAtlPos(&lActPos);
	GT_GetAtlVel(&fActualVel);
	
	glVelArray.Add(atlVel[count]);	
	fActualY[0]=(float)lActPos;
	fActualY[1]=(float)fActualVel;	

	fTime=(float) count*0.1;
	if(count==0)
		fActualY[2]=0;
	else
	{
		fActualY[2]=(float)((glVelArray.GetAt(count)-glVelArray.GetAt(count-1)))/0.1;
		if(fActualY[2]==(float)0.0) fActualY[2]=(float)0.00001;
	}
	
	if (fTime>=m_fEndTimeOfPage) {
		m_DrawPageCount++;
		m_CurDrawPage++;		
		m_fEndTimeOfPage = m_fEndTimeOfPage + m_fIntervalOfPage;
		m_fStartTimeOfPage = m_fEndTimeOfPage - m_fIntervalOfPage;
		for(int i=0;i<3;i++)
		{
			PEvset(m_hPE[i], PEP_fMANUALMINX, &m_fStartTimeOfPage, 1);
			PEvset(m_hPE[i], PEP_fMANUALMAXX, &m_fEndTimeOfPage, 1);	
		}
	}
	
	PEvsetcellEx(m_hPE[0], PEP_faYDATA, 0, count, &fActualY[0]);
	PEvsetcellEx(m_hPE[0], PEP_faXDATA, 0, count, &fTime);
	PEvsetcellEx(m_hPE[1], PEP_faYDATA, 0, count, &fActualY[1]);
	PEvsetcellEx(m_hPE[1], PEP_faXDATA, 0, count, &fTime);	
	PEvsetcellEx(m_hPE[2], PEP_faYDATA, 0, count, &fActualY[2]);
	PEvsetcellEx(m_hPE[2], PEP_faXDATA, 0, count, &fTime);	

	count++;	
	
	for( int i=0;i<3;i++)
	{	
		PEreinitialize(m_hPE[i]);
		PEresetimage(m_hPE[i], 0, 0);  
		::InvalidateRect(m_hPE[i], NULL, FALSE);
	}
	IsNotFirstRun=1;
	
	GT_GetSts(&status);
	
	if((status&0x01)&&IsNotFirstRun)
	{
		IsNotFirstRun=0;	
		//	rtn = GT_StopWatch();		
		KillTimer(1);
	}
	
}

bool CACMotorCtrlWnd::CheckPara()
{
	UpdateData(TRUE);

#if 0
	if((m_nKi<0)||(m_nKi>200))
	{
		MessageBox("Ki参数设置错误\nKi设置范围是0～200","XX科技",MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_SETKI)->SetFocus();
		return false;
	}
	if((m_nKd<0)||(m_nKd>200))
	{
		MessageBox("Kd参数设置错误\nKd设置范围是0～200","XX科技",MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_SETKD)->SetFocus();
		return false;
	}
	if((m_nKp<0)||(m_nKp>200))
	{
		MessageBox("Kp参数设置错误\nKp设置范围是0～200","XX科技",MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_SETKP)->SetFocus();
		return false;
	}
	if(m_nKp==0)
	{
		MessageBox("Kp参数设置错误\nKp值不应为0","XX科技",MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_SETKP)->SetFocus();
		return false;
	}
    #endif
	return true;
}

void CACMotorCtrlWnd::OnUpdateSetkp() 
{
	m_bParaIsChanged=true;
}

void CACMotorCtrlWnd::OnUpdateSetki() 
{
	m_bParaIsChanged=true;	
}

void CACMotorCtrlWnd::OnUpdateSetkd() 
{
  m_bParaIsChanged=true;	
}

/****************************************************************************/
/*  函数名: InterfaceHandleWithAxisNO										*/
/*  功  能：根据当前轴号行界面设置											*/
/*  参  数： const char * AxisName	轴号									*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::InterfaceHandleWithAxisNO(const char * AxisName)
{
	UpdateData(FALSE);
	#if 0
	CMotorControlBenchApp* pApp=(CMotorControlBenchApp*)AfxGetApp();
    int nCtrlMode = pApp->m_nCtrlMode[m_nAxis];
	((CComboBox *)GetDlgItem(IDC_AC_CTRLMODE))->SetCurSel(nCtrlMode);
	
	if(nCtrlMode==SERVO_MOTOR)
	{
		GetDlgItem(IDC_SETKP)->EnableWindow(true);
		GetDlgItem(IDC_SETKI)->EnableWindow(true);
		GetDlgItem(IDC_SETKD)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_SETKP)->EnableWindow(false);
		GetDlgItem(IDC_SETKI)->EnableWindow(false);
		GetDlgItem(IDC_SETKD)->EnableWindow(false);		
	}
	
    SetAuxAxisNo(AxisName);
	
	if(!m_bGearModeSet)
	{
		for(int i=0;i<4;i++)
		{
			GT_Axis(i);
			GT_AxisOff();
		}
		m_bAxisOn=false;
	}
    #endif
}

/****************************************************************************/
/*  函数名: BufferSampleInit												*/
/*  功  能：缓冲区数据采集初始化设置										*/
/*  参  数： int dataStyle   数据采集类型									*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::BufferSampleInit(int dataStyle)
{
	short rtn;
	rtn = GT_AddWatch(m_nAxis+1,dataStyle,atlPos);
	if (rtn==-1)
	{
		m_bCanDrawWithBuffer=false;
		int res=MessageBox("您所使用的板卡版本不支持数据缓冲采集功能，无法实现绘图功能！\n如您需要绘图功能，请与XX公司联系！\n 继续运行请选“YES” ","XX科技",MB_ICONINFORMATION|MB_YESNO);
		      if (res==IDNO)
			  {
				  return;
			  }
	}
}

/****************************************************************************/
/*  函数名: SMotion															*/
/*  功  能：S曲线运动														*/
/*  参  数： int iAxisNum  进行S曲线运动的轴号								*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CACMotorCtrlWnd::SMotion(int iAxisNum)
{
    #if 0
    double fJerk=0.1;		//加加速度
	double fAcc;			//加速度
	double fVel;			//速度
	long   lPos=0;			//目标位置
	double fMaxAcc=0.4;		//最大加速度
	CString str;
	CEdit *pedit;	

	pedit=(CEdit*)m_tabModeSel.m_tabPages[0]->GetDlgItem(IDC_SMODE_JERK);
	pedit->GetWindowText(str);
	fJerk=atof(str);
	if ((fJerk<=0)||(fJerk>0.01)) {
		MessageBox("加加速度超出范围!\n设置范围是0~0.01pluse/st^3","XX科技",MB_ICONWARNING);
		return false;
	}
	//加速度//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[0]->GetDlgItem(IDC_SMODE_ACC);
	pedit->GetWindowText(str);
	fAcc=atof(str);
	if ((fAcc<=0)||(fAcc>1)){
		MessageBox("加速度超出范围！\n设置范围是0~1Pluse/st^2","XX科技",MB_ICONWARNING);
		return false;
	}
	//速度//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[0]->GetDlgItem(IDC_SMODE_VEL);
	pedit->GetWindowText(str);
	fVel=atof(str);
	if ((fVel<=0)||(fVel>50)){
		MessageBox("速度超出范围！\n设置范围是0~50Pluse/st","XX科技",MB_ICONWARNING);
		return false;
	}
	//目标位置
	pedit=(CEdit*)m_tabModeSel.m_tabPages[0]->GetDlgItem(IDC_SMODE_POS);
	pedit->GetWindowText(str);
	lPos=atol(str);    
	if ((lPos<-1000000)||(lPos>1000000)){
		MessageBox("终点位置超出范围！\n设置范围是-1000000~1000000Pluse","XX科技",MB_ICONWARNING);
		return false;
	}
	m_fTargetVel=fVel;
	
	//绘图区参数设置
	if(lPos>0)
		SetChartScale(0,1,0,lPos*1.2);
	else
		SetChartScale(0,1,lPos*1.2,0);		
	if(fVel>0)
		SetChartScale(1,1,0,fVel*1.3);
	else
		SetChartScale(1,1,fVel*1.3,0);	
	
	/****************************/
	//解决反向运动速度不显示问题//
	//2006.12.1//
	if((m_nShowDataTYpe==ACTUAL_VALUE)&&(lPos<0))
		SetChartScale(1,1,-fVel*1.3,0);
	/********************************/
	SetChartScale(2,1,-fAcc*2,fAcc*2);
	
	//轴S模式运动
	   gtcard.AxisSMotion(iAxisNum,lPos,fVel,fAcc,fJerk);
    #endif
	   return true;
}

/****************************************************************************/
/*  函数名: TMotion															*/
/*  功  能：S曲线运动														*/
/*  参  数： int iAxisNum  进行T曲线运动的轴号								*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CACMotorCtrlWnd::TMotion(int iAxisNum)
{	
#if 0
	double fAcc=0.2;			//加速度
	double fVel=3;			//速度
	long   lPos=0;			//目标位置
	double fMaxAcc=0.4;		//最大加速度
	CString str;
	CEdit *pedit;	

	//加速度//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[1]->GetDlgItem(IDC_TMODE_ACC);
	pedit->GetWindowText(str);
	fAcc=atof(str);
	if ((fAcc<=0)||(fAcc>1)){
		MessageBox("加速度超出范围！\n设置范围是0~1Pluse/st^2","XX科技",MB_ICONWARNING);
		return false;
	}
	//最大速度//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[1]->GetDlgItem(IDC_TMODE_VEL);
	pedit->GetWindowText(str);
	fVel=atof(str);
	if ((fVel<=0)||(fVel>50)){
		MessageBox("速度超出范围！\n设置范围是0~50Pluse/st","XX科技",MB_ICONWARNING);
		return false;
	}
	//目标位置//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[1]->GetDlgItem(IDC_TMODE_POS);
	pedit->GetWindowText(str);
	lPos=atol(str);
	if ((lPos<-1000000)||(lPos>1000000)){
		MessageBox("终点位置超出范围！\n设置范围是-1000000~1000000Pluse","XX科技",MB_ICONWARNING);
		return false;
	}	
	
	m_fTargetVel=fVel;	
	//绘图区刻度设置
	if(lPos>0)
		SetChartScale(0,1,0,lPos*1.2);
	else
		SetChartScale(0,1,lPos*1.2,0);
	
	if(fVel>0)
		SetChartScale(1,1,0,fVel*1.3);
	else
		SetChartScale(1,1,fVel*1.3,0);
	/****************************/
	//解决反向运动速度不显示问题//
	//2006.12.1//
	if((m_nShowDataTYpe==ACTUAL_VALUE)&&(lPos<0))
		SetChartScale(1,1,-fVel*1.3,0);
	/********************************/

	

	SetChartScale(2,1,-fAcc*2,fAcc*2);		
	
	gtcard.AxisTMotion(iAxisNum,lPos,fVel,fAcc);
    #endif
    
	return true;

}

/****************************************************************************/
/*  函数名: VelMotion														*/
/*  功  能：速度模式运动													*/
/*  参  数： int iAxisNum  进行速度模式运动的轴号							*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CACMotorCtrlWnd::VelMotion(int iAxisNum)
{
	double fAcc=0.2;			//加速度
	double fVel=3;			//速度
	double fMaxAcc=0.4;		//最大加速度
	CString str;
	CEdit *pedit;	

	pedit=(CEdit*)m_tabModeSel.m_tabPages[2]->GetDlgItem(IDC_VELMODE_ACC);
	pedit->GetWindowText(str);
	fAcc=atof(str);
	if ((fAcc<=0)||(fAcc>1)){
		MessageBox("加速度超出范围！\n设置范围是0~50Pluse/st^2","XX科技",MB_ICONWARNING);
		return false;
	}
	//最大速度//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[2]->GetDlgItem(IDC_VELMODE_VEL);
	pedit->GetWindowText(str);
	fVel=atof(str);
	if ((fVel<=0)||(fVel>50)){
		MessageBox("速度超出范围！\n设置范围是0~50Pluse/st","XX科技",MB_ICONWARNING);
		return false;
	}
	m_fTargetVel=fVel;
	//绘图区刻度设置//		
	SetChartScale(0,3);		
	
	if(fVel>0)
		SetChartScale(1,1,0,fVel*1.3);
	else
		SetChartScale(1,1,fVel*1.3,0);
	
	SetChartScale(2,1,-2*fAcc,2*fAcc);		
	
	gtcard.AxisVelMotion(iAxisNum,fAcc,fVel);
	return true;
}


void CACMotorCtrlWnd::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
   
    POSITION pos;
    pos = m_listCtrl.GetFirstSelectedItemPosition();
   
   // 得到项目索引
    IndexInFieldList = m_listCtrl.GetNextSelectedItem(pos);  

    *pResult = 0;
}


#if 0
void CACMotorCtrlWnd::OnButtonXpositive()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFormView::OnTimer(2);

      SetTimer(2,100,NULL);
}

void CACMotorCtrlWnd::OnButtonXpositive()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFormView::OnTimer(2);

      SetTimer(2,100,NULL);
}
void CACMotorCtrlWnd::OnBnDropDownButtonXpositive(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMBCDROPDOWN pDropDown = reinterpret_cast<LPNMBCDROPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(2,100,NULL);
	*pResult = 0;
}

#endif

void CACMotorCtrlWnd::GetJogIncValue(double * fDist)
{
	CString str;
	GetDlgItem(IDC_EDIT_STEP)->GetWindowText(str);
	str.Remove('m');
	*fDist=atof(str);
}


void CACMotorCtrlWnd::OnBnClickedButtonXpositive()
{
    double dist;
    GetJogIncValue(&dist);
    gtcard.Jog(AXIS_1,dist,1,POSITIVE_DIRCT);
}

//void CACMotorCtrlWnd::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CFormView::OnLButtonDown(nFlags, point);
//}


void CACMotorCtrlWnd::OnBnClickedButtonXreverse()
{
	// TODO: 在此添加控件通知处理程序代码
	double dist;

      GetJogIncValue(&dist);
	gtcard.Jog(0,dist,1,NEGTIVE_DIRCT);
}




void CACMotorCtrlWnd::OnBnClickedButtonYpositive()
{
	// TODO: 在此添加控件通知处理程序代码
	double dist;

      GetJogIncValue(&dist);
	gtcard.Jog(1,dist,1,POSITIVE_DIRCT);
}


void CACMotorCtrlWnd::OnBnClickedButtonYrerverse()
{
	// TODO: 在此添加控件通知处理程序代码
	double dist;

      GetJogIncValue(&dist);
	gtcard.Jog(1,dist,1,NEGTIVE_DIRCT);
}


void CACMotorCtrlWnd::OnBnClickedButtonZpositive()
{
	// TODO: 在此添加控件通知处理程序代码
	double dist;

       GetJogIncValue(&dist);
	gtcard.Jog(AXIS_4,dist,1,POSITIVE_DIRCT);
}


void CACMotorCtrlWnd::OnBnClickedButtonZrerverse()
{
	// TODO: 在此添加控件通知处理程序代码
      double dist;

      GetJogIncValue(&dist);
	gtcard.Jog(AXIS_4,dist,1,NEGTIVE_DIRCT);
}

void CACMotorCtrlWnd::OnBnClickedCheckLeftIn()
{
    // TODO: 在此添加控件通知处理程序代码
    #if 0
    CButton* but = (CButton*)GetDlgItem(IDC_CHECK_LEFT_IN);

    if (but->GetCheck())
    {
        Side = LEFT_SIDE;
        ((CButton*)GetDlgItem(IDC_CHECK_RIGHT_IN))->SetCheck(FALSE); 
 //       GetDlgItem(IDC_CHECK_RIGHT_IN)->EnableWindow(TRUE);
    }
    #endif
}



void CACMotorCtrlWnd::OnBnClickedCheckRightIn()
{
	// TODO: 在此添加控件通知处理程序代码
	#if 0
	CButton* but = (CButton*)GetDlgItem(IDC_CHECK_RIGHT_IN);

    if (but->GetCheck())
    {
        Side = RIGHT_SIDE;
        ((CButton*)GetDlgItem(IDC_CHECK_LEFT_IN))->SetCheck(FALSE); 
    }
    #endif
}

void CACMotorCtrlWnd::OnBnClickedButtonOpenfile()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strGCode;
    CStdioFile fs;
    CString strLine;
    
    CString filters;
    CString suffix;
    CString strResult;

    if(Side == NO_SIDE  || Side >= MAX_SIDE_VALUE)
    {
          AfxMessageBox("请首先选择左进或右进！",MB_OK);
          return;
    }

    if(Side == LEFT_SIDE)
    {
         filters = "CNC File(*.lll)|*.lll|";
         suffix = "*.lll";
    }
    else if(Side == RIGHT_SIDE)
    {
         filters = "CNC File(*.rrr)|*.rrr|";
         suffix = "*.rrr";
    }
    
    CFileDialog fileDlg(TRUE,NULL,suffix,NULL,filters,NULL);
    
    int result = fileDlg.DoModal();	
    if(result==IDOK)
    {
        strFilePathName=fileDlg.GetPathName();
    }
    else
    {
        return;
    }
    
    if(fs.Open(strFilePathName, CFile::modeRead | CFile::typeText)==NULL) 
    {
        AfxMessageBox("打开工作档案出错！",MB_OK);
        return;
    }
    
    if (fs.GetLength()==NULL) 
    {
        AfxMessageBox("指定的文件内容为空！",MB_OK);
        return;
    }


    int index(0);
    int idx;
    stringstream ss ; 

    if(Side == LEFT_SIDE)
    {
        if(fs.ReadString(strLine) !=NULL)
        {
            idx = strLine.Find(_T("LB0"), 0);
            if(idx != -1)
            {
                strResult = strLine.Mid(idx+strlen("LB0")); //strResult == ab
                ss << strResult.GetBuffer() ;
                if (! ss.eof())
                {
                    ss >> LB0_ValueX ;
                }
                if (! ss.eof())
                {
                    ss >> LB0_ValueY ;
                }
            }
        }

        if(fs.ReadString(strLine) !=NULL)
        {
            idx = strLine.Find(_T("LB1"), 0);

            if(idx != -1)
            {
                ss.clear();
                strResult = strLine.Mid(idx + strlen("LB1")); //strResult == ab
                ss << strResult.GetBuffer() ;
                if (! ss.eof())
                {
                    ss >> LB1_ValueX ;
                }
                if (! ss.eof())
                {
                    ss >> LB1_ValueY ;
                }
            }
        }

        if(fs.ReadString(strLine) !=NULL)
        {
            idx = strLine.Find(_T("M0"), 0);
            if(idx != -1)
            {
                strResult = strLine.Mid(idx+strlen("M0")); //strResult == ab
                ss << strResult.GetBuffer() ;
                if (! ss.eof())
                {
                    ss >> L_MPoints[0].PosX;
                }
                if (! ss.eof())
                {
                    ss >> L_MPoints[0].PosY;
                }
                if (! ss.eof())
                {
                    ss >> L_MPoints[0].PosZ;
                }
            }
        }

        if(fs.ReadString(strLine) !=NULL)
        {
            idx = strLine.Find(_T("M1"), 0);
            if(idx != -1)
            {
                strResult = strLine.Mid(idx+strlen("M1")); //strResult == ab
                ss << strResult.GetBuffer() ;
                if (! ss.eof())
                {
                    ss >> L_MPoints[1].PosX;
                }
                if (! ss.eof())
                {
                    ss >> L_MPoints[1].PosY; 
                }
                if (! ss.eof())
                {
                    ss >> L_MPoints[1].PosZ; 
                }
            }
        }
        
        m_ccdDeltaX = L_MPoints[0].PosX - L_MPoints[1].PosX;
        m_ccdDeltaY = L_MPoints[0].PosY - L_MPoints[1].PosY;

        /* 这里需要判断一下上面是否合法，文件是否合法*/
        
        while(fs.ReadString(strLine) !=NULL)
        {
            /* */
            idx = strLine.Find(_T("DownCut"), 0);
            if(idx != -1)
            {
                ss.clear();
                L_Points[index].type = "DownCut"; //DOWN_CUT;
                strResult = strLine.Mid(idx + strlen("DownCut")); //strResult == ab
                ss << strResult.GetBuffer() ;
                if (! ss.eof())
                {
                    ss >> L_Points[index].PosX ;
                }
                if (! ss.eof())
                {
                    ss >> L_Points[index].PosY  ;
                }
                if (! ss.eof())
                {
                    ss >> L_Points[index].PosZ  ;
                }    
                if (! ss.eof())
                {
                    ss >> L_Points[index].CutSpeed;
                }   
                
 //               L_Points[index].CurrentNum = index;
                index ++;
                PointNum = index;
            }
            else 
            {
                //index = 0;
                idx = strLine.Find(_T("DryRun"), 0);
                if(idx != -1)
                {
                    ss.clear();
                    L_Points[index].type = "DryRun"; //DRY_RUN;
                    strResult = strLine.Mid(idx + strlen("DryRun")); //strResult == ab
                    ss << strResult.GetBuffer() ;
                    if (! ss.eof())
                    {
                        ss >> L_Points[index].PosX ;
                    }
                    if (! ss.eof())
                    {
                        ss >> L_Points[index].PosY;
                    }
                    if (! ss.eof())
                    {
                        ss >> L_Points[index].PosZ  ;
                    }    
                    if (! ss.eof())
                    {
                        ss >> L_Points[index].CutSpeed;
                    }  
                //                   L_Points[index].CurrentNum = index;
                    index ++;
                    PointNum = index;
                }
            }
        }

        int nItem;

        if(m_tabModeSel.GetCurSel() == LABAL_TAB)
        {
            CString f1, f2, f3, f4;
            m_listCtrl.DeleteItem(0);

    		// 格式化字符串
            f1.Format("LB0");
            f2.Format("%d", LB0_ValueX);
            f3.Format("%d", LB0_ValueY);
            nItem =  m_listCtrl.InsertItem(0, _T(f1));  // 插入第一列
            m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
            m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列

            m_listCtrl.DeleteItem(1);
            f1.Format("LB1");
            f2.Format("%d", LB1_ValueX);
            f3.Format("%d", LB1_ValueY);
            nItem =  m_listCtrl.InsertItem(1, _T(f1));  // 插入第一列
            m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
            m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列
        }
        
        if(m_tabModeSel.GetCurSel() == EXC_TAB || m_tabModeSel.GetCurSel() == TECH_TAB)
        {
            CString f1, f2, f3, f4,f5;

            m_listCtrl.DeleteAllItems();

            for(int i=0; i< PointNum; i++)
            {
                // 格式化字符串
                f1.Format("L%d", i);
                f2.Format("%d", L_Points[i].PosX);
                f3.Format("%d", L_Points[i].PosY);
                f4.Format("%d", L_Points[i].PosZ);

                m_listCtrl.DeleteItem(i);

                nItem = m_listCtrl.InsertItem(i, _T(f1));  // 插入第一列
                m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
                m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列
                m_listCtrl.SetItemText(nItem, 3, _T(f4));  // 插入第三列
                if(L_Points[i].type == "DryRun")
                {
                    m_listCtrl.SetItemText(nItem, 4, L_Points[i].type);  // 插入第三列
                }
                else
                {
                    f5.Format("%d", L_Points[i].CutSpeed);          
                    m_listCtrl.SetItemText(nItem, 4, _T(f5));  // 插入第三列
                }
           }
        }

        if(m_tabModeSel.GetCurSel() == SYS_TAB)
        {
            CString f1, f2, f3, f4;
            m_listCtrl.DeleteItem(0);

    		// 格式化字符串
            f1.Format("M0");
            f2.Format("%d", L_MPoints[0].PosX);
            f3.Format("%d", L_MPoints[0].PosY);
            nItem =  m_listCtrl.InsertItem(0, _T(f1));  // 插入第一列
            m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
            m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列

            m_listCtrl.DeleteItem(1);
            f1.Format("M1");
            f2.Format("%d", L_MPoints[1].PosX);
            f3.Format("%d", L_MPoints[1].PosY);
            nItem =  m_listCtrl.InsertItem(1, _T(f1));  // 插入第一列
            m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
            m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列
            
            m_ccdDeltaX = L_MPoints[0].PosX - L_MPoints[1].PosX;
            m_ccdDeltaY = L_MPoints[0].PosY - L_MPoints[1].PosY;
        }
    }
    
    fs.Close();

    //GetDlgItem(IDC_BUTTON_SAVE_FILE)->EnableWindow(TRUE);
    //GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(strFilePathName);
}


void CACMotorCtrlWnd::OnBnClickedButtonNewfile()
{
    CStdioFile fs;//NC文件
    CString strLine;
    CString strNCFileName;//NC文件名		
    CEdit * pGCodeEdit;

    int nItem;
    CString f1;

    CString filters;
    CString suffix1;
    CString suffix2;

    if(Side <= NO_SIDE  || Side >= MAX_SIDE_VALUE)
    {
          AfxMessageBox("请首先选择左进或右进！",MB_OK);
          return;
    }

    if(Side == LEFT_SIDE)
    {
         filters = "CNC File(*.lll)|*.lll|";
         suffix1 = "lll";
         suffix2 = "*.lll";
    }
    else if(Side == RIGHT_SIDE)
    {
         filters = "CNC File(*.rrr)|*.rrr|";
         suffix1 = "rrr";
         suffix2 = "*.rrr";
    }

    CFileDialog fileDlg(FALSE, suffix1, suffix2,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filters,NULL);
    int result = fileDlg.DoModal();	
    if(result==IDOK)
    {
        strFilePathName = fileDlg.GetPathName();
    }
    else
    {
        return;
    }
    //文件保存
    if(fs.Open(strFilePathName, CFile::modeCreate |CFile::modeWrite)==NULL) 
    {
        AfxMessageBox("文件操作出错！",MB_OK);
        return;
    }

    LB0_ValueX = 0;
    LB0_ValueY = 0;
    LB1_ValueX = 0;
    LB1_ValueY = 0;
    RB0_ValueX = 0;
    RB0_ValueY = 0;
    RB1_ValueX = 0;
    RB1_ValueY = 0;

    PointNum = 0;
       
    for(int i=0; i<MAX_POINT; i++)
    {
        L_Points[i].type = "NULL";
        L_Points[i].PosX = 0;
        L_Points[i].PosY = 0;
        L_Points[i].PosZ = 0;
        R_Points[i].type = "NULL";
        R_Points[i].PosX = 0;
        R_Points[i].PosY = 0;
        R_Points[i].PosZ = 0;
    }

    CString strBuffer;
    strBuffer.ReleaseBuffer();
    if(Side == LEFT_SIDE)
    {
        strLine.Format("LB0 0  0                      \n",strBuffer);
        fs.WriteString(strLine);		
        strLine.Format("LB1 0  0                      \n",strBuffer);
        fs.WriteString(strLine);	

        if(m_tabModeSel.GetCurSel() == LABAL_TAB)
        {
            m_listCtrl.DeleteItem(0);

            f1.Format("LB0");
            nItem =  m_listCtrl.InsertItem(0, _T(f1));  // 插入第一列
            m_listCtrl.SetItemText(0, 1, "0");  // 插入第二列
            m_listCtrl.SetItemText(0, 2, "0");  // 插入第三列

            m_listCtrl.DeleteItem(1);
            f1.Format("LB1");
            nItem =  m_listCtrl.InsertItem(1, _T(f1));  // 插入第一列
            m_listCtrl.SetItemText(1, 1, "0");  // 插入第二列
            m_listCtrl.SetItemText(1, 2, "0");  // 插入第三列
        }

    }
    else if(Side == RIGHT_SIDE)
    {
        strLine.Format("RB0 0  0\n",strBuffer);
        fs.WriteString(strLine);		
        strLine.Format("RB1 0  0\n",strBuffer);
        fs.WriteString(strLine);	

        m_listCtrl.DeleteItem(0);

        f1.Format("RB0");
        nItem =  m_listCtrl.InsertItem(0, _T(f1));  // 插入第一列
        m_listCtrl.SetItemText(0, 1, "0");  // 插入第二列
        m_listCtrl.SetItemText(0, 2, "0");  // 插入第三列

        m_listCtrl.DeleteItem(1);

        f1.Format("RB1");
        nItem =  m_listCtrl.InsertItem(1, _T(f1));  // 插入第一列
        m_listCtrl.SetItemText(1, 1, "0");  // 插入第二列
        m_listCtrl.SetItemText(1, 2, "0");  // 插入第三列
    }

    strLine.Format("M0 0  0  0                   \n",strBuffer);
    fs.WriteString(strLine);		
    strLine.Format("M1 0  0  0                   \n",strBuffer);
    fs.WriteString(strLine);	

    if(m_tabModeSel.GetCurSel() == SYS_TAB)
    {
        m_listCtrl.DeleteItem(0);

        f1.Format("M0");
        nItem =  m_listCtrl.InsertItem(0, _T(f1));  // 插入第一列
        m_listCtrl.SetItemText(0, 1, "0");  // 插入第二列
        m_listCtrl.SetItemText(0, 2, "0");  // 插入第三列
        m_listCtrl.SetItemText(0, 3, "0");  // 插入第三列

        m_listCtrl.DeleteItem(1);
        f1.Format("M1");
        nItem =  m_listCtrl.InsertItem(1, _T(f1));  // 插入第一列
        m_listCtrl.SetItemText(1, 1, "0");  // 插入第二列
        m_listCtrl.SetItemText(1, 2, "0");  // 插入第三列           
        m_listCtrl.SetItemText(1, 3, "0");  // 插入第三列
    }

    fs.Close();	
    
#if 0
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(strNCFileName);
	bShouldSave=false;
#endif
}





void CACMotorCtrlWnd::OnEnChangeEditDiameter()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT_DIAMETER)->GetWindowText(str);
	m_Diameter = atol(str);
}



void CACMotorCtrlWnd::OnDeltaposSpinDiameter(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

    if(pNMUpDown->iDelta == -1)  // 如果此值为-1 , 说明点击了Spin的往上箭头
    {
        m_Diameter++;
    }
    else if(pNMUpDown->iDelta == 1)  // 如果此值为, 说明点击了Spin的往下箭头
    {
        m_Diameter--;
    }
    
    CString ss;
    ss.Format(_T("%d "), m_Diameter);
    GetDlgItem(IDC_EDIT_DIAMETER)->SetWindowText(ss);   //EDIT控件显示数据
    
    *pResult = 0;
    
}



void CACMotorCtrlWnd::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
    int nItem;
    long pnt[4] = {0};

    CString f1,f2,f3;
    CString strLine, strLineTmp;
    CStdioFile fs;
    int ret = 0;

   if(strFilePathName.IsEmpty())
   {
         AfxMessageBox("请先打开工作档案！",MB_OK);
         return;
   }
    
    /* 保存在档案中*/
    if(fs.Open(strFilePathName, CFile::modeReadWrite | CFile::typeText)==NULL) 
    {
        AfxMessageBox("打开工作档案出错！",MB_OK);
        return;
    }

    for(int i=0; i<3; i++)
    {
        ret=GT_Axis(i+1);
        ret=GT_GetAtlPos(&pnt[i]);
    }
   
    if(m_tabModeSel.GetCurSel() ==  SYS_TAB)
    {
        switch (IndexInFieldList)
        {
            case 0:
                L_MPoints[0].PosX = pnt[0];
                L_MPoints[0].PosY = pnt[1];
                L_MPoints[0].PosZ=  pnt[2];
                strLine.ReleaseBuffer();

                strLine.Format("M0  %d %d %d     ", pnt[0], pnt[1], pnt[2]);
                if(fs.ReadString(strLineTmp) !=NULL && fs.ReadString(strLineTmp) !=NULL)
                {
                    fs.Seek(0,CFile::current);
                    fs.WriteString(strLine);		
                }

                /* 插入列表中 */
                f1.Format("%d", pnt[0]);
                f2.Format("%d", pnt[1]);
                f3.Format("%d", pnt[2]);
                m_listCtrl.SetItemText(0, 1, _T(f1));  // 插入第二列
                m_listCtrl.SetItemText(0, 2, _T(f2));  // 插入第三列
                m_listCtrl.SetItemText(0, 3, _T(f3));  // 插入第三列	
            
                break;
            case 1:
                L_MPoints[1].PosX = pnt[0];
                L_MPoints[1].PosY = pnt[1];
                L_MPoints[1].PosZ=  pnt[2];
                strLine.ReleaseBuffer();

                strLine.Format("M1  %d %d %d     ", pnt[0], pnt[1], pnt[2]);
                if(fs.ReadString(strLineTmp) !=NULL && fs.ReadString(strLineTmp) !=NULL && fs.ReadString(strLineTmp) !=NULL)
                {
                    fs.Seek(0,CFile::current);
                    fs.WriteString(strLine);		
                }              

                /* 插入列表中 */
                f1.Format("%d", pnt[0]);
                f2.Format("%d", pnt[1]);
                f3.Format("%d", pnt[2]);
                m_listCtrl.SetItemText(1, 1, _T(f1));  // 插入第二列
                m_listCtrl.SetItemText(1, 2, _T(f2));  // 插入第三列
                m_listCtrl.SetItemText(1, 3, _T(f3));  // 插入第三列

                m_ccdDeltaX = L_MPoints[0].PosX - L_MPoints[1].PosX;
                m_ccdDeltaY = L_MPoints[0].PosY - L_MPoints[1].PosY;
                    
                break;
            default:
                break;   
        }
    }

    fs.Close();	

}


void CACMotorCtrlWnd::OnBnClickedCheckLeftProgram()
{
	// TODO: 在此添加控件通知处理程序代码
    CButton* but = (CButton*)GetDlgItem(IDC_CHECK_LEFT_PROGRAM);

    if (but->GetCheck())
    {
        Side = LEFT_SIDE;
        ((CButton*)GetDlgItem(IDC_CHECK_RIGHT_PROGRAM))->SetCheck(FALSE); 
 //       GetDlgItem(IDC_CHECK_RIGHT_IN)->EnableWindow(TRUE);
    }
}


void CACMotorCtrlWnd::OnBnClickedCheckRightProgram()
{
	// TODO: 在此添加控件通知处理程序代码
    CButton* but = (CButton*)GetDlgItem(IDC_CHECK_RIGHT_PROGRAM);

    if (but->GetCheck())
    {
        Side = RIGHT_SIDE;
        ((CButton*)GetDlgItem(IDC_CHECK_LEFT_PROGRAM))->SetCheck(FALSE); 
    }

}

void CACMotorCtrlWnd::OnBnClickedCcdLightOnoff()
{
	// TODO: 在此添加控件通知处理程序代码
    CButton* but = (CButton*)GetDlgItem(IDC_CCD_LIGHT_ONOFF);

    if (but->GetCheck())
    {
        gtcard.OpenPort(0,true);
    }
    else
    {
        gtcard.OpenPort(0,false);
    }
}


void CACMotorCtrlWnd::OnBnClickedMainAxic()
{
	// TODO: 在此添加控件通知处理程序代码
    CButton* but = (CButton*)GetDlgItem(IDC_MAIN_AXIC);

    if (but->GetCheck())
    {
        gtcard.OpenPort(1,true);
    }
    else
    {
        gtcard.OpenPort(1,false);
    }
}


void CACMotorCtrlWnd::OnBnClickedDustCollector()
{
	// TODO: 在此添加控件通知处理程序代码
    CButton* but = (CButton*)GetDlgItem(IDC_DUST_COLLECTOR);

    if (but->GetCheck())
    {
        gtcard.OpenPort(2,true);
    }
    else
    {
        gtcard.OpenPort(2,false);
    }
}


void CACMotorCtrlWnd::OnBnClickedLeftAirDoor()
{
	// TODO: 在此添加控件通知处理程序代码
    CButton* but = (CButton*)GetDlgItem(IDC_LEFT_AIR_DOOR);

    if (but->GetCheck())
    {
        gtcard.OpenPort(3,true);
    }
    else
    {
        gtcard.OpenPort(3,false);
    }
}


void CACMotorCtrlWnd::OnBnClickedRightAirDoor()
{
	// TODO: 在此添加控件通知处理程序代码
    CButton* but = (CButton*)GetDlgItem(IDC_RIGHT_AIR_DOOR);

    if (but->GetCheck())
    {
        gtcard.OpenPort(4,true);
    }
    else
    {
        gtcard.OpenPort(4,false);
    }
}


void CACMotorCtrlWnd::OnBnClickedZeroButton()
{
	// TODO: 在此添加控件通知处理程序代码
	gtcard.HomeAll(HOME_TWOLIMITS);

}

