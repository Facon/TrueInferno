#include "TimeManager.h"
#include "Logic/Events/EventManager.h"

namespace Logic
{
	const unsigned long long TimeManager::_maxTime = 2 * 60 * 1000; // 2 minutos

	TimeManager TimeManager::_instance = TimeManager();

	TimeManager::TimeManager() : _time(_maxTime), _pause(false) // 2 min.
	{
	}

	void TimeManager::tick(unsigned int msecs)
	{
		//_time = (_time - msecs) % _maxTime;

		if (!_pause)
		{
			_time -= msecs;
			if (_time < 0) {
				_time = 0;
				// @TODO Hacer esto bien...
				Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::END_GAME);
			}
		}
	}
}