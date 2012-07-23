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

	int    GetSteps(void) { return steps; }
	void   SetSteps(int s) { steps=s; }

	string GetNote(void) { return note; }
	void   SetNote(string &n) { note=string(n); }

	int    GetTrackLength(void) { return trackLength; }
	void   SetTrackLength(int tl) { trackLength=tl; }

	int    GetNoteLength(void) { return noteLength; }
	void   SetNoteLength(int nl) { noteLength=nl; }

	string GetTrackName() { return trackName; }
	void   SetTrackName(string &tn) { trackName=string(tn); }

private:
	int trackId;
	int length;
	int channel;
	int volume;
	int steps;
	string note;
	int noteLength;
	int trackLength;
	string trackName;

	bool muted, muteStore[11];
};
