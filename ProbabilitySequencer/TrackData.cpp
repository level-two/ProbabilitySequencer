#include "StdAfx.h"
#include "TrackData.h"

CTrackData::CTrackData(void)
: volume(127),length(4),step("1/2"),channel(1),note("C3"),trackLength("1"), noteLength("1/8"),trackName("New Track")
{
}


CTrackData::CTrackData(CTrackData *td)
{
	SetTrackLength( td->GetTrackLength() );
	SetChannel( td->GetChannel() );
	SetVolume( td->GetVolume() );
	SetStep( td->GetStep() );
	SetNote( td->GetNote() );
	SetNoteLength( td->GetNoteLength() );
	SetTrackName( td->GetTrackName() );
}

CTrackData::~CTrackData(void)
{
}


void CTrackData::Update()
{

}

void CTrackData::Mute(bool mute)
{
	muted = mute;
	// send note off message here
}