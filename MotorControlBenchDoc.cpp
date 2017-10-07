// MotorControlBenchDoc.cpp : implementation of the CMotorControlBenchDoc class
//

#include "stdafx.h"
#include "MotorControlBench.h"
#include <math.h>

#include "GTCard.h"
#include "GT400.h"
#include "Global.h"

#include "MotorControlBenchDoc.h"
#include "GCodeCtrlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern bool HookState;
extern CGTCard gtcard;

extern CRITICAL_SECTION s_cs;

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchDoc

IMPLEMENT_DYNCREATE(CMotorControlBenchDoc, CDocument)

BEGIN_MESSAGE_MAP(CMotorControlBenchDoc, CDocument)
	//{{AFX_MSG_MAP(CMotorControlBenchDoc)
//	ON_BN_CLICKED(IDC_BTN_COORD_EFFECT, OnBtnCoordEffect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchDoc construction/destruction

/*******************************************/
CMachinePara::CMachinePara()
{
	int i;
	CMotorControlBenchApp * pApp=((CMotorControlBenchApp *) AfxGetApp());
	for(i=0;i<6;i++)
		machRect[i]=0;
	for(i=0;i<4;i++)
	{
		mmPrePulse[i]=2000;
		mmPrePulse[i]=pApp->m_nAxisPPR[i];
		arcPrePulse[i]=100;
		HomeVel[i]=pApp->m_fHomeVel[i];
		HomeAcc[i]=pApp->m_fHomeAcc[i];
		HomeDir[i]=-1;
	}
    G00Spd=pApp->m_fG00Spd;
	G01Spd=pApp->m_fG01Spd;
	G00Acc=pApp->m_fG00Acc;
	G01Acc=pApp->m_fG01Acc;

	ciraxis=0;
	calibratehig=10;
	calibrateVel=1000;
	LimSns=255;HomeSns=0;sysMaxVel=6000;SpineDelay=0;startGohome=FALSE;
	OptimizeVel[0]=OptimizeVel[1]=OptimizeVel[2]=1000;
	OptimizeAngle[0]=OptimizeAngle[1]=15;
	CtrlMode[0]=CtrlMode[1]=CtrlMode[2]=0;
	LookLength=100;
	OneCirCleLength[0]=OneCirCleLength[1]=OneCirCleLength[2]=5;
	gacc=2;
	Mode=0;	//0:Step;1:Server
}


CDrawCoordData::CDrawCoordData()
{
	prfx=0;prfy=0;prfz=0;
}

CCoordPara::CCoordPara()
{
	int i;
	for (i=0;i<3;i++)
	{
		curCoord[i]=0;g54[i]=0;
		g55[i]=0;
		g56[i]=0;
		g57[i]=0;
		g58[i]=0;
		g59[i]=0;
		LastPos[i]=0;
		LastRefCoord[i]=0;
	}
}


////////////////////DrawBuffer
CDrawBuffer::CDrawBuffer()
{
	init();
}

void CDrawBuffer::init()
{
	drawx=drawy=drawz=0;
	segno=0;
	vel=0;
	inMax=outMax=0;
	LimitState=0;  //1:1axis 2:2axis 3:3axis
	Error=FALSE;
}

void CDrawBuffer::Clear()
{
	init();
}

int CDrawBuffer::Insert(double prfx,double prfy,double prfz)
{
	CDrawCoordData data;
	static double xo=0,yo=0,zo=0;
	if(fabs(xo-prfx)<1 && fabs(yo-prfy)<1 && fabs(zo-prfz)<1)
		return 1;
	data.prfx =prfx;
	data.prfy =prfy;
	data.prfz =prfz;
	if(q.size()<5000)
	{
	TRACE("1\n");
	q.push(data);
	}
	xo=prfx;
	yo=prfy;
	zo=prfz;
	return 1;	

}

int CDrawBuffer::GetData(double *prfx,double *prfy,double *prfz)
{
	CDrawCoordData data;
	if(!q.empty())
	{
		data=q.front(); 
		q.pop();
		TRACE("2\n");
		*prfx=data.prfx;
		*prfy=data.prfy;
		*prfz=data.prfz;
		return 0;
	}
	return 1;
}

int CDrawBuffer::GetData(CDrawCoordData * pCrd,int *count)
{
/*	int ror,i;

	if(benter)
	{
		*count=0;
		return -1;
	}

	ror=nOut>*count?*count:nOut;
	for(i=0;i<ror;i++)
	{
		if((inMax<0)||(nOut<=0)) 
			break;
		if(getindex>LINE_DATA-1) getindex=0;
		pCrd->prfx=drawdata[getindex].prfx;
		pCrd->prfy=drawdata[getindex].prfy;
		pCrd->prfz=drawdata[getindex].prfz;
//		TRACE( "IPOS[0] = %f,IPOS[1] = %f,IPOS[2] = %f \n", pCrd->prfx,pCrd->prfy,pCrd->prfz);
		pCrd++;
		getindex++;
		inMax--;
		nOut--;
	}

	*count=i;
	if((inMax<0)||(nOut<=0))
	{
		benter=true;
		nOut=MAXOUTNUM;
	}
*/
	return inMax+1;
}

/*******************************************/


CMotorControlBenchDoc::CMotorControlBenchDoc()
{
	m_dOptiAngle[0]=100;
	m_dOptiAngle[1]=200;
	MaxCoord[0]=50; 
	MaxCoord[1]=-50;
	MaxCoord[2]=50;
	MaxCoord[3]=-50;
	gtcard.pDoc =this;
	bIsAxisMapped=false;
	m_worksts=NULL_STATUS;   
}

CMotorControlBenchDoc::~CMotorControlBenchDoc()
{
//	delete pos;
}



void CMotorControlBenchDoc::DeleteContents()
{
	TRACE("Entering CMotorControlBenchDoc::DeleteContents\n");
	/*
	while(m_pointlist.GetHeadPosition())
	{
		delete m_pointlist.RemoveHead();
	}
	*/
}
/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchDoc serialization

void CMotorControlBenchDoc::Serialize(CArchive& ar)
{
	TRACE("Entering CMotorControlBenchDoc::Serialize\n");
	if (ar.IsStoring())
	{

	}
	else
	{

	}
//	m_pointlist.Serialize(ar);
//	m_pointlist1.Serialize(ar);	
}

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchDoc diagnostics

#ifdef _DEBUG
void CMotorControlBenchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMotorControlBenchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchDoc commands

































































