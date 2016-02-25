#ifndef PATH_FINDER_
#define PATH_FINDER_

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/StateMachineExecutor.h"

namespace Logic {
	class CPathFinder : public CStateMachineExecutor {
		RTTI_DECL;
		DEC_FACTORY(CPathFinder);

	public:
		/**
		Constructor por defecto.
		*/
		CPathFinder();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualización por frame
		*/
		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction> *getStateMachine();

	}; // class CPathFinder

	REG_FACTORY(CPathFinder);

} // namespace Logic

#endif // PATH_FINDER_