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
		AfxMessageBox("��G�����ļ�����",MB_OK);
		return;
	}
    
	if (fs.GetLength()==NULL) 
	{
		AfxMessageBox("ָ�����ļ�����Ϊ�գ�",MB_OK);
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
		AfxMessageBox("��ǰû���κ�G����ɱ��룡\n ��򿪻��½�һ��G�����ļ�",MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(gtcard.IsCrdMove())
	{
		AfxMessageBox("��ǰ��G��������ִ�У�",MB_OK|MB_ICONWARNING);
		return;
	}
	if(bShouldSave&&!bIsSaved)
	{
		if(IDNO==AfxMessageBox("���޸���G�����ļ������ȱ��棡\n���YES�������޸�ǰ���ļ������NOȡ���˴β���",MB_YESNO|MB_ICONINFORMATION))
		{
			bShouldSave=false;
			return;
		}	
	}

	((CStatic *) GetDlgItem(IDC_STATIC_FILENAME))->GetWindowText(strNCFileName);
	
	if(strNCFileName=="")
	{
		AfxMessageBox("����δָ�������G�����ļ���",MB_OK|MB_ICONWARNING);
		return;
	}	
	
	//����G����
	char i=Compile(strNCFileName,&glcncarray,m_dOptiAngle,MaxCoord,true); 

	//������
	if(i){ 
		FreeMem();
		glcncarray.RemoveAll();
		switch(i)
		{
		case 1:ErrorMessage("�ڴ�ָ�����!");break;
		case 4:ErrorMessage("��ȡ�ļ�����!");break;
		case 3:ErrorMessage("�ļ�ȱ��M30��M02���ȡ�ļ�����!");break;
		case 11:ErrorMessage("�ļ�̫��,�뽫�ļ��ȴ�����ټӹ�!");break;
		default:
			ErrorMessage("��ȡ�ļ�ʱ����δ֪����!");break;
		}
		return;
	}	 
    
	CncLength=glcncarray.GetSize();


	int nG0Line(0);	
	CCNCData tempNCData;	
	for(int iLine=0;iLine<CncLength;iLine++)
	{
		tempNCData=glcncarray.operator [](iLine);
	/*������
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
/*  ������: FreeMem		      												*/
/*  ��  �ܣ����NC��������													*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�  glcncarray �����												*/
/*  ˵  ����																*/ 
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
	MessageBox(str,"ϵͳ����",MB_OK | MB_ICONSTOP);

}

void CGCodeCtrlWnd::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_tabViewSel.InsertItem(0,"ѡ����ʾƽ��");
	m_tabViewSel.InsertItem(1,"���Ų���");
	m_tabViewSel.InsertItem(2,"�㶯����");
	m_tabViewSel.InitTabPage();

	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();	
	int nAxisCount=pApp->m_nAxisCount;    
	switch(nAxisCount) {
	case 1:
		break;
	case 2:
		m_cmbAxisX.ResetContent();
        m_cmbAxisX.InsertString(0,"1����");
		m_cmbAxisX.InsertString(1,"2����");
		m_cmbAxisY.ResetContent();
        m_cmbAxisY.InsertString(0,"1����");
		m_cmbAxisY.InsertString(1,"2����");
		m_cmbAxisZ.ResetContent();
        m_cmbAxisZ.InsertString(0,"1����");
		m_cmbAxisZ.InsertString(1,"2����");
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
	GetDlgItem(IDC_BTN_CONTROL_RUN)->EnableWindow(FALSE);//����
	GetDlgItem(IDC_BTN_CONTROL_PAUSE)->EnableWindow(FALSE);//��ͣ
    GetDlgItem(IDC_BTN_CONTROL_STOP)->EnableWindow(FALSE);//ֹͣ
	GetDlgItem(IDC_BTN_CLOSEAXIS)->EnableWindow(FALSE);//�ر���
	GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);//��λ
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
		AfxMessageBox("�忨�ѱ���λ������������ӳ����Ч����ť��������",MB_OK|MB_ICONWARNING);
		return;
	}
	if (!bIsAxisMapped) {
		AfxMessageBox("��δȷ����ӳ���ϵ������������ӳ����Ч����ť��������",MB_OK|MB_ICONWARNING);
		return;
	}
	if (!m_bIsAxisOn) {
		AfxMessageBox("����δ�ŷ��ϵ磬����������ӳ����Ч����ť��������",MB_OK|MB_ICONWARNING);
		return;
	}	
	
	if(!bFileLoaded)
	{
		AfxMessageBox("��δ����G���룬���������롱��ť��������",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(!bStop)//�����ǰG����δ�������
	{
		AfxMessageBox("��ǰG������δ������ϣ����ȵ����ֹͣ����ťֹͣ��ǰ�������У�",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(gtcard.IsCrdMove())
	{
		AfxMessageBox("���˶��У�",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if (CheckG92()) return;  //δ�Ե���G92ָ��ָ��λ�ã��򷵻�
	
	if (CheckIsZero()==false) {
		int res;
		res=MessageBox("������ʼλ�ò��ڱ��ԭ�㣬�Ƿ����������","XX�Ƽ�",MB_YESNO|MB_ICONQUESTION);
		if (IDNO==res) return;
	}

	OnBtnCoordEffect(); 
	
	SendLine=0;				//���͵�����
	if(!gtcard.InitBuffer())//��ʼ��������  
	{EndSent();	return;}	//��������ʼ��ʧ�ܣ�ֹͣ���ݷ��ͣ�������
	
	m_StartLine=0;			//�����κ�λ��

	MotionStarted=FALSE;	//�Ƿ�����������ִ��

	bSending=TRUE;			//�Ƿ������򻺳�����������

	bPause=FALSE;			//�Ƿ�����ͣ��

	bSendEnd=FALSE;			//�����Ƿ����

	HookState=FALSE;		//�Ƿ�ֹͣ���̹���

	bAutoCuting=TRUE;		//�Ƿ����Զ�������

	BakGy.Clear();			//�������������

	gtcard.GetPrflPnt(pnt);	//ȷ���ᵱǰ��λ�� 
	oldxx=pnt[0];	oldyy=pnt[1];	oldzz=pnt[2];

	CurrentCutNo=0;			//��¼��ǰG������ͼ����,�����ظ�ˢ��

	SendSeg=0;				//��¼�򻺳����ѷ��͵�ָ������

	NeedRestoreMtn=FALSE;	//�ǻָ����л�����������

	bAutoStarted=FALSE;		//�������ƿ����� T=������  F=δ����

	Paused=TRUE;			//��ʼϵͳΪ��ͣ״̬

	bStop=FALSE;			//��ʼʱ��ɱ�־ΪFALSE
	
	CString strNCFileName,strLine;
	CStdioFile fs;
	((CStatic *) GetDlgItem(IDC_STATIC_FILENAME))->GetWindowText(strNCFileName);
	if(strNCFileName=="")
	{
		AfxMessageBox("����δָ�������G�����ļ���",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(fs.Open(strNCFileName, CFile::modeRead | CFile::typeText)==NULL) 
	{
		AfxMessageBox("����G�����ļ�����",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if (fs.GetLength()==NULL) 
	{
		AfxMessageBox("�ļ�����Ϊ�գ�",MB_OK|MB_ICONWARNING);
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
		MessageBox("�忨�ѱ���λ��\n����������ӳ����Ч����������","XX�Ƽ�",MB_OK|MB_ICONINFORMATION);
		return;	
	}
	if (bStop) {
		MessageBox("G�����ļ�ִ���Ѿ�ֹͣ��\n���������С���������","XX�Ƽ�",MB_OK|MB_ICONINFORMATION);
		return;
	}
	
	if(!bPause)
	{
		MotionPause();
		PauseRun();
		GetDlgItem(IDC_BTN_CONTROL_PAUSE)->SetWindowText("����");
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
		GetDlgItem(IDC_BTN_CONTROL_PAUSE)->SetWindowText("��ͣ");
		return;
	}	
}

void CGCodeCtrlWnd::OnBtnControlStop() 
{
	CString str;
	if (!m_bIsCardInit) {
		MessageBox("�忨�ѱ���λ��\n���ȵ��������ӳ����Ч����ť ","XX�Ƽ�",MB_OK|MB_ICONINFORMATION);
		return;	
	}
	/**********************/
	//���ֹͣ���˳���������//
	//2006.12.01//
	gtcard.EndList();
	FreeMem();
	glcncarray.RemoveAll();
	/**********************/
	if (!gtcard.StopMotion()) {
		
      bStop=true;
	  str="�����˶�ֹͣ��";
	  GetDlgItem(IDC_BTN_CONTROL_PAUSE)->SetWindowText("��ͣ");
	  bPause=false;
	}
	else 
		str="�˶�ֹͣʧ�ܣ�";

	MessageBox(str,"XX�Ƽ�",MB_OK|MB_ICONINFORMATION);	
}

void CGCodeCtrlWnd::OnBtnCloseaxis() 
{
   short rtn;
   if (!m_bIsCardInit) {
	   MessageBox("�忨�ѱ���λ0��\n����������ӳ����Ч����������","XX�Ƽ�",MB_OK|MB_ICONINFORMATION);
	   return;	
   }
	for(int i=1;i<=m_nAxisCount;i++)
	{
		rtn=GT_Axis(i);
		rtn=GT_AxisOff();
	}    
	GetDlgItem(IDC_BTN_CONTROL_PAUSE)->SetWindowText("ֹͣ");
	bPause=false;
	m_bIsAxisOn=false;
	bStop=true;
	MessageBox("�����ŷ��ѹرգ�","XX�Ƽ�",MB_OK|MB_ICONINFORMATION);
}

void CGCodeCtrlWnd::OnBtnReset() 
{
	if (gtcard.IsCardMove()) {
		MessageBox("����ֹͣ���˶����ٽ��а忨��λ����","XX�Ƽ�");
		return;
	}
	InitCard();    
	m_bIsCardInit=false;
	bStop=true;
	MessageBox("�˶����ƿ��Ѹ�λ��","XX�Ƽ�",MB_OK|MB_ICONINFORMATION);
	

}

/****************************************************************************/
/*  ������: EndSent		      												*/
/*  ��  �ܣ�NC���뷢���괦��												*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::EndSent()
{
	StartUserSet=FALSE;	
	if(gtcard.EndList()!=0) 
	{
		MessageBox("�رջ�����ʧ�ܣ�","XX�Ƽ�",MB_OK|MB_ICONWARNING);
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
/*  ������: AxisGo		      												*/
/*  ��  �ܣ���������ϵ�˶�������������										*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�  ��������ɹ�=T�����ɹ�=F										*/
/*  ˵  ����																*/ 
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
		rtn=gtcard.StartMotion();//�������� 		
	}
	if(rtn==0)
	{
		bAutoStarted=TRUE;//���ƿ�����
		NeedRestoreMtn=TRUE;
	}
	return  (bool) rtn;
}

/****************************************************************************/
/*  ������: LoopSend	      												*/
/*  ��  �ܣ��򻺳�����ѭ������CNCָ��										*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::LoopSend()
{
	while(!(bSendEnd&&gtcard.IsBufferEmpty())) //������ݷ������һ������Ѿ�Ϊ�գ���ִ��ѭ��
	{
		ProcessMessage(); //������Ϣ����
		if(bSending&&!bPause) //������ڷ���������δ������ͣ
		{
			if(!bSendEnd&&(SendLine<CncLength))//�������ֹͣδ��λ������δ���꣬��
			{
				EnterCriticalSection(&s_cs);
				senddata=glcncarray.operator [](SendLine++); //��ȡ��Ҫ���͵�CNCָ��
				SentCnc(); //��������
				LeaveCriticalSection(&s_cs);
			}
			else 
			{
				bSendEnd=TRUE;//����ֹͣ��־��λ
				if(!bAutoStarted) AxisGo();
			}
		}
		if(StartUserSet) //����û���������ʼ��
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
		if(bPause) //�����������ͣ
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
/*  ������: LoopSend	      												*/
/*  ��  �ܣ��򻺳�������һ��CNCָ��											*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::SentCnc()
{
	short rtn=0;
	double dl=0;//dl is �ռ��߶γ���  
	bool bCmdIn=false;//���������˶�ָ��
	double dwLineLen=0;//�߶γ���

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
	
	//�ٶȸ���
	/*
	if(!m_sysPara.useF)//���ʹ��ϵͳ���õ����ʣ���
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
	
	  
		if(senddata.Code>=0 && senddata.Code<4) //����
		{
		SetDccVel(senddata.v);
}*///deleted by debugger

	if(!bSending)return;
	
	switch(senddata.Code){
	case 0: //G00 
		
		//�ж�G00ֱ�߳���
		if(IsLengZero(oldxx,senddata.x,oldyy,senddata.y,oldzz,senddata.z))break;

		//��G00ָ����ӵ�����ϵ�˶�����
 		rtn=gtcard.LineXYZG0(senddata.x,senddata.y,senddata.z);
		
		//������
		while(rtn==1)
		{			
			if (rtn==1 && !bAutoStarted) AxisGo(); //�������ؿ�������
			ProcessMessage();
			GeneralRun();
			
            //��ͣ����
			if(bPause) PauseRun(); 
			while(bPause)
			{
				ProcessMessage();
				GeneralRun();
				if(!Paused) PauseRun();
				if(!bSending)return;
			}
		
			if(!bSending)return;
			
			//���·���G00ָ�������
			rtn=gtcard.LineXYZG0(senddata.x,senddata.y,senddata.z);
			
			if(rtn==-1)
			{
				ErrorMessage("���ƿ�ͨѶʧ��!");
				bSending=FALSE;
				break;
			}
		}
        
		//��¼���յ�λ�� 
		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;

		//���ͳɹ���������BakGy
		if(rtn==0)
		{
			SendSeg++;
			GetMotionNum();
			BakGy.SaveLinePos(senddata.N);
		}
		break;  
	case 1: //G01
	
		//�ж�G00ֱ�߳���
		if(IsLengZero(oldxx,senddata.x,oldyy,senddata.y,oldzz,senddata.z))break;
		dl=sqrt((senddata.x-oldxx)*(senddata.x-oldxx)+(senddata.y-oldyy)*(senddata.y-oldyy)+(senddata.z-oldzz)*(senddata.z-oldzz));
		
         //��G00ָ����ӵ�����ϵ�˶�����
		rtn=gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);

		//������//
		while(rtn==1 ){
			if (rtn==1 && !bAutoStarted) AxisGo();
			ProcessMessage();
			GeneralRun();

			 //��ͣ����
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
		
		//��¼���յ�λ��
		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;

		//���ͳɹ���������BakGy
		if(rtn==0)
		{
		SendSeg++;
		GetMotionNum();
		BakGy.SaveLinePos(senddata.N);
		}
		break;
	case 2: //G02      
		if(CrdPlan==18)//��ΪG18
		{			
			if(oldzz==senddata.z && oldxx==senddata.x) //�����㡢�յ�������ͬ
			{
				rtn=gtcard.ArcZX(senddata.rcx,senddata.rcy,-360); //������Բ�������뻺����
			}
			else
			{
				rtn=gtcard.ArcZXP(senddata.z,senddata.x,senddata.r,-1);//����Բ�������뻺����
			}
            //������//
			while(rtn==1 ){			
				if (rtn==1 && !bAutoStarted) AxisGo();
				ProcessMessage();
				GeneralRun();
				
				//��ͣ����
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
	case 8://M30��M02
			bSendEnd=true;	break;
	case 28: //G28
	    gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);
		oldxx=senddata.x;oldyy=senddata.y;oldzz=senddata.z;
		break;
	case 92:	
		gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);
		//������//
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
	case 100:  //M����
		if(WaitBufferEmpty())return ;	
		HandleMCode((int)senddata.x);
		break;
		/*Ԥ����������ϵѡ��
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
/*  ������: GeneralRun	      												*/
/*  ��  �ܣ�G����������״̬����												*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*  ˵  ���� ��ȡ��															*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CGCodeCtrlWnd::GeneralRun()
{
  	/*
	gtcard.ViewPos(); ////��ʾ���������ϵͳ����
	OverrideVel();
	if(m_drawdata.LimitState!=0)
	{
		gtcard.pMainFrame->ModifyStatus(1,"��λ���ش���!");
		bSending=FALSE;
	}
	
	if(m_drawdata.Error!=0)
	{
		gtcard.pMainFrame->ModifyStatus(1,"ϵͳ����������!");
		bSending=FALSE;
	}
	*/
}

/****************************************************************************/
/*  ������: PauseRun	      												*/
/*  ��  �ܣ�G������������ͣ													*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*			1.�رջ������������˶���ͣ										*/
/*			2.�رջ�����ʧ�ܣ���Ϣ��										*/
/*  ˵  ����																*/ 
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
		ErrorMessage("�˶�ֹͣ����!");
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
/*  ������: WaitBufferEmpty    												*/
/*  ��  �ܣ��ȴ��������գ�Ȼ���ٴδ򿪻�����								*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�������ԭ������ִ���꣬�ٴδ򿪻�����							*/
/*  ˵  ����																*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGCodeCtrlWnd::WaitBufferEmpty()
{
	BYTE i=0;
	char s[20]="";
	CString msgstr;

	//�������ǿ�
	if(!gtcard.IsBufferEmpty())
	{
		//��������������
		AxisGo();
		bSendEnd=TRUE;
	}

	//������ѯ������״̬
	while(!gtcard.IsBufferEmpty()){
		ProcessMessage();
		GeneralRun();
		GetMotionNum();

		//��ͣ����
		if(bPause) 
			PauseRun();
		while(bPause){
			ProcessMessage();
			GeneralRun();
			if(!Paused) PauseRun();
			if(!bSending)return 0;
		}
		
		if(!bAutoStarted) //���δ��������
			AxisGo();  //����������
		if(!bSending)return 0;	
	}
	
	Sleep(10);
	//�������պ󣬹رջ�����
	if(gtcard.EndList()!=0) ErrorMessage("�ȴ��������յĹ�����,��������������!");
	
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
	
	//��ӻ���������
	if(gtcard.AddList()!=0) ErrorMessage("�ȴ��������յĹ�����,���¿�������������!");
	
	if(bAutoStarted) {NeedRestoreMtn=FALSE;bAutoStarted=FALSE;}

	bSendEnd=FALSE;
	return 0;
	
}

/****************************************************************************/
/*  ������: HandleMCode    													*/
/*  ��  �ܣ�����G����Mָ��													*/
/*  ��  ����int code	Mָ�����											*/
/*  ��  �ڣ�																*/
/*  ˵  ����M03��Ӧ�ʼ����䣬M05��Ӧ�ʼ�̧��								*/ 
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
bool CGCodeCtrlWnd::HandleMCode(int code)//Ԥ��M����
{

	short rtn;
	switch(code)
	{
	case 6:
		//gtcard.ClosePort(0); 
		ErrorMessage("�뻻��!");
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
/*  ������: InitCard	      												*/
/*  ��  �ܣ��忨�����ʼ������												*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/ 
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
		if(((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i] == SERVO_MOTOR) //�����Ϊ�ŷ����
		{
			nCtrlMode[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i];
			nKp[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKp[i];
			nKi[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKi[i];
			nKd[i] = ((CMotorControlBenchApp*)AfxGetApp())->m_nKd[i];
		}
		
		else if(((CMotorControlBenchApp*)AfxGetApp())->m_nCtrlMode[i] == STEP_MOTOR)//�����Ϊ�������
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
	CStdioFile fs;//NC�ļ�
	CString strLine;
	CString strNCFileName;//NC�ļ���		
	CEdit * pGCodeEdit;

	//�жϱ༭�����Ƿ����豣������
	pGCodeEdit=(CEdit *) GetDlgItem(IDC_EDIT_GCODE);
	if(pGCodeEdit->GetLineCount()<2) {
		AfxMessageBox("����δ��NC����༭����������Ч���룡",MB_OK);
		return;
	}

	char filters[] = "NC file(*.txt)|*.txt|";
	CFileDialog fileDlg(FALSE,"txt","*.txt",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filters,NULL);
	int result = fileDlg.DoModal();	
	if(result==IDOK)
		strNCFileName = fileDlg.GetPathName();
	else
		return;
	//�ļ�����
	if(fs.Open(strNCFileName, CFile::modeCreate |CFile::modeWrite)==NULL) 
	{
		AfxMessageBox("�ļ���������",MB_OK);
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
		MessageBox("���˶��в�����������ӳ�䣡","����",MB_ICONEXCLAMATION);
		return;
	}

	xMap=m_cmbAxisX.GetCurSel()+1;
	yMap=m_cmbAxisY.GetCurSel()+1;
	zMap=m_cmbAxisZ.GetCurSel()+1;
	
	//��ӳ������ж�
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
		MessageBox("��ѡ��ͬ��ţ�","��ӳ�����",MB_ICONERROR);
		return;
	}

  //�������ŷ�//
	for(int i = 0; i < m_nAxisCount; i ++)
	{
		rtn=GT_Axis(i+1);		
		rtn=GT_AxisOn();
		if(rtn){
			MessageBox("�Ὺ������","����",MB_ICONERROR);
			return;
		}
	}
	m_bIsAxisOn=true;

	
	//������ӳ���ϵ//
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
/*  ������: CheckG92	      												*/
/*  ��  �ܣ����G���뿪ʼ����ʱ�����Ƿ��ڶԵ�λ��							*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ����ڶԵ�λ��=T �������ڶԵ�λ��=F								*/
/*  ˵  ����																*/ 
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

	tol=0.1; //�Ե����
	
	for(int i=0;i<3;i++)
	{
		tempNCData=glcncarray.operator [](i);
		if (tempNCData.Code==92)
		{
			gtcard.GetEncPos(pt);
			if ((fabs(pt[xMap-1]-tempNCData.x)>tol)||(fabs(pt[yMap-1]-tempNCData.y)>tol)||(fabs(pt[zMap-1]-tempNCData.z)>tol)) 
			{
				strTemp.Format("��δ�����߶�λ��G92ָ��ָ���ĶԵ��㴦��\n\n�����ֶ��Ե�������λ�ã�\nX:%3.2fmm\nY:%3.2fmm\nZ:%3.2fmm",tempNCData.x,tempNCData.y,tempNCData.z);
				MessageBox(strTemp,"XX�Ƽ�",MB_OK|MB_ICONWARNING);
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
		MessageBox("�忨�ѱ���λ��\n���ȵ��������ӳ����Ч����ť ","XX�Ƽ�",MB_OK|MB_ICONINFORMATION);
		return;	
	}
	if (gtcard.IsCardMove()) {
		MessageBox("����ֹͣ���˶��ٻ���","XX�Ƽ�",MB_OK|MB_ICONWARNING);
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
		MessageBox("�忨�ѱ���λ��\n���ȵ��������ӳ����Ч����ť ","XX�Ƽ�",MB_OK|MB_ICONINFORMATION);
		return;	
	}
	if (gtcard.IsCardMove()) {
		MessageBox("����ֹͣ���˶��ٻ���","XX�Ƽ�",MB_OK|MB_ICONWARNING);
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
/*  ������: CheckIsZero	      												*/
/*  ��  �ܣ���鵱ǰ���Ƿ���ԭ��											*/
/*  ��  ���� 	��															*/
/*  ��  �ڣ�  ��ǰλ����(0,0,0)=T����ǰλ�ò���(0,0,0)=F					*/
/*  ˵  ����																*/ 
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





















