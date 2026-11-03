// Profil.h : main header file for the PROFIL application
//

#if !defined(AFX_PROFIL_H__2006842A_6E98_4230_AE1C_2D6F86B5EDB6__INCLUDED_)
#define AFX_PROFIL_H__2006842A_6E98_4230_AE1C_2D6F86B5EDB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "const.h"

/////////////////////////////////////////////////////////////////////////////
// CProfilApp:
// See Profil.cpp for the implementation of this class
//

class CProfilApp : public CWinApp
{
public:
	CProfilApp();
	CString GetFilePath();
	void SetFilePath(CString sFilePath);
	CString GetFileName();
	void SetFileName(CString sFilePath);
	double GetProcentaKorel();
	void SetProcentaKorel(double iKorel);
	double GetFrekvence();
	void SetFrekvence(double dFrek);
	COLORREF GetColor(int i);
	void SetColor(int i, COLORREF col);
	BOOL UrciTyp (CString str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfilApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL SaveAllModified();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CProfilApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL OpenIniFile(); //otevre profil.ini, pokud neexistuje vytvori nový
	COLORREF SetColor(CString str); //vrati barvu (str je ve tvaru 0,0,0 (R,G,B))
	CString SetStr_Color(COLORREF col);

protected:
	CString  m_sFilePath;
	CString  m_sFileName;
	double   m_dKorel; //procenta pro korelacni funkci vice nez 0 maximalne 100%, 100% cela osa x, 0% nesmysl zadna osa x  
	double   m_dFrek;  //frekvence pro Vykonove spektrum
	COLORREF m_clrTrueMap[COUNT_COL];  //Aktualni RGB pro jednotlive prvky v grafech
};

extern CProfilApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFIL_H__2006842A_6E98_4230_AE1C_2D6F86B5EDB6__INCLUDED_)
