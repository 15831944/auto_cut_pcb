// InterpExpDrawView.cpp : implementation file
//

#include "stdafx.h"
#include"MotorControlBenchDoc.h"

#include "InterpExpDrawView.h"
#include "GTCARD.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CGTCard gtcard;
extern CPOSITIONList g_PointList;
/////////////////////////////////////////////////////////////////////////////
// CInterpExpDrawView
//���弸�ֳ�����ɫ
const COLORREF RED = RGB(255,0,0);
const COLORREF GREEN = RGB(0,255,0);
const COLORREF BLUE =  RGB(0,0,255);
const COLORREF YELLOW = RGB(255,255,0);
const COLORREF BLACK  = RGB(0,0,0);
const COLORREF WHITE = RGB(255,255,255);
const COLORREF BRIGHT = RGB(100,100,100);
const COLORREF GRAY   = RGB(0,255,255);

IMPLEMENT_DYNCREATE(CInterpExpDrawView, CView)

CInterpExpDrawView::CInterpExpDrawView()
{
	m_nAxisFlag=0;
	m_bDrawFlag=false;
	m_nXMax=100000;
	m_nYMax=100000;
	m_nZMax=100000;
	m_lXEnd=m_lYEnd=0;
//	pDoc=gtcard.pDoc;
		
}

CInterpExpDrawView::~CInterpExpDrawView()
{
}


BEGIN_MESSAGE_MAP(CInterpExpDrawView, CView)
	//{{AFX_MSG_MAP(CInterpExpDrawView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterpExpDrawView drawing

void CInterpExpDrawView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

}

/////////////////////////////////////////////////////////////////////////////
// CInterpExpDrawView diagnostics

#ifdef _DEBUG
void CInterpExpDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CInterpExpDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInterpExpDrawView message handlers

BOOL CInterpExpDrawView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//����ɫ������//	
/*	CBrush greenBrush(RGB(79,124,185));
    CBrush * pOldBrush=pDC->SelectObject(&greenBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);
	pDC->SelectObject(pOldBrush);*/
	/////////////////////////////

	//�ý���ɫͼƬ������//
/*	CRect rectBk;
	CDC dcMem;
	CBitmap bmpBk;

	bmpBk.LoadBitmap(IDB_BACKGROUND); 
	BITMAP stBitmap;
	bmpBk.GetObject(sizeof(BITMAP),&stBitmap);
	CSize bmsize(stBitmap.bmWidth,stBitmap.bmHeight);

	dcMem.CreateCompatibleDC(pDC);

	CBitmap *pOldBitmap=dcMem.SelectObject(&bmpBk);
	GetClientRect(&rectBk);

	pDC->StretchBlt(rectBk.left,rectBk.top,rectBk.Width(),
		rectBk.Height(),&dcMem,0,0,bmsize.cx,bmsize.cy,SRCCOPY);

	dcMem.SelectObject(pOldBitmap);
	dcMem.DeleteDC();
	*/


	return true;
//	return CView::OnEraseBkgnd(pDC);
}

/****************************************************************************/
/*  ������: DrawChart	    												*/
/*  ��  �ܣ�����ͼ��														*/
/*  ��  ����																*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpExpDrawView::DrawChart()
{
	CClientDC  dc1(this);
	CMemDC2   pDC(&dc1);
	CRect rc;
	GetClientRect(&rc);
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetTextColor(YELLOW);
	pDC->SetBkMode(1);
	CPen *pen = new CPen(PS_SOLID,1,GREEN);
	CPen *pOldPen = pDC->SelectObject(pen);	
	if(m_nAxisFlag==0)
	{
		pDC->TextOut(rc.left+rc.Width()/3,20,"X�ᡪY��ͶӰ");
	}
    if(m_nAxisFlag==1)
    {
		pDC->TextOut(rc.left+rc.Width()/3,20,"Y�ᡪZ��ͶӰ");
    }
	if(m_nAxisFlag==2)
    {
		pDC->TextOut(rc.left+rc.Width()/3,20,"X�ᡪZ��ͶӰ");
    }
	this->DrawCoord(pDC);
	this->DrawScale(pDC);
	if(m_bDrawFlag==TRUE)
	{
		switch(m_nCurveType) {
		case 1:
			this->DrawProfileLine(pDC,m_lXEnd,m_lYEnd);
			break;
		case 2:
			this->DrawProfileArc(pDC,m_lXSt,m_lYst,m_lXEnd,m_lYEnd,m_lRadius,m_bIsCW);		
			break;
		}
		
		this->DrawCurve(pDC);
	}
	pDC->SelectObject(pOldPen);
	delete pen;

}

/****************************************************************************/
/*  ������: DrawCurve	    												*/
/*  ��  �ܣ�����ʵʱ�켣����												*/
/*  ��  ����CDC *pDC  ��ͼ��ͼ����DC										*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpExpDrawView::DrawCurve(CDC *pDC)
{
	
	CPOSITION  *startpt,*endpt; 
	POSITION m_position,pos;
	CPOSITIONList * tempList;
	tempList=&g_PointList;
	
	CPen *pen = new CPen(PS_SOLID,1,RED);
	CPen *pOldPen = pDC->SelectObject(pen);

	startpt=g_PointList.GetHead();
 	m_position=g_PointList.GetHeadPosition();
	while((pos=m_position)!=NULL)
	{	
		endpt=g_PointList.GetNext(m_position);
		if(m_nAxisFlag==0)//X-Y������
		{
			pDC->MoveTo(rectAxis.left+rectAxis.Width()/2+300*startpt->x/m_nXMax,
				rectAxis.bottom-rectAxis.Height()/2-300*startpt->y/m_nYMax);
			pDC->LineTo(rectAxis.left+rectAxis.Width()/2+300*endpt->x/m_nXMax,
				rectAxis.bottom-rectAxis.Height()/2-300*endpt->y/m_nYMax);
		}
		if(m_nAxisFlag==1)//Y-Z������
		{
			pDC->MoveTo(rectAxis.left+rectAxis.Width()/2+300*startpt->y/m_nYMax,
				rectAxis.bottom-rectAxis.Height()/2-300*startpt->z/m_nZMax);
			pDC->LineTo(rectAxis.left+rectAxis.Width()/2+300*endpt->y/m_nYMax,
				rectAxis.bottom-rectAxis.Height()/2-300*endpt->z/m_nZMax);
		}
		if(m_nAxisFlag==2)//X-Z������
		{
			pDC->MoveTo(rectAxis.left+rectAxis.Width()/2+300*startpt->x/m_nXMax,
				rectAxis.bottom-rectAxis.Height()/2-300*startpt->z/m_nZMax);
			pDC->LineTo(rectAxis.left+rectAxis.Width()/2+300*endpt->x/m_nXMax,
				rectAxis.bottom-rectAxis.Height()/2-300*endpt->z/m_nZMax);
		}
		startpt=endpt;
	}
	pDC->SelectObject(pOldPen);
	delete pen;  

}

/****************************************************************************/
/*  ������: DrawScale	    												*/
/*  ��  �ܣ�����������̶�													*/
/*  ��  ����CDC *pDC  ��ͼ��ͼ����DC										*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpExpDrawView::DrawScale(CDC *pDC)
{
	if(m_nAxisFlag==0)
	{
		DrawXScale(pDC,m_nXMax);
		DrawYScale(pDC,m_nYMax);//����X-Y��̶�
	}
	else if(m_nAxisFlag==1)
	{   
        DrawXScale(pDC,m_nYMax);
		DrawYScale(pDC,m_nZMax);//����Y-Z��̶�
	}
	else
	{
		DrawXScale(pDC,m_nXMax);//����X-Z��̶�
		DrawYScale(pDC,m_nZMax);
	}
    //�̶�ֵ����
	for(int i=-10;i<=10;i++)
	{
		pDC->MoveTo(rectAxis.left+rectAxis.Width()/2+30*i,rectAxis.bottom-rectAxis.Height()/2);
		pDC->LineTo(rectAxis.left+rectAxis.Width()/2+30*i,rectAxis.bottom-rectAxis.Height()/2-5);
		CString str;
		str.Format("%d",i);
		pDC->TextOut(rectAxis.left+rectAxis.Width()/2+30*i,rectAxis.bottom-rectAxis.Height()/2+5,str);
		pDC->MoveTo(rectAxis.left+rectAxis.Width()/2,rectAxis.bottom-rectAxis.Height()/2-30*i);
		pDC->LineTo(rectAxis.left+rectAxis.Width()/2+5,rectAxis.bottom-rectAxis.Height()/2-30*i);
		str.Format("%d",i);
		pDC->TextOut(rectAxis.left+rectAxis.Width()/2-15,rectAxis.bottom-rectAxis.Height()/2-30*i-5,str);
	}
	
}

/****************************************************************************/
/*  ������: DrawXScale	    												*/
/*  ��  �ܣ�����X������̶�													*/
/*  ��  ����CDC *pDC  ��ͼ��ͼ����DC										*/
/*          int nScaleData  �̶ȵ�λ										*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpExpDrawView::DrawXScale(CDC *pDC, int nScaleData)
{
	CString str;
	CFont font;
	font.CreatePointFont(100,"Arial",pDC);
	pDC->SetTextColor(WHITE);
	pDC->SelectObject(&font);

	switch(nScaleData) {
	case 100:
		str="1*10^1";
		break;
	case 1000:
		str="1*10^2";
		break;
	case 10000:
		str="1*10^3";
		break;
	case 100000:
		str="1*10^4";
		break;
	case 1000000:
		str="1*10^5";
		break;
	case 10000000:
		str="1*10^6";
		break;
	}
	pDC->TextOut(rectAxis.right-60,rectAxis.bottom-280,str);

}


/****************************************************************************/
/*  ������: DrawCoord	    												*/
/*  ��  �ܣ�����������														*/
/*  ��  ����CDC *pDC  ��ͼ��ͼ����DC										*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpExpDrawView::DrawCoord(CDC *pDC)
{
	CRect rc;
	this->GetClientRect(&rc);
	CBrush Brush (RGB(79,124,185));
	pDC->FillRect(&rc,&Brush);
	CPen cPen1;
	cPen1.CreatePen(PS_SOLID,1,WHITE);
	pDC->SelectObject(&cPen1);
	pDC->SetTextAlign(TA_LEFT);
	CFont font;
    font.CreatePointFont(100,"Arial",pDC);
	pDC->SetTextColor(WHITE);
	pDC->SelectObject(&font);

	if(m_nAxisFlag==0)//X-Y��
	{
		pDC->TextOut(rc.right-30,rc.bottom-rc.Height()/2+20,"X��");
		pDC->TextOut(rc.left+rc.Width()/2-30,20,"Y��");
	}
	if(m_nAxisFlag==1)//Y-Z��
	{
		pDC->TextOut(rc.right-30,rc.bottom-rc.Height()/2+20,"Y��");
		pDC->TextOut(rc.left+rc.Width()/2-30,20,"Z��");
	}
	if(m_nAxisFlag==2)//X-Z��
	{
		pDC->TextOut(rc.right-30,rc.bottom-rc.Height()/2+20,"X��");
		pDC->TextOut(rc.left+rc.Width()/2-30,20,"Z��");
	}
    rectAxis.left=10;
	rectAxis.bottom=rc.bottom-10;
	rectAxis.top=10;
	rectAxis.right=rc.right-10;
	//x��
	pDC->MoveTo(rectAxis.left,rectAxis.bottom-rectAxis.Height()/2);
	pDC->LineTo(rectAxis.right,rectAxis.bottom-rectAxis.Height()/2);
	pDC->MoveTo(rectAxis.right,rectAxis.bottom-rectAxis.Height()/2);
	pDC->LineTo(rectAxis.right-10,rectAxis.bottom-rectAxis.Height()/2-15);
	pDC->MoveTo(rectAxis.right,rectAxis.bottom-rectAxis.Height()/2);
	pDC->LineTo(rectAxis.right-10,rectAxis.bottom-rectAxis.Height()/2+15);
	//y��
	pDC->MoveTo(rectAxis.left+rectAxis.Width()/2,rectAxis.bottom);
	pDC->LineTo(rectAxis.left+rectAxis.Width()/2,rectAxis.top);
	pDC->MoveTo(rectAxis.left+rectAxis.Width()/2,rectAxis.top);
	pDC->LineTo(rectAxis.left+rectAxis.Width()/2-10,rectAxis.top+15);
	pDC->MoveTo(rectAxis.left+rectAxis.Width()/2,rectAxis.top);
	pDC->LineTo(rectAxis.left+rectAxis.Width()/2+10,rectAxis.top+15);

}


/****************************************************************************/
/*  ������: DrawYScale	    												*/
/*  ��  �ܣ�����Y������̶�													*/
/*  ��  ����CDC *pDC  ��ͼ��ͼ����DC										*/
/*          int nScaleData  �̶ȵ�λ										*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpExpDrawView::DrawYScale(CDC *pDC, int nScaleData)
{
	CString str;
	CFont font;
	font.CreatePointFont(100,"Arial",pDC);
	pDC->SetTextColor(WHITE);
	pDC->SelectObject(&font);

	switch(nScaleData) {
	case 100:
		str="1*10^1";
		break;
	case 1000:
		str="1*10^2";
		break;
	case 10000:
		str="1*10^3";
		break;
	case 100000:
		str="1*10^4";
		break;
	case 1000000:
		str="1*10^5";
		break;
	case 10000000:
		str="1*10^6";
		break;
	}
	pDC->TextOut(rectAxis.left+rectAxis.Width()/2,rectAxis.top+20,str);
}

void CInterpExpDrawView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	dc.SetMapMode(MM_TEXT);
	CMemDC2  pDC(&dc);  // non flickering painting
	
	CDocument* pDoc = GetDocument();
	CRect rc;
	GetClientRect(&rc);
	pDC->SetBkColor(RGB(79,124,185));
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetTextColor(WHITE);
	pDC->SetBkMode(TRANSPARENT);
	
	if(m_nAxisFlag==1)
	{
		pDC->TextOut(rc.left+rc.Width()/3,20,"X�ᡪY��ͶӰ");
		
	}
    if(m_nAxisFlag==2)
    {
		pDC->TextOut(rc.left+rc.Width()/3,20,"Y�ᡪZ��ͶӰ");
    }
	if(m_nAxisFlag==3)
    {
		pDC->TextOut(rc.left+rc.Width()/3,20,"X�ᡪZ��ͶӰ");
    }
	this->DrawCoord(pDC);
	this->DrawScale(pDC);
	switch(m_nCurveType) {
	case 0:
		break;
	case 1:
		this->DrawProfileLine(pDC,m_lXEnd,m_lYEnd);
		break;
	case 2:
		this->DrawProfileArc(pDC,m_lXSt,m_lYst,m_lXEnd,m_lYEnd,m_lRadius,m_bIsCW);
		break;
	}

	if(m_bDrawFlag==TRUE)
	{
		
		this->DrawCurve(pDC);
	}
}

void CInterpExpDrawView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CMainFrame * pMainFrm;	
	pMainFrm=(CMainFrame *) AfxGetApp()->m_pMainWnd;
	m_pInterpFrm=pMainFrm->m_pInterpFrame;	
}

/****************************************************************************/
/*  ������: DrawProfileLine    												*/
/*  ��  �ܣ�����MIT��DDAֱ�߲岹��׼ֱ�߹켣								*/
/*  ��  ����CDC *pDC  ��ͼ��ͼ����DC										*/
/*           long xEnd, long yEnd  ֱ�߲岹�յ�����							*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpExpDrawView::DrawProfileLine(CDC *pDC, long xEnd, long yEnd)
{
	CPen *pen = new CPen(PS_SOLID,1,GREEN);
	CPen *pOldPen = pDC->SelectObject(pen);
	pDC->MoveTo(rectAxis.left+rectAxis.Width()/2,rectAxis.bottom-rectAxis.Height()/2);
	pDC->LineTo(rectAxis.left+rectAxis.Width()/2+300*xEnd/m_nXMax,
		rectAxis.bottom-rectAxis.Height()/2-300*yEnd/m_nYMax);
	pDC->SelectObject(pOldPen);
	delete pen;

}

/****************************************************************************/
/*  ������: DrawProfileArc    												*/
/*  ��  �ܣ�����MIT��DDAԲ���岹��׼Բ�켣									*/
/*  ��  ����CDC *pDC  ��ͼ��ͼ����DC										*/
/*           long XSt, long YSt  Բ���岹�������							*/	
/*           long xEnd, long yEnd  Բ���岹�յ�����							*/
/*           long R		Բ���岹�뾶										*/
/*           bool bIsCW  �岹����											*/
/*  ��  �ڣ�																*/
/*  ˵  ����																*/
/*  Version:  1.0											                */
/*  Modified date:  2006-11-20                                              */
/*  Editor: Long TY                                                         */
/****************************************************************************/
void CInterpExpDrawView::DrawProfileArc(CDC *pDC, long XSt, long YSt, long XEnd, long YEnd, long R,bool bIsCW)
{
	CRect rect;
	rect.left=rectAxis.left+rectAxis.Width()/2-300*R/m_nXMax;
	rect.top=rectAxis.bottom-rectAxis.Height()/2-300*R/m_nYMax;
	rect.right=rectAxis.left+rectAxis.Width()/2+300*R/m_nXMax;
	rect.bottom=rectAxis.bottom-rectAxis.Height()/2+300*R/m_nYMax;	
	
	CPen penGreen;
	CPen* pOldPen;	
	
	penGreen.CreatePen(PS_SOLID | PS_COSMETIC, 1, GREEN);	
	pOldPen = pDC->SelectObject(&penGreen);
	
	pDC->Arc(rect,
		CPoint(rect.left,rect.CenterPoint().y),
		CPoint(rect.left,rect.CenterPoint().y));
	pDC->SelectObject(pOldPen);
	
	pDC->SelectObject(pOldPen);
}
