// TrackEditDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ProbabilitySequencer.h"
#include "TrackEditDlg.h"


// диалоговое окно CTrackEditDlg

IMPLEMENT_DYNAMIC(CTrackEditDlg, CDialog)

CTrackEditDlg::CTrackEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrackEditDlg::IDD, pParent)
	, m_Length(0)
	, m_Steps(0)
	, m_Volume(0)
	, m_Channel(0)
	, m_Note(_T(""))
	, m_NoteLength(0)
	, m_TrackName(_T(""))
{

}

CTrackEditDlg::~CTrackEditDlg()
{
}

void CTrackEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATTERN_LENGTH, m_Length);
//	DDV_MinMaxInt(pDX, m_Length, 1, 64);
	DDX_Text(pDX, IDC_STEP, m_Steps);
//	DDV_MinMaxInt(pDX, m_Steps, 1, 64);
	DDX_Text(pDX, IDC_VOLUME, m_Volume);
//	DDV_MinMaxInt(pDX, m_Volume, 0, 127);
	DDX_Text(pDX, IDC_CHANNEL, m_Channel);
//	DDV_MinMaxInt(pDX, m_Channel, 1, 16);
	DDX_Text(pDX, IDC_NOTE, m_Note);
	DDX_Text(pDX, IDC_NOTE_LENGTH, m_NoteLength);
//	DDV_MinMaxInt(pDX, m_NoteLength, 1, 64);
	DDX_Text(pDX, IDC_TRACK_NAME, m_TrackName);
}


BEGIN_MESSAGE_MAP(CTrackEditDlg, CDialog)
	ON_EN_CHANGE(IDC_TRACK_NAME, &CTrackEditDlg::OnEnChangeTrackName)
	ON_EN_CHANGE(IDC_PATTERN_LENGTH, &CTrackEditDlg::OnEnChangePatternLength)
	ON_EN_CHANGE(IDC_STEP, &CTrackEditDlg::OnEnChangeStep)
	ON_EN_CHANGE(IDC_VOLUME, &CTrackEditDlg::OnEnChangeVolume)
	ON_EN_CHANGE(IDC_CHANNEL, &CTrackEditDlg::OnEnChangeChannel)
	ON_EN_CHANGE(IDC_NOTE, &CTrackEditDlg::OnEnChangeNote)
	ON_EN_CHANGE(IDC_NOTE_LENGTH, &CTrackEditDlg::OnEnChangeNoteLength)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_PATTERN_LENGTH, &CTrackEditDlg::OnEnKillfocus)
	ON_EN_KILLFOCUS(IDC_STEP, &CTrackEditDlg::OnEnKillfocus)
	ON_EN_KILLFOCUS(IDC_NOTE_LENGTH, &CTrackEditDlg::OnEnKillfocus)
	ON_WM_TIMER()
END_MESSAGE_MAP()




void CTrackEditDlg::SetEditedTrack(CTrackData *td)
{
	editedTrack = td;

	m_Length = editedTrack->GetTrackLength();
	m_Channel = editedTrack->GetChannel();
	m_Volume = editedTrack->GetVolume();
	m_Steps = editedTrack->GetSteps();
	m_Note = editedTrack->GetNote().c_str();
	m_NoteLength = editedTrack->GetNoteLength();
	m_TrackName = editedTrack->GetTrackName().c_str();
}

// обработчики сообщений CTrackEditDlg

void CTrackEditDlg::OnEnChangeTrackName()
{
	UpdateData();
	editedTrack->SetTrackName( string(m_TrackName) );
}

void CTrackEditDlg::OnEnChangePatternLength()
{
	UpdateData();
	editedTrack->SetTrackLength( m_Length );

	KillTimer(1);
	SetTimer(1,1000,NULL);
}

void CTrackEditDlg::OnEnChangeStep()
{
	UpdateData();
	editedTrack->SetSteps( m_Steps );

	KillTimer(1);
	SetTimer(1,1000,NULL);
}

void CTrackEditDlg::OnEnChangeVolume()
{
	UpdateData();
	editedTrack->SetVolume( m_Volume );
}

void CTrackEditDlg::OnEnChangeChannel()
{
	UpdateData();
	editedTrack->SetChannel( m_Channel );
}

void CTrackEditDlg::OnEnChangeNote()
{
	UpdateData();
	editedTrack->SetNote( string(m_Note) );
}

void CTrackEditDlg::OnEnChangeNoteLength()
{
	UpdateData();
	editedTrack->SetNoteLength( m_NoteLength );

	KillTimer(1);
	SetTimer(1,1000,NULL);
}

#define X 11
#define Y 110
#define XX 400
#define YY 250

void CTrackEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	COLORREF qLineColor = RGB(255,255,255);
	CPen qLinePen(PS_SOLID, 1, qLineColor);
	dc.SelectObject(&qLinePen);

	dc.Rectangle(X,Y,XX,YY);

	COLORREF qLineColor1 = RGB(0,0,0);
	CPen qLinePen1(PS_SOLID, 1, qLineColor1);
	dc.SelectObject(&qLinePen1);
	dc.Rectangle(X-1, (Y+YY)/2, XX, (Y+YY)/2+1);

	int length = editedTrack->GetTrackLength();
	int steps = editedTrack->GetSteps();
	float dx = (float)(XX-X)/(length*steps);
	float x = X;

	for (int i=0; i<length*steps; i++)
	{
		COLORREF qLineColor2 = RGB(100,100,100);
		CPen qLinePen2(PS_SOLID, 1, qLineColor2);
		dc.SelectObject(&qLinePen2);

		if (i%steps == 0) {
			dc.Rectangle(x-1,Y,x+1,YY);
		}
		else {
			dc.Rectangle(x,Y,x+1,YY);
		}

		float p = rand()%10 / 10.0;
		int yp = YY-(YY-Y)*p;

		COLORREF qLineColor3 = RGB(0,0,0);
		CPen qLinePen3(PS_SOLID, 1, qLineColor3);
		dc.SelectObject(&qLinePen3);

		dc.Rectangle(x-2,yp,x+2,YY);
		dc.Ellipse(x-5,yp-5,x+5,yp+5);

		x+=dx;
	}
	
	// TODO: draw values here
}

void CTrackEditDlg::OnEnKillfocus()
{
	RedrawWindow();
}

void CTrackEditDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(1);

	RedrawWindow();
	CDialog::OnTimer(nIDEvent);
}
