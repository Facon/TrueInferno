#ifndef SOUL_TASK_H_
#define SOUL_TASK_H_

#include "Logic\Entity\Components\Placeable.h"
#include "Logic\Maps\Map.h"

namespace AI{
	class CSoulTask {

	public:
		virtual bool execute(Logic::CMap *map) = 0;
		
		/** Método para clonar una instancia y poder transmitir con tranquilidad copias de la instancia como punteros a CSoulTask. Debe ser implementado en la clase hija */
		virtual CSoulTask* clone() = 0;
		
		/** Método (poco génerico) de conveniencia para obtener el objetivo de esta tarea */
		Logic::CPlaceable* getTarget() {
			return _target;
		}

	protected:
		Logic::CPlaceable* _target;

	};

}

#endif