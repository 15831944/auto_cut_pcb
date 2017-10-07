// GCodeCtrlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MotorControlBench.h"
#include "GCodeCtrlWnd.h"
#include "MotorControlBenchDoc.h"
#include "GTCard.h"
#include "GT400.h"
#include"GTCNCLib.h"
#include <math.h>
#include "Global.h"

#include "DigitalDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CRITICAL_SECTION s_cs;
bool HookState;
CCncArray glcncarray;
extern CGTCard gtcard;

IMPLEMENT_DYNCREATE(CGCodeCtrlWnd, CFormView)

CGCodeCtrlWnd::CGCodeCtrlWnd()
	: CFormView(CGCodeCtrlWnd::IDD)
{
	//{{AFX_DATA_INIT(CGCodeCtrlWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
   CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
   m_nAxisCount=pApp->m_nAxisCount;
    xMap=1;
	yMap=2;
	zMap=3;   
    
	m_StartLine=0;
	bIsAxisMapped=false;
	m_bIsAxisOn=false;
	m_bIsCardInit=false;
	bFileLoaded=false;
	m_bXHomed=false;
	m_bYHomed=false;
	bShouldSave=FALSE;
	bIsSaved=FALSE;
	bIsModified=FALSE;
    bAutoCuting=FALSE;
	override=CurrentOverride=1;
	bStop=TRUE;
	CrdPlan=17;
	StartPosition=0;
	EndPosition=0;
	StartUserSet=FALSE;
	SingleRun=FALSE;
	//	Paused=FALSE;
	InitializeCriticalSection(&s_cs);
	//////////////    
	pos=new CPOSITION;
    InitCard();
}

CGCodeCtrlWnd::~CGCodeCtrlWnd()
{
}

void CGCodeCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGCodeCtrlWnd)
	DDX_Control(pDX, IDC_TAB_VIEWSELECT, m_tabViewSel);
	DDX_Control(pDX, IDC_LIST_NCCODE, m_ctrlNCList);
	DDX_Control(pDX, IDC_EDIT_GCODE, m_GCodeEdit);
	DDX_Control(pDX, IDC_CMB_AXIS_Z, m_cmbAxisZ);
	DDX_Control(pDX, IDC_CMB_AXIS_Y, m_cmbAxisY);
	DDX_Control(pDX, IDC_CMB_AXIS_X, m_cmbAxisX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGCodeCtrlWnd, CFormView)
	//{{AFX_MSG_MAP(CGCodeCtrlWnd)
	ON_BN_CLICKED(IDC_BTN_NEW_NCCOED, OnBtnNewNccoed)
	ON_BN_CLICKED(IDC_BTN_OPEN_NCCODE, OnBtnOpenNccode)
	ON_BN_CLICKED(IDC_BTN_LOAD_NCCODE, OnBtnLoadNccode)
	ON_BN_CLICKED(IDC_BTN_CONTROL_RUN, OnBtnControlRun)
	ON_BN_CLICKED(IDC_BTN_CONTROL_PAUSE, OnBtnControlPause)
	ON_BN_CLICKED(IDC_BTN_CONTROL_STOP, OnBtnControlStop)
	ON_BN_CLICKED(IDC_BTN_CLOSEAXIS, OnBtnCloseaxis)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_SAVE_NCCODE, OnBtnSaveNccode)
	ON_BN_CLICKED(IDC_BTN_COORD_EFFECT, OnBtnCoordEffect)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_GCODE, OnChangeEditGcode)
	ON_BN_CLICKED(IDC_BTN_CONTROL_XHOME, OnBtnControlXhome)
	ON_BN_CLICKED(IDC_BTN_CONTROL_YHOME, OnBtnControlYhome)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCodeCtrlWnd diagnostics

#ifdef _DEBUG
void CGCodeCtrlWnd::AssertValid() const
{
	CFormView::AssertValid();
}

void CGCodeCtrlWnd::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGCodeCtrlWnd message handlers


void CGCodeCtrlWnd::OnBtnNewNccoed() 
{
	GetDlgItem(IDC_LIST_NCCODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_GCODE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_GCODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText("");	
	GetDlgItem(IDC_EDIT_GCODE)->SetWindowText("");
	GetDlgItem(IDC_EDIT_GCODE)->SetFocus();
	GetDlgItem(IDC_BTN_SAVE_NCCODE)->EnableWindow(TRUE);

	gtcard.pDoc->m_worksts=NULL_STATUS;
	gtcard.pDrawView->Invalidate();
	bFileLoaded=false;
	
}

void CGCodeCtrlWnd::OnBtnOpenNccode() 
{
	CString strFilePathName;
	CString strGCode;
	CStdioFile fs;
	CString strLine;
	
	char filters[] = "CNC File(*.txt)|*.txt|*.nc";
	CFileDialog fileDlg(TRUE,NULL,"*.txt",NULL,filters,NULL);

	int result = fileDlg.DoModal();	
	if(result==IDOK)
	{
		strFilePathName=fileDlg.GetPathName();
    }
	else
		return;
	
	if(fs.Open(strFilePathName, CFile::modeRead | CFile::typeText)==NULL) 
	{
		AfxMessageBox("打开G代码文件出错！",MB_OK);
		return;
	}
    
	if (fs.GetLength()==NULL) 
	{
		AfxMessageBox("指定的文件内容为空！",MB_OK);
		return;
	}
	GetDlgItem(IDC_LIST_NCCODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_GCODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_GCODE)->ShowWindow(SW_SHOW); 
	
	
	int index(1);
	while(1){
		if(fs.ReadString(strLine)==NULL)break; 	
		strLine.Format("%s\r\n",strLine);
		strGCode+=strLine;	
	}
	fs.Close();
	GetDlgItem(IDC_EDIT_GCODE)->SetWindowText(strGCode);
	GetDlgItem(IDC_BTN_SAVE_NCCODE)->EnableWindow(TRUE);
    GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(strFilePathName);
	GetDlgItem(IDC_LIST_NCCODE)->SetFocus();
	
	gtcard.pDoc->m_worksts=NULL_STATUS;
	gtcard.pDrawView->Invalidate();   
    bFileLoaded=false;
}

void CGCodeCtrlWnd::OnBtnLoadNccode() 
{
	CString strNCFileName;
	CStdioFile fs;
	CString strLine;
	CString str;

	CEdit* pNCEdit=(CEdit *)GetDlgItem(IDC_EDIT_GCODE);
	
	pNCEdit->GetWindowText(str);
    if(str=="")
	{
		AfxMessageBox("当前没有任何G代码可编译！\n 请打开或新建一个G代码文件",MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(gtcard.IsCrdMove())
	{
		AfxMessageBox("当前有G代码正在执行！",MB_OK|MB_ICONWARNING);
		return;
	}
	if(bShouldSave&&!bIsSaved)
	{
		if(IDNO==AfxMessageBox("您修改了G代码文件，请先保存！\n点击YES将编译修改前的文件！点击NO取消此次操作",MB_YESNO|MB_ICONINFORMATION))
		{
			bShouldSave=false;
			return;
		}	
	}

	((CStatic *) GetDlgItem(IDC_STATIC_FILENAME))->GetWindowText(strNCFileName);
	
	if(strNCFileName=="")
	{
		AfxMessageBox("您尚未指定编译的G代码文件名",MB_OK|MB_ICONWARNING);
		return;
	}	
	
	//编译G代码
	char i=Compile(strNCFileName,&glcncarray,m_dOptiAngle,MaxCoord,true); 

	//出错处理
	if(i){ 
		FreeMem();
		glcncarray.RemoveAll();
		switch(i)
		{
		case 1:ErrorMessage("内存指针错误!");break;
		case 4:ErrorMessage("读取文件错误!");break;
		case 3:ErrorMessage("文件缺少M30或M02或读取文件出错!");break;
		case 11:ErrorMessage("文件太大,请将文件先处理后再加工!");break;
		default:
			ErrorMessage("读取文件时产生未知错误!");break;
		}
		return;
	}	 
    
	CncLength=glcncarray.GetSize();


	int nG0Line(0);	
	CCNCData tempNCData;	
	for(int iLine=0;iLine<CncLength;iLine++)
	{
		tempNCData=glcncarray.operator [](iLine);
	/*调试用
	if(tempNCData.Code==0)
		{			
			m_nG0LineNo[nG0Line]=iLine+1;
			nG0Line++;
		}
	*/
	}

	GetDlgItem(IDC_BTN_CONTROL_RUN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CONTROL_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_RESET)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CLOSEAXIS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CONTROL_PAUSE)->EnableWindow(TRUE);
	
    GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(strNCFileName);

	gtcard.pDoc->m_worksts=NEW_STATUS;
	int temp=gtcard.pDoc->m_worksts;
	gtcard.pDrawView->bReDraw=true;
	gtcard.pDrawView->Invalidate();
//	gtcard.pDoc->HookState=TRUE;
    bFileLoaded=true;
	
}

/****************************************************************************/
/*  函数名: FreeMem		      												*/
/*  功  能：清空NC代码阵列													*/
/*  参  数： 	无															*/
/*  出  口：  glcncarray 被清空												*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
 void CGCodeCtrlWnd::FreeMem()
{

 char *temp=NULL;
 	for(int i=0;i<glcncarray.GetSize()-1;i++) 
 	{
 		if(glcncarray.operator [](i).gcode !=NULL && glcncarray.operator [](i).gcode!=temp)
 		{
 	    	temp=glcncarray.operator [](i).gcode;
 	    	free(glcncarray.operator [](i).gcode); 		
 		}
 	} 
 }

void CGCodeCtrlWnd::ErrorMessage(CString str)
{
	MessageBox(str,"系统错误",MB_OK | MB_ICONSTOP);

}

void CGCodeCtrlWnd::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_tabViewSel.InsertItem(0,"选择显示平面");
	m_tabViewSel.InsertItem(1,"缩放操作");
	m_tabViewSel.InsertItem(2,"点动操作");
	m_tabViewSel.InitTabPage();

	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();	
	int nAxisCount=pApp->m_nAxisCount;    
	switch(nAxisCount) {
	case 1:
		break;
	case 2:
		m_cmbAxisX.ResetContent();
        m_cmbAxisX.InsertString(0,"1轴电机");
		m_cmbAxisX.InsertString(1,"2轴电机");
		m_cmbAxisY.ResetContent();
        m_cmbAxisY.InsertString(0,"1轴电机");
		m_cmbAxisY.InsertString(1,"2轴电机");
		m_cmbAxisZ.ResetContent();
        m_cmbAxisZ.InsertString(0,"1轴电机");
		m_cmbAxisZ.InsertString(1,"2轴电机");
		m_cmbAxisZ.EnableWindow(FALSE);

		m_cmbAxisX.SetCurSel(0);
		m_cmbAxisY.SetCurSel(1);
		m_cmbAxisZ.SetCurSel(0); 
		break;
	case 3:
	case 4:
		m_cmbAxisX.SetCurSel(0);
		m_cmbAxisY.SetCurSel(1);
		m_cmbAxisZ.SetCurSel(2);  
		break;	
	}
 	
	GetDlgItem(IDC_BTN_SAVE_NCCODE)->EnableWindow(FALSE);	
	GetDlgItem(IDC_BTN_CONTROL_RUN)->EnableWindow(FALSE);//运行
	GetDlgItem(IDC_BTN_CONTROL_PAUSE)->EnableWindow(FALSE);//暂停
    GetDlgItem(IDC_BTN_CONTROL_STOP)->EnableWindow(FALSE);//停止
	GetDlgItem(IDC_BTN_CLOSEAXIS)->EnableWindow(FALSE);//关闭轴
	GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);//复位
	GetDlgItem(IDC_EDIT_GCODE)->EnableWindow(FALSE);	
	GetDlgItem(IDC_LIST_NCCODE)->ShowWindow(SW_HIDE);	
}

bool CGCodeCtrlWnd::IsLengZero(double xs, double xe, double ys, double ye, double zs, double ze)
{
	double TOL=0.001;
	return((fabs(xs-xe)<TOL)&&(fabs(ys-ye)<TOL)&&(fabs(zs-ze)<TOL));
}

void CGCodeCtrlWnd::OnBtnControlRun() 
{
	double pnt[4];
	
	if (!m_bIsCardInit) {
		AfxMessageBox("板卡已被复位，请点击“坐标映射生效”按钮后再运行",MB_OK|MB_ICONWARNING);
		return;
	}
	if (!bIsAxisMapped) {
		AfxMessageBox("尚未确认轴映射关系，请点击“坐标映射生效”按钮后再运行",MB_OK|MB_ICONWARNING);
		return;
	}
	if (!m_bIsAxisOn) {
		AfxMessageBox("轴尚未伺服上电，请点击“坐标映射生效”按钮后再运行",MB_OK|MB_ICONWARNING);
		return;
	}	
	
	if(!bFileLoaded)
	{
		AfxMessageBox("尚未编译G代码，请点击“编译”按钮后再运行",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(!bStop)//如果当前G代码未运行完毕
	{
		AfxMessageBox("当前G代码尚未运行完毕，请先点击“停止”按钮停止当前代码运行！",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(gtcard.IsCrdMove())
	{
		AfxMessageBox("轴运动中！",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if (CheckG92()) return;  //未对刀至G92指令指定位置，则返回
	
	if (CheckIsZero()==false) {
		int res;
		res=MessageBox("运行起始位置不在编程原点，是否继续操作？","XX科技",MB_YESNO|MB_ICONQUESTION);
		if (IDNO==res) return;
	}

	OnBtnCoordEffect(); 
	
	SendLine=0;				//发送的行数
	if(!gtcard.InitBuffer())//初始化缓冲区  
	{EndSent();	return;}	//缓冲区初始化失败，停止数据发送，并返回
	
	m_StartLine=0;			//启动段号位置

	MotionStarted=FALSE;	//是否启动缓冲区执行

	bSending=TRUE;			//是否正在向缓冲区发送数据

	bPause=FALSE;			//是否在暂停中

	bSendEnd=FALSE;			//发送是否结束

	HookState=FALSE;		//是否停止键盘钩子

	bAutoCuting=TRUE;		//是否在自动运行中

	BakGy.Clear();			//清除缓冲区备份

	gtcard.GetPrflPnt(pnt);	//确定轴当前点位置 
	oldxx=pnt[0];	oldyy=pnt[1];	oldzz=pnt[2];

	CurrentCutNo=0;			//记录当前G代码试图行数,避免重复刷新

	SendSeg=0;				//记录向缓冲区已发送的指令条数

	NeedRestoreMtn=FALSE;	//是恢复运行还是启动运行

	bAutoStarted=FALSE;		//启动控制卡运行 T=已启动  F=未启动

	Paused=TRUE;			//开始系统为暂停状态

	bStop=FALSE;			//开始时完成标志为FALSE
	
	CString strNCFileName,strLine;
	CStdioFile fs;
	((CStatic *) GetDlgItem(IDC_STATIC_FILENAME))->GetWindowText(strNCFileName);
	if(strNCFileName=="")
	{
		AfxMessageBox("您尚未指定编译的G代码文件名",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(fs.Open(strNCFileName, CFile::modeRead | CFile::typeText)==NULL) 
	{
		AfxMessageBox("载入G代码文件出错！",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if (fs.GetLength()==NULL) 
	{
		AfxMessageBox("文件内容为空！",MB_OK|MB_ICONWARNING);
		return;
	}	
	
	GetDlgItem(IDC_EDIT_GCODE)->ShowWindow(SW_HIDE);
	m_ctrlNCList.ShowWindow(SW_SHOW);
	
	m_ctrlNCList.ResetContent();
	
	char NoStr[6];
	int index(1);
	while(1){
		if(fs.ReadString(strLine)==NULL)break; 	
		sprintf(NoStr,"%04d  ",index);
		m_ctrlNCList.InsertString(-1,NoStr+strLine);
		index++;
	}
	fs.Close();
	
	LoopSend();
	
	GetDlgItem(IDC_BTN_CONTROL_PAUSE)->EnableWindow(TRUE);
	(GetDlgItem(IDC_BTN_CONTROL_STOP))->EnableWindow(TRUE);
}

void CGCodeCtrlWnd::OnBtnControlPause() 
{
	if (!m_bIsCardInit) {
		MessageBox("板卡已被复位，\n请点击“坐标映射生效”重新运行","XX科技",MB_OK|MB_ICONINFORMATION);
		return;	
	}
	if (bStop) {
		MessageBox("G代码文件执行已经停止，\n请点击“运行”重新运行","XX科技",MB_OK|MB_ICONINFORMATION);
		return;
	}
	
	if(!bPause)
	{
		MotionPause();
		PauseRun();
		GetDlgItem(IDC_BTN_CONTROL_PAUSE)->SetWindowText("继续");
	}
	else
	{
		short rtn;
		char  repeat=0;
		
		while(gtcard.IsCrdMove());
		Sleep(10);		
		
		if(!bSendEnd&&(SendLine<CncLength)){
			repeat=0;			
			while(rtn!=0)
			{
				if(repeat++>10)
				{
					AfxMessageBox("err GT_AddList()");
					break;
				}
				Sleep(10);
				rtn=gtcard.AddList();
			}
		}
		if(bAutoStarted)
			NeedRestoreMtn=TRUE;
		bAutoStarted=FALSE;
		rtn=GT_StrtMtn();
		bPause=FALSE;
		HookState=FALSE;
		GetDlgItem(IDC_BTN_CONTROL_PAUSE)->SetWindowText("暂停");
		return;
	}	
}

void CGCodeCtrlWnd::OnBtnControlStop() 
{
	CString str;
	if (!m_bIsCardInit) {
		MessageBox("板卡已被复位，\n请先点击“坐标映射生效”按钮 ","XX科技",MB_OK|MB_ICONINFORMATION);
		return;	
	}
	/**********************/
	//解决停止后退出错误问题//
	//2006.12.01//
	gtcard.EndList();
	FreeMem();
	glcncarray.RemoveAll();
	/**********************/
	if (!gtcard.StopMotion()) {
		
      bStop=true;
	  str="各轴运动停止！";
	  GetDlgItem(IDC_BTN_CONTROL_PAUSE)->SetWindowText("暂停");
	  bPause=false;
	}
	else 
		str="运动停止失败！";

	MessageBox(str,"XX科技",MB_OK|MB_ICONINFORMATION);	
}

void CGCodeCtrlWnd::OnBtnCloseaxis() 
{
   short rtn;
   if (!m_bIsCardInit) {
	   MessageBox("板卡已被复位0，\n请点击“坐标映射生效”重新运行","XX科技",MB_OK|MB_ICONINFORMATION);
	   return;	
   }
	for(int i=1;i<=m_nAxisCount;i++)
	{
		rtn=GT_Axis(i);
		rtn=GT_AxisOff();
	}    
	GetDlgItem(IDC_BTN_CONTROL_PAUSE)->SetWindowText("停止");
	bPause=false;
	m_bIsAxisOn=false;
	bStop=true;
	MessageBox("各轴伺服已关闭！","XX科技",MB_OK|MB_ICONINFORMATION);
}

void CGCodeCtrlWnd::OnBtnReset() 
{
	if (gtcard.IsCardMove()) {
		MessageBox("请先停止轴运动，再进行板卡复位操作","XX科技");
		return;
	}
	InitCard();    
	m_bIsCardInit=false;
	bStop=true;
	MessageBox("运动控制卡已复位！","XX科技",MB_OK|MB_ICONINFORMATION);
	

}

/****************************************************************************/
/*  函数名: EndSent		      												*/
/*  功  能：NC代码发送完处理												*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::EndSent()
{
	StartUserSet=FALSE;	
	if(gtcard.EndList()!=0) 
	{
		MessageBox("关闭缓冲区失败！","XX科技",MB_OK|MB_ICONWARNING);
		return;
	}

	while(gtcard.IsCrdMove())
	{	
		ProcessMessage();
		GetMotionNum();
	}
	GetMotionNum();

	bStop=true;
	gtcard.Drawing=true;
	m_ctrlNCList.SetCurSel(m_ctrlNCList.GetCount()-1);
}

void CGCodeCtrlWnd::GetMotionNum()
{
	//	GT_GetMtnNm(&line); 
}

/****************************************************************************/
/*  函数名: AxisGo		      												*/
/*  功  能：启动坐标系运动缓冲区中命令										*/
/*  参  数： 	无															*/
/*  出  口：  启动命令成功=T，不成功=F										*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGCodeCtrlWnd::AxisGo()
{
	short rtn;	
	if(NeedRestoreMtn)
	{
		rtn=gtcard.ReStartMotion();	 
	}
	else
	{
		rtn=gtcard.StartMotion();//启动命令 		
	}
	if(rtn==0)
	{
		bAutoStarted=TRUE;//控制卡启动
		NeedRestoreMtn=TRUE;
	}
	return  (bool) rtn;
}

/****************************************************************************/
/*  函数名: LoopSend	      												*/
/*  功  能：向缓冲区内循环发送CNC指令										*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::LoopSend()
{
	while(!(bSendEnd&&gtcard.IsBufferEmpty())) //如果数据发送完且缓冲区已经为空，则不执行循环
	{
		ProcessMessage(); //界面消息处理
		if(bSending&&!bPause) //如果正在发送数据且未按下暂停
		{
			if(!bSendEnd&&(SendLine<CncLength))//如果发送停止未置位且数据未发完，则
			{
				EnterCriticalSection(&s_cs);
				senddata=glcncarray.operator [](SendLine++); //获取将要发送的CNC指令
				SentCnc(); //发送数据
				LeaveCriticalSection(&s_cs);
			}
			else 
			{
				bSendEnd=TRUE;//发送停止标志置位
				if(!bAutoStarted) AxisGo();
			}
		}
		if(StartUserSet) //如果用户定义了起始段
		{
			if(SendLine>=(ULONG)EndPosition) 
			{
				AxisGo();
				break;
			}
		}
		GeneralRun();
		if(bSendEnd) GetMotionNum();
		//GetMotionNum();
		if(bPause) //如果设置了暂停
		{
			PauseRun();
		}
		
		while(bPause)
		{
			ProcessMessage();
			if(!Paused) PauseRun();
			if(!bSending) break;
		}
	
        if(!bSending )break;	
	}
	
	EndSent();
	bAutoCuting=FALSE;
	HookState=TRUE;	
}

/****************************************************************************/
/*  函数名: LoopSend	      												*/
/*  功  能：向缓冲区发送一条CNC指令											*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::SentCnc()
{
	short rtn=0;
	double dl=0;//dl is 空间线段长度  
	bool bCmdIn=false;//本次输入运动指令
	double dwLineLen=0;//线段长度

	switch(senddata.Code)
	{
	case 0:	
	case 1:		
	case 2:
	case 3:
		if(senddata.x>=NOVALIDDATA) senddata.x=oldxx;
		if(senddata.y>=NOVALIDDATA) senddata.y=oldyy;
		if(senddata.z>=NOVALIDDATA) senddata.z=oldzz;
		break;
	}     	
	
	//速度更新
	/*
	if(!m_sysPara.useF)//如果使用系统设置的速率，则
	{
	if(senddata.Code==0)
	{
	ModfiySynVel(m_sysPara.g0Vel);
	}
	else if((senddata.Code==1)||(senddata.Code==2)||(senddata.Code==3))
	{
	ModfiySynVel(m_sysPara.g1Vel);
	}
	}
	else
	{
	ModfiySynVel(senddata.f);
	}
	
	  
		if(senddata.Code>=0 && senddata.Code<4) //设置
		{
		SetDccVel(senddata.v);
}*///deleted by debugger

	if(!bSending)return;
	
	switch(senddata.Code){
	case 0: //G00 
		
		//判断G00直线长度
		if(IsLengZero(oldxx,senddata.x,oldyy,senddata.y,oldzz,senddata.z))break;

		//将G00指令添加到坐标系运动缓区
 		rtn=gtcard.LineXYZG0(senddata.x,senddata.y,senddata.z);
		
		//出错处理
		while(rtn==1)
		{			
			if (rtn==1 && !bAutoStarted) AxisGo(); //错误则重开缓冲区
			ProcessMessage();
			GeneralRun();
			
            //暂停处理
			if(bPause) PauseRun(); 
			while(bPause)
			{
				ProcessMessage();
				GeneralRun();
				if(!Paused) PauseRun();
				if(!bSending)return;
			}
		
			if(!bSending)return;
			
			//重新发送G00指令到缓冲区
			rtn=gtcard.LineXYZG0(senddata.x,senddata.y,senddata.z);
			
			if(rtn==-1)
			{
				ErrorMessage("控制卡通讯失败!");
				bSending=FALSE;
				break;
			}
		}
        
		//记录下终点位置 
		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;

		//发送成功，保存至BakGy
		if(rtn==0)
		{
			SendSeg++;
			GetMotionNum();
			BakGy.SaveLinePos(senddata.N);
		}
		break;  
	case 1: //G01
	
		//判断G00直线长度
		if(IsLengZero(oldxx,senddata.x,oldyy,senddata.y,oldzz,senddata.z))break;
		dl=sqrt((senddata.x-oldxx)*(senddata.x-oldxx)+(senddata.y-oldyy)*(senddata.y-oldyy)+(senddata.z-oldzz)*(senddata.z-oldzz));
		
         //将G00指令添加到坐标系运动缓区
		rtn=gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);

		//出错处理//
		while(rtn==1 ){
			if (rtn==1 && !bAutoStarted) AxisGo();
			ProcessMessage();
			GeneralRun();

			 //暂停处理
			if(bPause) PauseRun();
			while(bPause){
				ProcessMessage();
				GeneralRun();
				if(!Paused) PauseRun();
				if(!bSending)return;
			}
			if(!bSending)return;
			rtn=gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);		
		}
		
		//记录下终点位置
		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;

		//发送成功，保存至BakGy
		if(rtn==0)
		{
		SendSeg++;
		GetMotionNum();
		BakGy.SaveLinePos(senddata.N);
		}
		break;
	case 2: //G02      
		if(CrdPlan==18)//且为G18
		{			
			if(oldzz==senddata.z && oldxx==senddata.x) //如果起点、终点坐标相同
			{
				rtn=gtcard.ArcZX(senddata.rcx,senddata.rcy,-360); //将画整圆命令送入缓冲区
			}
			else
			{
				rtn=gtcard.ArcZXP(senddata.z,senddata.x,senddata.r,-1);//将画圆命令送入缓冲区
			}
            //出错处理//
			while(rtn==1 ){			
				if (rtn==1 && !bAutoStarted) AxisGo();
				ProcessMessage();
				GeneralRun();
				
				//暂停处理
				if(bPause) PauseRun();
				while(bPause){
					ProcessMessage();
					GeneralRun();
					if(!Paused) PauseRun();
					if(!bSending)return;
				}
				
				if(!bSending)return;
				if(oldzz==senddata.z && oldxx==senddata.x)
				{
					
					rtn=gtcard.ArcZX(senddata.rcx,senddata.rcy,-360);
				}
				else
					rtn=gtcard.ArcZXP(senddata.z,senddata.x,senddata.r,-1);
				
			}
		}
		else if(CrdPlan==19)
		{
			
			if(oldyy==senddata.y && oldzz==senddata.z)
			{
				rtn=gtcard.ArcYZ(senddata.rcx,senddata.rcy,-360);
			}
			else
				rtn=gtcard.ArcYZP(senddata.y,senddata.z,senddata.r,-1);
			while(rtn==1 ){
				
				if (rtn==1 && !bAutoStarted) AxisGo();
				ProcessMessage();
				GeneralRun();
				
				if(bPause) PauseRun();
				while(bPause){
					ProcessMessage();
					GeneralRun();
					if(!Paused) PauseRun();
					if(!bSending)return;
				}				
				if(!bSending)return;
				if(oldyy==senddata.y && oldzz==senddata.z)
				{
					rtn=gtcard.ArcYZ(senddata.rcx,senddata.rcy,-360);
				}
				else
					rtn=gtcard.ArcYZP(senddata.y,senddata.z,senddata.r,-1);				
			}
		}
		else //G17
		{
			if(oldxx==senddata.x && oldyy==senddata.y)
			{
				rtn=gtcard.ArcXY(senddata.rcx,senddata.rcy,-360);
			}
			else
				rtn=gtcard.ArcXYP(senddata.x,senddata.y,senddata.r,-1);

			while(rtn==1 ){
				if (rtn==1 && !bAutoStarted) AxisGo();
				ProcessMessage();
				GeneralRun();

				if(bPause) PauseRun();
				while(bPause){
					ProcessMessage();
					GeneralRun();
					if(!Paused) PauseRun();
					if(!bSending)return;
				}
				
				if(!bSending)return;
				
				if(oldxx==senddata.x && oldyy==senddata.y)
				{
					rtn=gtcard.ArcXY(senddata.rcx,senddata.rcy,-360);
				}
				else
					rtn=gtcard.ArcXYP(senddata.x,senddata.y,senddata.r,-1);		
			}
		}

		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;

		if(rtn==0)
		{
			SendSeg++;
			GetMotionNum();
			BakGy.SaveLinePos(senddata.N);
		}
		break;
	case 3://G03
			
		if(CrdPlan==18)
		{
			
			if(oldzz==senddata.z && oldxx==senddata.x)
			{
				rtn=gtcard.ArcZX(senddata.rcx,senddata.rcy,360);
			}
			else
			rtn=gtcard.ArcZXP(senddata.z,senddata.x,senddata.r,1);
			while(rtn==1)
			{				
			if (rtn==1 && !bAutoStarted) AxisGo();
			ProcessMessage();
			GeneralRun();
			if(bPause) PauseRun();
			while(bPause){
				ProcessMessage();
				GeneralRun();
				if(!Paused) PauseRun();
				if(!bSending)return;
			}
			if(!bSending)return;
			if(oldzz==senddata.z && oldxx==senddata.x)
			{
				rtn=gtcard.ArcZX(senddata.rcx,senddata.rcy,360);
			}
			else
			rtn=gtcard.ArcZXP(senddata.z,senddata.x,senddata.r,1);			
			}
		}
		else if(CrdPlan==19)
		{
			if(oldyy==senddata.y && oldzz==senddata.z)
			{
				rtn=gtcard.ArcYZ(senddata.rcx,senddata.rcy,360);
			}
			else
			rtn=gtcard.ArcYZP(senddata.y,senddata.z,senddata.r,1);
			while(rtn==1)
			{			
				if (rtn==1 && !bAutoStarted) AxisGo();
				ProcessMessage();
				GeneralRun();
				if(bPause) PauseRun();
				while(bPause){
					ProcessMessage();
					GeneralRun();
					if(!Paused) PauseRun();
					if(!bSending)return;
				}
			if(!bSending)return;
			if(oldyy==senddata.y && oldzz==senddata.z)
			{
				rtn=gtcard.ArcYZ(senddata.rcx,senddata.rcy,360);
			}
			else
			rtn=gtcard.ArcYZP(senddata.y,senddata.z,senddata.r,1);			
			}
		}
		else 
		{
			if(oldxx==senddata.x && oldyy==senddata.y)
			{
				rtn=gtcard.ArcXY(senddata.rcx,senddata.rcy,360);
			}
			else
			rtn=gtcard.ArcXYP(senddata.x,senddata.y,senddata.r,1);
			while(rtn==1)
			{		
			if (rtn==1 && !bAutoStarted) AxisGo();
			ProcessMessage();
			GeneralRun();
			if(bPause) PauseRun();
			while(bPause){
				ProcessMessage();
				GeneralRun();
				if(!Paused) PauseRun();
				if(!bSending)return;
			}
			if(!bSending)return;
			if(oldxx==senddata.x && oldyy==senddata.y)
			{
				rtn=gtcard.ArcXY(senddata.rcx,senddata.rcy,360);
			}
			else
			rtn=gtcard.ArcXYP(senddata.x,senddata.y,senddata.r,1);			
			}
		}
		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;
		if(rtn==0)
		{
		SendSeg++;
		GetMotionNum();
		BakGy.SaveLinePos(senddata.N);
		}
		break;
	case 4://G04
		if(WaitBufferEmpty())return ;
		Sleep((DWORD)senddata.x*1000);		
		break;
	case 8://M30或M02
			bSendEnd=true;	break;
	case 28: //G28
	    gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);
		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;
		break;
	case 92:	
		gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);
		//出错处理//
		while(rtn==1 ){
			if (rtn==1 && !bAutoStarted) AxisGo();
			ProcessMessage();
			GeneralRun();
			if(bPause) PauseRun();
			while(bPause){
				ProcessMessage();
				GeneralRun();
				if(!Paused) PauseRun();
				if(!bSending)return;
			}
			if(!bSending)return;
		gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);
		}
		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;
		if(rtn==0)
		{
			SendSeg++;
			GetMotionNum();
			BakGy.SaveLinePos(senddata.N);
		}
			break;
	case 100:  //M功能
		if(WaitBufferEmpty())return ;	
		HandleMCode((int)senddata.x);
		break;
		/*预留工件坐标系选择
	case 54:
		if(WaitBufferEmpty())return ;
		gtcard.SetCoord(m_coord.g54[0],m_coord.g54[1],m_coord.g54[2]);
		break;
	case 55:
		if(WaitBufferEmpty())return ;
		gtcard.SetCoord(m_coord.g55[0],m_coord.g55[1],m_coord.g55[2]);
		break;
	case 56:
		if(WaitBufferEmpty())return ;
		gtcard.SetCoord(m_coord.g56[0],m_coord.g56[1],m_coord.g56[2]);
		break;
	case 57:
		if(WaitBufferEmpty())return ;
		gtcard.SetCoord(m_coord.g57[0],m_coord.g57[1],m_coord.g57[2]);
		break;
	case 58:
		if(WaitBufferEmpty())return ;
		gtcard.SetCoord(m_coord.g58[0],m_coord.g58[1],m_coord.g58[2]);
		break;
	case 59:
		if(WaitBufferEmpty())return ;
		gtcard.SetCoord(m_coord.g59[0],m_coord.g59[1],m_coord.g59[2]);
		break;
		*///
	case 17:
	case 18:
	case 19:
		CrdPlan=(char)senddata.Code;
		break;
	default:
		break;
	}

}

/****************************************************************************/
/*  函数名: GeneralRun	      												*/
/*  功  能：G代码运行中状态处理												*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明： 已取消															*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::GeneralRun()
{
  	/*
	gtcard.ViewPos(); ////显示机床坐标和系统坐标
	OverrideVel();
	if(m_drawdata.LimitState!=0)
	{
		gtcard.pMainFrame->ModifyStatus(1,"限位开关触发!");
		bSending=FALSE;
	}
	
	if(m_drawdata.Error!=0)
	{
		gtcard.pMainFrame->ModifyStatus(1,"系统驱动器报警!");
		bSending=FALSE;
	}
	*/
}

/****************************************************************************/
/*  函数名: PauseRun	      												*/
/*  功  能：G代码运行中暂停													*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*			1.关闭缓冲区，各轴运动暂停										*/
/*			2.关闭缓冲区失败，消息框										*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::PauseRun()
{
	int i=0;
	char s[20]="";
	CString msgstr;
	if(gtcard.StopMotion()!=0)
	{
		ErrorMessage("运动停止出错!");
		return;
	}
	gtcard.EndList();
	Sleep(5);
	gtcard.EndList();
	gtcard.StopMotion(); 
	Sleep(5);
	gtcard.StopMotion();

	HookState=TRUE;
	Paused=TRUE;
}

/****************************************************************************/
/*  函数名: WaitBufferEmpty    												*/
/*  功  能：等待缓冲区空，然后再次打开缓冲区								*/
/*  参  数： 	无															*/
/*  出  口：缓冲区原有命令执行完，再次打开缓冲区							*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGCodeCtrlWnd::WaitBufferEmpty()
{
	BYTE i=0;
	char s[20]="";
	CString msgstr;

	//缓冲区非空
	if(!gtcard.IsBufferEmpty())
	{
		//启动缓冲区运行
		AxisGo();
		bSendEnd=TRUE;
	}

	//反复查询缓冲区状态
	while(!gtcard.IsBufferEmpty()){
		ProcessMessage();
		GeneralRun();
		GetMotionNum();

		//暂停处理
		if(bPause) 
			PauseRun();
		while(bPause){
			ProcessMessage();
			GeneralRun();
			if(!Paused) PauseRun();
			if(!bSending)return 0;
		}
		
		if(!bAutoStarted) //如果未启动运行
			AxisGo();  //则启动运行
		if(!bSending)return 0;	
	}
	
	Sleep(10);
	//缓冲区空后，关闭缓冲区
	if(gtcard.EndList()!=0) ErrorMessage("等待缓冲区空的过程中,结束缓冲区错误!");
	
	while(gtcard.IsCrdMove())
	{		ProcessMessage();
	GeneralRun();
	GetMotionNum();
	if(bPause) 
		PauseRun();
	while(bPause){
		ProcessMessage();
		GeneralRun();
		if(!Paused) PauseRun();
		if(!bSending)return 0;
	}
	
	if(!bAutoStarted)
		AxisGo();
	if(!bSending)return 0;	
	}
	
	//添加缓冲区命令
	if(gtcard.AddList()!=0) ErrorMessage("等待缓冲区空的过程中,重新开启缓冲区错误!");
	
	if(bAutoStarted) {NeedRestoreMtn=FALSE;bAutoStarted=FALSE;}

	bSendEnd=FALSE;
	return 0;
	
}

/****************************************************************************/
/*  函数名: HandleMCode    													*/
/*  功  能：处理G代码M指令													*/
/*  参  数：int code	M指令代码											*/
/*  出  口：																*/
/*  说  明：M03对应笔架下落，M05对应笔架抬起								*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGCodeCtrlWnd::HandleMCode(int code)//预留M功能
{

	short rtn;
	switch(code)
	{
	case 6:
		//gtcard.ClosePort(0); 
		ErrorMessage("请换刀!");
		bPause=TRUE;
		//gtcard.OpenPort(0);
		//Sleep(m_machPara.SpineDelay);
		break;
	case 3:rtn=gtcard.OpenPort(1,true);break;
	case 5:rtn=gtcard.OpenPort(1,false);break;
	case 10:rtn=gtcard.OpenPort(2,true);break;	
	case 20:rtn=gtcard.OpenPort(5,false);break; 
	case 11:rtn=gtcard.OpenPort(6,true);break;
	case 21:rtn=gtcard.OpenPort(6,false);break;
	case 12:rtn=gtcard.OpenPort(7,true);break;
	case 22:rtn=gtcard.OpenPort(7,false);break;
	case 13:rtn=gtcard.OpenPort(8,true);break;
	case 23:rtn=gtcard.OpenPort(8,false);break;
	case 14:rtn=gtcard.OpenPort(9,true);break;
	case 24:rtn=gtcard.OpenPort(9,false);break;
	case 15:rtn=gtcard.OpenPort(10,true);break;
	case 25:rtn=gtcard.OpenPort(10,false);break;
	case 16:rtn=gtcard.OpenPort(11,true);break;
	case 26:rtn=gtcard.OpenPort(11,false);break;
	case 17:rtn=gtcard.OpenPort(12,true);break;
	case 27:rtn=gtcard.OpenPort(12,false);break;
	case 18:rtn=gtcard.OpenPort(13,true);break;
	case 28:rtn=gtcard.OpenPort(13,false);break;
	case 19:rtn=gtcard.OpenPort(14,true);break;
	case 29:rtn=gtcard.OpenPort(14,false);break;
		
/*	case 40:rtn=ReadPortUntil(5);break;
		//case 50:gtcard.ClosePort(14);break;
	case 41:rtn=ReadPortUntil(6);break;
		//case 51:gtcard.ClosePort(14);break;
	case 42:rtn=ReadPortUntil(7);break;
		//case 52:gtcard.ClosePort(14);break;
	case 43:rtn=ReadPortUntil(8);break;
		//case 53:gtcard.ClosePort(14);break;
	case 44:rtn=ReadPortUntil(9);break;
		//case 54:gtcard.ClosePort(14);break;
	case 45:rtn=ReadPortUntil(10);break;
		//case 55:gtcard.ClosePort(14);break;
	case 46:rtn=ReadPortUntil(11);break;
		//case 56:gtcard.ClosePort(14);break;
	case 47:rtn=ReadPortUntil(12);break;
		//case 57:gtcard.ClosePort(14);break;
	case 48:rtn=ReadPortUntil(13);break;
		//case 58:gtcard.ClosePort(14);break;
	case 49:rtn=ReadPortUntil(14);break;
		//case 59:gtcard.ClosePort(14);break;*/
	}
	
	return rtn;	
}

/****************************************************************************/
/*  函数名: InitCard	      												*/
/*  功  能：板卡及轴初始化设置												*/
/*  参  数： 	无															*/
/*  出  口：																*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::InitCard()
{	
	int i;
	int nCtrlMode[4];
	int nDirPulse[4];
	short nKp[4],nKi[4],nKd[4];		   	
	gtcard.InitCard(); 	    	
	
	for(i = 0; i < 4; i ++)
	{
		if(((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i] == SERVO_MOTOR) //如果轴为伺服电机
		{
			nCtrlMode[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i];
			nKp[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKp[i];
			nKi[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKi[i];
			nKd[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKd[i];
		}
		
		else if(((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i] == STEP_MOTOR)//如果轴为步进电机
		{
			nCtrlMode[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i];
			nDirPulse[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nDirPls[i];
		}
	}	
	
	for(i = 0; i < 4; i ++)
	{
		gtcard.AxisInitial(nCtrlMode[i],nDirPulse[i],nKp[i],nKi[i],nKd[i],i+1);
	}
	m_bIsCardInit=true;
}

void CGCodeCtrlWnd::OnBtnSaveNccode() 
{
	CStdioFile fs;//NC文件
	CString strLine;
	CString strNCFileName;//NC文件名		
	CEdit * pGCodeEdit;

	//判断编辑区内是否有需保存内容
	pGCodeEdit=(CEdit *) GetDlgItem(IDC_EDIT_GCODE);
	if(pGCodeEdit->GetLineCount()<2) {
		AfxMessageBox("您尚未在NC代码编辑区中输入有效代码！",MB_OK);
		return;
	}

	char filters[] = "NC file(*.txt)|*.txt|";
	CFileDialog fileDlg(FALSE,"txt","*.txt",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filters,NULL);
	int result = fileDlg.DoModal();	
	if(result==IDOK)
		strNCFileName = fileDlg.GetPathName();
	else
		return;
	//文件保存
	if(fs.Open(strNCFileName, CFile::modeCreate |CFile::modeWrite)==NULL) 
	{
		AfxMessageBox("文件操作出错！",MB_OK);
	}
	
	int nLineCount=pGCodeEdit->GetLineCount();
	for(int i=0;i<nLineCount-1;i++)
	{	
		CString strBuffer;
		int nLength=pGCodeEdit->LineLength(pGCodeEdit->LineIndex(i));
		pGCodeEdit->GetLine(i,strBuffer.GetBuffer(nLength));
		strBuffer.ReleaseBuffer();
		strLine.Format("%s\n",strBuffer);
		fs.WriteString(strLine);		
	}
	fs.Close();	
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(strNCFileName);
	bShouldSave=false;
}

void CGCodeCtrlWnd::OnBtnCoordEffect() 
{
	short rtn;
	if (gtcard.IsCrdMove())
	{
		MessageBox("轴运动中不能设置坐标映射！","警告",MB_ICONEXCLAMATION);
		return;
	}

	xMap=m_cmbAxisX.GetCurSel()+1;
	yMap=m_cmbAxisY.GetCurSel()+1;
	zMap=m_cmbAxisZ.GetCurSel()+1;
	
	//轴映射错误判断
	bool bMapIsWrong;
	int nViewPlane;

	nViewPlane=((CMotorControlBenchApp *)AfxGetApp())->nViewPlnSel;
	switch(m_nAxisCount) {
	case 1:
		break;
	case 2:
		switch(nViewPlane) {
		case XY_PLANE:
			bMapIsWrong=(xMap==yMap);
			break;
		case XZ_PLANE:
			bMapIsWrong=(xMap==zMap);
			break;
		case YZ_PLANE:
			bMapIsWrong=(yMap==zMap);
			break;
		}        
		break;
	case 3:		
	case 4:
		bMapIsWrong=(xMap==yMap)||(xMap==zMap)||(yMap==zMap);
		break;
	}
	if(bMapIsWrong)
	{
		MessageBox("请选择不同轴号！","轴映射出错",MB_ICONERROR);
		return;
	}

  //开启轴伺服//
	for(int i = 0; i < m_nAxisCount; i ++)
	{
		rtn=GT_Axis(i+1);		
		rtn=GT_AxisOn();
		if(rtn){
			MessageBox("轴开启错误！","出错",MB_ICONERROR);
			return;
		}
	}
	m_bIsAxisOn=true;

	
	//设置轴映射关系//
	switch(m_nAxisCount) {
	case 1:
		break;
	case 2:
		switch(nViewPlane){
		case XY_PLANE:	
			zMap=3;
			gtcard.MapAxis(xMap,yMap,zMap,2,XY_PLANE);
			break;
		case YZ_PLANE:
			xMap=3;
			gtcard.MapAxis(xMap,yMap,zMap,2,YZ_PLANE);
			break;
		case XZ_PLANE:	
			yMap=3;
			gtcard.MapAxis(xMap,yMap,zMap,2,XZ_PLANE);
			break;
		}
		break;
		case 3:
		case 4:	
			gtcard.MapAxis(xMap,yMap,zMap,3,0);
			break;
	}

	gtcard.pDrawView->m_xMap=xMap-1;
	gtcard.pDrawView->m_yMap=yMap-1;
	gtcard.pDrawView->m_zMap=zMap-1;
	
	GetDlgItem(IDC_BTN_LOAD_NCCODE)->EnableWindow(TRUE);
	gtcard.pDoc->bIsAxisMapped=true;
	bIsAxisMapped=true;
	m_bIsCardInit=true;

	int m_timer=SetTimer(1,100,NULL);
}

void CGCodeCtrlWnd::UpdateLed(UINT nID, double dispValue)
{
	CDigitalDisp * pLed=(CDigitalDisp *) GetDlgItem(nID);
   	pLed->SetNumToDisp(dispValue);
}

void CGCodeCtrlWnd::MotionPause()
{
	if(!bAutoCuting) return;
	bPause=true;
	Paused=FALSE;
}

void CGCodeCtrlWnd::OnTimer(UINT nIDEvent) 
{
	unsigned short line;
	double actlPos[4]; 	

	UpdateData(FALSE); 

	if (!bStop){
	GT_GetMtnNm(&line);
	m_ctrlNCList.SetCurSel(line-1);
	}	
	
	gtcard.GetEncPos(actlPos);
	UpdateLed(IDC_AXIS1_POSITION,actlPos[xMap-1]);
	UpdateLed(IDC_AXIS2_POSITION,actlPos[yMap-1]);
	UpdateLed(IDC_AXIS3_POSITION,actlPos[zMap-1]);

	CFormView::OnTimer(nIDEvent);
}


void CGCodeCtrlWnd::OnChangeEditGcode() 
{
	bShouldSave=true;	
}

bool CGCodeCtrlWnd::ModifySynVel(double vel)
{
	
	short rtn;
	rtn=gtcard.SetSynVel(vel);
	return true;
}

/****************************************************************************/
/*  函数名: CheckG92	      												*/
/*  功  能：检查G代码开始运行时各轴是否处在对刀位置							*/
/*  参  数： 	无															*/
/*  出  口：处于对刀位置=T ，不处于对刀位置=F								*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGCodeCtrlWnd::CheckG92()
{
	double pt[4];
	double tol;
	CString strTemp;
	CCNCData tempNCData;

	tol=0.1; //对刀误差
	
	for(int i=0;i<3;i++)
	{
		tempNCData=glcncarray.operator [](i);
		if (tempNCData.Code==92)
		{
			gtcard.GetEncPos(pt);
			if ((fabs(pt[xMap-1]-tempNCData.x)>tol)||(fabs(pt[yMap-1]-tempNCData.y)>tol)||(fabs(pt[zMap-1]-tempNCData.z)>tol)) 
			{
				strTemp.Format("您未将刀具定位到G92指令指定的对刀点处！\n\n请先手动对刀至以下位置：\nX:%3.2fmm\nY:%3.2fmm\nZ:%3.2fmm",tempNCData.x,tempNCData.y,tempNCData.z);
				MessageBox(strTemp,"XX科技",MB_OK|MB_ICONWARNING);
				return true;
			}
			return false;
		}	
		else continue;	
	}
	return false;
}

void CGCodeCtrlWnd::OnBtnControlXhome() 
{
	int nAxis;
	int Pln;

	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	Pln=pApp->nViewPlnSel;	

	if (!m_bIsCardInit) {
		MessageBox("板卡已被复位，\n请先点击“坐标映射生效”按钮 ","XX科技",MB_OK|MB_ICONINFORMATION);
		return;	
	}
	if (gtcard.IsCardMove()) {
		MessageBox("请先停止轴运动再回零","XX科技",MB_OK|MB_ICONWARNING);
		return;
	}	

	switch(Pln) {
	case XY_PLANE:
		nAxis=xMap;		
			break;
	case XZ_PLANE:
		nAxis=xMap;	
		break;
	case YZ_PLANE:
		nAxis=yMap;	
		break;
	}
	
	gtcard.pDrawView->DrawWorkLine(1);
	gtcard.AxisTMotion(nAxis,0,5,0.1);
	m_bXHomed=true;	
}

void CGCodeCtrlWnd::OnBtnControlYhome() 
{
	int nAxis;
	int Pln;
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	Pln=pApp->nViewPlnSel;	
    
	if (!m_bIsCardInit) {
		MessageBox("板卡已被复位，\n请先点击“坐标映射生效”按钮 ","XX科技",MB_OK|MB_ICONINFORMATION);
		return;	
	}
	if (gtcard.IsCardMove()) {
		MessageBox("请先停止轴运动再回零","XX科技",MB_OK|MB_ICONWARNING);
		return;
	}
	
	switch(Pln) {
	case XY_PLANE:
		nAxis=yMap;		
		break;
	case XZ_PLANE:
		nAxis=zMap;	
		break;
	case YZ_PLANE:
		nAxis=zMap;	
		break;
	}
	gtcard.pDrawView->DrawWorkLine(1);
	gtcard.AxisTMotion(nAxis,0,5,0.1);
	m_bYHomed=true;	
}

/****************************************************************************/
/*  函数名: CheckIsZero	      												*/
/*  功  能：检查当前点是否在原点											*/
/*  参  数： 	无															*/
/*  出  口：  当前位置在(0,0,0)=T；当前位置不在(0,0,0)=F					*/
/*  说  明：																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGCodeCtrlWnd::CheckIsZero()
{
	double atlPos[4];
		
	gtcard.GetEncPos(atlPos);
	for(int i=0;i<4;i++)
	{
		if (fabs(atlPos[i]-0.00)>0.1) return false;		
	}
	return true;
}





















