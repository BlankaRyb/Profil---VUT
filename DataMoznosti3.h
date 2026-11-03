#if !defined(AFX_DATAMOZNOSTI3_H__8D7AD91C_E4D7_4E7B_8B5B_B9B35935E6E5__INCLUDED_)
#define AFX_DATAMOZNOSTI3_H__8D7AD91C_E4D7_4E7B_8B5B_B9B35935E6E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMoznosti3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti3 dialog

class CDataMoznosti3 : public CPropertyPage
{
// Construction
public:
	CDataMoznosti3();

// Dialog Data
	//{{AFX_DATA(CDataMoznosti3)
	enum { IDD = IDD_DATA_MOZNOSTI_3 };
	double	m_dFrek;
	double	m_dOmega;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMoznosti3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMoznosti3)
	afx_msg void OnChangeFrekvence();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMOZNOSTI3_H__8D7AD91C_E4D7_4E7B_8B5B_B9B35935E6E5__INCLUDED_)
