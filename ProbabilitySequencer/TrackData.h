#pragma once

#include <string>
#include <vector>

#define TICKS_PER_BEAT 32

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

	int    GetNote(void) { return note; }
	void   SetNote(int n) { note=n; }

	int    GetTrackLength(void) { return trackLength; }
	void   SetTrackLength(int tl); // should update vector size

	int    GetNoteLength(void) { return noteLength; }
	void   SetNoteLength(int nl) { noteLength=nl; }

	string GetTrackName() { return trackName; }
	void   SetTrackName(string &tn) { trackName=string(tn); }

	float  GetValue(int pos) { if (pos<values.size()) return values[pos]; return 0; }
	void   SetValue(int pos, float val) { if (pos<values.size()) values[pos]=val;}

	void   ReadTrackFromFile(FILE *f);
	void   SaveTrackToFile(FILE *f);
	//..............
	void Tick(unsigned long ticks);
private:
	int trackId;
	int channel;
	int volume;
	int steps;
	int note;
	int noteLength;
	int trackLength;
	string trackName;

	bool muted;
	vector<float> values;
	bool noteOnSent;

	// private procedures
	void UpdateVectorSize(int newLen, int newSteps);
	bool isInt(float a);

	// 
	bool vectorSizeUpdating;

protected:
	bool muteStore[11];
};
