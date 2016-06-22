#ifndef AUDIO_SERVER_H
#define AUDIO_SERVER_H

#include <boost/array.hpp>
#include <unordered_map>

typedef unsigned int FMOD_MODE;

namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

namespace Audio
{
	enum Channel
	{
		BACKGROUND,
		SFX,
	};

	class CServer
	{
	public:
		static CServer* getSingletonPtr()
		{ return &_instance; }
		
		bool Init();
		void Release();

		FMOD::Sound* createSound(std::string& name, FMOD_MODE mode);
		FMOD::Sound* createStream(std::string& name, FMOD_MODE mode);
		void playSound(const std::string& sound, Channel channel, float volume);
		void playBgSound(const std::string& sound);
		void playSfxSound(const std::string& sound);
		void tick(unsigned int secs);

	protected:
		CServer();
		~CServer();
		bool open();
		void close();
		void appendSounds();

		static CServer _instance;
		static std::unordered_map<std::string, FMOD::Sound*> _sounds;

		std::string _path;
		FMOD::System* _system;

		boost::array<FMOD::Channel*, 2> _channels;

		bool _mute;
	};
}

#endif //AUDIO_SERVER_H