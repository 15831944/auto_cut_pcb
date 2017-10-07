// GY3.cpp: implementation of the CCNCData class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <math.h>
#include "GY3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ER 0.00001

CCNCData::CCNCData()
{
	Clear();
}

CCNCData::~CCNCData()
{
}

void CCNCData::Copy(const CCNCData &old)
{
	N=old.N;
	Code=old.Code;
	x=old.x;
	y=old.y;
	z=old.z;
    r=old.r;
	v=old.v;
	f=old.f;
	rcx=old.rcx;
	rcy=old.rcy;
	//gcode=(char *)malloc(strlen(old.gcode)+1);
	//memset(gcode,0,strlen(old.gcode)+1);
	//strcpy(gcode,old.gcode); 
	//gcode=old.gcode;
}

void CCNCData::Clear()
{
	N=0; Code=0; x=y=z=r=v=f=rcx=rcy=0;//gcode=NULL;
}

bool CCNCData::operator==(CCNCData gy)
{
	if(N!=gy.N)return false;
	else if(Code!=gy.Code)return false;
	else if(fabs(x-gy.x)>ER)return false;
	else if(fabs(y-gy.y)>ER)return false;
	else if(fabs(z-gy.z)>ER)return false;
	else if(fabs(r-gy.r)>ER)return false;
	else if(fabs(rcx-gy.rcx)>ER)return false;
	else if(fabs(rcy-gy.rcy)>ER)return false;
	return true;
}