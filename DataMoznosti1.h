#if !defined(AFX_DATAMOZNOSTI1_H__86421D88_CF5F_4B3A_85E9_19578116DEC8__INCLUDED_)
#define AFX_DATAMOZNOSTI1_H__86421D88_CF5F_4B3A_85E9_19578116DEC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMoznosti1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti1 dialog

class CDataMoznosti1 : public CPropertyPage
{
// Construction
public:
	CDataMoznosti1();

// Dialog Data
	//{{AFX_DATA(CDataMoznosti1)
	enum { IDD = IDD_DATA_MOZNOSTI_1 };
	CButton	m_box13;
	CButton	m_box12;
	CButton	m_box11;
	CButton	m_box1;
	CButton	m_radio_vlnitost;
	CButton	m_radio_drsnost;
	CButton	m_radio_trojuhel;
	CButton	m_radio_gauss;
	CEdit	m_edit_delka;
	CButton	m_radio_008;
	CButton	m_radio_025;
	CButton	m_radio_08;
	CButton	m_radio_25;
	CButton	m_radio_8;
	CButton	m_radio_zadej;
	CButton	m_check_filtr;
	BOOL	m_bFiltr;
	double	m_dDelka;
	int		m_iVlnitost;
	int		m_iDruh;
	int		m_iDelkaTyp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMoznosti1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMoznosti1)
	afx_msg void OnFiltr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMOZNOSTI1_H__86421D88_CF5F_4B3A_85E9_19578116DEC8__INCLUDED_)
