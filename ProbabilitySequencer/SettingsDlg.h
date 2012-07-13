#pragma once
#include "afxwin.h"


// ���������� ���� SettingsDlg

class SettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(SettingsDlg)

public:
	SettingsDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~SettingsDlg();

// ������ ����������� ����
	enum { IDD = IDD_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditBpm();
	afx_msg void OnCbnKillfocusComboDeviceSelect();
	afx_msg void OnCbnCloseupComboDeviceSelect();
	UINT m_bpm;
	CComboBox m_comboBoxDeviceSelect;
};
