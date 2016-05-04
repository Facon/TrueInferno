#ifndef TIMEMANAGER_H_
#define TIMEMANAGER_H_

#include <algorithm>

namespace Logic
{
	class TimeManager
	{
	protected:
		static TimeManager _instance;

		/** Tiempo (ms) transcurrido */
		long _time;

		/** Flag a true si el tiempo está pausado */
		bool _pause;

	private:
		TimeManager();

	public:
		static TimeManager& getSingleton()
		{ return _instance; }

		static TimeManager* getSingletonPtr()
		{ return &_instance; }

		/** Devuelve el tiempo (ms) transcurrido */
		long getElapsedTime() const
		{ return _time; }

		/** Devuelve el tiempo (ms) restante */
		long getRemainingTime() const
		{ return (_maxRoundTime - _time); }

		bool getPause() const
		{ return _pause; }

		void setPause(bool pause)
		{ _pause = pause; }

		//void setTime(const unsigned int time) { _time = time; }
		void tick(unsigned int msecs);

	private:
		/** Tiempo (ms) inicial */
		static const long _maxRoundTime; // 2 minutos
	};
}

#endif // TIMEMANAGER_H_