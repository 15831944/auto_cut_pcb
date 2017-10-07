// PageSMode.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "PageEdit.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageExec dialog


CPageEdit::CPageEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CPageEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageExec)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageExec)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageEdit, CDialog)
	//{{AFX_MSG_MAP(CPageExec)
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CPageEdit::OnBnClickedButtonOpenfile)
	//ON_BN_CLICKED(IDC_BUTTON_AUTO_RUN, &CPageEdit::OnBnClickedButtonAutoRun)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageExec message handlers

BOOL CPageEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageEdit::OnBnClickedButtonOpenfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


#if 0
void CPageExec::LnXY(long x, long y, double acc, double fVel)
{
	short rtn;
       rtn = GT_SetSynAcc(acc);    //Error(rtn,"GT_SetSynAcc");
	rtn = GT_SetSynVel(fVel);    //Error(rtn,"GT_SetSynVel");
	rtn=GT_LnXY(x, y);  // Error(rtn,"GT_LnXY");
	unsigned short mode;
	rtn = GT_GetMode(&mode);
}


void CPageExec::GetActMark(&MarkAX, &MarkAY, &MarkBX, &MarkBY);
{
    /* ��ȡ��������*/
    /* ת�����˶�����*/
}
#endif

void CPageEdit::OnBnClickedButtonAutoRun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
       /* �˶���Mark �㣬�ó��µ�Mark������ */
       long LB0_ValueX=0, LB0_ValueY=0,LB1_ValueX=0,LB1_ValueY=0;
       long MarkAX, MarkAY, MarkBX, MarkBY;

        #if 0
       LB0_ValueX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->LB0_ValueX;
       LB0_ValueY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->LB0_ValueY;
       LB1_ValueX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->LB1_ValueX;
       LB1_ValueY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->LB1_ValueY;

       rtn=LnXY(LB0_ValueX, LB0_ValueY);
       GetMark(&MarkAX, &MarkAY, &MarkBX, &MarkBY);
       
        alpha = atan(abs(LB1_ValueY-LB0_ValueY) / abs(LB1_ValueX - LB0_ValueX))
        beta = atan(MarkY / MarkX);
        
        thet = beta - alpha;

        GetActPoint();
        
       /* ������ԭ���ƶ�����һ����㴦*/
       rtn=gtcard.LineXYZ(senddata.x,senddata.y,senddata.z);

       while()
        {
            /* �����ֱ�߲������µ���*/
            if(Segment[].type == LINE && )
            {
                /* */
                AC = sqrt(power(Segment[0].startX,2) + power(Segment[0].startY,2));
                eta  = atan(Segment[0].startY/Segment[0].startX);
                     
                acturalX = AC*cos(eta + thet) + MarkAX;
                acturalY = AC*sin(eta + thet) + MarkAY;
                
                /* �Ȱ�Z������ȥ*/

                rtn=gtcard.LineXY(acturalX, acturalY);
            }
            if(��ת�ᵶ��)
            {
                /* ��Z �������� */
            }
            if(arc)
            {
                Segment.type = arc;
                
            }

        }
       #endif
}



