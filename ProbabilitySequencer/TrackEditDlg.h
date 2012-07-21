#pragma once
#include "TrackData.h"

// ���������� ���� CTrackEditDlg

class CTrackEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrackEditDlg)

public:
	CTrackEditDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CTrackEditDlg();

// ������ ����������� ����
	enum { IDD = IDD_TRACK_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	DECLARE_MESSAGE_MAP()

public:
	void SetEditedTrack(CTrackData *td);


private:
	CTrackData *editedTrack;
public:
	CString m_Length;
	CString m_Step;
	int m_Volume;
	int m_Channel;
	CString m_Note;
	CString m_NoteLength;
	CString m_TrackName;
};
