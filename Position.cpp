#include "stdafx.h"
#include "Position.h"
//IMPLEMENT_DYNAMIC(CPOSITION,CObject)
IMPLEMENT_SERIAL(CPOSITION,CObject,0)
#ifdef _DEBUG
void CPOSITION::Dump(CDumpContext&dc) const
{
	CObject::Dump(dc);
	dc<<"x="<<x<<"\n y="<<y<<"\n z="<<z;

}
#endif//_DEBUG
void CPOSITION::Serialize(CArchive& ar)
{
	TRACE("Entering CPOSITION::Serialize\n");
   if (ar.IsStoring())
	{
     ar<<x<<y<<z;
	}
	else
	{
     ar>>x>>y>>z;
	}
}