#include "Server.h"

#include <FMOD/studio/fmod_studio.hpp>
#include <cassert>

namespace Audio
{
	CServer::CServer()
	{}

	CServer::~CServer()
	{}

	bool CServer::Init()
	{
		_system = nullptr;
		assert(FMOD::Studio::System::create(&_system));
	}

	void CServer::Release()
	{
		_system->release();
	}

	void CServer::tick(unsigned int secs)
	{}

	bool CServer::open()
	{}
	
	void CServer::close()
	{}
}