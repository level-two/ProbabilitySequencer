#include "StdAfx.h"
#include "TrackData.h"

CTrackData::CTrackData(void)
: volume(0),length(0),step(0),channel(0),note(0),noteLength(0),trackName("")
{
}


CTrackData::CTrackData(CTrackData *td)
{


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