#if !defined(AFX_SETCOLOR_H__7598CCF7_CBBF_4A21_857B_E02F48069B24__INCLUDED_)
#define AFX_SETCOLOR_H__7598CCF7_CBBF_4A21_857B_E02F48069B24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetColor dialog

class CSetColor : public CDialog
{
// Construction
public:
	CSetColor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetColor)
	enum { IDD = IDD_COLOR };
	CStatic	m_colPozadi;
	CStatic	m_colProfil;
	CStatic	m_colOsy;
	CStatic	m_colFiltr;
	int		m_radColor;
	int		m_iRedIndex;
	int		m_iGreenIndex;
	int		m_iBlueIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetColor(int i, COLORREF col);
	COLORREF GetColor(int i);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetColor)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioBut();
	afx_msg void OnKillfocusRedIndex();
	afx_msg void OnKillfocusGreenIndex();
	afx_msg void OnKillfocusBlueIndex();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetScrollBars(COLORREF clrRgb, BOOL bUpdate);//barevne ScrollBary
	BOOL CheckIndex(int NColorID);

protected:
	HBRUSH m_hColorBrush;   // Barva pro vyplneni barevnych boxu

private:
	COLORREF m_clrTrueMap[COUNT_COL];  //Aktualni RGB pro jednotlive prvky v grafech
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCOLOR_H__7598CCF7_CBBF_4A21_857B_E02F48069B24__INCLUDED_)
