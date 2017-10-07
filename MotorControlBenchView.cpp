// MotorControlBenchView.cpp : implementation of the CMotorControlBenchView class
//

#include "stdafx.h"
#include "MotorControlBench.h"

#include "MotorControlBenchDoc.h"
#include "MotorControlBenchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchView

IMPLEMENT_DYNCREATE(CMotorControlBenchView, CView)

BEGIN_MESSAGE_MAP(CMotorControlBenchView, CView)
	//{{AFX_MSG_MAP(CMotorControlBenchView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchView construction/destruction

CMotorControlBenchView::CMotorControlBenchView()
:CFormView(CMotorControlBenchView::IDD)
{
	// TODO: add construction code here

}

CMotorControlBenchView::~CMotorControlBenchView()
{
}

BOOL CMotorControlBenchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchView diagnostics

#ifdef _DEBUG
void CMotorControlBenchView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMotorControlBenchView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMotorControlBenchDoc* CMotorControlBenchView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMotorControlBenchDoc)));
	return (CMotorControlBenchDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchView message handlers
