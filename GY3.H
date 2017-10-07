// GY3.h: interface for the CCNCData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GY3_H__7BEE0D61_AB9E_11D7_9CBD_5254AB25C1AA__INCLUDED_)
#define AFX_GY3_H__7BEE0D61_AB9E_11D7_9CBD_5254AB25C1AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCNCData  
{
public:
	CCNCData();
	virtual ~CCNCData();
public:
	void Clear();
	void Copy(const CCNCData &);
	bool operator ==(CCNCData);

	//char * gcode;
	DWORD	N;
 	BYTE    Code;
    double 	x,y,z,r,rcx,rcy;
	double  v;
	double f;
};

struct COMPLEX
	{double Re,Im;};
#endif // !defined(AFX_GY3_H__7BEE0D61_AB9E_11D7_9CBD_5254AB25C1AA__INCLUDED_)
