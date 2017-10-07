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
/*  �ṹ��: CMachinePara													*/
/*  ��  ��: ���湤��̨�������������ݽṹ��					                */
/*  ��  ��:														     		*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
/****************************************************************************/
struct CMachinePara
{
	double machRect[6];//����̨�г�
	double mmPrePulse[4];
	double arcPrePulse[4];
	int ciraxis;//��ת���
	double calibratehig;//�Ե����߶�
	double calibrateVel;//�Ե��ٶ�
	short LimSns,HomeSns;
	double HomeVel[4];//����Ļ����ٶ�
	double HomeAcc[4];//����Ļ�����ٶ�
	int HomeDir[4];//���㷽��
	int Homestyle;//�źŲ�׽��ʽ
	bool getindex;
	int HomePosition[4];//ԭ����뿪�صĳ���
	double delaccpos[4];//���ٵ�λ��
	double sysMaxVel;//ϵͳ����ٶ�
	int SpineDelay;//���������ӳ�ʱ��
	bool startGohome;//���������Զ�����
	double OptimizeVel[3];
	double OptimizeAngle[2],gacc;	
	int CtrlMode[3] ;  //���Ʒ�ʽPulse/Dir
	int LookLength;	
	double G00Spd,G01Spd; //G00,G01
	double G00Acc,G01Acc;	
	double OneCirCleLength[3];
	int Mode;
	CMachinePara();
};

/****************************************************************************/
/*  �ṹ��: CCoordPara														*/
/*  ��  ��: ����G����G54~G59ֵ�����ݽṹ��					                */
/*  ��  ��:														     		*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
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
/*  �ṹ��: CDrawCoordData													*/
/*  ��  ��: ����G�������е�ǰ��λ�õ����ݽṹ��				                */
/*  ��  ��:														     		*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
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
/*  ��  ��: CDrawBuffer														*/
/*  ��  ��: G�������е������										        */
/*  ��  ��:	��G��������ʱ�Ĺ켣����в���						     		*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
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
/*  ��  ��: CMotorControlBenchDoc											*/
/*  ��  ��: �ĵ���													        */
/*  ��  ��:														     		*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
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
	double MaxCoord[4]; //G�����ͼ����X��Y��ȡֵ��Χ,����ȷ��ͼ�ηŴ���� 
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





















