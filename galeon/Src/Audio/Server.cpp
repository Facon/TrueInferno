#include "Server.h"

#include <FMOD/lowlevel/fmod.hpp>
#include <cassert>

namespace FMOD
{
	class Sound;
}

namespace Audio
{
	std::string CServer::_path("media/sounds/");
	CServer CServer::_instance;
	FMOD::System* CServer::_system = nullptr;
	FMOD::Channel* CServer::_channel = nullptr;
	FMOD::Channel* CServer::_channel2 = nullptr;

	CServer::CServer() : _mute(true)
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
		result = _system->createStream((_path + std::string("audio_hito_3_suave.ogg")).c_str(), FMOD_LOOP_NORMAL, nullptr, &sound);

		assert(!result);

		//FMOD::Channel* channel = nullptr;

		result = _system->playSound(sound, 0, true, &_channel);
		//result = _system->playSound(sound, 0, true, nullptr);
		
		assert(!result);

		float volume = 0.3f;
		// valor entre 0 y 1
		_channel->setVolume(volume);

		return true;
	}

	void CServer::Release()
	{
		_system->release();
		_system->close();
	}

	FMOD::Sound* CServer::createSound(std::string& name, FMOD_MODE mode)
	{
		FMOD::Sound* sound;
		_system->createSound((_path + name).c_str(), mode, nullptr, &sound);
		
		return sound;
	}

	void CServer::play()
	{
		// TODO Disable sound
		if (_mute)
			return;

		FMOD_RESULT result = FMOD_RESULT::FMOD_OK;

		result = _channel->setPaused(false);

		assert(!result);
	}

	void CServer::playSound(FMOD::Sound* sound)
	{
		if (_mute)
			return;

		_system->playSound(sound, 0, false, &_channel2);
		_channel2->setVolume(0.4f);
	}

	void CServer::tick(unsigned int secs)
	{
		_system->update();
	}

	bool CServer::open()
	{
		return true;
	}
	
	void CServer::close()
	{}
}