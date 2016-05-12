#ifndef RESOURCE_BUILDING_H_
#define RESOURCE_BUILDING_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMResourceBuilding.h"
#include "Logic/ResourcesManager.h"

namespace Logic {
	class CResourceBuilding : public CStateMachineExecutor<AI::CSMResourceBuildingData> {
		RTTI_DECL;
		DEC_FACTORY(CResourceBuilding);

	public:
		CResourceBuilding() {}
		
		virtual ~CResourceBuilding() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		/** Devuelve la cantidad de recursos almacenados del tipo dado */
		int getStoredResources(ResourceType type);

		/** Devuelve la cantidad de recursos disponibles del tipo dado = Recursos existentes menos los reservados */
		int getAvailableResources(ResourceType type);

		/** Devuelve la cantidad máxima de recursos almacenables del tipo dado */
		int getMaxResources(ResourceType type);

		/** Modifica los recursos del tipo dado según la cantidad positiva o negativa indicada.
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool changeStoredResources(ResourceType type, int quantity);

		/** Reserva los recursos del tipo dado según la cantidad positiva indicada.
		Con el flag allowPartial a true se permiten reservas parciales, esto es, se reserva todo lo que haya disponible aunque no llegue a lo solicitado.
		En finallyReserved se almacena la cantidad finalmente reservada.
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool reserveResources(ResourceType type, int quantity, bool allowPartial, int& finallyReserved);

		/** Libera los recursos reservados del tipo dado según la cantidad positiva indicada.
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool freeReservedResources(ResourceType type, int quantity);

		/** Reclama los recursos previamente reservados del tipo dado en la cantidad indicada
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool claimReservedResources(ResourceType type, int quantity);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMResourceBuildingData>* getStateMachine(){
			return new AI::CSMResourceBuilding(_entity);
		}
	};
	
	REG_FACTORY(CResourceBuilding);

} // namespace Logic

#endif // RESOURCE_BUILDING_H_