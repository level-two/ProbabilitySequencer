// SettingsDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ProbabilitySequencer.h"
#include "SettingsDlg.h"
#include <mmsystem.h>

extern int portId;
extern int beatPeriod;

// диалоговое окно SettingsDlg

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	if (beatPeriod) m_bpm = 60000/beatPeriod;
	else m_bpm = 120;
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BPM, m_bpm);
	DDV_MinMaxUInt(pDX, m_bpm, 0, 500);
	DDX_Control(pDX, IDC_COMBO_DEVICE_SELECT, m_comboBoxDeviceSelect);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_BPM, &CSettingsDlg::OnEnChangeEditBpm)
	ON_CBN_KILLFOCUS(IDC_COMBO_DEVICE_SELECT, &CSettingsDlg::OnCbnKillfocusComboDeviceSelect)
	ON_CBN_CLOSEUP(IDC_COMBO_DEVICE_SELECT, &CSettingsDlg::OnCbnCloseupComboDeviceSelect)
END_MESSAGE_MAP()


// обработчики сообщений SettingsDlg

BOOL CSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//m_bpm = 60000/beatPeriod;
	
	int num = midiOutGetNumDevs();
	for (int i = 0; i < num; i++)
	{
		MIDIOUTCAPS caps;
		midiOutGetDevCaps(i, &caps, sizeof(caps));

		m_comboBoxDeviceSelect.AddString(caps.szPname);
	}

	m_comboBoxDeviceSelect.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSettingsDlg::OnEnChangeEditBpm()
{
	UpdateData();
	if (m_bpm > 0)
		beatPeriod = (int)(60000/m_bpm);
}

void CSettingsDlg::OnCbnKillfocusComboDeviceSelect()
{
	portId = m_comboBoxDeviceSelect.GetCurSel();
}

void CSettingsDlg::OnCbnCloseupComboDeviceSelect()
{
	portId = m_comboBoxDeviceSelect.GetCurSel();
}
