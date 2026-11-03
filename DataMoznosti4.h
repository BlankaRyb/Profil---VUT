#if !defined(AFX_DATAMOZNOSTI4_H__5C63AB1F_7FD8_41A9_BF6C_B7087A75443C__INCLUDED_)
#define AFX_DATAMOZNOSTI4_H__5C63AB1F_7FD8_41A9_BF6C_B7087A75443C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMoznosti4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti4 dialog

class CDataMoznosti4 : public CPropertyPage
{
// Construction
public:
	CDataMoznosti4();

// Dialog Data
	//{{AFX_DATA(CDataMoznosti4)
	enum { IDD = IDD_DATA_MOZNOSTI_4 };
	CString	m_Datum;
	CString	m_Popis;
	CString	m_sTypPrvku;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMoznosti4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMoznosti4)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMOZNOSTI4_H__5C63AB1F_7FD8_41A9_BF6C_B7087A75443C__INCLUDED_)
