#include "Audio.h"

Audio::Audio(Timings * timings)
{
	this->timings = timings;
	YSE::System().init();

	MusicOrchestral.create("assets/TownTheme.ogg");
	MusicOrchestral.setPosition(YSE::Vec(0, 60, 0));
	MusicOrchestral.setDoppler(true);
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
		YSE::Listener().setOrientation(YSE::Vec(cameraRotation.x, cameraRotation.y, cameraRotation.z), YSE::Vec(0, 1, 0));
		YSE::System().update();

		
	}
	
}


