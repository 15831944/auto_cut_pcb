// PageSMode.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "ACMotorCtrlWnd.h"
#include "CcdSelTabCtrl.h"
#include "PageExec.h"
#include <math.h>

#include "GT400.h"
#include "GTCard.h"

#include "cv.h"
#include "highgui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PI 1.54159
extern CGTCard gtcard;

/////////////////////////////////////////////////////////////////////////////
// CPageExec dialog


CPageExec::CPageExec(CWnd* pParent /*=NULL*/)
	: CDialog(CPageExec::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageExec)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageExec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageExec)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageExec, CDialog)
	//{{AFX_MSG_MAP(CPageExec)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CPageExec::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_VIEW_TEST, &CPageExec::OnBnClickedViewTest)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CPageExec::OnBnClickedButtonExec)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageExec message handlers

BOOL CPageExec::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_SMODE_JERK)->SetWindowText("0.0001");
	GetDlgItem(IDC_SMODE_ACC)->SetWindowText("0.2");
	GetDlgItem(IDC_SMODE_VEL)->SetWindowText("10");
	GetDlgItem(IDC_SMODE_POS)->SetWindowText("30000");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageExec::OnBnClickedButtonOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
}


#if 0
void CPageExec::LnXY(long x, long y, double acc, double fVel)
{
	short rtn;
       rtn = GT_SetSynAcc(acc);    //Error(rtn,"GT_SetSynAcc");
	rtn = GT_SetSynVel(fVel);    //Error(rtn,"GT_SetSynVel");
	rtn=GT_LnXY(x, y);  // Error(rtn,"GT_LnXY");
	unsigned short mode;
	rtn = GT_GetMode(&mode);
}


void CPageExec::GetActMark(&MarkAX, &MarkAY, &MarkBX, &MarkBY);
{
    /* 获取像素坐标*/
    /* 转换成运动坐标*/
}
#endif


int CPageExec::GetActualMark(int MarkNum, double *MarkBX, double *MarkBY)
{
    // TODO: 在此添加命令处理程序代码
    #if 0
    CvRect selection;
    int i;
    double s1,len1;  
    CvPoint* p;    
    CvScalar center;//重心位置
    IplImage* img_8uc1;
    int ccdWidth = 0, ccdHeight = 0;
    long Diameter = 0, MillingDiameter = 0;
    double len = 0, S = 0;
    double Coefficient;
    double R = 0;
    double alpha = 0;
        
    CvCapture *Video = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_Video;   
    img_8uc1 = cvQueryFrame(Video);             //读取摄像头一帧数据

    ccdWidth =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_CcdWidth;
    ccdHeight = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_CcdHeight;
    
    selection.x = img_8uc1->width/2 - ccdWidth/2;     
    selection.y = img_8uc1->height/2 - ccdHeight/2;
    selection.width = ccdWidth;
    selection.height = ccdHeight;
    cvSetImageROI(img_8uc1, selection);

    //cvShowImage("orig", img_8uc1);

    IplImage* img_edge = cvCreateImage(cvGetSize(img_8uc1),8,1);
    IplImage* img_8uc3 = cvCreateImage(cvGetSize(img_8uc1),8,3);

    cvSmooth( img_8uc1, img_8uc1, CV_GAUSSIAN, 9, 9 ); // smooth it, otherwise a lot of false circles may be detected 

    //cvThreshold(img_8uc1,img_edge,128,255,CV_THRESH_BINARY);
    cvAdaptiveThreshold( img_8uc1, img_edge, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 61);

    CvMemStorage* storage = cvCreateMemStorage();
    CvSeq* first_contour = NULL;
 
     int Nc = cvFindContours(
         img_edge,
         storage,
         &first_contour,
         sizeof(CvContour),
         CV_RETR_LIST
         );

     Diameter = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_Diameter;
     len = 2 * PI * (Diameter/2);
     S = PI * (Diameter/2)*(Diameter/2);
     
     int n=0;
     //初始化重心位置
     center = cvScalarAll(0);
     for (CvSeq* c=first_contour;c!=NULL;c=c->h_next)
     {
         cvCvtColor(img_8uc1,img_8uc3,CV_GRAY2BGR);
         s1 = fabs(cvContourArea(c));//轮廓面积  
         len1=cvArcLength(c);  

         if(s1> (S-10) && s1 > (S+10) && len1 > (len -10) && len1 < (len+10))
         {
             cvDrawContours(
                 img_8uc3,
                 c,
                 CV_RGB(0,0,255),
                 CV_RGB(0,255,0),
                 0,
                 2,
                 8
                 );
                
                for(i = 0;i < c->total;i++)
                {
                    p = (CvPoint* )CV_GET_SEQ_ELEM(CvPoint,c,i);
                    center.val[0] += p->x;
                    center.val[1] += p->y;
                }
                
                center.val[0] /= c->total;
                center.val[1] /= c->total;

                R = sqrt((center.val[0] - img_8uc1->width/2)*(center.val[0] - img_8uc1->width/2) 
                             + (center.val[1] - img_8uc1->height/2) * (center.val[1] - img_8uc1->height/2));
                alpha = atan((double)abs(center.val[1] - img_8uc1->height/2) / abs(center.val[0] - img_8uc1->width/2));
                Coefficient = Diameter / MillingDiameter;

                *MarkBX = R * cos(alpha) * Coefficient; 
                *MarkBY = R * sin(alpha) * Coefficient;
                 cvReleaseImage(&img_8uc3);
                 cvReleaseImage(&img_edge);
                 cvReleaseMemStorage(&storage); //閲婃斁鍐呭瓨
                 return OK;
         }
         
         n++;
     }
 
     cvReleaseImage(&img_8uc3);
    // cvReleaseImage(&img_8uc1);
     cvReleaseImage(&img_edge);
     cvReleaseMemStorage(&storage); //閲婃斁鍐呭瓨

     return ERROR;
     #endif

    if(MarkNum == 0)
    {
        *MarkBX = LB0_OriginalValueX; 
        *MarkBY = LB0_OriginalValueY;
    }
    else if(MarkNum == 1)
    {
        *MarkBX = LB1_OriginalValueX; 
        *MarkBY = LB1_OriginalValueY;        
    }
    
     return 0;
}

void CPageExec::GetActualPoint(PCB_CUT_POINT_S *pstCutPoint, PCB_CUT_POINT_S *pstActualPoint)
{
    CString strInfo;
    double R = 0;
    double alpha = 0, beta = 0, thet = 0, gama = 0;
    long LB0_ValueX=0, LB0_ValueY=0,LB1_ValueX=0,LB1_ValueY=0;
    double ccdDeltaX, ccdDeltaY;

    ccdDeltaX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_ccdDeltaX;
    ccdDeltaY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_ccdDeltaY;
    
 //   strInfo.Format("xxxposition:%f,%f", ccdDeltaX, ccdDeltaY);
 //   MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

    LB0_ValueX = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB0_ValueX + ccdDeltaX;
    LB0_ValueY = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB0_ValueY + ccdDeltaY;
    LB1_ValueX = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB1_ValueX + ccdDeltaX;
    LB1_ValueY = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB1_ValueY + ccdDeltaY;

    #if 0
    alpha = atan((double)abs(LB1_ValueY-LB0_ValueY) / abs(LB1_ValueX - LB0_ValueX));
    beta = atan((double)abs(LB1_ActuralValueX-LB0_ActuralValueY) / abs(LB1_ActuralValueX - LB0_ActuralValueX));

    thet = beta - alpha;

    R = sqrt((double)(pstCutPoint->PosX - LB0_ValueX)*(pstCutPoint->PosX - LB0_ValueX) + (pstCutPoint->PosY - LB0_ValueY)*(pstCutPoint->PosY - LB0_ValueY));
    gama  = atan((double)abs(pstCutPoint->PosY-LB0_ValueY) / abs(pstCutPoint->PosX - LB0_ValueX));

    pstActualPoint->PosX = R * cos(gama + thet);
    pstActualPoint->PosY = R * sin(gama + thet);
    #endif
     pstActualPoint->PosX = pstCutPoint->PosX  + ccdDeltaX;
     pstActualPoint->PosY = pstCutPoint->PosY + ccdDeltaY; 
}


short CPageExec::GetCenter(CIRCUL_POINT_S &p1,CIRCUL_POINT_S &p2, CIRCUL_POINT_S &p3, CIRCUL_POINT_S &center)
{ 
    CIRCUL_POINT_S C1,C2; 
    
    C1.x=(p1.x+p2.x)/2.0; 
    C1.y=(p1.y+p2.y)/2.0; 
    C2.x=(p2.x+p3.x)/2.0; 
    C2.y=(p2.y+p3.y)/2.0; 
    float k1,k2; 
    
    if((p2.y-p1.y)==0.0) 
    { 
        if((p3.y-p2.y)==0.0)  
        {   
            AfxMessageBox("三点共线，无法构成一个圆!", MB_OK);
            return -1; 
        }  
        else  
        {   
            k2=-1*(p3.x-p2.x)/(p3.y-p2.y);  
            center.x=C1.x;     
            center.y=k2*(center.x-C2.x)+C2.y; 
        } 
    } 
    else 
    {     
        k1=-1*(p2.x-p1.x)/(p2.y-p1.y); 
        if((p3.y-p2.y)==0.0) 
        {  
            center.x=C2.x;   
            center.y=k1*(center.x-C1.x)+C1.y;  
        } 
        else  
        {   
            k2=-1*(p3.x-p2.x)/(p3.y-p2.y);   
            if(k1==k2)   
            {    
                AfxMessageBox("三点共线，无法构成一个圆!", MB_OK);    
                return -1;   
            }   
            else  
            {   
                center.x=(C2.y-C1.y+k1*C1.x-k2*C2.x)/(k1-k2);    
                center.y=k1*(center.x-C1.x)+C1.y;  
            }  
        } 
    } 

    return 0;
}


int CPageExec::GetDir(CIRCUL_POINT_S &center, CIRCUL_POINT_S &p1, CIRCUL_POINT_S &p2)
{
    double X1, X2, Y1, Y2;
    double cross;
    
    X1 = p1.x-center.x;
    Y1 = p1.y-center.y;

    X2 = p2.x-p1.x;
    Y2 = p2.y-p1.y;

    cross = X1 * Y2 - X2 * Y1;

    if(cross > 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
    
}


void CPageExec::OnBnClickedViewTest()
{
    // TODO: 在此添加控件通知处理程序代码
    short rtn;
    double ccdDeltaX, ccdDeltaY;
    PCB_CUT_POINT_S  stTempPoint, stTmpFirstPoint, stTmpSecondPoint, stTmpLastPoint, stTempActualPoint, stCutPoint, stActualPoint;
    int index;
    CString strInfo;
    unsigned short status=0;
    long actl_pos[4];
    int pointIndex = 0, IsCircle = false;
    CIRCUL_POINT_S firstPoint, secondPoint, lastPoint, center;
    double radius;
    int dir;

    ccdDeltaX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_ccdDeltaX;
    ccdDeltaY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_ccdDeltaY;
    
   // strInfo.Format("position:%f,%f", ccdDeltaX, ccdDeltaY);
   // MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);   
    
    LB0_OriginalValueX = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB0_ValueX;
    LB0_OriginalValueY = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB0_ValueY;
    LB1_OriginalValueX = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB1_ValueX;
    LB1_OriginalValueY = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB1_ValueY;

  //  strInfo.Format("position:%f,%f,%f,%f", LB0_OriginalValueX, LB0_OriginalValueY,LB1_OriginalValueX, LB1_OriginalValueY);
   // MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
    
    GT_SetSynAcc(0.1);    
    GT_SetSynVel(8.0);    

  //  rtn=GT_LnXY(8969,6961);  

  //  return;
    
    rtn=GT_LnXY(LB0_OriginalValueX, LB0_OriginalValueY);  

    #if 0
    rtn=GT_Axis(1);
//    rtn=GT_ClrSts();
    
    rtn=GT_GetSts(&status);	
    while(!(status&AXIS_STS_MOTION_FINISH))
    {
        rtn=GT_GetSts(&status);
        Sleep(10);
    }	
    rtn=GT_ClrSts();


   while(1)
   {
      rtn=GT_Axis(1);
      rtn=GT_ClrSts();
      rtn=GT_GetAtlPos(&actl_pos[0]);
      if(actl_pos[0] + 50 > )
   }
#endif
	while(gtcard.IsCardMove())
	{
	      ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->FindCircle();
            Sleep(100);
	}
    
      Sleep(1000);
  //  strInfo.Format("111position:%f,%f,%f,%f", LB0_OriginalValueX, LB0_OriginalValueY, LB1_OriginalValueX, LB1_OriginalValueY);
  //  MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
    
    LB0_ActuralValueX = 0;
    LB0_ActuralValueY = 0;
    if(-1 == GetActualMark(0, &LB0_ActuralValueX, &LB0_ActuralValueY))
    {
       AfxMessageBox("can not find mark", MB_OK);
       return;
    }

    if(LB0_ActuralValueX == 0 || LB0_ActuralValueY == 0)
    {
       AfxMessageBox("can not find mark", MB_OK);
       return;
    }

    GT_SetSynAcc(0.1);    
    GT_SetSynVel(8.0);    
        
    rtn=GT_LnXY(LB1_OriginalValueX, LB1_OriginalValueY);

    while(gtcard.IsCardMove())
    {
    	   ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->FindCircle();
          Sleep(100);
    }

      Sleep(1000);


    LB1_ActuralValueX = 0;
    LB1_ActuralValueY = 0;

    if(-1 == GetActualMark(1, &LB1_ActuralValueX, &LB1_ActuralValueY))
    {
       AfxMessageBox("can not find mark",MB_OK);
       return;
    }

    if(LB1_ActuralValueX == 0 || LB1_ActuralValueY == 0)
    {
       AfxMessageBox("can not find mark", MB_OK);
       return;
    }

    #if 0
    stCutPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[0].PosX;
    stCutPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[0].PosY;

    GetActualPoint(&stCutPoint, &stActualPoint);

    /* 首先由原点移动到第一个起点处*/
    rtn=GT_LnXY(stActualPoint.PosX, stActualPoint.PosY);
    rtn=GT_Axis(1);
    rtn=GT_GetSts(&status);	
    while(!(status&AXIS_STS_MOTION_FINISH))
    {
        rtn=GT_GetSts(&status);
        Sleep(10);
    }	

    rtn=GT_Axis(2);
    rtn=GT_GetSts(&status);	
    while(!(status&AXIS_STS_MOTION_FINISH))
    {
        rtn=GT_GetSts(&status);
        Sleep(10);
    }	
    #endif

  //  strInfo.Format("222position:%f,%f,%f,%f", LB0_ActuralValueX, LB0_ActuralValueY, LB1_ActuralValueX, LB1_ActuralValueY);
 //   MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

    for(index = 0; index < ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->PointNum; index++)
    {
        stCutPoint = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index];
        
        //GetActualPoint(&stCutPoint, &stActualPoint);
        stActualPoint = stCutPoint;
     //   strInfo.Format("555position:%d,%d", stActualPoint.PosX, stActualPoint.PosY);
    //    MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

        /* 如果是直线并且是下刀点*/
        if(stCutPoint.type == "DownCut" )
        {
          //  strInfo.Format("333position:%d,%d", stActualPoint.PosX, stActualPoint.PosY);
         //   MessageBox(strInfo,"ERROR",MB_OK|MB_ICONERROR);
            if(pointIndex == 0)
            {
                rtn = GT_StrtList();
                rtn=GT_MvXY(stActualPoint.PosX, stActualPoint.PosY, 2, 0.1);      
     //        GT_LnXY(stCutPoint.PosX, stCutPoint.PosY);
                rtn=GT_EndList();
                rtn=GT_StrtMtn();
                while(gtcard.IsCardMove())
                {
                	 ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->FindCircle();
                      Sleep(100);
                }

                #if 0
                status=0;
                rtn=GT_Axis(AXIS_1 + 1);
                rtn=GT_GetSts(&status);	
                while(!(status&AXIS_STS_MOTION_FINISH))
                {
                    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->FindCircle();
                    rtn=GT_GetSts(&status);
                    Sleep(10);
                }	            
                #endif
                Sleep(100);

                /* 切下去*/
            }
            
            pointIndex ++;
            if(pointIndex == 2)
            {
                IsCircle =  true;
                continue;
            }

        }
        
        if(stCutPoint.type == "DryRun")
        {
       //     strInfo.Format("444position:%d,%d", stActualPoint.PosX, stActualPoint.PosY);
        //   MessageBox(strInfo,"ERROR",MB_OK|MB_ICONERROR);
            pointIndex = 0;

            if(IsCircle == true)
            { 
				stTmpFirstPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-2].PosX;
                stTmpFirstPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-2].PosY;
				//GetActualPoint(&stTmpFirstPoint, &stTempActualPoint);
				stTempActualPoint = stTmpFirstPoint;
				firstPoint.x = stTempActualPoint.PosX;
                firstPoint.y = stTempActualPoint.PosY;

				stTmpSecondPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosX;
                stTmpSecondPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosY;
				//GetActualPoint(&stTmpSecondPoint, &stTempActualPoint);
				stTempActualPoint = stTmpSecondPoint;
				secondPoint.x = stTempActualPoint.PosX;
                secondPoint.y = stTempActualPoint.PosY;

				
                stTmpLastPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index].PosX;
                stTmpLastPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index].PosY;
				//GetActualPoint(&stTmpLastPoint, &stTempActualPoint);
				stTempActualPoint = stTmpLastPoint;
                lastPoint.x = stTempActualPoint.PosX;
                lastPoint.y = stTempActualPoint.PosY;

                rtn = GetCenter(firstPoint, secondPoint, lastPoint, center);
                radius = sqrt((double)(lastPoint.x-center.x)*(lastPoint.x-center.x) + (double)(lastPoint.y-center.y)*(lastPoint.y-center.y));    
                dir = GetDir(center, firstPoint, lastPoint);
                rtn = GT_StrtList();
                rtn=GT_MvXY(firstPoint.x, firstPoint.y, 2, 0.1);      
                GT_ArcXYP(lastPoint.x, lastPoint.y, radius, dir);
                rtn=GT_EndList();
                rtn=GT_StrtMtn();

                IsCircle = false;
            }
            else
            {
          //      firstPoint.x = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosX;
          //      firstPoint.y = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosY;

				stTempPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosX;
				stTempPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosY;
				//GetActualPoint(&stTempPoint, &stTempActualPoint);
                            stTempActualPoint  = stTempPoint;
				firstPoint.x = stTempActualPoint.PosX;
				firstPoint.y = stTempActualPoint.PosY;
                secondPoint.x = stActualPoint.PosX;
                secondPoint.y = stActualPoint.PosY;
                
                rtn = GT_StrtList();
                rtn=GT_MvXY(firstPoint.x, firstPoint.y, 2, 0.1);      
                GT_LnXY(secondPoint.x, secondPoint.y);
                rtn=GT_EndList();
                rtn=GT_StrtMtn();
            }
            
            while(gtcard.IsCardMove())
            {
            	    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->FindCircle();
                  Sleep(100);
            }

            #if 0
            status=0;
            rtn=GT_Axis(AXIS_1 + 1);
            rtn=GT_GetSts(&status);	
            while(!(status&AXIS_STS_MOTION_FINISH))
            {
                ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->FindCircle();
                rtn=GT_GetSts(&status);
                Sleep(10);
            }	            
            #endif
            Sleep(100);

             /* 把Z 轴提起来 */
            #if 0
            gtcard.Jog(2, 5000,1,POSITIVE_DIRCT);
           /* 到达终点后才开始下一循环*/
            rtn=GT_Axis(2);
            rtn=GT_GetSts(&status);	
	      while(!(status&AXIS_STS_MOTION_FINISH))
             {
			rtn=GT_GetSts(&status);
                   Sleep(10);
		}	
            #endif

        }
        
        #if 0
        if(arc)
        {
            Segment.type = arc;
        }
        #endif
        
    }
}



void CPageExec::OnBnClickedButtonExec()
{
    // TODO: 在此添加控件通知处理程序代码
    short rtn;
    double ccdDeltaX, ccdDeltaY;
    PCB_CUT_POINT_S  stTempPoint, stTmpFirstPoint, stTmpSecondPoint, stTmpLastPoint, stTempActualPoint, stCutPoint, stActualPoint;
    int index;
    CString strInfo;
    unsigned short status=0;
    long actl_pos[4];
    int pointIndex = 0, IsCircle = false;
    CIRCUL_POINT_S firstPoint, secondPoint, lastPoint, center;
    double radius;
    int dir;

	gtcard.OpenPort(15,true);

    ccdDeltaX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_ccdDeltaX;
    ccdDeltaY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_ccdDeltaY;
    
   // strInfo.Format("position:%f,%f", ccdDeltaX, ccdDeltaY);
   // MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);   
    
    LB0_OriginalValueX = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB0_ValueX;
    LB0_OriginalValueY = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB0_ValueY;
    LB1_OriginalValueX = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB1_ValueX;
    LB1_OriginalValueY = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB1_ValueY;

  //  strInfo.Format("position:%f,%f,%f,%f", LB0_OriginalValueX, LB0_OriginalValueY,LB1_OriginalValueX, LB1_OriginalValueY);
   // MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
    
    GT_SetSynAcc(0.1);    
    GT_SetSynVel(8.0);    

  //  rtn=GT_LnXY(8969,6961);  

  //  return;
    
    rtn=GT_LnXY(LB0_OriginalValueX, LB0_OriginalValueY);  

    #if 0
    rtn=GT_Axis(1);
//    rtn=GT_ClrSts();
    
    rtn=GT_GetSts(&status);	
    while(!(status&AXIS_STS_MOTION_FINISH))
    {
        rtn=GT_GetSts(&status);
        Sleep(10);
    }	
    rtn=GT_ClrSts();


   while(1)
   {
      rtn=GT_Axis(1);
      rtn=GT_ClrSts();
      rtn=GT_GetAtlPos(&actl_pos[0]);
      if(actl_pos[0] + 50 > )
   }
#endif
	while(gtcard.IsCardMove())
	{
            Sleep(100);
	}
    
      Sleep(1000);
  //  strInfo.Format("111position:%f,%f,%f,%f", LB0_OriginalValueX, LB0_OriginalValueY, LB1_OriginalValueX, LB1_OriginalValueY);
  //  MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);
    
    LB0_ActuralValueX = 0;
    LB0_ActuralValueY = 0;
    if(-1 == GetActualMark(0, &LB0_ActuralValueX, &LB0_ActuralValueY))
    {
       AfxMessageBox("can not find mark", MB_OK);
       return;
    }

    if(LB0_ActuralValueX == 0 || LB0_ActuralValueY == 0)
    {
       AfxMessageBox("can not find mark", MB_OK);
       return;
    }

    GT_SetSynAcc(0.1);    
    GT_SetSynVel(8.0);    
        
    rtn=GT_LnXY(LB1_OriginalValueX, LB1_OriginalValueY);

    while(gtcard.IsCardMove())
    {
          Sleep(100);
    }

      Sleep(1000);


    LB1_ActuralValueX = 0;
    LB1_ActuralValueY = 0;

    if(-1 == GetActualMark(1, &LB1_ActuralValueX, &LB1_ActuralValueY))
    {
       AfxMessageBox("can not find mark",MB_OK);
       return;
    }

    if(LB1_ActuralValueX == 0 || LB1_ActuralValueY == 0)
    {
       AfxMessageBox("can not find mark", MB_OK);
       return;
    }

    #if 0
    stCutPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[0].PosX;
    stCutPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[0].PosY;

    GetActualPoint(&stCutPoint, &stActualPoint);

    /* 首先由原点移动到第一个起点处*/
    rtn=GT_LnXY(stActualPoint.PosX, stActualPoint.PosY);
    rtn=GT_Axis(1);
    rtn=GT_GetSts(&status);	
    while(!(status&AXIS_STS_MOTION_FINISH))
    {
        rtn=GT_GetSts(&status);
        Sleep(10);
    }	

    rtn=GT_Axis(2);
    rtn=GT_GetSts(&status);	
    while(!(status&AXIS_STS_MOTION_FINISH))
    {
        rtn=GT_GetSts(&status);
        Sleep(10);
    }	
    #endif

  //  strInfo.Format("222position:%f,%f,%f,%f", LB0_ActuralValueX, LB0_ActuralValueY, LB1_ActuralValueX, LB1_ActuralValueY);
 //   MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

    for(index = 0; index < ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->PointNum; index++)
    {
        stCutPoint = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index];
        
        GetActualPoint(&stCutPoint, &stActualPoint);
     //   strInfo.Format("555position:%d,%d", stActualPoint.PosX, stActualPoint.PosY);
    //    MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

        /* 如果是直线并且是下刀点*/
        if(stCutPoint.type == "DownCut" )
        {
          //  strInfo.Format("333position:%d,%d", stActualPoint.PosX, stActualPoint.PosY);
         //   MessageBox(strInfo,"ERROR",MB_OK|MB_ICONERROR);
            if(pointIndex == 0)
            {
            #if 0
                rtn = GT_StrtList();
                rtn=GT_MvXY(stActualPoint.PosX, stActualPoint.PosY, 2, 0.1);      
     //        GT_LnXY(stCutPoint.PosX, stCutPoint.PosY);
                rtn=GT_EndList();
                rtn=GT_StrtMtn();
                while(gtcard.IsCardMove())
                {
                      Sleep(100);
                }
                Sleep(1000);
            #endif
                rtn = GT_SetSynAcc(0.1);    //Error(rtn,"GT_SetSynAcc");
                rtn = GT_SetSynVel(1);    //Error(rtn,"GT_SetSynVel");
                rtn=GT_LnXY(stActualPoint.PosX,  stActualPoint.PosY);  // Error(rtn,"GT_LnXY");
                unsigned short mode;
                rtn = GT_GetMode(&mode);
                
                status=0;
                rtn=GT_Axis(AXIS_1 + 1);
                rtn=GT_GetSts(&status);	
                while(!(status&AXIS_STS_MOTION_FINISH))
                {
                    rtn=GT_GetSts(&status);
                    Sleep(10);
                }	            

                while(gtcard.IsCardMove())
                {
                       Sleep(100);
                }
                Sleep(100);

               //strInfo.Format("down cut");
               //MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

                gtcard.Jog(AXIS_4, 25, 1, NEGTIVE_DIRCT);
                /* 到达终点后才开始下一循环*/
                status=0;
                rtn=GT_Axis(AXIS_4 + 1);
                rtn=GT_GetSts(&status);	
                while(!(status&AXIS_STS_MOTION_FINISH))
                {
                    rtn=GT_GetSts(&status);
                    Sleep(10);
                }	 

                while(gtcard.IsCardMove())
                {
                       Sleep(100);
                }
                Sleep(100);

               //strInfo.Format("down cut OVER");
               //MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

                /* 切下去*/
            }
            
            pointIndex ++;
            if(pointIndex == 2)
            {
                IsCircle =  true;
                continue;
            }

        }
        
        if(stCutPoint.type == "DryRun")
        {
       //     strInfo.Format("444position:%d,%d", stActualPoint.PosX, stActualPoint.PosY);
        //   MessageBox(strInfo,"ERROR",MB_OK|MB_ICONERROR);
            pointIndex = 0;
       
            status=0;
            rtn=GT_Axis(AXIS_1 + 1);
            rtn=GT_GetSts(&status);	
            while(!(status&AXIS_STS_MOTION_FINISH))
            {
                rtn=GT_GetSts(&status);
                Sleep(100);
            }	 

            while(gtcard.IsCardMove())
            {
                   Sleep(100);
            }
            Sleep(100);
            
            if(IsCircle == true)
            { 
                stTmpFirstPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-2].PosX;
                stTmpFirstPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-2].PosY;
                GetActualPoint(&stTmpFirstPoint, &stTempActualPoint);
                firstPoint.x = stTempActualPoint.PosX;
                firstPoint.y = stTempActualPoint.PosY;

                stTmpSecondPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosX;
                stTmpSecondPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosY;
                GetActualPoint(&stTmpSecondPoint, &stTempActualPoint);
                secondPoint.x = stTempActualPoint.PosX;
                secondPoint.y = stTempActualPoint.PosY;


                stTmpLastPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index].PosX;
                stTmpLastPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index].PosY;
                GetActualPoint(&stTmpLastPoint, &stTempActualPoint);
                lastPoint.x = stTempActualPoint.PosX;
                lastPoint.y = stTempActualPoint.PosY;

                rtn = GetCenter(firstPoint, secondPoint, lastPoint, center);
                radius = sqrt((double)(lastPoint.x-center.x)*(lastPoint.x-center.x) + (double)(lastPoint.y-center.y)*(lastPoint.y-center.y));    
                dir = GetDir(center, firstPoint, lastPoint);

                while(gtcard.IsCardMove())
                {
                    Sleep(100);
                }

                #if 1
                rtn = GT_StrtList();
                rtn=GT_MvXY(firstPoint.x, firstPoint.y, 2, 0.1);      
                GT_ArcXYP(lastPoint.x, lastPoint.y, radius, dir);
                rtn=GT_EndList();
                rtn=GT_StrtMtn();
                #endif

                #if 0
                rtn = GT_SetSynAcc(0.1);    //Error(rtn,"GT_SetSynAcc");
                rtn = GT_SetSynVel(1);    //Error(rtn,"GT_SetSynVel");
                rtn=GT_LnXY(stActualPoint.PosX,  stActualPoint.PosY);  // Error(rtn,"GT_LnXY");
                unsigned short mode;
                rtn = GT_GetMode(&mode);
                #endif
                
                status=0;
                rtn=GT_Axis(AXIS_1 + 1);
                rtn=GT_GetSts(&status);	
                while(!(status&AXIS_STS_MOTION_FINISH))
                {
                    rtn=GT_GetSts(&status);
                    Sleep(10);
                }	 

                while(gtcard.IsCardMove())
                {
                       Sleep(100);
                }
                
                Sleep(100);

                //strInfo.Format("DRY RUN");
                //MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

                gtcard.Jog(AXIS_4, 25, 1, POSITIVE_DIRCT);
                /* 到达终点后才开始下一循环*/

                status=0;
                rtn=GT_Axis(AXIS_4 + 1);
                rtn=GT_GetSts(&status);	
                while(!(status&AXIS_STS_MOTION_FINISH))
                {
                    rtn=GT_GetSts(&status);
                    Sleep(10);
                }	            
                
                IsCircle = false;
            }
            else
            {
                //      firstPoint.x = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosX;
                //      firstPoint.y = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosY;

                stTempPoint.PosX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosX;
                stTempPoint.PosY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[index-1].PosY;
                GetActualPoint(&stTempPoint, &stTempActualPoint);

                firstPoint.x = stTempActualPoint.PosX;
                firstPoint.y = stTempActualPoint.PosY;
                secondPoint.x = stActualPoint.PosX;
                secondPoint.y = stActualPoint.PosY;

                status=0;
                rtn=GT_Axis(AXIS_4 + 1);
                rtn=GT_GetSts(&status);	
                while(!(status&AXIS_STS_MOTION_FINISH))
                {
                    rtn=GT_GetSts(&status);
                    Sleep(10);
                }	            

                while(gtcard.IsCardMove())
                {
                      Sleep(100);
                }

                rtn = GT_StrtList();
                rtn=GT_MvXY(firstPoint.x, firstPoint.y, 2, 0.1);      
                GT_LnXY(secondPoint.x, secondPoint.y);
                rtn=GT_EndList();
                rtn=GT_StrtMtn();

                status=0;
                rtn=GT_Axis(AXIS_1 + 1);
                rtn=GT_GetSts(&status);	
                while(!(status&AXIS_STS_MOTION_FINISH))
                {
                    rtn=GT_GetSts(&status);
                    Sleep(10);
                }	 

                while(gtcard.IsCardMove())
                {
                       Sleep(100);
                }

                Sleep(100);
                //strInfo.Format("DRY RUN");
                //MessageBox(strInfo,"错误",MB_OK|MB_ICONERROR);

                gtcard.Jog(AXIS_4, 25, 1, POSITIVE_DIRCT);
                /* 到达终点后才开始下一循环*/
                status=0;
                rtn=GT_Axis(AXIS_4 + 1);
                rtn=GT_GetSts(&status);	
                while(!(status&AXIS_STS_MOTION_FINISH))
                {
                    rtn=GT_GetSts(&status);
                    Sleep(10);
                }	            

                while(gtcard.IsCardMove())
                {
                      Sleep(100);
                }
                Sleep(100);
            }
            
            while(gtcard.IsCardMove())
            {
                  Sleep(100);
            }

            Sleep(100);

             /* 把Z 轴提起来 */
	

            #if 0
            gtcard.Jog(2, 5000,1,POSITIVE_DIRCT);
           /* 到达终点后才开始下一循环*/
            rtn=GT_Axis(2);
            rtn=GT_GetSts(&status);	
	      while(!(status&AXIS_STS_MOTION_FINISH))
             {
		    rtn=GT_GetSts(&status);
                  Sleep(10);
		}	
            #endif

        }
        
        #if 0
        if(arc)
        {
            Segment.type = arc;
        }
        #endif
        
    }

    Sleep(1000);
    gtcard.OpenPort(15,false);
}
