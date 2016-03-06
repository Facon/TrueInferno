#ifndef AUDIO_SERVER_H
#define AUDIO_SERVER_H

namespace FMOD
{
	class System;
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

		void tick(unsigned int secs);

	protected:
		CServer();
		~CServer();
		bool open();
		void close();

		static CServer _instance;
		static FMOD::System* _system;
	};
}

#endif //AUDIO_SERVER_H