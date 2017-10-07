#if !defined(AFX_GCODEDRAWVIEW_H__EF342D62_0E7F_433D_8CE2_6B5C93E9673F__INCLUDED_)
#define AFX_GCODEDRAWVIEW_H__EF342D62_0E7F_433D_8CE2_6B5C93E9673F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GCodeDrawView.h : header file
//
#include "MyDataType.h"

/////////////////////////////////////////////////////////////////////////////

/****************************************************************************/
/*  �ṹ��: TCenRec															*/
/*  ��  ��: �岹Բ������ֵ�ṹ��							                */
/*  ��  ��:														     		*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
/****************************************************************************/
typedef struct TCenRec {
    double I,J,K;
} CenRec;

/****************************************************************************/
/*  �ṹ��: TAngle															*/
/*  ��  ��: �岹Բ���ǶȽṹ��								                */
/*  ��  ��:														     		*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
/****************************************************************************/
typedef struct TAngle {
    double MR,MTH,MTHC;
} StuAngle;

#ifndef NOVALIDDATA
#define NOVALIDDATA        1000000
#endif

#define INITVALUE    (NOVALIDDATA+1)
#define MDivide      30

#ifndef  PI
#define PI 3.1415926
#endif


/****************************************************************************/
/*  ��  ��: CGCodeDrawView													*/
/*  ��  ��: G����켣��ʾ��ͼ��								                */
/*  ˵	��:														     		*/
/*            1.ʵ��G����켣��ģ���ʵʱ��ʾ						    	*/
/*  ��  ��:	  1.0											                */
/*  �޸�����:  20006-11-20	                                                */
/*  �޶���:    Long TY					                                    */
/****************************************************************************/
// CGCodeDrawView view

class CGCodeDrawView : public CView
{
public:
    CGCodeDrawView();   
	virtual ~CGCodeDrawView();
protected:
        // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGCodeDrawView)
public:
	// Operations
	void	DrawGImage(CDC *);
	void	SetCurCoord(double,double,double);
	double	GetCurAngle(int angle);
    double	GetCurRatio(double scale);
	void	GetCurPostion();
	short	IN_Center(double xs,double ys,double *xe,double *ye,double r,double *xc,double *yc,int cw);
	void	ChangeViewPoint();
	void	DrawCourse(double X,double Y,double Z);	
    void DrawWorkLine(int bIsFirst); 
	StuAngle CalculateAngel (double Xs,double Ys,double Xe,double Ye,double I,double J,int GCode);
    CenRec	CalculateCenter(double Xs,double Ys,double Xe,double Ye,double R,int mode);
	
public:	
	// Attributes
	double m_scale;	
	bool	bReDraw;
	CRect	bodyCanvas;
	int m_xMap;
	int m_yMap;
	int m_zMap;
    double	drawRatio;
	// Attributes
	CMetaFileDC	*pMetaFileDC; 
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
	
	// Implementation
private:
	// Attributes
	int		zeroposx,zeroposy;  //ԭ������
	CPoint	orgpoint;
	int		ViewHigh,ViewWidth;  //��ͼ�ߡ���
	int		OriginX,OriginY;
	CPoint  ViewOrg;
	double	lastX,lastY,lastZ;
	HPEN    curosrPen1,curosrPen2;
	HPEN    moveDrawPen,workDrawPen;
	bool    bRequireMove;//�Ƿ���ʾ��ͼ���	T=����ʾ F=��ʾ
	bool    bdrawing;
	double  pos[3];
	double	PicZoomWidth,PicZoomHeight;
	int		X_Ror_Angle,Z_Ror_Angle;
	CMetaFileDC* ReplacementMetaFile;
public:

protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGCodeDrawView)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCODEDRAWVIEW_H__EF342D62_0E7F_433D_8CE2_6B5C93E9673F__INCLUDED_)
