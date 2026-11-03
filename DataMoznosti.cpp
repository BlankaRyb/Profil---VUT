// DataMoznosti.cpp : implementation file
//

#include "stdafx.h"
#include "Profil.h"
#include "DataMoznosti.h"
#include "const.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti dialog


CDataMoznosti::CDataMoznosti(CWnd* pParent /*=NULL*/)
	: CDialog(CDataMoznosti::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataMoznosti)
	//}}AFX_DATA_INIT
	dlgMoznosti0.m_moznosti = -1;
	dlgMoznosti0.m_bFiltr = dlgMoznosti1.m_bFiltr = m_FiltrA = FALSE;
	dlgMoznosti1.m_dDelka = 0.0;
	dlgMoznosti1.m_iVlnitost = -1;
	dlgMoznosti1.m_iDruh = -1;
	dlgMoznosti1.m_iDelkaTyp = -1;
	dlgMoznosti0.m_iKrok = 1;
	dlgMoznosti3.m_dFrek = 0.0;
}

void CDataMoznosti::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMoznosti)
	DDX_Control(pDX, IDC_TAB1, m_CtrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataMoznosti, CDialog)
	//{{AFX_MSG_MAP(CDataMoznosti)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab1)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMoznosti message handlers

BOOL CDataMoznosti::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rc, drc;
	m_CtrlTab.GetClientRect(rc);
	CString str;

	//Zaloky v dialogu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	dlgMoznosti0.Create(CDataMoznosti0::IDD, &m_CtrlTab);
	dlgMoznosti0.GetClientRect(drc);
	dlgMoznosti0.SetWindowPos(0,4,27,rc.Width()-10,rc.Height()-30,SWP_NOZORDER);
	dlgMoznosti1.Create(CDataMoznosti1::IDD, &m_CtrlTab);
	dlgMoznosti1.SetWindowPos(0,4,27,rc.Width()-10,rc.Height()-30,SWP_NOZORDER);
	dlgMoznosti2.Create(CDataMoznosti2::IDD, &m_CtrlTab);
	dlgMoznosti2.SetWindowPos(0,4,27,rc.Width()-10,rc.Height()-30,SWP_NOZORDER);
	dlgMoznosti3.Create(CDataMoznosti3::IDD, &m_CtrlTab);
	dlgMoznosti3.SetWindowPos(0,4,27,rc.Width()-10,rc.Height()-30,SWP_NOZORDER);
	dlgMoznosti4.Create(CDataMoznosti4::IDD, &m_CtrlTab);
	dlgMoznosti4.SetWindowPos(0,4,27,rc.Width()-10,rc.Height()-30,SWP_NOZORDER);

	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	str.LoadString(IDS_TABCTRL0);
	TabCtrlItem.pszText = str.GetBuffer(IDS_TABCTRL0);
	m_CtrlTab.InsertItem( 0, &TabCtrlItem );

	str.LoadString(IDS_TABCTRL1);
	TabCtrlItem.pszText = str.GetBuffer(IDS_TABCTRL1);
	m_CtrlTab.InsertItem( 1, &TabCtrlItem );

	str.LoadString(IDS_TABCTRL2);
	TabCtrlItem.pszText = str.GetBuffer(IDS_TABCTRL2);
	m_CtrlTab.InsertItem( 2, &TabCtrlItem );
	
	str.LoadString(IDS_TABCTRL3);
	TabCtrlItem.pszText = str.GetBuffer(IDS_TABCTRL3);
	m_CtrlTab.InsertItem( 3, &TabCtrlItem );

	str.LoadString(IDS_TABCTRL4);
	TabCtrlItem.pszText = str.GetBuffer(IDS_TABCTRL4);
	m_CtrlTab.InsertItem( 4, &TabCtrlItem );

	dlgMoznosti0.ShowWindow(SW_SHOW);
	//Zaloky v dialogu <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//#define GfxSetTabDialog(numDialog, pDialog)	__gfxTabDialog[numDialog] = (CDialog *) pDialog;
//	GfxSetTabDialog(0, &dlgSheet1);
//	GfxSetTabDialog(1, &dlgSheet2);

	if (!dlgMoznosti0.m_bFiltr)
	{
		dlgMoznosti1.m_radio_vlnitost.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_drsnost.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_trojuhel.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_gauss.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_008.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_025.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_08.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_25.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_8.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_zadej.EnableWindow(FALSE);
		dlgMoznosti1.m_edit_delka.EnableWindow(FALSE);
		dlgMoznosti1.m_box1.EnableWindow(FALSE);
		dlgMoznosti1.m_box11.EnableWindow(FALSE);
		dlgMoznosti1.m_box12.EnableWindow(FALSE);
		dlgMoznosti1.m_box13.EnableWindow(FALSE);
	}
	
	//Zaloky v dialogu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	dlgMoznosti0.UpdateData(FALSE);
	dlgMoznosti1.UpdateData(FALSE);
	dlgMoznosti2.UpdateData(FALSE);
	dlgMoznosti3.UpdateData(FALSE);
	dlgMoznosti4.UpdateData(FALSE);
	//Zaloky v dialogu <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataMoznosti::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	__int32 iTab = m_CtrlTab.GetCurSel();

	switch(iTab)
	{
	case 0:
		dlgMoznosti0.ShowWindow(SW_SHOW);
		dlgMoznosti1.ShowWindow(SW_HIDE);
		dlgMoznosti2.ShowWindow(SW_HIDE);
		dlgMoznosti3.ShowWindow(SW_HIDE);
		dlgMoznosti4.ShowWindow(SW_HIDE);
		break;
	case 1:
		dlgMoznosti0.ShowWindow(SW_HIDE);
		dlgMoznosti1.ShowWindow(SW_SHOW);
		dlgMoznosti2.ShowWindow(SW_HIDE);
		dlgMoznosti3.ShowWindow(SW_HIDE);
		dlgMoznosti4.ShowWindow(SW_HIDE);
		break;
	case 2:
		dlgMoznosti0.ShowWindow(SW_HIDE);
		dlgMoznosti1.ShowWindow(SW_HIDE);
		dlgMoznosti2.ShowWindow(SW_SHOW);
		dlgMoznosti3.ShowWindow(SW_HIDE);
		dlgMoznosti4.ShowWindow(SW_HIDE);
		break;
	case 3:
		dlgMoznosti0.ShowWindow(SW_HIDE);
		dlgMoznosti1.ShowWindow(SW_HIDE);
		dlgMoznosti2.ShowWindow(SW_HIDE);
		dlgMoznosti3.ShowWindow(SW_SHOW);
		dlgMoznosti4.ShowWindow(SW_HIDE);
		break;
	case 4:
		dlgMoznosti0.ShowWindow(SW_HIDE);
		dlgMoznosti1.ShowWindow(SW_HIDE);
		dlgMoznosti2.ShowWindow(SW_HIDE);
		dlgMoznosti3.ShowWindow(SW_HIDE);
		dlgMoznosti4.ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}

void CDataMoznosti::OnSelchangingTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	dlgMoznosti0.UpdateData(TRUE);
	dlgMoznosti1.UpdateData(TRUE);
	//propojeni check bootonu fitru v 1. a 2. zalozce
	if (m_FiltrA != dlgMoznosti0.m_bFiltr)
	{
		dlgMoznosti1.m_bFiltr = m_FiltrA = dlgMoznosti0.m_bFiltr;
		Filtr();//zadiseblovani oddiseblovani polozek filtru
		dlgMoznosti1.UpdateData(FALSE);
	}
	if (m_FiltrA != dlgMoznosti1.m_bFiltr)
	{
		dlgMoznosti0.m_bFiltr = m_FiltrA = dlgMoznosti1.m_bFiltr;
		Filtr();//zadiseblovani oddiseblovani polozek filtru
		dlgMoznosti0.UpdateData(FALSE);
	}

	*pResult = 0;
}

void CDataMoznosti::Filtr()
{
	if (dlgMoznosti0.m_bFiltr)
	{
		dlgMoznosti1.m_radio_vlnitost.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_drsnost.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_trojuhel.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_gauss.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_008.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_025.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_08.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_25.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_8.EnableWindow(TRUE);
		dlgMoznosti1.m_radio_zadej.EnableWindow(TRUE);
		dlgMoznosti1.m_edit_delka.EnableWindow(TRUE);
		dlgMoznosti1.m_box1.EnableWindow(TRUE);
		dlgMoznosti1.m_box11.EnableWindow(TRUE);
		dlgMoznosti1.m_box12.EnableWindow(TRUE);
		dlgMoznosti1.m_box13.EnableWindow(TRUE);
	}
	else
	{
		dlgMoznosti1.m_radio_vlnitost.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_drsnost.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_trojuhel.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_gauss.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_008.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_025.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_08.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_25.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_8.EnableWindow(FALSE);
		dlgMoznosti1.m_radio_zadej.EnableWindow(FALSE);
		dlgMoznosti1.m_edit_delka.EnableWindow(FALSE);
		dlgMoznosti1.m_box1.EnableWindow(FALSE);
		dlgMoznosti1.m_box11.EnableWindow(FALSE);
		dlgMoznosti1.m_box12.EnableWindow(FALSE);
		dlgMoznosti1.m_box13.EnableWindow(FALSE);
	}
}

void CDataMoznosti::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);

	dlgMoznosti0.UpdateData(TRUE);
	dlgMoznosti1.UpdateData(TRUE);
	dlgMoznosti2.UpdateData(TRUE);
	dlgMoznosti3.UpdateData(TRUE);
	dlgMoznosti4.UpdateData(TRUE);

	__int32 iTab = m_CtrlTab.GetCurSel();
	
	//pokud je otevren dlgMoznosti1, tak je potreba
	//nasteveni fitru dat i do dlgMoznosti0.m_bFiltr.
	if (iTab == 1)
	{
		dlgMoznosti0.m_bFiltr = dlgMoznosti1.m_bFiltr;
	}

}

void CDataMoznosti::GetFiltr(BOOL bFiltr) //nastavi m_FiltrA
{
	m_FiltrA = bFiltr;
	dlgMoznosti0.m_bFiltr = bFiltr;
}
