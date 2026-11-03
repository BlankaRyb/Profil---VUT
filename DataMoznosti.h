#if !defined(AFX_DATAMOZNOSTI_H__84DBB41C_514D_49D4_965B_42F62E733D85__INCLUDED_)
#define AFX_DATAMOZNOSTI_H__84DBB41C_514D_49D4_965B_42F62E733D85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMoznosti.h : header file
//

#include "DataMoznosti0.h"
#include "DataMoznosti1.h"
#include "DataMoznosti2.h"
#include "DataMoznosti3.h"
#include "DataMoznosti4.h"

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti dialog

class CDataMoznosti : public CDialog
{
// Construction
public:
	CDataMoznosti0 dlgMoznosti0;
	CDataMoznosti1 dlgMoznosti1;
	CDataMoznosti2 dlgMoznosti2;
	CDataMoznosti3 dlgMoznosti3;
	CDataMoznosti4 dlgMoznosti4;
	BOOL           m_FiltrA;

	CDataMoznosti(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataMoznosti)
	enum { IDD = IDD_DATA_MOZNOSTI };
	CTabCtrl	m_CtrlTab;
	//}}AFX_DATA
//	CButton	m_check_filtr;???


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMoznosti)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMoznosti)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void Filtr();

public:
	void GetFiltr(BOOL bFiltr); //nastavi m_FiltrA

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMOZNOSTI_H__84DBB41C_514D_49D4_965B_42F62E733D85__INCLUDED_)
