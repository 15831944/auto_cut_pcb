// CirBuffer.cpp: implementation of the CBufPtr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BufPtr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBufPtr::CBufPtr()
{
	Clear();
}

CBufPtr::~CBufPtr()
{
}

bool  CBufPtr::SeekActLine(unsigned short ln,long& AtlLine)
{
	unsigned short n=0;
	AtlLine=0;
	for(n=0;n<BUF_LENGTH;n++)
	{
		if(d[n].x==ln)
		{
			AtlLine=d[n].y;
			return true;
		}
	}
	return false;
}

void CBufPtr::SaveLinePos(long l)
{//1----32767
	pLn++;
	if(pLn>65535)
		pLn=1;
	d[i].x=pLn;
	d[i].y=l;
	i++;
	if(i>=BUF_LENGTH)i=0;
}

void CBufPtr::Clear()
{
	for(i=0;i<BUF_LENGTH;i++)d[i].x=d[i].y=0;
	i=pLn=0;
}
