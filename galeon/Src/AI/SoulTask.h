#ifndef SOUL_TASK_H_
#define SOUL_TASK_H_

#include "Logic\Maps\Map.h"

namespace AI{
	class CSoulTask {

	public:
		CSoulTask(Logic::CMap *map, const Logic::TEntityID& target) : _map(map), _target(target) {}

		virtual ~CSoulTask() {};

		/** M�todo que se ejecutar� al comenzar la tarea */
		virtual bool start() = 0;

		/** M�todo que se ejecutar� en el objetivo al llegar el alma */
		virtual bool execute() = 0;

		/** Devuelve el mapa al que pertenece el alma */
		virtual Logic::CMap* getMap(){
			return _map;
		}

		/** Devuelve el objetivo hasta donde debe desplazarse el alma */
		virtual Logic::TEntityID getTarget(){
			return _target;
		}

		/** M�todo para clonar una instancia y poder transmitir con tranquilidad copias de la instancia como punteros a CSoulTask. Debe ser implementado en la clase hija */
		virtual CSoulTask* clone() = 0;

	protected:
		Logic::CMap *_map;

		/** Objetivo hasta donde debe desplazarse el alma */
		Logic::TEntityID _target;
	};

}

#endif