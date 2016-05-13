#include "TimeManager.h"

#include <math.h>

#include "BaseSubsystems/ScriptManager.h"
#include "Logic/Events/EventManager.h"
#include "Logic/Maps/Managers/GameManager.h"

namespace Logic
{
	CTimeManager CTimeManager::_instance = CTimeManager();

	CTimeManager::CTimeManager() : _roundTime(0), _globalTime(0), _pause(false)
	{
	}

	CTimeManager::~CTimeManager() {};

	bool CTimeManager::Init()
	{
		if (!_instance.open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	void CTimeManager::Release()
	{
		_instance.close();

	} // Release

	/** Devuelve el tiempo (ms) global transcurrido */
	long CTimeManager::getElapsedGlboalTime() const
	{
		return _globalTime;
	}

	/** Devuelve el tiempo (ms) de ronda transcurrido */
	long CTimeManager::getElapsedRoundTime() const
	{
		return _roundTime;
	}

	/** Devuelve el tiempo (ms) de ronda restante */
	long CTimeManager::getRemainingRoundTime() const
	{
		return std::max(_maxRoundTime - _roundTime, (long) 0);
	}

	void CTimeManager::startNextRound(){
		// Dejamos activado el tiempo
		_pause = false;

		// Reiniciamos el tiempo de ronda
		_roundTime = 0;

		// Reiniciamos el tiempo máximo de ronda
		_maxRoundTime = MAX_ROUND_TIME;
	}

	void CTimeManager::changeCurrentRoundTime(int roundTimeChange){
		_maxRoundTime += roundTimeChange;
	}

	bool CTimeManager::open() {
		luaRegister();

		_roundTime = 0;
		_globalTime = 0;
		_pause = false;
		_maxRoundTime = MAX_ROUND_TIME;
		
		return true;
	} // open

	void CTimeManager::close() {
	} // close

	void CTimeManager::luaRegister() {
		// CTimeManager.
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CTimeManager>("CTimeManager")
				.def("getElapsedGlboalTime", &CTimeManager::getElapsedGlboalTime)
				.def("getElapsedRoundTime", &CTimeManager::getElapsedRoundTime)
				.def("getRemainingRoundTime", &CTimeManager::getRemainingRoundTime)
				.scope
				[
					luabind::def("getSingletonPtr", &CTimeManager::getSingletonPtr)
				]
			];

	} // luaRegister

	void CTimeManager::tick(unsigned int msecs)
	{
		_globalTime += msecs;

		if (!_pause)
		{
			_roundTime += msecs;
			if (_roundTime > _maxRoundTime){
				// Fijamos el tiempo de ronda en el tiempo máximo
				_roundTime = _maxRoundTime;

				// Paramos el tiempo
				_pause = true;

				// @TODO Hacer esto bien...
				//Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::END_GAME);

				// Notificamos el final de ronda al GameManager
				Logic::CGameManager::getSingletonPtr()->roundFinished();
			}
		}
	}

}