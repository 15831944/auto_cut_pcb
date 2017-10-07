// PageBasePara.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageBasePara.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageBasePara dialog


CPageBasePara::CPageBasePara(CWnd* pParent /*=NULL*/)
: CDialog(CPageBasePara::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageBasePara)
	m_nMotorType = 1;
	m_nStepMode = 0;
	//}}AFX_DATA_INIT
	m_nCardType=1;
	m_bIsFirstRun=TRUE;
	//  m_nCtrlMode=new int[4];
	// m_nDirPls=new int[4];
	m_nKP=new short[5];
	for(int n=0;n<5;n++)
	{
		m_nKP[n]=1;
	}

	m_nKI=new short[5];
	m_nKD=new short[5];
	m_fG00Acc=new double;
	m_fG01Acc=new double;
//	m_nHomeStyle=new int[4];
	m_fHomeVel=new double[4];
	m_fHomeAcc=new double[4];
}


void CPageBasePara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageBasePara)
	DDX_Radio(pDX, IDC_RADIO_STEP, m_nMotorType);
	DDX_Radio(pDX, IDC_RADIO_DIR, m_nStepMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageBasePara, CDialog)
	//{{AFX_MSG_MAP(CPageBasePara)
	ON_CBN_SELCHANGE(IDC_CMB_AXISCOUNT, OnSelchangeCmbAxiscount)
	ON_CBN_SELCHANGE(IDC_CMB_AXISNO_PARSETPAGE, OnSelchangeCmbAxisnoParsetpage)
	ON_BN_CLICKED(IDC_RADIO_STEP, OnRadioStep)
	ON_BN_CLICKED(IDC_RADIO_SERVO, OnRadioServo)
	ON_BN_CLICKED(IDC_RADIO_DIR, OnRadioDir)
	ON_BN_CLICKED(IDC_RADIO_PULSE, OnRadioPulse)
	ON_EN_UPDATE(IDC_EDIT_KP_PARASET, OnUpdateEditKpParaset)
	ON_EN_UPDATE(IDC_EDIT_KI_PARASET, OnUpdateEditKiParaset)
	ON_EN_UPDATE(IDC_EDIT_KD_PARASET, OnUpdateEditKdParaset)
	ON_EN_UPDATE(IDC_EDIT_PPR, OnUpdateEditPpr)
	ON_EN_UPDATE(IDC_EDIT_G00_SPD, OnUpdateEditG00Spd)
	ON_EN_UPDATE(IDC_EDIT_G01_SPD, OnUpdateEditG01Spd)
	ON_CBN_DROPDOWN(IDC_CMB_AXISNO_PARSETPAGE, OnDropdownCmbAxisnoParsetpage)
	ON_EN_UPDATE(IDC_EDIT_G01_ACC, OnUpdateEditG01Acc)
	ON_WM_CLOSE()
	ON_EN_UPDATE(IDC_EDIT_G00_ACC, OnUpdateEditG00Acc)
	ON_EN_UPDATE(IDC_HOME_VEL, OnUpdateHomeVel)
	ON_EN_UPDATE(IDC_HOME_ACC, OnUpdateHomeAcc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageBasePara message handlers

void CPageBasePara::OnSelchangeCmbAxiscount() 
{

	int nAxisCount=((CComboBox *)GetDlgItem(IDC_CMB_AXISCOUNT))->GetCurSel()+1;
   
   CComboBox * pCmbAxisNo=(CComboBox *)GetDlgItem(IDC_CMB_AXISNO_PARSETPAGE);
   switch(nAxisCount) {
   case 1:
	   pCmbAxisNo->ResetContent();
	   pCmbAxisNo->InsertString(0,"轴1");  
   	break;
   case 2:
	    pCmbAxisNo->ResetContent();
	   pCmbAxisNo->InsertString(0,"轴1");
	   pCmbAxisNo->InsertString(1,"轴2");
   	break;
   case 3:
	    pCmbAxisNo->ResetContent();
	   pCmbAxisNo->InsertString(0,"轴1");
	   pCmbAxisNo->InsertString(1,"轴2");
	   pCmbAxisNo->InsertString(2,"轴3");
	   break;
   case 4:
	   pCmbAxisNo->ResetContent();
	   pCmbAxisNo->InsertString(0,"轴1");
	   pCmbAxisNo->InsertString(1,"轴2");
	   pCmbAxisNo->InsertString(2,"轴3");
	   pCmbAxisNo->InsertString(3,"轴4");
	   break;
   }
   
   pCmbAxisNo->SetCurSel(0);

   m_nAxisCount=nAxisCount;

   ((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
	
}

void CPageBasePara::OnSelchangeCmbAxisnoParsetpage() 
{
  CString str;
  m_nAxisNo=((CComboBox *) GetDlgItem(IDC_CMB_AXISNO_PARSETPAGE))->GetCurSel();
  LoadAxisPara(m_nAxisNo);	
}

BOOL CPageBasePara::OnInitDialog() 
{
	int i=0;
	CDialog::OnInitDialog();
	
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();

	m_nAxisNo=0;
	m_nCardType=pApp->m_nCardType;
	m_nAxisCount=pApp->m_nAxisCount;
	m_fG00Spd=pApp->m_fG00Spd;
	m_fG01Spd=pApp->m_fG01Spd;
	* m_fG00Acc=pApp->m_fG00Acc;
	* m_fG01Acc=pApp->m_fG01Acc;
    
//	GetPID();
	for(i=0;i<4;i++)
	{
		m_nCtrlMode[i]=pApp->m_nCtrlMode[i];
		m_nDirPls[i]=pApp->m_nDirPls[i];		
		m_nKP[i]=pApp->m_nKp[i];
		m_nKI[i]=pApp->m_nKi[i];
		m_nKD[i]=pApp->m_nKd[i];
		m_nPPR[i]=pApp->m_nAxisPPR[i];
//		m_nHomeStyle[i]=pApp->m_nHomeStyle[i];
	    m_fHomeVel[i]=pApp->m_fHomeVel[i];
	    m_fHomeAcc[i]=pApp->m_fHomeAcc[i];
	}   
	
//	UpdateData(FALSE);

	CComboBox * pCmbAxisNo=(CComboBox *)GetDlgItem(IDC_CMB_AXISNO_PARSETPAGE);
	CString strAxis[4]={"轴1","轴2","轴3","轴4"};	
	pCmbAxisNo->ResetContent();
	for(i=0;i<m_nAxisCount;i++)
		pCmbAxisNo->InsertString(i,strAxis[i]);  	

	CString str;
	str.Format("%3.2f",m_fG00Spd);
	GetDlgItem(IDC_EDIT_G00_SPD)->SetWindowText(str);

	str.Format("%3.2f",m_fG01Spd);
	GetDlgItem(IDC_EDIT_G01_SPD)->SetWindowText(str);

	str.Format("%3.2f",*m_fG00Acc);
	GetDlgItem(IDC_EDIT_G00_ACC)->SetWindowText(str);
	
	str.Format("%3.2f",*m_fG01Acc);
	GetDlgItem(IDC_EDIT_G01_ACC)->SetWindowText(str);

	((CComboBox *)(GetDlgItem(IDC_CMB_AXISCOUNT)))->SetCurSel(m_nAxisCount-1);//set the count of system axes 
	((CComboBox *)(GetDlgItem(IDC_CMB_AXISNO_PARSETPAGE)))->SetCurSel(0);
	((CComboBox *)(GetDlgItem(IDC_CMB_HOME)))->SetCurSel(0);
	((CComboBox *)(GetDlgItem(IDC_CMB_HOME)))->EnableWindow(false); 


	switch(m_nCardType) {
	case SV_CARD:		
		break;
	case SG_CARD:
	   GetDlgItem(IDC_RADIO_SERVO)->EnableWindow(FALSE);
	   GetDlgItem(IDC_EDIT_KP_PARASET)->EnableWindow(FALSE);
	   GetDlgItem(IDC_EDIT_KI_PARASET)->EnableWindow(FALSE);
	   GetDlgItem(IDC_EDIT_KD_PARASET)->EnableWindow(FALSE);
	   ((CButton *) GetDlgItem(IDC_RADIO_STEP))->SetCheck(TRUE);
		break;
	}
   
	LoadAxisPara(0);

	m_bIsFirstRun=FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CPageBasePara::~CPageBasePara()
{
	delete m_fG00Acc;
	delete m_fG01Acc;
//	delete [] m_nCtrlMode;
//	delete [] m_nDirPls;
	delete [] m_nKP;
	delete [] m_nKI;
	delete [] m_nKD;
//	delete [] m_nHomeStyle;
	delete [] m_fHomeVel;
	delete [] m_fHomeAcc;
	
}

void CPageBasePara::OnRadioStep() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_RADIO_DIR)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_PULSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_KP_PARASET)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_KI_PARASET)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_KD_PARASET)->EnableWindow(FALSE);

	m_nCtrlMode[m_nAxisNo]=STEP_MOTOR;

	((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
	
}

void CPageBasePara::OnRadioServo() 
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_RADIO_DIR)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PULSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_KP_PARASET)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_KI_PARASET)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_KD_PARASET)->EnableWindow(TRUE);
	
	m_nCtrlMode[m_nAxisNo]=SERVO_MOTOR;

	((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
}

void CPageBasePara::OnRadioDir() 
{
   m_nDirPls[m_nAxisNo]=DIR_MODE;
  ((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;	  
}

void CPageBasePara::OnRadioPulse() 
{
	m_nDirPls[m_nAxisNo]=PULSE_MODE;
	((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
}

void CPageBasePara::OnUpdateEditKpParaset() 
{
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_KP_PARASET)->GetWindowText(str);
	
	m_nKP[m_nAxisNo]=atoi(str);
    
    if(!m_bIsFirstRun)
	((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
	
}

void CPageBasePara::OnUpdateEditKiParaset() 
{
	
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_KI_PARASET)->GetWindowText(str);
	
	m_nKI[m_nAxisNo]=atoi(str);

	if(!m_bIsFirstRun)
	((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
	
}

void CPageBasePara::OnUpdateEditKdParaset() 
{
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_KD_PARASET)->GetWindowText(str);	
	m_nKD[m_nAxisNo]=atoi(str);
	if(!m_bIsFirstRun)
		((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
	
}

void CPageBasePara::LoadAxisPara(short nAxisNo)
{
	CString str;
	
	if (m_nCtrlMode[nAxisNo]==STEP_MOTOR) { //当前轴为步进// 
		((CButton *) GetDlgItem(IDC_RADIO_STEP))->SetCheck(TRUE);
		((CButton *) GetDlgItem(IDC_RADIO_SERVO))->SetCheck(FALSE);
		
		GetDlgItem(IDC_EDIT_KP_PARASET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_KI_PARASET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_KD_PARASET)->EnableWindow(FALSE);
		((CButton *) GetDlgItem(IDC_RADIO_DIR))->EnableWindow(TRUE);
		((CButton *) GetDlgItem(IDC_RADIO_PULSE))->EnableWindow(TRUE);

		
		if (m_nDirPls[nAxisNo]==DIR_MODE) {//步进模式为脉冲方向型// 
			((CButton *) GetDlgItem(IDC_RADIO_DIR))->SetCheck(TRUE);
			((CButton *) GetDlgItem(IDC_RADIO_PULSE))->SetCheck(FALSE);
		}
		if(m_nDirPls[nAxisNo]==PULSE_MODE){//步进模式为正负脉冲型//
			((CButton *) GetDlgItem(IDC_RADIO_DIR))->SetCheck(FALSE);
			((CButton *) GetDlgItem(IDC_RADIO_PULSE))->SetCheck(TRUE);
		}
	}
	else if(m_nCardType!=SG_CARD){//当前轴为伺服轴// 
       	GetDlgItem(IDC_EDIT_KP_PARASET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_KI_PARASET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_KD_PARASET)->EnableWindow(TRUE);
		
		((CButton *) GetDlgItem(IDC_RADIO_DIR))->EnableWindow(FALSE);
		((CButton *) GetDlgItem(IDC_RADIO_PULSE))->EnableWindow(FALSE);
		((CButton *) GetDlgItem(IDC_RADIO_STEP))->SetCheck(FALSE);
		((CButton *) GetDlgItem(IDC_RADIO_SERVO))->SetCheck(TRUE);
		
		str.Format("%d",m_nKP[nAxisNo]);
		GetDlgItem(IDC_EDIT_KP_PARASET)->SetWindowText(str);
		
		str.Format("%d",m_nKI[nAxisNo]);
		GetDlgItem(IDC_EDIT_KI_PARASET)->SetWindowText(str);
		
		str.Format("%d",m_nKD[nAxisNo]);
		GetDlgItem(IDC_EDIT_KD_PARASET)->SetWindowText(str);	
	}	 
	
	str.Format("%d",m_nPPR[nAxisNo]);
	GetDlgItem(IDC_EDIT_PPR)->SetWindowText(str);

//	((CComboBox *)GetDlgItem(IDC_CMB_HOME))->SetCurSel(m_nHomeStyle[nAxisNo]);

	str.Format("%3.2f",m_fHomeVel[nAxisNo]);
	GetDlgItem(IDC_HOME_VEL)->SetWindowText(str);

	str.Format("%3.2f",m_fHomeAcc[nAxisNo]);
	GetDlgItem(IDC_HOME_ACC)->SetWindowText(str);

}

bool CPageBasePara::SaveIniFile()
{
	int i=0;
	CMotorControlBenchApp * pApp;
	pApp=(CMotorControlBenchApp *)AfxGetApp();

	int cardtype = pApp->m_nCardType;	
	CString axis[4] = {"Axis1","Axis2","Axis3","Axis4"};
	
	m_nAxisCount=((CComboBox * ) (GetDlgItem(IDC_CMB_AXISCOUNT)))->GetCurSel()+1;
    
	pApp->m_nAxisCount=m_nAxisCount;
	pApp->m_fG00Spd=m_fG00Spd;
	pApp->m_fG01Spd=m_fG01Spd;
	pApp->m_fG00Acc=*m_fG00Acc;
	pApp->m_fG01Acc=*m_fG01Acc;
	
	for(i=0;i<m_nAxisCount;i++)
	{		
		pApp->m_nCtrlMode[i]=m_nCtrlMode[i];
		pApp->m_nAxisPPR[i]=m_nPPR[i];
//		pApp->m_nHomeStyle[i]=m_nHomeStyle[i];
		pApp->m_fHomeVel[i]=m_fHomeVel[i];
		pApp->m_fHomeAcc[i]=m_fHomeAcc[i];
		if(m_nCtrlMode[i]==SERVO_MOTOR)
		{
			pApp->m_nKp[i]=m_nKP[i];
			pApp->m_nKi[i]=m_nKI[i];
			pApp->m_nKd[i]=m_nKD[i];			
		}				
		if (m_nCtrlMode[i]==STEP_MOTOR) {
			pApp->m_nDirPls[i]=m_nDirPls[i];
		}
	}
		
//	pApp->m_inifile.SetVarInt("CARD","CardType",cardtype);//保存卡类型
	pApp->m_inifile.SetVarInt("SYSTEM","AxisCount",m_nAxisCount);//保存系统的总轴数
	pApp->m_inifile.SetVarFloat("SYSTEM","G00Speed",m_fG00Spd);
	pApp->m_inifile.SetVarFloat("SYSTEM","G01Speed",m_fG01Spd);
	pApp->m_inifile.SetVarFloat("SYSTEM","G00ACC",*m_fG00Acc);
	pApp->m_inifile.SetVarFloat("SYSTEM","G01ACC",*m_fG00Acc);

	for(i = 0 ; i < m_nAxisCount ; i ++)//保存电机参数
	{
		pApp->m_inifile.SetVarInt(axis[i],"ControlMode",pApp->m_nCtrlMode[i]);
		pApp->m_inifile.SetVarInt(axis[i],"PPR",pApp->m_nAxisPPR[i]);
		pApp->m_inifile.SetVarInt(axis[i],"HomeStyle",pApp->m_nHomeStyle[i]);
		pApp->m_inifile.SetVarFloat(axis[i],"HomeVel",pApp->m_fHomeVel[i]);
		pApp->m_inifile.SetVarFloat(axis[i],"HomeAcc",pApp->m_fHomeAcc[i]);
	
		if(pApp->m_nCtrlMode[i] == STEP_MOTOR)
		{
			pApp->m_inifile.SetVarInt(axis[i],"Dir/Pulse",pApp->m_nDirPls[i]);
		}
		else if(pApp->m_nCtrlMode[i]== SERVO_MOTOR)
		{
			pApp->m_inifile.SetVarInt(axis[i],"Kp",pApp->m_nKp[i]);  
			pApp->m_inifile.SetVarInt(axis[i],"Ki",pApp->m_nKi[i]);  
			pApp->m_inifile.SetVarInt(axis[i],"Kd",pApp->m_nKd[i]);  
		}	
	}	
	return TRUE;
}

void CPageBasePara::OnOK() 
{
	UpdateData(TRUE);

	CString str;
	int nAxisNo;
	
	nAxisNo=((CComboBox *) GetDlgItem(IDC_CMB_AXISNO_PARSETPAGE))->GetCurSel();
	SaveCurAxisPara(nAxisNo);

	for(int i=0;i<4;i++)
	{
		if (m_nKP[i]<0||m_nKP[i]>200) {
			str.Format("Kp参数的设置范围是0~200，您设置的%d轴Kp值超出范围",i+1);
			MessageBox(str,"XX科技",MB_OK|MB_ICONWARNING);
			return;
		}
		if (m_nKI[i]<0||m_nKI[i]>200) {
			str.Format("Ki参数的设置范围是0~200，您设置的%d轴Ki值超出范围",i+1);
			MessageBox(str,"XX科技",MB_OK|MB_ICONWARNING);
			return;
		}
		if (m_nKD[i]<0||m_nKD[i]>200) {
			str.Format("Kd参数的设置范围是0~200，您设置的%d轴Kd值超出范围",i+1);
			MessageBox(str,"XX科技",MB_OK|MB_ICONWARNING);
			return;
		}
		if (m_nPPR[i]<=0||m_nPPR[i]>10000) {
			str.Format("脉冲当量的设置范围是0~10000，您设置的%d轴脉冲当量值超出范围",i+1);
			MessageBox(str,"XX科技",MB_OK|MB_ICONWARNING);
			return;
		}
		if (m_fHomeVel[i]<=0||m_fHomeVel[i]>100) {
			str.Format("回零速度的设置范围是0~100pulse/st，您设置的%d轴回零速度值超出范围",i+1);
			MessageBox(str,"XX科技",MB_OK|MB_ICONWARNING);
			return;
		}
		if (m_fHomeAcc[i]<=0||m_fHomeAcc[i]>2) {
			str.Format("回零加速度的设置范围是0~2pulse/st^2，您设置的%d轴回零加速度超出范围",i+1);
			MessageBox(str,"XX科技",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	
	if (m_fG00Spd<=0||m_fG00Spd>10) {
		MessageBox("G00速度的设置范围是0~10m/min，您设置的G00速度值超出范围","XX科技",MB_OK|MB_ICONWARNING);
		return;
	}

	if (m_fG01Spd<0||m_fG01Spd>10) {
		MessageBox("G01速度的设置范围是0~10m/min，您设置的G01速度值超出范围","XX科技",MB_OK|MB_ICONWARNING);
		return;
	}

	if (*m_fG00Acc<=0||*m_fG00Acc>500) {
		MessageBox("G00加速度的设置范围是0~500m/min^2，您设置的G00加速度值超出范围","XX科技",MB_OK|MB_ICONWARNING);
		return;
	}

	if (*m_fG01Acc<=0||*m_fG01Acc>500) {
		MessageBox("G01加速度的设置范围是0~500m/min^2，您设置的G01加速度值超出范围","XX科技",MB_OK|MB_ICONWARNING);
		return;
	}

	int nRET;
    CFileDialog IniFileName(TRUE,NULL,NULL,OFN_CREATEPROMPT,"ini文件(*.ini)",NULL);
	
	if(((CMotorControlBenchApp *)AfxGetApp())->m_bSetupUpdated)
	{
		nRET = AfxMessageBox("是否将修改后的参数保存到default.ini?文件中",MB_OKCANCEL|MB_ICONINFORMATION);
		switch(nRET)
		{
		case IDOK:		
			SaveIniFile(/*DEFAULT_INI_FILE*/);	
		case IDCANCEL:
			break;
		}
	}
	MessageBox("您修改的参数已经保存。\n请您退出并重启程序，以使设置的系统生效","XX科技",MB_ICONINFORMATION);
}

void CPageBasePara::OnUpdateEditPpr() 
{
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_PPR)->GetWindowText(str);
	
	m_nPPR[m_nAxisNo]=atoi(str);
	
	if(!m_bIsFirstRun)
		((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;		
}

void CPageBasePara::OnCancel() 
{		
	CDialog::OnCancel();
}

void CPageBasePara::OnUpdateEditG00Spd() 
{
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_G00_SPD)->GetWindowText(str);
	
	m_fG00Spd=atof(str);
	
	if(!m_bIsFirstRun)
	((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE; 
	
}

void CPageBasePara::OnUpdateEditG01Spd() 
{
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_G01_SPD)->GetWindowText(str);	
	m_fG01Spd=atof(str);	
	if(!m_bIsFirstRun)
	((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE; 	
}


void CPageBasePara::OnDropdownCmbAxisnoParsetpage() 
{
	int nAxisNo;
	CString str;
	
	UpdateData(TRUE);	
	nAxisNo=((CComboBox *) GetDlgItem(IDC_CMB_AXISNO_PARSETPAGE))->GetCurSel();
	SaveCurAxisPara(nAxisNo);

}

void CPageBasePara::OnUpdateEditG01Acc() 
{
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_EDIT_G00_ACC)->GetWindowText(str);
	
   * m_fG01Acc=atof(str);
	
	if(!m_bIsFirstRun)
		((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;

}

void CPageBasePara::GetPID()
{
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	
	for( int i=0;i<4;i++)
	{
		m_nCtrlMode[i]=pApp->m_nCtrlMode[i];
		m_nDirPls[i]=pApp->m_nDirPls[i];
		m_nKP[i]=pApp->m_nKp[i];
		m_nKI[i]=pApp->m_nKi[i];
		m_nKD[i]=pApp->m_nKd[i];
		m_nPPR[i]=pApp->m_nAxisPPR[i];
	}
}

void CPageBasePara::OnClose() 
{
	
	CDialog::OnClose();
}

void CPageBasePara::OnUpdateEditG00Acc() 
{
	CString str;
	UpdateData(TRUE);	
	GetDlgItem(IDC_EDIT_G00_ACC)->GetWindowText(str);
	
   * m_fG00Acc=atof(str);
	
	if(!m_bIsFirstRun)
		((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
	
}

void CPageBasePara::OnUpdateHomeVel() 
{
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_HOME_VEL)->GetWindowText(str);	
	m_fHomeVel[m_nAxisNo]=atof(str);
	if(!m_bIsFirstRun)
		((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;
	
}

void CPageBasePara::OnUpdateHomeAcc() 
{
	CString str;
	UpdateData(TRUE);
	
	GetDlgItem(IDC_HOME_ACC)->GetWindowText(str);	
	m_fHomeAcc[m_nAxisNo]=atof(str);
	if(!m_bIsFirstRun)
		((CMotorControlBenchApp * )AfxGetApp())->m_bSetupUpdated=TRUE;	
}

void CPageBasePara::SaveCurAxisPara(int nAxisNo)
{
	CString str;
	
	if (m_nMotorType==1) {//为伺服电机
		
		m_nCtrlMode[nAxisNo]=SERVO_MOTOR;
		
		GetDlgItem(IDC_EDIT_KP_PARASET)->GetWindowText(str);		
		m_nKP[nAxisNo]=atoi(str);
		
		GetDlgItem(IDC_EDIT_KI_PARASET)->GetWindowText(str);		
		m_nKI[m_nAxisNo]=atoi(str);
		
		GetDlgItem(IDC_EDIT_KD_PARASET)->GetWindowText(str);		
		m_nKD[m_nAxisNo]= atoi(str);
	}
	if (m_nMotorType==0) {//为步进电机
		
		m_nCtrlMode[nAxisNo]=STEP_MOTOR;
		
		if (m_nStepMode==0) {
			m_nDirPls[m_nAxisNo]=DIR_MODE;
		}
		if (m_nStepMode==1) {
			m_nDirPls[m_nAxisNo]=PULSE_MODE;
		}		
	}
	
	GetDlgItem(IDC_EDIT_PPR)->GetWindowText(str);	
	m_nPPR[nAxisNo]=atoi(str);
	
//	m_nHomeStyle[nAxisNo]=((CComboBox *)GetDlgItem(IDC_CMB_HOME))->GetCurSel();
	GetDlgItem(IDC_HOME_VEL)->GetWindowText(str);	
	m_fHomeVel[nAxisNo]=atof(str);
	
	GetDlgItem(IDC_HOME_ACC)->GetWindowText(str);	
	m_fHomeAcc[nAxisNo]=atof(str);

}
