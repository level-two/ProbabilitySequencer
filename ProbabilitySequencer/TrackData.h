#pragma once

#include <string>

using namespace std;

class CTrackData
{
public:
	CTrackData(void);
	CTrackData(CTrackData *td);
	~CTrackData(void);

	void Update();

	void Mute(bool mute);
	bool isMuted() { return muted; }
	
	void SaveMuteState(int memoryCell) { if (memoryCell<11) muteStore[memoryCell]=muted; }
	void RestoreMuteState(int memoryCell) { if (memoryCell<11) muted=muteStore[memoryCell]; }

	int    GetTrackId(void) { return trackId; }
	void   SetTrackId(int tId) { trackId=tId; }

	int    GetVolume(void) { return volume; }
	void   SetVolume(int v) { volume=v; }

	int    GetChannel(void) { return channel; }
	void   SetChannel(int c) { channel=c; }

	string GetStep(void) { return step; }
	void   SetStep(string &s) { step=string(s); }

	string GetNote(void) { return note; }
	void   SetNote(string &n) { note=string(n); }

	string GetTrackLength(void) { return trackLength; }
	void   SetTrackLength(string &tl) { trackLength=string(tl); }

	string GetNoteLength(void) { return noteLength; }
	void   SetNoteLength(string &nl) { noteLength=string(nl); }

	string GetTrackName() { return trackName; }
	void   SetTrackName(string &tn) { trackName=string(tn); }

private:
	int trackId;
	int length;
	int channel;
	int volume;
	string step;
	string note;
	string noteLength;
	string trackLength;
	string trackName;

	bool muted, muteStore[11];
};
