#include "Server.h"

#include <FMOD/lowlevel/fmod.hpp>
#include <cassert>

namespace Audio
{
	FMOD::System* CServer::_system = nullptr;

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
		result = _system->createStream("media/sounds/hades.ogg", FMOD_DEFAULT, nullptr, &sound);

		assert(!result);

		FMOD::Channel* channel = nullptr;

		result = _system->playSound(sound, 0, false, &channel);

		assert(!result);

		float volume = 1.0f;
		// valor entre 0 y 1
		channel->setVolume(volume);

		return true;
	}

	void CServer::Release()
	{
		_system->release();
		_system->close();
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