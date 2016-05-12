#include "Server.h"

#include <FMOD/lowlevel/fmod.hpp>
#include <cassert>

namespace Audio
{
	CServer CServer::_instance;
	FMOD::System* CServer::_system = nullptr;
	FMOD::Channel* CServer::_channel = nullptr;

	CServer::CServer()
	{}

	CServer::~CServer()
	{}

	bool CServer::Init()
	{
		FMOD_RESULT result;
		
		result = FMOD::System_Create(&_system);

		assert(!result);
		
		result = _system->init(32, FMOD_INIT_NORMAL, 0);

		assert(!result);

		FMOD::Sound* sound;
		result = _system->createStream("media/sounds/audio_hito_3_suave.ogg", FMOD_DEFAULT, nullptr, &sound);

		assert(!result);

		//FMOD::Channel* channel = nullptr;

		result = _system->playSound(sound, 0, true, &(CServer::_channel));

		assert(!result);

		float volume = 1.0f;
		// valor entre 0 y 1
		_channel->setVolume(volume);

		return true;
	}

	void CServer::Release()
	{
		_system->release();
		_system->close();
	}

	void CServer::play()
	{
		// TODO Disable sound
		if (true)
			return;

		FMOD_RESULT result;

		result = _channel->setPaused(false);

		assert(!result);
		_system->update();
	}

	void CServer::tick(unsigned int secs)
	{}

	bool CServer::open()
	{
		return true;
	}
	
	void CServer::close()
	{}
}