// ModeSelTabCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////
// This class is provided as is and Ben Hill takes no
// responsibility for any loss of any kind in connection
// to this code.
/////////////////////////////////////////////////////
// Is is meant purely as a educational tool and may
// contain bugs.
/////////////////////////////////////////////////////
// ben@shido.fsnet.co.uk
// http://www.shido.fsnet.co.uk
/////////////////////////////////////////////////////
// Thanks to a mystery poster in the C++ forum on 
// www.codeguru.com I can't find your name to say thanks
// for your Control drawing code. If you are that person 
// thank you very much. I have been able to use some of 
// you ideas to produce this sample application.
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"

#include "CcdSelTabCtrl.h"
#include "ModeSelTabCtrl.h"
#include "ACMotorCtrlWnd.h"

#include "PageExec.h"
#include "PageMark.h"
#include "PageEdit.h"
#include "PageSystem.h"
#include "TechSelTabCtrl.h"
#include "PageTech.h"
#include "PageGearMode.h"
#include "MotorControlBench.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModeSelTabCtrl
CString pacString[3] = {"主轴钻孔点",  "CCD对准M0主轴钻孔点", "XY治具CCD原点"};

CModeSelTabCtrl::CModeSelTabCtrl()
{
	::ZeroMemory(&m_tabPages, sizeof(m_tabPages));
      m_tabPages[0]=new CPageExec;
	m_tabPages[1]=new CPageMark;
	m_tabPages[2]=new CPageTech;
	m_tabPages[3]=new CPageEdit;  
    	m_tabPages[4]=new CPageSystem;
	m_nNumberOfPages=5;
}

CModeSelTabCtrl::~CModeSelTabCtrl()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
}

void CModeSelTabCtrl::InitTabPage()
{
	CMotorControlBenchApp * pApp=(CMotorControlBenchApp *)AfxGetApp();
	
	m_tabCurrent=0;

	m_tabPages[0]->Create(IDD_PAGE_EXEC, this);
	m_tabPages[1]->Create(IDD_PAGE_MARK, this);
	m_tabPages[2]->Create(IDD_PAGE_TECH, this);
      m_tabPages[3]->Create(IDD_PAGE_EDIT, this);	
      m_tabPages[4]->Create(IDD_PAGE_SYSTEM, this);	
	
	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);
	m_tabPages[2]->ShowWindow(SW_HIDE);
	m_tabPages[3]->ShowWindow(SW_HIDE);
	m_tabPages[4]->ShowWindow(SW_HIDE);
	
	SetRectangle();
}

void CModeSelTabCtrl::SetRectangle()
{
#if 0
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++){
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
    #endif
    	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	//nY=itemRect.bottom+1;
	nY=1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-30;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	m_tabPages[1]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	m_tabPages[2]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
      m_tabPages[3]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
      m_tabPages[4]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);

}

BEGIN_MESSAGE_MAP(CModeSelTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CModeSelTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeSelTabCtrl message handlers

void CModeSelTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus())
      {
             CString f1, f2, f3, f4, f5;
            	int nItem;

		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();

            if(m_tabCurrent == LABAL_TAB)
            {     
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.DeleteAllItems();
                CHeaderCtrl* pHeaderCtrl = ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.GetHeaderCtrl();

                if (pHeaderCtrl != NULL)
                {
                    int  nColumnCount = pHeaderCtrl->GetItemCount();
                    for (int i=0; i<nColumnCount; i++)
                    {
                        ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.DeleteColumn(0);
                    }
                }

                 // 插入列表控件纵列
                 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(0, "No", LVCFMT_LEFT, 100);        	
                 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(1, "X um", LVCFMT_LEFT, 120);
                 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(2, "Y um", LVCFMT_LEFT, 120);
            	// 设置列表控件扩展样式为整行高亮
                 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);

            	// 往列表控件中添加项目
                {
                    if( ((CACMotorCtrlWnd *)GetParent())->Side == LEFT_SIDE)
                    {
                		// 格式化字符串
                		f1.Format("LB0");
                		f2.Format("%d",  ((CACMotorCtrlWnd *)GetParent())->LB0_ValueX);
                		f3.Format("%d",  ((CACMotorCtrlWnd *)GetParent())->LB0_ValueY);
                		nItem =  ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertItem(0, _T(f1));  // 插入第一列
                		 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
                		 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列

                          f1.Format("LB1");
                		f2.Format("%d",  ((CACMotorCtrlWnd *)GetParent())->LB1_ValueX);
                		f3.Format("%d",  ((CACMotorCtrlWnd *)GetParent())->LB1_ValueY);
                		nItem =  ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertItem(1, _T(f1));  // 插入第一列
                		 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
                		 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列
                   }
                   else if( ((CACMotorCtrlWnd *)GetParent())->Side == RIGHT_SIDE)
                   {
                          f1.Format("RB0");
                		f2.Format("%d", ((CACMotorCtrlWnd *)GetParent())->RB0_ValueX);
                		f3.Format("%d", ((CACMotorCtrlWnd *)GetParent())->RB0_ValueY);
                		nItem =  ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertItem(2, _T(f1));  // 插入第一列
                		 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
                		 ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列

                          f1.Format("RB1");
                		f2.Format("%d", ((CACMotorCtrlWnd *)GetParent())->RB1_ValueX);
                		f3.Format("%d", ((CACMotorCtrlWnd *)GetParent())->RB1_ValueY);
                		nItem =  ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertItem(3, _T(f1));  // 插入第一列
                		((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第二列
                		((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列
                    }
            	  }
            }

             if(m_tabCurrent == TECH_TAB || m_tabCurrent == EXC_TAB)
             {
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.DeleteAllItems();

                CHeaderCtrl* pHeaderCtrl = ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.GetHeaderCtrl();

                if (pHeaderCtrl != NULL)
                {
                    int  nColumnCount = pHeaderCtrl->GetItemCount();
                    for (int i=0; i<nColumnCount; i++)
                    {
                        ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.DeleteColumn(0);
                    }
                }

                // 插入列表控件纵列
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(0, "T0", LVCFMT_LEFT, 100);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(1, "X(um)", LVCFMT_LEFT, 120);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(2, "Y(um)", LVCFMT_LEFT, 120);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(3, "Z or R(um)", LVCFMT_LEFT, 120);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(4, "Spd(mm/sec)", LVCFMT_LEFT, 120);

                // 设置列表控件扩展样式为整行高亮
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES);   

                for(int i=0; i< ((CACMotorCtrlWnd *)GetParent())->PointNum; i++)
                {
                    if( ((CACMotorCtrlWnd *)GetParent())->Side == LEFT_SIDE)
                   {
                		// 格式化字符串
                		f1.Format("L%d", i);
                		f2.Format("%d", ((CACMotorCtrlWnd *)GetParent())->L_Points[i].PosX);
                          f3.Format("%d", ((CACMotorCtrlWnd *)GetParent())->L_Points[i].PosY);
                          f4.Format("%d", ((CACMotorCtrlWnd *)GetParent())->L_Points[i].PosZ);
                          f5.Format("%d", ((CACMotorCtrlWnd *)GetParent())->L_Points[i].CutSpeed);
                          nItem =  ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertItem(i, _T(f1));  // 插入第一列
                		((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第三列
                		((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列
                		((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 3, _T(f4));  // 插入第三列
                		if(((CACMotorCtrlWnd *)GetParent())->L_Points[i].type == "DownCut")
                		{
                              ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 4, _T(f5));  // 插入第三列
                	      }
                        else
                         {
                		    ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 4, ((CACMotorCtrlWnd *)GetParent())->L_Points[i].type); 
                         }
                   }
                   if( ((CACMotorCtrlWnd *)GetParent())->Side == RIGHT_SIDE)
                   {
                		// 格式化字符串
                		f1.Format("R%d", i);
                		f2.Format("%d", ((CACMotorCtrlWnd *)GetParent())->R_Points[i].PosX);
                          f3.Format("%d", ((CACMotorCtrlWnd *)GetParent())->R_Points[i].PosY);                          
                          nItem =  ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertItem(i, _T(f1));  // 插入第一列
                		((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第三列
                		((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列
                		((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 3, ((CACMotorCtrlWnd *)GetParent())->R_Points[i].type); 
                   }
               }
	    }

           if(m_tabCurrent == SYS_TAB)
            {
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.DeleteAllItems();

                CHeaderCtrl* pHeaderCtrl = ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.GetHeaderCtrl();

                if (pHeaderCtrl != NULL)
                {
                    int  nColumnCount = pHeaderCtrl->GetItemCount();
                    for (int i=0; i<nColumnCount; i++)
                    {
                        ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.DeleteColumn(0);
                    }
                }

                // 插入列表控件纵列
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(0, "No", LVCFMT_LEFT, 100);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(1, "X(um)", LVCFMT_LEFT, 120);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(2, "Y(um)", LVCFMT_LEFT, 120);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(3, "Z(um)", LVCFMT_LEFT, 120);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(4, "WH", LVCFMT_LEFT, 120);
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertColumn(5, "Note", LVCFMT_LEFT, 120);

                // 设置列表控件扩展样式为整行高亮
                ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT |  LVS_EX_GRIDLINES); 
                for(int i=0; i< 3; i++)
                {
                    if( ((CACMotorCtrlWnd *)GetParent())->Side == LEFT_SIDE)
                    {
                        // 格式化字符串
                        f1.Format("M%d", i);
                        f2.Format("%d", ((CACMotorCtrlWnd *)GetParent())->L_MPoints[i].PosX);
                        f3.Format("%d", ((CACMotorCtrlWnd *)GetParent())->L_MPoints[i].PosY);
                        f4.Format("%d", ((CACMotorCtrlWnd *)GetParent())->L_MPoints[i].PosZ);
                        f5.Format("%d", ((CACMotorCtrlWnd *)GetParent())->L_MPoints[i].Para);
                        nItem =  ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.InsertItem(i, _T(f1));  // 插入第一列
                        ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 1, _T(f2));  // 插入第三列
                        ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 2, _T(f3));  // 插入第三列
                        ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 3, _T(f4));  // 插入第三列
                        ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 4, _T(f5));  // 插入第三列
                        ((CACMotorCtrlWnd *)GetParent())->m_listCtrl.SetItemText(nItem, 5, pacString[i]);
                    }
                    if( ((CACMotorCtrlWnd *)GetParent())->Side == RIGHT_SIDE)
                    {
                        
                    }
                }
	    }
	}

}
























