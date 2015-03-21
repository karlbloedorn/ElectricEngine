#include "Audio.h"
#include <yse/reverb/reverb.hpp>

Audio::Audio(Timings * timings)
{

	this->timings = timings;
	YSE::System().init();

	YSE::System().getGlobalReverb().setActive(true);
	YSE::System().getGlobalReverb().setPreset(YSE::REVERB_GENERIC);
	YSE::ChannelMaster().attachReverb();


	GameReverb.create();
	GameReverb.setPosition(YSE::Vec(0, 4, 0)).setSize(100).setRollOff(200);
	GameReverb.setPreset(YSE::REVERB_SEWERPIPE);

	MusicOrchestral.create("assets/sounds/Pop-31.wav");
	MusicOrchestral.setPosition(YSE::Vec(0, 4, 0));
	MusicOrchestral.setDoppler(false);
	MusicOrchestral.setRelative(false);
	MusicOrchestral.setVolume(0.7, 0);
	MusicOrchestral.setLooping(true);
	MusicOrchestral.setSize(100);

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
