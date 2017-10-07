// Interpolation.h: interface for the CInterpolation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERPOLATION_H__C6AE3C14_19C4_44EB_8C89_6BD50893DF7A__INCLUDED_)
#define AFX_INTERPOLATION_H__C6AE3C14_19C4_44EB_8C89_6BD50893DF7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/****************************************************************************/
/*  ��  ��: CInterpolation													*/
/*  ��  ��: �岹��											                */
/*  ˵	��:														     		*/
/*            1.��װMIT,DDA �Ȳ岹����									 	*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
/****************************************************************************/
class CInterpolation  
{
public:
	void CircleInterp_DDA_Buffer(unsigned short n,int step,long xSt,long ySt,long lXEnd, long lYEnd,long radius,bool bIsCW);
	void Start_LineInterp(long xSt,long ySt);
	void End_Interpolation();
	void CI_DDA_All(int nXRes,int nYRes,int nZRes,long xEnd,long yEnd,long zEnd);
	void LineInterp_DDA_Buffer(int n,int step,long x,long y,long z);
	void LineInterp_MIT_Buffer(int step, long XEnd, long YEnd);
	void CirInterp_MIT_Buffer(long step, long XStart, long YStart, long XEnd, long YEnd,long radius,bool bIsCW);
	void Axis_Step(int nAxis, int nStep);
	unsigned short Judge_Quadrant(long x,long y);
	void CirInterp_MIT(long step, long XStart, long YStart, long XEnd, long YEnd,long radius,bool bIsCW);
	void LineInterp_MIT(int step,long XEnd,long YEnd);//���ȽϷ�ֱ�߲岹
	CInterpolation();
	virtual ~CInterpolation();

};

#endif // !defined(AFX_INTERPOLATION_H__C6AE3C14_19C4_44EB_8C89_6BD50893DF7A__INCLUDED_)
	