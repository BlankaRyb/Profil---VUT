#if !defined(AFX_ODECETKOREL_H__0CE2F05E_CDC6_4EA1_8202_F5A3059A99F8__INCLUDED_)
#define AFX_ODECETKOREL_H__0CE2F05E_CDC6_4EA1_8202_F5A3059A99F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ODecetKorel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CODecetKorel dialog

class CODecetKorel : public CDialog
{
// Construction
public:
	CODecetKorel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CODecetKorel)
	enum { IDD = IDD_ODECET_KOREL };
	CEdit	m_editOdecety;
	CEdit	m_editOdecetx;
	int		m_typ;
	double	m_misto;
	double	m_odecetX;
	double	m_odecetY;
	//}}AFX_DATA
	double *m_xData;    //data po kroku x-delka-osa x
	double *m_yKorel;	//data ktera jsou zpracovana autokorelaci
	//data podle kterych je udelana obalka kolem autokorelacni fce, ta je urcena
	//body mezi ktere se da primka, ty ktera nejsou potreba maji hodnotu m_MinYKorel-1 u horni obalky,
	//u dolni obalky m_MaxYKorel+1
	double *m_yObalka1;	//horní obálka
	double *m_yObalka2;	//dolní obálka
	__int32 m_SizeX;    //pocet dat po kroku pro *x
	double m_Dy;	    //pro z normovani autokorelacni fce
	double m_MinYKorel; //min delka; osa y pro Kolrelacni funkci
	double m_MaxYKorel; //max delka; osa y pro Kolrelacni funkci
	double m_dKorel; //procentove nastaveni osy x pri zobrazovani autokorelacni funkce
	BOOL   m_bVypocet; //TRUE pokud probehl vypocet jinak FALSE


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CODecetKorel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CODecetKorel)
	virtual BOOL OnInitDialog();
	afx_msg void OnVypocet();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void ObalkaHorni();
	void ObalkaDolni();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODECETKOREL_H__0CE2F05E_CDC6_4EA1_8202_F5A3059A99F8__INCLUDED_)
