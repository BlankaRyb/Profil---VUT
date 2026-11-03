// ODecetKorel.cpp : implementation file
//

#include "stdafx.h"
#include "Profil.h"
#include "ODecetKorel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CODecetKorel dialog


CODecetKorel::CODecetKorel(CWnd* pParent /*=NULL*/)
	: CDialog(CODecetKorel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CODecetKorel)
	m_typ = 0;
	m_misto = 37;
	m_odecetX = 0.0;
	m_odecetY = 0.0;
	//}}AFX_DATA_INIT
	m_dKorel = 90;
	m_bVypocet = FALSE;
}


void CODecetKorel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CODecetKorel)
	DDX_Control(pDX, IDC_ODECETY, m_editOdecety);
	DDX_Control(pDX, IDC_ODECETX, m_editOdecetx);
	DDX_Radio(pDX, IDC_RADIO1, m_typ);
	DDX_Text(pDX, IDC_MISTO, m_misto);
	DDV_MinMaxDouble(pDX, m_misto, 0., 100.);
	DDX_Text(pDX, IDC_ODECETX, m_odecetX);
	DDX_Text(pDX, IDC_ODECETY, m_odecetY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CODecetKorel, CDialog)
	//{{AFX_MSG_MAP(CODecetKorel)
	ON_BN_CLICKED(IDC_VYPOCET, OnVypocet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CODecetKorel message handlers

BOOL CODecetKorel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CODecetKorel::ObalkaHorni()
{
	__int32 i,i_max,poc;
	double max; //pro hledani maxima ze zbytku dat

	poc = (__int32)(m_SizeX*m_dKorel/100);

	//>>inicializace obalky
	m_yObalka1 = (double*)calloc (m_SizeX, sizeof(double));
	//vzhledem k tomu, jak jsou pocitany korelacni body je jich maximalne
	//o jeden mene nez omerenych bodu
	for (i=0; i<poc; i++)
	{
		m_yObalka1[i] = m_MinYKorel-1;
	}
	//<<inicializace obalky
	//1. bod obalky
	m_yObalka1[0] = m_yKorel[0];
	i_max = 0;
	max = m_MinYKorel-1;
	//dalsi body obalky
	do
	{
		for (i=i_max+1; i<poc; i++)
		{
			if(max<m_yKorel[i])
			{
				max = m_yKorel[i];
				i_max = i;
			}
		}
		m_yObalka1[i_max] = max;
		max = m_MinYKorel-1;
	} while (i_max<(poc-1));
}

void CODecetKorel::ObalkaDolni()
{
	__int32 i,i_min,poc;
	double min; //pro hledani minima ze zbytku dat

	poc = (__int32)(m_SizeX*m_dKorel/100);

	//>>inicializace obalky
	m_yObalka2 = (double*)calloc (m_SizeX, sizeof(double));
	//vzhledem k tomu, jak jsou pocitany korelacni body je jich maximalne
	//o jeden mene nez omerenych bodu
	for (i=0; i<poc; i++)
	{
		m_yObalka2[i] = m_MaxYKorel+1;
	}
	//<<inicializace obalky
	//1. bod obalky
	m_yObalka2[0] = m_yKorel[0];
	i_min = 0;
	min = m_MaxYKorel+1;
	//dalsi body obalky
	do
	{
		for (i=i_min+1; i<poc; i++)
		{
			if(min>m_yKorel[i])
			{
				min = m_yKorel[i];
				i_min = i;
			}
		}
		m_yObalka2[i_min] = min;
		min = m_MaxYKorel+1;
	} while (i_min<(poc-1));
}

void CODecetKorel::OnVypocet() 
{
	__int32 i;
	double hodnota; //hodnota odpovidajici pozadovanym procentum

	//m_Dy
	UpdateData(TRUE);
//	hodnota = (m_MaxYKorel-m_MinYKorel)*m_misto/100+m_MinYKorel;
	hodnota = m_yKorel[0]*m_misto/100;

	m_bVypocet = FALSE;

	if (m_typ==0)//odecet z grafu
	{
		for (i=0; i<m_SizeX; i++)
		{
			if (m_yKorel[i]<=hodnota)
			{
				m_odecetX = m_xData[i];
				m_odecetY = m_yKorel[i];
				i = m_SizeX-1;
				m_bVypocet = TRUE;
			}
		}
	}

	if (m_typ==1)//odecet po obalce grafu
	{
		__int32 poc;
		__int32 i_pred; //predchozi i

		ObalkaHorni();
		ObalkaDolni();

		poc = (__int32)(m_SizeX*m_dKorel/100);

		i_pred = -1;

		//odecet probiha po horni obalce
		for (i=0; i<poc; i++)
		{
			if (m_yObalka1[i] != m_MinYKorel-1)
			{
				if (m_yObalka1[i]<=hodnota)
				{
					if (i_pred != -1) //je potreba predchozi (prvni) bod primky
					{
						m_odecetX = m_xData[i]-
							(hodnota-m_yObalka1[i])/(m_yObalka1[i_pred]-m_yObalka1[i])*
							(m_xData[i]-m_xData[i_pred]);
						m_odecetY = hodnota;
						i = poc-1;
						m_bVypocet = TRUE;
					}
				}
				i_pred = i;
			}
		}
	}
	UpdateData(FALSE);
}

void CODecetKorel::OnOK() 
{
	CDialog::OnOK();

	OnVypocet();
}
