//Position.h
#ifndef _INSIDE_VISUAL_CPP_POSITION
#define _INSIDE_VISUAL_CPP_POSITION
class CPOSITION:public CObject
{
//	DECLARE_DYNAMIC(CPOSITION)
	DECLARE_SERIAL(CPOSITION)
public:
	long x;
	long y;
	long z;
	CPOSITION()
	{
		x=0;
		y=0;
		z=0;
	}
	virtual void Serialize(CArchive& ar);
#ifdef _DEBUG
	void Dump(CDumpContext& dc) const;
#endif//_DEBUG
};

#endif//
typedef CTypedPtrList<CObList,CPOSITION*> CPOSITIONList;