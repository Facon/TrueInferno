#include "TimeManager.h"
#include "Logic/Events/EventManager.h"

namespace Logic
{
	const long TimeManager::_maxRoundTime = 2 * 60 * 1000; // 2 minutos

	TimeManager TimeManager::_instance = TimeManager();

	TimeManager::TimeManager() : _time(0), _pause(false)
	{
	}

	void TimeManager::tick(unsigned int msecs)
	{
		//_time = (_time - msecs) % _maxTime;

		if (!_pause)
		{
			_time += msecs;
			if (_time > _maxRoundTime) {
				_time = _maxRoundTime;
				// @TODO Hacer esto bien...
				Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::END_GAME);
			}
		}
	}
}