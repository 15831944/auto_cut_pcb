// CNC_a.cpp: implementation of the CCNC_a class.
//
/////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CNC_c.h"
#include <math.h>
#include "CNCData.h"
#include "GTCNCLib.h"
#include "dy_math.h"

#define PI 3.14159265

//将所搜索的字符串及当前位置传送给函数；
//返回读到的数;及搜索位置,
//返回0 OK
double x_old,y_old,z_old;
COMPLEX cenx[3];			//用于存储圆心坐标
int curvepos[3];
double MaxAngle,SubAngle;
struct CStartPos
{
	int pos;
	COMPLEX center;
};

CStartPos Firstpos; //纪录第一段曲线起点
double dwAngleSub;
/*****************************
					速度V特殊值的意义
					-1：当前段要减速
					-2：当前段要减速且Z轴速度为抬刀速度
					-3：当前段要减速且Z轴速度为下刀速度
					-4: 当前段的角度差太大
*****************************/



double CalAngle(double L0x,double L0y,double L0z, double L1x,double L1y,double L1z)
{
	double den,cos1,num,arccos;
	num = (L0x*L1x + L0y*L1y + L0z*L1z);//分子
	den = (sqrt(L0x*L0x + L0y*L0y + L0z*L0z))*(sqrt(L1x*L1x + L1y*L1y + L1z*L1z));//分母
	cos1 = num/den;
	if(cos1<=-1) 
		cos1=-1;
	if(cos1>=1)
		cos1=1;
	arccos = acos(cos1)*180/PI;//计算角度，由于vc计算出来的是弧度，所以划算成度了
	return arccos;
}


int ReadNum(CString & str1,int & n,unsigned char & num)
{   
	char c1,*pb,sb[20];
	char flag=1;//表示本次有无读到数；
	pb=sb;   
	while(str1[n+1]==' ')n++;//读掉空格
	do{					                                      
		c1=str1[++n];                                   	  
		if((c1>='0' && c1<='9') || c1=='.'){                  
			*pb=c1;							                  
			pb++;	
			flag=0;
		}										              
		if(c1=='\n'||c1==' '||c1==','||c1==';'||isalpha(c1)!=0)break;      
		      
	}while(1);								      
	*pb='\0';											      
	n--;
	if(flag==1)return -1;
	num=atoi(sb);
	return 0;	
}

int ReadNum(CString & str1,int & n,double & num)
{   
	char c1,*pb,sb[20];
	int sign=1;
	char flag=1;//表示本次有无读到数；
	while(str1[n+1]==' ')n++;//读掉空格
	pb=sb;                                     
	do{					                                      
		c1=str1[++n];                                   	  
		if((c1>='0' && c1<='9') || c1=='.'){                  
			*pb=c1;							                  
			pb++;	
			flag=0;
		}										              
		if(c1=='\n'||c1==' '||c1==','||c1==';'||isalpha(c1)!=0)break;      
		if(c1=='-')  sign=-1;							      
	}while(1);								      
	*pb='\0';											      
	n--;
	if(flag==1)return -1;
	num=sign*(double)atof(sb);
	return 0;	
}

char CalDccPos(CCncArray *pAry,int PlanGroup)
{	
	double L0[3],L1[3],tempx,tempy,angle;
	int dir;
	
/*	CCNCData temp1,temp2,temp3;

	temp1=pAry->operator [](curvepos[0]);
	temp2=pAry->operator [](curvepos[1]);
	temp3=pAry->operator [](curvepos[2]);*/

	if((pAry->operator [](curvepos[1]).Code==0 ||pAry->operator [](curvepos[1]).Code==1)\
		&&(pAry->operator [](curvepos[2]).Code==0 ||pAry->operator [](curvepos[2]).Code==1))//直线接直线
	{
		L0[0]=pAry->operator [](curvepos[1]).x-pAry->operator [](curvepos[0]).x;
		L0[1]=pAry->operator [](curvepos[1]).y-pAry->operator [](curvepos[0]).y;
		L0[2]=pAry->operator [](curvepos[1]).z-pAry->operator [](curvepos[0]).z;
		L1[0]=pAry->operator [](curvepos[2]).x-pAry->operator [](curvepos[1]).x;
		L1[1]=pAry->operator [](curvepos[2]).y-pAry->operator [](curvepos[1]).y;
		L1[2]=pAry->operator [](curvepos[2]).z-pAry->operator [](curvepos[1]).z;
		angle=CalAngle(L0[0],L0[1],L0[2],L1[0],L1[1],L1[2]);
		if(angle>MaxAngle)
			pAry->operator [](curvepos[1]).v=-1;
	}
	else if((pAry->operator [](curvepos[1]).Code==0 ||pAry->operator [](curvepos[1]).Code==1)\
		&&(pAry->operator [](curvepos[2]).Code==2 ||pAry->operator [](curvepos[2]).Code==3))//直线接圆弧
	{
		//计算直线向量
		L0[0]=pAry->operator [](curvepos[1]).x-pAry->operator [](curvepos[0]).x;
		L0[1]=pAry->operator [](curvepos[1]).y-pAry->operator [](curvepos[0]).y;
		L0[2]=pAry->operator [](curvepos[1]).z-pAry->operator [](curvepos[0]).z;
		//计算圆弧向量
		if(pAry->operator [](curvepos[2]).Code==2) dir=-1;
		else dir=1;
		tempx = ( cenx[2].Im - pAry->operator [](curvepos[1]).y)*dir;
		tempy = -(cenx[2].Re - pAry->operator [](curvepos[1]).x)*dir;
		if(PlanGroup==17)
		{L1[0]=tempx;L1[1]=tempy;L1[2]=0;}
		else if(PlanGroup==18)
		{L1[0]=tempy;L1[1]=0;L1[2]=tempx;}
		else if(PlanGroup==19)
		{L1[0]=0;L1[1]=tempx;L1[2]=tempy;}
		angle=CalAngle(L0[0],L0[1],L0[2],L1[0],L1[1],L1[2]);
		if(angle>MaxAngle)
			pAry->operator [](curvepos[1]).v=-1;
	}
	else if((pAry->operator [](curvepos[1]).Code==2 ||pAry->operator [](curvepos[1]).Code==3)\
		&&(pAry->operator [](curvepos[2]).Code==0 ||pAry->operator [](curvepos[2]).Code==1))//圆弧接直线
	{
		//计算圆弧向量
		if(pAry->operator [](curvepos[1]).Code==2) dir=-1;
		else dir=1;
		tempx = ( cenx[1].Im - pAry->operator [](curvepos[1]).y)*dir;
		tempy = -(cenx[1].Re - pAry->operator [](curvepos[1]).x)*dir;
		if(PlanGroup==17)
		{L0[0]=tempx;L0[1]=tempy;L0[2]=0;}
		else if(PlanGroup==18)
		{L0[0]=tempy;L0[1]=0;L0[2]=tempx;}
		else if(PlanGroup==19)
		{L0[0]=0;L0[1]=tempx;L0[2]=tempy;}
		//计算直线向量
		L1[0]=pAry->operator [](curvepos[2]).x-pAry->operator [](curvepos[1]).x;
		L1[1]=pAry->operator [](curvepos[2]).y-pAry->operator [](curvepos[1]).y;
		L1[2]=pAry->operator [](curvepos[2]).z-pAry->operator [](curvepos[1]).z;
		angle=CalAngle(L0[0],L0[1],L0[2],L1[0],L1[1],L1[2]);
		if(angle>MaxAngle)
			pAry->operator [](curvepos[1]).v=-1;
	}
	else if((pAry->operator [](curvepos[1]).Code==2 ||pAry->operator [](curvepos[1]).Code==3)\
		&&(pAry->operator [](curvepos[2]).Code==2 ||pAry->operator [](curvepos[2]).Code==3))//圆弧接圆弧
	{
		//计算第一段圆弧向量
		if(pAry->operator [](curvepos[1]).Code==2) dir=-1;
		else dir=1;
		tempx = ( cenx[1].Im - pAry->operator [](curvepos[1]).y)*dir;
		tempy = -(cenx[1].Re - pAry->operator [](curvepos[1]).x)*dir;
		if(PlanGroup==17)
		{L0[0]=tempx;L0[1]=tempy;L0[2]=0;}
		else if(PlanGroup==18)
		{L0[0]=tempy;L0[1]=0;L0[2]=tempx;}
		else if(PlanGroup==19)
		{L0[0]=0;L0[1]=tempx;L0[2]=tempy;}
		//计算第二段圆弧向量
		if(pAry->operator [](curvepos[2]).Code==2) dir=-1;
		else dir=1;
		tempx = ( cenx[2].Im - pAry->operator [](curvepos[1]).y)*dir;
		tempy = -(cenx[2].Re - pAry->operator [](curvepos[1]).x)*dir;
		if(PlanGroup==17)
		{L1[0]=tempx;L1[1]=tempy;L1[2]=0;}
		else if(PlanGroup==18)
		{L1[0]=tempy;L1[1]=0;L1[2]=tempx;}
		else if(PlanGroup==19)
		{L1[0]=0;L1[1]=tempx;L1[2]=tempy;}
		angle=CalAngle(L0[0],L0[1],L0[2],L1[0],L1[1],L1[2]);
		if(angle>MaxAngle)
			pAry->operator [](curvepos[1]).v=-1;
			}
	pAry->operator [](curvepos[1]).v=angle;
/*if(angle>1)
{
dwAngleSub=dwAngleSub-angle;
if(fabs(dwAngleSub)>SubAngle) pAry->operator [](curvepos[1]).v=-4;
}
dwAngleSub=angle;
if((pAry->operator [](curvepos[0]).x==pAry->operator [](curvepos[1]).x)\
   &&(pAry->operator [](curvepos[0]).y==pAry->operator [](curvepos[1]).y))
{
	if((pAry->operator [](curvepos[0]).z<pAry->operator [](curvepos[1]).z))
		pAry->operator [](curvepos[1]).v=-2;
	else if((pAry->operator [](curvepos[0]).z>pAry->operator [](curvepos[1]).z))
		pAry->operator [](curvepos[1]).v=-3;
}*/

//pAry->operator [](curvepos[1]).v=angle;//debug
//将圆心前移
Firstpos.center.Re= cenx[0].Re;
Firstpos.center.Im= cenx[0].Im;

cenx[0].Im=cenx[1].Im;
cenx[0].Re=cenx[1].Re;
cenx[1].Im=cenx[2].Im;
cenx[1].Re=cenx[2].Re;
cenx[2].Im=0;
cenx[2].Re=0;
return 0;
}

char Compile (CString SourPath,CCncArray * pAry,double* angle,double Coord[],bool bSelDel)
{
#define NONUM 1000000
#define TOL   0.0001
#define SAVE {pAry->Add(gy);}
	BOOL compileFinish=FALSE;
	
	MaxAngle=angle[0];
	SubAngle=angle[1];
	int gcodenum=0;//预处理用
	
	
	cenx[0].Im =cenx[0].Re =0;
	cenx[1].Im =cenx[1].Re =0;
	cenx[2].Im =cenx[2].Re =0;
	curvepos[0]=curvepos[1]=curvepos[2]=0;

	dwAngleSub=0;
	Firstpos.pos=-1; 
	
	if(pAry==NULL)return 1;
	if(SourPath.IsEmpty())return 2;
	pAry->RemoveAll();
	
	CStdioFile fs; 
	CString str1;
	CCNCData gy;
	double xn,yn,zn,xo,yo,zo,i,j,k,v;
	double xCur,yCur,zCur;
	double depth=0,dR=0,dQ=1;
	double rpn=1000;
	
	bool bLenBC, bG28, bG92, bNewTool, bABS, bHead, bReturnR, bZK;
	bool bX,bY,bZ;
	unsigned char LineGroup,PlanGroup,CrdGrop,BCmode,Rmode;
	unsigned char code,ZKcode,H=1,tool=0;
	int n;
	
	if(fs.Open(SourPath, CFile::modeRead | CFile::typeText)==NULL)return 10;
	DWORD filelen=fs.GetLength()/8;
	if((filelen*8)>20971520)
	{
		return 11;
	}
	pAry->SetSize(0,filelen);  
	
	Coord[0]=Coord[1]=Coord[2]=0;
	Coord[3]=Coord[4]=Coord[5]=0;

	bLenBC=bG28=bG92=bNewTool=bHead=bZK=false;
	bABS=bReturnR=true;
		LineGroup=0; PlanGroup=17; BCmode=49; CrdGrop=54; 
	xn=yn=zn=xo=yo=zo=0; gy.N=0; 
	v=0;
	while(1){
		if(fs.ReadString(str1)==NULL)break; 
		str1+='\n'; gy.N++;
		str1.Remove(0x20); 
		//gy.gcode=(char *)malloc(strlen(str1)+1);
///		memset(gy.gcode,0,strlen(str1)+1);
//		strcpy(gy.gcode,str1);
		n=Rmode=0;
		i=j=k=0; 
		bX=bY=bZ=false;	
		
		while(str1[n]!='\n'){
			switch(str1[n]){
			case '(':
			case 'O':
			case '/':	
				while(str1[n+1]!='\n')n++; 
				break;
			case 'N':case 'n':
				while((str1[n+1]!='\n')&&(isalpha(str1[n+1])==0))n++;	
				break;
			case '%':
				if(!bHead)
				{
					bHead=true;
					gy.Code=9;
					SAVE;
				}
				while(str1[n+1]!='\n')n++; 				
				break;
			case 'X':case 'x':
				ReadNum(str1,n,xn); 
				bX=true;
				break;
			case 'Y':case 'y':
				ReadNum(str1,n,yn); 
				bY=true;
				break;
			case 'Z':case 'z':
				if(bZK)ReadNum(str1,n,depth);
				else	ReadNum(str1,n,zn);
				bZ=true;
				break;
			case 'I':case 'i':
				ReadNum(str1,n,i); Rmode=2; 
				break;
			case 'J':case 'j':
				ReadNum(str1,n,j); Rmode=2; 
				break;
			case 'K':case 'k':
				ReadNum(str1,n,k); Rmode=2;
				break;
			case 'R':case 'r':
				ReadNum(str1,n,dR); Rmode=1;
				break;
			case 'Q':case 'q':
				ReadNum(str1,n,dQ); 
				break;
			case 'T':case 't':
				ReadNum(str1,n,tool);
				break;
			case 'H':
			case 'D':
				ReadNum(str1,n,H);	 
				break;
			case 'F':case 'f':
				ReadNum(str1,n,v);	
				break;
			case 'S':case 's':	
				ReadNum(str1,n,rpn);	 
				break;
			case 'G':case 'g':
				ReadNum(str1,n,code);
				switch(code){
				case 0:
				case 1:
				case 2:
				case 3:
					LineGroup=code; 
					break;
				case 4:
					n++;
					while(str1[n]!='X'&&str1[n]!='P'&&str1[n]!='\n')n++;
					if(str1[n]=='X'||str1[n]=='P')
					{
						ReadNum(str1,n,gy.x);
						gy.Code=4;
						SAVE;					
					}else n--;					
					break;
				case 17:
				case 18:
				case 19:
					PlanGroup=gy.Code=code;
					SAVE;
					break;
				case 22:
				case 68:
					while(str1[n+1]!='\n')n++;
					break;
				case 28:
					bG28=true;  
					break;
				case 43:
				case 44:
				case 49:
					bLenBC=true;
					BCmode=code;
					break;
				case 54:
				case 55:
				case 56:
				case 57: 
				case 58:
				case 59:
					gy.Code=code;	SAVE;
					xn=yn=zn=xo=yo=zo=NONUM+1;
					break;
				case 73:
				case 81:
				case 83:
					bZK=true;	ZKcode=code;
					break;
				case 80:
					bZK=false;	
					break;						
				case 90:
					bABS=true; 
					break;
				case 91:
					bABS=false; 
					break;
				case 92:
					bG92=true;					
					break;
				case 98:
					bReturnR=false;
					break;
				case 99:
					bReturnR=true;
					break;	
				default: 
					break;
				}
				break;	
				case 'M':case 'm':
					ReadNum(str1,n,code);
					switch(code){
					case 2:
					case 30:
						gy.Code=8;	SAVE;
						compileFinish=TRUE;
						break;
					case 3:
						gy.y=rpn;
					case 0:
					case 1:
					case 5:
					case 7:
					case 8: 
					case 9:
					case 10:gy.Code=100;gy.x=code;SAVE;break;
					case 20:gy.Code=100;gy.x=code;SAVE;break;
					case 11:gy.Code=100;gy.x=code;SAVE;break;
					case 21:gy.Code=100;gy.x=code;SAVE;break;
					case 12:gy.Code=100;gy.x=code;SAVE;break;
					case 22:gy.Code=100;gy.x=code;SAVE;break;
					case 13:gy.Code=100;gy.x=code;SAVE;break;
					case 23:gy.Code=100;gy.x=code;SAVE;break;
					case 14:gy.Code=100;gy.x=code;SAVE;break;
					case 24:gy.Code=100;gy.x=code;SAVE;break;
					case 15:gy.Code=100;gy.x=code;SAVE;break;
					case 25:gy.Code=100;gy.x=code;SAVE;break;
					case 16:gy.Code=100;gy.x=code;SAVE;break;
					case 26:gy.Code=100;gy.x=code;SAVE;break;
					case 17:gy.Code=100;gy.x=code;SAVE;break;
					case 27:gy.Code=100;gy.x=code;SAVE;break;
					case 18:gy.Code=100;gy.x=code;SAVE;break;
					case 28:gy.Code=100;gy.x=code;SAVE;break;
					case 19:gy.Code=100;gy.x=code;SAVE;break;
					case 29:gy.Code=100;gy.x=code;SAVE;break;
						
					case 40:gy.Code=100;gy.x=code;SAVE;break;
					case 50:gy.Code=100;gy.x=code;SAVE;break;
					case 41:gy.Code=100;gy.x=code;SAVE;break;
					case 51:gy.Code=100;gy.x=code;SAVE;break;
					case 42:gy.Code=100;gy.x=code;SAVE;break;
					case 52:gy.Code=100;gy.x=code;SAVE;break;
					case 43:gy.Code=100;gy.x=code;SAVE;break;
					case 53:gy.Code=100;gy.x=code;SAVE;break;
					case 44:gy.Code=100;gy.x=code;SAVE;break;
					case 54:gy.Code=100;gy.x=code;SAVE;break;
					case 45:gy.Code=100;gy.x=code;SAVE;break;
					case 55:gy.Code=100;gy.x=code;SAVE;break;
					case 46:gy.Code=100;gy.x=code;SAVE;break;
					case 56:gy.Code=100;gy.x=code;SAVE;break;
					case 47:gy.Code=100;gy.x=code;SAVE;break;
					case 57:gy.Code=100;gy.x=code;SAVE;break;
					case 48:gy.Code=100;gy.x=code;SAVE;break;
					case 58:gy.Code=100;gy.x=code;SAVE;break;
					case 49:gy.Code=100;gy.x=code;SAVE;break;
					case 59:gy.Code=100;gy.x=code;SAVE;break;
						
					case 32:
						gy.Code=100; gy.x=code;	SAVE;
						break;
					case 6:
						bNewTool=true;
						break;					
					}
					break;
					default:
						break;
			}
			n++;
		}
		
		if(bNewTool)
		{
			bNewTool=false;
			gy.Code=100; gy.x=6; gy.y=tool;
			SAVE;
		}
		
		if(bG28)
		{
			gy.Code=28;gy.r=bABS;
		//	gy.x=gy.y=gy.z=NONUM+1;
			if(bX){gy.x=xn;xn=xo=0;}
			if(bY){gy.y=yn;yn=yo=0;}
			if(bZ){gy.z=zn;zn=zo=0;}
			SAVE;
			BCmode=49;
			bG28=bX=bY=bZ=false;
		}
		if (bG92) {
			gy.Code=92;			
			if (bX) {gy.x=xn;xo=xn;}
			if (bY) {gy.y=yn;yo=yn;}
			if (bZ) {gy.z=zn;zo=zn;}
			SAVE;
            bG92=bX=bY=bZ=false;
	    //  	continue;
		}
		if(bLenBC)
		{
			gy.Code=BCmode; gy.x=H;
			SAVE;
			if(!bZ)zn=NONUM+1;
			zo=NONUM+1;
			bLenBC=false;
		}
		
		if(!(bX||bY||bZ))continue;
		
		if(bZK)
		{
			gy.Code=0;
			gy.x=xo;gy.y=yo;gy.z=zo;
			if((fabs(xn-xo)>TOL)||(fabs(yn-yo)>TOL))
			{
				gy.x=xn; gy.y=yn; gy.z=zo; SAVE;			
			} 
			if(zo!=dR){gy.z=dR;SAVE;}
			
			double dd,dz=1; 
			
			switch(ZKcode)
			{
			case 73:
			case 83:
				dd=dR-dQ;
				while(dd>depth)
				{
					if(ZKcode==83)
					{
						gy.Code=0; gy.z=dd+dQ+dz;
						SAVE;
					}
					gy.Code=1;gy.z=dd;SAVE;
					if(ZKcode==73)gy.z=dd+dz;
					else gy.z=dR;
					gy.Code=0;SAVE;
					dd-=dQ;
				}
				if(ZKcode==83)
				{
					gy.Code=0;gy.z=dd+dQ+dz;
					SAVE;
				}
				gy.Code=1;gy.z=depth;SAVE;
				break;
			case 81:
				gy.Code=1;gy.z=depth;SAVE;
				break;
			}
			if(bReturnR)gy.z=dR;
			else gy.z=zo;
			gy.Code=0;SAVE;
			zn=gy.z;
			xo=xn; yo=yn; zo=zn;
		}
		else 
		{
			if (!bABS) {	
				if (bX) xn+=xo;
				if (bY)	yn+=yo;
				if (bX)	zn+=zo;
			} 
			if((LineGroup==0)||(LineGroup==1))
			{
				if((fabs(xn-xo)>TOL)||(fabs(yn-yo)>TOL)||(fabs(zn-zo)>TOL))
				{
					gy.x=xo; gy.y=yo; gy.z=zo;					
					gy.Code=LineGroup;	gy.x=xn; gy.y=yn; gy.z=zn;gy.f=v;
					SAVE;
					gcodenum++;
					curvepos[gcodenum-1]=pAry->GetSize()-1; 
					if(gcodenum==3)
					{
						CalDccPos(pAry,PlanGroup);
						gcodenum=2;
						Firstpos.pos=curvepos[0];
						curvepos[0]=curvepos[1];
						curvepos[1]=curvepos[2];
					}
					xo=xn; yo=yn; zo=zn;
				}
			}
			
			if((LineGroup==2)||(LineGroup==3))
			{
				if(Rmode==1)//G02/03 r mode
				{ 
					int IsG03=1;
					if(LineGroup==2) IsG03=-1;
					gy.x=xo; gy.y=yo; gy.z=zo;
					gy.Code=LineGroup;	gy.x=xn; gy.y=yn; gy.z=zn; gy.r=dR;gy.f=v;
					SAVE;
					gcodenum++;
					curvepos[gcodenum-1]=pAry->GetSize()-1; 
					if(PlanGroup==17)
					{
						Center(xo,yo,xn,yn,dR,IsG03,&cenx[gcodenum-1].Re,&cenx[gcodenum-1].Im);
					}
					else if(PlanGroup==18)
					{
						Center(zo,xo,zn,xn,dR,IsG03,&cenx[gcodenum-1].Re,&cenx[gcodenum-1].Im);
					}
					else if(PlanGroup==19)
					{
						Center(yo,zo,yn,zn,dR,IsG03,&cenx[gcodenum-1].Re,&cenx[gcodenum-1].Im);
					}

					gy.rcx=cenx[gcodenum-1].Re;
					gy.rcy=cenx[gcodenum-1].Im;

					if(gcodenum==3)
					{
						CalDccPos(pAry,PlanGroup);
						gcodenum=2;
						Firstpos.pos=curvepos[0];
						curvepos[0]=curvepos[1];
						curvepos[1]=curvepos[2];
					}
					xo=xn; yo=yn; zo=zn;
				}
				else if(Rmode==2)//G02/03 i,j mode
				{
					int IsG02=-1;
					if(LineGroup==2)IsG02=1;
					i=xo+i;j=yo+j;k=zo+k;
					if(PlanGroup==17)
					{
						//gy.cx=i;gy.cy=j;
						gy.rcx=i;gy.rcy=j;
						cenx[gcodenum].Re=i; cenx[gcodenum].Im =j;
						gy.r=GetRadius(xo,yo,xn,yn,i,j,IsG02);
					}
					//gy.r=IJ_ToR(xo,yo,xn,yn,i,j,IsG02);
					else if(PlanGroup==18)
					{
						gy.rcx=k;gy.rcy=i;
						cenx[gcodenum].Re=k; cenx[gcodenum].Im =i;
						gy.r=GetRadius(zo,xo,zn,xn,k,i,IsG02);
					}
					//gy.r=IJ_ToR(zo,xo,zn,xn,k,i,IsG02);
					else if(PlanGroup==19)
					{
						gy.rcx=j;gy.rcy=k;
						cenx[gcodenum].Re=j; cenx[gcodenum].Im =k;
						gy.r=GetRadius(yo,zo,yn,zn,j,k,IsG02);
					}
					//gy.r=IJ_ToR(yo,zo,yn,zn,j,k,IsG02);
					gy.x=xo; gy.y=yo; gy.z=zo;
					gy.Code=LineGroup;	gy.x=xn; gy.y=yn; gy.z=zn; gy.f=v;
					SAVE;	
					gcodenum++;
					curvepos[gcodenum-1]=pAry->GetSize()-1; 
					
					if(gcodenum==3)
					{
						CalDccPos(pAry,PlanGroup);
						gcodenum=2;
						Firstpos.pos=curvepos[0];
						curvepos[0]=curvepos[1];
						curvepos[1]=curvepos[2];
					}
					xo=xn; yo=yn; zo=zn;
				}
			}
			if(xo<Coord[0]) Coord[0]=xo;
			if(yo<Coord[1]) Coord[1]=yo;
			if(zo<Coord[2]) Coord[2]=zo;
			if(xo>Coord[3] && xo!=NONUM+1) Coord[3]=xo;
			if(yo>Coord[4]&& yo!=NONUM+1) Coord[4]=yo;
			if(zo>Coord[5]&& zo!=NONUM+1) Coord[5]=zo;
		}
	}//end while(1);
	//AfxMessageBox("ss");
	gcodenum=0;
	while(1)
	{
		if(pAry->operator [](gcodenum).Code==0 || pAry->operator [](gcodenum).Code==1\
			||pAry->operator [](gcodenum).Code==2 ||pAry->operator [](gcodenum).Code==3)
		{
			pAry->operator [](gcodenum).v=100; 
			break;
		}
		gcodenum++;
	}
	if(pAry->GetSize()>1)pAry->operator [](0).r=gy.N;
	fs.Close();

//	InitLookAhead(angle[1],angle[0]*10*3600*1000,1500000,2000,102);

	/*********************************************/
	FILE *fpp;//debug
	fpp=fopen(SourPath+".tmp","wt");//debug
	for(int iii=0;iii<pAry->GetSize();iii++)
	{
		//fprintf(fpp,"G%d X%.3fY%.3fZ%.3fF%.3f\n",pAry->operator [](iii).Code,pAry->operator [](iii).x,\
		//	pAry->operator [](iii).y,pAry->operator [](iii).z,pAry->operator [](iii).v);
		fprintf(fpp,"%.3f	%.3f	%.3f	%.3f\n",pAry->operator [](iii).x,pAry->operator [](iii).y,pAry->operator [](iii).z,pAry->operator [](iii).v);
	}
	fclose(fpp);//debug*/
	/******************************************/
	if(!compileFinish)
		return 3;
	
	//gy.gcode=NULL; 
	//	str1.Format("%ld,%ld" ,uuu,filelen);
	//	AfxMessageBox(str1);
    return 0;	
}
//////////////////////////////////////////////////////////////////////
//将标准PLT文档转化为CCNCData为记录组成的文档；
//返回0 OK
char PLT_GY3_A (CString SourPath,CCncArray * pAry,WORD dnVel,double scale)
{
	
	if(pAry==NULL)return 1;
	if(SourPath.IsEmpty())return 2;
	pAry->RemoveAll();
	
	CStdioFile fs;
	CCNCData gy;//gy为临时变量
	CString str1;
	int flag1,n;     //该行需存储标志；
	char c1,c2,c3;
	unsigned char  itemp;
	double dtmp;
	bool bPU=true;
	//	long uuu;
	
	if(fs.Open(SourPath, CFile::modeRead | CFile::typeText)==NULL)return 10;
	
	while(1)
	{
		if(fs.ReadString(str1)==NULL)break; 
        str1+='\n';
		gy.N++;
		n=0;
		flag1=0;
		while(str1[n]!='\n'){
			c1=str1[n];if(c1=='\n')break; 
			switch(c1){
			case 'I':
				c2=str1[++n];if(c2=='\n')break;
				if(c2=='N'){gy.Code=9;flag1++;}	  				
				break;
			case 'S':					
				c2=str1[++n];if(c2=='\n')break;	
				if(c2=='P'){
					if(ReadNum(str1,n,itemp)!=0)break;
                    if(itemp==0){gy.Code=8;flag1++;}
					else {gy.Code=11;gy.x=itemp;flag1++;}
				}				
				break;
			case 'P':
				c2=str1[++n];if(c2=='\n')break;	
				if(c2=='U'){
					bPU=true;
					gy.Code=0;flag1++; 
					if(ReadNum(str1,n,dtmp)!=0)break;						
					gy.x=dtmp*scale;flag1++;	
					c3=str1[++n];
					while(!((c3=='\n')||(isdigit(c3)!=0)||(c3==';')||(c3=='-')))c3=str1[++n];	
                    if((c3=='\n')||(c3==';'))break;
					else {
						n--;if(ReadNum(str1,n,dtmp)!=0)break;
						gy.y=dtmp*scale;flag1++;
					}
					c3=str1[++n];
					while(!((c3=='\n')||(isdigit(c3)!=0)||(c3==';')||(c3=='-')))c3=str1[++n];	
                    if((c3=='\n')||(c3==';'))break;
					else{
						n--;if(ReadNum(str1,n,dtmp)!=0)break;
						gy.z=dtmp*scale;flag1++;
					}
				}
				if(c2=='D'){                        
					if(ReadNum(str1,n,dtmp)!=0)break;
					if(bPU){
						bPU=false;gy.Code=101;gy.x=dnVel;SAVE;
					}
					gy.Code=1;  gy.x=dtmp*scale;
					c3=str1[++n];flag1++;
					while(!((c3=='\n')||(isdigit(c3)!=0)||(c3==';')||(c3=='-')))c3=str1[++n];	
                    if((c3=='\n')||(c3==';'))break;
					else {
						n--;if(ReadNum(str1,n,dtmp)!=0)break;
						gy.y=dtmp*scale;flag1++;
					}
					c3=str1[++n];
					while(!((c3=='\n')||(isdigit(c3)!=0)||(c3==';')||(c3=='-')))c3=str1[++n];	
                    if((c3=='\n')||(c3==';'))break;
					else{
						n--;if(ReadNum(str1,n,dtmp)!=0)break;
						gy.z=dtmp*scale;flag1++;
					}
				}
				break;
			default:
				break;
			}//end  switch(c1)
			n++;
		}//while(str1[n]!='\n')
		if(flag1>0)SAVE;	
	}
	if(pAry->GetSize()>1)pAry->operator [](0).r=gy.N;
	fs.Close();//关闭打开文件
	return 0;
}


BOOL  TranslateCode(CString str1,CCNCData* gy)
{
	int n=0,LineGroup,Rmode,PlanGroup;
	unsigned char code;
	double i,j,k,dR,rpn;
	bool bHead=false;
	double xn=0,yn=0,zn=0;
	while(str1[n]!='\n')
	{
		switch(str1[n])
		{
		case '(':
		case 'O':
		case '/':	
			while(str1[n+1]!='\n')n++; 
			break;
		case 'N':
			while((str1[n+1]!='\n')&&(isalpha(str1[n+1])==0))n++;	
			break;
		case '%':
			if(!bHead){bHead=true;gy->Code=9;}
			while(str1[n+1]!='\n')n++; 				
			break;
		case 'X':
			ReadNum(str1,n,xn); 
			break;
		case 'Y':	
			ReadNum(str1,n,yn); 
			break;
		case 'Z':
			ReadNum(str1,n,zn);
			break;
		case 'I':
			ReadNum(str1,n,i); Rmode=2; 
			break;
		case 'J':
			ReadNum(str1,n,j); Rmode=2; 
			break;
		case 'K':
			ReadNum(str1,n,k); Rmode=2;
			break;
		case 'R'://G02.G03的半径,钻孔的回归点
			ReadNum(str1,n,dR); Rmode=1;
			break;
		case 'F':
			ReadNum(str1,n,gy->x);	
			gy->Code=100;
			break;
		case 'S':	
			ReadNum(str1,n,rpn);	 
			break;
		case 'G':
			ReadNum(str1,n,code);
			switch(code){
			case 0:
			case 1:
			case 2:
			case 3:
				LineGroup=code; 
				break;
			case 17://平面选择
			case 18:
			case 19:
				PlanGroup=gy->Code=code;
				break;
			default:
				break;
			}//end switch(str1[n])
			n++;
		}//end while(str1[n]!='\n')  一行分析结束
		if((LineGroup==0)||(LineGroup==1))
		{
			if((fabs(xn-x_old)>TOL)||(fabs(yn-y_old)>TOL)||(fabs(zn-z_old)>TOL))
			{
				gy->Code=LineGroup;	gy->x=xn; gy->y=yn; gy->z=zn;
				x_old=xn; y_old=yn; z_old=zn;
			}
		}
		
		if((LineGroup==2)||(LineGroup==3))
		{
			if(Rmode==1)//G02/03 r mode
			{ 
				gy->Code=LineGroup;	gy->x=xn; gy->y=yn; gy->z=zn; gy->r=dR;
				x_old=xn; y_old=yn; z_old=zn;
			}
			else if(Rmode==2)//G02/03 i,j mode
			{
				bool IsG02=false;
				if(LineGroup==2)IsG02=true;
				i=x_old+i;j=y_old+j;k=z_old+k;
				if(PlanGroup==17)
					gy->r=IJ_ToR(x_old,y_old,xn,yn,i,j,IsG02);
				else if(PlanGroup==18)
					gy->r=IJ_ToR(z_old,x_old,zn,xn,k,i,IsG02);
				else if(PlanGroup==19)
					gy->r=IJ_ToR(y_old,z_old,yn,zn,j,k,IsG02);
				gy->Code=LineGroup;	gy->x=xn; gy->y=yn; gy->z=zn; 
				x_old=xn; y_old=yn; z_old=zn;
			}
		}
	}
	return true;
}

void  ResetOldData()
{
	x_old=y_old=z_old=0;
}

//根据圆弧起点、终点和圆心，求圆弧半径（有符号） 
struct COMPLEX complex(double Re,double Im)
{
	struct COMPLEX c;
	c.Re=Re; c.Im=Im;
	return c;
}

struct COMPLEX add(struct COMPLEX c1, struct COMPLEX c2)
{
	struct COMPLEX c;
	c.Re=c1.Re+c2.Re;c.Im=c1.Im+c2.Im;
	return c;
}

struct COMPLEX sub(struct COMPLEX c1, struct COMPLEX c2)
{
	struct COMPLEX c;
	c.Re=c1.Re-c2.Re;c.Im=c1.Im-c2.Im;
	return c;
}

struct COMPLEX mul(struct COMPLEX c1, struct COMPLEX c2)
{
	struct COMPLEX c;
	c.Re=c1.Re*c2.Re-c1.Im*c2.Im;
	c.Im=c1.Im*c2.Re+c1.Re*c2.Im;
	return c;
}

struct COMPLEX div(struct COMPLEX c1, struct COMPLEX c2)
{
	struct COMPLEX c;
	c.Re=(c1.Re*c2.Re+c1.Im*c2.Im)/(c2.Re*c2.Re+c2.Im*c2.Im);
	c.Im=(c1.Im*c2.Re-c1.Re*c2.Im)/(c2.Re*c2.Re+c2.Im*c2.Im);
	return c;
}

//求复数模
double cabs(struct COMPLEX c)
{
	return sqrt(c.Re*c.Re+c.Im*c.Im);
}

//求复数角度,范围(-PI,PI]
double arg(struct COMPLEX c)	
{
	double angle,Re,Im;
	Re=c.Re; Im=c.Im;
	
	if(Re==0) 
	{ if(Im==0) return 0;
		  else if(Im>0) return PI/2;
		  else return -PI/2;
	}
	return angle=atan2(Im,Re); //atan2返回值(-PI,PI)
	
}

//返回由模和角度(弧度)表示的复数
struct COMPLEX polar(double mag,double angle)
{	
	struct COMPLEX c;
	c.Re=mag*cos(angle);
	c.Im=mag*sin(angle);
	return c;
}

int sign(double num)
{
	if(num>=0) return 1;
	else return -1;
}


double GetRadius(double xs,double ys,double xe,double ye,double xc,double yc,int CircleDirection)
//COMPLEX p1,COMPLEX p2,COMPLEX pcc,
//	  int CircleDirection)
{
	COMPLEX line1,line2;
	double r1,r2,R,angle;	
	COMPLEX p1;
	COMPLEX p2;
	COMPLEX pcc;
	p1.Re =xs;p1.Im =ys;
	p2.Re =xe;p2.Im =ye;
	pcc.Re =xc;pcc.Im =yc;
	
	if (xs==28.75)
		xs=28.75;
	
	line1=sub(p1,pcc);
	line2=sub(p2,pcc);
	angle=CircleDirection*(arg(div(line1,line2)));
	r1=cabs(sub(p1,pcc));
	r2=cabs(sub(p2,pcc));
	if(angle>0)
		R=0.5*(r1+r2);
	else 
		R=-0.5*(r1+r2);
	return R;
}

//已知圆弧起点、终点、方向和圆弧半径(优弧<0，劣弧>0)，求圆心
void Center(double sx,double sy,double ex,double ey,double &r,int CircleDirection,double *re,double *im)
{
#define VALID_DATA 0.0000001
/*struct COMPLEX pcc,line;
double alpha;
COMPLEX p1,p2;
p1.Re=sx,p1.Im=sy;
p2.Re=ex,p2.Im=ey;
line=sub(p2,p1);
alpha=arg(line)-sign(r)*CircleDirection*acos(0.5*cabs(line)/fabs(r));
pcc=add(p1,polar(fabs(r),alpha));
*re=pcc.Re ;
	*im=pcc.Im ;*/
	int flag1=(r<0) ?  -1:1;
	int flag2=CircleDirection;
	double dx=(double)(ex-sx),dy=(double)(ey-sy);
	double length=sqrt(dx*dx+dy*dy);
	double tempr=r;
	
	r=fabs(r);
	if(r < VALID_DATA) 
	{
		return ;
	}
	if( length<VALID_DATA ) 
	{
		return ;
	}
	if( length-2*r > VALID_DATA )
	{
		if(fabs(length-2*r)<0.5)
		{
			tempr=flag1 * length/2;
		}
		else
		return ;
	}
	dx=dx/length;
	dy=dy/length;
	double ppp=fabs(r*r-(dx*dx+dy*dy)/4);
	length=sqrt(ppp);
	*re=((ex+sx)/2-flag1*flag2*dy*length);
	*im=((ey+sy)/2+flag1*flag2*dx*length);
	r=tempr;
	return ;
}


BOOL FileCut(CString SourPath)
{
#define NONUM 1000000
#define TOL   0.0001
	BOOL fileOpen=FALSE;
	CString writeFilename;
	int serial=1;
	writeFilename.Format("%d",serial);
	writeFilename=SourPath+writeFilename;
	CStdioFile ws;
	
	ULONG filesize=0,allSize=0,firstfilesize;
	
	if(SourPath.IsEmpty())return 2;
	
	CStdioFile fs; 
	CString str1;
	CCNCData gy;//gy为临时变量
	double xn,yn,zn,xo,yo,zo,i,j,k,v;//new old x,y,z;
	double depth=0,dR=0,dQ=1;//钻孔深，
	double rpn=1000;//Spindle speed;
	
	bool bLenBC, bG28, bNewTool, bABS, bHead, bReturnR, bZK;//bLenBC刀长补正改变,
	bool bX,bY,bZ;//x,y,z更新
	unsigned char LineGroup,PlanGroup,CrdGrop,BCmode,Rmode;//补偿号，是R或IJK模式
	unsigned char code,ZKcode,H=1,tool=0;
	int n;
	
	if(fs.Open(SourPath, CFile::modeRead | CFile::typeText)==NULL)return 10;
	firstfilesize=fs.GetLength() ;
	if(ws.Open(writeFilename,CFile::modeCreate | CFile::modeWrite | CFile::typeText)==NULL) return 11;;
	fileOpen=TRUE;
	//开机默认
	bLenBC=bG28=bNewTool=bHead=bZK=false;
	bABS=bReturnR=true,
		LineGroup=0; PlanGroup=17; BCmode=49; CrdGrop=54; 
	xn=yn=zn=xo=yo=zo=NONUM+1; gy.N=0; 
	while(1){
		if(fs.ReadString(str1)==NULL)break; //新的一行开始，初始化变量
		str1+='\n';  gy.N++;
		n=Rmode=0;//Rmode 预防G02G03中插入无效行
		i=j=k=v=0; 
		bX=bY=bZ=false;	
		//SAVE;
		
		while(str1[n]!='\n'){
			switch(str1[n]){
			case '(':
			case 'O':
			case '/':	
				while(str1[n+1]!='\n')n++; 
				break;
			case 'N':case 'n':
				while((str1[n+1]!='\n')&&(isalpha(str1[n+1])==0))n++;	
				break;
			case '%':
				if(!bHead){bHead=true;gy.Code=9;}
				while(str1[n+1]!='\n')n++; 				
				break;
			case 'X':case 'x':
				ReadNum(str1,n,xn); 
				bX=true;
				break;
			case 'Y':case 'y':
				ReadNum(str1,n,yn); 
				bY=true;
				break;
			case 'Z':case 'z':
				if(bZK)ReadNum(str1,n,depth);
				else	ReadNum(str1,n,zn);
				bZ=true;
				break;
			case 'I':case 'i':
				ReadNum(str1,n,i); Rmode=2; 
				break;
			case 'J':case 'j':
				ReadNum(str1,n,j); Rmode=2; 
				break;
			case 'K':case 'k':
				ReadNum(str1,n,k); Rmode=2;
				break;
			case 'R':case 'r'://G02.G03的半径,钻孔的回归点
				ReadNum(str1,n,dR); Rmode=1;
				break;
			case 'Q':case 'q'://每次下刀深度
				ReadNum(str1,n,dQ); 
				break;
			case 'T':case 't':
				ReadNum(str1,n,tool);
				break;
			case 'H':
			case 'D':
				ReadNum(str1,n,H);	 
				break;
			case 'F':case 'f':
				ReadNum(str1,n,v);	
				//gy.Code=101;
				break;
			case 'S':case 's':	
				ReadNum(str1,n,rpn);	 
				break;
			case 'G':case 'g':
				ReadNum(str1,n,code);
				switch(code){
				case 0:
				case 1:
				case 2:
				case 3:
					LineGroup=code; 
					break;
				case 4://dwell
					n++;
					while(str1[n]!='X'&&str1[n]!='P'&&str1[n]!='\n')n++;
					if(str1[n]=='X'||str1[n]=='P')
					{
						ReadNum(str1,n,gy.x);
						gy.Code=4;					
					}else n--;					
					break;
				case 17://平面选择
				case 18:
				case 19:
					PlanGroup=gy.Code=code;
					break;
				case 22://软极限，影响x,y,z,i,j,k
				case 68://坐标系旋转，影响x,y,R
					//AfxMessageBox("G22/G68 can't accept");
					while(str1[n+1]!='\n')n++;
					break;
				case 28://回归原点，隐含G49
					bG28=true;  
					break;
				case 43://刀长补正
				case 44:
				case 49:
					bLenBC=true;
					BCmode=code;
					break;
				case 54://原点调用
				case 55:
				case 56:
				case 57: 
				case 58:
				case 59:
					gy.Code=code;	
					xn=yn=zn=xo=yo=zo=NONUM+1;
					break;
				case 73://钻孔功能
				case 81:
				case 83:
					bZK=true;	ZKcode=code;
					break;
				case 80:
					bZK=false;	
					break;						
				case 90:
					bABS=true; 
					break;
				case 91:
					bABS=false; 
					break;
				case 98:
					bReturnR=false;
					break;
				case 99:
					bReturnR=true;
					break;	
				default: 
					break;
				}//End 'G'
				break;	
				case 'M':case 'm':
					ReadNum(str1,n,code);
					switch(code){
					case 2://end 
					case 30:
						gy.Code=8;	
						break;
					case 3:
						gy.y=rpn;
					case 0:
					case 1:
					case 5:
					case 7:
					case 8: 
					case 9:
					case 10:gy.Code=100;gy.x=code;break;
					case 20:gy.Code=100;gy.x=code;break;
					case 11:gy.Code=100;gy.x=code;break;
					case 21:gy.Code=100;gy.x=code;break;
					case 12:gy.Code=100;gy.x=code;break;
					case 22:gy.Code=100;gy.x=code;break;
					case 13:gy.Code=100;gy.x=code;break;
					case 23:gy.Code=100;gy.x=code;break;
					case 14:gy.Code=100;gy.x=code;break;
					case 24:gy.Code=100;gy.x=code;break;
					case 15:gy.Code=100;gy.x=code;break;
					case 25:gy.Code=100;gy.x=code;break;
					case 16:gy.Code=100;gy.x=code;break;
					case 26:gy.Code=100;gy.x=code;break;
					case 17:gy.Code=100;gy.x=code;break;
					case 27:gy.Code=100;gy.x=code;break;
					case 18:gy.Code=100;gy.x=code;break;
					case 28:gy.Code=100;gy.x=code;break;
					case 19:gy.Code=100;gy.x=code;break;
					case 29:gy.Code=100;gy.x=code;break;
						
					case 40:gy.Code=100;gy.x=code;break;
					case 50:gy.Code=100;gy.x=code;break;
					case 41:gy.Code=100;gy.x=code;break;
					case 51:gy.Code=100;gy.x=code;break;
					case 42:gy.Code=100;gy.x=code;break;
					case 52:gy.Code=100;gy.x=code;break;
					case 43:gy.Code=100;gy.x=code;break;
					case 53:gy.Code=100;gy.x=code;break;
					case 44:gy.Code=100;gy.x=code;break;
					case 54:gy.Code=100;gy.x=code;break;
					case 45:gy.Code=100;gy.x=code;break;
					case 55:gy.Code=100;gy.x=code;break;
					case 46:gy.Code=100;gy.x=code;break;
					case 56:gy.Code=100;gy.x=code;break;
					case 47:gy.Code=100;gy.x=code;break;
					case 57:gy.Code=100;gy.x=code;break;
					case 48:gy.Code=100;gy.x=code;break;
					case 58:gy.Code=100;gy.x=code;break;
					case 49:gy.Code=100;gy.x=code;break;
					case 59:gy.Code=100;gy.x=code;break;
					case 32:
						gy.Code=100; gy.x=code;	
						break;
					case 6://换刀
						bNewTool=true;
						break;					
					}//end 'M';
					break;
					default:
						break;
			}//end switch(str1[n])
			n++;
		}//end while(str1[n]!='\n')  一行分析结束
		
		filesize+=str1.GetLength(); 
		allSize+=str1.GetLength(); 
		if(LineGroup==0) 
		{
			if(filesize>5242880 && (firstfilesize-allSize)>1048576)
			{
				ws.WriteString(str1);
				ws.WriteString("M30");
				ws.WriteString("%");
				ws.Close();
				fileOpen=FALSE;
				filesize=0;
				serial++;
				writeFilename.Format("%d",serial);
				writeFilename=SourPath+writeFilename;
				if(ws.Open(writeFilename,CFile::modeCreate | CFile::modeWrite | CFile::typeText)==NULL) return 11;;
				fileOpen=TRUE;
			}
			else
			{
				ws.WriteString(str1); 
			}
		}
		else
		{
			ws.WriteString(str1); 
		}
	}//end while(1);
	if(fileOpen)
		ws.Close(); 
	fs.Close(); 
	return 0;
}