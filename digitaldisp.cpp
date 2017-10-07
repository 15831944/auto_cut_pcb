// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "digitaldisp.h"

/////////////////////////////////////////////////////////////////////////////
// CDigitalDisp

IMPLEMENT_DYNCREATE(CDigitalDisp, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CDigitalDisp properties

OLE_COLOR CDigitalDisp::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CDigitalDisp::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

OLE_COLOR CDigitalDisp::GetForeColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
	return result;
}

void CDigitalDisp::SetForeColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

double CDigitalDisp::GetNumToDisp()
{
	double result;
	GetProperty(0x1, VT_R8, (void*)&result);
	return result;
}

void CDigitalDisp::SetNumToDisp(double propVal)
{
	SetProperty(0x1, VT_R8, propVal);
}

long CDigitalDisp::GetNumAfterDot()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CDigitalDisp::SetNumAfterDot(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long CDigitalDisp::GetNumBeforeDot()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void CDigitalDisp::SetNumBeforeDot(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CDigitalDisp operations

void CDigitalDisp::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
