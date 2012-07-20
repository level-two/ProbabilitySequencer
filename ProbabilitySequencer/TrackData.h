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

	int  GetTrackId(void) { return trackId; }
	void SetTrackId(int tId) { trackId=tId; }

	int  GetVolume(void) { return volume; }
	void SetVolume(int v) { volume=v; }

	int  GetStep(void) { return step; }
	void SetStep(int s) { step=s; }

	int  GetChannel(void) { return channel; }
	void SetChannel(int c) { channel=c; }

	int  GetNote(void) { return note; }
	void SetNote(int n) { note=n; }

	int  GetNoteLengthe(void) { return noteLength; }
	void SetNoteLength(int nl) { noteLength=nl; }

	string GetTrackName() { return trackName; }
	void SetTrackName(string tn) { trackName=tn; }

private:
	int trackId;
	int length;
	int step;
	int channel;
	int note;
	int noteLength;
	int volume;
	bool muted, muteStore[11];
	string trackName;
};
