// MotorControlBenchDoc.h : interface of the CMotorControlBenchDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTORCONTROLBENCHDOC_H__DCC4714F_688D_4E7D_8F8B_4082F3DD716C__INCLUDED_)
#define AFX_MOTORCONTROLBENCHDOC_H__DCC4714F_688D_4E7D_8F8B_4082F3DD716C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CNCData.h"
#include "BufPtr.h"
#include "Position.h"
#include "MainFrm.h"


#include <list>
#include <queue>
using namespace std;

#define LINE_DATA 1000
#define NOVALIDDATA 1000000
#define MAXINNUM 20
#define MAXOUTNUM 5

typedef enum {NULL_STATUS=0,NEW_STATUS=1,START_STATUS,STOP_STATUS,PAUSE_STATUS,CONTINUE_STATUS,RUN_STATUS,OTHER_STATUS} MachineStatus;


/****************************************************************************/
/*  结构名: CMachinePara													*/
/*  描  述: 保存工作台基本参数的数据结构体					                */
/*  功  能:														     		*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
struct CMachinePara
{
	double machRect[6];//工作台行程
	double mmPrePulse[4];
	double arcPrePulse[4];
	int ciraxis;//旋转轴号
	double calibratehig;//对刀器高度
	double calibrateVel;//对刀速度
	short LimSns,HomeSns;
	double HomeVel[4];//四轴的回零速度
	double HomeAcc[4];//四轴的回零加速度
	int HomeDir[4];//回零方向
	int Homestyle;//信号捕捉方式
	bool getindex;
	int HomePosition[4];//原点距离开关的长度
	double delaccpos[4];//减速点位置
	double sysMaxVel;//系统最高速度
	int SpineDelay;//主轴启动延迟时间
	bool startGohome;//程序启动自动回零
	double OptimizeVel[3];
	double OptimizeAngle[2],gacc;	
	int CtrlMode[3] ;  //控制方式Pulse/Dir
	int LookLength;	
	double G00Spd,G01Spd; //G00,G01
	double G00Acc,G01Acc;	
	double OneCirCleLength[3];
	int Mode;
	CMachinePara();
};

/****************************************************************************/
/*  结构名: CCoordPara														*/
/*  描  述: 保存G代码G54~G59值的数据结构体					                */
/*  功  能:														     		*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
struct CCoordPara
{
	double curCoord[3];
	double g54[3];
	double g55[3];
	double g56[3];
	double g57[3];
	double g58[3];
	double g59[3];
	double LastPos[3];
	double LastRefCoord[3];
	CCoordPara();
};

/****************************************************************************/
/*  结构名: CDrawCoordData													*/
/*  描  述: 保存G代码运行当前点位置的数据结构体				                */
/*  功  能:														     		*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
struct CDrawCoordData
{
	double prfx;
	double prfy;
	double prfz;
	CDrawCoordData();
public:	
};

typedef queue<CDrawCoordData>  CCoordQueue;

/****************************************************************************/
/*  类  名: CDrawBuffer														*/
/*  描  述: G代码运行点队列类										        */
/*  功  能:	对G代码运行时的轨迹点进行操作						     		*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
class CDrawBuffer
{
public:
	CCoordQueue q;
	double vel,segno,drawx,drawy,drawz,prfx,prfy,prfz;
	unsigned short LimitState;
	BOOL Error;
private:
	int index,getindex;
	long inMax,outMax;
	int  nInsert,nOut;
	bool benter;
private:
	void init();
public:
	CDrawBuffer();
	//	~CDrawBuffer();
	void Clear();
	int Insert(double prfx,double prfy,double prfz);
	int GetData(CDrawCoordData * pCrd,int *count);
	int GetData(double *prfx,double *prfy,double *prfz);
};


/****************************************************************************/
/*  类  名: CMotorControlBenchDoc											*/
/*  描  述: 文档类													        */
/*  功  能:														     		*/
/*  版  本:	  1.0											                */
/*  修改日期:  20006-11-20	                                                */
/*  修订人:    Long TY					                                    */
/****************************************************************************/
class CMotorControlBenchDoc : public CDocument
{
protected: // create from serialization only
	CMotorControlBenchDoc();
	DECLARE_DYNCREATE(CMotorControlBenchDoc)
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotorControlBenchDoc)
public:
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	// Attributes
	bool bIsAxisMapped;
	double m_dOptiAngle[2];
	double m_dCoord[6];
	double MaxCoord[4]; //G代码绘图区域X、Y的取值范围,用于确定图形放大比例 
						//[0]=XMax,[1]=XMin,[2]=YMax,[3]=YMin
	CDrawBuffer m_drawdata;	
	CCoordPara m_coord;
	MachineStatus m_worksts;
	CMachinePara m_machPara;

public:
	virtual ~CMotorControlBenchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	

	// Generated message map functions
protected:
	CCNCData senddata;
	//{{AFX_MSG(CMotorControlBenchDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()   
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTORCONTROLBENCHDOC_H__DCC4714F_688D_4E7D_8F8B_4082F3DD716C__INCLUDED_)





















