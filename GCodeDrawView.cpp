// GCodeDrawView.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "MotorControlBench.h"
#include "MotorControlBenchDoc.h"
#include "GCodeDrawView.h"
#include "GTCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCncArray  glcncarray;
extern CGTCard gtcard;

#define clCutColor  0x0080FF00
#define clMovColor  0x00C0C0C0
#define clBackColor  0x00B97C4F

#define XYPLANE 0
#define XZPLANE	1
#define	YZPLANE 2
//#define 3DPLANE 3

static int LBtnDown=0;
HMETAFILE MetaFileHandle;

/////////////////////////////////////////////////////////////////////////////
// CGCodeDrawView

IMPLEMENT_DYNCREATE(CGCodeDrawView, CView)

CGCodeDrawView::CGCodeDrawView():ViewOrg(0,0)
{
	X_Ror_Angle=270;	Z_Ror_Angle=45;

    PicZoomWidth=1;
 	PicZoomHeight=1;
	lastX=lastY=lastZ=INITVALUE;
	bRequireMove=false; //绘制光标
	bReDraw=true;

	//设置画笔
	curosrPen1=CreatePen(PS_SOLID,2,0x009289F8);
	curosrPen2=CreatePen(PS_SOLID,1,0x00FFFFFF);
	workDrawPen=CreatePen(PS_SOLID,2,clCutColor);
	moveDrawPen=CreatePen(PS_SOLID,2,clMovColor);

    pMetaFileDC = new CMetaFileDC(); 
	pMetaFileDC->Create();
	MetaFileHandle=pMetaFileDC->Close();
	gtcard.pDrawView =this;
	ViewHigh=ViewWidth=0;
	OriginX=OriginY=0;
    m_xMap=1;
	m_yMap=2;
	m_zMap=3;
	m_scale=1.0;
}

CGCodeDrawView::~CGCodeDrawView()
{
	delete pMetaFileDC;
}


BEGIN_MESSAGE_MAP(CGCodeDrawView, CView)
	//{{AFX_MSG_MAP(CGCodeDrawView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCodeDrawView drawing
void CGCodeDrawView::OnDraw(CDC* pDC)
{

	CPoint FactOrg;
	MachineStatus  CurWorkSts;
	bool bdraw=true;

	bRequireMove=true;//不显示绘图光标
	lastX=lastY=lastZ=INITVALUE;	
	CurWorkSts=gtcard.pDoc->m_worksts;//获取当前绘图状态
	GetClientRect(bodyCanvas); 
	if((CurWorkSts==NEW_STATUS)||(CurWorkSts==START_STATUS)||(CurWorkSts==NULL_STATUS))
	{
		if (CurWorkSts==NULL_STATUS) 
		{
			bRequireMove=false;
			drawRatio=1;
			GetCurPostion();
			pDC->FillSolidRect(bodyCanvas,clBackColor);		
			return;
		} 
		else if(CurWorkSts==NEW_STATUS||CurWorkSts==START_STATUS)
		{ 
			if(bReDraw)
			{
				bdraw=false;
				GetCurRatio(m_scale);
				GetCurPostion();
			}
		}
	}
	else if(CurWorkSts==STOP_STATUS)
	{	} 
	else if(CurWorkSts==PAUSE_STATUS)
	{	} 
	else if(CurWorkSts==CONTINUE_STATUS)
	{	}

	pDC->FillSolidRect(bodyCanvas,clBackColor);

	//重新设置视窗位置
	OriginX=ViewWidth+ViewOrg.x;
	OriginY=ViewHigh+ViewOrg.y;
	pDC->SetWindowOrg(-OriginX,-OriginY);

	if(bdraw)
	{
		pDC->PlayMetaFile(MetaFileHandle);
		bRequireMove=false; //允许显示光标	
	}
	
	if((CurWorkSts==NEW_STATUS||(CurWorkSts==START_STATUS))&& bReDraw)
	{
		DeleteMetaFile(MetaFileHandle); 
		CMetaFileDC* ReplacementMetaFile = new CMetaFileDC(); 
		ReplacementMetaFile->Create(); 
		delete pMetaFileDC; 
		pMetaFileDC = ReplacementMetaFile; 
		bReDraw=false;
		bRequireMove=false; //允许显示光标
		DrawGImage(pDC);
		MetaFileHandle = pMetaFileDC->Close();
		ViewOrg.x=0;
		ViewOrg.y=0;
		InvalidateRect(bodyCanvas,true);
	}

}

double CGCodeDrawView::GetCurAngle(int angle)
{
   return (PI*angle)/180;
}

/****************************************************************************/
/*  函数名: GetCurRatio      												*/
/*  功  能：获取绘图比例  													*/
/*  参  数：double scale  设定的缩放比例									*/
/*  出  口：1.drawratio变量保存绘图比例										*/
/*			2.绘图比例不变返回1，否则返回0									*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
double CGCodeDrawView::GetCurRatio(double scale)
{
	double CanvasWidth,CanvasHeght;
	double ObjectWidth,ObjectHeght;
	double MaxXX,MaxYY,MinXX,MinYY;
	double XRatio;
	
	if(glcncarray.GetSize()<2)
	{
		drawRatio=1;
		return 1;
	}

	//获取绘图区尺寸参数
	CanvasWidth=fabs((double)(bodyCanvas.right-bodyCanvas.left))-120;
	CanvasHeght=fabs((double)(bodyCanvas.top-bodyCanvas.bottom))-120;
	
	//获取DOC中设置的绘图对象尺寸参数
	MaxXX=gtcard.pDoc->MaxCoord[0];
	MinXX=gtcard.pDoc->MaxCoord[1];
	MaxYY=gtcard.pDoc->MaxCoord[2];
	MinYY=gtcard.pDoc->MaxCoord[3];   
	ObjectWidth=fabs(MaxXX-MinXX);
	ObjectHeght=fabs(MaxYY-MinYY);
	
	if(ObjectWidth<0.0000001)
		XRatio=1;
	else
		XRatio=CanvasWidth/ObjectWidth; 

	if(ObjectHeght<0.0000001)
		drawRatio=1;
	else
		drawRatio=CanvasHeght/ObjectHeght;

	if(XRatio<drawRatio) drawRatio=XRatio;
      drawRatio=drawRatio*scale;
	return 0;
}

/****************************************************************************/
/*  函数名: DrawGImage      												*/
/*  功  能：绘制G代码模拟轨迹  												*/
/*  参  数：CDC *pDC  绘图窗口DC											*/
/*  出  口：																*/
/*  说  明：1.可根据当前选择绘制不同工作平面上图形							*/
/*			2.暂不提供三维显示												*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeDrawView::DrawGImage(CDC *pDC)
{
	int			i,maxnum;
	int			nViewPlnSel;
	int			nCurWorkPln;
	double		px,py,pz;
	double      dx,dy,dz;
	short int	UU;
	double		I,J,K,DLT;
	double		XX,YY;
	double      LX,LY,HX,HY;
	double		Sta,Fi;
	CenRec		CCen;
	StuAngle	ArcAngle;
	
	nCurWorkPln=XYPLANE;
	nViewPlnSel=((CMotorControlBenchApp*)AfxGetApp())->nViewPlnSel;
	Sta = GetCurAngle(X_Ror_Angle);
	Fi  = GetCurAngle(Z_Ror_Angle);
	LX=LY=100000000;
	HX=HY=-100000000;

	maxnum=glcncarray.GetSize();
	if(maxnum<2)  return;
	px=py=pz=0;
	
    //绘制坐标轴
	SelectObject(pMetaFileDC->m_hDC,curosrPen2);
	pMetaFileDC->MoveTo((int)(zeroposx-300),(int)(zeroposy));
	pMetaFileDC->LineTo((int)(zeroposx+300),(int)(zeroposy));
	pMetaFileDC->LineTo((int)(zeroposx+280),(int)(zeroposy+5));
	pMetaFileDC->MoveTo((int)(zeroposx+300),(int)(zeroposy));
	pMetaFileDC->LineTo((int)(zeroposx+280),(int)(zeroposy-5));
	
	pMetaFileDC->MoveTo((int)(zeroposx),(int)(zeroposy+300));
	pMetaFileDC->LineTo((int)(zeroposx),(int)(zeroposy-300));
	pMetaFileDC->LineTo((int)(zeroposx-5),(int)(zeroposy-280));
	pMetaFileDC->MoveTo((int)(zeroposx),(int)(zeroposy-300));
	pMetaFileDC->LineTo((int)(zeroposx+5),(int)(zeroposy-280));

	pMetaFileDC->SetTextColor(RGB(255,255,255));
	//根据不同的显示平面写坐标轴名
	switch (nViewPlnSel) 
	{
	case 0://XY平面
		pMetaFileDC->TextOut((int)(zeroposx+300),(int)(zeroposy+5),"X轴");
		pMetaFileDC->TextOut((int)(zeroposx+5),(int)(zeroposy-300),"Y轴");
		break;
	case 1://ZX平面
		pMetaFileDC->TextOut((int)(zeroposx+300),(int)(zeroposy+5),"Z轴");
		pMetaFileDC->TextOut((int)(zeroposx+5),(int)(zeroposy-300),"X轴");		
		break;
	case 2:
		pMetaFileDC->TextOut((int)(zeroposx+300),(int)(zeroposy+5),"Y轴");
		pMetaFileDC->TextOut((int)(zeroposx+5),(int)(zeroposy-300),"Z轴");
		break;
	case 3:		
		break;
	}
	pMetaFileDC->TextOut((int)(zeroposx+10),(int)(zeroposy+20),"原点");

	//画笔定位到原点
	SelectObject(pMetaFileDC->m_hDC,workDrawPen);
	pMetaFileDC->MoveTo((int)(zeroposx),(int)(zeroposy));

	//确定绘图平面
	for(i=0;i<maxnum;i++)
	{	
		switch (glcncarray[i].Code)
		{
		case 17:
			nCurWorkPln=XYPLANE;
			break;
		case 18:
			nCurWorkPln=XZPLANE;
			break;
		case 19:
			nCurWorkPln=YZPLANE;
			break;
		}
		if((glcncarray[i].Code!=0)&&(glcncarray[i].Code!=1)&&(glcncarray[i].Code!=2)&&(glcncarray[i].Code!=3)&&(glcncarray[i].Code!=92))
			continue;
       
		//选择G00或非G00指令所用画笔
		if(glcncarray[i].Code==0)
		{
			SelectObject(pMetaFileDC->m_hDC,moveDrawPen);
		}
		else
		{
			SelectObject(pMetaFileDC->m_hDC,workDrawPen);
		}

		//保存当前点坐标
		if (glcncarray[i].x==INITVALUE) 	dx=px;
		else  	dx=glcncarray[i].x;
		if (glcncarray[i].y==INITVALUE) 	dy=py;
		else  	dy=glcncarray[i].y;
		if (glcncarray[i].z==INITVALUE) 	dz=pz;
		else  	dz=glcncarray[i].z;
		
		//当G92时
		if (glcncarray[i].Code==92)
		{		
			switch (nViewPlnSel) //根据不同的显示平面确定坐标值
			{
			case 0://XY平面
				XX=dx;
				YY=dy;
				break;
			case 1://ZX平面
				XX=dz;
				YY=dx;
				break;
			case 2://YZ平面
				XX=dy;
				YY=dz;
				break;			
			}
					
			XX=zeroposx+XX*drawRatio;
			YY=zeroposy-YY*drawRatio;
			pMetaFileDC->MoveTo((int)(XX),(int)(YY));
			pMetaFileDC->TextOut((int)(XX+5),(int)(YY-5),"对刀点");
			if(XX<LX) LX=XX;
			if(YY<LY) LY=YY;
			if(XX>HX) HX=XX;
			if(YY>HY) HY=YY;
		}

		//当G00和G01时
		if ((glcncarray[i].Code==0)||(glcncarray[i].Code==1))
		{	
			switch (nViewPlnSel) //根据不同的显示平面确定坐标值
			{
			case 0://XY平面
				XX=dx;
				YY=dy;
				break;
			case 1://ZX平面
				XX=dz;
				YY=dx;
				break;
			case 2://YZ平面
				XX=dy;
				YY=dz;
				break;
			case 3:
				XX=(-1)*dx*sin(Sta)+dy*cos(Sta);
				YY=(-1)*dx*cos(Sta)*cos(Fi)-dy*sin(Sta)*cos(Fi)+dz*sin(Fi);
				break;
			}		
			
			 XX=zeroposx+XX*drawRatio;
 			 YY=zeroposy-YY*drawRatio;
			pMetaFileDC->LineTo((int)(XX),(int)(YY));
			if(XX<LX) LX=XX;
			if(YY<LY) LY=YY;
			if(XX>HX) HX=XX;
			if(YY>HY) HY=YY;
		}
		//当G02，G03时
		else if ((glcncarray[i].Code==2)||(glcncarray[i].Code==3))
		{
			I=0;   J=0;   K=0;
			if (nViewPlnSel==3) //如果显示为3D视图
			{
				if((px==dx) && (py==dy)) //如果起点等于终点（整圆）
				{
					ArcAngle.MTH=2*3.14159265;
					ArcAngle.MR=glcncarray[i].r;
					ArcAngle.MTHC=3.14159265;
					DLT=ArcAngle.MTH/MDivide;
					for (UU=0;UU<=MDivide;UU++)
					{
						
						XX=px+ArcAngle.MR+ArcAngle.MR*cos(ArcAngle.MTHC+DLT*UU);
						YY=py+ArcAngle.MR*sin(ArcAngle.MTHC+DLT*UU);				
					
						
						XX=(-1)*XX*sin(Sta)+YY*cos(Sta);
						YY=(-1)*XX*cos(Sta)*cos(Fi)-YY*sin(Sta)*cos(Fi)+dz*sin(Fi);
						
						XX=zeroposx+XX*drawRatio;
						YY=zeroposy-YY*drawRatio;
						pMetaFileDC->LineTo((int)(XX),(int)(YY));
						
						if(XX<LX) LX=XX;
						if(YY<LY) LY=YY;
						if(XX>HX) HX=XX;
						if(YY>HY) HY=YY;
					}
				}
				else //非整圆时
				{
					if (glcncarray[i].r<NOVALIDDATA)
					{
						CCen=CalculateCenter(px,py,dx,dy,glcncarray[i].r,glcncarray[i].Code);
						I=CCen.I;   J=CCen.J;   K=CCen.K;
					}
					ArcAngle=CalculateAngel(px,py,dx,dy,I,J,glcncarray[i].Code);
					DLT=ArcAngle.MTH/MDivide;
					for (UU=0;UU<=MDivide;UU++)
					{
						
						XX=px+I+ArcAngle.MR*cos(ArcAngle.MTHC+DLT*UU);
						YY=py+J+ArcAngle.MR*sin(ArcAngle.MTHC+DLT*UU);						
					
						XX=(-1)*XX*sin(Sta)+YY*cos(Sta);
						YY=(-1)*XX*cos(Sta)*cos(Fi)-YY*sin(Sta)*cos(Fi)+dz*sin(Fi);
						
						XX=zeroposx+XX*drawRatio;
						YY=zeroposy-YY*drawRatio;
						pMetaFileDC->LineTo((int)(XX),(int)(YY));
						if(XX<LX) LX=XX;
						if(YY<LY) LY=YY;
						if(XX>HX) HX=XX;
						if(YY>HY) HY=YY;
					}
				}			
			}
			else{
				switch (nCurWorkPln)
				{
				case XYPLANE:  //当工作面为XY面时
					switch(nViewPlnSel)
					{
					case XYPLANE: //当工作面为XY面且显示平面为XY面
						if((px==dx) && (py==dy)) //如果起点等于终点（整圆）
						{
							ArcAngle.MTH=2*3.14159265;
							ArcAngle.MR=glcncarray[i].r;
							ArcAngle.MTHC=3.14159265;
							DLT=ArcAngle.MTH/MDivide;
							for (UU=0;UU<=MDivide;UU++)
							{
								
								XX=px+ArcAngle.MR+ArcAngle.MR*cos(ArcAngle.MTHC+DLT*UU);
								YY=py+ArcAngle.MR*sin(ArcAngle.MTHC+DLT*UU);							
								XX=zeroposx+XX*drawRatio;
								YY=zeroposy-YY*drawRatio;
								pMetaFileDC->LineTo((int)(XX),(int)(YY));
								
								if(XX<LX) LX=XX;
								if(YY<LY) LY=YY;
								if(XX>HX) HX=XX;
								if(YY>HY) HY=YY;
							}
						}
						else //非整圆时
						{
							if (glcncarray[i].r<NOVALIDDATA)
							{
								CCen=CalculateCenter(px,py,dx,dy,glcncarray[i].r,glcncarray[i].Code);
								I=CCen.I;   J=CCen.J;   K=CCen.K;
							}
							ArcAngle=CalculateAngel(px,py,dx,dy,I,J,glcncarray[i].Code);
							DLT=ArcAngle.MTH/MDivide;
							for (UU=0;UU<=MDivide;UU++)
							{
								
								XX=px+I+ArcAngle.MR*cos(ArcAngle.MTHC+DLT*UU);
								YY=py+J+ArcAngle.MR*sin(ArcAngle.MTHC+DLT*UU);					
								
								XX=zeroposx+XX*drawRatio;
								YY=zeroposy-YY*drawRatio;
								pMetaFileDC->LineTo((int)(XX),(int)(YY));
								if(XX<LX) LX=XX;
								if(YY<LY) LY=YY;
								if(XX>HX) HX=XX;
								if(YY>HY) HY=YY;
							}
						}
						break;
					case XZPLANE://当工作面为XY面而显示平面为ZX面
						//确定横坐标
						ArcAngle=CalculateAngel(px,py,dx,dy,I,J,glcncarray[i].Code);
						CCen=CalculateCenter(px,py,dx,dy,glcncarray[i].r,glcncarray[i].Code);
						I=CCen.I;   J=CCen.J;   K=CCen.K;
						if(ArcAngle.MTH>3.14159265)
							YY=px+I+ArcAngle.MR;
						else					   
							YY=dx;		
						//确定纵坐标
						XX=pz;
						
						XX=zeroposx+XX*drawRatio;
						YY=zeroposy-YY*drawRatio;
						pMetaFileDC->LineTo((int)(XX),(int)(YY));
						if(XX<LX) LX=XX;
						if(YY<LY) LY=YY;
						if(XX>HX) HX=XX;
						if(YY>HY) HY=YY;
						break;
					case YZPLANE://当工作面为XY面而显示平面为YZ面
						XX=dy;						
						YY=pz;
						XX=zeroposx+XX*drawRatio;
						YY=zeroposy-YY*drawRatio;
						pMetaFileDC->LineTo((int)(XX),(int)(YY));
						if(XX<LX) LX=XX;
						if(YY<LY) LY=YY;
						if(XX>HX) HX=XX;
						if(YY>HY) HY=YY;
						break;
					}
					break;
					case XZPLANE:	///////////////////	工作平面为XZ	   
						switch(nViewPlnSel)
						{
						case XZPLANE://工作平面为XZ，显示平面为XZ
							if((px==dx) && (pz==dz)) //如果起点等于终点（整圆）
							{
								ArcAngle.MTH=2*3.14159265;
								ArcAngle.MR=glcncarray[i].r;
								ArcAngle.MTHC=3.14159265;
								DLT=ArcAngle.MTH/MDivide;
								for (UU=0;UU<=MDivide;UU++)
								{									
									YY=px+ArcAngle.MR+ArcAngle.MR*cos(ArcAngle.MTHC+DLT*UU);
									XX=pz+ArcAngle.MR*sin(ArcAngle.MTHC+DLT*UU);									
									XX=zeroposx+XX*drawRatio;
									YY=zeroposy-YY*drawRatio;
									pMetaFileDC->LineTo((int)(XX),(int)(YY));
									
									if(XX<LX) LX=XX;
									if(YY<LY) LY=YY;
									if(XX>HX) HX=XX;
									if(YY>HY) HY=YY;
								}
							}
							else
							{
								if (glcncarray[i].r<NOVALIDDATA)
								{
									CCen=CalculateCenter(pz,px,dz,dx,glcncarray[i].r,glcncarray[i].Code);
									I=CCen.I;   J=CCen.J;   K=CCen.K;
								}					
								ArcAngle=CalculateAngel(pz,px,dz,dx,I,J,glcncarray[i].Code);
								DLT=ArcAngle.MTH/MDivide;
								for (UU=0;UU<=MDivide;UU++)
								{									
									XX=pz+I+ArcAngle.MR*cos(ArcAngle.MTHC+DLT*UU);
									YY=px+J+ArcAngle.MR*sin(ArcAngle.MTHC+DLT*UU);					
										
									XX=zeroposx+XX*drawRatio;
									YY=zeroposy-YY*drawRatio;
									pMetaFileDC->LineTo((int)(XX),(int)(YY));
									if(XX<LX) LX=XX;
									if(YY<LY) LY=YY;
									if(XX>HX) HX=XX;
									if(YY>HY) HY=YY;
								}
							}
							break;
						case XYPLANE://工作平面为XZ，显示平面为XY
							
							ArcAngle=CalculateAngel(px,py,dx,dy,I,J,glcncarray[i].Code);
							CCen=CalculateCenter(px,py,dx,dy,glcncarray[i].r,glcncarray[i].Code);
							I=CCen.I;   J=CCen.J;   K=CCen.K;
							if(ArcAngle.MTH>3.14159265)
								XX=px+I+ArcAngle.MR;
							else					   
								XX=dx;				   
							YY=py;
							XX=zeroposx+XX*drawRatio;
							YY=zeroposy-YY*drawRatio;
							pMetaFileDC->LineTo((int)(XX),(int)(YY));
							if(XX<LX) LX=XX;
							if(YY<LY) LY=YY;
							if(XX>HX) HX=XX;
							if(YY>HY) HY=YY;
							break;
						case YZPLANE://工作平面为XZ，显示平面为YZ
							
							YY=dz;					   
							XX=py;					 
							XX=zeroposx+XX*drawRatio;
							YY=zeroposy-YY*drawRatio;
							pMetaFileDC->LineTo((int)(XX),(int)(YY));
							if(XX<LX) LX=XX;
							if(YY<LY) LY=YY;
							if(XX>HX) HX=XX;
							if(YY>HY) HY=YY;
							break;
						}
						break;
						case YZPLANE://工作平面为YZ
							switch(nViewPlnSel)
							{
							case YZPLANE://工作平面为YZ，显示平面为YZ
								if((px==dy) && (pz==dz)) //如果起点等于终点（整圆）
								{
									ArcAngle.MTH=2*3.14159265;
									ArcAngle.MR=glcncarray[i].r;
									ArcAngle.MTHC=3.14159265;
									//I=0;J=0;K=0;
									DLT=ArcAngle.MTH/MDivide;
									for (UU=0;UU<=MDivide;UU++)
									{
										
										XX=py+ArcAngle.MR+ArcAngle.MR*cos(ArcAngle.MTHC+DLT*UU);
										YY=pz+ArcAngle.MR*sin(ArcAngle.MTHC+DLT*UU);								
									
										XX=zeroposx+XX*drawRatio;
										YY=zeroposy-YY*drawRatio;
										pMetaFileDC->LineTo((int)(XX),(int)(YY));
										
										if(XX<LX) LX=XX;
										if(YY<LY) LY=YY;
										if(XX>HX) HX=XX;
										if(YY>HY) HY=YY;
									}
								}
								else
								{
									if (glcncarray[i].r<NOVALIDDATA)
									{
										CCen=CalculateCenter(py,pz,dy,dz,glcncarray[i].r,glcncarray[i].Code);
										I=CCen.I;   J=CCen.J;   K=CCen.K;
									}
									ArcAngle=CalculateAngel(py,pz,dy,dz,I,J,glcncarray[i].Code);
									DLT=ArcAngle.MTH/MDivide;
									for (UU=0;UU<=MDivide;UU++)
									{										
										XX=py+I+ArcAngle.MR*cos(ArcAngle.MTHC+DLT*UU);
										YY=pz+J+ArcAngle.MR*sin(ArcAngle.MTHC+DLT*UU);
										
										XX=zeroposx+XX*drawRatio;
										YY=zeroposy-YY*drawRatio;
										pMetaFileDC->LineTo((int)(XX),(int)(YY));
										if(XX<LX) LX=XX;
										if(YY<LY) LY=YY;
										if(XX>HX) HX=XX;
										if(YY>HY) HY=YY;
									}
								}
								break;
							case XYPLANE://工作平面为YZ，显示平面为XY
								
								ArcAngle=CalculateAngel(px,py,dx,dy,I,J,glcncarray[i].Code);
								CCen=CalculateCenter(px,py,dx,dy,glcncarray[i].r,glcncarray[i].Code);
								I=CCen.I;   J=CCen.J;   K=CCen.K;
								if(ArcAngle.MTH>3.14159265)
									YY=py+I+ArcAngle.MR;
								else					   
									YY=dy;									
								XX=px;								
								XX=zeroposx+XX*drawRatio;
								YY=zeroposy-YY*drawRatio;
								pMetaFileDC->LineTo((int)(XX),(int)(YY));
								if(XX<LX) LX=XX;
								if(YY<LY) LY=YY;
								if(XX>HX) HX=XX;
								if(YY>HY) HY=YY;
								break;
							case XZPLANE://工作平面为YZ，显示平面为ZX								
								XX=dz;								
								YY=px;
								XX=zeroposx+XX*drawRatio;
								YY=zeroposy-YY*drawRatio;
								pMetaFileDC->LineTo((int)(XX),(int)(YY));
								if(XX<LX) LX=XX;
								if(YY<LY) LY=YY;
								if(XX>HX) HX=XX;
								if(YY>HY) HY=YY;
								break;
							}
							break;
			}
			}
		}
		
		px=dx;		py=dy;		pz=dz;
		}

	
	if(LX==100000000||HX==-100000000)
		ViewWidth=0;
	else
		ViewWidth=zeroposx-(LX+(int)(HX-LX)/2);
		
	if(LY==100000000||HY==-100000000)
		ViewHigh=0;
	else
		ViewHigh=zeroposy-(HY-(int)(HY-LY)/2);
}

/****************************************************************************/
/*  函数名: CalculateCenter    												*/
/*  功  能：计算圆弧圆心  													*/
/*  参  数：																*/
/*			1.double Xs,double Ys  圆弧起点坐标								*/
/*			2.double Xe,double Ye  圆弧终点坐标								*/	
/*			3.double R			   圆弧半径									*/
/*			4.int mode			   圆弧模式									*/
/*  出  口：1.计算结果保存在CenRec型变量Result中							*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
CenRec CGCodeDrawView::CalculateCenter(double Xs,double Ys,double Xe,double Ye,double R,int mode)
{
	CenRec Result;
	double Xc,Yc;
	int dir=0;

	if(mode==3)
		dir=1;
	IN_Center(Xs,Ys,&Xe,&Ye,R,&Xc,&Yc,dir);
	Result.I=Xc-Xs;
	Result.J=Yc-Ys;
	return Result;
}

/****************************************************************************/
/*  函数名: IN_Center    													*/
/*  功  能：根据圆弧起点、终点、半径计算圆弧圆心坐标						*/
/*  参  数：																*/
/*			1.double Xs,double Ys   圆弧起点坐标							*/
/*			2.double* Xe,double Ye  圆弧终点坐标							*/	
/*			3.double r			    圆弧半径								*/
/*			4.double *xc ,yc	    圆心坐标								*/
/*			5.int cw				圆弧方向								*/
/*  出  口：圆心坐标返回至*xc ,*yc											*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
short CGCodeDrawView::IN_Center(double xs,double ys,double *xe,double *ye,double r,double *xc,double *yc,int cw)
{
   int flag1=(r<0) ?  -1:1;
   int flag2=(cw)?     1:-1;
   double dx=*xe-xs,dy=*ye-ys;
   double length=sqrt(dx*dx+dy*dy);
   r=fabs(r);
   if(r < 1e-12) 
   {
	   return -1;
   }
   if( length<1e-12 ) 
   {
	   return -1;
   }
   if( length-2*r > 1e-12 )
   {
	   return -1;
   }
   dx=dx/length;
   dy=dy/length;
   length=sqrt(r*r-length*length/4);
   *xc=(*xe+xs)/2-flag1*flag2*dy*length;
   *yc=(*ye+ys)/2+flag1*flag2*dx*length;
   return 0;
}

/****************************************************************************/
/*  函数名: CalculateAngel 													*/
/*  功  能：计算圆弧插补角度												*/
/*  参  数：																*/
/*			1.double Xs,double Ys  圆弧起点坐标								*/
/*			2.double Xe,double Ye  圆弧终点坐标								*/	
/*			3.double I，J		   圆心坐标增量								*/
/*			4.GCode			       G代码号									*/
/*  出  口：计算结果保存在StuAngle型变量Result中							*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
StuAngle CGCodeDrawView::CalculateAngel (double Xs,double Ys,double Xe,double Ye,double I,double J,int GCode)
{
	StuAngle  Result;
	double II,JJ,R,C,TH,THC,JieJu;
	 
     memset(&Result,0,sizeof(StuAngle));
     TH=0.0;
     THC=0.0;
     R=sqrt(I*I+J*J);
     C=sqrt((Xe-Xs)*(Xe-Xs)+(Ye-Ys)*(Ye-Ys));
     II=Xe-Xs;
     JJ=Ye-Ys;

     if ((I==0) && (J==0))  return Result;
     else if ((I==0) && (J!=0))
		THC=PI+asin(J/R);
     else if ((I!=0) && (J==0))
         THC=acos(-I/R);
     else if ((I!=0) && (J!=0))
     {
               //PI<=TH<1.5*PI
         if ((I>0) && (J>0))     THC= PI+asin(J/R);  //confirmed
               // 0.5*PI---PI
         if ((I>0) && (J<0))     THC= acos(-I/R);     //confirmed
               //1.5-2 (PI)
         if ((I<0) && (J>0))     THC= 2*PI-asin(J/R);  //confirmed
               //  0-0.5(PI)
         if ((I<0) && (J<0))     THC= asin(-J/R);     //confirmed
	 }

     JieJu=0.0;
	 {
		 if((Xe-Xs)==0)
			JieJu=0.0;
		 else
			JieJu=((Ye-Ys)/(Xe-Xs))*I-J;
	 }


     if ((II>0) && (JJ>0))
	 {
        if (JieJu>0)
		{
           if (GCode==2)  TH=-acos(1-(C*C)/(R*R)/2);
           if (GCode==3)  TH=2*PI-acos(1-(C*C)/(R*R)/2);
		}
        else
        {
           if (GCode==3)  TH=acos(1-(C*C)/(R*R)/2);
           if (GCode==2)  TH=-(2*PI-acos(1-(C*C)/(R*R)/2));
		}
	 }
     if ((II<0) && (JJ>0))
     {
        if (JieJu>0)
		{
           if (GCode==3)  TH=acos(1-(C*C)/(R*R)/2);
           if (GCode==2)  TH=-(2*PI-acos(1-(C*C)/(R*R)/2));
		}
        else
        {
           if (GCode==2)  TH=-acos(1-(C*C)/(R*R)/2);
           if (GCode==3)  TH=2*PI-acos(1-(C*C)/(R*R)/2);
		}
	 }

     if ((II==0) && (JJ>0))
     {
        if (I<=0)
        {
           if (GCode==3)  TH=acos(1-(C*C)/(R*R)/2);
           if (GCode==2)  TH=-(2*PI-acos(1-(C*C)/(R*R)/2));
		}
        if (I>0)
		{
           if (GCode==2)  TH=-acos(1-(C*C)/(R*R)/2);
           if (GCode==3)  TH=(2*PI-acos(1-(C*C)/(R*R)/2));
		}
	 }

     if ((II==0) && (JJ<0))
     {
        if (I<=0)
		{
           if (GCode==2)  TH=-acos(1-(C*C)/(R*R)/2);
           if (GCode==3)  TH=(2*PI-acos(1-(C*C)/(R*R)/2));
		}
        if (I>0)
		{
           if (GCode==3)  TH=acos(1-(C*C)/(R*R)/2);
           if (GCode==2)  TH=-(2*PI-acos(1-(C*C)/(R*R)/2));
		}
	 }


	 if ((II<0) && (JJ<=0))
     {
        if (JieJu>0) 
        {
           if (GCode==3)  TH=acos(1-(C*C)/(R*R)/2);
           if (GCode==2)  TH=-(2*PI-acos(1-(C*C)/(R*R)/2));
		}
        else
        {
           if (GCode==2)  TH=-acos(1-(C*C)/(R*R)/2);
           if (GCode==3)  TH=2*PI-acos(1-(C*C)/(R*R)/2);
		}

	 }
      
	 if ((II>0) && (JJ<=0)) 
	 {
         if (JieJu>0)
		 {
           if (GCode==2)  TH=-acos(1-(C*C)/(R*R)/2);
           if (GCode==3)  TH=2*PI-acos(1-(C*C)/(R*R)/2);
		 }
         else
         {
           if (GCode==3)  TH=acos(1-(C*C)/(R*R)/2);
           if (GCode==2)  TH=-(2*PI-acos(1-(C*C)/(R*R)/2));
		 }
	 }

	 Result.MTH=TH;
     Result.MTHC=THC;
     Result.MR  =R;

	 return Result;

}


/////////////////////////////////////////////////////////////////////////////
// CGCodeDrawView diagnostics

#ifdef _DEBUG
void CGCodeDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CGCodeDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGCodeDrawView message handlers


void CGCodeDrawView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    LBtnDown=1;
	orgpoint=point;
	SetCursor(LoadCursor(NULL,IDC_SIZEALL));
	CView::OnLButtonDown(nFlags, point);
}

void CGCodeDrawView::OnLButtonUp(UINT nFlags, CPoint point) 
{

    if ((LBtnDown==1)&&((abs(point.x-orgpoint.x)>5) || (abs(point.y-orgpoint.y)>5)))
	{
		int StepX,StepY;
		StepX=point.x-orgpoint.x;
		StepY=point.y-orgpoint.y;
		ViewOrg.x+=StepX;
		ViewOrg.y+=StepY;
		InvalidateRect(NULL,true);
	}
	LBtnDown=0;
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	CView::OnLButtonUp(nFlags, point);
}

void CGCodeDrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(LBtnDown==1) 
	{
		SetCapture();
		if (bodyCanvas.PtInRect(point))
		{   
			if (LBtnDown!=1)	ReleaseCapture();
			else	SetCursor(LoadCursor(NULL,IDC_SIZEALL));
		
		}
		else
		{
			LBtnDown=0;
			ReleaseCapture();
		}
	}
	else
		ReleaseCapture();
	CView::OnMouseMove(nFlags, point);
}

/****************************************************************************/
/*  函数名: DrawCourse      												*/
/*  功  能：绘制G代码实时轨迹十字光标										*/
/*  参  数：double X,double Y,double Z 当前G代码轨迹运动点坐标				*/
/*  出  口：																*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeDrawView::DrawCourse(double X,double Y,double Z)
{
	int		i;
	int nView;
	double	XX,YY,SX,SY;
	double Sta,Fi;
	CClientDC cdc(this);
	int    penmode;
    
	
	Sta = GetCurAngle(X_Ror_Angle);
	Fi = GetCurAngle(Z_Ror_Angle);

	nView=((CMotorControlBenchApp*)AfxGetApp())->nViewPlnSel;
	penmode=SetROP2(cdc.m_hDC,R2_NOTXORPEN);
	for(i=0;i<2;i++)
	{
		if ((lastX==INITVALUE) && (lastY==INITVALUE) && (lastZ==INITVALUE)&&(i==0))
		{
			continue;
		}
		else
		{
			if (i==0)
			{
				if(nView!=3){
				SX=lastX;
				SY=lastY;
				}
				else
				{
				SX=(-1)*lastX*sin(Sta)+lastY*cos(Sta);
				SY=(-1)*lastX*cos(Sta)*cos(Fi)-lastY*sin(Sta)*cos(Fi)+lastZ*sin(Fi);
				}
			}
			else
			{
			switch(nView)

				{
				case 0:				
					SX=X;
					SY=Y;
					break;
				case 1:
					SX=Z;
					SY=X;
					break;
				case 2:
					SX=Y;
					SY=Z;
					break;
				case 3:
					SX=(-1)*X*sin(Sta)+Y*cos(Sta);
					SY=(-1)*X*cos(Sta)*cos(Fi)-Y*sin(Sta)*cos(Fi)+Z*sin(Fi);
					break;
				}		
			}

			XX=zeroposx+SX*drawRatio;
			YY=zeroposy-SY*drawRatio;

			SelectObject(cdc.m_hDC,curosrPen1);
			cdc.MoveTo((int)(XX+OriginX),(int)(YY+OriginY)-15);
			cdc.LineTo((int)(XX+OriginX),(int)(YY+OriginY)+15);
			cdc.MoveTo((int)(XX+OriginX)-15,(int)(YY+OriginY));
			cdc.LineTo((int)(XX+OriginX)+15,(int)(YY+OriginY));

			SelectObject(cdc.m_hDC,curosrPen1);
			cdc.MoveTo((int)(XX+OriginX)-2,(int)(YY+OriginY)-2);
			cdc.LineTo((int)(XX+OriginX)+2,(int)(YY+OriginY)-2);
			cdc.LineTo((int)(XX+OriginX)+2,(int)(YY+OriginY)+2);
			cdc.LineTo((int)(XX+OriginX)-2,(int)(YY+OriginY)+2);
			cdc.LineTo((int)(XX+OriginX)-2,(int)(YY+OriginY)-2);
		}
	}
	SetROP2(cdc.m_hDC,penmode);
	switch(nView)
	{
	case 0:
		lastX=X;   lastY=Y;   lastZ=Z;
		break;
	case 1:
		lastX=Z;   lastY=X;   lastZ=Y;
		break;
	case 2:
		lastX=Y;   lastY=Z;   lastZ=X;
		break;
	case 3:
		lastX=X;   lastY=Y;   lastZ=Z;
		break;
	} 
}


void CGCodeDrawView::OnTimer(UINT nIDEvent) 
{
	if((!gtcard.Drawing)||(bRequireMove)) return;
	DrawWorkLine(1); 
	CView::OnTimer(nIDEvent);
}


int CGCodeDrawView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(1,50,NULL);	//debug
	return 0;
}

BOOL CGCodeDrawView::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect; 
	pDC->GetClipBox(rect); 
	if((rect.left!=0)||(rect.top!=0)||(rect.right!=0)||(rect.bottom!=0))
		return false;
	
	return true;
}

/****************************************************************************/
/*  函数名: GetCurPostion      												*/
/*  功  能：获取绘图原点位置												*/
/*  参  数：无																*/
/*  出  口：zeroposx,zeroposy为原点坐标										*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeDrawView::GetCurPostion()
{

	zeroposx=fabs((double)(bodyCanvas.right-bodyCanvas.left))/2-10;
	zeroposy=fabs((double)(bodyCanvas.top-bodyCanvas.bottom))/2-10;
}


/****************************************************************************/
/*  函数名: DrawWorkLine      												*/
/*  功  能：绘制G代码实时轨迹												*/
/*  参  数：int bIsFirst 判断是否第一次绘制									*/
/*  出  口：																*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeDrawView::DrawWorkLine(int bIsFirst)
{
	int nViewPlnSel;
	double dx,dy,dz;
	double actlPos[4];
	double XX,YY;
	double Sta,Fi;
	static double lastX,lastY;
	static int i(1);
	CClientDC dc(this);	

	Sta = GetCurAngle(X_Ror_Angle);
	Fi  = GetCurAngle(Z_Ror_Angle);
    nViewPlnSel=((CMotorControlBenchApp*)AfxGetApp())->nViewPlnSel;
	OriginX=ViewWidth+ViewOrg.x;
	OriginY=ViewHigh+ViewOrg.y;
	dc.SetWindowOrg(-OriginX,-OriginY);
	SelectObject(dc.m_hDC,curosrPen1);
	if(i) 
	{
		lastX=zeroposx;
		lastY=zeroposy;
		dc.MoveTo((int)(lastX),(int)(lastY));
        i=0;
	}		
	
	gtcard.GetEncPos(actlPos);
	dx=actlPos[m_xMap];
	dy=actlPos[m_yMap];
	dz=actlPos[m_zMap];
	TRACE("dx=%ld,dy=%ld,dz=%ld\n,",dx,dy,dz);
	switch (nViewPlnSel)
	{
	case 0://XY平面
		XX=dx;
		YY=dy;
		break;
	case 1://ZX平面
		XX=dz;
		YY=dx;
		break;
	case 2://YZ平面
		XX=dy;
		YY=dz;
		break;			
	}
			 
	XX=zeroposx+XX*drawRatio;
	YY=zeroposy-YY*drawRatio;		 
	
	dc.MoveTo((int)(lastX),(int)(lastY));
	dc.LineTo((int)(XX),(int)(YY));	
	
	DrawCourse(actlPos[m_xMap],actlPos[m_yMap],actlPos[m_zMap]);
	
	lastX=XX;
	lastY=YY; 
}
