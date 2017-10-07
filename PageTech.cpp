// PageVelMode.cpp : implementation file
//

#include "stdafx.h"
#include "motorcontrolbench.h"
#include "TechSelTabCtrl.h"
#include "PageTech.h"
#include "GTCard.h"
#include "Global.h"
#include "GT400.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CGTCard gtcard;

/////////////////////////////////////////////////////////////////////////////
// CPageVelMode dialog


CPageTech::CPageTech(CWnd* pParent /*=NULL*/)
	: CDialog(CPageTech::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageVelMode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageTech::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageVelMode)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB_TECH, m_tabTechSel);
}


BEGIN_MESSAGE_MAP(CPageTech, CDialog)
	//{{AFX_MSG_MAP(CPageVelMode)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NEWPOINT, &CPageTech::OnBnClickedButtonNewpoint)
	ON_BN_CLICKED(IDC_TECH_INSERT, &CPageTech::OnBnClickedTechInsert)
	ON_BN_CLICKED(IDC_TECH_MODIFY, &CPageTech::OnBnClickedTechModify)
	ON_BN_CLICKED(IDC_TECH_DELETE, &CPageTech::OnBnClickedTechDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageTech message handlers

BOOL CPageTech::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	#if 0
	GetDlgItem(IDC_VELMODE_ACC)->SetWindowText("0.05");
	GetDlgItem(IDC_VELMODE_VEL)->SetWindowText("10");
      #endif
      
      m_tabTechSel.InsertItem(0,"切割下刀");
      m_tabTechSel.InsertItem(1,"空跑提刀");
      m_tabTechSel.InitTabPage();
          
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPageTech::OnBnClickedButtonNewpoint()
{
	// TODO: 在此添加控件通知处理程序代码
    #if 1
    int nItem;
    double dpnt[4];
    long pnt[4];

    CString f1, f2, f3, f4,f5;
    CString strFilePathName;
    CStdioFile fs;
    CString strLine, strLineTmp;
    int CurrentPoint = 0;
    int ret = 0;
   // long ccdDeltaX, ccdDeltaY;
  
    #if 0
    gtcard.GetPrflPnt(pnt);	//确定轴当前点位置 
    #endif

    strFilePathName =  ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->strFilePathName;

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

   // ccdDeltaX = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_ccdDeltaX;
   // ccdDeltaY = ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_ccdDeltaY;

    for(int i=0; i<3; i++)
    {
        ret=GT_Axis(i+1);
        ret=GT_GetAtlPos(&pnt[i]);
    }
     

    /*脉冲值要转换成um值 */
    CurrentPoint = ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->PointNum;

    if(m_tabTechSel.m_tabCurrent == 0)
    {
        if(((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->Side == LEFT_SIDE)
        {
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].type = "DownCut";
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].PosX = pnt[0];
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].PosY = pnt[1];
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].PosZ = pnt[2];
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].CutSpeed = m_CutSpeedEdit; 
            f1.Format("L%d", CurrentPoint);
            f2.Format("%d", pnt[0]);
            f3.Format("%d", pnt[1]);
            f4.Format("%d", pnt[2]);
            f5.Format("%d", m_CutSpeedEdit);

            nItem =  ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.InsertItem(CurrentPoint, _T(f1));  // 插入第一列
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 1, _T(f2));  // 插入第二列
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 2, _T(f3));  // 插入第三列
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 3, _T(f4));  // 插入第三列
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 4, _T(f5));  // 插入第三列

            /* 写入到文件中*/
            strLine.Format("DownCut  %d %d %d %d                                          \n", pnt[0], pnt[1], pnt[2], m_CutSpeedEdit);
            while(fs.ReadString(strLineTmp) !=NULL)
            {}
            fs.WriteString(strLine);		
        }
        if(((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->Side == RIGHT_SIDE)
        {
            f1.Format("R%d  %d %d", CurrentPoint, pnt[0]);
        }
    }
	
    if(m_tabTechSel.m_tabCurrent == 1)
    {
         pnt[2] = 0;

        if(((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->Side == LEFT_SIDE)
        {
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].type = "DryRun";
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].PosX = pnt[0];
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].PosY = pnt[1];
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].PosZ = pnt[2];
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->L_Points[CurrentPoint].CutSpeed = 0; 
            
            f1.Format("L%d", CurrentPoint);
            f2.Format("%d", pnt[0]);
            f3.Format("%d", pnt[1]);
            f4.Format("%d", pnt[2]);

            nItem =  ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.InsertItem(CurrentPoint, _T(f1));  // 插入第一列
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 1, _T(f2));  // 插入第二列
            ((CACMotorCtrlWnd *)((CTechSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 2, _T(f3));  // 插入第三列
            ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 3, _T(f4));  // 插入第三列
            ((CACMotorCtrlWnd *)((CTechSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 4, "DryRun");  // 插入第三列

            /* 写入到文件中*/
            strLine.Format("DryRun  %d %d %d 0                                                 \n", pnt[0], pnt[1], pnt[2]);
            while(fs.ReadString(strLineTmp) !=NULL)
            {}
            fs.WriteString(strLine);		
        }
        if(((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->Side == LEFT_SIDE)
        {
            f1.Format("R%d  %d %d DryRun", CurrentPoint, pnt[0]);
        }
    }

    ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->PointNum ++;
    
    fs.Close(); 
     #endif
     
}



void CPageTech::OnBnClickedTechInsert()
{
	// TODO: 在此添加控件通知处理程序代码
    int nItem;
    double dpnt[4];
    long pnt[4];

    CString f1, f2, f3, f4,f5;
    CString strFilePathName;
    CStdioFile fs;
    CString strLine, strLineTmp;
    int CurrentPoint = 0, PointNum = 0;
    int ret = 0;
    PCB_CUT_POINT_S tempPoint;
   int i=0,  j = 0;
    DWORD dwPos = 0;

    strFilePathName =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->strFilePathName;

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

    for(i=0; i<3; i++)
    {
        ret=GT_Axis(i+1);
        ret=GT_GetAtlPos(&pnt[i]);
    }

    PointNum = ++ (((CACMotorCtrlWnd *)(GetParent()->GetParent()))->PointNum);

    /*脉冲值要转换成um值 */
    CurrentPoint =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->IndexInFieldList;
    if(CurrentPoint < 0)
    {
         AfxMessageBox("请先选定要插入的地方！",MB_OK);
         return;
    }
     
    fs.SeekToBegin();
    
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);

    do
    {
        if(j == CurrentPoint)
        {
            break;
        }
		j++;
    }while(fs.ReadString(strLineTmp) !=NULL);

    dwPos = (DWORD)fs.GetPosition();
    fs.Seek(dwPos, CFile::begin);
    
    for(i=PointNum-1; i>CurrentPoint; i--)
    {
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i] =
               ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i-1] ;

        f1.Format("L%d", i);
        f2.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i-1].PosX);
        f3.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i-1].PosY);
        f4.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i-1].PosZ);
        f5.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i-1].CutSpeed);

        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.DeleteItem(i);
        nItem =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.InsertItem(i, _T(f1));  // 插入第一列
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 1, _T(f2));  // 插入第二列
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 2, _T(f3));  // 插入第三列
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 3, _T(f4));  // 插入第三列

        if(((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i-1].type == "DryRun")
        {
            ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 4, "DryRun");  // 插入第三列
        }
        else
        {
            ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(nItem, 4, _T(f5));  // 插入第三列
        }
    }

                
    if(m_tabTechSel.m_tabCurrent == 0)
    {
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].type = "DownCut";
    }
    else if(m_tabTechSel.m_tabCurrent == 1)
    {
         ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].type = "DryRun";
    }

    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosX = pnt[0];
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosY = pnt[1];
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosZ = pnt[2];
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].CutSpeed = m_CutSpeedEdit; 
            
    f1.Format("L%d", CurrentPoint);
    f2.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosX);
    f3.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosY);
    f4.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosZ);
    f5.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].CutSpeed);

    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.DeleteItem(CurrentPoint);
    nItem =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.InsertItem(CurrentPoint, _T(f1));  // 插入第一列
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 1, _T(f2));  // 插入第二列
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 2, _T(f3));  // 插入第三列
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 3, _T(f4));  // 插入第三列
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 4, _T(f5));  // 插入第三列

    for(i = CurrentPoint; i<PointNum; i++)
    {               
        /* 写入到文件中*/
        strLine.Format("%s  %d %d %d %d                                                        \n", 
                    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].type, 
                    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosX,
                    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosY,
                    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosZ,
                    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].CutSpeed);
        fs.WriteString(strLine);
    }
    
    fs.Close();
}


void CPageTech::OnBnClickedTechModify()
{
	// TODO: 在此添加控件通知处理程序代码
    int nItem;
    double dpnt[4];
    long pnt[4];

    CString f1, f2, f3, f4,f5;
    CString strFilePathName;
    CStdioFile fs;
    CString strLine, strLineTmp;
    int CurrentPoint = 0, PointNum = 0;
    int ret = 0;
    int j = 0;
    DWORD dwPos = 0;
    
    strFilePathName =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->strFilePathName;

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

    for(int i=0; i<3; i++)
    {
        ret=GT_Axis(i+1);
        ret=GT_GetAtlPos(&pnt[i]);
    }

    PointNum = ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->PointNum;

    /*脉冲值要转换成um值 */
    CurrentPoint =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->IndexInFieldList;
    if(CurrentPoint < 0)
    {
         AfxMessageBox("请先选定要修改的点！",MB_OK);
         return;
    }

    fs.SeekToBegin();
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);

    do
    {
        if(j == CurrentPoint)
        {
            break;
        }
		j++;
    }while(fs.ReadString(strLineTmp) !=NULL);
                    
    dwPos = (DWORD)fs.GetPosition();
    fs.Seek(dwPos, CFile::begin);

    if(m_tabTechSel.m_tabCurrent == 0)
    {
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].type = "DownCut";
    }
    else if(m_tabTechSel.m_tabCurrent == 1)
    {
         ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].type = "DryRun";
    }

    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosX = pnt[0];
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosY = pnt[1];
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].PosZ = pnt[2];
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].CutSpeed = m_CutSpeedEdit; 

    f1.Format("L%d", CurrentPoint);
    f2.Format("%d", pnt[0]);
    f3.Format("%d", pnt[1]);
    f4.Format("%d", pnt[2]);
    f5.Format("%d", m_CutSpeedEdit);

    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.DeleteItem(CurrentPoint);

     nItem =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.InsertItem(CurrentPoint, _T(f1));  // 插入第一列
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 1, _T(f2));  // 插入第二列
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 2, _T(f3));  // 插入第三列
    ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 3, _T(f4));  // 插入第三列

    if(((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[CurrentPoint].type == "DryRun")
    {
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 4, "DryRun");  // 插入第三列
    }
    else
    {
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(CurrentPoint, 4, _T(f5));  // 插入第三列
    }

    /* 写入到文件中*/
    strLine.Format("DownCut  %d %d %d %d                 ", pnt[0], pnt[1], pnt[2], m_CutSpeedEdit);
    fs.WriteString(strLine);

    fs.Close();
}


void CPageTech::OnBnClickedTechDelete()
{
	// TODO: 在此添加控件通知处理程序代码
    int nItem;

    CString f1, f2, f3, f4,f5;
    CString strFilePathName;
    CStdioFile fs;
    CString strLine, strLineTmp;
    int CurrentPoint = 0, PointNum = 0;
    int ret = 0;
    int j = 0;

    DWORD dwPos = 0;

    strFilePathName =  ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->strFilePathName;

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

    PointNum = ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->PointNum;

    /*脉冲值要转换成um值 */
    CurrentPoint =  ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->IndexInFieldList;
    if(CurrentPoint < 0)
    {
         AfxMessageBox("请先选定要删除的点！",MB_OK);
         return;
    }

    fs.SeekToBegin();
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);
    fs.ReadString(strLineTmp);


	//fs.SeekToBegin();
//    fs.WriteString("testtesttesttest111111111\n");
    while(fs.ReadString(strLineTmp) !=NULL)
    {
        j++;
        if(j == CurrentPoint)
        {
            break;
        }
    }

    
    dwPos = (DWORD)fs.GetPosition();
    fs.Seek(dwPos, CFile::begin);
    
    for(int i=CurrentPoint; i<PointNum-1; i++)
    {
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i] = 
        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i+1];

        f1.Format("L%d", i);
        f2.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosX);
        f3.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosY);
        f4.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosZ);
        f5.Format("%d", ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].CutSpeed);

        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.DeleteItem(i);

        nItem =  ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.InsertItem(i, _T(f1));  // 插入第一列
        ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 1, _T(f2));  // 插入第二列
        ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 2, _T(f3));  // 插入第三列
        ((CACMotorCtrlWnd *)((CModeSelTabCtrl*)GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 3, _T(f4));  // 插入第三列
        if(((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].type == "DryRun")
        {
            ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 4, "DryRun");  // 插入第三列
        }
        else
        {
            ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.SetItemText(i, 4, _T(f5));  // 插入第三列
        }

        ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->m_listCtrl.DeleteItem(PointNum - 1);

        /* 写入到文件中*/
        strLine.Format("%s  %d %d %d %d                                              \n", 
            ((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].type,
			((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosX, 
			((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosY, 
			((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].PosZ, 
			((CACMotorCtrlWnd *)(GetParent()->GetParent()))->L_Points[i].CutSpeed);

        fs.WriteString(strLine);
    }
    
    strLine.Format("                                                                                     \n");
    fs.WriteString(strLine);

    fs.Close();
}
