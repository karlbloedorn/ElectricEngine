#include "Sound.hpp"

Sound::Sound(){}

void Sound::Setup(string assetPath){
	auto result = FMOD::System_Create(&system);   
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
	result = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	system->createSound( (assetPath + "audio/" + "vulture-1.wav").c_str(), FMOD_3D, 0, &birdSound);
	birdSound->setMode(FMOD_LOOP_NORMAL);

	system->playSound(birdSound, false, true, &birdChannel);

}
void Sound::Update(glm::vec3 position, glm::vec3 look, glm::vec3 up, glm::vec3 birdPosition){
	FMOD_VECTOR listenerpos;
	listenerpos.x = position.x;
	listenerpos.y = position.y;
	listenerpos.z = position.z;

	FMOD_VECTOR lookvector;
	lookvector.x = look.x;
	lookvector.y = look.y;
	lookvector.z = look.z;

	FMOD_VECTOR upvector;
	upvector.x = up.x;
	upvector.y = up.y;
	upvector.z = up.z;

	system->set3DListenerAttributes(0, &listenerpos,NULL, &lookvector, &upvector);


	//testing bird position
	FMOD_VECTOR birdposition;
	birdposition.x = birdPosition.x;
	birdposition.y = birdPosition.y;
	birdposition.z = birdPosition.z;
	birdChannel->set3DAttributes(&birdposition, NULL, NULL);
	birdChannel->setPaused(false);

	system->update();
}



bool Sound::AddEntity(Entity * entity, string assetPath){
	return false;
}

