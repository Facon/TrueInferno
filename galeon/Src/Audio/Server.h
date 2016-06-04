#ifndef AUDIO_SERVER_H
#define AUDIO_SERVER_H

#include <string>

typedef unsigned int FMOD_MODE;

namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

namespace Audio
{
	class CServer
	{
	public:
		static CServer* getSingletonPtr()
		{ return &_instance; }
		
		static bool Init();
		static void Release();

		FMOD::Sound* createSound(std::string& name, FMOD_MODE mode);
		void play();
		void playSound(FMOD::Sound* sound);
		void tick(unsigned int secs);

	protected:
		CServer();
		~CServer();
		bool open();
		void close();

		static CServer _instance;
		static std::string _path;
		static FMOD::System* _system;
		static FMOD::Channel* _channel;
		static FMOD::Channel* _channel2;

		bool _mute;
	};
}

#endif //AUDIO_SERVER_H