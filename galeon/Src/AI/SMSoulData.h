#ifndef SM_SOUL_H_DATA_
#define SM_SOUL_H_DATA_

#include <vector>
#include "OgreVector3.h"

namespace AI {
	class CSoulTask;
}

namespace AI {
	class CSMSoulData {

	public:
		CSMSoulData() : _task(nullptr), _path(nullptr), _numSouls(0) {}
		
		virtual ~CSMSoulData()
		{
			if (_task)
			{
				delete _task;
				_task = nullptr;
			}
		}

		CSoulTask* getTask(){
			return _task;
		}

		void setTask(CSoulTask* task){
			_task = task;
		}

		int getNumSouls(){
			return _numSouls;
		}

		void setNumSouls(int numSouls){
			_numSouls = numSouls;
		}

		std::vector<Ogre::Vector3>* getPath(){
			return _path;
		}

		void setPath(std::vector<Ogre::Vector3>* path){
			_path = path;
		}
	
	private:
		CSoulTask* _task;

		int _numSouls;

		std::vector<Ogre::Vector3>* _path;
	};
}

#endif