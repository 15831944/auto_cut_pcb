// MotorControlBench.h : main header file for the MOTORCONTROLBENCH application
//

#include "CIniFile.h"	// Added by ClassView
#if !defined(AFX_MOTORCONTROLBENCH_H__D1061071_9B16_4D22_AEF5_8EB3052071DA__INCLUDED_)
#define AFX_MOTORCONTROLBENCH_H__D1061071_9B16_4D22_AEF5_8EB3052071DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "CIniFile.h"
#include "global.h"
/////////////////////////////////////////////////////////////////////////////
// CMotorControlBenchApp:
// See MotorControlBench.cpp for the implementation of this class
//

#define DEFAULT_INI_FILE ".\\default.ini"
//#define SV_CARD 0
//#define SG_CARD 1


class CMotorControlBenchApp : public CWinApp
{
public:
	bool RegisterReg();
	int m_nAxisPPR[4];
	int m_nAxisCount;
	bool m_bSetupUpdated;
	short m_nKd[4];
	short m_nKi[4];
	short m_nKp[4];
	int m_nCardType;
	int m_nAxis;
	int m_nDirPls[4];
	int m_nCtrlMode[4];
	double m_fG00Acc,m_fG01Acc;
	double m_fG00Spd,m_fG01Spd;
	double m_fHomeVel[4];
	double m_fHomeAcc[4];
	int m_nHomeStyle[4];


	bool LoadIniFile();
	CIniFile m_inifile;
	int nViewPlnSel;
	CMotorControlBenchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotorControlBenchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMotorControlBenchApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTORCONTROLBENCH_H__D1061071_9B16_4D22_AEF5_8EB3052071DA__INCLUDED_)
bool ProcessMessage();




















bool ProcessMessage();