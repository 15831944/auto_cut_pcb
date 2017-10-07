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
	int nCurAxis;   //�Ӷ����
	int MainAxis;   //�������
	double fRatio;  //������
	CString strRatio,strCurAixs,strInfo;

	UpdateData(TRUE);
	
//��������//	
	//��ȡ�Ӷ����
    GetDlgItem(IDC_PASSIVEAXIS)->GetWindowText(strCurAixs);	
    nCurAxis=atoi(strCurAixs);
	((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_nAuxAxisNo=nCurAxis;
    //��ȡ�������
	MainAxis=((CComboBox *) GetDlgItem(IDC_CMB_MAIN_AXIS))->GetCurSel()+1;
	//������
	((CEdit *) GetDlgItem(IDC_GEAR_RATIO))->GetWindowText(strRatio);
	fRatio=atof(strRatio);

	//�������ô���//
	if(0==fRatio) 
	{
		MessageBox("���ӳ��ִ����Ȳ�������Ϊ0","���ô���",MB_OK|MB_ICONWARNING);
		return;
	}
	if((fRatio<-100)||(fRatio>100) )
	{
		MessageBox("���ӳ��ִ��������ó�����Χ","���ô���",MB_OK|MB_ICONWARNING);
		return;
	}
	
	if(MainAxis==nCurAxis)
	{
		MessageBox("�Ӷ���(��ǰ��)�����������ó���ͬһ���!","���ô���",MB_OK|MB_ICONWARNING);
		return;
	}

	//���ӳ�������
	if(!gtcard.AxisGMotion(MainAxis,nCurAxis,fRatio))
	{
		MessageBox("���ӳ�������ʧ�ܣ�","XX�Ƽ�",MB_OK|MB_ICONERROR);
		return;
	}

	((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_bGearModeSet=true;
	strInfo.Format("���ӳ������óɹ����밴���²��裺\n\n1.�ڵ��ѡ����ѡ��%d�ᣬ\n\n2.�л�������ģʽ��\n\n3.�����������˶�����\n\n4.����������ᡱ��ť\n\n5.��������С���ť",MainAxis);
	MessageBox(strInfo,"XX�Ƽ�",MB_ICONINFORMATION);
	
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
      MessageBox("����ģʽ�µ��ӳ���ģʽ��Ч��","����",MB_ICONWARNING|MB_OK);	
    	break;
    case 2:
		pAxisSelBox->ResetContent();
        pAxisSelBox->InsertString(0,"1��");
		pAxisSelBox->InsertString(1,"2��");
    	break;
	case 3:
        pAxisSelBox->ResetContent();
        pAxisSelBox->InsertString(0,"1��");
		pAxisSelBox->InsertString(1,"2��");
		pAxisSelBox->InsertString(2,"3��");
		break;
    case 4:
		pAxisSelBox->ResetContent();
        pAxisSelBox->InsertString(0,"1��");
		pAxisSelBox->InsertString(1,"2��");
		pAxisSelBox->InsertString(2,"3��");
        pAxisSelBox->InsertString(3,"4��");
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
		MessageBox("��ǰ�ĵ��ӳ��������ѱ�ȡ��","XX�Ƽ�",MB_ICONINFORMATION);
}





















