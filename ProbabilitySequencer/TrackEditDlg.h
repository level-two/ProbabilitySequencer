#pragma once
#include "TrackData.h"

// диалоговое окно CTrackEditDlg

class CTrackEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrackEditDlg)

public:
	CTrackEditDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CTrackEditDlg();

// Данные диалогового окна
	enum { IDD = IDD_TRACK_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()

public:
	void SetEditedTrack(CTrackData *td);

private:
	CTrackData *editedTrack;
	bool mouseButtonPressed;
	int editedPos;

public:
	int m_Length;
	int m_Steps;
	int m_Volume;
	int m_Channel;
	CString m_Note;
	int m_NoteLength;
	CString m_TrackName;
	afx_msg void OnEnChangeTrackName();
	afx_msg void OnEnChangePatternLength();
	afx_msg void OnEnChangeStep();
	afx_msg void OnEnChangeVolume();
	afx_msg void OnEnChangeChannel();
	afx_msg void OnEnChangeNote();
	afx_msg void OnEnChangeNoteLength();
	afx_msg void OnPaint();
	afx_msg void OnEnKillfocus();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
