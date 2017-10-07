// CirBuffer.h: interface for the CBufPtr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRBUFFER_H__701F3861_C33B_11D7_9CBF_5254AB25C1AA__INCLUDED_)
#define AFX_CIRBUFFER_H__701F3861_C33B_11D7_9CBF_5254AB25C1AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CNCData.h"

#define BUF_LENGTH  2000
class CBufPtr  
{
public:
	void Clear();
	CBufPtr();
	virtual ~CBufPtr();
	bool SeekActLine(unsigned short line,long& AtlLine);
	void SaveLinePos(long l);
private:
	POINT d[BUF_LENGTH];//X is index,Y is actline
	int i;//index
	unsigned short pLn;
};

#endif // !defined(AFX_CIRBUFFER_H__701F3861_C33B_11D7_9CBF_5254AB25C1AA__INCLUDED_)
