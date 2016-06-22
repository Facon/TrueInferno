#include "Server.h"

#include <FMOD/lowlevel/fmod.hpp>
#include <cassert>

namespace FMOD
{
	class Sound;
}

namespace Audio
{
	CServer CServer::_instance;
	std::unordered_map<std::string, FMOD::Sound*> CServer::_sounds;

	CServer::CServer() : _path("media/sounds/"), _system(nullptr), _mute(false)
	{}

	CServer::~CServer()
	{}

	bool CServer::Init()
	{
		FMOD_RESULT result;
		
		result = FMOD::System_Create(&_system);

		assert(!result);
		
		result = _system->init(32, FMOD_INIT_NORMAL, nullptr);

		assert(!result);

		appendSounds();

		return true;
	}

	void CServer::Release()
	{
		_system->release();
		_system->close();
	}

	void CServer::appendSounds()
	{
		_sounds.insert(std::make_pair("audio_hito_3", createStream(std::string("audio_hito_3_suave.ogg"), FMOD_LOOP_NORMAL)));
		_sounds.insert(std::make_pair("building_complete", createSound(std::string("building_complete.ogg"), FMOD_3D)));
		_sounds.insert(std::make_pair("error", createSound(std::string("error.wav"), FMOD_DEFAULT)));
		_sounds.insert(std::make_pair("event", createSound(std::string("event.wav"), FMOD_DEFAULT)));
		_sounds.insert(std::make_pair("round_finished", createSound(std::string("round_finished.wav"), FMOD_DEFAULT)));
		_sounds.insert(std::make_pair("victory", createSound(std::string("victory.mp3"), FMOD_DEFAULT)));
	}


	FMOD::Sound* CServer::createSound(std::string& name, FMOD_MODE mode)
	{
		FMOD_RESULT result;
		FMOD::Sound* sound;
		
		result = _system->createSound((_path + name).c_str(), mode, nullptr, &sound);
		
		assert(!result);

		return sound;
	}

	FMOD::Sound* CServer::createStream(std::string& name, FMOD_MODE mode)
	{
		FMOD_RESULT result;
		FMOD::Sound* sound;

		result = _system->createStream((_path + name).c_str(), mode, nullptr, &sound);

		assert(!result);

		return sound;
	}

	void CServer::playSound(const std::string& sound, Channel channel, float volume)
	{
		if (_mute)
			return;

		assert((_sounds.find(sound) != _sounds.end()) && "Sonido no encontrado.");

		_system->playSound(_sounds[sound], nullptr, false, &_channels[channel]);
		_channels[channel]->setVolume(volume);
	}


	void CServer::playBgSound(const std::string& sound)
	{
		const float volume = 0.2f;

		playSound(sound, Channel::BACKGROUND, volume);
	}

	void CServer::playSfxSound(const std::string& sound)
	{
		const float volume = 0.2f;

		playSound(sound, Channel::SFX, volume);
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