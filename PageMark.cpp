// CPageMark.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "ACMotorCtrlWnd.h"
#include "CcdSelTabCtrl.h"
#include "PageMark.h"
#include "GTCard.h"
#include "Global.h"

#include "GT400.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageMark dialog

extern CGTCard gtcard;

CPageMark::CPageMark(CWnd* pParent /*=NULL*/)
	: CDialog(CPageMark::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageMark)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CPageMark::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMark)
		// NOTE: the ClassWizard will add DDX and DDV calls here
      DDX_Control(pDX, IDC_TAB_CCD, m_tabViewSel);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageMark, CDialog)
	//{{AFX_MSG_MAP(CPageMark)
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CPageMark::OnDeltaposSpin3)
	ON_BN_CLICKED(IDC_BUTTON_MARK_MODIFY, &CPageMark::OnBnClickedButtonMarkModify)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Z_COMPENSATE, &CPageMark::OnDeltaposSpinZCompensate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMark message handlers

BOOL CPageMark::OnInitDialog() 
{
	RECT LineListRect;

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	GetDlgItem(IDC_TMODE_ACC)->SetWindowText("0.01");
//	GetDlgItem(IDC_TMODE_VEL)->SetWindowText("5");
//	GetDlgItem(IDC_TMODE_POS)->SetWindowText("20000");
      #if 1
	m_tabViewSel.InsertItem(0,"范围");
	m_tabViewSel.InsertItem(1,"亮度");
	m_tabViewSel.InsertItem(2,"参数");
	m_tabViewSel.InsertItem(3,"连片");

	m_tabViewSel.InitTabPage();
       #endif
#if 0
    LineListRect=CRect(0,530,800,620);	

    m_tabViewSel.Create(LVS_EX_GRIDLINES | LVS_REPORT | LVS_EDITLABELS | WS_CHILD | WS_VISIBLE, LineListRect, this, IDC_LINE_LIST);
    m_tabViewSel.InsertColumn(0, "Column 1", LVCFMT_LEFT, 40, 500);
    m_tabViewSel.InsertItem( 0, "aaa");
#endif


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageMark::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

#if 0
void CPageMark::GetActualMark(double *MarkBX, double *MarkBY)
{
    // TODO: 在此添加命令处理程序代码
    CvRect selection;
    int i;
    double s1,len1,h1,d1;  
    CvPoint* p;    
    CvScalar center;//重心位置
    int m_CcdWidth = 0, m_CcdHeight = 0;
    IplImage* img_8uc1;
        
    pedit=(CEdit*)(m_tabViewSel.m_tabPages[0]->GetDlgItem(IDC_CCD_WIDTH));
    pedit->GetWindowText(str);
    m_CcdWidth = atol(str);
    pedit=(CEdit*)(m_tabViewSel.m_tabPages[0]->GetDlgItem(IDC_CCD_HEIGHT));
    pedit->GetWindowText(str);
    m_CcdHeight = atol(str);

    while(1)
    {
        img_8uc1=cvQueryFrame(m_Video);             //读取摄像头一帧数据

        selection.x=img_8uc1->width/2 - m_CcdWidth/2;     
        selection.y=img_8uc1->height/2 - m_CcdHeight/2;
        selection.width=m_CcdWidth;
        selection.height=m_CcdHeight;
        cvSetImageROI(img_8uc1, selection);
        cvShowImage("flower1",img_8uc1);

        IplImage* img_edge = cvCreateImage(cvGetSize(img_8uc1),8,1);
        IplImage* img_8uc3 = cvCreateImage(cvGetSize(img_8uc1),8,3);


        cvSmooth( img_8uc1, img_8uc1, CV_GAUSSIAN, 9, 9 ); // smooth it, otherwise a lot of false circles may be detected 

        //cvThreshold(img_8uc1,img_edge,128,255,CV_THRESH_BINARY);
        cvAdaptiveThreshold( img_8uc1, img_edge, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 61);
        cvShowImage("binary", img_edge);

        CvMemStorage* storage = cvCreateMemStorage();
        CvSeq* first_contour = NULL;

        int Nc = cvFindContours(
                                         img_edge,
                                         storage,
                                         &first_contour,
                                         sizeof(CvContour),
                                         CV_RETR_LIST
                                         );

        int n=0;
        //初始化重心位置
        center = cvScalarAll(0);
        for (CvSeq* c=first_contour;c!=NULL;c=c->h_next)
        {
            cvCvtColor(img_8uc1,img_8uc3,CV_GRAY2BGR);
            s1 = fabs(cvContourArea(c));//轮廓面积  
            len1=cvArcLength(c);  

            if(s1>30 && len1 >50)
            {
                cvDrawContours(
                                        img_8uc3,
                                        c,
                                        CV_RGB(0,0,255),
                                        CV_RGB(0,255,0),
                                        0,
                                        2,
                                        8
                                        );

                
                for(i = 0;i < c->total;i++)
                {
                    p = (CvPoint* )cvGetSeqElem(c, i);
                    center.val[0] += p->x;
                    center.val[1] += p->y;
                }

                center.val[0] /= c->total;
                center.val[1] /= c->total;

                img_8uc1->width/2
                    
                s32CurX = center.val[0] - img_8uc1->width/2;
                s32CurY = center.val[1] - img_8uc1->height/2;

                if(center.val[0] - img_8uc1->width/2 < -0.5)
                {
                    
                }
                if(center.val[0] - img_8uc1->width/2 > 0.5)
                {
                }
                if(center.val[1] - img_8uc1->height/2 < -0.5)
                {
                    
                }
                if(center.val[1] - img_8uc1->height/2 > 0.5)
                {

                }
            }
        }
         
        n++;
    }
 
    // printf("Finished all contours.\n");
 
     cvCvtColor(img_8uc1,img_8uc3,CV_GRAY2BGR);
     cvShowImage("flower2",img_8uc3);
 
     cvWaitKey(0);
 
     cvDestroyWindow("flower");
     cvReleaseImage(&img_8uc3);
     cvReleaseImage(&img_8uc1);
     cvReleaseImage(&img_edge);
    // return 0;
    
}
#endif

void CPageMark::OnBnClickedButtonMarkModify()
{
	// TODO: 在此添加控件通知处理程序代码
#if 1
    int nItem;
    long pnt[4];

    CString f1,f2;
    CString strLine, strLineTmp;
    CString strFilePathName;
    CStdioFile fs;
    int ret = 0;
//    long ccdDeltaX, ccdDeltaY;
    
    strFilePathName =  ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->strFilePathName;

   if(strFilePathName.IsEmpty())
   {
         AfxMessageBox("请先打开工作档案！",MB_OK);
         return;
   }
    
    /* 保存在档案中*/
    if(fs.Open(strFilePathName, CFile::modeReadWrite | CFile::typeText)==NULL) 
    {
        AfxMessageBox("打开工作档案出错！",MB_OK);
        return;
    }

  //  ccdDeltaX = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->m_ccdDeltaX;
  //  ccdDeltaY = ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->m_ccdDeltaY;

    for(int i=0; i<3; i++)
    {
        ret=GT_Axis(i+1);
        ret=GT_GetAtlPos(&pnt[i]);
    }
    
    /*脉冲值要转换成um值 */
    
    switch (((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->IndexInFieldList)
    {
        case 0:
            ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB0_ValueX = pnt[0];
            ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB0_ValueY = pnt[1];
            strLine.ReleaseBuffer();

            strLine.Format("LB0  %d %d", pnt[0], pnt[1]);
            fs.WriteString(strLine);		
            /* 插入列表中 */
  //          f1.Format("LB0");
            f1.Format("%d", pnt[0]);
            f2.Format("%d", pnt[1]);
  //          nItem =  ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.InsertItem(0, _T(f1));  // 插入第一列
            ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(0, 1, _T(f1));  // 插入第二列
            ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(0, 2, _T(f2));  // 插入第三列
            
            break;
        case 1:
            ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB1_ValueX = pnt[0];
            ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->LB1_ValueY = pnt[1];

            strLine.ReleaseBuffer();
            strLine.Format("LB1  %d %d", pnt[0], pnt[1]);
            if(fs.ReadString(strLineTmp) !=NULL)
            {
                 fs.Seek(0,CFile::current);
                 fs.WriteString(strLine);		
            }
            else
            {
                AfxMessageBox("写入文件失败",MB_OK);
                fs.Close();	
                return;
            }
            
   //        f1.Format("LB1");
            f1.Format("%d", pnt[0]);
            f2.Format("%d", pnt[1]);
   //         nItem =  ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.InsertItem(1, _T(f1));  // 插入第一列
            ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(1, 1, _T(f1));  // 插入第二列
            ((CACMotorCtrlWnd *)((CCcdSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(1, 2, _T(f2));  // 插入第三列

            break;
        default:
            break;   
    }

    fs.Close();	

#endif
}


void CPageMark::OnDeltaposSpinZCompensate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
