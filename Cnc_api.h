#ifndef __CNC_API_H__
#define __CNC_API_H__

#include "mydatatype.h"

///////define in cnc_c.cpp///////
char   Compile (CString SourPath,CCncArray * pAry,double angle,double Coord[],bool bSelDel=1);
char   PLT_GY3_A (CString SourPath,CCncArray * pAry,WORD dnVel,double scale=0.025);
///////define in cnc_math.cpp///////
//double CalArcLen(CCNCData,CCNCData,unsigned char);
//int    R_ToIJ(double xs,double ys,double xe,double ye,double r,double &xc,double &yc);
//int    DY_AngleValue(double x,double y,double xc,double yc,double & Angle);
BOOL  TranslateCode(CString str1,CCNCData* gy);
void  ResetOldData();
double GetRadius(double xs,double ys,double xe,double ye,double xc,double yc,int CircleDirection);
//double GetRadius(double &xs,double &ys,double &xe,double &ye,double &i,double &j,bool IsG02);//
void Center(double sx,double sy,double ex,double ey,double &r,int CircleDirection,double *re,double *im);
BOOL FileCut(CString SourPath);
void InitLookAhead(double T,double amax,double  am,double vmax,int n,double con);
int AddLookdata(BYTE Code,BYTE PlaneGroup,double r,double x,double y,double z,double F,double cx,double cy,int i,long n);
int AddLookdata1(BYTE Code,BYTE PlaneGroup,double r,double x,double y,double z,double F,double cx,double cy,int i,long n,bool flag);
int calvel(double &vel,long &num);
void refreshlookaheadary();
#endif