#ifndef TIMEMANAGER_H_
#define TIMEMANAGER_H_

namespace Logic
{
	class TimeManager
	{
	protected:
		static TimeManager _instance;

		long _time;
		bool _pause;

	private:
		TimeManager();

	public:
		static TimeManager& getSingleton()
		{ return _instance; }

		static TimeManager* getSingletonPtr()
		{ return &_instance; }

		long getTime() const
		{ return _time; }

		bool getPause() const
		{ return _pause; }

		void setPause(bool pause)
		{ _pause = pause; }

		//void setTime(const unsigned int time) { _time = time; }
		void tick(unsigned int msecs);

	private:
		/** Tiempo (ms) inicial */
		static const unsigned long long _maxTime; // 2 minutos
	};
}

#endif // TIMEMANAGER_H_