#include "stdafx.h"
#include "GTCARD.h"
#include "GT400.H"
#include "Global.h"
#include <math.h>

#include <string.h>

#define cst_PosZero 0.0000001

CGTCard gtcard;
extern bool Escapted;


#define PEEKWHILE(msg)    {ltoa(i++,s,10);\
							msgstr=msg;\
							msgstr+=s;\
							}

CGTCard::CGTCard()
{
	pDoc=NULL;
	pDrawView=NULL;
	Drawing=true;
//	m_nIOValue=0x0;  //�˴�����Ҫ����ʵ����ȷ��
	m_nIOValue=0xFFFF;
}

CGTCard::~CGTCard()
{
  GT_Close();
}

/****************************************************************************/
/*  ������: IsCardMove      												*/
/*  ��  �ܣ��ж��Ƿ����ᴦ������ϵ�˶�״̬									*/
/*  ��  ����																*/
/*  ��  �ڣ�����������ϵ�˶�=T,�������������˶�=F							*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGTCard::IsCardMove()
{
	unsigned short rtn,sts=0;	
	rtn=GT_GetCrdSts(&sts);
	if (rtn!=0) return FALSE;
	if((sts&0x1)==0)return TRUE;
	if (sts&0x200) return FALSE;
	return FALSE;
}


/****************************************************************************/
/*  ������: InitBuffer      												*/
/*  ��  �ܣ���ʼ���忨����ϵ�˶�������										*/
/*  ��  ����																*/
/*  ��  �ڣ���ʼ���ɹ�=T,�������򲻳ɹ�=F									*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGTCard::InitBuffer()
{
	BYTE i=0;
	char s[20]="";
	CString msgstr;
	if(IsCrdMove()) 
	{
	//	ShowErrorMsg("�����˶��в��ܽ��д˲���!");
	//	return FALSE;
	}
	if(StartList()) 
	{
	//	ShowErrorMsg("�����˶��в��ܽ��д˲���!");
	//	return FALSE;
	}

	double cp[4]={0,0,0,0};          //��ʼλ������ֵ
//	double cp[4];	
	if(GetPrflPnt(cp)) 
	{
		ShowErrorMsg("�����˶��в��ܽ��д˲���!");
		return FALSE;
	}

	int temp=pDoc->m_machPara.G01Spd;
    if(MoveXYZ(cp[0],cp[1],cp[2],pDoc->m_machPara.G01Spd,pDoc->m_machPara.G01Acc)) 
	{
		ShowErrorMsg("�����˶��в��ܽ��д˲���!");
		return FALSE;
	}
	if(SetSynVel(pDoc->m_machPara.G01Spd)) 
	{
		ShowErrorMsg("�����˶��в��ܽ��д˲���!");
		return FALSE;
	}
	
	return TRUE;
}


/****************************************************************************/
/*  ������: StartList	      												*/
/*  ��  �ܣ��򿪰忨����ϵ�˶�������										*/
/*  ��  ����																*/
/*  ��  �ڣ��򿪳ɹ�=0														*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
short CGTCard::StartList()
{
	short rtn,i=0;
	while(1)
	{
		rtn=GT_StrtList();
		if (rtn==0)break;
		else
		{i++;if (i>10) break;}
	}
	if (i>10) 
	{
		ShowErrorMsg("�򿪿��ƿ�������ʧ��!");
		return rtn;
	}
	else return 0;
}

/****************************************************************************/
/*  ������: AddList	      													*/
/*  ��  �ܣ��������ϵ�˶�����������										*/
/*  ��  ����																*/
/*  ��  �ڣ��ɹ�=0															*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
short CGTCard::AddList()
{
 	short rtn,i=0;
	while(1)
	{
		rtn=GT_AddList();
		if (rtn==0)break;
		else
		{i++;if (i>10) break;}
	}
	if (i>10) 
	{
		ShowErrorMsg("���´򿪿��ƿ�������ʧ��!");
		return rtn;
	}
	else return 0;
}

bool CGTCard::IsCrdMove()//�����˶�������T
{
	unsigned short rtn,sts=0;	
	rtn=GT_GetCrdSts(&sts);
	if (rtn!=0) return FALSE;
	if((sts&0x1)==0)return TRUE;
	if (sts&0x200) return FALSE;
	return FALSE;
}

void CGTCard::ShowErrorMsg(CString str)
{
  AfxMessageBox(str,/*"ϵͳ����",*/MB_OK | MB_ICONSTOP); 
}

short CGTCard::GetPrflPnt(double *pnt)
{
	short rtn=GT_GetPrfPnt(pnt); 
	return rtn;
}


/****************************************************************************/
/*  ������: MoveXYZ	      													*/
/*  ��  �ܣ���ϵ�˶���������λ												*/
/*  ��  ����double x, double y, double z, ��λ������,��λΪ����ӳ�䵥λ		*/
/*			double vel    ��λ�ٶ�											*/
/*			double acc	  ��λ���ٶ�										*/
/*  ��  �ڣ��ɹ�=0															*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
short CGTCard::MoveXYZ(double x, double y, double z, double vel, double acc)
{
	short rtn=GT_MvXYZ(x,y,z,vel*1000*0.0002/60,acc*1000*0.0002/60*0.0002/60);	
	return rtn;	  
}

short CGTCard::SetSynVel(double vel)
{
	short rtn,i=0;
	double tempVel=vel*1000*0.0002/60;
	while(1)
	{
		rtn=GT_SetSynVel(vel*1000*0.0002/60); //vel��λ��m/min
		
		if (rtn==0)break;
		else
		{i++;if (i>10) break;}
	}
	if (i>10) 
		return rtn;
	else return 0;
}


/****************************************************************************/
/*  ������: ClrAxisStatus    												*/
/*  ��  �ܣ���������ᵱǰ״̬												*/
/*  ��  ����																*/
/*  ��  �ڣ��ɹ�=0															*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGTCard::ClrAxisStatus()
{
		short rtn;
		for (int i=1;i<4;i++)
		{
			rtn=GT_Axis(i);
			rtn=GT_ClrSts();
		}
		return 0;
}


/****************************************************************************/
/*  ������: IsBufferEmpty    												*/
/*  ��  �ܣ��ж�����ϵ�˶��������Ƿ�Ϊ��									*/
/*  ��  ����																*/
/*  ��  �ڣ�������Ϊ��=T													*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGTCard::IsBufferEmpty()
{
	
	unsigned short sts,rtn;
	rtn=GT_GetCrdSts(&sts);
	if (rtn!=0) return FALSE;
	if ((sts&0x2000)==0x2000)//�������գ�������ֵ
		return TRUE;
	return FALSE;
	
}

short CGTCard::LineXYZG0(double x, double y, double z)
{
	short rtn=GT_LnXYZ(x,y,z);
	if(rtn==0)
	{
		TagPosition[0]=x;
		TagPosition[1]=y;
		TagPosition[2]=z;
	}
	error(rtn);
	return rtn;	
}

short CGTCard::LineXYZ(double x, double y, double z)
{
	short rtn=GT_LnXYZ(x,y,z);
	if(rtn==0)
	{
		TagPosition[0]=x;
		TagPosition[1]=y;
		TagPosition[2]=z;
	}
	return rtn;	
}

short CGTCard::ArcXY(double x_cen, double y_cen, double angle)
{
  // short rtn=GT_ArcXY(x_cen*pDoc->m_machPara.mmPrePulse[0],y_cen*pDoc->m_machPara.mmPrePulse[0],angle);
  short rtn=GT_ArcXY(x_cen,y_cen,angle);

	return rtn;
}

short CGTCard::ArcYZ(double y_cen, double z_cen, double angle)
{	
 //  short rtn=GT_ArcYZ(y_cen*pDoc->m_machPara.mmPrePulse[0],z_cen*pDoc->m_machPara.mmPrePulse[0],angle);
  short rtn=GT_ArcYZ(y_cen,z_cen,angle);
 
   return rtn;
}

short CGTCard::ArcZX(double z_cen, double x_cen, double angle)
{
//	short rtn=GT_ArcZX(z_cen*pDoc->m_machPara.mmPrePulse[0],x_cen*pDoc->m_machPara.mmPrePulse[0],angle);
	short rtn=GT_ArcZX(z_cen,x_cen,angle);

	return rtn;
}

bool CGTCard::ArcXYP(double x_end, double y_end, double r, short dir)
{
//	if(GT_ArcXYP(x_end*pDoc->m_machPara.mmPrePulse[0],y_end*pDoc->m_machPara.mmPrePulse[0],r*pDoc->m_machPara.mmPrePulse[0],dir)!=0) return 1;  //debug
	if(GT_ArcXYP(x_end,y_end,r,dir)!=0) return 1;  //debug
		
	return 0;
}

bool CGTCard::ArcYZP(double y_end, double z_end, double r, short dir)
{
//	if(GT_ArcYZP(y_end*pDoc->m_machPara.mmPrePulse[0],z_end*pDoc->m_machPara.mmPrePulse[0],r*pDoc->m_machPara.mmPrePulse[0],dir)!=0) return 1;
	if(GT_ArcYZP(y_end,z_end,r,dir)!=0) return 1;

	return 0;
}

bool CGTCard::ArcZXP(double z_end, double x_end, double r, short dir)
{
//	if(GT_ArcZXP(z_end*pDoc->m_machPara.mmPrePulse[0],x_end*pDoc->m_machPara.mmPrePulse[0],r*pDoc->m_machPara.mmPrePulse[0],dir)!=0) return 1;
   if(GT_ArcZXP(z_end,x_end,r,dir)!=0) return 1;

	return 0;
}


/****************************************************************************/
/*  ������: StartMotion		   												*/
/*  ��  �ܣ���ʼ������������ִ��											*/
/*  ��  ����																*/
/*  ��  �ڣ�������Ϊ��=T													*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
short CGTCard::StartMotion()
{
	short rtn,i=0;
	while(1)
	{
		rtn=GT_StrtMtn();
		if (rtn==0)break;
		else
		{i++;if (i>10) break;}
	}
	if (i>10) return rtn;
	else return 0;

}

short CGTCard::ReStartMotion()
{
// return GT_RestoreMtn();
	return 0;
}

short CGTCard::StopMotion()//=0,�ɹ�ֹͣ
{
	short rtn,i=0;
	while(1)
	{
		rtn=GT_EStpMtn();
		if (rtn==0)break;
		else
		{i++;if (i>10) break;}
	}
	if (i>10) return rtn;
	else return 0;

}

short CGTCard::EndList()
{
	short rtn,i=0;
	while(1)
	{
		rtn=GT_EndList();
		if (rtn==0)break;
		else
		{i++;if (i>10) break;}
	}
	if (i>10) 
	{
		ShowErrorMsg("�������ƿ�������ʧ��!");
		return rtn;
	}
	else return 0;
}


/****************************************************************************/
/*  ������: OpenPort		   												*/
/*  ��  �ܣ�ִ��ͨ��IO������												*/
/*  ��  ����unsigned short val    ������IO��λ								*/
/*          bool bIsOpen    T=������IO��,F=�رո�IO��						*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
short CGTCard::OpenPort(unsigned short val,bool bIsOpen)
{
	int port=0x1;
	if(val<0 || val>15) return 3;
	port=(int)pow((double)2, (int)val);
	if (bIsOpen )
	{
		port=~port;
		m_nIOValue=m_nIOValue & port;
	}
	else
	{
		m_nIOValue=m_nIOValue| port;
	}
	return 	GT_ExOpt(m_nIOValue);

}

short CGTCard:: InitCard()
{
	short rtn;
	unsigned short LmtSense=0xff;
	unsigned int EncSense=0xF;

	rtn=GT_Close();
	Sleep(20);
	rtn=GT_Open();
	if(rtn)
	{
		AfxMessageBox("�忨����ʧ�ܣ������˳���\n�������ļ�������Ƿ���ȷ��װ�忨",MB_OK|MB_ICONERROR);
//		return 2;

	}
	if(GT_Reset()!=0) 
	{
		TRACE0("�忨���ô���!");
		AfxMessageBox("�忨���ô���!",MB_OK|MB_ICONERROR);
		return 1;
	}

    rtn=GT_SetSmplTm(200);
	for(int i=1;i<5;i++)
	{
		rtn=GT_Axis(i);
		rtn=GT_SetIntrMsk(0);
	}


	rtn=GT_LmtSns(LmtSense);
    //  rtn=GT_Axis(3);
    //  rtn=GT_RstSts(0xff9f);

	#ifdef GT_SV
	rtn=GT_EncSns(EncSense);
	#endif

   return rtn;
}

void CGTCard::AxisInitial(int ctrl_mode, int dir_pulse, short kp, short ki, short kd, int axis)
{
    short rtn;
    if(kp<=0||kp>30) 	
    {
        kp=1;	
    }
    
    rtn=GT_Axis(axis);
    rtn=GT_ClrSts();

    if(1==ctrl_mode)
    	rtn = GT_CtrlMode(1);
    else
    	rtn=GT_CtrlMode(0);

    switch(ctrl_mode) {
    case STEP_MOTOR:
    	if(dir_pulse == DIR_MODE)       rtn=GT_StepDir();
    	else if(dir_pulse == PULSE_MODE)     rtn = GT_StepPulse();
    	else 
    		AfxMessageBox("choose DIR+PULSE or +/- PULSE for the motor");
    	break;
    case SERVO_MOTOR:
         rtn = GT_SetKp(kp);        //Error(rtn,"SetKp");
    	 rtn = GT_SetKi(ki);        //Error(rtn,"SetKi");
         rtn = GT_SetKd(kd);        //Error(rtn,"SetKd");
      	 break;
    default:
    	 break;
    }
    Sleep(10);
    rtn = GT_Update();
}

void CGTCard::error(short rtn)
{
	switch(rtn)
	{
	case -1:
		TRACE0("error:communciation error\n");break;
	case 0:
		break;
	case 1:
		TRACE0("error:command error\n");break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 7:
		TRACE0("error:parameter error\n");break;
	case 6:
		TRACE0("ERROR:map is error\n");break;
	default:
		break;
	}		
}


/****************************************************************************/
/*  ������: MapAxis			   												*/
/*  ��  �ܣ�������ӳ���ϵ													*/
/*  ��  ����int xMap    X��ӳ������										*/
/*	        int yMap    Y��ӳ������										*/
/*	        int zMap    Z��ӳ������										*/
/*          int nAxisCount ϵͳ������										*/
/*          int nViewPln    G��������ʱ��ǰѡ�����ʾƽ��					*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGTCard::MapAxis(int xMap,int yMap,int zMap,int nAxisCount,int nViewPln)
{
  short rtn;
  double cnt1[5]={pDoc->m_machPara.mmPrePulse[0],0,0,0,0};
  double cnt2[5]={0,pDoc->m_machPara.mmPrePulse[1],0,0,0};
  double cnt3[5]={0,0,pDoc->m_machPara.mmPrePulse[2],0,0};
  double cnt4[5]={0,0,0,pDoc->m_machPara.mmPrePulse[3],0};
  
  switch(nAxisCount) {
  case 1:
	  break;
  case 2:
	  switch(nViewPln) {
	  case XY_PLANE:
		  rtn=GT_MapAxis(xMap,cnt1);
		  rtn=GT_MapAxis(yMap,cnt2);
		  break;
	  case YZ_PLANE:
		  rtn=GT_MapAxis(yMap,cnt2);
		  rtn=GT_MapAxis(zMap,cnt3);		  
		  break;
	  case XZ_PLANE:
		  rtn=GT_MapAxis(xMap,cnt1);
		  rtn=GT_MapAxis(zMap,cnt3);
		  break;	  
	  }
	  break;
  case 3:	 
  case 4:
	  rtn=GT_MapAxis(xMap,cnt1);
	  rtn=GT_MapAxis(yMap,cnt2);
	  rtn=GT_MapAxis(zMap,cnt3); 
	  break;
  } 
}

short CGTCard::SetSynAcc(double acc)
{
	short rtn,i=0;
	
	while(1)
	{
 	 rtn=GT_SetSynAcc(acc*1000*0.0002/60*0.0002/60);//acc��λ��m/min^2
		if (rtn==0)break;
		else
		{i++;if (i>10) break;}
	}
	if (i>10) 
	{
		ShowErrorMsg("����ϵͳ���ٶ�ʧ��!");
		return rtn;
	}
	else return 0;
}

short CGTCard::ViewPos()
{
	/*
	double pos[4];	
	GetEncPos(pos);
	pDoc->m_drawdata.drawx=pos[0];
	pDoc->m_drawdata.drawy=pos[1];
	pDoc->m_drawdata.drawz=pos[2];	   
	GetPrflPnt(pos);
	pDoc->m_drawdata.Insert(pos[0],pos[1],pos[2]);
	pDoc->m_drawdata.prfx=pos[0];
	pDoc->m_drawdata.prfy=pos[1];
	pDoc->m_drawdata.prfz=pos[2];	
	double vel=0;
	GT_GetPrfVel(&vel);
	pDoc->m_drawdata.vel=(vel*60)/(pDoc->m_machPara.mmPrePulse[0]*0.0002);	
	*/
	return 0;
}

void CGTCard::GetEncPos(double *pos)
{
	long pos1;
	GT_Axis(1);
	GT_GetAtlPos(&pos1);
	long temp=pDoc->m_machPara.mmPrePulse[0];//debug
	pos[0]=((double)pos1)/pDoc->m_machPara.mmPrePulse[0];

	GT_Axis(2);
	GT_GetAtlPos(&pos1);
	pos[1]=((double)pos1)/pDoc->m_machPara.mmPrePulse[1];

	GT_Axis(3);
	GT_GetAtlPos(&pos1);
	pos[2]=((double)pos1)/pDoc->m_machPara.mmPrePulse[2];
    
	GT_Axis(4);
	GT_GetAtlPos(&pos1);
	pos[3]=((double)pos1)/pDoc->m_machPara.mmPrePulse[3];

	//��դ�߸���������ͨ��,���ϴ�ѧ//
	/*_Axis(5);
	GT_GetAtlPos(&pos1);
	pos[4]=((double)pos1)/500;

	GT_Axis(6);
	GT_GetAtlPos(&pos1);
	pos[5]=((double)pos1)/500;*/
}	

short CGTCard::MakeCoord(double *p)
{
	short rtn=0;
	double cnt1[5]={p[0]*pDoc->m_machPara.mmPrePulse[0],0,0,0};
    double cnt2[5]={0,p[0]*pDoc->m_machPara.mmPrePulse[0],0,0};
	double cnt3[5]={0,0,p[0]*pDoc->m_machPara.mmPrePulse[0],0};
	rtn=GT_MapAxis(1,cnt1);
	if(rtn!=0) return rtn;
	rtn=GT_MapAxis(2,cnt2);
	if(rtn!=0) return rtn;
	rtn=GT_MapAxis(3,cnt3);
	return rtn;
}

void CGTCard::SetCoord(double x, double y, double z)
{
	/*
	if(x!=VALIDNUM)
	pDoc->m_coord.curCoord[0]=x;
	if(y!=VALIDNUM)
	pDoc->m_coord.curCoord[1]=y;
	if(z!=VALIDNUM)
	pDoc->m_coord.curCoord[2]=z;
	MakeCoord(pDoc->m_coord.curCoord);
	*/
}

void CGTCard::SetPID(UINT nAxis,UINT Kp, UINT Ki, UINT Kd)
{
	short rtn;
	rtn=GT_Axis(nAxis);
	rtn = GT_SetKp(Kp);      error(rtn);
	rtn = GT_SetKi(Ki);      error(rtn);
	rtn = GT_SetKd(Kd);      error(rtn);
	Sleep(10);
	rtn=GT_Update();

}
bool ProcessMessage();

/****************************************************************************/
/*  ������: Home			   												*/
/*  ��  �ܣ�����															*/
/*  ��  ����unsigned short nAxis    ��������								*/
/*	        short nHomeMode    ���㷽ʽ										*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
short CGTCard::Home(unsigned short nAxis, short nHomeMode)
{
    short rtn;
	unsigned short status = 0;
	long lPosLimPos,lNegLimPos,lZeroPos;
	double fHomeVel(3),fHomeAcc(0.01);


	fHomeVel=pDoc->m_machPara.HomeVel[nAxis-1]/**1000*0.0002/60*/;
	fHomeAcc=pDoc->m_machPara.HomeAcc[nAxis-1]/**1000*0.0002/60*0.0002/60*/;

	switch(nHomeMode) {
	case HOME_TWOLIMITS:
		//������ת��//
		rtn=GT_Axis(nAxis);
		rtn=GT_ClrSts();
		rtn=GT_PrflT();
		rtn=GT_SetVel(fHomeVel);
		rtn=GT_SetAcc(fHomeAcc);
		rtn=GT_SetPos(500000);
		rtn=GT_Update();
		rtn=GT_GetSts(&status);		
		//�������λ�ź�//
		while ((status&AXIS_STS_POSITIVE_LIMIT)!=AXIS_STS_POSITIVE_LIMIT) {
		//	ProcessMessage();
			if(status&AXIS_STS_MOTION_FINISH==AXIS_STS_MOTION_FINISH) 
			{
				AfxMessageBox("�˶���ɣ�δ��⵽����λ�ź�",MB_OK);
				return 0;
			}
			rtn=GT_GetSts(&status);	
		}
	//	AfxMessageBox("����λ����",MB_OK);        
		//�ᷴ��ת��//
		Sleep(200);
		rtn=GT_GetAtlPos(&lPosLimPos);     
		rtn=GT_SetPos(-500000);		
		rtn=GT_Update();

		rtn=GT_ClrSts();
		Sleep(200);
		//��⸺��λ�ź�//
		rtn=GT_GetSts(&status);	
		while ((status&AXIS_STS_NEGATIVE_LIMIT)!=AXIS_STS_NEGATIVE_LIMIT) {
		//		ProcessMessage();
			if(status&AXIS_STS_MOTION_FINISH==AXIS_STS_MOTION_FINISH) 
			{
				AfxMessageBox("�˶���ɣ�δ��⵽����λ�ź�",MB_OK);
				return 0;
			}
			rtn=GT_GetSts(&status);	
		}		
	//	AfxMessageBox("����λ����",MB_OK);
        //ת����������λ�е�//
		rtn=GT_ClrSts();
		Sleep(200);
        rtn=GT_GetAtlPos(&lNegLimPos);
        lZeroPos=(long) abs(lPosLimPos-lNegLimPos)/2;
	//	rtn=GT_ZeroPos();
	//	Sleep(10);
		rtn=GT_SetPos(lNegLimPos+lZeroPos);
		rtn=GT_Update();  
		
		rtn=GT_ClrSts();
		Sleep(100);
		//�������Ƿ����//
		rtn=GT_GetSts(&status);	
		while(!(status&AXIS_STS_MOTION_FINISH)){
	//		ProcessMessage();
			rtn=GT_GetSts(&status);
		}	
		AfxMessageBox("������ɣ�",MB_OK);
		rtn=GT_ZeroPos();
		return 1;
	
		break;
	case HOME_ONELIMIT:
		break;
	case HOME_ZERO:
		break;
	case HOME_ZERO_INDEX:
		break;
	}

	return 0;
}

short CGTCard::HomeAll(short nHomeMode)
{
    short rtn;
    CString strInfo;

	unsigned short statusX=0,statusY=0 ;
	long lPosLimPosX, lNegLimPosX, lPosLimPosY,lNegLimPosY,lZeroPos;
	double fHomeVelX(3),fHomeAccX(0.01);
	double fHomeVelY(3),fHomeAccY(0.01);


	fHomeVelX=pDoc->m_machPara.HomeVel[0]/**1000*0.0002/60*/;
	fHomeAccX=pDoc->m_machPara.HomeAcc[0]/**1000*0.0002/60*0.0002/60*/;
    
	fHomeVelY=pDoc->m_machPara.HomeVel[1]/**1000*0.0002/60*/;
	fHomeAccY=pDoc->m_machPara.HomeAcc[1]/**1000*0.0002/60*0.0002/60*/;

	switch(nHomeMode) {
	case HOME_TWOLIMITS:
		//������ת��//
		rtn=GT_Axis(1);
		rtn=GT_ClrSts();
		rtn=GT_PrflT();
		rtn=GT_SetVel(fHomeVelX);
		rtn=GT_SetAcc(fHomeAccX);
		rtn=GT_SetPos(500000);
		rtn=GT_Update();

        rtn=GT_Axis(2);
		rtn=GT_ClrSts();
		rtn=GT_PrflT();
		rtn=GT_SetVel(fHomeVelY);
		rtn=GT_SetAcc(fHomeAccY);
		rtn=GT_SetPos(500000);
		rtn=GT_Update();

		rtn=GT_Axis(1);
		rtn=GT_GetSts(&statusX);	
        rtn=GT_Axis(2);
        rtn=GT_GetSts(&statusY);	
		//�������λ�ź�//
		while ((statusX&AXIS_STS_POSITIVE_LIMIT_X)!=AXIS_STS_POSITIVE_LIMIT_X
                      || (statusY&AXIS_STS_POSITIVE_LIMIT_Y)!=AXIS_STS_POSITIVE_LIMIT_Y) 
             {
		//	ProcessMessage();
			if(statusX&AXIS_STS_MOTION_FINISH_X==AXIS_STS_MOTION_FINISH_X
                     && statusY&AXIS_STS_MOTION_FINISH_Y==AXIS_STS_MOTION_FINISH_Y) 
			{
				AfxMessageBox("�˶���ɣ�δ��⵽����λ�ź�",MB_OK);
				return 0;
			}

        		rtn=GT_Axis(1);
        		rtn=GT_GetSts(&statusX);	
                   rtn=GT_Axis(2);
                	rtn=GT_GetSts(&statusY);	
		}

       //        strInfo.Format("position:0x%x, 0x%x", statusX, statusY);
	//	  AfxMessageBox(strInfo, MB_OK);

	//	AfxMessageBox("����λ����",MB_OK);        
		//�ᷴ��ת��//
		Sleep(200);
    
    	rtn=GT_Axis(1);
		rtn=GT_ClrSts();
		rtn=GT_PrflT();

		rtn=GT_GetAtlPos(&lPosLimPosX);  
        rtn=GT_SetVel(fHomeVelX);
		rtn=GT_SetAcc(fHomeAccX);

		rtn=GT_SetPos(-500000);		
		rtn=GT_Update();
		//rtn=GT_ClrSts();

        rtn=GT_Axis(2);
		rtn=GT_ClrSts();
		rtn=GT_PrflT();

		rtn=GT_GetAtlPos(&lPosLimPosY);     
        rtn=GT_SetVel(fHomeVelY);
		rtn=GT_SetAcc(fHomeAccY);

		rtn=GT_SetPos(-500000);		
		rtn=GT_Update();
		//rtn=GT_ClrSts();

		Sleep(200);
		//��⸺��λ�ź�//
		rtn=GT_Axis(1);
		rtn=GT_GetSts(&statusX);	
             rtn=GT_Axis(2);
        	rtn=GT_GetSts(&statusY);	
             
		while ((statusX&AXIS_STS_NEGATIVE_LIMIT_X)!=AXIS_STS_NEGATIVE_LIMIT_X 
                      || (statusY&AXIS_STS_NEGATIVE_LIMIT_Y)!=AXIS_STS_NEGATIVE_LIMIT_Y) 
             {
		//		ProcessMessage();
			if(statusX&AXIS_STS_MOTION_FINISH_X==AXIS_STS_MOTION_FINISH_X
                      && statusY&AXIS_STS_MOTION_FINISH_Y==AXIS_STS_MOTION_FINISH_Y) 
			{
				AfxMessageBox("�˶���ɣ�δ��⵽����λ�ź�",MB_OK);
				return 0;
			}

        		rtn=GT_Axis(1);
        		rtn=GT_GetSts(&statusX);	
                   rtn=GT_Axis(2);
                	rtn=GT_GetSts(&statusY);	
		}		
	//	AfxMessageBox("����λ����",MB_OK);
        //ת����������λ�е�//
      		rtn=GT_Axis(1);

		rtn=GT_ClrSts();
		Sleep(200);
            rtn=GT_GetAtlPos(&lNegLimPosX);
            lZeroPos=(long) abs(lPosLimPosX-lNegLimPosX)/2;
	//	rtn=GT_ZeroPos();
	//	Sleep(10);
		rtn=GT_SetPos(lNegLimPosX+lZeroPos);
		rtn=GT_Update();  
		
		rtn=GT_ClrSts();

        rtn=GT_Axis(2);

		rtn=GT_ClrSts();
		Sleep(200);
            rtn=GT_GetAtlPos(&lNegLimPosY);
            lZeroPos=(long) abs(lPosLimPosY-lNegLimPosY)/2;
	//	rtn=GT_ZeroPos();
	//	Sleep(10);
		rtn=GT_SetPos(lNegLimPosY+lZeroPos);
		rtn=GT_Update();  
             rtn=GT_ClrSts();

		Sleep(100);
		//�������Ƿ����//
		rtn=GT_Axis(1);
		rtn=GT_GetSts(&statusX);	
             rtn=GT_Axis(2);
        	rtn=GT_GetSts(&statusY);	

		while(!(statusX&AXIS_STS_MOTION_FINISH_X)
             ||!(statusY&AXIS_STS_MOTION_FINISH_Y))
             {
	//		ProcessMessage();
        		rtn=GT_Axis(1);
        		rtn=GT_GetSts(&statusX);	
                   rtn=GT_Axis(2);
                	rtn=GT_GetSts(&statusY);	
		}	
		AfxMessageBox("������ɣ�",MB_OK);
		rtn=GT_ZeroPos();
		return 1;
	
		break;
	case HOME_ONELIMIT:
		break;
	case HOME_ZERO:
		break;
	case HOME_ZERO_INDEX:
		break;
	}

	return 0;
}

/****************************************************************************/
/*  ������: Jog				   												*/
/*  ��  �ܣ��㶯															*/
/*  ��  ����unsigned short nAxis    �㶯�����								*/
/*	        double lDist	�㶯����										*/
/*	        double fVel		�㶯�ٶ�										*/
/*	        short nDirect	�㶯����	1=����,-1=����						*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
int CGTCard::Jog(unsigned short nAxis, double lDist, double fVel,short nDirect)
{
    short rtn;
    unsigned short status;
    long lPos;
    int nPPR=pDoc->m_machPara.mmPrePulse[nAxis];

    fVel=fVel*nPPR*1000*0.0002/60;//���ٶȵ�λ���������/��������

    rtn=GT_Axis(nAxis+1);
    rtn=GT_GetSts(&status);
    if(IsCardMove()) 
    {
        AfxMessageBox("�����˶���!",MB_OK);
        return -1;
    }
    rtn=GT_ClrSts();
    rtn=GT_PrflT();
    rtn=GT_SetVel(fVel);
    rtn=GT_SetAcc(0.1);
    rtn=GT_GetAtlPos(&lPos);
    lPos=lPos+nDirect*nPPR*lDist;
    rtn=GT_SetPos(lPos);	
    rtn=GT_Update();

    return 0;

}

bool CGTCard::GetInptSts(unsigned short nInPortNo)
{
	short rtn;
	int port;
	unsigned short ex_inp;
	port=(int)pow((double)2, (int)nInPortNo);
    
	rtn=GT_ExInpt(&ex_inp);
	if ((~ex_inp)&port) {
		return true;
	}
	else
		return false;
}

bool CGTCard::CloseCard()
{
	short rtn;
	for(int i=0;i<4;i++)
	{
		rtn=GT_Axis(i);
		rtn=GT_AxisOff();
	}
	rtn=GT_Close();
	return (bool) rtn;
}


/****************************************************************************/
/*  ������: IsAxisMoving	   												*/
/*  ��  �ܣ��ж��Ƿ����ᴦ�ڵ����˶�										*/
/*  ��  ����int nAxis      ���												*/
/*  ��  �ڣ������˶�=T,	���˶�ֹͣ=F										*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGTCard::IsAxisMoving(int nAxis)
{
	short rtn;
	unsigned short sts;
	
	rtn=GT_Axis(nAxis);
	rtn=GT_ClrSts();
	Sleep(10);
	rtn=GT_GetSts(&sts);
	if (sts&0x1==0x400) {
		return true;
	}
	else
		return false;	
}

short CGTCard::AxisTMotion(int nAxis, long pos, double vel, double acc)
{
	short rtn;
	GT_Axis(nAxis);
	GT_ClrSts();
	rtn=GT_PrflT();
	GT_SetVel(vel);
	GT_SetAcc(acc);
	rtn=GT_SetMAcc(0.4);
	rtn=GT_SetPos(pos);
	rtn=GT_Update();
	return rtn;
}

void CGTCard::SingleAxisMove(int nAxis, int step)
{
	short rtn;
	//	unsigned short status;
	long lPos;
	
	rtn=GT_Axis(nAxis);
	rtn=GT_ClrSts();
	rtn=GT_PrflT();
	rtn=GT_SetVel(5);
	rtn=GT_SetAcc(0.1);
	rtn=GT_GetAtlPos(&lPos);
	lPos=lPos+step;
	rtn=GT_SetPos(lPos);	
	rtn=GT_Update();	
}

short CGTCard::AxisSMotion(int nAxis, long pos, double vel, double acc, double jerk)
{
	short rtn;
    rtn=GT_Axis(nAxis);
	rtn=GT_PrflS();	  	   
	rtn=GT_SetVel(vel);	 
	rtn=GT_SetAcc(acc);
    rtn=GT_SetMAcc(0.4);	   
    rtn=GT_SetJerk(jerk);
    rtn=GT_SetPos(pos);
	rtn=GT_Update();
   return rtn;
}

void CGTCard::AxisVelMotion(short nAxis, double fAcc, double fVel)
{
	short rtn;
	rtn=GT_Axis(nAxis);
	rtn=GT_PrflV();
	rtn=GT_SetAcc(fAcc);
	rtn=GT_SetVel(fVel);		
	rtn=GT_SetMAcc(0.4);
	rtn=GT_Update();
}

/****************************************************************************/
/*  ������: AxisGMotion		   												*/
/*  ��  �ܣ����õ��ӳ����˶�												*/
/*  ��  ����int mainAxis      ���ӳ����������								*/
/*			int AuxAxis	      ���ӳ��ִӶ����								*/
/*			int Ratio	      ���ӳ��ִ�����								*/
/*  ��  �ڣ������˶�=T,	���˶�ֹͣ=F										*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGTCard::AxisGMotion(int mainAxis, int AuxAxis, int Ratio)
{
	short rtn;
	rtn=GT_Axis(AuxAxis);
	if(rtn)
	{
	//	MessageBox("�Ӷ��Ὺ��ʧ�ܣ�","�̸߿Ƽ�",MB_OK|MB_ICONERROR);
		return false;
	}
	rtn=GT_PrflG(mainAxis);
	rtn=GT_SetRatio(Ratio);
	if (rtn) {
	//	MessageBox("���ӳ�������ʧ�ܣ�","�̸߿Ƽ�",MB_OK|MB_ICONERROR);
		return false;
	}
	Sleep(100);
	rtn=GT_Update();
	if (rtn) {
	//	MessageBox("���ӳ�������ʧ�ܣ�","�̸߿Ƽ�",MB_OK|MB_ICONERROR);
		return false;
	}
	return true;
}
