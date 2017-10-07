// dy_math.cpp: implementation of the dy_math class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "CNC.h"
#include <math.h>
#include "gy3.h"
#include "cnc_api.h"

#include "dy_math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//给定start, end point and r ,cal arc length; 
double WINAPI CalArcLen(CCNCData str,CCNCData end,unsigned char PlanGroup)
{
	double a,b,c,len;
	b=c=(double)fabs(end.r);
	if(b<0.01)return 0;
	switch(PlanGroup)
	{
	case 18:
		a=(double)sqrt((str.x-end.x)*(str.x-end.x)+(str.z-end.z)*(str.z-end.z));
		break;
	case 19:
		a=(double)sqrt((str.y-end.y)*(str.y-end.y)+(str.z-end.z)*(str.z-end.z));
		break;
	default:
		a=(double)sqrt((str.x-end.x)*(str.x-end.x)+(str.y-end.y)*(str.y-end.y));
		break;
	}

	if(end.r>0)len=(double)acos((b*b+c*c-a*a)/(2*b*c))*b;
	else  len=(double)(acos((b*b+c*c-a*a)/(2*b*c))+3.1415926)*b;
	return len;
}
//////////////////////////////////////////////////////////////////////
//给出起点xs,ys和终点 xe,ye、半径r;计算圆心坐标xc,yc;逆时针时计算R值求反；
int  WINAPI R_ToIJ(double xs,double ys,double xe,double ye,double r,double &xc,double &yc)
{
	double T1,T2,T3,a1,b1,c11,yr;
	const double eh=0.0001;
	T1=(xs*xs)+(ys*ys)-(xe*xe)-(ye*ye);
	T2=(2*ys)-(2*ye);
	T3=(2*xs)-(2*xe);	
	a1=(T2*T2+T3*T3);
	b1=(2*xs*T3*T2-2*T1*T2-2*T3*T3*ys);
	c11=(xs*xs*T3*T3+T1*T1-2*xs*T3*T1+ys*ys*T3*T3-r*r*T3*T3);
	yr=((double)b1*( double)b1)-(4*( double)a1*( double)c11);
	if(((double)fabs(xs-xe)>eh)&&(yr<0)){//
		if(r>0)r+=0.10;
		else if(r<0)r-=0.10;
		c11=(xs*xs*T3*T3+T1*T1-2*xs*T3*T1+ys*ys*T3*T3-r*r*T3*T3);
		yr=(( double)b1*( double)b1)-(4*( double)a1*( double)c11);
		if(yr<-eh){
			xc=(xs+xe)/2;      yc=(ys+ye)/2;
			return 1;
		}else if(yr<0)yr=0;
	}
	if(r>0){
		if(fabs(xs-xe)<eh){
			yc=(ys+ye)/2;
			ys>ye ? xc=xs-(double)sqrt(r*r-(ye-ys)*(ye-ys)/4) : xc=xs+(double)sqrt(r*r-(ye-ys)*(ye-ys)/4);
		}
		else{
			xs<xe ? yc=(-b1-(double)sqrt(yr))/(2*a1) : yc=(-b1+(double)sqrt(yr))/(2*a1);
			xc=(T1-yc*T2)/T3;
		}
	}
	if(r<0){
		if(fabs(xs-xe)<eh){
			yc=(ys+ye)/2;
			ys>ye ? xc=xs+(double)sqrt(r*r-(ye-ys)*(ye-ys)/4) : xc=xs-(double)sqrt(r*r-(ye-ys)*(ye-ys)/4);
		}
		else{
			xs<xe ? yc=(-b1+(double)sqrt(yr))/(2*a1) : yc=(-b1-(double)sqrt(yr))/(2*a1);
			xc=(T1-yc*T2)/T3;
		}		
	}
//	xc=xc-xs;yc=yc-ys;
	return 0;
}      
//////////////////////////////////////////////////////////////////////
//给出起点xs,ys和终点xe,ye、圆心坐标i,j，顺时针加工，计算圆的半径。 arg
//其中：圆心坐标是绝对值；
//返回：R 正为劣弧(<180)
//增加r1 判断最大半径
double IJ_ToR(double &xs,double &ys,double &xe,double &ye,double &i,double &j,bool IsG02)
{
	double xc,yc,r,r1;
	char sign;
	xc=(xs+xe)/2;	yc=(ys+ye)/2;
	if(fabs(ys-ye)<0.000001)
	{
		if(xs>xe)j>yc ?  sign=1 : sign=-1 ;
		else j>yc ?  sign=-1 : sign=1 ;
	}
	else if(ys<ye)
		i>xc ? sign=1 :sign=-1;
	else  if(ys>ye)
		i>xc ? sign=-1 :sign=1; 
	 
	r=sqrt((xe-i)*(xe-i)+(ye-j)*(ye-j));
    r1=(double)sqrt((xs-i)*(xs-i)+(ys-j)*(ys-j));
	if(r1>r)r=r1;
	if(IsG02)r=r*sign;
	else r=-r*sign;
	return r;
}
//////////////////////////////////////////////////////////////////////
//给出起点 xc,yc；终点x,y坐标，计算线段与X轴夹角；
//return angle 采用弧度为单位；
int WINAPI DY_AngleValue(double x,double y,double xc,double yc,double & Angle)
{	
    #define RAD 0.0174533 
    const double eh=0.0001;
	if((fabs(y-yc)<eh) && x>xc)   Angle=0;
	else if(y>yc && (fabs(x-xc)<eh)) Angle=1.570796;//90*RAD;
	else if((fabs(y-yc)<eh)&& x<xc) Angle=3.14159265;//180*RAD;
	else if(y<yc && (fabs(x-xc)<eh)) Angle=4.712389;//270*RAD;
	else if(x>xc && y>yc) Angle=atan(fabs(y-yc)/fabs(x-xc));
	else if(x<xc && y>yc) Angle=atan(fabs(x-xc)/fabs(y-yc))+1.570796;//90*RAD;
	else if(x<xc && y<yc) Angle=atan(fabs(y-yc)/fabs(x-xc))+3.14159265;//180*RAD;
	else if(x>xc && y<yc) Angle=atan(fabs(x-xc)/fabs(y-yc))+4.712389;//270*RAD;
	return 0;
}



