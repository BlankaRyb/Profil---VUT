// DataMoznosti0.cpp : implementation file
//

#include "stdafx.h"
#include "profil.h"
#include "DataMoznosti0.h"
#include "const.h"
#include "DataMoznosti.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti0 dialog


CDataMoznosti0::CDataMoznosti0()
	: CPropertyPage(CDataMoznosti0::IDD)
{
	//{{AFX_DATA_INIT(CDataMoznosti0)
	m_moznosti = -1;
	m_bFiltr = FALSE;
	//}}AFX_DATA_INIT
}


void CDataMoznosti0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMoznosti0)
	DDX_Control(pDX, IDC_FILTR, m_check_filtr);
	DDX_Radio(pDX, IDC_RADIO1, m_moznosti);
	DDX_Check(pDX, IDC_FILTR, m_bFiltr);
	DDX_Text(pDX, IDC_KROK, m_iKrok);
	DDV_MinMaxInt(pDX, m_iKrok, 1, 10000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataMoznosti0, CDialog)
	//{{AFX_MSG_MAP(CDataMoznosti0)
	ON_BN_CLICKED(IDC_RADIO1, OnProfil)
	ON_BN_CLICKED(IDC_RADIO2, OnProfil)
	ON_BN_CLICKED(IDC_RADIO3, OnProfil)
	ON_BN_CLICKED(IDC_RADIO4, OnProfil)
	ON_BN_CLICKED(IDC_RADIO5, OnProfil)
	ON_BN_CLICKED(IDC_RADIO6, OnProfil)
	ON_BN_CLICKED(IDC_RADIO7, OnProfil)
	ON_BN_CLICKED(IDC_RADIO8, OnProfil)
	ON_BN_CLICKED(IDC_RADIO9, OnProfil)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti0 message handlers

void CDataMoznosti0::OnProfil() 
{
	UpdateData(TRUE);	
}
