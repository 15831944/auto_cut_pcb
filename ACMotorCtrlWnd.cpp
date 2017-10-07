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
#define BUFFER_SIZE 20    //����˶����ݵİ忨��������С
#define SAMPLE_INTERVAL 49  //�ɼ�����ʱ���� 

#if 1
CWnd *pwnd;                         //窗体基类
CDC  *pDC;   
CRect lRect3;   
CStatic *pStaic;

long        Smooth_Step=5;     //����1��ƽ���˲�΢������ֵ��Ĭ��Ϊ3
IplImage*   src=0;             //opencv IplImage��ͼ��ָ��������źʹ����ȡ����ͼ��
CRect lRect;   
HDC Disp_hDC1 ;                 //设备句柄DC 

#define INTERVAL_VALUE	5  //������ʾˢ��ʱ����

#endif


//ȫ�ֱ���//
extern CGTCard gtcard;
CArray<double,double>  glVelArray;
double  atlPos[BUFFER_SIZE], atlVel[BUFFER_SIZE];

//��ͼ�ؼ��ṹ��
struct CHARTPARA {
	HWND  hWnd;  //��ͼ�ؼ����ھ��
	UINT nID;    //��ͼ�ؼ�ID��
	RECT  ChartRect;//��ͼ�ؼ����ھ���
	char * chTitle;//��ͼ�ؼ�����
	char * XLabel; //X�����
	char *  YLabel;	//Y�����
	int nLineCount; //��ͼ�ؼ��е�������
	long nPointCount;//��ʾ����
	double fXMax,fXMin;//X�������ֵ����Сֵ
	double fYMax,fYMin;//Y�������ֵ����Сֵ
};



//IMPLEMENT_DYNCREATE(CACMotorCtrlWnd, CFormView)
DWORD WINAPI DrawThread(LPVOID pParam)
{	
	//�ɼ��������ʱ����
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
	}while(status&0x400);//�����˶�		 
	
				for(i = 0; i < 3; ++i) //�������ֹͣʱ�̵�����
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
	float fActualY[3];				//����,����������λ��[0],�ٶ�[1],���ٶ�[2]
	unsigned short nCountOfBuffer;	//��������С��Ϊ0��ʾ��������
	unsigned short status; 			//�忨״̬	
	int IsReachTargetVel(0);	   //�ж��ٶ��Ƿ�ﵽĿ��ֵ�������ٶ����ݴ���	 

	CACMotorCtrlWnd * pWnd=(CACMotorCtrlWnd *) pParam;
	
	//�ӻ�������ȡ���˶�����			  
	#if 0
	do {	
		rtn=GT_GetWatch(&nCountOfBuffer);						  
		if(nCountOfBuffer != 0)			//��������������
		{   
			for(int i = 0;i < nCountOfBuffer;++i)
			{	
				//��ȡ��λ�ú��ٶ�����
				fActualY[0]=(float)atlPos[i];
				fActualY[1]=(float)atlVel[i];
				
				//���ٶ����ݼ��뵽glVelArray�����ڼ�����ٶ�
				glVelArray.Add(atlVel[pWnd->count]);
				//	TRACE("Pos: No %d %lf\n",i,atlPos[i]);
				
				//�ٶ�ֵ�������ڰ忨���������ص�ʵ���ٶ�ֵ���ܻ���+-1�������ƫ�
				//�����ڷǼӼ��ٽ׶μ��ٶȼ��㲻��ȷ���������´���
				if(fActualY[1]==pWnd->m_fTargetVel)  IsReachTargetVel=1;		
				if(IsReachTargetVel&&(pWnd->m_nShowDataTYpe!=PROFILE_VALUE)) 
				{
					if((fActualY[1]<=pWnd->m_fTargetVel+1)&&(fActualY[1]>=pWnd->m_fTargetVel-1))
						fActualY[1]=pWnd->m_fTargetVel;
					glVelArray.SetAt(pWnd->count,fActualY[1]);
					//	fActualY[1]=(float)(glVelArray.GetAt(count-2)+glVelArray.GetAt(count-1)+fActualY[1])/3;
				}
				
				//����ʱ��ֵ
				fTime=(float) pWnd->count*(SAMPLE_INTERVAL+1)*0.0002;
				
				//������ٶ�
				if(pWnd->count==0) fActualY[2]=0; //��ʼʱ�����ٶ�Ϊ0
				else
				{
					fActualY[2]=(float)((glVelArray.GetAt(pWnd->count)-glVelArray.GetAt(pWnd->count-1)))/(SAMPLE_INTERVAL+1);
					if(fActualY[2]==(float)0.0) fActualY[2]=(float)0.00001; //Ϊ0ʱ��ͼ�����޷���ʾ���ߣ�����Ϊ0.00001
				}
				
				//��ҳ��ʾ����
				if (fTime>=pWnd->m_fEndTimeOfPage) {
					pWnd->m_DrawPageCount++;
					pWnd->m_CurDrawPage++;			
					pWnd->m_fEndTimeOfPage = pWnd->m_fEndTimeOfPage +pWnd->m_fIntervalOfPage;
					pWnd->m_fStartTimeOfPage = pWnd->m_fEndTimeOfPage - pWnd->m_fIntervalOfPage;
					//������������̶�
					for(int i=0;i<3;i++)
					{
						PEvset(pWnd->m_hPE[i], PEP_fMANUALMINX, &pWnd->m_fStartTimeOfPage, 1);
						PEvset(pWnd->m_hPE[i], PEP_fMANUALMAXX, &pWnd->m_fEndTimeOfPage, 1);	
					}
				}
				
				//�������ݱ��浽��Ӧ�Ļ�ͼ�ؼ����ݴ洢��Ԫ
				//λ��
				PEvsetcellEx(pWnd->m_hPE[0], PEP_faYDATA, 0, pWnd->count, &fActualY[0]);
				PEvsetcellEx(pWnd->m_hPE[0], PEP_faXDATA, 0, pWnd->count, &fTime);
				//�ٶ�
				PEvsetcellEx(pWnd->m_hPE[1], PEP_faYDATA, 0, pWnd->count, &fActualY[1]);
				PEvsetcellEx(pWnd->m_hPE[1], PEP_faXDATA, 0, pWnd->count, &fTime);	
				//���ٶ�
				PEvsetcellEx(pWnd->m_hPE[2], PEP_faYDATA, 0, pWnd->count, &fActualY[2]);
				PEvsetcellEx(pWnd->m_hPE[2], PEP_faXDATA, 0, pWnd->count, &fTime);	
				//	TRACE("�� %d �Σ�%f\n",count,fActualY[2]);
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
					//	str.Format("��%dҳ\n",m_DrawPageCount);
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
				//	TRACE("�� %d �Σ�%f\n",count,fActualY[2]);
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
	//������ͼ�ؼ�
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

	//��ʼ���忨
	InitCard();  
      
	//����ҳ���ʼ��
	m_tabModeSel.InsertItem(0,"ִ��");
	m_tabModeSel.InsertItem(1,"��׼");
	m_tabModeSel.InsertItem(2,"�̵�");
	m_tabModeSel.InsertItem(3,"�༭");
	m_tabModeSel.InsertItem(4,"ϵͳ");
	m_tabModeSel.InitTabPage();

      // �����б�ؼ�����
    	m_listCtrl.InsertColumn(0, "T0", LVCFMT_LEFT, 100);
    	m_listCtrl.InsertColumn(1, "X(um)", LVCFMT_LEFT, 120);
    	m_listCtrl.InsertColumn(2, "Y(um)", LVCFMT_LEFT, 120);
      m_listCtrl.InsertColumn(3, "Z or R(um)", LVCFMT_LEFT, 120);
    	m_listCtrl.InsertColumn(4, "Spd(mm/sec)", LVCFMT_LEFT, 120);

    	// �����б�ؼ���չ��ʽΪ���и���
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
	//��ȡϵͳ������
	int nAxisCount;
	CMotorControlBenchApp* pApp=(CMotorControlBenchApp*)AfxGetApp();
	nAxisCount=pApp->m_nAxisCount;

	//��ʼ����ͼ��
	SetChartPara();
	
	//��ʼ���忨
	InitCard();  

	//����ϵͳ���������ý���
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

	//��ʼ���忨
	InitCard();  
      
	//����ҳ���ʼ��
	m_tabModeSel.InsertItem(0,"ִ��");
	m_tabModeSel.InsertItem(1,"��׼");
	m_tabModeSel.InsertItem(2,"�̵�");
	m_tabModeSel.InsertItem(3,"�༭");
	m_tabModeSel.InitTabPage();

      // �����б�ؼ�����
    	m_listCtrl.InsertColumn(0, "T0", LVCFMT_LEFT, 100);
    	m_listCtrl.InsertColumn(1, "X(um)", LVCFMT_LEFT, 120);
    	m_listCtrl.InsertColumn(2, "Y(um)", LVCFMT_LEFT, 120);
      m_listCtrl.InsertColumn(3, "Z or R(um)", LVCFMT_LEFT, 120);
    	m_listCtrl.InsertColumn(4, "Spd(mm/sec)", LVCFMT_LEFT, 120);

    	// �����б�ؼ���չ��ʽΪ���и���
    	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);   

#if 0
    LineListRect=CRect(0,530,800,620);	

    m_LineList.Create(LVS_EX_GRIDLINES | LVS_REPORT | LVS_EDITLABELS | WS_CHILD | WS_VISIBLE, LineListRect, this, IDC_LINE_LIST);
    m_LineList.InsertColumn(0, "Column 1", LVCFMT_LEFT, 40, 500);
    m_LineList.InsertItem( 0, "aaa");
#endif
    
    #if 0
	//���������ļ�ϵͳ�������ý���
	CComboBox * pModeSel;
	int nCtrlMode = pApp->m_nCtrlMode[m_nAxis];
	m_nCardType=pApp->m_nCardType;	
	pModeSel=(CComboBox *)GetDlgItem(IDC_AC_CTRLMODE);

    if(SG_CARD==m_nCardType)
	{	
		pModeSel->InsertString(0,"������");
		pModeSel->SetCurSel(0);
		GetDlgItem(IDC_SETKP)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETKI)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETKD)->EnableWindow(FALSE);		
	}
	else
	{
		pModeSel->InsertString(0,"ģ���ѹ");
		pModeSel->InsertString(1,"������");
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
/*  ������: InitChart       												*/
/*  ��  �ܣ���ʼ�����߻��ƿؼ�												*/
/*  ��  ���� 																*/
/*            1. UINT nWndNo    �ؼ���								    	*/	 
/*            2. CHARTPARA para  �ؼ������ṹ�����							*/	
/*  ��  �ڣ�																*/ 
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::InitChart(UINT nWndNo,CHARTPARA para)
{
    HWND hPE;
	RECT rect;       //��ͼ���ھ���
	UINT nID;        //�����Ļ�ͼ����ID��
	long nPtCount;   //������ʾ����
	int nLineCount;  //��ʾ������
	int nLineStyle;  //����
	double fXMin,fXMax;
	double fYMin,fYMax;
	char * strXLabel,* strYLabel;	
	char * strTitle;

	//���ݴ���Ĳ�����ʼ����ͼ�ؼ�
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

  //�������пؼ�
	if(m_hPE[nWndNo])
	{
		PEdestroy(m_hPE[nWndNo]);
		Invalidate();
	}
	
  // Construct Object ,������ʾ����//
   hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, nID); 
   
  //set subset's number,������ʾ������
   PEnset(hPE, PEP_nSUBSETS, nLineCount);
  
   //set point count,���õ�����ʾ����
   PEnset(hPE, PEP_nPOINTS, nPtCount);
 
   // Set Manual Y scale ,����Y���ֶ��̶�//   
   PEnset(hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
   PEvset(hPE, PEP_fMANUALMINY, &fYMin, 1);
   PEvset(hPE, PEP_fMANUALMAXY, &fYMax, 1);

   // Set Manual X scale,����X���ֶ��̶� //
   PEnset(hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
   PEvset(hPE, PEP_fMANUALMINX, &fXMin, 1);
   PEvset(hPE, PEP_fMANUALMAXX, &fXMax, 1);
 
   //��ͼ���ݳ�ʼ��
   float val = 0;
   for (int i=0;i<nLineCount;i++)
   {	  
	   // Clear out default data,���ԭ������ //
	   PEvsetcellEx(hPE, PEP_faXDATA, i, 0, &val);
	   PEvsetcellEx(hPE, PEP_faXDATA, i, 1, &val);
	   PEvsetcellEx(hPE, PEP_faXDATA, i, 2, &val);
	   PEvsetcellEx(hPE, PEP_faXDATA, i, 3, &val);
	   
	   PEvsetcellEx(hPE, PEP_faYDATA, i, 0, &val);
	   PEvsetcellEx(hPE, PEP_faYDATA, i, 1, &val);
	   PEvsetcellEx(hPE, PEP_faYDATA, i, 2, &val);
	   PEvsetcellEx(hPE, PEP_faYDATA, i, 3, &val);

	   //set the line type,������ʾ���ߵ�����//	   	   
	   PEvsetcell(hPE, PEP_naSUBSETLINETYPES, i, &nLineStyle); 
   } 
 
  //set line title,������������//   
   char szSubsets[] = "ʵ��λ������\tλ������\t";
   PEvset (hPE, PEP_szaSUBSETLABELS, szSubsets, nLineCount);

   // Set Various Other Properties,������ʾ���� //
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
  
   //set line's color,������ʾ���ߵ���ɫ//
   DWORD col = RGB(255,0,0);
   PEvsetcell(hPE, PEP_dwaSUBSETCOLORS, 0, &col);   
   
   col = RGB(0, 0, 255);
   PEvsetcell(hPE, PEP_dwaSUBSETCOLORS, 1, &col); 
   
   //������������ 
   PEnset(hPE,PEP_dwWDESKCOLOR,col);
   PEnset(hPE, PEP_nGRADIENTBARS, 8);
   PEnset(hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
   PEnset(hPE, PEP_bMAINTITLEBOLD, TRUE);//������Ϊ����
   PEnset(hPE, PEP_bSUBTITLEBOLD, TRUE);//������Ϊ����
   PEnset(hPE, PEP_bLABELBOLD, TRUE);   //������Ϊ����  
   PEnset(hPE, PEP_nFONTSIZE, PEFS_SMALL);//����ΪС���� 
  
 //���ݲ����ػ�
   PEreinitialize(hPE);
   PEresetimage(hPE, 0, 0);  
   m_hPE[nWndNo]=hPE;
}

/****************************************************************************/
/*  ������: SetChartPara       												*/
/*  ��  �ܣ����ø���ͼ�ؼ�����												*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
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
	char * chYLabel[3]={"λ�� (pulse)","�ٶ� (pulse/ST)","���ٶ� (pulse/ST^2)"};
	char * chTitle[3]={"λ�ƣ�ʱ������ͼ","�ٶȣ�ʱ������ͼ","���ٶȣ�ʱ������ͼ"};	
	
    //��ȡ��ͼ�ؼ����ڸ߶�ֵ�����Ӵ���1/3�߶�
	pMainFrm=(CMainFrame*) AfxGetApp()->m_pMainWnd;    
	pMainFrm->GetClientRect( &rectClient );
	pMainFrm->m_pWndFunctionBar->GetWindowRect(&rectBar);
	rectHeight=(rectClient.bottom-(rectBar.bottom-rectBar.top)-50)/3;	
    
	for(int i=0;i<3;i++)
	{
		//���ø���ͼ�ؼ�����
		rectChart[i].left =rectClient.left+5;
		rectChart[i].right=(rectClient.right-rectClient.left)*7/10+20;	
		rectChart[i].bottom=(rectClient.bottom-(rectBar.bottom-rectBar.top)-35)*(i+1)/3;
		rectChart[i].top=rectChart[i].bottom-rectHeight;
		
		chartPara[i].chTitle=chTitle[i];
		chartPara[i].YLabel=chYLabel[i];
		chartPara[i].XLabel="ʱ�䣨�룩";
		chartPara[i].ChartRect=rectChart[i];
		chartPara[i].nID=1001+i;
		
		chartPara[i].fXMin=0.0f;
		chartPara[i].fXMax=m_fIntervalOfPage;
		chartPara[i].fYMin=0.0f;
		chartPara[i].fYMax=20000.0f;
		
		chartPara[i].nLineCount=1;	
		chartPara[i].nPointCount=100*m_fIntervalOfPage*1000/INTERVAL_VALUE;
		
		//��ʼ���ؼ�
		InitChart(i,chartPara[i]);
	}
}
#endif

void CACMotorCtrlWnd::OnRadioAxis1() 
{
	m_nAxis=0;
	UpdateData(FALSE);
	InterfaceHandleWithAxisNO("1��");
}

void CACMotorCtrlWnd::OnRadioAxis2() 
{
	m_nAxis=1;
	UpdateData(FALSE);
	InterfaceHandleWithAxisNO("2��");
}

void CACMotorCtrlWnd::OnRadioAxis3() 
{
	m_nAxis=2;
	UpdateData(FALSE);

	InterfaceHandleWithAxisNO("3��");
}

void CACMotorCtrlWnd::OnRadioAxis4() 
{
	m_nAxis=3;
	UpdateData(FALSE);

	InterfaceHandleWithAxisNO("4��");
}

void CACMotorCtrlWnd::OnSelchangeAcCtrlmode() 
{
	UpdateData(TRUE);
	#if 0
	int ctrlmode=((CComboBox*)GetDlgItem(IDC_AC_CTRLMODE))->GetCurSel();
	
	if(ctrlmode==0)
	{//ģ���ѹ 
		if (m_nCardType==SG_CARD)
		{
			MessageBox("��ʹ�õ���SG������������Ϊģ���ѹ��ʽ","XX�Ƽ�",MB_ICONINFORMATION);
			return;
		}
		GetDlgItem(IDC_SETKP)->EnableWindow(true);
		GetDlgItem(IDC_SETKI)->EnableWindow(true);
		GetDlgItem(IDC_SETKD)->EnableWindow(true);
		MessageBox("�����ʹ�õ��ǽ����ŷ����\n��ȷ�ϵ��������������Ϊ�ٶȿ���ģʽ","XX�Ƽ�",MB_ICONINFORMATION);	
	}
	else
	{//����
		GetDlgItem(IDC_SETKP)->EnableWindow(false);
		GetDlgItem(IDC_SETKI)->EnableWindow(false);
		GetDlgItem(IDC_SETKD)->EnableWindow(false);
		MessageBox("�����ʹ�õ��ǽ����ŷ����\n��ȷ�ϵ��������������Ϊλ�ÿ���ģʽ!","XX�Ƽ�",MB_ICONINFORMATION);	

	}	
       #endif
}

void CACMotorCtrlWnd::OnAcAxison() 
{
    #if 0
	short rtn;
	unsigned short  nCtrlMode;	//����ģʽ
	int nPulseMode;   //���巽ʽ
	
    UpdateData(TRUE);
	
	nCtrlMode=((CComboBox *) (GetDlgItem(IDC_AC_CTRLMODE)))->GetCurSel();
	nPulseMode = ((CMotorControlBenchApp *)AfxGetApp())->m_nDirPls[m_nAxis];
	
	rtn=GT_Axis(m_nAxis+1);
	rtn=GT_ClrSts();	
	if (m_nCardType==SV_CARD) {	//��ǰΪSV��
		
		//���ݿ��Ʒ�ʽ���ò���
		switch(nCtrlMode)
		{
		case SERVO_MOTOR://ģ���ѹ
			//��������
			if(!CheckPara()) return;			
			if (m_nKp>50) 
			{
				if(	IDNO==MessageBox("�����õ�Kp����50���п��������������ȷ��Ҫ������","PID����",MB_ICONQUESTION|MB_YESNO))
					return;
			}				
			//���ÿ��Ʒ�ʽ��PID	
			rtn=GT_ClrSts();
			Sleep(20);
			rtn = GT_CtrlMode(0);  //Error(rtn,"GT_CtrlMode");			
			rtn = GT_SetKp(m_nKp); // Error(rtn,"SetKp");
			rtn = GT_SetKi(m_nKi); // Error(rtn,"SetKi");
			rtn = GT_SetKd(m_nKd); // Error(rtn,"SetKd");							
			break;
		case STEP_MOTOR: //������	
			rtn=GT_ClrSts();
			Sleep(20);
			rtn = GT_CtrlMode(1);  // Error(rtn,"GT_CtrlMode");	
			
			//�������巽ʽ
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
				AfxMessageBox("��ѡ���������������+��ʽģʽ");
				return;
				break;
			}	
			break;
		}			
	}
	else  //��ǰΪSG��
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
			AfxMessageBox("��ѡ���������������+��ʽģʽ");
			return;
			break;
		}				
	}
	rtn=GT_Update();
	rtn=GT_AxisOn();	
	
	if(m_bGearModeSet) //���ӳ��ַ�ʽ
	{
		GT_Axis(m_nAuxAxisNo);//�����Ӷ���
		GT_AxisOn();
	//	GT_Axis(m_nAxis+1);//����������
	//	GT_AxisOn();
	}	
	m_bAxisOn=true;
	m_bParaIsChanged=false;
	MessageBox("���Ѿ���������һ����������С���ť","XX�Ƽ�",MB_ICONINFORMATION);
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
	MessageBox("���ѹر�","XX�Ƽ�");	
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
    CWnd *pwnd;                         //窗体基类


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

	int iAxisNum;			//���
	int nCardType;			//�忨����
	int nCurMode;
	CString str;	
    int i=1;  

	CMotorControlBenchApp * pApp;
	pApp=(CMotorControlBenchApp *)AfxGetApp();
	nCardType=pApp->m_nCardType;
	iAxisNum=m_nAxis+1;
	m_bCanDraw=false;
	
   //����״̬���
	if(!m_bAxisOn)
	{
		MessageBox("���ȿ����ᣡ","XX�Ƽ�",MB_ICONINFORMATION);
		return;
	} 
	if (m_bParaIsChanged) {
		MessageBox("PID���������˱仯.\n���ȵ���������ᡱ��ť�����µ�PID����","",MB_ICONINFORMATION);
	    return;
	}
	for(i=1;i<5;i++)
	{		
		if(gtcard.IsAxisMoving(i))
		{
			MessageBox("�������˶�������˶�ֹͣ�������У�","��ʾ",MB_ICONINFORMATION|MB_OK);
			return;
		}
	}  
	 
	//�忨���������ݲɼ�//
	UpdateData(TRUE);  	
	if (nCardType==SV_CARD) {		
		
		if(m_nShowDataTYpe==PROFILE_VALUE) //���ݲɼ�����Ϊ�滮ֵ
		{
			rtn = GT_AddWatch(m_nAxis+1,AXIS_PROFILE_POS,atlPos);
			if (rtn==-1) {
				MessageBox("����ʹ�õİ忨�汾��֧�����ݻ���ɼ����ܣ��޷�ʵ�ֻ�ͼ���ܣ�\n������Ҫ��ͼ���ܣ�����XX��˾��ϵ��","XX�Ƽ�",MB_ICONINFORMATION|MB_OK);
			}
			rtn = GT_AddWatch(m_nAxis+1,AXIS_PROFILE_VEL,atlVel);
			if (rtn==-1) {
				MessageBox("����ʹ�õİ忨�汾��֧�����ݻ���ɼ����ܣ��޷�ʵ�ֻ�ͼ���ܣ�\n������Ҫ��ͼ���ܣ�����XX��˾��ϵ��","XX�Ƽ�",MB_ICONINFORMATION|MB_OK);
			}
			
		}
		else  //���ݲɼ�����Ϊʵ��ֵ
		{
			rtn = GT_AddWatch(m_nAxis+1,AXIS_ACTUAL_POS,atlPos);
			rtn = GT_AddWatch(m_nAxis+1,AXIS_ACTUAL_VEL,atlVel);
		}
		
		rtn = GT_EndWatch(BUFFER_SIZE);		
		if (rtn==-1)
		{
			m_bCanDrawWithBuffer=false;
			int res=MessageBox("����ʹ�õİ忨�汾��֧�����ݻ���ɼ����ܣ��޷�ʵ�ֻ�ͼ���ܣ�\n������Ҫ��ͼ���ܣ�����XX��˾��ϵ��\n ����������ѡ��YES�� ","XX�Ƽ�",MB_ICONINFORMATION|MB_YESNO);
			if (res==IDNO) return;
		}
	}

	
	//�ػ�������ʾ��//
	OnBtnClear();

	//����λ������//
    for(i=1;i<5;i++)
	{
		rtn=GT_Axis(i);			
		rtn=GT_ZeroPos();
		Sleep(20);
		rtn=GT_Update();
	}     
 
	//�˶���������
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
		//���ٶ�//
		if (!VelMotion(iAxisNum)) return; 
		if (m_bCanDrawWithBuffer) 
		{
			rtn = GT_StartWatch(SAMPLE_INTERVAL);
		}
	//	Sleep(10);
	//	rtn=GT_Update();		
	   break;
	case G_MODE:	
	   	MessageBox("���ӳ�������ģʽ��,���л�������ģʽ�ٵ�����м�","��ʾ",MB_OK);
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
      static int  k=0;   //��ֹ�ظ�����	

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

   //λ������	
	for(int i=1;i<5;i++)
	{
		rtn=GT_Axis(i);   //   Error(rtn,"GT_Axis");
		rtn=GT_ZeroPos();   // Error(rtn,"GT_ZeroPos");
	}
	
      if(k==0)
      {  
        m_Video=cvCreateCameraCapture(-1);      //������ͷ
        if (!m_Video)
        {
            k=0;
            MessageBox("�޷�������ͷ");
            return;
        }
        k=1;
    }
    else
    {
        MessageBox("����ͷ�Ѵ�");
    }
    #endif

    pwnd = GetDlgItem(IDC_SHOW_IMG); 	//根据ID值获取对话框中子窗体控件对象指针,这里是按�?窗体

    pDC =pwnd->GetDC();                //取得客户区显示设备dc
    Disp_hDC1= pDC->GetSafeHdc();      //获取客户区按�?窗体的设备描述DC给Disp_hDC1
    pStaic = (CStatic*)GetDlgItem(IDC_SHOW_IMG);
    pStaic->GetClientRect(&lRect3);    

    stCenter.x = (lRect3.left + lRect3.right)/2;
    stCenter.y = (lRect3.top + lRect3.bottom)/2;

        
    SetTimer(1,100,NULL);
}

void CACMotorCtrlWnd::DrawView(CDC *pDC) 
{
    #if 0
    	 src=cvQueryFrame(m_Video);             //��ȡ����ͷһ֡����
    	cvShowImage("orig", src);
       //DrawToHDC(pDC->GetSafeHdc(), src, lRect) ;   
    #endif
}

void CACMotorCtrlWnd::OnMouseMove2(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    CRect rect_ctr;
    (this->GetDlgItem(IDC_SHOW_IMG))->GetWindowRect(&rect_ctr);//��ȡPicture�ؼ������Ļ���Ͻǵ����꣬
     //�洢��rect_ctr��

    ScreenToClient(rect_ctr);//��ȡPicture�ؼ���ԶԻ���ͻ������Ͻǵ�����
    point.x -=  rect_ctr.left;//point��ȡ���������ԶԻ���ͻ������Ͻǵ����꣬��ȥrect_ctr.left��
    point.y -=  rect_ctr.top;//rect_ctr.top�󣬼�Ϊ������Picture�ؼ����Ͻǵ�����

    //��ȡ״̬���ϵ����֣���������������ʾ��״̬����
    #if 0
    char	szString[100]={0};
    char szText = {0};

    HWND hWnd = ::GetActiveWindow();
    ::GetWindowText(hWnd, szText,200);
    memset(szText+strlen(szText),' ',20);
    #endif
    sprintf(szString, "%s", "");
    
    //������λ��Picture�ؼ�֮�⣬�ڶԻ���״̬���ϲ���ʾ������
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
    
    //�����������ʾ���Ի���״̬����
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

    src=cvQueryFrame(m_Video);             //��ȡ����ͷһ֡����

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
       // cout<<"Բ������x= "<<cvRound(p[0])<<endl<<"Բ������y= "<<cvRound(p[1])<<endl;
       // cout<<"�뾶="<<cvRound(p[2])<<endl; 
       #if 0
        center.x = cvRound(p[0]);
        center.y = cvRound(p[1]);
        radius = cvRound(p[2]);
        cvCircle (src, center, radius, CV_RGB(255,0,0), 1, 8, 0 );
      #endif
        
    }
    #endif
    
    CvPoint pt1 = cvPoint(0, src->height/2),     //顶点坐标rect.x, rect.y转成 CvPoint pt1 格式，后面划线函数需要这种格式，rect.x, rect.y可以看成是一条对角线顶点坐标
    pt2 = cvPoint(src->width, src->height/2); //在这里算出相对于顶点坐标的对角线上另一头坐标�?

    cvLine(src, pt1,  pt2, cvScalar(0,0,255), 1);     //画两坐标间的直线，也就是对角�?

    pt1 = cvPoint(src->width/2, 0);  //顶点坐标rect.x, rect.y转成 CvPoint pt1 格式，后面划线函数需要这种格式，rect.x, rect.y可以看成是一条对角线顶点坐标
    pt2 = cvPoint(src->width/2, src->height);
    cvLine(src,pt1,  pt2, cvScalar(0,0,255), 1);     //画两坐标间的直线，也就是对角�?

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

        pt1 = cvPoint(center.x - m_CcdWidth/2, center.y - m_CcdHeight/2);  //顶点坐标rect.x, rect.y转成 CvPoint pt1 格式，后面划线函数需要这种格式，rect.x, rect.y可以看成是一条对角线顶点坐标
        pt2 = cvPoint(center.x + m_CcdWidth/2, center.y - m_CcdHeight/2);
        cvLine(src,pt1,  pt2, cvScalar(0,255,0), 1);     //画两坐标间的直线，也就是对角�?

        pt1 = cvPoint(center.x - m_CcdWidth/2, center.y - m_CcdHeight/2);  //顶点坐标rect.x, rect.y转成 CvPoint pt1 格式，后面划线函数需要这种格式，rect.x, rect.y可以看成是一条对角线顶点坐标
        pt2 = cvPoint(center.x - m_CcdWidth/2, center.y + m_CcdHeight/2);
        cvLine(src,pt1,  pt2, cvScalar(0,255,0), 1);     //画两坐标间的直线，也就是对角�?

        pt1 = cvPoint(center.x - m_CcdWidth/2, center.y + m_CcdHeight/2);  //顶点坐标rect.x, rect.y转成 CvPoint pt1 格式，后面划线函数需要这种格式，rect.x, rect.y可以看成是一条对角线顶点坐标
        pt2 = cvPoint(center.x + m_CcdWidth/2, center.y + m_CcdHeight/2);
        cvLine(src,pt1,  pt2, cvScalar(0,255,0), 1);     //画两坐标间的直线，也就是对角�?

        pt1 = cvPoint(center.x + m_CcdWidth/2, center.y - m_CcdHeight/2);  //顶点坐标rect.x, rect.y转成 CvPoint pt1 格式，后面划线函数需要这种格式，rect.x, rect.y可以看成是一条对角线顶点坐标
        pt2 = cvPoint(center.x + m_CcdWidth/2, center.y + m_CcdHeight/2);
        cvLine(src,pt1,  pt2, cvScalar(0,255,0), 1);     //画两坐标间的直线，也就是对角�?

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
       src=cvQueryFrame(m_Video);             //��ȡ����ͷһ֡����

        CvPoint pt1 = cvPoint(0, src->height/2),     //顶点坐标rect.x, rect.y转成 CvPoint pt1 格式，后面划线函数需要这种格式，rect.x, rect.y可以看成是一条对角线顶点坐标
        pt2 = cvPoint(src->width, src->height/2); //在这里算出相对于顶点坐标的对角线上另一头坐标�?

       cvLine(src, pt1,  pt2, cvScalar(0,0,255), 1);     //画两坐标间的直线，也就是对角�?

       pt1 = cvPoint(src->width/2, 0);  //顶点坐标rect.x, rect.y转成 CvPoint pt1 格式，后面划线函数需要这种格式，rect.x, rect.y可以看成是一条对角线顶点坐标
        pt2 = cvPoint(src->width/2, src->height);
       cvLine(src,pt1,  pt2, cvScalar(0,0,255), 1);     //画两坐标间的直线，也就是对角�?

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
/*  ������: SetChartScale      												*/
/*  ��  �ܣ����û�ͼ�ؼ�����̶�											*/
/*  ��  ���� 																*/
/*         1. UINT nChartNo   �ؼ����										*/
/*         2. UINT nAxisTitle ��̶����÷�ʽ  0=X���ֶ��̶�  1=Y���ֶ��̶�  */
/*											  2=X���Զ��̶�  3=Y���Զ��̶�  */
/*         3. double fMin     �ֶ��̶���Сֵ								*/
/*         4. double fMax     �ֶ��̶����ֵ								*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::SetChartScale(UINT nChartNo,UINT nAxisTitle, double fMin, double fMax)
{
	if(nChartNo>2||nChartNo<0)
		return;

	switch(nAxisTitle) {
	case 0://X���ֶ��̶� 
		PEnset(m_hPE[nChartNo], PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
		PEvset(m_hPE[nChartNo], PEP_fMANUALMINX, &fMin, 1);
		PEvset(m_hPE[nChartNo], PEP_fMANUALMAXX, &fMax, 1);
		break;
	case 1://Y���ֶ��̶� 
		PEnset(m_hPE[nChartNo], PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
		PEvset(m_hPE[nChartNo], PEP_fMANUALMINY, &fMin, 1);
		PEvset(m_hPE[nChartNo], PEP_fMANUALMAXY, &fMax, 1);
		break;
	case 2://X���Զ��̶� 
		PEnset(m_hPE[nChartNo], PEP_nMANUALSCALECONTROLX, PEMSC_NONE);
		break;
	case 3://Y���Զ��̶�
		 PEnset(m_hPE[nChartNo], PEP_nMANUALSCALECONTROLY, PEMSC_NONE);

	}	
	PEreinitialize(m_hPE[nChartNo]);
	PEresetimage(m_hPE[nChartNo], 0, 0);  
}

/****************************************************************************/
/*  ������: InitCard	      												*/
/*  ��  �ܣ��忨�����ʼ������												*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
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
	
	//��ʼ���忨
	gtcard.InitCard();
	
	//��ȡ�����
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
   
	//��ʼ����
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
			// Clear out default data,���ԭ������ //
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
					//	str.Format("��%dҳ\n",m_DrawPageCount);
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
				//	TRACE("�� %d �Σ�%f\n",count,fActualY[2]);
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
/*  ������: DrawChart_SV      												*/
/*  ��  �ܣ�����ʵʱ����	  												*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*  ˵  ����1.�����˻��������ݱ���Ļ��ƣ�GT_GetWatch()						*/
/*			2.���忨ΪSV�����ô˺���										*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CACMotorCtrlWnd::DrawChart_SV()
{
    #if 0
	short rtn; 
	float fTime;
	float fActualY[3];				//����,����������λ��[0],�ٶ�[1],���ٶ�[2]
	unsigned short nCountOfBuffer;	//��������С��Ϊ0��ʾ��������
	unsigned short status; 			//�忨״̬
	static int IsNotFirstRun(0);	//�ж��Ƿ��һ��	
	static int IsReachTargetVel(0);	//�ж��ٶ��Ƿ�ﵽĿ��ֵ�������ٶ����ݴ���		
	int i = 0;

	//�ӻ�������ȡ���˶�����
	rtn = GT_GetWatch(&nCountOfBuffer);
	rtn = GT_GetSts(&status);
	
	
	if(nCountOfBuffer != 0)			//��������������
	{   
		for(i = 0;i < nCountOfBuffer;++i)
		{	
			//��ȡ��λ�ú��ٶ�����
			fActualY[0]=(float)atlPos[i];
			fActualY[1]=(float)atlVel[i];

			//���ٶ����ݼ��뵽glVelArray�����ڼ�����ٶ�
			glVelArray.Add(atlVel[i]);
			//	TRACE("Pos: No %d %lf\n",i,atlPos[i]);

			//�ٶ�ֵ�������ڰ忨���������ص�ʵ���ٶ�ֵ���ܻ���+-1�������ƫ�
			//�����ڷǼӼ��ٽ׶μ��ٶȼ��㲻��ȷ���������´���
			if(fActualY[1]==m_fTargetVel)  IsReachTargetVel=1;		
			if(IsReachTargetVel&&(m_nShowDataTYpe!=PROFILE_VALUE)) 
			{
				if((fActualY[1]<=m_fTargetVel+1)&&(fActualY[1]>=m_fTargetVel-1))
					fActualY[1]=m_fTargetVel;
				glVelArray.SetAt(count,fActualY[1]);
				//	fActualY[1]=(float)(glVelArray.GetAt(count-2)+glVelArray.GetAt(count-1)+fActualY[1])/3;
			}
			
			//����ʱ��ֵ
			fTime=(float) count*(SAMPLE_INTERVAL+1)*0.0002*1.25;

			//������ٶ�
			if(count==0) fActualY[2]=0; //��ʼʱ�����ٶ�Ϊ0
			else
			{
				fActualY[2]=(float)((glVelArray.GetAt(count)-glVelArray.GetAt(count-1)))/(SAMPLE_INTERVAL+1);
				if(fActualY[2]==(float)0.0) fActualY[2]=(float)0.00001; //Ϊ0ʱ��ͼ�����޷���ʾ���ߣ�����Ϊ0.00001
			}
			
			//��ҳ��ʾ����
			if (fTime>=m_fEndTimeOfPage) {
				m_DrawPageCount++;
				m_CurDrawPage++;			
				m_fEndTimeOfPage = m_fEndTimeOfPage + m_fIntervalOfPage;
				m_fStartTimeOfPage = m_fEndTimeOfPage - m_fIntervalOfPage;
			 //������������̶�
				for(int i=0;i<3;i++)
				{
					PEvset(m_hPE[i], PEP_fMANUALMINX, &m_fStartTimeOfPage, 1);
					PEvset(m_hPE[i], PEP_fMANUALMAXX, &m_fEndTimeOfPage, 1);	
				}
			}
			
			//�������ݱ��浽��Ӧ�Ļ�ͼ�ؼ����ݴ洢��Ԫ
			//λ��
			PEvsetcellEx(m_hPE[0], PEP_faYDATA, 0, count, &fActualY[0]);
			PEvsetcellEx(m_hPE[0], PEP_faXDATA, 0, count, &fTime);
			//�ٶ�
			PEvsetcellEx(m_hPE[1], PEP_faYDATA, 0, count, &fActualY[1]);
			PEvsetcellEx(m_hPE[1], PEP_faXDATA, 0, count, &fTime);	
			//���ٶ�
			PEvsetcellEx(m_hPE[2], PEP_faYDATA, 0, count, &fActualY[2]);
			PEvsetcellEx(m_hPE[2], PEP_faXDATA, 0, count, &fTime);	
			//	TRACE("�� %d �Σ�%f\n",count,fActualY[2]);
			count++;				
		}
		
		//��ͼ
		for( i=0;i<3;i++)
		{	
			PEreinitialize(m_hPE[i]);
			PEresetimage(m_hPE[i], 0, 0);  
			::InvalidateRect(m_hPE[i], NULL, FALSE);
		}
		IsNotFirstRun=1;
	}
	
    //���˶�ֹͣ����
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
		MessageBox("Ki�������ô���\nKi���÷�Χ��0��200","XX�Ƽ�",MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_SETKI)->SetFocus();
		return false;
	}
	if((m_nKd<0)||(m_nKd>200))
	{
		MessageBox("Kd�������ô���\nKd���÷�Χ��0��200","XX�Ƽ�",MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_SETKD)->SetFocus();
		return false;
	}
	if((m_nKp<0)||(m_nKp>200))
	{
		MessageBox("Kp�������ô���\nKp���÷�Χ��0��200","XX�Ƽ�",MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_SETKP)->SetFocus();
		return false;
	}
	if(m_nKp==0)
	{
		MessageBox("Kp�������ô���\nKpֵ��ӦΪ0","XX�Ƽ�",MB_ICONWARNING|MB_OK);
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
/*  ������: InterfaceHandleWithAxisNO										*/
/*  ��  �ܣ����ݵ�ǰ����н�������											*/
/*  ��  ���� const char * AxisName	���									*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
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
/*  ������: BufferSampleInit												*/
/*  ��  �ܣ����������ݲɼ���ʼ������										*/
/*  ��  ���� int dataStyle   ���ݲɼ�����									*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
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
		int res=MessageBox("����ʹ�õİ忨�汾��֧�����ݻ���ɼ����ܣ��޷�ʵ�ֻ�ͼ���ܣ�\n������Ҫ��ͼ���ܣ�����XX��˾��ϵ��\n ����������ѡ��YES�� ","XX�Ƽ�",MB_ICONINFORMATION|MB_YESNO);
		      if (res==IDNO)
			  {
				  return;
			  }
	}
}

/****************************************************************************/
/*  ������: SMotion															*/
/*  ��  �ܣ�S�����˶�														*/
/*  ��  ���� int iAxisNum  ����S�����˶������								*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CACMotorCtrlWnd::SMotion(int iAxisNum)
{
    #if 0
    double fJerk=0.1;		//�Ӽ��ٶ�
	double fAcc;			//���ٶ�
	double fVel;			//�ٶ�
	long   lPos=0;			//Ŀ��λ��
	double fMaxAcc=0.4;		//�����ٶ�
	CString str;
	CEdit *pedit;	

	pedit=(CEdit*)m_tabModeSel.m_tabPages[0]->GetDlgItem(IDC_SMODE_JERK);
	pedit->GetWindowText(str);
	fJerk=atof(str);
	if ((fJerk<=0)||(fJerk>0.01)) {
		MessageBox("�Ӽ��ٶȳ�����Χ!\n���÷�Χ��0~0.01pluse/st^3","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}
	//���ٶ�//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[0]->GetDlgItem(IDC_SMODE_ACC);
	pedit->GetWindowText(str);
	fAcc=atof(str);
	if ((fAcc<=0)||(fAcc>1)){
		MessageBox("���ٶȳ�����Χ��\n���÷�Χ��0~1Pluse/st^2","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}
	//�ٶ�//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[0]->GetDlgItem(IDC_SMODE_VEL);
	pedit->GetWindowText(str);
	fVel=atof(str);
	if ((fVel<=0)||(fVel>50)){
		MessageBox("�ٶȳ�����Χ��\n���÷�Χ��0~50Pluse/st","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}
	//Ŀ��λ��
	pedit=(CEdit*)m_tabModeSel.m_tabPages[0]->GetDlgItem(IDC_SMODE_POS);
	pedit->GetWindowText(str);
	lPos=atol(str);    
	if ((lPos<-1000000)||(lPos>1000000)){
		MessageBox("�յ�λ�ó�����Χ��\n���÷�Χ��-1000000~1000000Pluse","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}
	m_fTargetVel=fVel;
	
	//��ͼ����������
	if(lPos>0)
		SetChartScale(0,1,0,lPos*1.2);
	else
		SetChartScale(0,1,lPos*1.2,0);		
	if(fVel>0)
		SetChartScale(1,1,0,fVel*1.3);
	else
		SetChartScale(1,1,fVel*1.3,0);	
	
	/****************************/
	//��������˶��ٶȲ���ʾ����//
	//2006.12.1//
	if((m_nShowDataTYpe==ACTUAL_VALUE)&&(lPos<0))
		SetChartScale(1,1,-fVel*1.3,0);
	/********************************/
	SetChartScale(2,1,-fAcc*2,fAcc*2);
	
	//��Sģʽ�˶�
	   gtcard.AxisSMotion(iAxisNum,lPos,fVel,fAcc,fJerk);
    #endif
	   return true;
}

/****************************************************************************/
/*  ������: TMotion															*/
/*  ��  �ܣ�S�����˶�														*/
/*  ��  ���� int iAxisNum  ����T�����˶������								*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CACMotorCtrlWnd::TMotion(int iAxisNum)
{	
#if 0
	double fAcc=0.2;			//���ٶ�
	double fVel=3;			//�ٶ�
	long   lPos=0;			//Ŀ��λ��
	double fMaxAcc=0.4;		//�����ٶ�
	CString str;
	CEdit *pedit;	

	//���ٶ�//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[1]->GetDlgItem(IDC_TMODE_ACC);
	pedit->GetWindowText(str);
	fAcc=atof(str);
	if ((fAcc<=0)||(fAcc>1)){
		MessageBox("���ٶȳ�����Χ��\n���÷�Χ��0~1Pluse/st^2","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}
	//����ٶ�//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[1]->GetDlgItem(IDC_TMODE_VEL);
	pedit->GetWindowText(str);
	fVel=atof(str);
	if ((fVel<=0)||(fVel>50)){
		MessageBox("�ٶȳ�����Χ��\n���÷�Χ��0~50Pluse/st","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}
	//Ŀ��λ��//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[1]->GetDlgItem(IDC_TMODE_POS);
	pedit->GetWindowText(str);
	lPos=atol(str);
	if ((lPos<-1000000)||(lPos>1000000)){
		MessageBox("�յ�λ�ó�����Χ��\n���÷�Χ��-1000000~1000000Pluse","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}	
	
	m_fTargetVel=fVel;	
	//��ͼ���̶�����
	if(lPos>0)
		SetChartScale(0,1,0,lPos*1.2);
	else
		SetChartScale(0,1,lPos*1.2,0);
	
	if(fVel>0)
		SetChartScale(1,1,0,fVel*1.3);
	else
		SetChartScale(1,1,fVel*1.3,0);
	/****************************/
	//��������˶��ٶȲ���ʾ����//
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
/*  ������: VelMotion														*/
/*  ��  �ܣ��ٶ�ģʽ�˶�													*/
/*  ��  ���� int iAxisNum  �����ٶ�ģʽ�˶������							*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CACMotorCtrlWnd::VelMotion(int iAxisNum)
{
	double fAcc=0.2;			//���ٶ�
	double fVel=3;			//�ٶ�
	double fMaxAcc=0.4;		//�����ٶ�
	CString str;
	CEdit *pedit;	

	pedit=(CEdit*)m_tabModeSel.m_tabPages[2]->GetDlgItem(IDC_VELMODE_ACC);
	pedit->GetWindowText(str);
	fAcc=atof(str);
	if ((fAcc<=0)||(fAcc>1)){
		MessageBox("���ٶȳ�����Χ��\n���÷�Χ��0~50Pluse/st^2","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}
	//����ٶ�//
	pedit=(CEdit*)m_tabModeSel.m_tabPages[2]->GetDlgItem(IDC_VELMODE_VEL);
	pedit->GetWindowText(str);
	fVel=atof(str);
	if ((fVel<=0)||(fVel>50)){
		MessageBox("�ٶȳ�����Χ��\n���÷�Χ��0~50Pluse/st","XX�Ƽ�",MB_ICONWARNING);
		return false;
	}
	m_fTargetVel=fVel;
	//��ͼ���̶�����//		
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
   
    POSITION pos;
    pos = m_listCtrl.GetFirstSelectedItemPosition();
   
   // �õ���Ŀ����
    IndexInFieldList = m_listCtrl.GetNextSelectedItem(pos);  

    *pResult = 0;
}


#if 0
void CACMotorCtrlWnd::OnButtonXpositive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CFormView::OnTimer(2);

      SetTimer(2,100,NULL);
}

void CACMotorCtrlWnd::OnButtonXpositive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CFormView::OnTimer(2);

      SetTimer(2,100,NULL);
}
void CACMotorCtrlWnd::OnBnDropDownButtonXpositive(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMBCDROPDOWN pDropDown = reinterpret_cast<LPNMBCDROPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CFormView::OnLButtonDown(nFlags, point);
//}


void CACMotorCtrlWnd::OnBnClickedButtonXreverse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	double dist;

      GetJogIncValue(&dist);
	gtcard.Jog(0,dist,1,NEGTIVE_DIRCT);
}




void CACMotorCtrlWnd::OnBnClickedButtonYpositive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	double dist;

      GetJogIncValue(&dist);
	gtcard.Jog(1,dist,1,POSITIVE_DIRCT);
}


void CACMotorCtrlWnd::OnBnClickedButtonYrerverse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	double dist;

      GetJogIncValue(&dist);
	gtcard.Jog(1,dist,1,NEGTIVE_DIRCT);
}


void CACMotorCtrlWnd::OnBnClickedButtonZpositive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	double dist;

       GetJogIncValue(&dist);
	gtcard.Jog(AXIS_4,dist,1,POSITIVE_DIRCT);
}


void CACMotorCtrlWnd::OnBnClickedButtonZrerverse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
      double dist;

      GetJogIncValue(&dist);
	gtcard.Jog(AXIS_4,dist,1,NEGTIVE_DIRCT);
}

void CACMotorCtrlWnd::OnBnClickedCheckLeftIn()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strGCode;
    CStdioFile fs;
    CString strLine;
    
    CString filters;
    CString suffix;
    CString strResult;

    if(Side == NO_SIDE  || Side >= MAX_SIDE_VALUE)
    {
          AfxMessageBox("������ѡ��������ҽ���",MB_OK);
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
        AfxMessageBox("�򿪹�����������",MB_OK);
        return;
    }
    
    if (fs.GetLength()==NULL) 
    {
        AfxMessageBox("ָ�����ļ�����Ϊ�գ�",MB_OK);
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

        /* ������Ҫ�ж�һ�������Ƿ�Ϸ����ļ��Ƿ�Ϸ�*/
        
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

    		// ��ʽ���ַ���
            f1.Format("LB0");
            f2.Format("%d", LB0_ValueX);
            f3.Format("%d", LB0_ValueY);
            nItem =  m_listCtrl.InsertItem(0, _T(f1));  // �����һ��
            m_listCtrl.SetItemText(nItem, 1, _T(f2));  // ����ڶ���
            m_listCtrl.SetItemText(nItem, 2, _T(f3));  // ���������

            m_listCtrl.DeleteItem(1);
            f1.Format("LB1");
            f2.Format("%d", LB1_ValueX);
            f3.Format("%d", LB1_ValueY);
            nItem =  m_listCtrl.InsertItem(1, _T(f1));  // �����һ��
            m_listCtrl.SetItemText(nItem, 1, _T(f2));  // ����ڶ���
            m_listCtrl.SetItemText(nItem, 2, _T(f3));  // ���������
        }
        
        if(m_tabModeSel.GetCurSel() == EXC_TAB || m_tabModeSel.GetCurSel() == TECH_TAB)
        {
            CString f1, f2, f3, f4,f5;

            m_listCtrl.DeleteAllItems();

            for(int i=0; i< PointNum; i++)
            {
                // ��ʽ���ַ���
                f1.Format("L%d", i);
                f2.Format("%d", L_Points[i].PosX);
                f3.Format("%d", L_Points[i].PosY);
                f4.Format("%d", L_Points[i].PosZ);

                m_listCtrl.DeleteItem(i);

                nItem = m_listCtrl.InsertItem(i, _T(f1));  // �����һ��
                m_listCtrl.SetItemText(nItem, 1, _T(f2));  // ����ڶ���
                m_listCtrl.SetItemText(nItem, 2, _T(f3));  // ���������
                m_listCtrl.SetItemText(nItem, 3, _T(f4));  // ���������
                if(L_Points[i].type == "DryRun")
                {
                    m_listCtrl.SetItemText(nItem, 4, L_Points[i].type);  // ���������
                }
                else
                {
                    f5.Format("%d", L_Points[i].CutSpeed);          
                    m_listCtrl.SetItemText(nItem, 4, _T(f5));  // ���������
                }
           }
        }

        if(m_tabModeSel.GetCurSel() == SYS_TAB)
        {
            CString f1, f2, f3, f4;
            m_listCtrl.DeleteItem(0);

    		// ��ʽ���ַ���
            f1.Format("M0");
            f2.Format("%d", L_MPoints[0].PosX);
            f3.Format("%d", L_MPoints[0].PosY);
            nItem =  m_listCtrl.InsertItem(0, _T(f1));  // �����һ��
            m_listCtrl.SetItemText(nItem, 1, _T(f2));  // ����ڶ���
            m_listCtrl.SetItemText(nItem, 2, _T(f3));  // ���������

            m_listCtrl.DeleteItem(1);
            f1.Format("M1");
            f2.Format("%d", L_MPoints[1].PosX);
            f3.Format("%d", L_MPoints[1].PosY);
            nItem =  m_listCtrl.InsertItem(1, _T(f1));  // �����һ��
            m_listCtrl.SetItemText(nItem, 1, _T(f2));  // ����ڶ���
            m_listCtrl.SetItemText(nItem, 2, _T(f3));  // ���������
            
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
    CStdioFile fs;//NC�ļ�
    CString strLine;
    CString strNCFileName;//NC�ļ���		
    CEdit * pGCodeEdit;

    int nItem;
    CString f1;

    CString filters;
    CString suffix1;
    CString suffix2;

    if(Side <= NO_SIDE  || Side >= MAX_SIDE_VALUE)
    {
          AfxMessageBox("������ѡ��������ҽ���",MB_OK);
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
    //�ļ�����
    if(fs.Open(strFilePathName, CFile::modeCreate |CFile::modeWrite)==NULL) 
    {
        AfxMessageBox("�ļ���������",MB_OK);
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
            nItem =  m_listCtrl.InsertItem(0, _T(f1));  // �����һ��
            m_listCtrl.SetItemText(0, 1, "0");  // ����ڶ���
            m_listCtrl.SetItemText(0, 2, "0");  // ���������

            m_listCtrl.DeleteItem(1);
            f1.Format("LB1");
            nItem =  m_listCtrl.InsertItem(1, _T(f1));  // �����һ��
            m_listCtrl.SetItemText(1, 1, "0");  // ����ڶ���
            m_listCtrl.SetItemText(1, 2, "0");  // ���������
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
        nItem =  m_listCtrl.InsertItem(0, _T(f1));  // �����һ��
        m_listCtrl.SetItemText(0, 1, "0");  // ����ڶ���
        m_listCtrl.SetItemText(0, 2, "0");  // ���������

        m_listCtrl.DeleteItem(1);

        f1.Format("RB1");
        nItem =  m_listCtrl.InsertItem(1, _T(f1));  // �����һ��
        m_listCtrl.SetItemText(1, 1, "0");  // ����ڶ���
        m_listCtrl.SetItemText(1, 2, "0");  // ���������
    }

    strLine.Format("M0 0  0  0                   \n",strBuffer);
    fs.WriteString(strLine);		
    strLine.Format("M1 0  0  0                   \n",strBuffer);
    fs.WriteString(strLine);	

    if(m_tabModeSel.GetCurSel() == SYS_TAB)
    {
        m_listCtrl.DeleteItem(0);

        f1.Format("M0");
        nItem =  m_listCtrl.InsertItem(0, _T(f1));  // �����һ��
        m_listCtrl.SetItemText(0, 1, "0");  // ����ڶ���
        m_listCtrl.SetItemText(0, 2, "0");  // ���������
        m_listCtrl.SetItemText(0, 3, "0");  // ���������

        m_listCtrl.DeleteItem(1);
        f1.Format("M1");
        nItem =  m_listCtrl.InsertItem(1, _T(f1));  // �����һ��
        m_listCtrl.SetItemText(1, 1, "0");  // ����ڶ���
        m_listCtrl.SetItemText(1, 2, "0");  // ���������           
        m_listCtrl.SetItemText(1, 3, "0");  // ���������
    }

    fs.Close();	
    
#if 0
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(strNCFileName);
	bShouldSave=false;
#endif
}





void CACMotorCtrlWnd::OnEnChangeEditDiameter()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_EDIT_DIAMETER)->GetWindowText(str);
	m_Diameter = atol(str);
}



void CACMotorCtrlWnd::OnDeltaposSpinDiameter(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

    if(pNMUpDown->iDelta == -1)  // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
    {
        m_Diameter++;
    }
    else if(pNMUpDown->iDelta == 1)  // �����ֵΪ, ˵�������Spin�����¼�ͷ
    {
        m_Diameter--;
    }
    
    CString ss;
    ss.Format(_T("%d "), m_Diameter);
    GetDlgItem(IDC_EDIT_DIAMETER)->SetWindowText(ss);   //EDIT�ؼ���ʾ����
    
    *pResult = 0;
    
}



void CACMotorCtrlWnd::OnBnClickedButtonModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    int nItem;
    long pnt[4] = {0};

    CString f1,f2,f3;
    CString strLine, strLineTmp;
    CStdioFile fs;
    int ret = 0;

   if(strFilePathName.IsEmpty())
   {
         AfxMessageBox("���ȴ򿪹���������",MB_OK);
         return;
   }
    
    /* �����ڵ�����*/
    if(fs.Open(strFilePathName, CFile::modeReadWrite | CFile::typeText)==NULL) 
    {
        AfxMessageBox("�򿪹�����������",MB_OK);
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

                /* �����б��� */
                f1.Format("%d", pnt[0]);
                f2.Format("%d", pnt[1]);
                f3.Format("%d", pnt[2]);
                m_listCtrl.SetItemText(0, 1, _T(f1));  // ����ڶ���
                m_listCtrl.SetItemText(0, 2, _T(f2));  // ���������
                m_listCtrl.SetItemText(0, 3, _T(f3));  // ���������	
            
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

                /* �����б��� */
                f1.Format("%d", pnt[0]);
                f2.Format("%d", pnt[1]);
                f3.Format("%d", pnt[2]);
                m_listCtrl.SetItemText(1, 1, _T(f1));  // ����ڶ���
                m_listCtrl.SetItemText(1, 2, _T(f2));  // ���������
                m_listCtrl.SetItemText(1, 3, _T(f3));  // ���������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CButton* but = (CButton*)GetDlgItem(IDC_CHECK_RIGHT_PROGRAM);

    if (but->GetCheck())
    {
        Side = RIGHT_SIDE;
        ((CButton*)GetDlgItem(IDC_CHECK_LEFT_PROGRAM))->SetCheck(FALSE); 
    }

}

void CACMotorCtrlWnd::OnBnClickedCcdLightOnoff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gtcard.HomeAll(HOME_TWOLIMITS);

}

