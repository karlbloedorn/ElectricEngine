#include "Audio.h"
#include <yse/reverb/reverb.hpp>

Audio::Audio(Timings * timings)
{
	this->timings = timings;
	YSE::System().init();
	MusicOrchestral.create("assets/sounds/footstep_a_mech_mega_01.wav");
	MusicOrchestral.setPosition(YSE::Vec(0, 60, 0));
	MusicOrchestral.setDoppler(false);
	MusicOrchestral.setRelative(false);
	MusicOrchestral.setVolume(0.2, 0);
	MusicOrchestral.setLooping(true);
	MusicOrchestral.setSize(160);
		//MusicOrchestral.moveTo(YSE::ChannelMusic());
	MusicOrchestral.play();
}

Audio::~Audio()
{
	YSE::System().close();
}

void Audio::Update(const glm::vec3 & cameraPosition, const glm::vec4 & cameraRotation){
	if (this->timings->CanAudioUpdate()){
		YSE::Listener().setPosition(YSE::Vec(cameraPosition.x, cameraPosition.y, cameraPosition.z));
		YSE::Listener().setOrientation(YSE::Vec(-cameraRotation.x, -cameraRotation.y, -cameraRotation.z), YSE::Vec(0, 1, 0));
		YSE::System().update();

	}
	
}


