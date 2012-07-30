#include "StdAfx.h"
#include "TrackData.h"
#include "Math.h"

bool CTrackData::isInt(float a)
{
	float dmy;
	float res = modf(a,&dmy);
	return res==0;
}

CTrackData::CTrackData(void)
: volume(127),length(4),steps(1),channel(1),note("C3"),trackLength(1),noteLength(1),trackName("New Track")
{
	values.push_back(0.0); // initialization for len=1, steps=1;
	SetTrackLength(4); // re-set for len=4, steps=2
	SetSteps(2);

	noteOnSent = false;
}


CTrackData::CTrackData(CTrackData *td)
{
	SetTrackLength( td->GetTrackLength() );
	SetChannel( td->GetChannel() );
	SetVolume( td->GetVolume() );
	SetSteps( td->GetSteps() );
	SetNote( td->GetNote() );
	SetNoteLength( td->GetNoteLength() );
	SetTrackName( td->GetTrackName() );
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
				values.erase(values.begin()+j, values.begin()+j+prevSteps-1);
				j++;
			}

			// add
			for (int j=values.size()-1; j>0; j--) {
				values.insert(values.begin()+j, newSteps, 0.0);
			}

			for (int i=0; i<newSteps; i++) {
				values.push_back(0.0);
			}
		}
	}
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
	bool shouldSendNoteOff = false;
	bool shouldSendNoteOn  = false;

	float stepLength = (float)TICKS_PER_BEAT/steps;
	int tick = ticks - stepLength*floor(ticks/stepLength);

	if (tick == 0)
	{
		int pos = ticks % (TICKS_PER_BEAT*length);
		int noteIndex = floor(pos/stepLength);

		float p = rand()%100 / 100.0;
		
		if ( p < values[noteIndex] )
			shouldSendNoteOn = true;
	}

	if ((tick==noteLength) || (tick==0 && stepLength==noteLength))
	{
		if (noteOnSent)
			shouldSendNoteOff = true;
	}


	/*
	int pos = ticks % (TICKS_PER_BEAT*length);

	int prevEvIndex = pos==0 ? TICKS_PER_BEAT : (pos-1)%TICKS_PER_BEAT;
	int evIndex = pos%TICKS_PER_BEAT;

	bool shouldSendNoteOff = false;
	bool shouldSendNoteOn  = false;

	if (prevEvIndex<=noteLength && evIndex>=noteLength && noteOnSent)
	{
		shouldSendNoteOff = true;
	}

	float stepLength = (float)TICKS_PER_BEAT/steps;
	if (prevEvIndex<stepLength && evIndex>=stepLength)
	{
		int noteIndex = floor(pos/stepLength);
		float p = rand()%100 / 100.0;
		if ( p < values[noteIndex] )
			shouldSendNoteOn = true;
	}
*/

	if (shouldSendNoteOff)
	{
		noteOnSent = false;

		// send NoteOff
	}

	if (shouldSendNoteOn)
	{
		noteOnSent = true;

		// send NoteOn
	}
}
