#ifndef AUDIO_SERVER_H
#define AUDIO_SERVER_H

namespace FMOD
{
	class System;
	class Channel;
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

		void play();
		void tick(unsigned int secs);

	protected:
		CServer();
		~CServer();
		bool open();
		void close();

		static CServer _instance;
		static FMOD::System* _system;
		static FMOD::Channel* _channel;
	};
}

#endif //AUDIO_SERVER_H