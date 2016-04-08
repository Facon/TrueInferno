#include "TimeManager.h"
#include "Logic/Events/EventManager.h"

namespace Logic
{
	TimeManager::TimeManager() // 2 min.
	{
		_time = _maxTime;
	}

	TimeManager::~TimeManager()
	{}

	void TimeManager::tick(unsigned int msecs)
	{
		//_time = (_time - msecs) % _maxTime;

		_time -= msecs;
		if (_time < 0) {
			_time = 0;
			// @TODO Hacer esto bien...
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::END_GAME);
		}
	}
}