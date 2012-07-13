// ProbabilitySequencerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProbabilitySequencer.h"
#include "ProbabilitySequencerDlg.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define baseBtnId		100000
#define MuteBtnIdBegin	1000
#define MuteBtnIdEnd	2000

// global variables
int portId;
int beatPeriod;

std::vector<char> tracks;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CProbabilitySequencerDlg dialog




CProbabilitySequencerDlg::CProbabilitySequencerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProbabilitySequencerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProbabilitySequencerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProbabilitySequencerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD_TRACK, &CProbabilitySequencerDlg::OnBnClickedAddTrack)
	// TODO: add messages for all buttons
	ON_CONTROL_RANGE(BN_CLICKED, baseBtnId + MuteBtnIdBegin,  baseBtnId + MuteBtnIdEnd, &CProbabilitySequencerDlg::OnMuteButton)
END_MESSAGE_MAP()


// CProbabilitySequencerDlg message handlers

BOOL CProbabilitySequencerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProbabilitySequencerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProbabilitySequencerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProbabilitySequencerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CProbabilitySequencerDlg::OnBnClickedAddTrack()
{
	// STUB: here will be instance creation of the Track class and pushing it to the Tracks array
	char a;
	itoa(tracks.size()+1, &a, 10);
	tracks.push_back(a);

	CRect rect1;
	GetDlgItem(IDC_ADD_TRACK)->GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	// move "Add Track" down
	GetDlgItem(IDC_ADD_TRACK)->SetWindowPos(NULL, rect1.left, rect1.top+25+2, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	
	// Create track buttons
	// TODO: Create track buttons and add them to the form
	int y = rect1.top;
	int x = rect1.left;
	UINT baseId = baseBtnId + tracks.size();
	
	CButton btnMute;
	CRect rect = CRect(x, y, x+25, y+25);
	btnMute.Create("M", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, baseId + MuteBtnIdBegin);
	btnMute.ShowWindow(TRUE);
}

void CProbabilitySequencerDlg::OnMuteButton( UINT nID )
{

}