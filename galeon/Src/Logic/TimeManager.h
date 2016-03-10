#ifndef TIMEMANAGER_H_
#define TIMEMANAGER_H_

namespace Logic
{
	class TimeManager
	{
	protected:
		long _time;

	public:
		TimeManager();
		~TimeManager();

		long getTime() const {
			return _time; 
		}
		//void setTime(const unsigned int time) { _time = time; }
		void tick(unsigned int msecs);

	private:
		/** Tiempo (ms) inicial */
		const unsigned long _maxTime = 10 * 1000; // 2 minutos
	};
}

#endif // TIMEMANAGER_H_