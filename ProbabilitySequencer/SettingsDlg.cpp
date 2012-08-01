// SettingsDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "ProbabilitySequencer.h"
#include "SettingsDlg.h"
#include <mmsystem.h>
#include "RtMidi.h"
#include <string>

using namespace std;

extern RtMidiOut *midiOut;

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	portId = 0;
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

	int nPorts = midiOut->getPortCount();
	for (int i=0; i<nPorts; i++)
	{
		try {
			string portName = midiOut->getPortName(i);
			m_comboBoxDeviceSelect.AddString(portName.c_str());
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
	}

	if (portId > nPorts) portId = 0; // protection
	m_comboBoxDeviceSelect.SetCurSel(portId);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSettingsDlg::OnEnChangeEditBpm()
{
	UpdateData();
}

void CSettingsDlg::OnCbnKillfocusComboDeviceSelect()
{
	portId = m_comboBoxDeviceSelect.GetCurSel();
}

void CSettingsDlg::OnCbnCloseupComboDeviceSelect()
{
	portId = m_comboBoxDeviceSelect.GetCurSel();
}