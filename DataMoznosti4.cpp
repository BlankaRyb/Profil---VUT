// DataMoznosti4.cpp : implementation file
//

#include "stdafx.h"
#include "profil.h"
#include "DataMoznosti4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti4 dialog


CDataMoznosti4::CDataMoznosti4()
	: CPropertyPage(CDataMoznosti4::IDD)
{
	//{{AFX_DATA_INIT(CDataMoznosti4)
	m_Datum = "";
	//popis
	m_Popis = "";
	//datum
	m_sTypPrvku = "";	//}}AFX_DATA_INIT
}


void CDataMoznosti4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMoznosti4)
	DDX_Text(pDX, IDC_DATUM, m_Datum);
	DDV_MaxChars(pDX, m_sTypPrvku, 30);
	DDX_Text(pDX, IDC_POPIS, m_Popis);
	DDV_MaxChars(pDX, m_Popis, 10000);
	DDX_Text(pDX, IDC_TYPPRVKU, m_sTypPrvku);
	DDV_MaxChars(pDX, m_sTypPrvku, 200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataMoznosti4, CDialog)
	//{{AFX_MSG_MAP(CDataMoznosti4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti4 message handlers

BOOL CDataMoznosti4::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
