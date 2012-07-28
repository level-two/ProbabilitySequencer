#pragma once

#include <string>
#include <vector>

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
	void   SetSteps(int s); // should update vector size

	string GetNote(void) { return note; }
	void   SetNote(string &n) { note=string(n); }

	int    GetTrackLength(void) { return trackLength; }
	void   SetTrackLength(int tl); // should update vector size

	int    GetNoteLength(void) { return noteLength; }
	void   SetNoteLength(int nl) { noteLength=nl; }

	string GetTrackName() { return trackName; }
	void   SetTrackName(string &tn) { trackName=string(tn); }

	//..............
	float  GetValue(int pos) { if (pos<values.size()) return values[pos]; return 0; }
	void   SetValue(int pos, float val) { if (pos<values.size()) values[pos]=val;}

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
	vector<float> values;

	// private procedures
	void UpdateVectorSize(int prevSteps, int newSteps, int prevLen, int newLen);
	bool isInt(float a);
};
