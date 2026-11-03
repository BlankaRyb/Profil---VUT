// DataMoznosti1.cpp : implementation file
//

#include "stdafx.h"
#include "profil.h"
#include "DataMoznosti1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti1 dialog


CDataMoznosti1::CDataMoznosti1()
	: CPropertyPage(CDataMoznosti1::IDD)
{
	//{{AFX_DATA_INIT(CDataMoznosti1)
	m_bFiltr = FALSE;
	//}}AFX_DATA_INIT
}


void CDataMoznosti1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMoznosti1)
	DDX_Radio(pDX, IDC_VLNITOST, m_iVlnitost);
	DDX_Radio(pDX, IDC_TROJUHEL, m_iDruh);
	DDX_Control(pDX, IDC_BOX13, m_box13);
	DDX_Control(pDX, IDC_BOX12, m_box12);
	DDX_Control(pDX, IDC_BOX11, m_box11);
	DDX_Control(pDX, IDC_BOX1, m_box1);
	DDX_Control(pDX, IDC_VLNITOST, m_radio_vlnitost);
	DDX_Control(pDX, IDC_DRSNOST, m_radio_drsnost);
	DDX_Control(pDX, IDC_TROJUHEL, m_radio_trojuhel);
	DDX_Control(pDX, IDC_GAUSS, m_radio_gauss);
	DDX_Control(pDX, IDC_DELKA, m_edit_delka);
	DDX_Control(pDX, IDC_008, m_radio_008);
	DDX_Control(pDX, IDC_025, m_radio_025);
	DDX_Control(pDX, IDC_08, m_radio_08);
	DDX_Control(pDX, IDC_25, m_radio_25);
	DDX_Control(pDX, IDC_8, m_radio_8);
	DDX_Control(pDX, IDC_ZADEJ, m_radio_zadej);
	DDX_Control(pDX, IDC_FILTR, m_check_filtr);
	DDX_Check(pDX, IDC_FILTR, m_bFiltr);
	DDX_Radio(pDX, IDC_008, m_iDelkaTyp);
	DDX_Text(pDX, IDC_DELKA, m_dDelka);
	DDV_MinMaxDouble(pDX, m_dDelka, 0., 1000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataMoznosti1, CDialog)
	//{{AFX_MSG_MAP(CDataMoznosti1)
	ON_BN_CLICKED(IDC_FILTR, OnFiltr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti1 message handlers

void CDataMoznosti1::OnFiltr() 
{
	UpdateData(TRUE);

	if (m_bFiltr)//zaput
	{
		m_radio_vlnitost.EnableWindow(TRUE);
		m_radio_drsnost.EnableWindow(TRUE);
		m_radio_trojuhel.EnableWindow(TRUE);
		m_radio_gauss.EnableWindow(TRUE);
		m_radio_008.EnableWindow(TRUE);
		m_radio_025.EnableWindow(TRUE);
		m_radio_08.EnableWindow(TRUE);
		m_radio_25.EnableWindow(TRUE);
		m_radio_8.EnableWindow(TRUE);
		m_radio_zadej.EnableWindow(TRUE);
		m_edit_delka.EnableWindow(TRUE);
		m_box1.EnableWindow(TRUE);
		m_box11.EnableWindow(TRUE);
		m_box12.EnableWindow(TRUE);
		m_box13.EnableWindow(TRUE);
	}
	else//Filtr vypnut
	{
		m_radio_vlnitost.EnableWindow(FALSE);
		m_radio_drsnost.EnableWindow(FALSE);
		m_radio_trojuhel.EnableWindow(FALSE);
		m_radio_gauss.EnableWindow(FALSE);
		m_radio_008.EnableWindow(FALSE);
		m_radio_025.EnableWindow(FALSE);
		m_radio_08.EnableWindow(FALSE);
		m_radio_25.EnableWindow(FALSE);
		m_radio_8.EnableWindow(FALSE);
		m_radio_zadej.EnableWindow(FALSE);
		m_edit_delka.EnableWindow(FALSE);
		m_box1.EnableWindow(FALSE);
		m_box11.EnableWindow(FALSE);
		m_box12.EnableWindow(FALSE);
		m_box13.EnableWindow(FALSE);
	}
	Invalidate();
}
