// ProbabilitySequencerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProbabilitySequencer.h"
#include "ProbabilitySequencerDlg.h"
#include "SettingsDlg.h"
#include "TrackEditDlg.h"
#include "TrackData.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define baseBtnId				15000
#define MuteBtnId				1000
#define SoloBtnId				2000
#define TrackInfoBtnId			3000
#define DeleteTrackBtnId		4000
#define DuplicateTrackBtnId		5000
#define LastBtnId				6000

// global variables
int portId;
int beatPeriod;

std::vector<CTrackData*> tracks;

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
	: CDialog(CProbabilitySequencerDlg::IDD, pParent), storeMode(false), soloMode(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CProbabilitySequencerDlg::~CProbabilitySequencerDlg()
{
	while (trackButtons.size()>0)
	{
		delete trackButtons[trackButtons.size()-1];
		trackButtons.pop_back();
	}

	while (tracks.size()>0)
	{
		delete tracks[tracks.size()-1];
		tracks.pop_back();
	}
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
	ON_CONTROL_RANGE(BN_CLICKED, baseBtnId+MuteBtnId, baseBtnId+LastBtnId, OnButton)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_MEMORY_1, IDC_MEMORY_0, OnMemoryButton)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_STORE, &CProbabilitySequencerDlg::OnBnClickedStore)
	ON_COMMAND(ID_SETTINGS, &CProbabilitySequencerDlg::OnSettings)
	ON_COMMAND(ID_FILE_NEWSESSION, &CProbabilitySequencerDlg::OnFileNewsession)
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

	//
	// this button will always be last in the buttons array
	CButton *addTrackButton = new CButton();
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY;
	addTrackButton->Create("+", dwStyle, CRect(11, 40, 36, 65), this, IDC_ADD_TRACK);
	trackButtons.push_back(addTrackButton);

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


//--------------------------------------------------------
// Track procedures

void CProbabilitySequencerDlg::AddTrack(unsigned int trackId)
{
	CTrackData *track = new CTrackData();

	tracks.push_back(track);
	track->SetTrackId(trackId);
	track->SetTrackName("TrackName");
}

void CProbabilitySequencerDlg::CopyTrack(unsigned int trackId)
{
	int i = GetTrackIndex(trackId);
	int newId = GetNextId(); // id of the last track + 1
	CTrackData *track = new CTrackData(tracks[i]); // duplicate

	tracks.push_back(track);
	track->SetTrackId(newId);
}

void CProbabilitySequencerDlg::DeleteTrack(unsigned int trackId)
{
	int i = GetTrackIndex(trackId);
	delete tracks[i];
	tracks.erase(tracks.begin()+i);
}

int CProbabilitySequencerDlg::GetTrackIndex(int trackId)
{
	int i=0;
	for (; i<tracks.size(); i++) {
		if (tracks[i]->GetTrackId() == trackId)
			break;
	}
	return i;
}

int CProbabilitySequencerDlg::GetNextId(void)
{
	if (tracks.size() == 0) return 0;
	int id = tracks[tracks.size()-1]->GetTrackId();
	return id+1;
}


//---------------------------------------------------------
// Button create and move procedures

void CProbabilitySequencerDlg::MoveButton(CWnd* btn, int dy)
{
	CRect rect;
	btn->GetWindowRect(&rect);
	ScreenToClient(&rect);
	// move button down
	btn->SetWindowPos(NULL, rect.left, rect.top+dy, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
}

void CProbabilitySequencerDlg::AddButtons(int index, int newId)
{
	// calculate position of new buttons
	// get frame of the button from current track or of the "+" button

	CRect rect1;
	trackButtons[index]->GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	int y = rect1.top;
	int x = rect1.left;

	// move buttons down
	for (int i = index; i < trackButtons.size(); i++) {
		MoveButton(trackButtons[i], 25+2);
	}

	// Create track buttons
	// TODO: Create track buttons and add them to the form

	UINT baseId = baseBtnId + newId;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY;
	vector<CButton*>::iterator it = trackButtons.begin()+index;

	// Mute button
	CButton *muteButton = new CButton();
	CRect rect = CRect(x, y, x+25, y+25);
	muteButton->Create("M", dwStyle, rect, this, baseId + MuteBtnId);
	it = trackButtons.insert(it, muteButton);
	it++;
	x+=28;

	// Solo button
	CButton *soloButton = new CButton();
	rect = CRect(x, y, x+25, y+25);
	soloButton->Create("S", dwStyle, rect, this, baseId + SoloBtnId);
	it = trackButtons.insert(it, soloButton);
	it++;
	x+=28;

	// Track Info button
	CButton *trackInfoButton = new CButton();
	rect = CRect(x, y, x+300, y+25);
	// track->GetTrackName().c_str()
	trackInfoButton->Create("", dwStyle, rect, this, baseId + TrackInfoBtnId);
	it = trackButtons.insert(it, trackInfoButton);
	it++;
	x+=303;

	// Delete Track button
	CButton *deleteTrackButton = new CButton();
	rect = CRect(x, y, x+25, y+25);
	deleteTrackButton->Create("X", dwStyle, rect, this, baseId + DeleteTrackBtnId);
	it = trackButtons.insert(it, deleteTrackButton);
	it++;
	x+=28;

	// Duplicate Track button
	CButton *duplicateTrackButton = new CButton();
	rect = CRect(x, y, x+25, y+25);
	duplicateTrackButton->Create("C", dwStyle, rect, this, baseId + DuplicateTrackBtnId);
	it = trackButtons.insert(it, duplicateTrackButton);
	it++;
}

int CProbabilitySequencerDlg::FindButtonIndexWithId(int id)
{
	int i=0;
	while (i<trackButtons.size()) {
		if(trackButtons[i]->GetDlgCtrlID() == id)
			return i;
		i++;
	}
	return 0;
}


void CProbabilitySequencerDlg::DeleteButtons(int trackId)
{
	int index = FindButtonIndexWithId(baseBtnId+MuteBtnId+trackId);
	for (int i=0; i<5; i++)
	{
		delete trackButtons[index];
		trackButtons.erase(trackButtons.begin()+index);
	}

	// move buttons up
	for (int i = index; i < trackButtons.size(); i++) {
		MoveButton(trackButtons[i], -25-2);
	}
}

//--------------------------------------------------
// Button callbacks

void CProbabilitySequencerDlg::OnBnClickedAddTrack()
{
	int newId = GetNextId();
	AddTrack(newId);
	AddButtons(FindButtonIndexWithId(IDC_ADD_TRACK), newId);
}

void CProbabilitySequencerDlg::OnButton(UINT nID)
{
	int id = nID - baseBtnId;
	int trackId = id % 1000;
	
	if (id >= DuplicateTrackBtnId)
	{
		int newId = GetNextId();
		AddTrack(newId);
		AddButtons(FindButtonIndexWithId(baseBtnId+MuteBtnId+trackId), newId);
	}
	else if (id >= DeleteTrackBtnId)
	{
		DeleteTrack(trackId);
		DeleteButtons(trackId);
		// delete buttons with trackId here
	}
	else if (id >= TrackInfoBtnId)
	{
		// show modal dialog with track editing
		int i = GetTrackIndex(trackId);
		CTrackEditDlg dlg;
		dlg.SetEditedTrack(tracks[i]);
		dlg.DoModal();
	}
	else if (id >= SoloBtnId)
	{
		// set Solo for track
		for (int i=0; i<tracks.size(); i++)
		{
			if (!soloMode)
			{
				tracks[i]->SaveMuteState(10);
				tracks[i]->Mute(i!=trackId);
			}
			else
			{
				tracks[i]->RestoreMuteState(10);
			}
			// change color for buttons
		}
		soloMode = !soloMode;
	}
	else if (id >= MuteBtnId)
	{
		BOOL muted = tracks[trackId]->isMuted();
		tracks[trackId]->Mute(!muted);
		CButton *muteBtn = (CButton*)GetDlgItem(nID);
		//muteBtn-> change color here
	}
}

void CProbabilitySequencerDlg::OnMemoryButton(UINT nID)
{
	int index = nID - IDC_MEMORY_1;

	if (storeMode) {
		for (int i=0; i<tracks.size(); i++)
			tracks[i]->SaveMuteState(index);
	}
	else
	{
		for (int i=0; i<tracks.size(); i++)
			tracks[i]->RestoreMuteState(index);
	}	
	storeMode = false;
	// change color of the Store Button here
}

void CProbabilitySequencerDlg::OnBnClickedStore()
{
	storeMode = !storeMode;
	// change color of the Store Button here
}

//-------------------------------------------------
// IDR_INTERFACE_MENU delegate methods

void CProbabilitySequencerDlg::OnSettings()
{
	CSettingsDlg sd;
	sd.DoModal();
	//CSettingsDlg *sd = new CSettingsDlg;
	//sd->Create(IDD_SETTINGS_DLG, this);
    //sd->ShowWindow(SW_SHOW);
}

void CProbabilitySequencerDlg::OnFileNewsession()
{
	while(tracks.size()>0)
	{
		int id = tracks[0]->GetTrackId();
		DeleteTrack(id);
		DeleteButtons(id);
	}
}
