// TrackEditDlg.cpp: ���� ����������
//

#include "stdafx.h"
#include "ProbabilitySequencer.h"
#include "TrackEditDlg.h"
#include <math.h>

// ���������� ���� CTrackEditDlg

IMPLEMENT_DYNAMIC(CTrackEditDlg, CDialog)

CTrackEditDlg::CTrackEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrackEditDlg::IDD, pParent)
	, m_Length(0)
	, m_Steps(0)
	, m_Volume(0)
	, m_Channel(0)
	, m_Note(0)
	, m_NoteLength(0)
	, m_TrackName(_T(""))
{
	editedPos = -1;
	mouseButtonPressed = false;
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
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_LOADTRACK, &CTrackEditDlg::OnFileLoadtrack)
	ON_COMMAND(ID_FILE_SAVETRACK, &CTrackEditDlg::OnFileSavetrack)
END_MESSAGE_MAP()

void CTrackEditDlg::SetEditedTrack(CTrackData *td)
{
	editedTrack = td;

	m_Length = editedTrack->GetTrackLength();
	m_Channel = editedTrack->GetChannel();
	m_Volume = editedTrack->GetVolume();
	m_Steps = editedTrack->GetSteps();
	m_Note = editedTrack->GetNote();
	m_NoteLength = editedTrack->GetNoteLength();
	m_TrackName = editedTrack->GetTrackName().c_str();
}

// ����������� ��������� CTrackEditDlg

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
	editedTrack->SetNote(m_Note);
}

void CTrackEditDlg::OnEnChangeNoteLength()
{
	UpdateData();
	float stepLen = TICKS_PER_BEAT/editedTrack->GetSteps();

	if (m_NoteLength > stepLen)
	{
		m_NoteLength = stepLen;
		UpdateData(false); 
	}
	editedTrack->SetNoteLength( m_NoteLength );

	KillTimer(1);
	SetTimer(1,1000,NULL);
}

#define X 11
#define Y 110
#define XX 400
#define YY 250
#define RADIUS 5

void CTrackEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	COLORREF qLineColor1 = RGB(255,255,255);
	CPen qLinePen1(PS_SOLID, 1, qLineColor1);
	COLORREF qLineColor2 = RGB(0,0,0);
	CPen qLinePen3(PS_SOLID, 1, qLineColor2);
	COLORREF qLineColor3 = RGB(150,150,150);
	CPen qLinePen2(PS_SOLID, 1, qLineColor3);

	dc.SelectObject(&qLinePen1);

	dc.Rectangle(X,Y,XX,YY);

	dc.SelectObject(&qLinePen2);
	dc.Rectangle(X-1, (Y+YY)/2, XX, (Y+YY)/2+1);

	int length = editedTrack->GetTrackLength();
	int steps = editedTrack->GetSteps();
	float dx = (float)(XX-X)/(length*steps);
	float x = X;

	for (int i=0; i<length*steps; i++)
	{
		dc.SelectObject(&qLinePen3);

		if (i%steps == 0) {
			dc.Rectangle(x-1,Y,x+1,YY);
		}
		else {
			dc.Rectangle(x,Y,x+1,YY);
		}

		// float p = rand()%10 / 10.0;
		float p = editedTrack->GetValue(i);
		int yp = YY-(YY-Y)*p;

		dc.SelectObject(&qLinePen2);

		dc.Rectangle(x-2,yp,x+2,YY);
		int r = RADIUS;
		dc.Ellipse(x-r,yp-r,x+r,yp+r);

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

void CTrackEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mouseButtonPressed &&
		point.x > X && point.x < XX &&
		point.y > Y && point.y < YY)
	{
		float pos;
		if (editedPos>=0)
		{
			pos = editedPos;
		}
		else
		{
			float length = editedTrack->GetTrackLength();
			float steps = editedTrack->GetSteps();
			pos = length*steps*(point.x - X)/(XX-X);
		}
		
		float value = (float)(YY - point.y)/(YY-Y);
		editedTrack->SetValue(floor(pos+0.5),value);
		int r = RADIUS;
		InvalidateRect(CRect(X-r,Y-r,XX+r,YY+r));
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CTrackEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x > X && point.x < XX &&
		point.y > Y && point.y < YY)
	{
		mouseButtonPressed = true;
		float length = editedTrack->GetTrackLength();
		float steps = editedTrack->GetSteps();
		editedPos = length*steps*(point.x - X)/(XX-X)+0.5;	

		float value = (float)(YY - point.y)/(YY-Y);
		editedTrack->SetValue(floor(editedPos+0.5),value);

		int r = RADIUS;
		InvalidateRect(CRect(X-r,Y-r,XX+r,YY+r));
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CTrackEditDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	mouseButtonPressed = false;
	editedPos = -1;
	CDialog::OnLButtonUp(nFlags, point);
}

void CTrackEditDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (point.x > X && point.x < XX &&
		point.y > Y && point.y < YY)
	{
		mouseButtonPressed = true;
		editedPos = -1;

		float length = editedTrack->GetTrackLength();
		float steps = editedTrack->GetSteps();
		int pos = length*steps*(point.x - X)/(XX-X)+0.5;	

		float value = (float)(YY - point.y)/(YY-Y);
		editedTrack->SetValue(floor(pos+0.5),value);

		int r = RADIUS;
		InvalidateRect(CRect(X-r,Y-r,XX+r,YY+r));
	}
	CDialog::OnRButtonDown(nFlags, point);
}

void CTrackEditDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	mouseButtonPressed = false;
	CDialog::OnRButtonUp(nFlags, point);
}

void CTrackEditDlg::OnFileLoadtrack()
{

	TCHAR szFilters[]= _T("Track Files (*.pst)|*.pst|All Files (*.*)|*.*||");

	CFileDialog fileDlg(TRUE, _T(".pst"), _T("*.pst"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	if(fileDlg.DoModal() != IDOK)
		return;

	CString pathName = fileDlg.GetPathName();

	FILE *f = fopen(pathName,"r");
	if (f == NULL) return; // error
	editedTrack->ReadTrackFromFile(f);
	fclose(f);

	SetEditedTrack(editedTrack);
	UpdateData(false);
	RedrawWindow();
}

void CTrackEditDlg::OnFileSavetrack()
{
	TCHAR szFilters[]= _T("Track Files (*.pst)|*.pst|All Files (*.*)|*.*||");

	std::string name = editedTrack->GetTrackName() + ".pst";
	CFileDialog fileDlg(FALSE, _T(".pst"), _T(name.c_str()), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	if(fileDlg.DoModal() != IDOK)
		return;

	CString pathName = fileDlg.GetPathName();

	FILE *f = fopen(pathName,"w");
	if (f == NULL) return; // error
	editedTrack->SaveTrackToFile(f);
	fclose(f);
}