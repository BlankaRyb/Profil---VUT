// SetColor.cpp : implementation file
//

#include "stdafx.h"
#include "profil.h"
#include "SetColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetColor dialog


CSetColor::CSetColor(CWnd* pParent /*=NULL*/)
	: CDialog(CSetColor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetColor)
	m_radColor = -1;
	m_iRedIndex = 0;
	m_iGreenIndex = 0;
	m_iBlueIndex = 0;
	//}}AFX_DATA_INIT
	m_hColorBrush = CreateSolidBrush(RGB(255,0,0));
}


void CSetColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetColor)
	DDX_Control(pDX, IDC_COL_POZADI, m_colPozadi);
	DDX_Control(pDX, IDC_COL_PROFIL, m_colProfil);
	DDX_Control(pDX, IDC_COL_OSY, m_colOsy);
	DDX_Control(pDX, IDC_COL_FILTR, m_colFiltr);
	DDX_Radio(pDX, IDC_RAD_POZADI, m_radColor);
	DDX_Text(pDX, IDC_RED_INDEX, m_iRedIndex);
	DDV_MinMaxInt(pDX, m_iRedIndex, 0, 255);
	DDX_Text(pDX, IDC_GREEN_INDEX, m_iGreenIndex);
	DDV_MinMaxInt(pDX, m_iGreenIndex, 0, 255);
	DDX_Text(pDX, IDC_BLUE_INDEX, m_iBlueIndex);
	DDV_MinMaxInt(pDX, m_iBlueIndex, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetColor, CDialog)
	//{{AFX_MSG_MAP(CSetColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RAD_OSY, OnRadioBut)
	ON_EN_KILLFOCUS(IDC_RED_INDEX, OnKillfocusRedIndex)
	ON_EN_KILLFOCUS(IDC_GREEN_INDEX, OnKillfocusGreenIndex)
	ON_EN_KILLFOCUS(IDC_BLUE_INDEX, OnKillfocusBlueIndex)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_RAD_FILTR, OnRadioBut)
	ON_BN_CLICKED(IDC_RAD_PROFIL, OnRadioBut)
	ON_BN_CLICKED(IDC_RAD_POZADI, OnRadioBut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetColor message handlers

HBRUSH CSetColor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
/*	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;*/
	switch (nCtlColor)
	{
		case CTLCOLOR_SCROLLBAR:
		{        // create solid brushes to paint scroll bars
			CBrush* pXX = CBrush::FromHandle(m_hColorBrush);
			if (pXX != NULL)
				/* BOOL  bTest = */ pXX->DeleteObject();

			int nScrollID = pWnd->GetDlgCtrlID();
			switch (nScrollID)
			{
				case IDC_RED_SCROLL:
					return m_hColorBrush = CreateSolidBrush(RGB(255,0,0));
				case IDC_GREEN_SCROLL:
					return m_hColorBrush = CreateSolidBrush(RGB(0,255,0));
				case IDC_BLUE_SCROLL:
					return m_hColorBrush = CreateSolidBrush(RGB(0,0,255));
			}
		}
		break;

		case CTLCOLOR_STATIC:
		{           // Vytvori vybarveni pro kresleni barevnych boxu
			CBrush* pXX = CBrush::FromHandle(m_hColorBrush);
			if (pXX != NULL)
			{
				/* BOOL  bTest = */ pXX->DeleteObject();
			}
			int nStaticID = pWnd->GetDlgCtrlID();
			switch (nStaticID)
			{
				case IDC_COL_POZADI:
					return m_hColorBrush = CreateSolidBrush(m_clrTrueMap[COLOR_BK]);
				case IDC_COL_OSY:
					return m_hColorBrush = CreateSolidBrush(m_clrTrueMap[COLOR_OSY]);
				case IDC_COL_PROFIL:
					return m_hColorBrush = CreateSolidBrush(m_clrTrueMap[COLOR_PROFIL]);
				case IDC_COL_FILTR:
					return m_hColorBrush = CreateSolidBrush(m_clrTrueMap[COLOR_FILTR]);
			}
		}
		break;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CSetColor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_iRedIndex   = (int)GetRValue(m_clrTrueMap[m_radColor]);
	m_iGreenIndex = (int)GetGValue(m_clrTrueMap[m_radColor]);
	m_iBlueIndex  = (int)GetBValue(m_clrTrueMap[m_radColor]);

	//ScrollBary
	CScrollBar* pCsb = (CScrollBar*)GetDlgItem(IDC_RED_SCROLL);
	pCsb->SetScrollRange(-255,0,FALSE);
	pCsb->SetScrollPos(-(int)m_iRedIndex,TRUE);

	pCsb = (CScrollBar*)GetDlgItem(IDC_GREEN_SCROLL);
	pCsb->SetScrollRange(-255,0,FALSE);
	pCsb->SetScrollPos(-(int)m_iGreenIndex,TRUE);

	pCsb = (CScrollBar*)GetDlgItem(IDC_BLUE_SCROLL);
	pCsb->SetScrollRange(-255,0,FALSE);
	pCsb->SetScrollPos(-(int)m_iBlueIndex,TRUE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetColor::OnRadioBut() 
{
	UpdateData(TRUE);
	SetScrollBars(m_clrTrueMap[m_radColor],FALSE);
}

//barevne ScrollBary
void CSetColor::SetScrollBars(COLORREF clrRGB, BOOL bUpdate)
{
	m_iRedIndex   = (int)GetRValue(clrRGB);
	SetDlgItemInt(IDC_RED_INDEX,m_iRedIndex,TRUE);
	m_iGreenIndex = (int)GetGValue(clrRGB);
	SetDlgItemInt(IDC_GREEN_INDEX,m_iGreenIndex,TRUE);
	m_iBlueIndex  = (int)GetBValue(clrRGB);
	SetDlgItemInt(IDC_BLUE_INDEX,m_iBlueIndex,TRUE);

	((CScrollBar*)GetDlgItem(IDC_RED_SCROLL))  ->SetScrollPos(-m_iRedIndex,  TRUE);
	((CScrollBar*)GetDlgItem(IDC_GREEN_SCROLL))->SetScrollPos(-m_iGreenIndex,TRUE);
	((CScrollBar*)GetDlgItem(IDC_BLUE_SCROLL)) ->SetScrollPos(-m_iBlueIndex, TRUE);
	if (bUpdate)
	{
		UpdateData(FALSE);
	}
}

BOOL CSetColor::CheckIndex(int nColorID)
{
	BOOL bTrans;
	int nIndex;

	nIndex = (int)GetDlgItemInt(nColorID,&bTrans,FALSE);
/*	if (nIndex < AMINRGB || nIndex > AMAXRGB || !bTrans)
	{
		// OUT OF RANGE MESSAGE BOX
		CString strMessage;
		char  szOutbuf1[20],szOutbuf2[20];

		MSPRINTF(szOutbuf1,"%d",AMINRGB);
		MSPRINTF(szOutbuf2,"%d",AMAXRGB);
		//      IDS_OPTCOLOR1 = "Color RGB Index is out of range\nvalid range is %1 - %2"
		MFORMATSTRING2(strMessage,IDS_OPTCOLOR1,szOutbuf1,szOutbuf2);
		MESSAGEBOXSTOP(strMessage);
		m_nLastEditSwitch=-99;
		CEdit* pMyWnd = (CEdit*)GetDlgItem(nColorID);
		GotoDlgCtrl(pMyWnd);
		pMyWnd->SetSel(0,-1,FALSE);
		m_nLastEditSwitch=0;
		return FALSE;
	}*/
	int nScrollID=0;
	switch (nColorID)
	{
		case IDC_RED_INDEX:
			m_iRedIndex = nIndex;
			nScrollID = IDC_RED_SCROLL;
		break;
		case IDC_GREEN_INDEX:
			m_iGreenIndex = nIndex;
			nScrollID = IDC_GREEN_SCROLL;
		break;
		case IDC_BLUE_INDEX:
			m_iBlueIndex = nIndex;
			nScrollID = IDC_BLUE_SCROLL;
		break;
	}
	CScrollBar* pCsb = (CScrollBar*)GetDlgItem(nScrollID);
	pCsb->SetScrollPos(-(int)nIndex,TRUE);
	m_clrTrueMap[m_radColor] = RGB(m_iRedIndex,m_iGreenIndex,m_iBlueIndex);
/*	CDC* pdcTemp = GetDC();
	if (pdcTemp==NULL)
		m_clrSolidMap[m_nCurColor] = m_clrTrueMap[m_nCurColor];
	else
		m_clrSolidMap[m_nCurColor] = pdcTemp->GetNearestColor(m_clrTrueMap[m_nCurColor]);
	ReleaseDC(pdcTemp);*/
//	SetButtonBoxes(m_nCurColor,FALSE);
	UpdateData(FALSE);

	return TRUE;
}

void CSetColor::OnKillfocusRedIndex() 
{
	UpdateData(TRUE);

	CWnd * pWindowFocus = GetFocus();

	if ( !pWindowFocus )
	{
		return;
	}
	// Get the control's window handle
	int nWndID = pWindowFocus->GetDlgCtrlID();
/*	if (m_nLastEditSwitch == -99)
	{
		return;
	}*/
	if (nWndID == IDCANCEL ||nWndID == IDOK)
	{
//		m_nLastEditSwitch = INFO_RED_INDEX;
		return;
	}
	CheckIndex(IDC_RED_INDEX);
	switch (m_radColor)
	{
		case COLOR_BK:
			m_colPozadi.Invalidate();
		case COLOR_OSY:
			m_colOsy.Invalidate();
		case COLOR_PROFIL:
			m_colProfil.Invalidate();
		case COLOR_FILTR:
			m_colFiltr.Invalidate();
	}
}

void CSetColor::OnKillfocusGreenIndex() 
{
	UpdateData(TRUE);

	CWnd * pWindowFocus = GetFocus();

	if ( !pWindowFocus )
	{
		return;
	}
	// Get the control's window handle
	int nWndID = pWindowFocus->GetDlgCtrlID();
/*	if (m_nLastEditSwitch == -99)
	{
		return;
	}*/
	if (nWndID == IDCANCEL ||nWndID == IDOK)
	{
//		m_nLastEditSwitch = INFO_RED_INDEX;
		return;
	}
	CheckIndex(IDC_GREEN_INDEX);
	switch (m_radColor)
	{
		case COLOR_BK:
			m_colPozadi.Invalidate();
		case COLOR_OSY:
			m_colOsy.Invalidate();
		case COLOR_PROFIL:
			m_colProfil.Invalidate();
		case COLOR_FILTR:
			m_colFiltr.Invalidate();
	}
}

void CSetColor::OnKillfocusBlueIndex() 
{
	UpdateData(TRUE);

	CWnd * pWindowFocus = GetFocus();

	if ( !pWindowFocus )
	{
		return;
	}
	// Get the control's window handle
	int nWndID = pWindowFocus->GetDlgCtrlID();
/*	if (m_nLastEditSwitch == -99)
	{
		return;
	}*/
	if (nWndID == IDCANCEL ||nWndID == IDOK)
	{
//		m_nLastEditSwitch = INFO_RED_INDEX;
		return;
	}
	CheckIndex(IDC_BLUE_INDEX);
	switch (m_radColor)
	{
		case COLOR_BK:
			m_colPozadi.Invalidate();
		case COLOR_OSY:
			m_colOsy.Invalidate();
		case COLOR_PROFIL:
			m_colProfil.Invalidate();
		case COLOR_FILTR:
			m_colFiltr.Invalidate();
	}
}

void CSetColor::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nIndex=0;

	int nScrollID = pScrollBar->GetDlgCtrlID();
	switch (nScrollID)
	{    // check which scroll bar
		case IDC_RED_SCROLL:
			nIndex = m_iRedIndex;
		break;
		case IDC_GREEN_SCROLL:
			nIndex = m_iGreenIndex;
		break;
		case IDC_BLUE_SCROLL:
			nIndex = m_iBlueIndex;
		break;
	}

//	nIndex =  pScrollBar->GetScrollPos();
	switch (nSBCode)
	{   // check for type and amount of change
		case SB_PAGEDOWN:
			nIndex  = max(0,nIndex-16);
		break;

		case SB_LINEDOWN:
			nIndex  = max(0,nIndex-1);
		break;

		case SB_PAGEUP:
			nIndex  = min(255,nIndex+16);
		break;

		case SB_LINEUP:
			nIndex  = min(255,nIndex+1);
		break;

		case SB_TOP:
			nIndex  =0;
		break;

		case SB_BOTTOM:
			nIndex  =255;
		break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			nIndex  = -(int)nPos;
		break;
	}

	switch (nScrollID)
	{                        //update proper scroll index
		case IDC_RED_SCROLL:
			m_iRedIndex = nIndex;
		break;
		case IDC_GREEN_SCROLL:
			m_iGreenIndex = nIndex;
		break;
		case IDC_BLUE_SCROLL:
			m_iBlueIndex = nIndex;
		break;
	}

	//nova pozice ScrollBaru
	pScrollBar->SetScrollPos(-nIndex,TRUE);
	//nova barva
	m_clrTrueMap[m_radColor] = RGB(m_iRedIndex,m_iGreenIndex,m_iBlueIndex);

	UpdateData(FALSE);

	switch (m_radColor)
	{
		case COLOR_BK:
			m_colPozadi.Invalidate();
		case COLOR_OSY:
			m_colOsy.Invalidate();
		case COLOR_PROFIL:
			m_colProfil.Invalidate();
		case COLOR_FILTR:
			m_colFiltr.Invalidate();
	}

}

//nastavi i barvu
//i je poradove cislo barvy
void CSetColor::SetColor(int i, COLORREF col)
{
	m_clrTrueMap[i] = col;
}

//vrati barvu
COLORREF CSetColor::GetColor(int i)
{
	return m_clrTrueMap[i];
}
