// DataMoznosti3.cpp : implementation file
//

#include "stdafx.h"
#include "profil.h"
#include "DataMoznosti3.h"
#include "Const.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti3 dialog


CDataMoznosti3::CDataMoznosti3()
	: CPropertyPage(CDataMoznosti3::IDD)
{
	//{{AFX_DATA_INIT(CDataMoznosti3)
	m_dOmega = 0.0;
	//}}AFX_DATA_INIT
}


void CDataMoznosti3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMoznosti3)
	DDX_Text(pDX, IDC_FREKVENCE, m_dFrek);
	DDV_MinMaxDouble(pDX, m_dFrek, 0., 1000.);
	DDX_Text(pDX, IDC_OMEGA, m_dOmega);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataMoznosti3, CDialog)
	//{{AFX_MSG_MAP(CDataMoznosti3)
	ON_EN_CHANGE(IDC_FREKVENCE, OnChangeFrekvence)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti3 message handlers

void CDataMoznosti3::OnChangeFrekvence() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData(TRUE);
	m_dOmega = 2*PI*m_dFrek;
	UpdateData(FALSE);
	
}

BOOL CDataMoznosti3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dOmega = 2*PI*m_dFrek;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
