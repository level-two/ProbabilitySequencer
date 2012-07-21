// TrackEditDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ProbabilitySequencer.h"
#include "TrackEditDlg.h"


// диалоговое окно CTrackEditDlg

IMPLEMENT_DYNAMIC(CTrackEditDlg, CDialog)

CTrackEditDlg::CTrackEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrackEditDlg::IDD, pParent)
	, m_Length(_T(""))
	, m_Step(_T(""))
	, m_Volume(0)
	, m_Channel(0)
	, m_Note(_T(""))
	, m_NoteLength(_T(""))
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
	DDX_Text(pDX, IDC_STEP, m_Step);
	DDX_Text(pDX, IDC_VOLUME, m_Volume);
	DDV_MinMaxInt(pDX, m_Volume, 0, 127);
	DDX_Text(pDX, IDC_CHANNEL, m_Channel);
	DDV_MinMaxInt(pDX, m_Channel, 1, 16);
	DDX_Text(pDX, IDC_NOTE, m_Note);
	DDX_Text(pDX, IDC_NOTE_LENGTH, m_NoteLength);
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
END_MESSAGE_MAP()




void CTrackEditDlg::SetEditedTrack(CTrackData *td)
{
	editedTrack = td;

	m_Length = editedTrack->GetTrackLength().c_str();
	m_Channel = editedTrack->GetChannel();
	m_Volume = editedTrack->GetVolume();
	m_Step = editedTrack->GetStep().c_str();
	m_Note = editedTrack->GetNote().c_str();
	m_NoteLength = editedTrack->GetNoteLength().c_str();
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
	editedTrack->SetTrackLength( string(m_Length) );
}

void CTrackEditDlg::OnEnChangeStep()
{
	UpdateData();
	editedTrack->SetStep( string(m_Step) );
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
	editedTrack->SetNoteLength( string(m_NoteLength) );
}
