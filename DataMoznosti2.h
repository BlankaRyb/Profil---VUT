#if !defined(AFX_DATAMOZNOSTI2_H__240AEDE2_FA54_4631_9229_8EEECC7501DD__INCLUDED_)
#define AFX_DATAMOZNOSTI2_H__240AEDE2_FA54_4631_9229_8EEECC7501DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMoznosti2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti2 dialog

class CDataMoznosti2 : public CPropertyPage
{
// Construction
public:
	CDataMoznosti2();

// Dialog Data
	//{{AFX_DATA(CDataMoznosti2)
	enum { IDD = IDD_DATA_MOZNOSTI_2 };
	double	m_dKorel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMoznosti2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMoznosti2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMOZNOSTI2_H__240AEDE2_FA54_4631_9229_8EEECC7501DD__INCLUDED_)
