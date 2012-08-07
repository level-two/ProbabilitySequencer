#include "StdAfx.h"
#include "TrackData.h"
#include "Math.h"
#include "RtMidi.h"

extern RtMidiOut *midiOut;

bool CTrackData::isInt(float a)
{
	float dmy;
	float res = modf(a,&dmy);
	return res==0;
}

CTrackData::CTrackData(void)
: volume(127),steps(1),channel(1),note(96),trackLength(1),noteLength(1),trackName("New Track")
{
	values.push_back(0.0); // initialization for len=1, steps=1;
	SetTrackLength(4); // re-set for len=4, steps=2
	SetSteps(2);

	noteOnSent = false;
	vectorSizeUpdating = false;
	Mute(false);
}


CTrackData::CTrackData(CTrackData *td)
{
	trackLength = 1;
	steps = 1;
	values.push_back(0.0); // initialization for len=1, steps=1;

	SetTrackLength( td->GetTrackLength() );
	SetChannel( td->GetChannel() );
	SetVolume( td->GetVolume() );
	SetSteps( td->GetSteps() );
	SetNote( td->GetNote() );
	SetNoteLength( td->GetNoteLength() );
	SetTrackName( td->GetTrackName() );

	int n = td->GetTrackLength() * td->GetSteps();
	for (int i=0; i<n; i++)
		SetValue(i,td->GetValue(i));

	Mute(td->isMuted());
	for (int i=0; i<11; i++)
		muteStore[i]=td->muteStore[i];

	noteOnSent = false;
}

CTrackData::~CTrackData(void)
{
}

void CTrackData::SetSteps(int s)
{
	UpdateVectorSize(trackLength, s);
}

void CTrackData::SetTrackLength(int tl)
{
	UpdateVectorSize(tl, steps);
}


void CTrackData::UpdateVectorSize(int newLen, int newSteps)
{
	vectorSizeUpdating = true;

	if (newLen>trackLength)
	{
		for (int i=values.size(); i<newLen*steps; i++)
			values.push_back(0.0);
	}

	float dummy;
	if (newSteps!=steps)
	{
		if (isInt((float)newSteps/steps))
		{
			for (int j=values.size()-1; j>0; j--) {
				values.insert(values.begin()+j, newSteps/steps-1, 0.0);
			}

			for (int i=0; i<newSteps/steps-1; i++) {
				values.push_back(0.0);
			}
		}
		else if (isInt((float)steps/newSteps))
		{
			int j=0;
			while (j<values.size()) {
				values.erase(values.begin()+j+1, values.begin()+j+steps/newSteps);
				j++;
			}
		}
		else
		{
			// delete 
			int j=0;
			while (j<values.size()) {
				values.erase(values.begin()+j+1, values.begin()+j+steps);
				j++;
			}

			// add
			for (int j=values.size()-1; j>0; j--) {
				values.insert(values.begin()+j, newSteps-1, 0.0);
			}

			for (int i=0; i<newSteps-1; i++) {
				values.push_back(0.0);
			}
		}
	}

	trackLength = newLen;
	steps = newSteps;

	vectorSizeUpdating = false;
}
void CTrackData::Update()
{

}

void CTrackData::Mute(bool mute)
{
	muted = mute;
	// send note off message here
}

void CTrackData::Tick(unsigned long ticks)
{
	if (vectorSizeUpdating) return;
	if (muted) return;

	bool shouldSendNoteOff = false;
	bool shouldSendNoteOn  = false;

	float stepLength = (float)TICKS_PER_BEAT/steps;
	int tick = ticks - stepLength*floor(ticks/stepLength);

	if (tick == 0)
	{
		int pos = ticks % (TICKS_PER_BEAT*trackLength);
		int noteIndex = floor(pos/stepLength);

		float p = rand()%100 / 100.0;

		if ( p < values[noteIndex] )
		{
			shouldSendNoteOff = true; // switch note off and then on
			shouldSendNoteOn = true;
		}
	}

	if ((tick==noteLength) || (tick==0 && stepLength==noteLength))
	{
		if (noteOnSent)
			shouldSendNoteOff = true;
	}

	if (shouldSendNoteOff)
	{
		noteOnSent = false;

		std::vector<unsigned char> message;
		message.push_back(0x80+channel-1); // bits: 1000 chnl
		message.push_back(note); // note
		message.push_back(volume); // velocity
		midiOut->sendMessage( &message );
	}

	if (shouldSendNoteOn)
	{
		noteOnSent = true;
		
		std::vector<unsigned char> message;
		message.push_back(0x90+channel-1); // bits: 1001 chnl
		message.push_back(note); // note
		message.push_back(volume); // velocity
		midiOut->sendMessage( &message );
	}
}

void CTrackData::ReadTrackFromFile(FILE *f)
{
	int newLen, newSteps;
	fscanf(f, "Length: %d\n", &newLen);
	fscanf(f, "Steps: %d\n", &newSteps);
	UpdateVectorSize(newLen, newSteps);

	fscanf(f, "Channel: %d\n", &channel);
	fscanf(f, "Volume: %d\n", &volume);
	fscanf(f, "Note: %d\n", &note);
	fscanf(f, "NoteLength: %d\n", &noteLength);

	char tn[255];
	fscanf(f, "TrackName: %s\n", tn);
	trackName = CString(tn);

	int m;
	fscanf(f, "Muted: %d\n", &m);
	muted = m>0;
	fscanf(f, "StoredMute:");
	
	for (int i=0; i<11; i++)
	{
		fscanf(f, "%d", &m);
		muteStore[i] = m>0;
	}
	
	int nValues;
	fscanf(f, "\nDataSize: %d\n", &nValues);

	if (nValues > trackLength*steps)
	{
		for (int i=trackLength*steps; i<nValues; i++)
		{
			values.push_back(0.0);
		}
	}

	fscanf(f, "Data: ");
	for(int i=0; i<nValues; i++)
	{
		float v;
		fscanf(f, "%f", &v);
		values[i] = v;
	}

	fscanf(f, "\n\n");

}

void CTrackData::SaveTrackToFile(FILE *f)
{
	fprintf(f, "Length: %d\n", trackLength);
	fprintf(f, "Steps: %d\n", steps);
	fprintf(f, "Channel: %d\n", channel);
	fprintf(f, "Volume: %d\n", volume);
	fprintf(f, "Note: %d\n", note);
	fprintf(f, "NoteLength: %d\n", noteLength);
	fprintf(f, "TrackName: %s\n", trackName.c_str());

	fprintf(f, "Muted: %d\n", muted ? 1 : 0);
	fprintf(f, "StoredMute: ");
	
	for (int i=0; i<11; i++)
		fprintf(f, "%d ", muteStore[i] ? 1 : 0);
	
	fprintf(f, "\nDataSize: %d\n", values.size());
	fprintf(f, "Data: ");
	for(int i=0; i<values.size(); i++)
		fprintf(f, "%.3f ", values[i]);

	fprintf(f, "\n\n");
}