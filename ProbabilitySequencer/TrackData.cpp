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
	UpdateVectorSize(steps, s, trackLength, trackLength);
	steps=s;
}

void CTrackData::SetTrackLength(int tl)
{
	UpdateVectorSize(steps, steps, trackLength, tl);
	trackLength=tl;
}


void CTrackData::UpdateVectorSize(int prevSteps, int newSteps, int prevLen, int newLen)
{
	vectorSizeUpdating = true;

	if (newLen>prevLen)
	{
		for (int i=values.size(); i<newLen*steps; i++)
			values.push_back(0.0);
	}

	float dummy;
	if (newSteps!=prevSteps)
	{
		if (/* newSteps>prevSteps && */isInt((float)newSteps/prevSteps))
		{
			for (int j=values.size()-1; j>0; j--) {
				values.insert(values.begin()+j, newSteps/prevSteps-1, 0.0);
			}

			for (int i=0; i<newSteps/prevSteps-1; i++) {
				values.push_back(0.0);
			}
		}
		else if (/* newSteps<prevSteps && */isInt((float)prevSteps/newSteps))
		{
			int j=0;
			while (j<values.size()) {
				values.erase(values.begin()+j+1, values.begin()+j+prevSteps/newSteps);
				j++;
			}
		}
		else
		{
			// delete 
			int j=0;
			while (j<values.size()) {
				values.erase(values.begin()+j+1, values.begin()+j+prevSteps);
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
