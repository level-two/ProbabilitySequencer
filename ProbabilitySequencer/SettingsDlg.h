#pragma once
#include "afxwin.h"


// ���������� ���� SettingsDlg

class CSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CSettingsDlg();

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

//---------------------------------------------------------

public:
	UINT GetBpm(void) { return m_bpm; }
	void SetBpm(UINT bpm) { m_bpm = bpm; }
	int  GetPortId(void) { return portId; }
	void SetPortId(int id) { portId = id; }

private:
	int portId;
};
