//sound frame that uses fmod
//by Tyler van Gastel

#include "SoundFrame.h"

SoundFrame::SoundFrame() {
	f_sys = 0;
	f_ver = 0;
	numdrivers = 0;
}

void SoundFrame::Init() {
	FMOD::System_Create(&f_sys);
	f_sys->getNumDrivers(&numdrivers);
	if(numdrivers == 0)
	{
		f_sys->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}
	else
	{
		f_sys->getDriverCaps(0,&f_caps,0,&speakermode);
		f_sys->setSpeakerMode(speakermode);
		if(f_caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			f_sys->setDSPBufferSize(1024,10);
		}
		f_sys->getDriverInfo(0,name,256,0);
		if(name == "SigmaTel")
		{
			f_sys->setSoftwareFormat(48000,FMOD_SOUND_FORMAT_PCMFLOAT, 0,0,FMOD_DSP_RESAMPLER_LINEAR);
		}
	}
	f_res = f_sys->init(100,FMOD_INIT_NORMAL,0);
	if(f_res == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		f_sys->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		f_sys->init(100,FMOD_INIT_NORMAL,0);
	}
}

void SoundFrame::Restart() {
	ShutDown();
	FMOD::System_Create(&f_sys);
	f_sys->getNumDrivers(&numdrivers);
	if(numdrivers == 0)
	{
		f_sys->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}
	else
	{
		f_sys->getDriverCaps(0,&f_caps,0,&speakermode);
		f_sys->setSpeakerMode(speakermode);
		if(f_caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			f_sys->setDSPBufferSize(1024,10);
		}
		f_sys->getDriverInfo(0,name,256,0);
		if(name == "SigmaTel")
		{
			f_sys->setSoftwareFormat(48000,FMOD_SOUND_FORMAT_PCMFLOAT, 0,0,FMOD_DSP_RESAMPLER_LINEAR);
		}
	}
	f_res = f_sys->init(100,FMOD_INIT_NORMAL,0);
	if(f_res == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		f_sys->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		f_sys->init(100,FMOD_INIT_NORMAL,0);
	}
}

void SoundFrame::ShutDown() {
	f_ver = 0;
	numdrivers = 0;
	if(f_sys != 0)
	{
		f_sys->release();
		f_sys = 0;
	}
}

void SoundFrame::load(const char* name, Sound *sound) {
	f_sys->createSound(name,FMOD_3D|FMOD_HARDWARE,0,sound);
}

void SoundFrame::loadStream(const char* name, Sound *stream) {
	f_sys->createStream(name,FMOD_LOOP_NORMAL|FMOD_2D|FMOD_HARDWARE,0,stream);
}

void SoundFrame::Play(SoundStruct& soundS,float xPos,float yPos,float zPos,float xVel,float yVel,float zVel) {
	FMOD::Channel *chan = 0;
	FMOD_VECTOR pos = {xPos,yPos,zPos};
	FMOD_VECTOR vel = {xVel,yVel,zVel};

	f_sys->playSound(FMOD_CHANNEL_FREE,soundS.sound,false,&chan);
	chan->set3DAttributes(&pos,&vel);
	chan->set3DMinMaxDistance(soundS.minDist,soundS.maxDist);
	chan->setVolume(soundS.volume);
}

void SoundFrame::PlayStream(MusicStruct& soundS,bool mute) {
	FMOD::Channel *chan = 0;
	f_sys->playSound(FMOD_CHANNEL_FREE,soundS.sound,mute,&chan);
	chan->setVolume(soundS.volume);
}

int SoundFrame::getNumListen() {
	int numLis = 0;
	f_sys->get3DNumListeners(&numLis);
	return numLis;
}

void SoundFrame::setNumListen(int numListen) {
	f_sys->set3DNumListeners(numListen);
}

void SoundFrame::setListenProp(int numListen,listenProp& prop) {
	f_sys->set3DListenerAttributes(numListen,&prop.pos,&prop.vel,&prop.forward,&prop.up);
}

void SoundFrame::update() {
	f_sys->update();
}

SoundFrame::~SoundFrame() {
	ShutDown();
}