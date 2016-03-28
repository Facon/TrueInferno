#ifndef RESOURCE_PROVIDER_H_
#define RESOURCE_PROVIDER_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMResourceProvider.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CResourceProvider : public CStateMachineExecutor<AI::CSMResourceProviderData> {
		RTTI_DECL;
		DEC_FACTORY(CResourceProvider);

	public:
		/**
		Constructor por defecto.
		*/
		CResourceProvider() {}

		virtual ~CResourceProvider() {}

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
		AI::CStateMachine<AI::CLatentAction, AI::CSMResourceProviderData>* getStateMachine(){
			return new AI::CSMResourceProvider(_entity);
		}

	}; // class CResourceProvider

	REG_FACTORY(CResourceProvider);

} // namespace Logic

#endif // RESOURCE_PROVIDER_H_