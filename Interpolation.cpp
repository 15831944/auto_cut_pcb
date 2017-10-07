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
/*  ������: LineInterp_MIT      											*/
/*  ��  �ܣ����ȽϷ�ֱ�߲岹(�ǻ�����ָ����)								*/
/*  ��  ����long XEnd, long YEnd  �岹�յ�									*/
/*			int step �岹����												*/
/*  ��  �ڣ�																*/
/*  ˵  �����˺����ð忨��������ִ��										*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::LineInterp_MIT(int step, long XEnd, long YEnd) //���ȽϷ�ֱ�߲岹
{
	long lDevVal;
	long xCurVal(0),yCurVal(0);
	long StepMount,StepCount(0);
	unsigned short nDir;
	
    nDir=Judge_Quadrant(XEnd,YEnd);//�����ж�	
	
	XEnd=abs(XEnd);
	YEnd=abs(YEnd);	

	StepMount=(XEnd+YEnd)/step;
	lDevVal=yCurVal*XEnd-xCurVal*YEnd;

	
    while (StepCount<StepMount) {//�յ��б�  	
		
		if (lDevVal>=0) { //ƫ�=0
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
		else{  //ƫ��<0
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
/*  ������: CirInterp_MIT      												*/
/*  ��  �ܣ����ȽϷ�Բ���岹(�ǻ�����ָ����)								*/
/*  ��  ����long XStart, long YStart  �岹���								*/
/*          long XEnd, long YEnd  �岹�յ�									*/
/*			long radius			�岹Բ��									*/		
/*			bool bIsCW			�岹����									*/
/*			int step			�岹����									*/
/*  ��  �ڣ�																*/
/*  ˵  �����˺����ð忨��������ִ��										*/
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
    StepAmount=(abs(XStart-XEnd)+abs(YStart-YEnd))/step;//�����ܲ���
    lDevVal=xCurVal*xCurVal+yCurVal*yCurVal-radius*radius;  //����ƫ��

	if(bIsCW){ //˳Բ		
		while( (xCurVal!=XEnd) ||(yCurVal!=YEnd)){  //�յ��б�
			nQuart=Judge_Quadrant(xCurVal,yCurVal);//��ȡ��ǰ��������
			if (lDevVal>=0) {	//������ƫ�0		
				switch(nQuart) {
				case 1:         //��һ����
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
			else{  	//������ƫ��<0	
				switch(nQuart) {
				case 1:         //��һ����
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
	else{ //��Բ
		while( (xCurVal!=XEnd) ||(yCurVal!=YEnd)){  //�յ��б�
			nQuart=Judge_Quadrant(xCurVal,yCurVal);//��ȡ��ǰ��������
			if (lDevVal>=0) {	//������ƫ�0		
				switch(nQuart) {
				case 1:         //��һ����
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
			else{  	//������ƫ��<0	
				switch(nQuart) {
				case 1:         //��һ����
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
/*  ������: Judge_Quadrant		      										*/
/*  ��  �ܣ��жϵ�ǰ����������												*/
/*  ��  ���� long x, long y  ��ǰ������										*/
/*  ��  �ڣ� ���ص�ǰ��������������											*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
unsigned short CInterpolation::Judge_Quadrant(long x, long y)
{
	unsigned short nDir;
	
	if (x>=0) { //�����ж�
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
/*  ������: CirInterp_MIT_Buffer      										*/
/*  ��  �ܣ����ȽϷ�Բ���岹(������ָ����)								*/
/*  ��  ���� long XStart, long YStart  �岹���								*/
/*          long XEnd, long YEnd  �岹�յ�									*/
/*			long radius			�岹Բ��									*/		
/*			bool bIsCW			�岹����									*/
/*			long step			�岹����									*/
/*  ��  �ڣ�																*/
/*  ˵  ����1.�˺����ð忨����������ϵ���ʽִ�����˶�					*/
/*			2.ֻ�ṩXYƽ��Բ���岹											*/
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
    StepAmount=(abs(XStart-XEnd)+abs(YStart-YEnd))/step;//�����ܲ���
    lDevVal=xCurVal*xCurVal+yCurVal*yCurVal-radius*radius;  //����ƫ��	
	rtn=GT_SetSynAcc(0.1);
	rtn=GT_SetSynVel(2);
	for(int i=0;i<4;i++)
	{
		rtn=GT_Axis(i);
		rtn=GT_ZeroPos();
	}
	
	rtn=GT_StrtList();
	rtn=GT_MvXY(XStart,YStart,2,0.1);

	if(bIsCW){ //˳Բ		
		while( (xCurVal!=XEnd) ||(yCurVal!=YEnd)){  //�յ��б�
			nQuart=Judge_Quadrant(xCurVal,yCurVal);//��ȡ��ǰ��������
			if (lDevVal>=0) {	//������ƫ�0		
				switch(nQuart) {
				case 1:         //��һ����
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
			else{  	//������ƫ��<0	
				switch(nQuart) {
				case 1:         //��һ����
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
	else{ //��Բ
		while( (xCurVal!=XEnd) ||(yCurVal!=YEnd)){  //�յ��б�
			nQuart=Judge_Quadrant(xCurVal,yCurVal);//��ȡ��ǰ��������
			if (lDevVal>=0) {	//������ƫ�0		
				switch(nQuart) {
				case 1:         //��һ����
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
			else{  	//������ƫ��<0	
				switch(nQuart) {
				case 1:         //��һ����
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
/*  ������: LineInterp_MIT_Buffer      										*/
/*  ��  �ܣ����ȽϷ�ֱ�߲岹(������ָ����)								*/
/*  ��  ����long XEnd, long YEnd  �岹�յ�									*/
/*			int step �岹����												*/
/*  ��  �ڣ�																*/
/*  ˵  �����˺����ð忨����������ϵ���ʽִ�����˶�						*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::LineInterp_MIT_Buffer(int step, long XEnd, long YEnd)
{
	long lDevVal;				      //ƫ��ֵ
	long xCurVal(0),yCurVal(0);       //��ǰ���ʵ��λ��
	long StepMount;                   //�岹�ܲ���
	long StepCount(0);			      //�岹����������
	unsigned short nDir;			  //��ǰ����������
	
    nDir=Judge_Quadrant(XEnd,YEnd);   //�����ж�	
	
	XEnd=abs(XEnd);
	YEnd=abs(YEnd);	
	
	StepMount=(XEnd+YEnd)/step;
	lDevVal=yCurVal*XEnd-xCurVal*YEnd;	

	Start_LineInterp(0,0);       //�岹ǰ׼��
    while (StepCount<StepMount) {//�յ��б�		
		
		if (lDevVal>=0) { //ƫ�=0
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
		else{  //ƫ��<0
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
/*  ������: LineInterp_DDA_Buffer      										*/
/*  ��  �ܣ�DDAֱ�߲岹(������ָ����)										*/
/*  ��  ����long x, y,z  �岹�յ�											*/
/*			int step      �岹����											*/
/*          int  n       �岹�ۼ���λ��									    */
/*  ��  �ڣ�																*/
/*  ˵  ����1.�˺����ð忨����������ϵ���ʽִ�����˶�					*/
/*          2. Ŀǰֻ�ṩXYƽ��岹											*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::LineInterp_DDA_Buffer(int n,int step,long x, long y,long z)
{
	long XRes,YRes; //�Ĵ������������
	long xEnd,yEnd;   //�岹�յ�ֵ
	long XCur(0),YCur(0),ZCur(0); //��ǰλ��
	int IPCount=0;//�ۼ�ֵ
	int nQuadrant;//����
	int Q;  //�ۼ�������
	bool bInterpXEnable,bInterpYEnable;

	XRes=YRes=0;
	bInterpXEnable=bInterpYEnable=false;
	xEnd=abs(x);yEnd=abs(y);
	Q=pow((double)2,(int)n);

	nQuadrant=Judge_Quadrant(x,y);//����岹����
      Start_LineInterp(0,0);//�岹ǰ׼��

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
/*  ������: End_Interpolation	    										*/
/*  ��  �ܣ��岹��ɴ���													*/
/*  ��  ����																*/									
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
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
/*  ������: Start_LineInterp	    										*/
/*  ��  �ܣ�ֱ�߲岹ǰ����													*/
/*  ��  ����long xSt, ySt  �岹���											*/									
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
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
/*  ������: CircleInterp_DDA_Buffer    										*/
/*  ��  �ܣ�DDAԲ���岹(������ָ����)										*/
/*  ��  ����long xSt, ySt  �岹���											*/
/*          long lXEnd, long lYEnd  �岹�յ�								*/
/*			long radius			�岹Բ��									*/		
/*			bool bIsCW			�岹����									*/
/*			int step			�岹����									*/
/*          unsigned short  n   �岹�ۼ���λ��							    */										
/*  ��  �ڣ�																*/
/*  ˵  ����1.�˺����ð忨����������ϵ���ʽִ�����˶�					*/
/*          2. Ŀǰֻ�ṩXYƽ��岹											*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpolation::CircleInterp_DDA_Buffer(unsigned short n, int step, long xSt, long ySt, long lXEnd, long lYEnd, long radius,bool bIsCW)
{
	long XRes,YRes; //�Ĵ������������
	long xEnd,yEnd;   //�岹�յ�ֵ
	long XCur(0),YCur(0),ZCur(0); //��ǰλ��
	int nQuadrant;//����
	int Q;  //�ۼ�������
	bool bInterpXEnable,bInterpYEnable;
	long Jvx,Jvy;//һ�β岹���ʱ�ĵ�ǰ����
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

    Start_LineInterp(xSt,ySt);//�岹ǰ׼��

	if(bIsCW){
		
		while(xCount>0||yCount>0)
		{
			nQuadrant=Judge_Quadrant(XCur,YCur);//����岹����
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
			nQuadrant=Judge_Quadrant(XCur,YCur);//����岹����
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
