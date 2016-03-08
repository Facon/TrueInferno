#ifndef PATH_FINDER_
#define PATH_FINDER_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMPathFinder.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CPathFinder : public CStateMachineExecutor<AI::CSMPathFinderData> {
		RTTI_DECL;
		DEC_FACTORY(CPathFinder);

	public:
		/**
		Constructor por defecto.
		*/
		CPathFinder() {};

		virtual ~CPathFinder() {}

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
		AI::CStateMachine<AI::CLatentAction, AI::CSMPathFinderData>* getStateMachine(){
			return new AI::CSMPathFinder(_entity);
		}

	}; // class CPathFinder

	REG_FACTORY(CPathFinder);

} // namespace Logic

#endif // PATH_FINDER_