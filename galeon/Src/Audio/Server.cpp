#include "Server.h"

#include <FMOD/studio/fmod_studio.hpp>
#include <cassert>

namespace Audio
{
	FMOD::Studio::System* CServer::_system = nullptr;

	CServer::CServer()
	{}

	CServer::~CServer()
	{}

	bool CServer::Init()
	{
		assert(FMOD::Studio::System::create(&_system));

		FMOD::Studio::Bank* bank = nullptr;
		//_system->loadBankFile("");

		return true;
	}

	void CServer::Release()
	{
		_system->release();
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