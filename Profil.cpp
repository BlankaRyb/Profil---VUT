// Profil.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Profil.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ProfilDoc.h"
#include "ProfilView.h"
#include "OpenFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfilApp

BEGIN_MESSAGE_MAP(CProfilApp, CWinApp)
	//{{AFX_MSG_MAP(CProfilApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfilApp construction

CProfilApp::CProfilApp()
{
	m_sFileName = "";
	m_sFilePath = "";
	m_dKorel = 100;
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CProfilApp object

CProfilApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CProfilApp initialization

BOOL CProfilApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_PROFILTYPE,
		RUNTIME_CLASS(CProfilDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CProfilView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//program se spusti bez otevreneho dokumentu
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//otevre profil.ini, pokud neexistuje vytvori nový
	if (!OpenIniFile())
	{
		return FALSE;
	}

	//>> da papir pri tisku nalezato
	LPDEVMODE lpDevMode;
	PRINTDLG pd;

	CPageSetupDialog dlg;
//	lpDevMode = dlg.GetDevMode();
//	CPrintDialog dlg1(FALSE);
//	lpDevMode = dlg1.GetDevMode();

	PAGESETUPDLG& psd = dlg.m_psd;

	theApp.GetPrinterDeviceDefaults(&pd);
	psd.hDevNames = pd.hDevNames;
	psd.hDevMode = pd.hDevMode;
	lpDevMode = dlg.GetDevMode();
//	lpDevMode = dlg1.GetDevMode();

	lpDevMode->dmOrientation = 2;
	//<< da papir pri tisku nalezato

	return TRUE;
}

//otevre profil.ini, pokud neexistuje vytvori nový
BOOL CProfilApp::OpenIniFile()
{
	CFile file;
	CFileException e;
	CString sFileName;
	char c[1], cc[3];
	CString str;
	__int32 len;
	__int16 i;

	sFileName.LoadString(IDS_INI_FILE);

	//soubor *.ini neexistuje
	if (!file.Open(sFileName,CFile::modeRead,&e))
	{
		if (!file.Open(sFileName,CFile::modeCreate,&e))
			return FALSE;
		file.Close();
		if (!file.Open(sFileName,CFile::modeWrite,&e))
			return FALSE;
		else
		{
			cc[0] = 13;
			cc[1] = 10;
			cc[2] = 0;
			//cesta k souborum
			file.Write("C:\\",3);
			file.Write(cc,2);
			m_sFilePath = "C:\\";
			//proceta pro osu x u autokorelacni funkce
			str.LoadString(IDS_INIT_KOREL);
			file.Write(str,str.GetLength());
			file.Write("90",2);
			file.Write(cc,2);
			m_dKorel = 90;
			//frekvence (Vykonove spektrum a Spekralni hustota)
			str.LoadString(IDS_INIT_FREKVENCE);
			file.Write(str,str.GetLength());
			file.Write("10",2);
			file.Write(cc,2);
			m_dFrek = 10;
			//barvy v grafech
			str.LoadString(IDS_INIT_BARVY);
			str += cc;
			file.Write(str,str.GetLength());
			str.LoadString(IDS_INIT_BARVY_POZADI);
			m_clrTrueMap[COLOR_BK] = SetColor(str);
			str += cc;
			file.Write(str,str.GetLength());
			str.LoadString(IDS_INIT_BARVY_OSY);
			m_clrTrueMap[COLOR_OSY] = SetColor(str);
			str += cc;
			file.Write(str,str.GetLength());
			str.LoadString(IDS_INIT_BARVY_PROFIL);
			m_clrTrueMap[COLOR_PROFIL] = SetColor(str);
			str += cc;
			file.Write(str,str.GetLength());
			str.LoadString(IDS_INIT_BARVY_FILTR);
			m_clrTrueMap[COLOR_FILTR] = SetColor(str);
			str += cc;
			file.Write(str,str.GetLength());
		}
	}
	//soubor *.ini existuje
	else
	{
		//nacte adresar ze ktereho se cetli naposledy data
		file.Read(c,1);
		m_sFilePath = "";
		while ((c[0] != 10)&&(c[0] != 13))
		{
			m_sFilePath += *c;
			file.Read(c,1);
		}
		while ((c[0] == 10)||(c[0] == 13))
		{
			file.Read(c,1);
		}
		len = file.GetPosition();
		file.Seek(len-1, CFile::begin);//posun o zank zpet
		//proceta pro osu x u autokorelacni funkce
		str.LoadString(IDS_INIT_KOREL);
		len = str.GetLength();
/*		for (i=1; i<len; i++)
		{
			file.Read(c,1);
		}*/
		len += file.GetPosition();
		file.Seek(len, CFile::begin);
		str = "";
		file.Read(c,1);
		while ((c[0] != 10)&&(c[0] != 13))
		{
			str += *c;
			file.Read(c,1);
		}
		m_dKorel = atof(str);
		while ((c[0] == 10)||(c[0] == 13))
		{
			file.Read(c,1);
		}
		len = file.GetPosition();
		file.Seek(len-1, CFile::begin);//posun o zank zpet
		//frekvence (Vykonove spektrum a Spekralni hustota)
		str.LoadString(IDS_INIT_FREKVENCE);
		len = str.GetLength();
		/*for (i=1; i<len; i++)
		{
			file.Read(c,1);
		}*/
		len += file.GetPosition();
		file.Seek(len, CFile::begin);
		str = "";
		file.Read(c,1);
		while ((c[0] != 10)&&(c[0] != 13))
		{
			str += *c;
			file.Read(c,1);
		}
		m_dFrek = atof(str);
		while ((c[0] == 10)||(c[0] == 13))
		{
			file.Read(c,1);
		}
		len = file.GetPosition();
		file.Seek(len-1, CFile::begin);//posun o zank zpet

		//barvy v grafech
		str.LoadString(IDS_INIT_BARVY);
		len = str.GetLength()+2;//2 ukonceni radku
		len += file.GetPosition();
		file.Seek(len, CFile::begin);
		for (i = 0; i<COUNT_COL; i++)
		{
			str = "";
			file.Read(c,1);
			//barva pro osy
			while ((c[0] != 10)&&(c[0] != 13))
			{
				str += *c;
				file.Read(c,1);
			}
			m_clrTrueMap[i] = SetColor(str);
			while (((c[0] == 10)||(c[0] == 13))&&(file.GetPosition()<file.GetLength()))
			{
				file.Read(c,1);
			}
			len = file.GetPosition();
			file.Seek(len-1, CFile::begin);//posun o znak zpet
		}
/*		while ((c[0] == 10)||(c[0] == 13))
		{
			file.Read(c,1);
		}*/
//		len = file.GetPosition();
//		file.Seek(len-1, CFile::begin);//posun o znak zpet
	}
	file.Close();

	return TRUE;
}

//vrati barvu (str je ve tvaru 0,0,0 (R,G,B))
COLORREF CProfilApp::SetColor(CString str)
{
	int i,k,len;
	CString str_cis;
	int col[3]; //3 R,B,G

	col[0] = col[1] = col[2] = 0;
	i = 0;
	len = str.GetLength();

	for (k = 0; (k<3)&&(i<(len-1)); k++)
	{
		str_cis = "";
		while ((i<len)&&((str[i] == ',')||(str[i] == ' ')||(str[i] == 10)||(str[i] == 13)))
		{
			i++;
		}
		while ((i<len)&&(str[i] != ',')&&(str[i] != ' ')&&(str[i] != 10)&&(str[i] != 13))
		{
			str_cis += str[i];
			//barva pro osy
			i++;
		}
		col[k] = atoi(str_cis);
	}
	return RGB(col[0],col[1],col[2]);
}

//vrati CString (str je ve tvaru 0,0,0 (R,G,B))
CString CProfilApp::SetStr_Color(COLORREF col)
{
	CString str, s;
	char ch[30];

	str = "";
	s   = ", ";

	itoa(GetRValue(col), ch, 10);
	str += ch;
	str += s;
	itoa(GetGValue(col), ch, 10);
	str += ch;
	str += s;
	itoa(GetBValue(col), ch, 10);
	str += ch;

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CProfilApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CProfilApp message handlers


void CProfilApp::OnFileOpen() 
{
	CString strFilter, str;

	char szfilePath[_MAX_PATH];
	char szFile[_MAX_PATH]={'\0'};
	char szName[_MAX_FNAME]={'\0'};

	HWND hParent = theApp.m_pMainWnd->GetSafeHwnd();

	// Create dummy CWnd and attach parent window to it, then create the
	// dialog box with the proper parent  !!! have to detach and delete !!!
	CWnd * pParent = new CWnd;
	if (pParent==0)
		return;

	// IDS_TXTFILTER: TXT formát (*.txt)|*.txt|
	strFilter.LoadString(IDS_TXTFILTER);

	//zde se nacte cesta ze ktere byl naposledy nacitan obrazek
	strcpy(szfilePath, m_sFilePath);

	// create dialog box
	COpenFile dlg( TRUE,
						"txt",
						szFile,
						OFN_ENABLETEMPLATE | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST,
						strFilter,
						pParent);

	// Initialize structure for common dialog box for choosing Metafile
	dlg.m_ofn.hInstance  = AfxGetResourceHandle();
	dlg.m_ofn.Flags &= ~OFN_EXPLORER;
	dlg.m_ofn.lpTemplateName=MAKEINTRESOURCE(IDD_OPEN_FILE);
	dlg.m_ofn.lpstrInitialDir = szfilePath;

	//Invoke the Dialog Box Class to display and process the dialog box
	int nRet = dlg.DoModal();

	if (nRet <=0 || nRet==IDABORT) {
		CString  strMessage, strMessage1;
		//Dialog provedl chybu.
		strMessage.LoadString(IDS_DIALOG_CRASH);
		strMessage1.LoadString(IDS_DIALOG_CRASH_TITLE);
		MessageBox(NULL,strMessage, strMessage1, MB_OK|MB_ICONERROR);
	}

	if (nRet==IDOK) {
		//ulozi cestu
		str = dlg.m_ofn.lpstrFile;
		m_sFilePath = str.Left(str.ReverseFind('\\'));
		//konec ukladani cesty
		//urci typ otevreneho souboru
		str = str.Right(str.GetLength()-str.ReverseFind('.')-1);
		if (UrciTyp(str))
		{
			m_sFileName = dlg.m_ofn.lpstrFile;
			CWinApp::OpenDocumentFile(dlg.m_ofn.lpstrFile);
		}
		else
		{
			CString  strMessage, strMessage1;

			strMessage.LoadString(IDS_DIALOG_NEPOVOLENY_TYP);
			strMessage1.LoadString(IDS_DIALOG_NEPOVOLENY_TYP_TITLE);
			MessageBox(NULL,strMessage, strMessage1, MB_OK|MB_ICONERROR);
		}
	}

	// detach and delete dummy Cwnd object
	pParent->Detach();

	delete pParent;
}

CString CProfilApp::GetFilePath()
{
	return m_sFilePath;
}

void CProfilApp::SetFilePath(CString sFilePath)
{
	m_sFilePath = sFilePath;
}

CString CProfilApp::GetFileName()
{
	return m_sFileName;
}

void CProfilApp::SetFileName(CString sFilePath)
{
	m_sFileName = sFilePath;
}

double CProfilApp::GetProcentaKorel()
{
	return m_dKorel;
}

void CProfilApp::SetProcentaKorel(double dKorel)
{
	m_dKorel = dKorel;
}

double CProfilApp::GetFrekvence()
{
	return m_dFrek;
}

void CProfilApp::SetFrekvence(double dFrek)
{
	m_dFrek = dFrek;
}

COLORREF CProfilApp::GetColor(int i)
{
	return m_clrTrueMap[i];
}

void CProfilApp::SetColor(int i, COLORREF col)
{
	m_clrTrueMap[i] = col;
}

BOOL CProfilApp::SaveAllModified() 
{
	//otevre profil.ini a ulozi do nej informace
	CFile file;
	CFileException e;
	CString sFileName,str;
	char c[3];
	char ch[20];
	__int16 i;

	c[0] = 13;
	c[1] = 10;
	c[2] = 0;
	sFileName.LoadString(IDS_INI_FILE);

	if (!file.Open(sFileName,CFile::modeWrite|CFile::modeCreate,&e))
		return CWinApp::SaveAllModified();
	else
	{
		//ulozi adresar ze ktereho se cetli naposledy obrazky
		file.Write(m_sFilePath,m_sFilePath.GetLength());
		file.Write(c,2);
		//ulozi "Korel [%]:" a zatim proceta
		str.LoadString(IDS_INIT_KOREL);
		file.Write(str,str.GetLength());
		_gcvt( m_dKorel, 6, ch);
		str = ch;
		file.Write(str,str.GetLength());
		file.Write(c,2);
		//frekvence (Vykonove spektrum a Spekralni hustota)
		str.LoadString(IDS_INIT_FREKVENCE);
		file.Write(str,str.GetLength());
		_gcvt(m_dFrek, 6, ch);
		str = ch;
		file.Write(str,str.GetLength());
		file.Write(c,2);
		//barvy v grafech
		str.LoadString(IDS_INIT_BARVY);
		file.Write(str,str.GetLength());
		file.Write(c,2);
		for (i=0; i<COUNT_COL; i++)
		{
			str = SetStr_Color(m_clrTrueMap[i]);
			file.Write(str,str.GetLength());
			file.Write(c,2);
		}
	}

//	int i = m_sFilePath.GetLength();
	file.Close();

	return CWinApp::SaveAllModified();
}

//V pripade nepovoleneho typu vrati FALSE
BOOL CProfilApp::UrciTyp (CString str)
{
	CString s;
	int i;
	BOOL b;

	b = FALSE;
	//txt		pokud by bylo vice souboru, tak jako odelovace stredniky
	s.LoadString(IDS_TYP_SOUBORU);//obsahuje jen mala pismena
	str.MakeLower();//mala pismena
	i = s.Find(str);
	if (i != -1)
	{
		b = TRUE;
	}

	return b;
}


CDocument* CProfilApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	m_sFileName = lpszFileName;

	//urci typ otevreneho souboru
	CString str = m_sFileName.Right(m_sFileName.GetLength()-m_sFileName.ReverseFind('.')-1);
	if (UrciTyp(str))
	{
		return CWinApp::OpenDocumentFile(lpszFileName);
	}
	return FALSE;
}
