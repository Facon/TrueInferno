#include "TimeManager.h"

namespace Logic
{
	TimeManager::TimeManager() : _time(120000) // 2 min.
	{}

	TimeManager::~TimeManager()
	{}

	void TimeManager::tick(unsigned int msecs)
	{
		_time = (_time - msecs) % 120000;
	}
}