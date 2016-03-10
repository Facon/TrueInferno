#ifndef SM_LOGISTICS_DATA_
#define SM_LOGISTICS_DATA_

#include "SoulTask.h"

namespace AI {
	class CSMLogisticsData {

	public:
		CSMLogisticsData() : _task(nullptr) {}

		virtual ~CSMLogisticsData() {
			if (_task != nullptr){
				delete _task;
				_task = nullptr;
			}
		}
		
		CSoulTask* getTask(){
			return _task;
		}

		void setTask(CSoulTask* task){
			// Limpiamos la task anterior si había
			if (_task != nullptr){
				delete _task;
				_task = nullptr;
			}

			// Guardamos una clon de la task pasada
			_task = task->clone();
		}

	private:
		CSoulTask* _task;
	};
}

#endif