#if !defined(AFX_DATAMOZNOSTI0_H__336D443D_2432_4D55_9B91_73B7685AE2A4__INCLUDED_)
#define AFX_DATAMOZNOSTI0_H__336D443D_2432_4D55_9B91_73B7685AE2A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMoznosti0.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti0 dialog

class CDataMoznosti0 : public CPropertyPage
{
// Construction
public:
	CDataMoznosti0();

// Dialog Data
	//{{AFX_DATA(CDataMoznosti0)
	enum { IDD = IDD_DATA_MOZNOSTI_0 };
	CButton	m_check_filtr;
	int		m_moznosti;
	BOOL	m_bFiltr;
	int		m_iKrok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMoznosti0)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMoznosti0)
	afx_msg void OnProfil();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMOZNOSTI0_H__336D443D_2432_4D55_9B91_73B7685AE2A4__INCLUDED_)
