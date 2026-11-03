// ProfilView.cpp : implementation of the CProfilView class
//

#include "stdafx.h"
#include "Profil.h"

#include "ProfilDoc.h"
#include "ProfilView.h"
#include "DataMoznosti.h"
#include "Vlastnosti.h"
#include "ODecetKorel.h"
#include "SetColor.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*sirka sloupcu v tabulce dat*/
int sloupec[SLOUPEC_POCET] = {SLOUPEC_X, SLOUPEC_DATA, SLOUPEC_FILTR,
    SLOUPEC_KOREL, SLOUPEC_OMEGA, SLOUPEC_SPETRUM, SLOUPEC_FREK, SLOUPEC_HUSTOTA};
/////////////////////////////////////////////////////////////////////////////
// CProfilView

IMPLEMENT_DYNCREATE(CProfilView, CScrollView)

BEGIN_MESSAGE_MAP(CProfilView, CScrollView)
	//{{AFX_MSG_MAP(CProfilView)
	ON_COMMAND(ID_DATA_MONOSTI, OnDataMonosti)
	ON_COMMAND(ID_DATA_VLASTNOSTI, OnDataVlastnosti)
	ON_UPDATE_COMMAND_UI(ID_AUTOKORELANFUNKCE_ODEETHODNOTYZGRAFU, OnUpdateAutokorelanfunkceOdecethodnotyzgrafu)
	ON_COMMAND(ID_AUTOKORELANFUNKCE_ODEETHODNOTYZGRAFU, OnAutokorelanfunkceOdecethodnotyzgrafu)
	ON_COMMAND(ID_DATA_NASTAVENBAREV, OnDataNastavenbarev)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfilView construction/destruction

CProfilView::CProfilView()
{
	m_bPrint = FALSE;
	m_bVse = FALSE;
	m_bOdecetKorel = FALSE;
	m_Obalka = FALSE;
	m_iTyp = PROFIL;
	m_sFileName = "";
	m_SizeX = m_SizeY = 0;
	m_mx = 0;
	m_Dy = 1;
	m_dKorel = 90;
	m_iKrok = 1;
	//>>Filtr
	m_Filtr.filtr = FALSE;
	m_Filtr.vlnitost = 0; //vlnitost
	m_Filtr.druh = TROJUHELNIK; //trojuhelnik
	m_Filtr.delkaTyp = 0; //0.08
	m_Filtr.delka = 0.01;
	//<<Filtr
	m_procenta = 37;
	m_dFrek = 10;
	m_bKorel = FALSE;
	m_bSpektrum = FALSE;
	m_bSklon = FALSE;
	m_bRozloz = FALSE;
	//>>Zalozka Zadani
	//datum
	CTime t = CTime::GetCurrentTime(); //aktualni cas a datum
	m_Datum = t.Format("%d.%m.%Y");
	m_Popis = "";
	m_sTypPrvku = "";
	//<<Zalozka Zadani

	//>>>>hodnoty pro kresleni grafu
	m_SizeNapis.cx  = m_SizeNapis.cy  = 0;
	m_SizeTextOx.cx = m_SizeTextOx.cy = 0;
	m_SizeTextOy.cx = m_SizeTextOy.cy = 0;
	m_SizePopisOx.cx = m_SizePopisOx.cy = 0;
	m_SizePopisOy.cx = m_SizePopisOy.cy = 0;
	//<<<<hodnoty pro kresleni grafu
}

CProfilView::~CProfilView()
{
}

BOOL CProfilView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CProfilView drawing

void CProfilView::OnDraw(CDC* pDC)
{
	CProfilDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if ((!m_bPrint)&&(!m_bVse)) //neprobiha tisk
	{
		//zjisti aktualni velikost dokumentu
		GetClientRect(m_rc);
	}

	//vypln pozadi
	CBrush pBrush;
	pBrush.CreateSolidBrush(theApp.GetColor(COLOR_BK));
	pDC->SetBkColor(theApp.GetColor(COLOR_BK));
	pDC->FillRect(m_rc, &pBrush);

	switch(m_iTyp)
	{
		case DATA: //0
		{
			m_SizeNapis = Nadpis(pDC, IDS_TITLE_DATA);
			TabulkaHodnot(pDC);
		}
		break;
		case PROFIL: //1
		{
			m_SizeNapis = Nadpis(pDC, IDS_TITLE_PROFIL);
			graf(pDC, m_iTyp);
		}
		break;
		case KORELACE: //2
		{
			m_SizeNapis = Nadpis(pDC, IDS_TITLE_KORELACE);
			graf(pDC, m_iTyp);
		}
		break;
		case NOR_KORELACE: //3
		{
			m_SizeNapis = Nadpis(pDC, IDS_TITLE_NOR_KORELACE);
			graf(pDC, m_iTyp);
		}
		break;
		case SPEKTRUM: //4
		{
			m_SizeNapis = Nadpis(pDC, IDS_TITLE_SPEKTRUM);
			graf(pDC, m_iTyp);
		}
		break;
		case HUSTOTA: //5
		{
			m_SizeNapis = Nadpis(pDC, IDS_TITLE_HUSTOTA);
			graf(pDC, m_iTyp);
		}
		break;
		case SKLON: //6
		{
			m_SizeNapis = Nadpis(pDC, IDS_TITLE_SKLON);
			graf(pDC, m_iTyp);
		}
		break;
		case ROZLOZENI: //7
		{
			m_SizeNapis = Nadpis(pDC, IDS_TITLE_ROZLOZENI);
			graf(pDC, m_iTyp);
		}
		break;
		case VSE: //7
		{
			graf(pDC, m_iTyp);
		}
		break;
	}
}

void CProfilView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: calculate the total size of this view
	InitScrollSizes();

	m_sFileName = theApp.GetFileName();
	m_dKorel = theApp.GetProcentaKorel();
	m_dFrek = theApp.GetFrekvence();

	NactiData();
	NactiDataZad();

	SpoctiDrsnosti();
}

/////////////////////////////////////////////////////////////////////////////
// CProfilView printing

BOOL CProfilView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CProfilView diagnostics

#ifdef _DEBUG
void CProfilView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CProfilView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CProfilDoc* CProfilView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProfilDoc)));
	return (CProfilDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProfilView message handlers

//nadpis na zpracovanymi daty: cisla nebo graf
//Vrati rozmer textu-nadpisu
CSize CProfilView::Nadpis(CDC* pDC, UINT nID)
{
	CFont fontText, *pOldFont;
	CString	str;
	CSize sizeText;
	__int32 y;

	pDC->SetTextColor(theApp.GetColor(COLOR_OSY));

	//vypocet vysky textu pro nadpis, zavisi na rozmeru "y" u aktualniho okna
	y = (__int32)(m_rc.bottom-m_rc.top)/POMERNADPIS;
	fontText.CreateFont(
			-y,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,                   // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial CE");                  // lpszFacename
	pOldFont = pDC->SelectObject(&fontText);

	str.LoadString(nID);
	sizeText = pDC->GetTextExtent(str);
	pDC->TextOut(m_rc.left, m_rc.top, str);

	pDC->SelectObject(pOldFont);

	return sizeText;
}

//zobrazi to co je v zalozce Zadani
//sirka - sirka mista pro text
CSize CProfilView::NapisZalZad(CDC* pDC, int sirka)
{
	CFont   fontText, *pOldFont;
	CString	str,strPopis;
	TCHAR   c[2];
	CSize   size, sizeText;
	int     i;
	__int32 y,vyska;

	pDC->SetTextColor(theApp.GetColor(COLOR_OSY));

	//vypocet vysky textu pro nadpis, zavisi na rozmeru "y" u aktualniho okna
	vyska = (__int32)(m_rc.bottom-m_rc.top)/POMERPISMA;
	fontText.CreateFont(
			-vyska,                    // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial CE");               // lpszFacename
	pOldFont = pDC->SelectObject(&fontText);

	c[0] = 13;
	c[1] = 10;
	c[2] = 0;

	//naradi 10,13(enter) mezernikem
	//to je kvuli lepsimu zobrazeni, je potreba vyuzit jen jeden radek, aby zbylo misto na grafy
	i = 0;
	str = "";
	strPopis = m_Popis;
	do
	{
		i = strPopis.Find(c,i);
   		if (i>0)
		{
			str += strPopis.Left(i);
			str += ' ';
			strPopis = strPopis.Right(strPopis.GetLength()-i-2);
			i++;
		}
	}
	while (i>0);
	str += strPopis;

	y = 0;
	sizeText = pDC->GetTextExtent("Datum: "+m_Datum);
	size = sizeText;
	pDC->ExtTextOut(0,y, ETO_CLIPPED, CRect(0,y,sirka-0,y+sizeText.cy), "Datum: "+m_Datum, NULL);
	y += sizeText.cy;
	sizeText = pDC->GetTextExtent("Typ prvku: "+m_sTypPrvku);
	if (size.cx < sizeText.cx)
	{
		size.cx = sizeText.cx;
	}
	size.cy += sizeText.cy;
	pDC->ExtTextOut(0,y,ETO_CLIPPED, CRect(0,y,sirka-0,y+sizeText.cy), "Typ prvku: "+m_sTypPrvku, NULL);
	y += sizeText.cy;
	sizeText = pDC->GetTextExtent("Popis: "+str);
	if (size.cx < sizeText.cx)
	{
		size.cx = sizeText.cx;
	}
	size.cy += sizeText.cy;
	pDC->ExtTextOut(0,y,ETO_CLIPPED, CRect(0,y,sirka-0,y+sizeText.cy), "Popis: "+str, NULL);		//m_Popis
/*	pDC->TextOut(20, 1,  "Datum: "+m_Datum);
	pDC->TextOut(20, 16, "Typ prvku: "+m_sTypPrvku);
	pDC->TextOut(20, 31, "Popis: "+str);		//m_Popis*/

	pDC->SelectObject(pOldFont);
	return size;
}

//zobrazi tabulku hodnot grafu, pokud je zapnuta fitrace tak i filtrovana data
void CProfilView::TabulkaHodnot(CDC* pDC)
{
	CRect rcDis;//rect tabuky na obrazovce
	CPen pen, penB, *pOldPen;
	__int32 i,sloupce,sloupceSuma,max;
	__int32 SizeX; //nahrada za m_SizeX, kdyby m_SizeX bylo velke

	//vypln pozadi
	CRect rcBrush;
	CBrush pBrush;
	pBrush.CreateSolidBrush(theApp.GetColor(COLOR_BK));
	
	SizeX = m_SizeX;

	rcDis.left = m_rc.left;
	rcDis.right = m_rc.left+TABX1+TABX2;
	for (i=0; i<SLOUPEC_POCET; i++)
	{
		if ((i!=2)||(m_Filtr.filtr))
		{
			rcDis.right += sloupec[i];
		}
	}
	rcDis.top = m_rc.top;
	rcDis.bottom = m_rc.top+TABY1+m_SizeNapis.cy+TABY2+RADEK*m_SizeX;
	
	//posuvniky
	CSize sizeTotal;
	sizeTotal.cx = rcDis.Width()+5;
	sizeTotal.cy = rcDis.Height()+5;
	if (sizeTotal.cy > TAB_SIZE_Y)
	{
		SizeX = (__int32)((TAB_SIZE_Y-m_rc.top-(TABY1+m_SizeNapis.cy)-TABY2)/RADEK);
		sizeTotal.cy = m_rc.top+TABY1+m_SizeNapis.cy+TABY2+RADEK*SizeX+5;
	}
	CSize s,s1,s2;
	int ii;
	ii = MM_TEXT;
	s = GetTotalSize();
	GetDeviceScrollSizes( ii, s, s1, s2);
	SetScrollSizes(MM_TEXT, sizeTotal);
	s = GetTotalSize();
	GetDeviceScrollSizes( ii, s, s1, s2);

	//vypln pozadi
	rcBrush = m_rc;
	rcBrush.bottom = m_rc.top+TABY1+m_SizeNapis.cy+RADEK*(SizeX+1);
	pDC->FillRect(rcBrush, &pBrush);

	//>>>>>>>>>>>>>>>>>>>>> TABULKA
	//tenke a silne pero
	pen.CreatePen (PS_SOLID, 1, RGB(0, 0, 0));
	penB.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	sloupceSuma = 0;
	for (i=0; i<SLOUPEC_POCET; i++)
	{
		//pokud je vypnut filtr, tak sloupëc s filtrovanymi daty se nezobrazi
		if ((i!=2)||(m_Filtr.filtr))
		{
			sloupceSuma += sloupec[i];
		}
	}
	//silne pero
	pOldPen = pDC->SelectObject(&penB);
	//ramecek kolem tabulky
	max = m_rc.top+TABY1+m_SizeNapis.cy+RADEK*(SizeX+1);
	pDC->MoveTo(m_rc.left+TABX1,m_rc.top+TABY1+m_SizeNapis.cy);
	pDC->LineTo(m_rc.left+TABX1,m_rc.top+TABY1+m_SizeNapis.cy+RADEK*(SizeX+1));
	pDC->LineTo(m_rc.left+TABX1+sloupceSuma,m_rc.top+TABY1+m_SizeNapis.cy+RADEK*(SizeX+1));
	pDC->LineTo(m_rc.left+TABX1+sloupceSuma,m_rc.top+TABY1+m_SizeNapis.cy);
	pDC->LineTo(m_rc.left+TABX1,m_rc.top+TABY1+m_SizeNapis.cy);
	//první radek
	pDC->MoveTo(m_rc.left+TABX1,m_rc.top+TABY1+m_SizeNapis.cy+RADEK);
	pDC->LineTo(m_rc.left+TABX1+sloupceSuma,m_rc.top+TABY1+m_SizeNapis.cy+RADEK);

	//tenke pero
	pDC->SelectObject(&pen);
	//radky
	for (i=0; i<(SizeX+1); i++)
	{
		pDC->MoveTo(m_rc.left+TABX1,m_rc.top+TABY1+m_SizeNapis.cy+RADEK*(i+2));
		pDC->LineTo(m_rc.left+TABX1+sloupceSuma,m_rc.top+TABY1+m_SizeNapis.cy+RADEK*(i+2));
	}
	//sloupce
	sloupce = 0;
	for (i=0; i<(SLOUPEC_POCET-1); i++)
	{
		if ((i!=2)||(m_Filtr.filtr))
		{
			sloupce += sloupec[i];
			if ((i==3) || (i==5))
			{
				pDC->SelectObject(&penB);
			}
			pDC->MoveTo(m_rc.left+TABX1+sloupce,m_rc.top+TABY1+m_SizeNapis.cy);
			pDC->LineTo(m_rc.left+TABX1+sloupce,m_rc.top+TABY1+m_SizeNapis.cy+RADEK*(SizeX+1));
			if ((i==3) || (i==5))
			{
				pDC->SelectObject(&pen);
			}
		}
	}

	pDC->SelectObject(pOldPen);
	//<<<<<<<<<<<<<<<<<<<<< TABULKA

	//>>>>>>>>>>>>>>>>>>>>> TEXT V TABULCE
	CFont fontText, *pOldFont;
	CString	str;
	char ch[50];

	fontText.CreateFont(
			15,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                   // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial CE");                  // lpszFacename
	pOldFont = pDC->SelectObject(&fontText);

	sloupce = 0;
	//prvni radek
	//X
	str.LoadString(IDS_SLOUPEC_X);
	pDC->TextOut(m_rc.left+TABX1+5,m_rc.top+TABY1+m_SizeNapis.cy+3, str);
	sloupce += sloupec[0];
	//Data-Profil Y
	str.LoadString(IDS_SLOUPEC_DATA);
	pDC->TextOut(m_rc.left+TABX1+5+sloupce,m_rc.top+TABY1+m_SizeNapis.cy+3, str);
	sloupce += sloupec[1];
	//Filtrovana data
	if (m_Filtr.filtr)
	{
		str.LoadString(IDS_SLOUPEC_FILTRACE);
		pDC->TextOut(m_rc.left+TABX1+5+sloupce,m_rc.top+TABY1+m_SizeNapis.cy+3, str);
		sloupce += sloupec[2];
	}
	//Korelacni data
	str.LoadString(IDS_SLOUPEC_KOREL);
	pDC->TextOut(m_rc.left+TABX1+5+sloupce,m_rc.top+TABY1+m_SizeNapis.cy+3, str);
	sloupce += sloupec[3];
	//Omega
	str.LoadString(IDS_SLOUPEC_OMEGA);
	pDC->TextOut(m_rc.left+TABX1+5+sloupce,m_rc.top+TABY1+m_SizeNapis.cy+3, str);
	sloupce += sloupec[4];
	//Vykonove spektrum
	str.LoadString(IDS_SLOUPEC_SPEKTRUM);
	pDC->TextOut(m_rc.left+TABX1+5+sloupce,m_rc.top+TABY1+m_SizeNapis.cy+3, str);
	sloupce += sloupec[5];
	//Frekvence
	str.LoadString(IDS_SLOUPEC_FREK);
	pDC->TextOut(m_rc.left+TABX1+5+sloupce,m_rc.top+TABY1+m_SizeNapis.cy+3, str);
	sloupce += sloupec[6];
	//Energeticka spektralni hustota
	str.LoadString(IDS_SLOUPEC_HUSTOTA);
	pDC->TextOut(m_rc.left+TABX1+5+sloupce,m_rc.top+TABY1+m_SizeNapis.cy+3, str);
	sloupce += sloupec[7];

	sloupce = 0;
	//X
	for(i=0; i<SizeX; i++)
	{
		_gcvt(m_xData[i], 6, ch);
		pDC->TextOut(m_rc.left+TABX1+5,m_rc.top+TABY1+m_SizeNapis.cy+4+RADEK*(i+1), ch);
	}
	sloupce += sloupec[0];
	//Data-profil
	for(i=0; i<SizeX; i++)
	{
		_gcvt(m_yData[i], 6, ch);
		pDC->TextOut(m_rc.left+TABX1+sloupce+5,m_rc.top+TABY1+m_SizeNapis.cy+4+RADEK*(i+1), ch);
	}
	sloupce += sloupec[1];
	//filtrovana data
	if (m_Filtr.filtr)
	{
		for(i=0; i<SizeX; i++)
		{
			_gcvt(m_yFiltr[i], 6, ch);
			pDC->TextOut(m_rc.left+TABX1+sloupce+5,m_rc.top+TABY1+m_SizeNapis.cy+4+RADEK*(i+1), ch);
		}
		sloupce += sloupec[2];
	}
	//korelacni data
	for(i=0; i<SizeX; i++)
	{
		_gcvt(m_yKorel[i], 6, ch);
		pDC->TextOut(m_rc.left+TABX1+sloupce+5,m_rc.top+TABY1+m_SizeNapis.cy+4+RADEK*(i+1), ch);
	}
	sloupce += sloupec[3];
	//Omega
	double dil = 2*PI*m_dFrek/m_SizeX;
	for(i=0; i<SizeX; i++)
	{
		_gcvt(dil*i, 6, ch);
		pDC->TextOut(m_rc.left+TABX1+sloupce+5,m_rc.top+TABY1+m_SizeNapis.cy+4+RADEK*(i+1), ch);
	}
	sloupce += sloupec[4];
	//vykonove spektrum
	for(i=0; i<SizeX; i++)
	{
		_gcvt(m_ySpektrum[i], 6, ch);
		pDC->TextOut(m_rc.left+TABX1+sloupce+5,m_rc.top+TABY1+m_SizeNapis.cy+4+RADEK*(i+1), ch);
	}
	sloupce += sloupec[5];
	//frekvence
	dil = m_dFrek/m_SizeX;
	for(i=0; i<SizeX; i++)
	{
		_gcvt(dil*i, 6, ch);
		pDC->TextOut(m_rc.left+TABX1+sloupce+5,m_rc.top+TABY1+m_SizeNapis.cy+4+RADEK*(i+1), ch);
	}
	sloupce += sloupec[6];
	//Energeticka spkralni hustota
	for(i=0; i<SizeX; i++)
	{
		_gcvt(m_ySpektrum[i]/(m_xData[1]-m_xData[0])*1000, 6, ch);
		pDC->TextOut(m_rc.left+TABX1+sloupce+5,m_rc.top+TABY1+m_SizeNapis.cy+4+RADEK*(i+1), ch);
	}
//	sloupce += sloupec[7]; //kdyby se pripojil dalsi sloupec

	pDC->SelectObject(pOldFont);
	//<<<<<<<<<<<<<<<<<<<<< TEXT V TABULCE
}

//nakresli graf typ grafu je urcen iTyp a muze nabyvat techto hodnot
//1,..,4 (PROFIL, KORELACE, SPEKTRUM,HUSTOTA)
void CProfilView::graf(CDC* pDC, __int16 iTyp)
{
	double  MinX, MaxX; //min max; osa x
	double  MinY, MaxY; //min max; osa y
	CPen    pen, penB, *pOldPen;
	char    ch[1024];
	CSize   size, sizeTextMin, sizeTextMax;
	CFont fontText, *pOldFont;
	__int32 y;

	//kontrola jestli je tam misto na graf
	if ((((m_rc.right-X2)-(m_rc.left+m_SizeTextOy.cx)) > 0) &&
		(((m_rc.bottom-m_SizeTextOx.cy)-(m_rc.top)) > 0))
	{

		m_carka = (__int32)((double)CARKA*(m_rc.bottom-m_rc.top)+0.5);

		switch(iTyp)
		{
			case DATA: //0
			{
				//graf zde neni;
			}
			break;
			case PROFIL: //1
			{
				MinX = m_MinX;
				MaxX = m_MaxX;
				MinY = m_MinY;
				MaxY = m_MaxY;
			}
			break;
			case KORELACE: //2
			{
				MinX = m_MinX;
				MaxX = m_MinX+(m_MaxX-m_MinX)*m_dKorel/100;
				MinY = m_MinYKorel;
				MaxY = m_MaxYKorel;
			}
			break;
			case NOR_KORELACE: //3
			{
				MinX = m_MinX;
				MaxX = m_MinX+(m_MaxX-m_MinX)*m_dKorel/100;
				MinY = m_MinYKorel/m_Dy;
				MaxY = m_MaxYKorel/m_Dy;
				if (MinY > MaxY)//chyba pri deleni
				{
					MinY = m_MinYKorel;
					MaxY = m_MaxYKorel;
				}
			}
			break;
			case SPEKTRUM: //4
			{
				MinX = 0;
				MaxX = 2*PI*m_dFrek;
				MinY = m_MinYSpektrum;
				MaxY = m_MaxYSpektrum;
			}
			break;
			case HUSTOTA: //5
			{
				MinX = 0;
				MaxX = m_dFrek;
				MinY = m_MinYSpektrum/(m_xData[1]-m_xData[0])*1000;
				MaxY = m_MaxYSpektrum/(m_xData[1]-m_xData[0])*1000;
			}
			break;
			case SKLON: //6
			{
				MinX = 1;
				MaxX = HL_SKLON; //9
				MinY = m_MinYSklon;
				MaxY = m_MaxYSklon;
			}
			break;
			case ROZLOZENI: //7
			{
				MinX = 0;
				MaxX = HL_ROZL; //20
				MinY = 0;
				MaxY = m_SizeY; //pocet odmerenych prvku
			}
			break;
			case VSE: //8
			{
				CRect rc_zal;

				//to co je v zalozce Zadami
				size = NapisZalZad(pDC, m_rc.Width()/3*2);

				m_bVse = TRUE;
				rc_zal = m_rc;
				m_rc.right = m_rc.left+(m_rc.Width()/3*2);
				m_rc.top += size.cy;
				m_rc.bottom = m_rc.top+(m_rc.Height()/2);
				m_SizeNapis = Nadpis(pDC, IDS_TITLE_PROFIL);
				graf(pDC, PROFIL);

				m_rc = rc_zal;
				m_rc.right = m_rc.left+(m_rc.Width()/3*2);
				m_rc.top = m_rc.top+((m_rc.Height()+size.cy)/2);
				m_SizeNapis = Nadpis(pDC, IDS_TITLE_KORELACE);
				graf(pDC, KORELACE);

				m_rc = rc_zal;
				m_rc.left = m_rc.left+(m_rc.Width()/3*2);
				m_rc.bottom = m_rc.top+(m_rc.Height()/3);
				m_SizeNapis = Nadpis(pDC, IDS_TITLE_HUSTOTA);
				graf(pDC, HUSTOTA);

				m_rc = rc_zal;
				m_rc.left = m_rc.left+(m_rc.Width()/3*2);
				m_rc.top = m_rc.top+(m_rc.Height()/3);
				m_rc.bottom = rc_zal.top+(rc_zal.Height()/3*2);
				m_SizeNapis = Nadpis(pDC, IDS_TITLE_SKLON);
				graf(pDC, SKLON);

				m_rc = rc_zal;
				m_rc.left = m_rc.left+(m_rc.Width()/3*2);
				m_rc.top = m_rc.top+(m_rc.Height()/3*2);
				m_SizeNapis = Nadpis(pDC, IDS_TITLE_ROZLOZENI);
				graf(pDC, ROZLOZENI);

				m_rc = rc_zal;
				m_bVse = FALSE;
			}
			break;
		}
		if (MinY == MaxY)
		{
			MinY -= 0.5;
			MaxY += 0.5;
		}

		//vypocet vysky textu pro nadpis, zavisi na rozmeru "y" u aktualniho okna
		y = GetVyskaTextu();

		//Nastaveni fondu
		fontText.CreateFont(
				y,                        // nHeight
				0,                         // nWidth
				0,                         // angle of escapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				"Arial CE");                  // lpszFacename
		pOldFont = pDC->SelectObject(&fontText);

		///////////////////////////////////////////////
		//osy										 //
		///////////////////////////////////////////////

		//>>vzdalenost car v grafu u kterych jsou hodnoty
		//m_vzdalenostX, m_vzdalenostY - nasobky 2, 5, 10
		double d, logar, cis;
		double dilek;

		y=1;
		//X
		if(m_bPrint)
		{
			//vypocet minymalni mezery mezi dluhymi cary v grafu
			y = (__int32)(m_rc.right-m_rc.left)/POMERTISK;
			d = ((m_rc.right-X2)-(m_rc.left+m_SizeTextOy.cx))/y;
		}
		else
		{
			d = ((m_rc.right-X2)-(m_rc.left+m_SizeTextOy.cx))/MEZERA;
		}
		if (d != 0)
		{
/*			d = (MaxX-MinX)/d;
			d *= 2;
			logar = log10(d)+0.5;
			logar = floor(logar);
			m_vzdalenostX = (pow(10,logar))/2;*/
			d = (MaxX-MinX)/d;
			logar = log10(d);
			if (d>0)
			{
				logar = floor(logar);
			}
			else
			{
				logar = ceil(logar);
			}
			cis = pow(10,logar);
			m_vzdalenostX = cis;
			if (d>cis)                         //*2
			{
				m_vzdalenostX = 2*cis;
				if(d>m_vzdalenostX)            //*5
				{
					m_vzdalenostX = 5*cis;     //*10
					if(d>m_vzdalenostX)
					{
						m_vzdalenostX = 10*cis;
					}
				}
			}
		}
		else
		{
			m_vzdalenostX = 0;
		}
		//Y
		if(m_bPrint)
		{
			//vypocet minymalni mezery mezi dluhymi cary v grafu
//			y = (__int32)(m_rc.bottom-m_rc.top)/POMERTISK;
			d = ((m_rc.bottom-m_SizeTextOx.cy*2)-(m_rc.top))/y;
		}
		else
		{
			d = ((m_rc.bottom-m_SizeTextOx.cy*2)-(m_rc.top))/MEZERA;
		}
		if (d != 0)
		{
/*			d = (MaxY-MinY)/d;
			d *= 2;
			logar = log10(d)+0.5;
			logar = floor(logar);
			m_vzdalenostY = (pow(10,logar))/2;*/
			d = (MaxY-MinY)/d;
			logar = log10(d);
			if (d>0)
			{
				logar = floor(logar);
			}
			else
			{
				logar = ceil(logar);
			}
			cis = pow(10,logar);
			m_vzdalenostY = cis;
			if (d>cis)                         //*2
			{
				m_vzdalenostY = 2*cis;
				if(d>m_vzdalenostY)            //*5
				{
					m_vzdalenostY = 5*cis;     //*10
					if(d>m_vzdalenostY)
					{
						m_vzdalenostY = 10*cis;
					}
				}
			}
		}
		else
		{
			m_vzdalenostY = 0;
		}
		//////////////////////////////////////

		if (iTyp != VSE)//pro VSE by to bylo zbytecne udela se to az u grafu
		{
			//texty - cisilka u os
			dilek = m_vzdalenostX/5;

			d = (MinX/dilek);
			d = floor(d)*dilek;
			//spoceteni sirky textu u osy x
			PrevedNaChar( d, 4, ch);
			sizeTextMin = pDC->GetTextExtent(ch);

			d = (MaxX/dilek);
			d = floor(d)*dilek;
			PrevedNaChar( d, 4, ch);
			sizeTextMax = pDC->GetTextExtent(ch);
			if (sizeTextMin.cx > sizeTextMax.cx)
			{
				m_SizeTextOx = sizeTextMin;
			}
			else
			{
				m_SizeTextOx = sizeTextMax;
			}

			m_SizeTextOx.cy += m_carka; //pricteni carecek o osy x

			dilek = m_vzdalenostY/5;

			d = (MinY/dilek);
			d = floor(d)*dilek;
			//spoceteni sirky textu u osy y
			PrevedNaChar( d, 4, ch);
			sizeTextMin = pDC->GetTextExtent(ch);

			d = (MaxY/dilek);
			d = floor(d)*dilek;
			PrevedNaChar( d, 4, ch);
			sizeTextMax = pDC->GetTextExtent(ch);
			if (sizeTextMin.cx > sizeTextMax.cx)
			{
				m_SizeTextOy = sizeTextMin;
			}
			else
			{
				m_SizeTextOy = sizeTextMax;
			}
			m_SizeTextOy.cx += m_carka; //pricteni carecek o osy x

			//texty popisujici nazev osy
			SetSizePopis(pDC, iTyp);
		}

		//tenke a silne pero
		pen.CreatePen (PS_SOLID, 1, theApp.GetColor(COLOR_OSY));
		penB.CreatePen(PS_SOLID, 2, theApp.GetColor(COLOR_OSY));

		//nakresleni ramecku kolem grafu
		pOldPen = pDC->SelectObject(&penB);
		if (iTyp != VSE)
		{
			pDC->MoveTo(m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx,m_rc.top+m_SizeNapis.cy+m_SizeTextOy.cy/2);
			pDC->LineTo(m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx,m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy);
			pDC->LineTo(m_rc.right-X2,m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy);
		}
		pDC->SelectObject(&pen);
		if (iTyp != VSE)
		{
			pDC->LineTo(m_rc.right-X2,m_rc.top+m_SizeNapis.cy+m_SizeTextOy.cy/2);
			pDC->LineTo(m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx,m_rc.top+m_SizeNapis.cy+m_SizeTextOy.cy/2);
		}

		PopisGrafu(pDC, iTyp);//Popis os a carecky na nich
		pDC->SelectObject(pOldFont);

		//graf
		switch(iTyp)
		{
			case DATA: //0
			{
				//graf zde neni;
			}
			break;
			case PROFIL: //1
			{
				grafProfil(pDC);
			}
			break;
			case KORELACE: //2
			{
				grafKorelace(pDC, iTyp);
			}
			break;
			case NOR_KORELACE: //3
			{
				grafKorelace(pDC, iTyp);
			}
			break;
			case SPEKTRUM: //4
			{
				grafVykonovehoSpektra(pDC);
			}
			break;
			case HUSTOTA: //5
			{
				grafEnerSpekHustoty(pDC);
			}
			break;
			case SKLON: //6
			{
				grafStredAritSklonuProf(pDC);
			}
			break;
			case ROZLOZENI: //7
			{
				grafHustotaPravRozlozeniSouradnicProfilu(pDC);
			}
			break;
		}
		pDC->SelectObject(pOldPen);
	}
}

//graf profilu
void CProfilView::grafProfil(CDC* pDC)
{
	int i;
	__int32 x,y;
	CPen pen, penFil, *pOldPen;

	pen.CreatePen (PS_SOLID, 1, theApp.GetColor(COLOR_PROFIL));
	pOldPen = pDC->SelectObject(&pen);

	x = GetX(m_xData[0]);
	y = GetY(m_yData[0]);
	pDC->MoveTo(x, y);

	for(i=1; i<m_SizeX; i++)
	{
		x = GetX(m_xData[i]);
		y = GetY(m_yData[i]);
		pDC->LineTo(x, y);
	}

	//pokud je zapnut filtr tak je zobrazi
	if (m_Filtr.filtr)
	{

//		pen.CreatePen (PS_SOLID, 1, RGB(0, 150, 0));
		penFil.CreatePen (PS_SOLID, 1, theApp.GetColor(COLOR_FILTR));
		pDC->SelectObject(&penFil);

		x = GetX(m_xData[0]);
		y = GetY(m_yFiltr[0]);
		pDC->MoveTo(x, y);

		for(i=1; i<m_SizeX; i++)
		{
			x = GetX(m_xData[i]);
			y = GetY(m_yFiltr[i]);
			pDC->LineTo(x, y);
		}
		pDC->SelectObject(pOldPen);
	}
}

//graf auto korelace
void CProfilView::grafKorelace(CDC* pDC, __int16 iTyp)
{
	__int32 i,poc;
	__int32 x,y;

	//prvni bod
	x = GetX(m_xData[0],m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
	switch(iTyp)
	{
		case KORELACE: //2
		{
			y = GetY(m_yKorel[0], m_MinYKorel, m_MaxYKorel);
		}
		break;
		case NOR_KORELACE: //3
		{
			if (m_Dy != 0)
			{
				y = GetY(m_yKorel[0]/m_Dy, m_MinYKorel/m_Dy, m_MaxYKorel/m_Dy);
			}
			else
			{
				y = GetY(m_yKorel[0], m_MinYKorel, m_MaxYKorel);
			}
		}
		break;
		case VSE: //8
		{
			y = GetY(m_yKorel[0], m_MinYKorel, m_MaxYKorel);
		}
		break;
	}
	pDC->MoveTo(x, y);

	//dalsi body

	//vzhledem k tomu, jak jsou pocitany korelacni body je jich maximalne
	//o jeden mene nez omerenych bodu
	poc = (__int32)(m_SizeX*m_dKorel/100);
	if (poc == m_SizeX)
	{
		poc -= 1;
	}
	for(i=1; i<=poc; i++)
	{
		x = GetX(m_xData[i],m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
		switch(iTyp)
		{
			case KORELACE: //2
			{
				y = GetY(m_yKorel[i], m_MinYKorel, m_MaxYKorel);
			}
			break;
			case NOR_KORELACE: //3
			{
				y = GetY(m_yKorel[i]/m_Dy, m_MinYKorel/m_Dy, m_MaxYKorel/m_Dy);
			}
			break;
			case VSE: //8
			{
				y = GetY(m_yKorel[i], m_MinYKorel, m_MaxYKorel);
			}
			break;
		}
		pDC->LineTo(x, y);
	}
	//je zapnuto kresleni odectenych procent z Autokorelacni funkce po obalce
	if (m_Obalka)
	{
		KresliObalku(pDC, iTyp, poc);
	}
	//je zapnuto kresleni odectenych procent z Autokorelacni funkce
	if (m_bOdecetKorel)
	{
		CPen  pen, *pOldPen;
		char  ch[20];
		CSize sizeText;

		pen.CreatePen (PS_SOLID, 2, RGB(255, 0, 0));

		pOldPen = pDC->SelectObject(&pen);
		x = GetX(0, m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
		switch(m_iTyp)
		{
			case KORELACE: //2
			{
				y = GetY(m_odecetY, m_MinYKorel, m_MaxYKorel);
			}
			break;
			case NOR_KORELACE: //3
			{
				y = GetY(m_odecetY/m_Dy, m_MinYKorel/m_Dy, m_MaxYKorel/m_Dy);
			}
			break;
			case VSE: //8
			{
				y = GetY(m_odecetY, m_MinYKorel, m_MaxYKorel);
			}
			break;
		}
		pDC->MoveTo(x, y);

		x = GetX(m_odecetX, m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
		pDC->LineTo(x, y);
		//zobrazi %
		_gcvt( m_procenta, 3, ch);
		strcat( ch, "%" );
		sizeText = pDC->GetTextExtent(ch);
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(x-sizeText.cx/2,y-sizeText.cy-2, ch);
		pDC->SetTextColor(theApp.GetColor(COLOR_PROFIL));

		y = GetY(0, m_MinYKorel, m_MaxYKorel);
		pDC->LineTo(x, y);

		pDC->SelectObject(pOldPen);
	}
}

/*
	if (bSelected)
	{
		pDC->FillRect(rcAllLabels, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));

		pensh = ::CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));//bila
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));//bila
	}
*/

/*
void CProfilView::PopisGrafu(CDC* pDC, __int16 iTyp)
{
	CFont fontText, *pOldFont;

	//Nastaveni fondu
	fontText.CreateFont(
			15,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial CE");                  // lpszFacename
	pOldFont = pDC->SelectObject(&fontText);

	switch(m_iTyp)
	{
		case DATA: //0
		{
			//zde graf neni
		}
		break;
		case PROFIL: //1
		{
			OsaX(pDC,m_MinX,m_MaxX);
			OsaY(pDC,m_MinY,m_MaxY);
		}
		break;
		case KORELACE: //2
		{
			OsaX(pDC,m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
			OsaY(pDC,m_MinYKorel,m_MaxYKorel);
		}
		break;
		case NOR_KORELACE: //3
		{
			OsaX(pDC,m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
			if (m_Dy != 0)
			{
				OsaY(pDC,m_MinYKorel/m_Dy,m_MaxYKorel/m_Dy);
			}
			else
			{
				OsaY(pDC,m_MinYKorel,m_MaxYKorel);
			}
		}
		break;
		case SPEKTRUM: //4
		{
		}
		break;
		case HUSTOTA: //5
		{
		}
		break;
	}
	pDC->SelectObject(pOldFont);
}
*/

void CProfilView::grafVykonovehoSpektra(CDC* pDC)
{
	__int32 i;
	__int32 x,y;
	double MaxX;
	double dilOmega;

	MaxX = 2*PI*m_dFrek;
	dilOmega = MaxX/m_SizeX;

	x = GetX(0,0,MaxX);
	y = GetY(m_ySpektrum[0], m_MinYSpektrum, m_MaxYSpektrum);
	pDC->MoveTo(x, y);

	for(i=1; i<m_SizeX; i++)
	{
		x = GetX(dilOmega*i,0,MaxX);
		y = GetY(m_ySpektrum[i], m_MinYSpektrum, m_MaxYSpektrum);
		pDC->LineTo(x, y);
	}
}

void CProfilView::grafEnerSpekHustoty(CDC* pDC)
{
	__int32 i;
	__int32 x,y;
	double dilFrek;
	double d;

	d = (m_xData[1]-m_xData[0])/1000;
	dilFrek = m_dFrek/m_SizeX;

	x = GetX(0,0,m_dFrek);
	y = GetY(m_ySpektrum[0]/d, m_MinYSpektrum/d, m_MaxYSpektrum/d);
	pDC->MoveTo(x, y);

	for(i=1; i<m_SizeX; i++)
	{
		x = GetX(dilFrek*i,0,m_dFrek);
		y = GetY(m_ySpektrum[i]/d, m_MinYSpektrum/d, m_MaxYSpektrum/d);
		pDC->LineTo(x, y);
	}
}

void CProfilView::grafStredAritSklonuProf(CDC* pDC)
{
	__int32 i;
	__int32 x,y;
	double MaxX;

	MaxX = HL_SKLON;

	x = GetX(0,0,MaxX);
	y = GetY(m_ySklon[0], m_MinYSklon, m_MaxYSklon);
	pDC->MoveTo(x, y);

	for(i=1; i<HL_SKLON; i++)
	{
		x = GetX(i,0,MaxX);
		y = GetY(m_ySklon[i], m_MinYSklon, m_MaxYSklon);
		pDC->LineTo(x, y);
	}
}

void CProfilView::grafHustotaPravRozlozeniSouradnicProfilu(CDC* pDC)
{
	__int32 i;
	__int32 x,y;
	double MaxX;

	MaxX = HL_ROZL;

	//1.graf
	x = GetX(1,1,MaxX);
	y = GetY(m_yRozloz[0], 0, m_SizeY);
	pDC->MoveTo(x, y);

	for(i=2; i<=HL_ROZL; i++)
	{
		x = GetX(i,1,MaxX);
		y = GetY(m_yRozloz[i-1], 0, m_SizeY);
		pDC->LineTo(x, y);
	}
	//2.graf konci na 100%
	double hodnota;

	hodnota = m_yRozloz[0]; 
	x = GetX(1,1,MaxX);
	y = GetY(hodnota, 0, m_SizeY);
	pDC->MoveTo(x, y);

	for(i=2; i<=HL_ROZL; i++)
	{
		hodnota += m_yRozloz[i-1]; 
		x = GetX(i,1,MaxX);
		y = GetY(hodnota, 0, m_SizeY);
		pDC->LineTo(x, y);
	}
}

//popis grafu + carky v grafu
void CProfilView::PopisGrafu(CDC* pDC, __int16 iTyp)
{
	CString	str;
	CSize   sizeText;

	//osy X, Y a text u osy X
	switch(iTyp)
	{
		case DATA: //0
		{
			//zde graf neni
		}
		break;
		case PROFIL: //1
		{
			OsaX(pDC,m_MinX,m_MaxX);
			OsaY(pDC,m_MinY,m_MaxY);
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.right-sizeText.cx-X2, m_rc.bottom-sizeText.cy, str);
		}
		break;
		case KORELACE: //2
		{
			OsaX(pDC,m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
			OsaY(pDC,m_MinYKorel,m_MaxYKorel);
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.right-sizeText.cx-X2, m_rc.bottom-sizeText.cy, str);
		}
		break;
		case NOR_KORELACE: //3
		{
			OsaX(pDC,m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
			if (m_Dy != 0)
			{
				OsaY(pDC,m_MinYKorel/m_Dy,m_MaxYKorel/m_Dy);
			}
			else
			{
				OsaY(pDC,m_MinYKorel,m_MaxYKorel);
			}
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.right-sizeText.cx-X2, m_rc.bottom-sizeText.cy, str);
		}
		break;
		case SPEKTRUM: //4
		{
			OsaX(pDC,0,2*PI*m_dFrek);
			OsaY(pDC,m_MinYSpektrum,m_MaxYSpektrum);
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.right-sizeText.cx-X2, m_rc.bottom-sizeText.cy, str);
		}
		break;
		case HUSTOTA: //5
		{
			OsaX(pDC,0,m_dFrek);
			OsaY(pDC,m_MinYSpektrum/(m_xData[1]-m_xData[0])*1000,m_MaxYSpektrum/(m_xData[1]-m_xData[0])*1000);
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.right-sizeText.cx-X2, m_rc.bottom-sizeText.cy, str);
		}
		break;
		case SKLON: //6
		{
			OsaX(pDC,0,HL_SKLON); //HL_SKLON - 9
			OsaY(pDC,m_MinYSklon,m_MaxYSklon);
			// popis osy X
			str.LoadString(IDS_OSAX_SKLON);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.right-sizeText.cx-X2, m_rc.bottom-sizeText.cy, str);
		}
		break;
		case ROZLOZENI: //7
		{
			OsaX(pDC,1,HL_ROZL); //HL_ROZL - 20
			OsaY(pDC,0,m_SizeY);
			// popis osy X
			str.LoadString(IDS_OSAX_ROZLOZENI);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.right-sizeText.cx-X2, m_rc.bottom-sizeText.cy, str);
		}
		break;
	}

	// popis osy Y (text u ni)
	TextOsaY(pDC, iTyp);
}

void CProfilView::OsaX(CDC* pDC, double MinX, double MaxX)
{
	double  dilek;
	double  d,dd;
	__int16 i;
	__int32 x;
	char    ch[20];
	CSize   sizeText;

	if (m_vzdalenostX != 0)
	{
		dilek = m_vzdalenostX/5;

		d = (MinX/dilek);
		d = floor(d)*dilek;

		//Pokud by byl dilek pred zacatkem osy
		if (d<MinX)
		{
			d += dilek;
		}

		//urceni od ktereho podle poradi se zacnou delat dlohe cary
		//i=0 ihned
		dd = floor(d/(dilek*5));
		dd = dd*(dilek*5);
		dd = floor((dd-d)/dilek+0.5);
		i = abs(int(dd));

		for(d=d; d<=MaxX; d+=dilek)
		{
			if ((-0.000001<d)&&(d<0.000001))
			{
				d = 0;
			}
			x = GetX(d,MinX,MaxX);
			if (i==5)
			{
				pDC->MoveTo(x,m_rc.top+m_SizeNapis.cy+m_SizeTextOy.cy/2);
				pDC->LineTo(x,m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy+m_carka);
				i = 0;
				_gcvt( d, 5, ch);
				sizeText = pDC->GetTextExtent(ch);
				pDC->TextOut(x-sizeText.cx/2,m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy+m_carka, ch);
			}
			else
			{
				pDC->MoveTo(x,m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy-m_carka);
				pDC->LineTo(x,m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy+m_carka);
			}
			i++;
		}
	}
}

void CProfilView::OsaY(CDC* pDC, double MinY, double MaxY)
{
	double  dilek;
	double  d,dd;
	__int16 i;
	__int32 y;
	char    ch[20];
	CSize   sizeText;
	double  k;

	if (MinY > MaxY)//chyba pri deleni v predchozi fci,
					//velmi malo ojedinele. Dojde k tomu kdyz m_Dy je velmi male
	{
		MinY = m_MinYKorel;
		MaxY = m_MaxYKorel;
	}

	if (m_vzdalenostY != 0)
	{
		dilek = m_vzdalenostY/5;
		i = 0;

		d = (MinY/dilek);
		d = floor(d)*dilek;

		//Pokud by byl dilek pred zacatkem osy
		if (d<MinY)
		{
			d += dilek;
		}
		//oprava spastneho zaokrohleni

		//urceni od ktereho podle poradi se zacnou delat dlouhe cary
		//i=0 ihned
		dd = floor(d/(dilek*5));
		dd = dd*(dilek*5);
		dd = floor((dd-d)/dilek+0.5);
		i = abs(int(dd));

//		i = (int)((floor(d*2)/2 - d)/dilek);
		k  = floor(log10(dilek));
		k -= 3;
		k = pow(10, k);
		
		for(d=d; d<=MaxY; d+=dilek)
		{
			//problémy s pøesnými poèty (kdyz se násobí 0.1 dostanou s nepresne vysledky)
			if ((-k<d)&&(d<k))
			{
				d = 0;
			}
			y = GetY(d,MinY,MaxY);
			if (i==5)
			{
				pDC->MoveTo(m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx-m_carka,y);
				pDC->LineTo(m_rc.right-X2,y);
				i = 0;
				//autori funkce _gcvt si z programatoru delaji "dobry den"
				//jednou vraci cislo prevedene na string v desetinem tvaru jednom v exponencialnim
				//chh = _gcvt( d, 5, ch);
				PrevedNaChar( d, 4, ch);
				sizeText = pDC->GetTextExtent(ch);
				//Vyska popisku osy Y (text je na vysku) sizeText.cy
				pDC->TextOut(m_rc.left+(int)(m_SizePopisOy.cx*1.3),y-sizeText.cy/2, ch);
			}
			else
			{
				pDC->MoveTo(m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx-m_carka,y);
				pDC->LineTo(m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx+m_carka,y);
			}
			i++;
		}
	}
}

//Nacte data se souboru *.zad, tedy pokud soubor existuje
void CProfilView::NactiDataZad()
{
	char c[1];
	char ch[20];
	char popis[10000];
	CString str;
	CFile file;
	CFileException e;
	DWORD size, d;
	__int16 i;
	__int32 j,len;
	CString sFileName;

	sFileName = m_sFileName.Left(m_sFileName.ReverseFind('.'))+".zad";

	if (!file.Open(sFileName,CFile::modeRead,&e))
		return;
	else
	{
		size = file.GetLength();
		d = 0;
		i = 0;
		j = 0;
		//nacte Data
		while (d<size)
		{
			file.Read(c,1);
			d++;
			if ((c[0] != 10)&&(c[0] != 13))
			{
				if (i!=2)
				{
					str += c[0];
				}
				else
				{
					ch[j] = c[0];
					j++;
				}
			}
			else
			{
				file.Read(c,1);
				switch(i)
				{
					case 0:
						m_Datum = str;
						break;
					case 1:
						m_sTypPrvku = str;
						break;
					case 2:
						ch[j] = 0;
						len = (__int32)atol(ch);

						i++;
						file.Read(popis,len);
						popis[len] = 0;
						m_Popis = popis;
						break;
				}
				str = "";
				i++;
			}
		}
		file.Close();
	}
}

//Nacte data a zjisti z nacenych dat
//min a max pro delku a nerovnost
void CProfilView::NactiData()
{
	//otevre *.txt a nacte si z nej informace
	CFile file;
	CFileException e;
	char c[1];
	CString str;
	DWORD size, d;
	__int32 radek;
	//typ jestli jde o x nebo y; pravidelne se strida x, y
	__int16 typ; //x-0; y-1

	//presypaci hodiny
	::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	m_SizeXX  = m_SizeYY = SpoctiRadky();
	m_xxData  = (double*)calloc (m_SizeXX, sizeof(double));
	m_yyData  = (double*)calloc (m_SizeYY, sizeof(double));
	if (!file.Open(m_sFileName,CFile::modeRead,&e))
		return;
	else
	{
		//nacte Data
		size = file.GetLength();
		str = "";
		d = 0;
		typ = 0;
		radek = 0;
		while (d<size)
		{
			file.Read(c,1);
			d++;
			if ((c[0] != ' ')&&(c[0] != ';')&&(c[0] != 10)&&(c[0] != 13))
			{
				if(c[0] == ',')
				{
					c[0] = '.';
				}
				str += c[0];
			}
			else//else1
			{
				if (str.GetLength()>0) //ulozit cislo
				{
					switch(typ)
					{
						case 0: //x
						{
							m_xxData[radek] = atof(str);
							typ = 1;
						}
						break;
						case 1: //y
						{
							m_yyData[radek] = atof(str);
							typ = 0;
							radek++;
						}
						break;
					}
					str = "";
				}
			}//else1
		}
		if (radek < m_SizeXX)
		{
			m_SizeXX = m_SizeYY = radek;
		}
	}
	file.Close();

	//nacte data po kroku
	NactiDataPoKroku();

/*	//vypocte fitrovana data pokud je fitr vybran
	DataFiltr();

	//vypocete data pro autokorelacni funkci a normovanou korelacni funkci
	DataKorel();

	//vypocete data Vykonoveho spektra
	DataSpektrum();*/

	//presypaci hodiny jsou pryc
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));

}

//nacte data podle kroku
void CProfilView::NactiDataPoKroku()
{
	CString str;
	__int32 radekX,radek;
	//typ jestli jde o x nebo y; pravidelne se strida x, y
	double suma; //pro vypocet stredni hodnoty
	__int16 i; //jen pro plneni m_Yp[i], m_Yv[i]
	__int32 iKrok;

	suma = 0;

	m_SizeX     = m_SizeY = m_SizeXX/m_iKrok;
	m_xData     = (double*)calloc (m_SizeX, sizeof(double));
	m_yData     = (double*)calloc (m_SizeY, sizeof(double));
	m_yKorel    = (double*)calloc (m_SizeY, sizeof(double));
	m_yFiltr    = (double*)calloc (m_SizeY, sizeof(double));
	m_ySpektrum = (double*)calloc (m_SizeY, sizeof(double));
	m_ySklon    = (double*)calloc (HL_SKLON,sizeof(double)); //9
	m_yRozloz   = (double*)calloc (HL_ROZL, sizeof(double)); //20
	radek = 0;
	iKrok = 0;
	for (radekX=0; radekX<m_SizeXX; radekX++)
	{
		if (iKrok==0)
		{
			//x
			m_xData[radek] = m_xxData[radekX];
			if (radek == 0)
			{
				m_MinX = m_MaxX = m_xData[radek];
			}
			else
			{
				if (m_MinX > m_xData[radek])
				{
					m_MinX = m_xData[radek];
				}
				if (m_MaxX < m_xData[radek])
				{
					m_MaxX = m_xData[radek];
				}
			}
			//y
			m_yData[radek] = m_yyData[radekX];
			suma += m_yData[radek];
			if (radek == 0)
			{
				m_MinY = m_MaxY = m_yData[radek];
				m_Yp[0] = m_Yv[0] = m_yData[radek];
			}
			else
			{
				if (m_MinY > m_yData[radek])
				{
					m_MinY = m_yData[radek];
				}
				if (m_MaxY < m_yData[radek])
				{
					m_MaxY = m_yData[radek];
				}
				if (radek < POCET_PRO_RZ)
				{
					m_Yp[radek] = m_Yv[radek] = m_yData[radek];
					if (radek ==(POCET_PRO_RZ-1))
					{
						SeraditYp();
						SeraditYv();
					}
				}
				else
				{
					for (i=(POCET_PRO_RZ-1); i>=0; i--)
					{
						if (m_Yp[i] > m_yData[radek])
						{
							m_Yp[i] = m_yData[radek];
							SeraditYp();
							i = 0;
						}
					}
					for (i=0; i<POCET_PRO_RZ; i++)
					{
						if (m_Yv[i] < m_yData[radek])
						{
							m_Yv[i] = m_yData[radek];
							SeraditYv();
							i = 4;
						}
					}
				}
			}
			radek++;
		}

		iKrok++;
		if (iKrok == m_iKrok)
		{
			iKrok = 0;
		}
	}
	if (radek < m_SizeX)
	{
		m_SizeX = m_SizeY = radek;
	}
	//zaokrouhleni
	if ((-0.000001<suma)&&(suma<0.000001))
	{
		suma = 0;
	}
	//vypocet stredni hodnoty
	m_mx = suma/m_SizeX;
}

void CProfilView::DataKorel()
{
	double d;
	__int32 i, j;
	//N -> m_SizeX

	//musi se skoncit jeden prvek pred koncem
	//posledni v rade je m_yData[m_SizeX-1], protoze se cisluje uz od 0
	d = 0;
/*	for(i=0; i<(m_SizeX-1); i++)
	{
		for(j=0; j<=i; j++)
		{
			d += (m_yData[j]-m_mx)*(m_yData[j+1]-m_mx);
		}
		m_yKorel[i] = (double)d/(i+1);
		d = 0;

		if (i == 0)
		{
			m_Dy = m_yKorel[i];
			if (m_Dy == 0)
			{
				m_Dy = 0.0000001;
			}
			m_MinYKorel = m_MaxYKorel = m_yKorel[i];
		}
		else
		{
			if (m_MinYKorel > m_yKorel[i])
			{
				m_MinYKorel = m_yKorel[i];
			}
			if (m_MaxYKorel < m_yKorel[i])
			{
				m_MaxYKorel = m_yKorel[i];
			}
		}
	}*/
	//otocena autokorelacni funkce
/*	for(i=0; i<(m_SizeX-1); i++)
	{
		for(j=0; j<(m_SizeX-1-i); j++)
		{
			d += (m_yData[j]-m_mx)*(m_yData[j+1]-m_mx);
		}
		m_yKorel[i] = (double)d/(m_SizeX-1-i);
		d = 0;

		if (i == 0)
		{
			m_Dy = m_yKorel[i];
			if (m_Dy == 0)
			{
				m_Dy = 0.0000001;
			}
			m_MinYKorel = m_MaxYKorel = m_yKorel[i];
		}
		else
		{
			if (m_MinYKorel > m_yKorel[i])
			{
				m_MinYKorel = m_yKorel[i];
			}
			if (m_MaxYKorel < m_yKorel[i])
			{
				m_MaxYKorel = m_yKorel[i];
			}
		}
	}*/
/*	double dd;
	dd = 0;
	for(i=0; i<(m_SizeX-1); i++)
	{
		for(j=0; j<=i; j++)
		{
			d += (m_yData[j]-m_mx)*(m_yData[j+1]-m_mx);
			dd += (m_yData[j]-m_mx);
		}
		m_yKorel[i] = (double)d/(i+1) - (dd/(i+1))*(dd/(i+1));
		d = 0;
		dd = 0;

		if (i == 0)
		{
			m_Dy = m_yKorel[i];
			if (m_Dy == 0)
			{
				m_Dy = 0.0000001;
			}
			m_MinYKorel = m_MaxYKorel = m_yKorel[i];
		}
		else
		{
			if (m_MinYKorel > m_yKorel[i])
			{
				m_MinYKorel = m_yKorel[i];
			}
			if (m_MaxYKorel < m_yKorel[i])
			{
				m_MaxYKorel = m_yKorel[i];
			}
		}
	}*/
/*	double dd;
	dd = 0;
	for(i=0; i<m_SizeX; i++)
	{
		dd += (m_yData[i]-m_mx);
	}
	dd = (dd/(m_SizeX))*(dd/(m_SizeX));
	if ((-0.000001<dd)&&(dd<0.000001))
	{
		dd = 0;
	}
	for(i=0; i<(m_SizeX-1); i++)
	{
		for(j=0; j<=i; j++)
		{
			d += (m_yData[j]-m_mx)*(m_yData[j+1]-m_mx);
		}
		m_yKorel[i] = (double)d/(i+1) - dd;
		d = 0;

		if (i == 0)
		{
			m_Dy = m_yKorel[i];
			if (m_Dy == 0)
			{
				m_Dy = 0.0000001;
			}
			m_MinYKorel = m_MaxYKorel = m_yKorel[i];
		}
		else
		{
			if (m_MinYKorel > m_yKorel[i])
			{
				m_MinYKorel = m_yKorel[i];
			}
			if (m_MaxYKorel < m_yKorel[i])
			{
				m_MaxYKorel = m_yKorel[i];
			}
		}
	}*/
/*	__int32 poc;
	poc = (__int32)(m_SizeX*m_dKorel/100);
	if (poc == m_SizeX) //100% osy x
	{
		poc -= 1;
	}*/
	for(i=0; i<m_SizeX; i++)	//jde po lambda nebo-li poradi
	{
		if (m_Filtr.filtr)
		{
			for(j=0; j<(m_SizeX-i); j++)
			{
				d += (m_yFiltr[j])*(m_yFiltr[j+i]);
			}
		}
		else
		{
			for(j=0; j<(m_SizeX-i); j++)
			{
				d += (m_yData[j])*(m_yData[j+i]);
			}
		}
		m_yKorel[i] = (double)d/(j);
		d = 0;

		if (i == 0)
		{
			m_Dy = m_yKorel[i];
			if (m_Dy == 0)
			{
				m_Dy = 0.0000001;
			}
			m_MinYKorel = m_MaxYKorel = m_yKorel[i];
		}
		else
		{
			if (m_MinYKorel > m_yKorel[i])
			{
				m_MinYKorel = m_yKorel[i];
			}
			if (m_MaxYKorel < m_yKorel[i])
			{
				m_MaxYKorel = m_yKorel[i];
			}
		}
	}
}

//data pro Vykonove spektrum
void CProfilView::DataSpektrum()
{
	__int32 i;
	double dilOmega;

	dilOmega = 2*PI*m_dFrek/m_SizeX;
	for(i=0; i<m_SizeX; i++)	//jde po lambda nebo-li poradi
	{
		//neni potraba lisit filtrovana a nefitrovana data,
		//pocita se z koleracnich dat a nich je uz filtrace
		m_ySpektrum[i] = FceSpektrum(dilOmega*i);
		if (i == 0)
		{
			m_MinYSpektrum = m_MaxYSpektrum = m_ySpektrum[i];
		}
		else
		{
			if (m_MinYSpektrum > m_ySpektrum[i])
			{
				m_MinYSpektrum = m_ySpektrum[i];
			}
			if (m_MaxYSpektrum < m_ySpektrum[i])
			{
				m_MaxYSpektrum = m_ySpektrum[i];
			}
		}
	}
}

//vypocete Stredni aritmeticky sklon profilu
void CProfilView::DataSklon()
{
	double dHladina;
	double dKrokPoHlad;
	double tg;//tg sklonu krivky profilu
	double SumTg;//suma tg sklonu
	double deltaX;
	__int32 n; //pocet pruseciku krivky s hladinou
	__int32 i,j;

	dKrokPoHlad = m_Rm/(HL_SKLON+1);
	dHladina = m_MinY;
	deltaX = 5*m_xData[1]-m_xData[0];
	//hladiny
	for (i=0; i<HL_SKLON; i++)
	{
		dHladina += dKrokPoHlad;
		n = 0;
		SumTg = 0;

		//pruseciky v hladine
		for (j=0; j<(m_SizeY-1); j += m_iKrok)
		{
			//doslo pruseciku krivky a hladiny
			if (((m_yData[j]<=dHladina)&&(dHladina<=m_yData[j+1]))||
				((m_yData[j]>=dHladina)&&(dHladina>=m_yData[j+1])))
			{
				n++;
				tg = 0;
				if ((j+3)<m_SizeYY)
				{
					tg += m_yData[j+3];
				}
				if ((j+2)<m_SizeYY)
				{
					tg -= 9*m_yData[j+2];
				}
				tg += 45*m_yData[j+1];
				tg -= 45*m_yData[j];
				if (0<=(j-1))
				{
					tg += 9*m_yData[j-1];
				}
				if (0<=(j-2))
				{
					tg -= m_yData[j-2];
				}
				tg = fabs(tg/(60*deltaX));
				SumTg += tg;
			}
		}
		m_ySklon[i] = SumTg/n;
		if (i==0)
		{
			m_MinYSklon = m_MaxYSklon = m_ySklon[i];
		}
		else
		{
			if (m_MinYSklon > m_ySklon[i])
			{
				m_MinYSklon = m_ySklon[i];
			}
			if (m_MaxYSklon < m_ySklon[i])
			{
				m_MaxYSklon = m_ySklon[i];
			}
		}
	}
}

//vypocte Hustotu pravdepodobnosti rozlozeni souradnic profilu
void CProfilView::DataRozlozeni()
{
	__int32 i,j;
	double dHladina1, dHladina2; //rozmezi pro hladinu
	double dKrokPoHlad;
	DWORD d;

	dKrokPoHlad = m_Rm/HL_ROZL;
	dHladina1 = m_MinY;
	dHladina2 = dHladina1+dKrokPoHlad;
	//hladiny
	for (i=0; i<HL_ROZL; i++)
	{
		d = 0;
		//pruseciky v hladine
		for (j=0; j<m_SizeY; j += m_iKrok)
		{
			if (i == 0)
			{
				if ((dHladina1 <= m_yData[j]) && (m_yData[j])<=dHladina2)
				{
					d++;
				}
			}
			else
			{
				if ((dHladina1 < m_yData[j]) && (m_yData[j])<=dHladina2)
				{
					d++;
				}
			}
		}
		m_yRozloz[i] = d;
		dHladina1 = dHladina2;
		dHladina2 += dKrokPoHlad;
	}
//	m_yRozloz
}

//vypocte pro konkretni omega Vykonove spektrum
double CProfilView::FceSpektrum(double omega)
{
	__int32 i;
	double fce,f1,h,d_cast;

	fce = 0;
	f1  = 0;
	h   = 1;
	//aby se zrychlil vypocet tak se to spocita mimo cyklus
	d_cast = cos(omega*(m_xData[1]-m_xData[0]));
	for(i=0; i<m_SizeX; i++)	//jde po lambda nebo-li poradi
	{
		//i - lambda - poradi
//		f1 = m_yKorel[i]*cos(omega)*(m_xData[1]-m_xData[0]);
		f1 = m_yKorel[i]*d_cast;
		if ((0<i)&&(i<m_SizeX)) //krome krajnich hodnot
		{
			f1 *= 2;
		}
		fce += f1;
	}
	fce *= h/2; //vyseldek integralu
	fce *= 2/PI; 
	return fce;
}

void CProfilView::DataFiltr()
{
//	__int16 vlnitost; //0-vlnitost 1-drsnot
//	__int16 druh; //0-tojuhelnik 1-Gauss
//	__int16 delkaTyp;//0 - 0.08; 1 - 0.25; 2 - 0.8 3 - 2.5; 4 - 8; 5 - libovolne
//	double delka; //zakladni delka pro filtr, tedy pokud je zvolena jako volitena
	__int32 pocet;	//pocet bodu na ktere se rozlozi jeden bod
	double dpocet;	//to stejne, ale v presnosti double; jen kvuli presnejsimu zaokrohlovani
	__int32 cislo_V; //druha mocnina poradoveno cisla 
	double gauss_V;  //12/pocet; vaha pro Gaussuv filtr
	double delka; //delka cutt-off
//	double vaha; //narust vahy mezi dvemi sousednimi body
//	double narust; //to co se pricte k jednomu bodu
	__int32 i,j,jj,x;
	__int32 k,l; //jen pro odkladani cisel, aby se vypocty neprovadeli dokola
	__int16 vlnitost; //vlnitost = 1 jestli zvolen filtr vnitost, vlnitost = -1 jestli zvolen filtr drsnost
	
	if (m_Filtr.filtr)
	{
		switch(m_Filtr.delkaTyp)
		{
			//honoty jsou v um /mikrometry/
			case 0:
				delka = 80;//0.08
				break;
			case 1:
				delka = 250;//0.25
				break;
			case 2:
				delka = 800;//0.8
				break;
			case 3:
				delka = 2500;//2.5
				break;
			case 4:
				delka = 8000;//8
				break;
			case 5:
				delka = m_Filtr.delka*1000;
				break;
		}
		if (m_Filtr.vlnitost == 0)
		{
			vlnitost = 1;
		}
		else
		{
			vlnitost = -1;
		}

		//>>>>>>>>>>>>>>>>> Urceni delky
		//delka musi byt sude cilo, protoze na obe strany o dledaneho
		//cisla musi byt stejny mezer-useku

		//pak se k tomu musi +1, toto je pocet mezer mezi body
		dpocet = delka/(m_xData[1]-m_xData[0]);
		pocet = (__int32)(delka/(m_xData[1]-m_xData[0])+0.5);
		//zajisteni aby vysledek byl sudy
		pocet = (__int32)(pocet/2);
		pocet *= 2;
		//zde jse presnejsi pouzit nejblizsi vetsi sude cilo
		if ((dpocet-pocet)>1)
		{
			pocet += 2;
		}
		//<<<<<<<<<<<<<<<<< Urceni delky

		//>>>>>>>>>>>>>>>>> Urceni cisla vrcholu
		cislo_V = (__int32)(pocet/2)*(pocet/2);
		//<<<<<<<<<<<<<<<<< Urceni cisla vrcholu

		if (pocet > 0)
		{
////////////////////// TROJUHELNIK ///////////////////////////
			if (m_Filtr.druh == TROJUHELNIK)
			{
//1
//neni tu zaoblobeni
/*				//vynulovani fitru
				for (i=0; i<m_SizeY; i++)
				{
					m_yFiltr[i] = 0;
				}
				//>>>>>>>>>>>>>>>>> Vypocet bodu fitru
				for (i=0; i<m_SizeX; i++)
				{
					vaha = m_yData[i]/cislo_V;
					narust = 0;
					j = i-(pocet/2);
					if (j < 0)
					{
						narust = (-1)*j*vaha;
						j = 0;
					}
					else
					{
						narust += vaha;
					}

					for (j=j; (narust!=0)&&(j<=m_SizeX); j++)
					{
						m_yFiltr[j] += narust;
						if (j < i)
						{
							narust += vaha;
						}
						else
						{
							narust -= vaha;
							if ((-0.000001<narust)&&(narust<0.000001))
							{
								narust = 0;
							}
						}
					}
				}
				//<<<<<<<<<<<<<<<<< Vypocet bodu fitru*/
//2
				//>>>>>>>>>>>>>>>>> Vypocet bodu fitru
				for (x=0; x<m_SizeX; x++)
				{
					m_yFiltr[x] = 0;
					j = x-(pocet/2);//pocet je sudy
					jj = j;
					if (j<0)//nesmi se resit mimo graf
					{
						jj = 0;
					}
					//nulovvy bod 0.125*narust
					l = pocet/2-x; //pocet/2-(x-i) == pocet/2-x+i
					//"zaobleni"
					if (jj>0)
					{
						m_yFiltr[x] += m_yData[jj-1]*0.125*vlnitost;
					}
					//1.dil
					for (i=jj; i<=x; i++)
					{
						m_yFiltr[x] += m_yData[i]*(l+i)*vlnitost;
					}
					//"zaobleni"
					m_yFiltr[x] += m_yData[x]*(-0.25)*vlnitost;
					if (vlnitost == -1)//stred
					{
						m_yFiltr[x] += m_yData[x]*cislo_V;
					}
					//2.dil
					k = (j+(pocet-1));
					if (k >= m_SizeX)//nesmi se resit mimo graf
					{
						k = m_SizeX-1;
					}
					l = j+pocet;
					for (i=x+1; i<=k;i++)
					{
						m_yFiltr[x] += m_yData[i]*(l-i)*vlnitost;
					}
					//"zaobleni"
					if (k<(m_SizeX-1))
					{
						m_yFiltr[x] += m_yData[k]*0.125*vlnitost;
					}
					m_yFiltr[x] /= cislo_V;
				}
				//<<<<<<<<<<<<<<<<< Vypocet bodu fitru
			}
///////////////////////// GAUSS //////////////////////////////
			//>>>>>>>>>>>>>>>>> Vypocet bodu fitru
			if (m_Filtr.druh == GAUSS)
			{
				gauss_V = (double)12/pocet;
				for (x=0; x<m_SizeX; x++)
				{
					m_yFiltr[x] = 0;
					j = x-(pocet/2);//pocet je sudy
					jj = j;
					if (j<0)//nesmi se resit mimo graf
					{
						jj = 0;
					}
					k = (j+pocet);
					if (k > m_SizeX)//nesmi se resit mimo graf
					{
						k = m_SizeX;
					}
					l = j+pocet;
					for (i=jj; i<=k; i++)
					{
						m_yFiltr[x] += m_yData[i]*GaussovaFce(-6+(double)12/pocet*(i-j))*vlnitost;
					}
					if (vlnitost == -1) //pro drsnost se uprosred pricte m_yData
					{
						m_yFiltr[x] += m_yData[x]/gauss_V;
					}
					m_yFiltr[x] *= gauss_V;
				}
			}
			//<<<<<<<<<<<<<<<<< Vypocet bodu fitru
		}
		else //(pocet <= 0)
		{
			for (x=0; x<m_SizeX; x++)
			{
				m_yFiltr[x] = m_yData[x];
			}
		}
	}//if (m_Filtr.filtr)
}

//funkce ktera vrati hodnotu po dosazeni x do gaussovi fce (normalni rozdeleni)
double CProfilView::GaussovaFce(double x)
{
	double fce;

	fce = 1/(sqrt(2*PI))*exp(-x*x/2);

	return fce;
}

__int32 CProfilView::SpoctiRadky()
{
	__int32 radky;
	//otevre *.txt a zjisti pocet radku
	CFile file;
	CFileException e;
	char c[1];
	DWORD size, d;
	BOOL b; //b-TRUE, jestli posledni 2 znaky byli konec

	radky = 0;

	if (!file.Open(m_sFileName,CFile::modeRead,&e))
		return 0;
	else
	{
		//nacte Data
		size = file.GetLength();
		d = 0;
		b = FALSE;
		while (d<size)
		{
			file.Read(c,1);
			d++;
			
			if (c[0] == 13)
			{
				if (d==(size-1))
				{
					b = TRUE; //predposledni znak 13
				}
				file.Read(c,1);
				d++;
				if (c[0] == 10)
				{
					radky++;
				}
				if ((b)&&(c[0] != 10))
				{
					b = FALSE; //ale posledni znak neni 10
				}
			}
		}
		if (!b) //posledni znaky v souboru nebylo zakonceni radku
		{
			radky++;
		}
	}

	file.Close();

	return radky;
}

//vrati podle hodnty x hodntu v pixelech
__int32 CProfilView::GetX(double d)
{
	double cis;
	
	if (m_MaxX-m_MinX)
	{
		cis = ((m_rc.right-X2)-(m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx))/(m_MaxX-m_MinX)*(d-m_MinX) + m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx+0.5;
		cis = floor(cis);
	}
	else
	{
		cis = m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx;
	}

	return (__int32)cis;
}

//vrati podle hodnoty x hodnotu v pixelech podle velikosti osy
__int32 CProfilView::GetX(double d, double minx, double maxx)
{
	double cis;
	
	if (maxx-minx)
	{
		cis = ((m_rc.right-X2)-(m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx))/(maxx-minx)*(d-minx) + m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx+0.5;
		cis = floor(cis);
	}
	else
	{
		cis = m_rc.left+m_SizePopisOy.cx+m_SizeTextOy.cx;
	}

	return (__int32)cis;
}

//vrati podle hodnoty y hodnotu v pixelech
__int32 CProfilView::GetY(double d)
{
	double cis;
	
	if ((m_MaxY-m_MinY)!=0)
	{
		cis = m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy-((m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy)-(m_rc.top+m_SizeNapis.cy+m_SizeTextOy.cy/2))/(m_MaxY-m_MinY)*(d-m_MinY) +0.5;
		cis = floor(cis);
	}
	else
	{
		cis = m_rc.top+m_SizeNapis.cy+m_SizeTextOy.cy/2;
	}

	return (__int32)cis;
}

//vrati podle hodnoty y hodnotu v pixelech podle velikosti osy
__int32 CProfilView::GetY(double d, double miny, double maxy)
{
	double cis;
	
	if ((maxy-miny)!=0)
	{
		cis = m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy-((m_rc.bottom-m_SizeTextOx.cy-m_SizePopisOx.cy)-(m_rc.top+m_SizeNapis.cy+m_SizeTextOy.cy/2))/(maxy-miny)*(d-miny) +0.5;
		cis = floor(cis);
	}
	else
	{
		cis = m_rc.top+m_SizeNapis.cy+m_SizeTextOy.cy/2;
	}

	return (__int32)cis;
}

//Ulozi parametry v zalozce zadani 
void CProfilView::UlozZalZadani()
{
	CFile file;
	CFileException e;
	CString sFileName;
	char c[2];
	char ch[20];

	sFileName = m_sFileName.Left(m_sFileName.ReverseFind('.'))+".zad";

	if (!file.Open(sFileName,CFile::modeCreate|CFile::modeWrite,&e))
		return;
	else
	{
		c[0] = 13;
		c[1] = 10;
		file.Write(m_Datum,strlen(m_Datum));
		file.Write(c,2);
		file.Write(m_sTypPrvku,strlen(m_sTypPrvku));
		file.Write(c,2);
		_ltoa(strlen(m_Popis),ch,10);
		file.Write(ch,strlen(ch));//delka m_Popis
		file.Write(c,2);
		file.Write(m_Popis,strlen(m_Popis));
		file.Write(c,2);
		file.Close();
	}
}

void CProfilView::OnDataMonosti() 
{
	CDataMoznosti dlg;
	CString str;
	BOOL b; //urci zmenu, jestli ma dojit k prekresleni
	BOOL bKrok;   //urci zmenu kroku, jestli se maji data znovu nacist
	BOOL bTyp;    //urci zmenu typu, jestli se maji data znovu nacist
	BOOL bTypTab; //urci zmenu typu, jestli se maji data znovu nacist jako graf nebo tabulka
	BOOL bFrek;   //urci zmenu krekvence, jestli se maji spocitat data pro Vykonove spektrum a Energetickou spek. hustotu
	BOOL bKorel;  //urci zmenu u korelacnich funkci, jestli se maji znovu prekrelit
	BOOL bZadani; //urci zmenu v zalozce zadani
//	m_ySklon    = (double*)calloc (HL_SKLON,sizeof(double)); //9
//	m_yRozloz   = (double*)calloc (HL_ROZL, sizeof(double)); //20

	b       = FALSE;
	bKrok   = FALSE;
	bTyp    = FALSE;
	bTypTab = FALSE;
	bFrek   = FALSE;
	bKorel  = FALSE;
	bZadani = FALSE;
	dlg.dlgMoznosti0.m_moznosti = m_iTyp;
	dlg.dlgMoznosti0.m_bFiltr = m_Filtr.filtr;
	dlg.dlgMoznosti1.m_iVlnitost = m_Filtr.vlnitost;
	dlg.dlgMoznosti1.m_iDruh = m_Filtr.druh;
	dlg.dlgMoznosti1.m_iDelkaTyp = m_Filtr.delkaTyp;
	dlg.dlgMoznosti1.m_dDelka = m_Filtr.delka;
	dlg.dlgMoznosti0.m_iKrok = m_iKrok;
	dlg.dlgMoznosti3.m_dFrek = m_dFrek;
	dlg.dlgMoznosti2.m_dKorel = m_dKorel;
	dlg.dlgMoznosti4.m_Datum = m_Datum;
	dlg.dlgMoznosti4.m_sTypPrvku = m_sTypPrvku;
	dlg.dlgMoznosti4.m_Popis = m_Popis;
	int nRet = dlg.DoModal();
	if (nRet <=0 || nRet==IDABORT)
	{
		CString  strMessage, strMessage1;
		//Dialog provedl chybu.
		strMessage.LoadString(IDS_DIALOG_CRASH);
		strMessage1.LoadString(IDS_DIALOG_CRASH_TITLE);
		MessageBox(strMessage, strMessage1, MB_OK|MB_ICONERROR);
	}

	if (nRet==IDOK)
	{
		m_bVse = FALSE;//nastaveni pro kresleni jednoho grafu

		//presypaci hodiny
		::SetCursor(::LoadCursor(NULL, IDC_WAIT));
		//>>zjisteni jestli doslo ke zmene
		if (m_iTyp != dlg.dlgMoznosti0.m_moznosti)
		{
//			b = TRUE;
/*			//nema smysl, aby pri zmene KORELACE a NOR_KORELACE se uvazovala zmena bTyp
			if (!((m_iTyp == KORELACE) && (dlg.dlgMoznosti0.m_moznosti == NOR_KORELACE)) ||
				((m_iTyp == NOR_KORELACE) && (dlg.dlgMoznosti0.m_moznosti == KORELACE)))
			{
				bTyp = TRUE;
			}*/
			bTyp = TRUE;
			if (m_iTyp == DATA)//zobrazovala se tabulka nyni to bude graf
			{
				bTypTab = TRUE;
			}
		}
		if (m_Filtr.filtr != dlg.dlgMoznosti0.m_bFiltr)
		{
			b = TRUE;
		}
		if (m_Filtr.vlnitost != dlg.dlgMoznosti1.m_iVlnitost)
		{
			b = TRUE;
		}
		if (m_Filtr.druh != dlg.dlgMoznosti1.m_iDruh)
		{
			b = TRUE;
		}
		if (m_Filtr.delkaTyp != dlg.dlgMoznosti1.m_iDelkaTyp)
		{
			b = TRUE;
		}
		if (m_Filtr.delka != dlg.dlgMoznosti1.m_dDelka)
		{
			b = TRUE;
		}
		if (m_iKrok != dlg.dlgMoznosti0.m_iKrok)
		{
			bKrok = TRUE;
			b = TRUE;
		}
		if (m_dFrek != dlg.dlgMoznosti3.m_dFrek)
		{
			bFrek = TRUE;
			b = TRUE;
		}
		if ((m_dKorel != dlg.dlgMoznosti2.m_dKorel)&&
			((dlg.dlgMoznosti0.m_moznosti == KORELACE)||(dlg.dlgMoznosti0.m_moznosti == NOR_KORELACE)||(dlg.dlgMoznosti0.m_moznosti == VSE)))
		{
			bKorel = TRUE;
		}
		if ((m_Datum != dlg.dlgMoznosti4.m_Datum)||
			(m_Popis != dlg.dlgMoznosti4.m_Popis)||
			(m_sTypPrvku != dlg.dlgMoznosti4.m_sTypPrvku))
		{
			bZadani = TRUE;//zmena v zalozce Zadani
		}
		//<<zjisteni jestli doslo ke zmene
		m_iTyp = dlg.dlgMoznosti0.m_moznosti;
		m_Filtr.filtr = dlg.dlgMoznosti0.m_bFiltr;
		m_Filtr.vlnitost = dlg.dlgMoznosti1.m_iVlnitost;
		m_Filtr.druh = dlg.dlgMoznosti1.m_iDruh;
		m_Filtr.delkaTyp = dlg.dlgMoznosti1.m_iDelkaTyp;
		m_Filtr.delka = dlg.dlgMoznosti1.m_dDelka;
		m_iKrok = dlg.dlgMoznosti0.m_iKrok;
		m_dFrek = dlg.dlgMoznosti3.m_dFrek;
		m_dKorel = dlg.dlgMoznosti2.m_dKorel;
		theApp.SetFrekvence(m_dFrek);
		theApp.SetProcentaKorel(m_dKorel);
		if (bKrok)
		{
			NactiDataPoKroku();
			m_bSklon = FALSE;//nejsou spoctena data pro Stredni aritmeticky skon profilu
			m_bRozloz = FALSE;//nejsou spoctena data pro Hustotu pravdepob. rozlozeni souradnic profilu
			//dale to navstivi: if (b)
		}
		if (bTypTab) //pokud byla zobrazena tabulka a ted se zobrazuje
		{		  //obr. je potreba nastavit ScrollSizes
			InitScrollSizes();
		}
		if (b)
		{
			DataFiltr();
			m_bKorel = FALSE;//nejsou spoctena data pro Korelacni funkci
			m_bSpektrum = FALSE;//nejsou spoctena data pro Vykonove spektrum
			m_bOdecetKorel = FALSE;
			m_Obalka = FALSE;
//			DataKorel();
//			Invalidate();
		}
		if ((m_iTyp == KORELACE) || (m_iTyp == NOR_KORELACE) || (m_iTyp == VSE))
		{
			if (!m_bKorel)
			{
				DataKorel();
				m_bKorel = TRUE;
			}
		}
		//Vykonove spektrum a Spektralni hustota
		if ((m_iTyp == SPEKTRUM) || (m_iTyp == HUSTOTA) || (m_iTyp == VSE))
		{
			if (!m_bSpektrum)
			{
				//kdyby nebyla spocitana autokorelacni fce
				if (!m_bKorel)
				{
					DataKorel();
					m_bKorel = TRUE;
				}
				DataSpektrum();
				m_bSpektrum = TRUE;
			}
		}
		//Stredni aritmeticky sklon profilu
		if ((m_iTyp == SKLON) || (m_iTyp == VSE))
		{
			if (!m_bSklon)
			{
				DataSklon();
				m_bSklon = TRUE;
			}
		}
		//Hustota pravdepodobnosti rozlozeni souradnic profilu
		if ((m_iTyp == ROZLOZENI) || (m_iTyp == VSE))
		{
			if (!m_bRozloz)
			{
				DataRozlozeni();
				m_bRozloz = TRUE;
			}
		}
		if (m_iTyp == DATA)
		{
			if (!m_bKorel)
			{
				DataKorel();
				m_bKorel = TRUE;
			}
			if (!m_bSpektrum)
			{
				DataSpektrum();
				m_bSpektrum = TRUE;
			}
			if (!m_bSklon)
			{
				DataSklon();
				m_bSklon = TRUE;
			}
			if (!m_bRozloz)
			{
				DataSklon();
				m_bRozloz = TRUE;
			}
		}
		if (bFrek)
		{
			DataSpektrum();
/*			if (!b)
			{
				Invalidate();
			}*/
		}
		if ((b)||(bTyp)||(bKorel))
		{
			Invalidate();
		}
		//ulozeni informaci o materialu (zal. Zadani)
		if (bZadani)
		{
			m_Datum = dlg.dlgMoznosti4.m_Datum;
			m_sTypPrvku = dlg.dlgMoznosti4.m_sTypPrvku;
			m_Popis = dlg.dlgMoznosti4.m_Popis;
			UlozZalZadani();
		}

		//presypaci hodiny jsou pryc
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
}

void CProfilView::OnDataVlastnosti() 
{
	CVlastnosti dlg;

	dlg.m_Rm = m_Rm;
	dlg.m_Rz = m_Rz;
	dlg.m_Ra = m_Ra;
	dlg.m_Rq = m_Rq;
	dlg.m_Rp = fabs(m_MinY);
	dlg.m_Rv = fabs(m_MaxY);
	int nRet = dlg.DoModal();
	if (nRet <=0 || nRet==IDABORT)
	{
		CString  strMessage, strMessage1;
		//Dialog provedl chybu.
		strMessage.LoadString(IDS_DIALOG_CRASH);
		strMessage1.LoadString(IDS_DIALOG_CRASH_TITLE);
		MessageBox(strMessage, strMessage1, MB_OK|MB_ICONERROR);
	}
}

void CProfilView::SpoctiDrsnosti()
{
	__int32 i;
	double  d,dd;

	//Rm
	m_Rm = m_MaxY-m_MinY;

	//Rz
	d = 0;
	for (i=0; i<POCET_PRO_RZ; i++)
	{
		if (m_Yp[i]>0)
		{
			d += m_Yp[i];
		}
		else
		{
			d += (-1)*m_Yp[i];
		}
		if (m_Yv[i]>0)
		{
			d += m_Yv[i];
		}
		else
		{
			d += (-1)*m_Yv[i];
		}
	}
	m_Rz = d/5;

	//Ra
	d = 0;
	for (i=0; i<m_SizeX; i++)
	{
		dd = m_yData[i]-m_mx;
		if (dd<0)
		{
			dd *= -1;
		}
		d += dd;
	}
	m_Ra = d/m_SizeX;

	//Rq
	d = 0;
	for (i=0; i<m_SizeX; i++)
	{
		d += pow(m_yData[i],2);
	}
	d = d/m_SizeX;
	m_Rq = sqrt(d);
}

//seradi m_Yp podle velikosti vzestupne
void CProfilView::SeraditYp()
{
	__int16 i,j;
	double d;
	__int16 poc; //pocet m_Yp

	poc = POCET_PRO_RZ;
	for (j=(poc-1); j>=0; j--)
	{
		for (i=0; i<j; i++)
		{
			if (m_Yp[i]>m_Yp[i+1])
			{
				d = m_Yp[i];
				m_Yp[i] = m_Yp[i+1];
				m_Yp[i+1] = d;
			}
		}
	}
}

//seradi m_Yv podle velikosti vzestupne
void CProfilView::SeraditYv()
{
	__int16 i,j;
	double d;
	__int16 poc; //pocet m_Yv

	poc = POCET_PRO_RZ;
	for (j=(poc-1); j>=0; j--)
	{
		for (i=0; i<j; i++)
		{
			if (m_Yv[i]>m_Yv[i+1])
			{
				d = m_Yv[i];
				m_Yv[i] = m_Yv[i+1];
				m_Yv[i+1] = d;
			}
		}
	}
}

void CProfilView::OnUpdateAutokorelanfunkceOdecethodnotyzgrafu(CCmdUI* pCmdUI) 
{
	//KORELACE: //2
	//NOR_KORELACE: //3
	pCmdUI->Enable((m_iTyp==KORELACE)||(m_iTyp==NOR_KORELACE));
}

void CProfilView::KresliObalku(CDC* pDC, __int16 iTyp, __int32 poc)
{
	__int32 i;
	__int32 x,y;
	CPen  pen, *pOldPen;
	BOOL b;//zjisteni jestli se jedna o pocatecni bod

	pen.CreatePen (PS_SOLID, 1, RGB(0, 0, 255));

	pOldPen = pDC->SelectObject(&pen);

	b = TRUE;
	//horni obalka
	for (i=0; i<=poc; i++)
	{
		if (m_yObalka1[i]>m_MinYKorel-1)
		{
			x = GetX(m_xData[i], m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
			switch(m_iTyp)
			{
				case KORELACE: //2
				{
					y = GetY(m_yObalka1[i], m_MinYKorel, m_MaxYKorel);
				}
				break;
				case NOR_KORELACE: //3
				{
					y = GetY(m_yObalka1[i]/m_Dy, m_MinYKorel/m_Dy, m_MaxYKorel/m_Dy);
				}
				break;
				case VSE: //8
				{
					y = GetY(m_yObalka1[i], m_MinYKorel, m_MaxYKorel);
				}
				break;
			}
			if (b) //prvni bod
			{
				pDC->MoveTo(x, y);
				b = FALSE;
			}
			else
			{
				pDC->LineTo(x, y);
			}
		}
	}

	b = TRUE;

	//dolni obalka
	for (i=0; i<=poc; i++)
	{
		if (m_yObalka2[i]<m_MaxYKorel+1)
		{
			x = GetX(m_xData[i], m_MinX,m_MinX+(m_MaxX-m_MinX)*m_dKorel/100);
			switch(m_iTyp)
			{
				case KORELACE: //2
				{
					y = GetY(m_yObalka2[i], m_MinYKorel, m_MaxYKorel);
				}
				break;
				case NOR_KORELACE: //3
				{
					y = GetY(m_yObalka2[i]/m_Dy, m_MinYKorel/m_Dy, m_MaxYKorel/m_Dy);
				}
				break;
			}
			if (b) //prvni bod
			{
				pDC->MoveTo(x, y);
				b = FALSE;
			}
			else
			{
				pDC->LineTo(x, y);
			}
		}
	}

	pDC->SelectObject(pOldPen);
}

void CProfilView::OnAutokorelanfunkceOdecethodnotyzgrafu() 
{
	CODecetKorel dlg;

	dlg.m_yKorel = m_yKorel;
	dlg.m_xData  = m_xData;
	dlg.m_Dy = m_Dy;
	dlg.m_MinYKorel = m_MinYKorel;
	dlg.m_MaxYKorel = m_MaxYKorel;
	dlg.m_SizeX = m_SizeX;
	dlg.m_misto = m_procenta;
	dlg.m_dKorel = m_dKorel;
	int nRet = dlg.DoModal();
	if (nRet <=0 || nRet==IDABORT)
	{
		CString  strMessage, strMessage1;
		//Dialog provedl chybu.
		strMessage.LoadString(IDS_DIALOG_CRASH);
		strMessage1.LoadString(IDS_DIALOG_CRASH_TITLE);
		MessageBox(strMessage, strMessage1, MB_OK|MB_ICONERROR);
	}
	if (nRet==IDOK)
	{
		if (dlg.m_bVypocet) //vypocet probehl uspesne
		{
			m_bOdecetKorel = TRUE;
			m_odecetX = dlg.m_odecetX;
			m_odecetY = dlg.m_odecetY;
			m_procenta = dlg.m_misto;
			if (dlg.m_typ == 1)//obalka
			{
				m_Obalka = TRUE;
				m_yObalka1 = dlg.m_yObalka1;	//horní obálka
				m_yObalka2 = dlg.m_yObalka2;	//dolní obálka
			}
			else
			{
				m_Obalka = FALSE;
			}
			Invalidate();
		}
		else
		{
			m_bOdecetKorel = FALSE;
			m_Obalka       = FALSE;
			Invalidate();
		}
	}
	if (nRet==IDCANCEL) //zrusit
	{
		m_bOdecetKorel = FALSE;
		m_Obalka = FALSE;
		Invalidate();
	}
}

//inicialzace okna jeho velikosti
void CProfilView::InitScrollSizes()
{
	CSize sizeTotal;

	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CProfilView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	m_rc = pInfo->m_rectDraw;
	m_bPrint = TRUE;
	m_rc -= CRect(X1Print, Y1Print, X2Print, Y2Print);

	CScrollView::OnPrint(pDC, pInfo);
	m_bPrint = FALSE;
}

//prevede double cislo 'd' na char* 'ch'
//'count' pocet cislic za desetinou teckou, pocitano od prvni nenulove cilice,
//ale zbytecne nuly na konci za dasetinou teckou budou ostraneny
//napr. d=0.00124586, count=4, tak ch="0.001246"
void CProfilView::PrevedNaChar(double d, int count, char *ch)
{
	double d_poz;
	BOOL znamenko; //TRUE - d je zaporne; FALSE - d j kladne
	int  decimal, sign, poz, i, z;
	char *buffer;
	CString str;

	znamenko = FALSE;

	//jedna se o zaporne cislo
	if (d<0)
	{
		znamenko = TRUE;
		d *= -1;
	}

	//zjisteni radu, kde cilo zacina
	//2 .. rad 100
	//1 .. rad 10
	//0 .. rad 1
	//-1 .. rad 0.1
	d_poz = floor(log10(d));

	if (d_poz<1) //prvni nenulove cislice jsou za desetinou teckou
	{
		count -= (int)(d_poz+1);
	}

	//vraceni cislic ve stringu, lae bez desetine tecky
	buffer = _fcvt( d, count, &decimal, &sign );

	//cislo je mesi nez 0
	if(decimal<=0)
	{
		z=0;
		if(znamenko)//zaporne cislo
		{
			ch[0]='-';
			z=1;
		}
		ch[0+z]='0';
		ch[1+z]='.';
		for(i=1; i<=-decimal; i++)
		{
			ch[1+i+z]='0';
		}
		ch[1+i+z]=0;
		strcat(ch, buffer);
	}
	if(decimal>0)
	{
		z=0;
		if(znamenko)//zaporne cislo
		{
			ch[0]='-';
			z=1;
		}
		for(i=0; i<decimal; i++)
		{
			ch[i+z]=buffer[i];
		}
		if(buffer[i]!=0)
		{
			ch[i+z]='.';
		}
		while(buffer[i]!=0)
		{
			ch[i+1+z]=buffer[i];
			i++;
		}
		ch[i+1+z]=buffer[i];
	}

	//orezani prebytecnych 0
	str = ch;
	poz = str.Find('.');
	for(i=str.GetLength()-1; i>=poz; i--)
	{
		if (ch[i] == '0')
		{
			ch[i] = 0;
		}
		else //nema smysl pokracovat
		{
			if (ch[i] == '.')
			{
				ch[i] = 0;
			}
			i = poz;
		}
	}
}

void CProfilView::TextOsaY(CDC* pDC, __int16 iTyp)
{
/*	double  MinX, MaxX; //min max; osa x
	double  MinY, MaxY; //min max; osa y
	CPen    pen, penB, *pOldPen;
	char    ch[1024];
	CSize   size, sizeTextMin, sizeTextMax;*/
	CFont fontText, *pOldFont;
	__int32 y;
	CString	str;
	CSize   sizeText;

	//vypocet vysky textu pro nadpis, zavisi na rozmeru "y" u aktualniho okna
	y = GetVyskaTextu();

	//Nastaveni fondu
	fontText.CreateFont(
			y,                         // nHeight
			0,                         // nWidth
			900,                       // angle of escapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial CE");               // lpszFacename
	pOldFont = pDC->SelectObject(&fontText);

	switch(iTyp)
	{
		case DATA: //0
		{
			//zde graf neni
		}
		break;
		case PROFIL: //1
		{
			str.LoadString(IDS_OSAY_PROFIL);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.left, m_rc.top+sizeText.cx+m_SizeNapis.cy+m_SizeTextOy.cy/2, str);
		}
		break;
		case KORELACE: //2
		{
			str.LoadString(IDS_OSAY_KOREL);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.left, m_rc.top+sizeText.cx+m_SizeNapis.cy+m_SizeTextOy.cy/2, str);
		}
		break;
		case NOR_KORELACE: //3
		{
			str.LoadString(IDS_OSAY_NOR_KOREL);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.left, m_rc.top+sizeText.cx+m_SizeNapis.cy+m_SizeTextOy.cy/2, str);
		}
		break;
		case SPEKTRUM: //4
		{
			str.LoadString(IDS_OSAY_SPEKTRUM);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.left, m_rc.top+sizeText.cx+m_SizeNapis.cy+m_SizeTextOy.cy/2, str);
		}
		break;
		case HUSTOTA: //5
		{
			str.LoadString(IDS_OSAY_SPETRALNI_HUSTOTA);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.left, m_rc.top+sizeText.cx+m_SizeNapis.cy+m_SizeTextOy.cy/2, str);
		}
		break;
		case SKLON: //6
		{
			str.LoadString(IDS_OSAY_SKLON);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.left, m_rc.top+sizeText.cx+m_SizeNapis.cy+m_SizeTextOy.cy/2, str);
		}
		break;
		case ROZLOZENI: //7
		{
			str.LoadString(IDS_OSAY_ROZLOZENI);
			sizeText = pDC->GetTextExtent(str);
			pDC->TextOut(m_rc.left, m_rc.top+sizeText.cx+m_SizeNapis.cy+m_SizeTextOy.cy/2, str);
		}
		break;
	}

	pDC->SelectObject(pOldFont);

}

//vypocet vysky textu pro nadpis, zavisi na rozmeru "y" u aktualniho okna
__int32 CProfilView::GetVyskaTextu()
{
	return (__int32)(m_rc.bottom-m_rc.top)/POMEROSY;
}

//popis textu u os X,Y
void CProfilView::SetSizePopis(CDC* pDC, __int16 iTyp)
{
	CString str;

	switch(iTyp)
	{
		case DATA: //0
		{
			//zde graf neni
		}
		break;
		case PROFIL: //1
		{
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			m_SizePopisOx = pDC->GetTextExtent(str);
			str.LoadString(IDS_OSAY_PROFIL);
			m_SizePopisOy = pDC->GetTextExtent(str);
		}
		break;
		case KORELACE: //2
		{
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			m_SizePopisOx = pDC->GetTextExtent(str);
			str.LoadString(IDS_OSAY_KOREL);
			m_SizePopisOy = pDC->GetTextExtent(str);
		}
		break;
		case NOR_KORELACE: //3
		{
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			m_SizePopisOx = pDC->GetTextExtent(str);
			str.LoadString(IDS_OSAY_NOR_KOREL);
			m_SizePopisOy = pDC->GetTextExtent(str);
		}
		break;
		case SPEKTRUM: //4
		{
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			m_SizePopisOx = pDC->GetTextExtent(str);
			str.LoadString(IDS_OSAY_SPEKTRUM);
			m_SizePopisOy = pDC->GetTextExtent(str);
		}
		break;
		case HUSTOTA: //5
		{
			// popis osy X
			str.LoadString(IDS_OSAX_DELKA);
			m_SizePopisOx = pDC->GetTextExtent(str);
			str.LoadString(IDS_OSAY_SPETRALNI_HUSTOTA);
			m_SizePopisOy = pDC->GetTextExtent(str);
		}
		break;
		case SKLON: //6
		{
			// popis osy X
			str.LoadString(IDS_OSAX_SKLON);
			m_SizePopisOx = pDC->GetTextExtent(str);
			str.LoadString(IDS_OSAY_SKLON);
			m_SizePopisOy = pDC->GetTextExtent(str);
		}
		break;
		case ROZLOZENI: //7
		{
			// popis osy X
			str.LoadString(IDS_OSAX_ROZLOZENI);
			m_SizePopisOx = pDC->GetTextExtent(str);
			str.LoadString(IDS_OSAY_ROZLOZENI);
			m_SizePopisOy = pDC->GetTextExtent(str);
		}
		break;
	}
	PrehodXY(&m_SizePopisOy);
}

void CProfilView::PrehodXY(CSize *size)
{
	size->cx += size->cy;
	size->cy =  size->cx - size->cy;
	size->cx -= size->cy;
}

void CProfilView::OnDataNastavenbarev() 
{
	CSetColor dlg;
	int       nRet;
	int       i;

	//vybrany je 1. radiobutton
	dlg.m_radColor = 0;

	//nastaveni barev
	for (i=0; i<COUNT_COL; i++)
	{
		dlg.SetColor(i, theApp.GetColor(i));
	}

	nRet = dlg.DoModal();
	if (nRet <=0 || nRet==IDABORT)
	{
		CString  strMessage, strMessage1;
		//Dialog provedl chybu.
		strMessage.LoadString(IDS_DIALOG_CRASH);
		strMessage1.LoadString(IDS_DIALOG_CRASH_TITLE);
		MessageBox(strMessage, strMessage1, MB_OK|MB_ICONERROR);
	}
	if (nRet==IDOK)
	{
		//ziskani barev
		for (i=0; i<COUNT_COL; i++)
		{
			theApp.SetColor(i, dlg.GetColor(i));
		}
		Invalidate();
	}
}

	//presypaci hodiny
//	::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	//presypaci hodiny jsou pryc
//	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
