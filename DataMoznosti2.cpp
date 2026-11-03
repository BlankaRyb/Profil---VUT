// DataMoznosti2.cpp : implementation file
//

#include "stdafx.h"
#include "profil.h"
#include "DataMoznosti2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti2 dialog


CDataMoznosti2::CDataMoznosti2()
	: CPropertyPage(CDataMoznosti2::IDD)
{
	//{{AFX_DATA_INIT(CDataMoznosti2)
	m_dKorel = 0.0;
	//}}AFX_DATA_INIT
}


void CDataMoznosti2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMoznosti2)
	DDX_Text(pDX, IDC_KOREL, m_dKorel);
	DDV_MinMaxDouble(pDX, m_dKorel, 1.e-004, 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataMoznosti2, CDialog)
	//{{AFX_MSG_MAP(CDataMoznosti2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti2 message handlers
