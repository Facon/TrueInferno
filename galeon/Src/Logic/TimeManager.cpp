#include "TimeManager.h"

#include <math.h>

#include "BaseSubsystems/ScriptManager.h"
#include "Logic/Maps/Managers/GameManager.h"
#include "Map/MapEntity.h"

namespace Logic
{
	CTimeManager CTimeManager::_instance = CTimeManager();

	CTimeManager::CTimeManager() : _elapsedRoundTime(0), _roundTime(0), _defaultRoundTime(0), _elapsedGlobalTime(0), _pause(false)
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


	bool CTimeManager::open() {
		luaRegister();

		return true;
	} // open

	void CTimeManager::close() {
	} // close

	void CTimeManager::spawn(const Map::CEntity *managerInfo)
	{
		_defaultRoundTime = managerInfo->getIntAttribute("defaultRoundTime");

		_elapsedRoundTime = 0;
		_elapsedGlobalTime = 0;
		_pause = false;
		_roundTime = _defaultRoundTime;
	}

	void CTimeManager::tick(unsigned int msecs)
	{
		_elapsedGlobalTime += msecs;

		if (!_pause)
		{
			_elapsedRoundTime += msecs;
			if (_elapsedRoundTime > _roundTime){
				// Fijamos el tiempo de ronda en el tiempo máximo
				_elapsedRoundTime = _roundTime;

				// Paramos el tiempo
				_pause = true;

				// @TODO Hacer esto bien...
				//Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::END_GAME);

				// Notificamos el final de ronda al GameManager
				Logic::CGameManager::getSingletonPtr()->roundFinished();
			}
		}
	}

	/** Devuelve el tiempo (ms) global transcurrido */
	long CTimeManager::getElapsedGlboalTime() const
	{
		return _elapsedGlobalTime;
	}

	/** Devuelve el tiempo (ms) de ronda transcurrido */
	long CTimeManager::getElapsedRoundTime() const
	{
		return _elapsedRoundTime;
	}

	/** Devuelve el tiempo (ms) de ronda restante */
	long CTimeManager::getRemainingRoundTime() const
	{
		return std::max(_roundTime - _elapsedRoundTime, (long) 0);
	}

	void CTimeManager::startNextRound(){
		// Dejamos activado el tiempo
		_pause = false;

		// Reiniciamos el tiempo de ronda
		_elapsedRoundTime = 0;

		// Reiniciamos el tiempo de ronda
		_roundTime = _defaultRoundTime;
	}

	void CTimeManager::changeCurrentRoundTime(int roundTimeChange){
		_roundTime += roundTimeChange;
	}

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

}
