// Interpolation.cpp: implementation of the CInterpolation class.
//
//////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "Interpolation.h"
#include "math.h"
#include "GTCard.h"
#include "GT400.h"

extern CGTCard gtcard;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInterpolation::CInterpolation()
{

}

CInterpolation::~CInterpolation()
{

}

/****************************************************************************/
/*  函数名: LineInterp_MIT      											*/
/*  功  能：逐点比较法直线插补(非缓冲区指令型)								*/
/*  参  数：long XEnd, long YEnd  插补终点									*/
/*			int step 插补步长												*/
/*  出  口：																*/
/*  说  明：此函数用板卡单轴命令执行										*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::LineInterp_MIT(int step, long XEnd, long YEnd) //逐点比较法直线插补
{
	long lDevVal;
	long xCurVal(0),yCurVal(0);
	long StepMount,StepCount(0);
	unsigned short nDir;
	
    nDir=Judge_Quadrant(XEnd,YEnd);//象限判断	
	
	XEnd=abs(XEnd);
	YEnd=abs(YEnd);	

	StepMount=(XEnd+YEnd)/step;
	lDevVal=yCurVal*XEnd-xCurVal*YEnd;

	
    while (StepCount<StepMount) {//终点判别  	
		
		if (lDevVal>=0) { //偏差〉=0
			switch(nDir) {
			case 1:
				xCurVal+=step;	
				Axis_Step(3,step);
				break;
			case 2:
				xCurVal-=step;
				Axis_Step(3,-step);
				break;
			case 3:
                xCurVal-=step;
				Axis_Step(3,-step);
				break;
			case 4:
				xCurVal+=step;
				Axis_Step(3,step);
				break;
			}
			lDevVal-=YEnd;
		}
		else{  //偏差<0
			switch(nDir) {
			case 1:
				yCurVal+=step;
				Axis_Step(2,step);
				break;
			case 2:
				yCurVal+=step;
				Axis_Step(2,step);
				break;
			case 3:
				yCurVal-=step;
				Axis_Step(2,-step);
				break;
			case 4:
				yCurVal-=step;
				Axis_Step(2,-step);
				break;
			}			
			lDevVal+=XEnd;
		}
		StepCount++;
    }

}


/****************************************************************************/
/*  函数名: CirInterp_MIT      												*/
/*  功  能：逐点比较法圆弧插补(非缓冲区指令型)								*/
/*  参  数：long XStart, long YStart  插补起点								*/
/*          long XEnd, long YEnd  插补终点									*/
/*			long radius			插补圆弧									*/		
/*			bool bIsCW			插补方向									*/
/*			int step			插补步长									*/
/*  出  口：																*/
/*  说  明：此函数用板卡单轴命令执行										*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::CirInterp_MIT(long step, long XStart, long YStart, long XEnd, long YEnd,long radius,bool bIsCW)
{
	long StepAmount,stepCount(0);
//	long xDist,yDist;
	long xCurVal,yCurVal;
	long lDevVal;
//	long lDistToCenter;
	int   nQuart;
	int Axis_X(2),Axis_Y(3);
	
	
    xCurVal=XStart;
	yCurVal=YStart;
    StepAmount=(abs(XStart-XEnd)+abs(YStart-YEnd))/step;//计算总步数
    lDevVal=xCurVal*xCurVal+yCurVal*yCurVal-radius*radius;  //计算偏差

	if(bIsCW){ //顺圆		
		while( (xCurVal!=XEnd) ||(yCurVal!=YEnd)){  //终点判别
			nQuart=Judge_Quadrant(xCurVal,yCurVal);//获取当前动点象限
			if (lDevVal>=0) {	//当动点偏差〉0		
				switch(nQuart) {
				case 1:         //第一象限
					lDevVal=lDevVal-2*yCurVal+step;
					yCurVal-=step;
					Axis_Step(Axis_Y,-step);
					break;
				case 2:
					lDevVal=lDevVal+2*xCurVal+step;
					xCurVal+=step;
					Axis_Step(Axis_X,step);
					break;
				case 3:
					lDevVal=lDevVal+2*yCurVal+step;
					yCurVal+=step;	
					Axis_Step(Axis_Y,step);
					break;
				case 4:
					lDevVal=lDevVal-2*xCurVal+step;
					xCurVal-=step;
					Axis_Step(Axis_X,-step);
					break;		
				}
			}
			else{  	//当动点偏差<0	
				switch(nQuart) {
				case 1:         //第一象限
					lDevVal=lDevVal+2*xCurVal+step;
					xCurVal+=step;
					Axis_Step(Axis_X,step);
					break;
				case 2:
					lDevVal=lDevVal+2*yCurVal+step;
					yCurVal+=step;
					Axis_Step(Axis_Y,step);
					break;
				case 3:
					lDevVal=lDevVal-2*xCurVal+step;
					xCurVal-=step;
					Axis_Step(Axis_X,-step);
					break;
				case 4:
					lDevVal=lDevVal-2*yCurVal+step;
					yCurVal-=step;
					Axis_Step(Axis_Y,-step);
					break;		
				}
			}
			stepCount++;
		}
	}
	else{ //逆圆
		while( (xCurVal!=XEnd) ||(yCurVal!=YEnd)){  //终点判别
			nQuart=Judge_Quadrant(xCurVal,yCurVal);//获取当前动点象限
			if (lDevVal>=0) {	//当动点偏差〉0		
				switch(nQuart) {
				case 1:         //第一象限
					lDevVal=lDevVal-2*xCurVal+step;
					xCurVal-=step;
					Axis_Step(Axis_X,-step);
					break;
				case 2:
					lDevVal=lDevVal-2*yCurVal+step;
					yCurVal-=step;
					Axis_Step(Axis_Y,-step);
					break;
				case 3:
					lDevVal=lDevVal+2*xCurVal+step;
					xCurVal+=step;	
					Axis_Step(Axis_X,step);
					break;
				case 4:
					lDevVal=lDevVal+2*yCurVal+step;
					yCurVal+=step;
					Axis_Step(Axis_Y,step);
					break;		
				}
			}
			else{  	//当动点偏差<0	
				switch(nQuart) {
				case 1:         //第一象限
					lDevVal=lDevVal+2*yCurVal+step;
					yCurVal+=step;
					Axis_Step(Axis_Y,step);
					break;
				case 2:
					lDevVal=lDevVal-2*xCurVal+step;
					xCurVal-=step;
					Axis_Step(Axis_X,-step);
					break;
				case 3:
					lDevVal=lDevVal-2*yCurVal+step;
					yCurVal-=step;	
					Axis_Step(Axis_Y,-step);
					break;
				case 4:
					lDevVal=lDevVal+2*xCurVal+step;
					xCurVal+=step;
					Axis_Step(Axis_X,step);
					break;		
				}
			}
			stepCount++;
		}		
	}	
    
}

/****************************************************************************/
/*  函数名: Judge_Quadrant		      										*/
/*  功  能：判断当前点所在象限												*/
/*  参  数： long x, long y  当前点坐标										*/
/*  出  口： 返回当前点坐标所在象限											*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
unsigned short CInterpolation::Judge_Quadrant(long x, long y)
{
	unsigned short nDir;
	
	if (x>=0) { //象限判断
		if (y>=0) {
			nDir=1;	
			return 1;
		}
		else {
			nDir=4;
			return 4;
		}
	}
	else {
		if (y>=0) {
			nDir=2;
			return 2;
		}
		else {
			nDir=3;
			return 3;
		}
	}

}

void CInterpolation::Axis_Step(int nAxis, int nStep)
{
	unsigned short rtn,sts=0;
	rtn=GT_ClrSts();
	   rtn=GT_GetSts(&sts);		
	   do {		 
		   rtn=GT_GetSts(&sts);
		   sts=sts&0x400;
	   } while(sts);	 
	   gtcard.SingleAxisMove(nAxis,nStep);    
	   
}

/****************************************************************************/
/*  函数名: CirInterp_MIT_Buffer      										*/
/*  功  能：逐点比较法圆弧插补(缓冲区指令型)								*/
/*  参  数： long XStart, long YStart  插补起点								*/
/*          long XEnd, long YEnd  插补终点									*/
/*			long radius			插补圆弧									*/		
/*			bool bIsCW			插补方向									*/
/*			long step			插补步长									*/
/*  出  口：																*/
/*  说  明：1.此函数用板卡缓冲区坐标系命令方式执行轴运动					*/
/*			2.只提供XY平面圆弧插补											*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::CirInterp_MIT_Buffer(long step, long XStart, long YStart, long XEnd, long YEnd, long radius, bool bIsCW)
{
	long StepAmount,stepCount(0);
	long xCurVal,yCurVal;
	long lDevVal;
	int   nQuart;
	short rtn=0;
	
    xCurVal=XStart;
	yCurVal=YStart;
    StepAmount=(abs(XStart-XEnd)+abs(YStart-YEnd))/step;//计算总步数
    lDevVal=xCurVal*xCurVal+yCurVal*yCurVal-radius*radius;  //计算偏差	
	rtn=GT_SetSynAcc(0.1);
	rtn=GT_SetSynVel(2);
	for(int i=0;i<4;i++)
	{
		rtn=GT_Axis(i);
		rtn=GT_ZeroPos();
	}
	
	rtn=GT_StrtList();
	rtn=GT_MvXY(XStart,YStart,2,0.1);

	if(bIsCW){ //顺圆		
		while( (xCurVal!=XEnd) ||(yCurVal!=YEnd)){  //终点判别
			nQuart=Judge_Quadrant(xCurVal,yCurVal);//获取当前动点象限
			if (lDevVal>=0) {	//当动点偏差〉0		
				switch(nQuart) {
				case 1:         //第一象限
					lDevVal=lDevVal-2*yCurVal+step;
					yCurVal-=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 2:
					lDevVal=lDevVal+2*xCurVal+step;
					xCurVal+=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 3:
					lDevVal=lDevVal+2*yCurVal+step;
					yCurVal+=step;	
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 4:
					lDevVal=lDevVal-2*xCurVal+step;
					xCurVal-=step;
					GT_LnXY(xCurVal,yCurVal);
					break;		
				}
			}
			else{  	//当动点偏差<0	
				switch(nQuart) {
				case 1:         //第一象限
					lDevVal=lDevVal+2*xCurVal+step;
					xCurVal+=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 2:
					lDevVal=lDevVal+2*yCurVal+step;
					yCurVal+=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 3:
					lDevVal=lDevVal-2*xCurVal+step;
					xCurVal-=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 4:
					lDevVal=lDevVal-2*yCurVal+step;
					yCurVal-=step;
					GT_LnXY(xCurVal,yCurVal);
					break;		
				}
			}
		  //  ProcessMessage();
			stepCount++;
		}
	}
	else{ //逆圆
		while( (xCurVal!=XEnd) ||(yCurVal!=YEnd)){  //终点判别
			nQuart=Judge_Quadrant(xCurVal,yCurVal);//获取当前动点象限
			if (lDevVal>=0) {	//当动点偏差〉0		
				switch(nQuart) {
				case 1:         //第一象限
					lDevVal=lDevVal-2*xCurVal+step;
					xCurVal-=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 2:
					lDevVal=lDevVal-2*yCurVal+step;
					yCurVal-=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 3:
					lDevVal=lDevVal+2*xCurVal+step;
					xCurVal+=step;	
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 4:
					lDevVal=lDevVal+2*yCurVal+step;
					yCurVal+=step;
					GT_LnXY(xCurVal,yCurVal);
					break;		
				}
			}
			else{  	//当动点偏差<0	
				switch(nQuart) {
				case 1:         //第一象限
					lDevVal=lDevVal+2*yCurVal+step;
					yCurVal+=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 2:
					lDevVal=lDevVal-2*xCurVal+step;
					xCurVal-=step;
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 3:
					lDevVal=lDevVal-2*yCurVal+step;
					yCurVal-=step;	
					GT_LnXY(xCurVal,yCurVal);
					break;
				case 4:
					lDevVal=lDevVal+2*xCurVal+step;
					xCurVal+=step;
					GT_LnXY(xCurVal,yCurVal);
					break;		
				}
			}		
		//	ProcessMessage();
			stepCount++;
		}		
	}	
	rtn=GT_EndList();
	rtn=GT_StrtMtn();
}

/****************************************************************************/
/*  函数名: LineInterp_MIT_Buffer      										*/
/*  功  能：逐点比较法直线插补(缓冲区指令型)								*/
/*  参  数：long XEnd, long YEnd  插补终点									*/
/*			int step 插补步长												*/
/*  出  口：																*/
/*  说  明：此函数用板卡缓冲区坐标系命令方式执行轴运动						*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::LineInterp_MIT_Buffer(int step, long XEnd, long YEnd)
{
	long lDevVal;				      //偏差值
	long xCurVal(0),yCurVal(0);       //当前点的实际位置
	long StepMount;                   //插补总步数
	long StepCount(0);			      //插补次数计数器
	unsigned short nDir;			  //当前点所在象限
	
    nDir=Judge_Quadrant(XEnd,YEnd);   //象限判断	
	
	XEnd=abs(XEnd);
	YEnd=abs(YEnd);	
	
	StepMount=(XEnd+YEnd)/step;
	lDevVal=yCurVal*XEnd-xCurVal*YEnd;	

	Start_LineInterp(0,0);       //插补前准备
    while (StepCount<StepMount) {//终点判别		
		
		if (lDevVal>=0) { //偏差〉=0
			switch(nDir) {
			case 1:
				xCurVal+=step;
				GT_LnXY(xCurVal,yCurVal);			
				break;
			case 2:
				xCurVal-=step;
				GT_LnXY(xCurVal,yCurVal);			
				break;
			case 3:
                xCurVal-=step;
				GT_LnXY(xCurVal,yCurVal);			
				break;
			case 4:
				xCurVal+=step;
				GT_LnXY(xCurVal,yCurVal);			
				break;
			}
			lDevVal-=YEnd;
		}
		else{  //偏差<0
			switch(nDir) {
			case 1:
				yCurVal+=step;
				GT_LnXY(xCurVal,yCurVal);		
				break;
			case 2:
				yCurVal+=step;
				GT_LnXY(xCurVal,yCurVal);		
				break;
			case 3:
				yCurVal-=step;
				GT_LnXY(xCurVal,yCurVal);		
				break;
			case 4:
				yCurVal-=step;
				GT_LnXY(xCurVal,yCurVal);			
				break;
			}			
			lDevVal+=XEnd;
		}
	//	ProcessMessage();
		StepCount++;
    }

	End_Interpolation();

}

/****************************************************************************/
/*  函数名: LineInterp_DDA_Buffer      										*/
/*  功  能：DDA直线插补(缓冲区指令型)										*/
/*  参  数：long x, y,z  插补终点											*/
/*			int step      插补步长											*/
/*          int  n       插补累加器位数									    */
/*  出  口：																*/
/*  说  明：1.此函数用板卡缓冲区坐标系命令方式执行轴运动					*/
/*          2. 目前只提供XY平面插补											*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::LineInterp_DDA_Buffer(int n,int step,long x, long y,long z)
{
	long XRes,YRes; //寄存器溢出后余数
	long xEnd,yEnd;   //插补终点值
	long XCur(0),YCur(0),ZCur(0); //当前位置
	int IPCount=0;//累加值
	int nQuadrant;//象限
	int Q;  //累加器容量
	bool bInterpXEnable,bInterpYEnable;

	XRes=YRes=0;
	bInterpXEnable=bInterpYEnable=false;
	xEnd=abs(x);yEnd=abs(y);
	Q=pow((double)2,(int)n);

	nQuadrant=Judge_Quadrant(x,y);//计算插补象限
      Start_LineInterp(0,0);//插补前准备

	switch(nQuadrant) {
	case 1:
		while (IPCount<Q) {
			XRes+=xEnd/step;
			YRes+=yEnd/step;				
		
			if (XRes>=Q) {				
				XRes=XRes%Q;
				XCur+=step;
				bInterpXEnable=true;				
			}

			if (YRes>=Q) {
				YRes=YRes%Q;
				YCur+=step;	
				bInterpYEnable=true;
				
			}
			if (bInterpXEnable||bInterpYEnable) {
					GT_LnXY(XCur,YCur);
					bInterpXEnable=bInterpYEnable=false;
			}
           IPCount++;         
		}
		break;
	case 2:
		while (IPCount<Q) {
			XRes+=xEnd/step;
			YRes+=yEnd/step;					
			
			if (XRes>=Q) {				
				XRes=XRes%Q;
				XCur-=step;
				bInterpXEnable=true;
			}
			
			if (YRes>=Q) {
				YRes=YRes%Q;
				YCur+=step;	
				bInterpYEnable=true;
			}			
			if (bInterpXEnable||bInterpYEnable) {
				GT_LnXY(XCur,YCur);
				bInterpXEnable=bInterpYEnable=false;
			}
			IPCount++;
		}
		break;
	case 3:
		while (IPCount<Q) {
			XRes+=xEnd/step;
			YRes+=yEnd/step;					
			
			if (XRes>=Q) {				
				XRes=XRes%Q;
				XCur-=step;
				bInterpXEnable=true;
			}
			
			if (YRes>=Q) {
				YRes=YRes%Q;
				YCur-=step;	
				bInterpYEnable=true;
			}
			if (bInterpXEnable||bInterpYEnable) {
				GT_LnXY(XCur,YCur);
				bInterpXEnable=bInterpYEnable=false;
			}
			IPCount++;		
		}
		break;
	case 4:
		while (IPCount<Q) {
			XRes+=xEnd/step;
			YRes+=yEnd/step;					
			
			if (XRes>=Q) {				
				XRes=XRes%Q;
				XCur+=step;	
				bInterpXEnable=true;
			}
			
			if (YRes>=Q) {
				YRes=YRes%Q;
				YCur-=step;	
				bInterpXEnable=true;
			}
			if (bInterpXEnable||bInterpYEnable) {
				GT_LnXY(XCur,YCur);
				bInterpXEnable=bInterpYEnable=false;
			}
			IPCount++;			
		}
		break;
	}

	End_Interpolation();

}

void CInterpolation::CI_DDA_All(int nXRes,int nYRes,int nZRes,long xEnd,long yEnd,long zEnd)
{
	nXRes=nXRes+xEnd;
	nYRes=nYRes+yEnd;
	nZRes=nZRes+zEnd;
}

/****************************************************************************/
/*  函数名: End_Interpolation	    										*/
/*  功  能：插补完成处理													*/
/*  参  数：																*/									
/*  出  口：																*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::End_Interpolation()
{
	short rtn;
	rtn=GT_EndList();
	rtn=GT_StrtMtn();

}

/****************************************************************************/
/*  函数名: Start_LineInterp	    										*/
/*  功  能：直线插补前处理													*/
/*  参  数：long xSt, ySt  插补起点											*/									
/*  出  口：																*/
/*  说  明：																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::Start_LineInterp(long xSt, long ySt)
{
	short rtn;
	rtn=GT_SetSynAcc(0.1);
	rtn=GT_SetSynVel(2);
	for(int i=0;i<4;i++)
	{
		rtn=GT_Axis(i);
		rtn=GT_ZeroPos();
	}
	
	rtn=GT_StrtList();
	rtn=GT_MvXY(xSt,ySt,2,0.1);

}

/****************************************************************************/
/*  函数名: CircleInterp_DDA_Buffer    										*/
/*  功  能：DDA圆弧插补(缓冲区指令型)										*/
/*  参  数：long xSt, ySt  插补起点											*/
/*          long lXEnd, long lYEnd  插补终点								*/
/*			long radius			插补圆弧									*/		
/*			bool bIsCW			插补方向									*/
/*			int step			插补步长									*/
/*          unsigned short  n   插补累加器位数							    */										
/*  出  口：																*/
/*  说  明：1.此函数用板卡缓冲区坐标系命令方式执行轴运动					*/
/*          2. 目前只提供XY平面插补											*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::CircleInterp_DDA_Buffer(unsigned short n, int step, long xSt, long ySt, long lXEnd, long lYEnd, long radius,bool bIsCW)
{
	long XRes,YRes; //寄存器溢出后余数
	long xEnd,yEnd;   //插补终点值
	long XCur(0),YCur(0),ZCur(0); //当前位置
	int nQuadrant;//象限
	int Q;  //累加器容量
	bool bInterpXEnable,bInterpYEnable;
	long Jvx,Jvy;//一次插补完成时的当前坐标
	int  xCount,yCount;

	XRes=YRes=0;
	bInterpXEnable=false;
	bInterpYEnable=false;
	xEnd=abs(lXEnd);yEnd=abs(lYEnd);
	Q=pow((double)2, (int)n);
	
	Jvy=XCur=xSt;Jvx=YCur=ySt;
	if (xEnd==xSt) {
		xCount=2*radius/step;
	}
	else
	{
		xCount=abs(xEnd-xSt)/step;
	}
	if (yEnd==ySt) {
		yCount=2*radius/step;
	}
	else{
		yCount=abs(yEnd-ySt)/step;
	}
   if((xEnd==xSt)&&(yEnd==ySt))
   {
	   xCount=4*radius/step;
	   yCount=4*radius/step;
   }

    Start_LineInterp(xSt,ySt);//插补前准备

	if(bIsCW){
		
		while(xCount>0||yCount>0)
		{
			nQuadrant=Judge_Quadrant(XCur,YCur);//计算插补相限
			switch(nQuadrant) {
			case 1:
				if (xCount>0) {
					XRes+=Jvx/step;
					if (XRes>=Q) {
						XRes=XRes%Q;
						XCur+=step;
						bInterpXEnable=true;
						xCount--;
					}		
				}
				if (yCount>0) {
					YRes+=Jvy/step;
					if (YRes>=Q) {
						YRes=YRes%Q;
						YCur-=step;	
						bInterpYEnable=true;
						yCount--;
					}		
				}	
				break;
			case 2:
				if (xCount>0) {
					XRes+=Jvx/step;
					if (XRes>=Q) {
						XRes=XRes%Q;
						XCur+=step;	
						bInterpXEnable=true;
						xCount--;
					}		
				}
				if (yCount>0) {
					YRes+=Jvy/step;
					if (YRes>=Q) {
						YRes=YRes%Q;
						YCur+=step;	
						bInterpYEnable=true;
						yCount--;
					}		
				}	
				break;
			case 3:
				if (xCount>0) {
					XRes+=Jvx/step;
					if (XRes>=Q) {
						XRes=XRes%Q;
						XCur-=step;	
						bInterpXEnable=true;
						xCount--;
					}		
				}
				if (yCount>0) {
					YRes+=Jvy/step;
					if (YRes>=Q) {
						YRes=YRes%Q;
						YCur+=step;	
						bInterpYEnable=true;
						yCount--;
					}		
				}	
				break;
			case 4:
				if (xCount>0) {
					XRes+=Jvx/step;
					if (XRes>=Q) {
						XRes=XRes%Q;
						XCur-=step;
						bInterpXEnable=true;
						xCount--;
					}		
				}
				if (yCount>0) {
					YRes+=Jvy/step;
					if (YRes>=Q) {
						YRes=YRes%Q;
						YCur-=step;	
						bInterpYEnable=true;
						yCount--;
					}		
				}	
				break;
			}
			
			Jvx=abs(YCur);
			Jvy=abs(XCur);
			if (bInterpYEnable||bInterpXEnable) {
				GT_LnXY(XCur,YCur);
				bInterpXEnable=false;
				bInterpYEnable=false;
			}
			
		}	
	}
	else{
		while(xCount>0||yCount>0)
		{
			nQuadrant=Judge_Quadrant(XCur,YCur);//计算插补相限
			switch(nQuadrant) {
			case 1:
				if (xCount>0) {
					XRes+=Jvx/step;
					if (XRes>=Q) {
						XRes=XRes%Q;
						XCur-=step;
						bInterpXEnable=true;
						xCount--;
					}		
				}
				if (yCount>0) {
					YRes+=Jvy/step;
					if (YRes>=Q) {
						YRes=YRes%Q;
						YCur+=step;	
						bInterpYEnable=true;
						yCount--;
					}		
				}	
				break;
			case 2:
				if (xCount>0) {
					XRes+=Jvx/step;
					if (XRes>=Q) {
						XRes=XRes%Q;
						XCur-=step;	
						bInterpXEnable=true;
						xCount--;
					}		
				}
				if (yCount>0) {
					YRes+=Jvy/step;
					if (YRes>=Q) {
						YRes=YRes%Q;
						YCur-=step;	
						bInterpYEnable=true;
						yCount--;
					}		
				}	
				break;
			case 3:
				if (xCount>0) {
					XRes+=Jvx/step;
					if (XRes>=Q) {
						XRes=XRes%Q;
						XCur-=step;	
						bInterpXEnable=true;
						xCount--;
					}		
				}
				if (yCount>0) {
					YRes+=Jvy/step;
					if (YRes>=Q) {
						YRes=YRes%Q;
						YCur+=step;	
						bInterpYEnable=true;
						yCount--;
					}		
				}	
				break;
			case 4:
				if (xCount>0) {
					XRes+=Jvx/step;
					if (XRes>=Q) {
						XRes=XRes%Q;
						XCur+=step;	
						bInterpXEnable=true;
						xCount--;
					}		
				}
				if (yCount>0) {
					YRes+=Jvy/step;
					if (YRes>=Q) {
						YRes=YRes%Q;
						YCur+=step;	
						bInterpYEnable=true;
						yCount--;
					}		
				}	
				break;
			}
			
			Jvx=abs(YCur);
			Jvy=abs(XCur);
			if (bInterpYEnable||bInterpXEnable) {
				GT_LnXY(XCur,YCur);
				bInterpXEnable=false;
				bInterpYEnable=false;
			}	
		}	

	}
	End_Interpolation();
}
