// DCMotrCtrlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MotorControlBench.h"
#include "DCMotrCtrlWnd.h"
#include "MainFrm.h"
#include "Pegrpapi.h"
#include "DigitalDisp.h"

#include "GTCard.h"
#include "GT400.h"
#include "Global.h"

#include <math.h>


#include "cv.h"
#include "highgui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GetRandom(min,max) ((rand()%(int)(((max)+1)-(min)))+(min))
#define INTERVAL_VALUE	50  //定义显示刷新时间间隔

typedef enum {
#define	ZoomAndShowTheroy 0
#define	ZoomAndNotShowTheroy 1
#define	NotZoomAndShowTheroy  2
#define	NotZoomANdNotShowTheroy 3
}ShowType;


extern CGTCard gtcard;

#if 0
CStatic *pStaic;
CvCapture*  m_Video; 
long        Smooth_Step=5;     //滑块1的平滑滤波微调参数值，默认为3
IplImage*   src=0;             //opencv IplImage类图像指针用来存放和处理截取到的图像
IplImage*   gray = 0;          //存放灰度图像。
CRect lRect3;   
CWnd *pwnd;                         //绐椾綋鍩虹被
CDC  *pDC;   
//HDC Disp_hDC1;                 //璁惧鍙ユ焺DC 
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCMotrCtrlWnd

IMPLEMENT_DYNCREATE(CDCMotrCtrlWnd, CFormView)

CDCMotrCtrlWnd::CDCMotrCtrlWnd()
	: CFormView(CDCMotrCtrlWnd::IDD)
{
	//{{AFX_DATA_INIT(CDCMotrCtrlWnd)
	m_TargetVal =80.0;
	m_nKp =1.0;
	m_nKi = 0;
	m_nKd = 0;
	//}}AFX_DATA_INIT
	m_nAxisNo=1;
	m_nRealTimeCounter=0;
	m_hPE=NULL;
	m_fManualMaxX=5.0f;
	m_fMaxY=0.0f;
	m_nLineCount=1;
	m_fTr=0.0f;
}

CDCMotrCtrlWnd::~CDCMotrCtrlWnd()
{
	
}

void CDCMotrCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDCMotrCtrlWnd)
	DDX_Text(pDX, IDC_TARGET_VALUE, m_TargetVal);
	DDV_MinMaxDouble(pDX, m_TargetVal, 0., 500000.);
	DDX_Text(pDX, IDC_EDIT_DC_KP, m_nKp);
	DDV_MinMaxUInt(pDX, m_nKp, 0, 200);
	DDX_Text(pDX, IDC_EDIT_DC_KI, m_nKi);
	DDV_MinMaxUInt(pDX, m_nKi, 0, 200);
	DDX_Text(pDX, IDC_EDIT_DC_KD, m_nKd);
	DDV_MinMaxUInt(pDX, m_nKd, 0, 200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDCMotrCtrlWnd, CFormView)
	//{{AFX_MSG_MAP(CDCMotrCtrlWnd)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CHART_STOP, OnBtnChartStop)
	ON_BN_CLICKED(IDC_BTN_CHART_START, OnBtnChartStart)
	ON_BN_CLICKED(IDC_BTN_CHART_CALCULATE, OnBtnChartCalculate)
	ON_BN_CLICKED(IDC_BTN_CHART_RESTORE, OnBtnChartRestore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCMotrCtrlWnd diagnostics

#ifdef _DEBUG
void CDCMotrCtrlWnd::AssertValid() const
{
	CFormView::AssertValid();
}

void CDCMotrCtrlWnd::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCMotrCtrlWnd message handlers

BOOL CDCMotrCtrlWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CDCMotrCtrlWnd::OnInitialUpdate() 
{
	CString str;
	int nAxisCount;
	unsigned int nKp,nKi,nKd;
	
	InitCard();
    //InitialChart();
	
	nAxisCount=((CMotorControlBenchApp *)AfxGetApp())->m_nAxisCount;
	CString strAxis[4]={"1轴","2轴","3轴","4轴"};

	CComboBox * pCmbMotorSel;
	pCmbMotorSel=(CComboBox *) GetDlgItem(IDC_CMB_MOTORSEL);
	
	pCmbMotorSel->ResetContent();	
	for(int i=0;i<nAxisCount;i++)
	{		
		pCmbMotorSel->InsertString(i,strAxis[i]);
	}
	pCmbMotorSel->SetCurSel(0);

	nKp = ((CMotorControlBenchApp*)AfxGetApp())->m_nKp[m_nAxisNo-1];
	nKi = ((CMotorControlBenchApp*)AfxGetApp())->m_nKi[m_nAxisNo-1];
	nKd = ((CMotorControlBenchApp*)AfxGetApp())->m_nKd[m_nAxisNo-1];
	SetPID(nKp,nKi,nKd);	
	GetDlgItem(IDC_TARGET_VALUE)->SetWindowText("10000");
	str.Format("%d",nKp);
	GetDlgItem(IDC_EDIT_DC_KP)->SetWindowText(str);
	str.Format("%d",nKi);
	GetDlgItem(IDC_EDIT_DC_KI)->SetWindowText(str);
    str.Format("%d",nKd);
	GetDlgItem(IDC_EDIT_DC_KD)->SetWindowText(str);
	GetDlgItem(IDC_MANUALXMAX)->SetWindowText("5.0");	

}

void CDCMotrCtrlWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::CalcWindowRect(lpClientRect, nAdjustType);
}

void CDCMotrCtrlWnd::InitialChart()
{
 	if(m_hPE)
	{
		PEdestroy(m_hPE);
		Invalidate();
	}

   //! Chart fills 100 points but x axis is initially
   //! manually scaled.  Once 100 point have been passed,
   //! the chart switches to autoscaling the x axis.

/*****************************************/
//defien the chart rect's size 定义图形显示区域矩形大小//
   RECT rectClient,rectBar,rect;  
   CMainFrame * pMainFrm;
   pMainFrm=(CMainFrame*) AfxGetApp()->m_pMainWnd;    
   pMainFrm->GetClientRect( &rectClient );
   pMainFrm->m_pWndFunctionBar->GetWindowRect(&rectBar);
   rect=rectClient;
   rect.bottom=rectClient.bottom-(rectBar.bottom-rectBar.top)-20;
   rect.right=rectClient.right*4/5;
 /******************************************/  
  
   m_hPE = PEcreate(PECONTROL_SGRAPH, WS_VISIBLE, &rect, m_hWnd, 1001); 
   // Construct Object ,创建显示窗口//
  
   PEnset(m_hPE, PEP_nSUBSETS, 1);//set subset's number,设置显示曲线数
  
   PEnset(m_hPE, PEP_nPOINTS, 5*1000/INTERVAL_VALUE); //set point count,设置单屏显示点数

 /**********************************************/ 
   // Set Manual Y scale ,设置Ｙ轴手动刻度//   
   PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
   double arg = 1.0F;
   PEvset(m_hPE, PEP_fMANUALMINY, &arg, 1);
   arg = 100.0F;
   PEvset(m_hPE, PEP_fMANUALMAXY, &arg, 1);
 /**********************************************/ 
  
 //  PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_NONE);//设置为自动方式

   // Set Manual X scale,设置Ｘ轴手动刻度 //
   PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
   arg = 0.0F;
   PEvset(m_hPE, PEP_fMANUALMINX, &arg, 1);
   arg = 5.0f;
   PEvset(m_hPE, PEP_fMANUALMAXX, &arg, 1);

 
/**********************************************/ 
     // Clear out default data,清空原有数据 //
   float val = 0;
   PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faXDATA, 1, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 1, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 1, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 1, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faXDATA, 2, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 2, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 2, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 2, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faYDATA, 2, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 2, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 2, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 2, 3, &val);
/**********************************************/ 

   
/**********************************************/ 
   //set the line type,设置显示曲线的线型//
   int nTmpStyle;
   // nTmpStyle = PELT_THINSOLID;
   // nTmpStyle =PELT_DASH;
   nTmpStyle=PELT_MEDIUMTHINSOLID;
   PEvsetcell(m_hPE, PEP_naSUBSETLINETYPES, 0, &nTmpStyle); 
   PEvsetcell(m_hPE, PEP_naSUBSETLINETYPES, 1, &nTmpStyle); 
   
  //set line title,设置曲线名称//   
   char szSubsets[] = "理论位置曲线\t实际位置曲线\t";
   PEvset (m_hPE, PEP_szaSUBSETLABELS, szSubsets, 2);
/**********************************************/ 


/**********************************************/     
   // Set Various Other Properties,设置显示参数 //
   PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
  // PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_MEDIUM_INSET);
    PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_LIGHT_SHADOW);
   
   PEszset(m_hPE, PEP_szMAINTITLE, "直流伺服电机位置实时曲线图");   
   PEszset(m_hPE, PEP_szSUBTITLE, "XX科技");
   PEszset(m_hPE, PEP_szXAXISLABEL, "时间　（秒）");
   PEszset(m_hPE, PEP_szYAXISLABEL, "位置");
  
   PEnset(m_hPE, PEP_bNORANDOMPOINTSTOEXPORT, TRUE);
   PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
   PEnset(m_hPE, PEP_bALLOWBAR, FALSE);
   PEnset(m_hPE, PEP_bALLOWPOPUP, FALSE);
   PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
   PEnset(m_hPE, PEP_bCACHEBMP, TRUE);
   PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
  /**********************************************/  

 
 /**********************************************/  
   //set line's color,设置显示曲线的颜色//
   DWORD col = RGB(0,0,198);
   PEvsetcell(m_hPE, PEP_dwaSUBSETCOLORS, 0, &col);   
   
   col = RGB(255, 0, 0);
   PEvsetcell(m_hPE, PEP_dwaSUBSETCOLORS, 1, &col); 
   
/**********************************************/
   PEnset(m_hPE,PEP_dwWDESKCOLOR,col);
   PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
   PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
   PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);//主标题为粗体
   PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);//副标题为粗体
   PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
   PEnset(m_hPE, PEP_bLINESHADOWS, TRUE);//线条带阴影
   PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);//设置为大字体
 /**********************************************/

   PEreinitialize(m_hPE);
   PEresetimage(m_hPE, 0, 0);
}

#if 0
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
	
    // if there is something wrong with these system calls, we cannot display image...
    if (GetBitmapData( hDCDst, &size, &channels, &dst_ptr ))
   {
	AfxMessageBox("GetBitmapData error!");
		return;
   }

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
	//StretchBlt(hDCDst,0,0,lRect.Width(), lRect.Height(),menDC,0,0,arr->width,arr->height, SRCCOPY);
		
	BitBlt(hDCDst,0,0,arr->width,arr->height,menDC,0,0,SRCCOPY);

	DeleteDC(menDC);

}
#endif

void CDCMotrCtrlWnd::OnTimer(UINT nIDEvent) 
{
    #if 0
	float fActualY,fTheroyY;
	double fActualVel;
	long  lActPos;
	float fTime,fIncTime;

	GT_Axis(m_nAxisNo);
	GT_GetAtlVel(&fActualVel);
	GT_GetAtlPos(&lActPos);

	fActualY=(float) lActPos;
//	Sleep(10);
	switch(m_nShowType) {
	case ZoomAndShowTheroy :
        //Update new actual data at current index,添加实际点//
    	fTheroyY=m_TargetVal;
    	fTime = float(m_nRealTimeCounter)*INTERVAL_VALUE/1000;		
		PEvsetcellEx(m_hPE, PEP_faYDATA, 0, m_nRealTimeCounter, &fTheroyY);
		PEvsetcellEx(m_hPE, PEP_faXDATA, 0, m_nRealTimeCounter, &fTime);
		
	    // Update new theoretic data at current index,添加理论点//
    	fTime = float(m_nRealTimeCounter)*INTERVAL_VALUE/1000;		
		PEvsetcellEx(m_hPE, PEP_faYDATA, 1, m_nRealTimeCounter, &fActualY);
		PEvsetcellEx(m_hPE, PEP_faXDATA, 1, m_nRealTimeCounter, &fTime);
		
		//get the max Y value in order to calcualte Mp//
        m_fMaxY=m_fMaxY>fActualY ? m_fMaxY:fActualY;

		//get the time (Tr)when the Y reach target value//
		
		if((m_fLastYValue<m_TargetVal)&&(fActualY>m_TargetVal)&&((m_fTr-0.0)<0.001))
		{
			//线性插值计算到达目标值的时间
			fIncTime=(INTERVAL_VALUE/1000)*(m_TargetVal-m_fLastYValue)/(fActualY-m_fLastYValue);
		    m_fTr=fTime+fIncTime;
		}

		// Reset counter at end of data //
		m_nRealTimeCounter = m_nRealTimeCounter + 1;
		
	   // Switch to AutoScaling x axis after receiving 1000*5/INTERVAL_VALUE data points //
		if (m_nRealTimeCounter ==1000*5/INTERVAL_VALUE)
		{
			PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_NONE);
		}	
		
		//readjust Y Scale if the value out of the range//
		if(fActualY>1.3*m_TargetVal)
		{
			double arg;
			arg=2.0*m_TargetVal;
			 PEvset(m_hPE, PEP_fMANUALMAXY, &arg, 1);
		}
		
		// SinCounter is only to produce sin wave data //	
		break;
	
	case NotZoomAndShowTheroy:		
		
		fTime = float(m_nRealTimeCounter)*INTERVAL_VALUE/1000;	
		fTheroyY=m_TargetVal;
				
		// store new theoretic data at current index,添加理论点//
		PEvsetcellEx(m_hPE, PEP_faYDATA, 0, m_nRealTimeCounter+ZoomStart, &fTheroyY);
		PEvsetcellEx(m_hPE, PEP_faXDATA, 0, m_nRealTimeCounter+ZoomStart, &fTime);
		
		// store new actual data at current index,添加实际点//	
		PEvsetcellEx(m_hPE, PEP_faYDATA, 1, m_nRealTimeCounter+ZoomStart, &fActualY);
     	PEvsetcellEx(m_hPE, PEP_faXDATA, 1, m_nRealTimeCounter+ZoomStart, &fTime);
		
		// Increment index counter //
	    m_nRealTimeCounter+=1;		
	
		//get the max Y value in order to calcualte Mp//
        m_fMaxY=m_fMaxY>fActualY ? m_fMaxY:fActualY;
		
		//get the time (Tr)when the Y reach target value//		
		if((m_fLastYValue<m_TargetVal)&&(fActualY>m_TargetVal)&&((m_fTr-0.0)<0.001))
		{
			//线性插值计算到达目标值的时间
			fIncTime=(INTERVAL_VALUE/1000)*(m_TargetVal-m_fLastYValue)/(fActualY-m_fLastYValue);
			m_fTr=fTime+fIncTime;
		}	

		if(fActualY>1.3*m_TargetVal)
		{
			double arg;
			arg=2.0*m_TargetVal;
			PEvset(m_hPE, PEP_fMANUALMAXY, &arg, 1);
		}

		// If current time is beyond zoom max, the re-adjust zoom max //
		if (m_nRealTimeCounter==ZoomEnd*1000/INTERVAL_VALUE) {
			ZoomEnd = ZoomEnd + ZoomInterval;
			ZoomStart = ZoomEnd - ZoomInterval;		
			PEvset(m_hPE, PEP_fMANUALMINX, &ZoomStart, 1);
			PEvset(m_hPE, PEP_fMANUALMAXX, &ZoomEnd, 1);
	
		}	
       break;	
		
	case ZoomAndNotShowTheroy:
		fTime = (float)m_nRealTimeCounter*INTERVAL_VALUE/1000; 
		
		// Switch to AutoScaling x axis after receiving 1000*5/INTERVAL_VALUE data points //
		if (m_nRealTimeCounter == 5*1000/INTERVAL_VALUE)
			PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_NONE);	
		
		// Append new values to chart  //
		PEvset(m_hPE, PEP_faAPPENDYDATA, &fActualY, 1);
		PEvset(m_hPE, PEP_faAPPENDXDATA, &fTime, 1); 
		
		//get the max Y value in order to calcualte Mp//
        m_fMaxY=m_fMaxY>fActualY ? m_fMaxY:fActualY;
		
		//get the time (Tr)when the Y reach target value//		
		if((m_fLastYValue<m_TargetVal)&&(fActualY>m_TargetVal)&&((m_fTr-0.0)<0.001))
		{
			//线性插值计算到达目标值的时间
			fIncTime=(INTERVAL_VALUE/1000)*(m_TargetVal-m_fLastYValue)/(fActualY-m_fLastYValue);
			m_fTr=fTime+fIncTime;
		}

		if(fActualY>1.3*m_TargetVal)
		{
			double arg;
			arg=2.0*m_TargetVal;
			PEvset(m_hPE, PEP_fMANUALMAXY, &arg, 1);
		}

		// Increment index counter //
		m_nRealTimeCounter = m_nRealTimeCounter + 1;	
	
		break;
		
	case NotZoomANdNotShowTheroy:
		fTime = (float)m_nRealTimeCounter*INTERVAL_VALUE/1000; 
	
		// Switch to AutoScaling x axis after receiving 1000*5/INTERVAL_VALUE data points //
		if (m_nRealTimeCounter==ZoomEnd*1000/INTERVAL_VALUE) {
			ZoomEnd = ZoomEnd + ZoomInterval;
			ZoomStart = ZoomEnd - ZoomInterval;		
			PEvset(m_hPE, PEP_fMANUALMINX, &ZoomStart, 1);
			PEvset(m_hPE, PEP_fMANUALMAXX, &ZoomEnd, 1);			
		}
		
		// Append new values to chart  //
		PEvset(m_hPE, PEP_faAPPENDYDATA, &fActualY, 1);
		PEvset(m_hPE, PEP_faAPPENDXDATA, &fTime, 1); 
		
		//get the max Y value in order to calcualte Mp//
        m_fMaxY=m_fMaxY>fActualY ? m_fMaxY:fActualY;
		
		//get the time (Tr)when the Y reach target value//		
		if((m_fLastYValue<m_TargetVal)&&(fActualY>m_TargetVal)&&((m_fTr-0.0)<0.001))
		{
			//线性插值计算到达目标值的时间
			fIncTime=(INTERVAL_VALUE/1000)*(m_TargetVal-m_fLastYValue)/(fActualY-m_fLastYValue);
			m_fTr=fTime+fIncTime;
		}
		
		if(fActualY>1.3*m_TargetVal)
		{
			double arg;
			arg=2.0*m_TargetVal;
			PEvset(m_hPE, PEP_fMANUALMAXY, &arg, 1);
		}
		// Increment index counter //
		m_nRealTimeCounter = m_nRealTimeCounter + 1;	
		
	
		break;

	default:
		break;
	}    


	// Update image and force paint //
	PEreinitialize( m_hPE );
	PEresetimage( m_hPE, 0, 0 );
	::InvalidateRect(m_hPE, NULL, FALSE);

	//refresh//
	m_fLastYValue=fActualY;
	UpdateLed(IDC_DISP_ACTVALUE,fActualY);
    #endif
    

        #if 0
	 src=cvQueryFrame(m_Video);             //读取摄像头一帧数据
	 //src2=cvCloneImage(src);  
	 cvFlip(src, NULL,0);  

       //gray = cvCreateImage( cvGetSize(src), 8, 1 );

  	 //cvCvtColor(src, gray, CV_BGR2GRAY);   //把数据图像转换成单通道的灰度图，存放在gray中，灰度图图像像素为0-255   
   	 //cvErode( gray, gray, NULL,1 );//3*3腐蚀模板过滤，以去掉零星的噪声 
	
	 //cvSmooth(gray, gray,CV_GAUSSIAN,Smooth_Step,Smooth_Step);

	CPaintDC dc(this); // device context for painting
	dc.SetMapMode(MM_TEXT);
	CMemDC  pDC(&dc);  // non flickering painting    

       DrawToHDC(pDC->GetSafeHdc(), src, lRect3);

	  //DrawToHDC(src, lRect3, IDC_PIC_STATIC);
	 cvShowImage("orig", src);
	 //cvReleaseImage( &gray );

	 #endif

	CView::OnTimer(nIDEvent);

}

void CDCMotrCtrlWnd::OnBtnChartStop() 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	gtcard.StopMotion();
	GT_Axis(m_nAxisNo);
	GT_AxisOff();
	
}



 void CDCMotrCtrlWnd::OnBtnChartStart() 
{
#if 0
	 if(m_TargetVal<=0)
	 {
		 MessageBox("目标位置应为正值","XX科技",MB_ICONINFORMATION);
		 return;
	 }
	InitialChart();
	UpdateData(TRUE);

	SetPID(m_nKp,m_nKi,m_nKd);

	GetDlgItem(IDC_STATIC_Mp)->SetWindowText("尚未计算");
	GetDlgItem(IDC_STATIC_Tr)->SetWindowText("尚未计算");

	m_nShowType=GetChartShowType();  
    SetChartShowPara(m_nShowType);

	m_nRealTimeCounter =0;
   
    CComboBox* pCmbAxisNo;
	pCmbAxisNo=(CComboBox* ) GetDlgItem(IDC_CMB_MOTORSEL);
	
	m_nAxisNo=pCmbAxisNo->GetCurSel()+1;
	
	GT_Axis(m_nAxisNo);
	GT_AxisOn();
	Sleep(200);

	short rtn;
	rtn=GT_Axis(m_nAxisNo);
	rtn=GT_ZeroPos();
    rtn=GT_PrflS();	  	   
    rtn=GT_SetVel(5);	  
	rtn=GT_SetMAcc(0.4);	   
	rtn=GT_SetJerk(0.00002);
	rtn=GT_SetPos(m_TargetVal);	  
	Sleep(10);
    rtn=GT_Update();	
    #endif
        #if 0
	 src=cvQueryFrame(m_Video);             //读取摄像头一帧数据
	 //src2=cvCloneImage(src);  
	 cvFlip(src, NULL,0);  

       //gray = cvCreateImage( cvGetSize(src), 8, 1 );

  	 //cvCvtColor(src, gray, CV_BGR2GRAY);   //把数据图像转换成单通道的灰度图，存放在gray中，灰度图图像像素为0-255   
   	 //cvErode( gray, gray, NULL,1 );//3*3腐蚀模板过滤，以去掉零星的噪声 
	
	 //cvSmooth(gray, gray,CV_GAUSSIAN,Smooth_Step,Smooth_Step);

       DrawToHDC(Disp_hDC1, src, lRect3);

	  //DrawToHDC(src, lRect3, IDC_PIC_STATIC);
	 //cvShowImage("orig", src);
	 //cvReleaseImage( &gray );

	 #endif

	SetTimer(1, INTERVAL_VALUE, NULL );
    
	
}

void CDCMotrCtrlWnd::UpdateLed(UINT nID, double dispValue)
{
	CDigitalDisp * pLed=(CDigitalDisp *) GetDlgItem(nID);
	pLed->SetNumToDisp(dispValue);

}

int CDCMotrCtrlWnd::GetChartShowType()
{
	if(((CButton *) GetDlgItem(IDC_AUTOZOOM))->GetCheck()){
		if(((CButton *) GetDlgItem(IDC_SHOW_THREOY))->GetCheck())		
			m_nShowType=ZoomAndShowTheroy;
		else
			m_nShowType=ZoomAndNotShowTheroy;
	}
	else{
		if(((CButton *) GetDlgItem(IDC_SHOW_THREOY))->GetCheck())
			m_nShowType=NotZoomAndShowTheroy;	
		else
			m_nShowType=NotZoomANdNotShowTheroy;
	}
/*	if(m_bAutoZoom){
		if(m_bShowTheroyValue)		
			m_nShowType=ZoomAndShowTheroy;
		else
			m_nShowType=ZoomAndNotShowTheroy;
	}
	else{
		if(m_bShowTheroyValue)
			m_nShowType=NotZoomAndShowTheroy;	
		else
			m_nShowType=NotZoomANdNotShowTheroy;
	}	  */
	return m_nShowType;
}

void CDCMotrCtrlWnd::SetChartShowPara(int nShowType)
{
#if 0
	/*PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
	arg = 0.0F;
	PEvset(m_hPE, PEP_fMANUALMINX, &arg, 1);
	arg = m_fManualMaxX;
	PEvset(m_hPE, PEP_fMANUALMAXX, &arg, 1);*/
 int nCount;

  switch(nShowType) {
  case 	ZoomAndShowTheroy  :
      m_nLineCount=2;
	  nCount=50;
  	break;
  case ZoomAndNotShowTheroy:
	  m_nLineCount=1;
	  nCount=50;
  	break;
  case NotZoomAndShowTheroy:///	
	  m_nLineCount=2;
	  nCount=50;
	  ZoomInterval =5;
	  ZoomStart=0;
	  ZoomEnd=5.0f;
	  break;
  case NotZoomANdNotShowTheroy:
      m_nLineCount=1;
	  nCount=50;
	  ZoomInterval =5;
	  ZoomStart=0;
	  ZoomEnd=5.0f;
	  break;
  }

   PEnset(m_hPE, PEP_nSUBSETS, m_nLineCount);//set subset's number,设置显示曲线数
  
   PEnset(m_hPE, PEP_nPOINTS, nCount*m_fManualMaxX*1000/INTERVAL_VALUE); //set point count,设置单屏显示点数

 /**********************************************/ 
   // Set Manual Y scale ,设置Ｙ轴手动刻度//   
   PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
   double arg = 1.0F;
   PEvset(m_hPE, PEP_fMANUALMINY, &arg, 1);
   arg = m_TargetVal*1.3;
   PEvset(m_hPE, PEP_fMANUALMAXY, &arg, 1);
 /**********************************************/ 
  
 //  PEnset(m_hPE, PEP_nMANUALSCALECONTROLY, PEMSC_NONE);//设置为自动方式

   // Set Manual X scale,设置Ｘ轴手动刻度 //
   PEnset(m_hPE, PEP_nMANUALSCALECONTROLX, PEMSC_MINMAX);
   arg = 0.0F;
   PEvset(m_hPE, PEP_fMANUALMINX, &arg, 1);
   arg = m_fManualMaxX;
   PEvset(m_hPE, PEP_fMANUALMAXX, &arg, 1);

 
/**********************************************/ 
     // Clear out default data,清空原有数据 //
   float val = 0;
   PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 0, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 0, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faXDATA, 1, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 1, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 1, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 1, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 1, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faXDATA, 2, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 2, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 2, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faXDATA, 2, 3, &val);

   PEvsetcellEx(m_hPE, PEP_faYDATA, 2, 0, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 2, 1, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 2, 2, &val);
   PEvsetcellEx(m_hPE, PEP_faYDATA, 2, 3, &val);
/**********************************************/ 

   
/**********************************************/ 
   //set the line type,设置显示曲线的线型//
 /*  int nTmpStyle;
   nTmpStyle = PELT_THINSOLID;
  // nTmpStyle =PELT_DASH;
   PEvsetcell(m_hPE, PEP_naSUBSETLINETYPES, 0, &nTmpStyle); 
   PEvsetcell(m_hPE, PEP_naSUBSETLINETYPES, 1, &nTmpStyle); 
   */
 
  //set line title,设置曲线名称//   
   char szSubsets[] = "位置理论曲线\t位置实际曲线\t";
   PEvset (m_hPE, PEP_szaSUBSETLABELS, szSubsets, 2);
/**********************************************/ 


/**********************************************/     
   // Set Various Other Properties,设置显示参数 ///
   PEnset(m_hPE, PEP_bBITMAPGRADIENTMODE, TRUE);
  // PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_MEDIUM_INSET);
    PEnset(m_hPE, PEP_nQUICKSTYLE, PEQS_LIGHT_SHADOW);
   
   PEszset(m_hPE, PEP_szMAINTITLE, "直流伺服电机位置实时曲线图");   
   PEszset(m_hPE, PEP_szSUBTITLE, "XX科技");
   PEszset(m_hPE, PEP_szXAXISLABEL, "时间　（秒）");
   PEszset(m_hPE, PEP_szYAXISLABEL, "位置");
  
   PEnset(m_hPE, PEP_bNORANDOMPOINTSTOEXPORT, TRUE);
   PEnset(m_hPE, PEP_bFOCALRECT, FALSE);
   PEnset(m_hPE, PEP_bALLOWBAR, FALSE);
   PEnset(m_hPE, PEP_bALLOWPOPUP, FALSE);
   PEnset(m_hPE, PEP_bPREPAREIMAGES, TRUE);
   PEnset(m_hPE, PEP_bCACHEBMP, TRUE);
   PEnset(m_hPE, PEP_bFIXEDFONTS, TRUE);
  /**********************************************/  

 
 /**********************************************/  
   //set line's color,设置显示曲线的颜色//
   DWORD col = RGB(0,0,198);
   PEvsetcell(m_hPE, PEP_dwaSUBSETCOLORS, 0, &col);   
   
   col = RGB(255, 0, 0);
   PEvsetcell(m_hPE, PEP_dwaSUBSETCOLORS, 1, &col);   
 
/**********************************************/
   PEnset(m_hPE,PEP_dwWDESKCOLOR,col);
   PEnset(m_hPE, PEP_nGRADIENTBARS, 8);
   PEnset(m_hPE, PEP_nTEXTSHADOWS, PETS_BOLD_TEXT);
   PEnset(m_hPE, PEP_bMAINTITLEBOLD, TRUE);//主标题为粗体
   PEnset(m_hPE, PEP_bSUBTITLEBOLD, TRUE);//副标题为粗体
   PEnset(m_hPE, PEP_bLABELBOLD, TRUE);
   PEnset(m_hPE, PEP_bLINESHADOWS, TRUE);//线条带阴影
   PEnset(m_hPE, PEP_nFONTSIZE, PEFS_LARGE);//设置为大字体
 /**********************************************/

   PEreinitialize(m_hPE);
   PEresetimage(m_hPE, 0, 0);
   #endif
}

void CDCMotrCtrlWnd::OnBtnChartCalculate() 
{
	float Mp;
	CString str;

	//Calculate Mp,计算超调量//
	if (m_fMaxY<m_TargetVal) {
	GetDlgItem(IDC_STATIC_Mp)->SetWindowText("无超调");
	return;
	}
	Mp=100*(m_fMaxY-m_TargetVal)/m_TargetVal;
	str.Format("%.2f%%",Mp);
	GetDlgItem(IDC_STATIC_Mp)->SetWindowText(str);
	
	//Calculate Tr,计算上升时间//
	if((m_fTr-0.0)<0.001)
		GetDlgItem(IDC_STATIC_Tr)->SetWindowText("无法计算上升时间");
	else{
		str.Format("%.2f秒",m_fTr);
		GetDlgItem(IDC_STATIC_Tr)->SetWindowText(str);	
		m_fTr=0.0f;//重置为0
	}

}

void CDCMotrCtrlWnd::SetPID(UINT nKp, UINT nKi, UINT nKd)
{
	gtcard.SetPID(0,nKp,nKi,nKd);
}

void CDCMotrCtrlWnd::OnBtnChartRestore() 
{
	// TODO: Add your control notification handler code here
	if(IDYES==MessageBox("是否恢复缺省设置？","XX科技",MB_YESNO|MB_ICONQUESTION))
	{
		InitialChart();
		InitCard();
		UpdateLed(IDC_DISP_ACTVALUE,0.0f);
		GetDlgItem(IDC_STATIC_Mp)->SetWindowText("尚未计算");
		GetDlgItem(IDC_STATIC_Tr)->SetWindowText("尚未计算");
	}
	
}



void CDCMotrCtrlWnd::InitCard()
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

         #if 0
    static int  k=0;   //防止重复按键	

    if(k==0)
    {  
        m_Video=cvCreateCameraCapture(-1);      //打开摄像头
        if (!m_Video)
        { k=0;
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

}





















