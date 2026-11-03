// ProfilView.h : interface of the CProfilView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILVIEW_H__48AA3F67_85FB_484F_AF82_B84CC2BE3FCA__INCLUDED_)
#define AFX_PROFILVIEW_H__48AA3F67_85FB_484F_AF82_B84CC2BE3FCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Const.h"

struct Filtr
{
	BOOL filtr;//1- zanuto 0-vynuto
	__int16 vlnitost; //0-vlnitost 1-drsnot
	__int16 druh; //0-tojuhelnik 1-Gauss
	__int16 delkaTyp;//0 - 0.08; 1 - 0.25; 2 - 0.8 3 - 2.5; 4 - 8; 5 - libovolne
	double delka; //zakladni delka pro filtr, tedy pokud je zvolena jako volitena
};

class CProfilView : public CScrollView
{
protected: // create from serialization only
	CProfilView();
	DECLARE_DYNCREATE(CProfilView)

// Attributes
public:
	CProfilDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfilView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProfilView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL   m_bPrint;//TRUE - probiha tisk, FALSE - ted se netiskne
	BOOL   m_bVse;  //TRUE - kresli se vsechny grafy, FALSE - kresli se jeden graf
	BOOL   m_bKorel;//TRUE - je spoctena Autokorelacni fce, FALSE - neni
	BOOL   m_bSpektrum;//TRUE - je spocteno Vykonove spektrum, FALSE - neni
	BOOL   m_bSklon;//TRUE - je spocten Stredni aritmeticky sklon profilu; FALSE - neni
	BOOL   m_bRozloz;//TRUE - je spoctena vypocte Hustota pravdepodobnosti rozlozeni souradnic profilu; FALSE - neni
	BOOL   m_bOdecetKorel;//TRUE - zobrazi odecet u AutoKorelacni funkce; FALSE - nic
	BOOL   m_Obalka;//TRUE - zobrazi obalku kolem AutoKorelacni funkce; FALSE - nic
	double m_Rm;	//Drsnost Rm
	double m_Rz;	//Drsnost Rz
	double m_Ra;	//Drsnost Ra
	double m_Rq;	//Drsnost Rq
	double m_Yp[POCET_PRO_RZ],m_Yv[POCET_PRO_RZ];	//Provypocet Rz, 5 nejvyssich a 5 nejnizsich hodnot z *m_yData
	double m_dKorel; //procentove nastaveni osy x pri zobrazovani autokorelacni funkce
	double m_dFrek;  //frekvence pro Vykonove spekrum a Energetickou spekralni hustotu (0..1000)
	double *m_xxData, *m_yyData;  //vsechna data x-delka-osa x, y-vyska nerovnosti
	double *m_xData, *m_yData;    //data po kroku x-delka-osa x, y-vyska nerovnosti
	double *m_yFiltr;	//Filtrovana data
	double *m_yKorel;	//data ktera jsou zpracovana autokorelaci
	double *m_ySpektrum;//data vykonoveho spektra; stejny kocet dat jako u korelacni fce
	double *m_ySklon;   //data stredniho aritmetickeho sklonu profilu; 9 hladin -> 9 polozek
	double *m_yRozloz;  //data hustoty pravdepodobnosti rozlozeni souradnic profilu; 20 hladin -> 20 polozek
	//data podle kterych je udelana obalka kolem autokorelacni fce, ta je urcena
	//body mezi ktere se da primka, ty ktera nejsou potreba maji hodnotu m_MinYKorel-1 u horni obalky,
	//u dolni obalky m_MaxYKorel+1
	double *m_yObalka1;	//horní obálka
	double *m_yObalka2;	//dolní obálka
	double m_Dy;	//pro z normovani autokorelacni fce
	__int32 m_SizeXX, m_SizeYY; //pocet dat pro *x *y, melo by platit SizeXX=SizeYY
	__int32 m_SizeX, m_SizeY;   //pocet dat po kroku pro *x *y, melo by platit SizeX=SizeY
	CString m_sFileName; //jmeno souboru s daty
	__int16 m_iTyp; //0-data 1-profil 2-korelacni fce 3-normovana korelacni fce 4-Vykonove spektrum 5-hustota
	Filtr m_Filtr; //nastaveni filru
	__int32 m_iKrok; //krok po kterem se necitaji data, krok=1 nacte vsechny data 
	double m_MinX; //min delka; osa x
	double m_MaxX; //max delka; osa x
	double m_MinY; //min nerovnost; osa y
	double m_MaxY; //max nerovnost; osa y
	double m_MinYKorel; //min delka; osa y pro Kolrelacni funkci
	double m_MaxYKorel; //max delka; osa y pro Kolrelacni funkci
	double m_MinYSpektrum; //min delka; osa y pro Vykonove spektrum
	double m_MaxYSpektrum; //max delka; osa y pro Vykonove spektrum
	double m_MinYSklon; //min delka; osa y pro Stredni aritmeticky sklon profilu
	double m_MaxYSklon; //max delka; osa y pro Stredni aritmeticky sklon profilu
	//vzdalenost v pixelech pro popis na osach x,y (vzdalenost carek u kterych jsou cisla)
	double m_vzdalenostX,m_vzdalenostY;
	CRect m_rc; //velikost aktualniho okna
	double m_mx;//stredni hodnota nebo-li aritmeticky prumer
	double	m_odecetX; //Odecet u Autokorelacni funkce
	double	m_odecetY; //Odecet u Autokorelacni funkce
	double	m_procenta; //Procenta u Autokorelacni funkce
	__int32  m_carka; //malinkate cerky na osach x,y

	//>>>>popis materialu
	CString m_Datum;
	CString m_sTypPrvku;
	CString m_Popis;
	//<<<<popis materialu

	//>>>>hodnoty pro kresleni grafu
	CSize m_SizeNapis;   //rozmery nadpisu u grafu
	CSize m_SizeTextOx;  //rozmery textu u osy "x" (popisky)
	CSize m_SizeTextOy;  //rozmery textu u osy "y" (popisky)
	CSize m_SizePopisOx; //rozmery popisku u osy "x" (text ktery pojmenovava osu)
	CSize m_SizePopisOy; //rozmery popisku u osy "y" (text ktery pojmenovava osu)
	//<<<<hodnoty pro kresleni grafu

// Generated message map functions
protected:
	//{{AFX_MSG(CProfilView)
	afx_msg void OnDataMonosti();
	afx_msg void OnDataVlastnosti();
	afx_msg void OnUpdateAutokorelanfunkceOdecethodnotyzgrafu(CCmdUI* pCmdUI);
	afx_msg void OnAutokorelanfunkceOdecethodnotyzgrafu();
	afx_msg void OnDataNastavenbarev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CSize Nadpis(CDC* pDC, UINT nID);
	void TabulkaHodnot(CDC* pDC);
	void graf(CDC* pDC, __int16 iTyp);
	void NactiDataZad();
	void NactiData();
	void NactiDataPoKroku();
	double GaussovaFce(double x);
	__int32 SpoctiRadky();
	void PopisGrafu(CDC* pDC, __int16 iTyp);
	__int32 GetX(double d);
	__int32 GetY(double d);
	__int32 GetX(double d, double minx, double maxx);
	__int32 GetY(double d, double miny, double maxy);
	void OsaX(CDC* pDC, double MinX, double MaxX);
	void OsaY(CDC* pDC, double MinY, double MaxY);
	void grafProfil(CDC* pDC);
	void grafKorelace(CDC* pDC, __int16 iTyp);
	void grafVykonovehoSpektra(CDC* pDC);
	void grafEnerSpekHustoty(CDC* pDC);
	void grafStredAritSklonuProf(CDC* pDC);
	void grafHustotaPravRozlozeniSouradnicProfilu(CDC* pDC);
	void DataKorel();
	void DataFiltr();
	void DataSpektrum();
	void DataSklon();
	void DataRozlozeni();
	double FceSpektrum(double omega); //vypocte pro konkretni omega Vykonove spektrum
	void SpoctiDrsnosti();
	void SeraditYp();
	void SeraditYv();
	void InitScrollSizes();
	void UlozZalZadani();   //Ulozi zalozku Zadani
	CSize NapisZalZad(CDC* pDC, int sirka);
	//prevede double cislo 'd' na char* 'ch'
	//'count' pocet cislic za desetinou teckou, pocitano od prvni nenulove cilice,
	//ale zbytecne nuly na konci za dasetinou teckou budou ostraneny
	//napr. d=0.00124586, count=4, tak ch="0.001246"
	void PrevedNaChar(double d, int count, char *ch);
	void KresliObalku(CDC* pDC, __int16 iTyp, __int32 poc);
	void TextOsaY(CDC* pDC, __int16 iTyp); //text u osy Y
	__int32 GetVyskaTextu();
	void SetSizePopis(CDC* pDC, __int16 iTyp); //popis textu u os X,Y
	void PrehodXY(CSize *size);
};

#ifndef _DEBUG  // debug version in ProfilView.cpp
inline CProfilDoc* CProfilView::GetDocument()
   { return (CProfilDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILVIEW_H__48AA3F67_85FB_484F_AF82_B84CC2BE3FCA__INCLUDED_)
