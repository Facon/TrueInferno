#ifndef TIMEMANAGER_H_
#define TIMEMANAGER_H_

namespace Logic
{
	class TimeManager
	{
	protected:
		unsigned int _time;
	public:
		TimeManager();
		~TimeManager();
		unsigned int getTime() const { return _time; }
		void setTime(const unsigned int time) { _time = time; }
		void tick(unsigned int msecs);
	};
}

#endif // TIMEMANAGER_H_