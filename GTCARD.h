
#include"MotorControlBenchDoc.h"
#include"GCodeDrawView.h"

#define VALIDNUM -100000000
  
 /***************************************************************************/
 /*  类  名: CGTCard														*/
 /*  描  述: 该类中封装GT卡功能函数和涉及板卡操作的函数		                */
 /*  说	明:														     		*/
 /*  版  本:	  1.0											            */
 /*  修改日期:  20006-11-20	                                                */
 /*  修订人:    Long TY					                                    */
 /***************************************************************************/
 class CGTCard  
 {
 public:
	 bool AxisGMotion(int mainAxis,int AuxAxis,int Ratio);
	 void AxisVelMotion(short nAxis,double fAcc,double fVel);
	 short AxisSMotion(int nAxis,long pos,double vel,double acc,double jerk);
	 short AxisTMotion(int nAxis,long pos,double vel,double acc);
	 bool IsAxisMoving(int nAxis);
	 bool CloseCard();
	 bool GetInptSts(unsigned short nInPortNo);
	 int Jog(unsigned short nAxis,double lDist, double fVel,short  nDirect);
	 short Home(unsigned short nAxis,short nHomeMode);
	 short HomeAll(short nHomeMode);	 
	 void SingleAxisMove(int nAxis, int step);
	 void SetPID(UINT nAxis,UINT Kp,UINT Ki,UINT Kd);
	 void SetCoord(double x,double y,double z);
	 short MakeCoord(double *p);
	 void GetEncPos(double *pos);
	 short ViewPos();
	 bool Drawing;
	 CGCodeDrawView * pDrawView;
	 short SetSynAcc(double acc);
	 void MapAxis(int xMap,int yMap,int zMap,int nAxisCount,int nViewPln);
	 void AxisInitial(int ctrl_mode,int dir_pulse,short kp,short ki,short kd,int axis);
	 short InitCard();
	 short OpenPort(unsigned short val,bool bIsOpen);
	 short EndList();
	 short StopMotion();
	 short ReStartMotion();
	 short StartMotion();
	 CMotorControlBenchDoc * pDoc;
	 short LineXYZ(double x,double y,double z);
	 short LineXYZG0(double x,double y,double z);
	 short ArcZX(double z_cen,double x_cen,double angle);
	 short ArcYZ(double y_cen,double z_cen,double angle);
	 short ArcXY(double x_cen,double y_cen,double angle);
	 bool ArcZXP(double z_end,double x_end,double r,short dir);
	 bool ArcYZP(double y_end,double z_end,double r,short dir);
	 bool ArcXYP(double x_end,double y_end,double r,short dir);
	 bool IsBufferEmpty();
	 bool ClrAxisStatus();
	 short SetSynVel(double vel);
	 short MoveXYZ(double x,double y,double z,double vel,double acc);
	 short GetPrflPnt(double *pnt);
	 void ShowErrorMsg(CString str);
	 bool IsCrdMove();
	 short AddList();
	 short StartList(); //启动缓冲区 
	 bool InitBuffer(); //初始化缓冲区
	 bool IsCardMove(); //轴在运动？
	 CGTCard();
	 virtual ~CGTCard();
	 
 private:
	 int m_nIOValue;
	 void error(short rtn);
	 double TagPosition[4];
 };



