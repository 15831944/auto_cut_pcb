// PageGearMode.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageGearMode.h"

#include "GT400.h"
#include "GTCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPageGearMode dialog
extern CGTCard gtcard;

CPageGearMode::CPageGearMode(CWnd* pParent /*=NULL*/)
	: CDialog(CPageGearMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageGearMode)
	//}}AFX_DATA_INIT
}


void CPageGearMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGearMode)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageGearMode, CDialog)
	//{{AFX_MSG_MAP(CPageGearMode)
	ON_BN_CLICKED(IDC_BTN_GEAR_OK, OnBtnGearOk)
	ON_BN_CLICKED(IDC_BTN_GEAR_CANCEL, OnBtnGearCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGearMode message handlers

void CPageGearMode::OnBtnGearOk() 
{
	int nCurAxis;   //从动轴号
	int MainAxis;   //主动轴号
	double fRatio;  //传动比
	CString strRatio,strCurAixs,strInfo;

	UpdateData(TRUE);
	
//参数设置//	
	//获取从动轴号
    GetDlgItem(IDC_PASSIVEAXIS)->GetWindowText(strCurAixs);	
    nCurAxis=atoi(strCurAixs);
	((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_nAuxAxisNo=nCurAxis;
    //获取主动轴号
	MainAxis=((CComboBox *) GetDlgItem(IDC_CMB_MAIN_AXIS))->GetCurSel()+1;
	//传动比
	((CEdit *) GetDlgItem(IDC_GEAR_RATIO))->GetWindowText(strRatio);
	fRatio=atof(strRatio);

	//错误设置处理//
	if(0==fRatio) 
	{
		MessageBox("电子齿轮传动比不能设置为0","设置错误",MB_OK|MB_ICONWARNING);
		return;
	}
	if((fRatio<-100)||(fRatio>100) )
	{
		MessageBox("电子齿轮传动比设置超出范围","设置错误",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(MainAxis==nCurAxis)
	{
		MessageBox("从动轴(当前轴)与主动轴设置成了同一轴号!","设置错误",MB_OK|MB_ICONWARNING);
		return;
	}

	//电子齿轮设置
	if(!gtcard.AxisGMotion(MainAxis,nCurAxis,fRatio))
	{
		MessageBox("电子齿轮设置失败！","XX科技",MB_OK|MB_ICONERROR);
		return;
	}

	((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_bGearModeSet=true;
	strInfo.Format("电子齿轮设置成功！请按以下步骤：\n\n1.在电机选择中选择%d轴，\n\n2.切换至其他模式下\n\n3.设置主动轴运动参数\n\n4.点击“开启轴”按钮\n\n5.点击“运行”按钮",MainAxis);
	MessageBox(strInfo,"XX科技",MB_ICONINFORMATION);
	
}

BOOL CPageGearMode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nAxisCount;
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	CComboBox * pAxisSelBox=(CComboBox *) GetDlgItem(IDC_CMB_MAIN_AXIS);
	nAxisCount=pApp->m_nAxisCount;
  
    switch(nAxisCount) {
    case 1:
      MessageBox("单轴模式下电子齿轮模式无效！","警告",MB_ICONWARNING|MB_OK);	
    	break;
    case 2:
		pAxisSelBox->ResetContent();
        pAxisSelBox->InsertString(0,"1轴");
		pAxisSelBox->InsertString(1,"2轴");
    	break;
	case 3:
        pAxisSelBox->ResetContent();
        pAxisSelBox->InsertString(0,"1轴");
		pAxisSelBox->InsertString(1,"2轴");
		pAxisSelBox->InsertString(2,"3轴");
		break;
    case 4:
		pAxisSelBox->ResetContent();
        pAxisSelBox->InsertString(0,"1轴");
		pAxisSelBox->InsertString(1,"2轴");
		pAxisSelBox->InsertString(2,"3轴");
        pAxisSelBox->InsertString(3,"4轴");
		break;    
    } 
	((CComboBox*) GetDlgItem(IDC_CMB_MAIN_AXIS))->SetCurSel(1);
	((CEdit *) GetDlgItem(IDC_GEAR_RATIO))->SetWindowText("-1");
	
	return TRUE;  
}

void CPageGearMode::OnBtnGearCancel() 
{
	for(int i=1;i<5;i++)
	{
		GT_Axis(i);
		GT_PrflT();
		GT_AxisOff();
	}
	((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_bGearModeSet=false;	
		MessageBox("先前的电子齿轮设置已被取消","XX科技",MB_ICONINFORMATION);
}





















