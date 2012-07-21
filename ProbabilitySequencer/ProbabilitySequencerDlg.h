// ProbabilitySequencerDlg.h : header file
//

#pragma once

#include <vector>

// CProbabilitySequencerDlg dialog
class CProbabilitySequencerDlg : public CDialog
{
// Construction
public:
	CProbabilitySequencerDlg(CWnd* pParent = NULL);	// standard constructor
	~CProbabilitySequencerDlg();					// destructor

// Dialog Data
	enum { IDD = IDD_PROBABILITYSEQUENCER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddTrack();
	afx_msg void OnButton(UINT nID);
	afx_msg void OnMemoryButton(UINT nID);


private:
	void MoveButton(CWnd* btn, int dy);
	void AddButtons(int index, int newId);
	void DeleteButtons(int trackId);
	void UpdateButtons(void);
	
	int  FindButtonIndexWithId(int id);
	int  GetTrackIndex(int trackId);
	int  GetNextId(void);

	void AddTrack(unsigned int trackId);
	int  CopyTrack(unsigned int trackId);
	void DeleteTrack(unsigned int trackId);
private:
	std::vector<CButton*> trackButtons;
	bool soloMode;
	bool storeMode;
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedStore();
	afx_msg void OnSettings();
	afx_msg void OnFileNewsession();
};
