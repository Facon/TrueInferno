#ifndef WORK_BUILDING_H_
#define WORK_BUILDING_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

namespace Logic {
	class CWorkBuilding : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CWorkBuilding);

	public:
		/**
		Constructor por defecto.
		*/
		CWorkBuilding();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualización por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Consulta si el edificio tiene suficientes trabajadores para considerarse activo */
		bool isActive();

		/** Gestor de mensajes de trabajadores. Modifica la cantidad de trabajadores presentes en el edificio */
		virtual bool CWorkBuilding::HandleMessage(const WorkerMessage& msg);

	private:
		// Número mínimo de trabajadores para que funcione el edificio
		unsigned int _minWorkers;

		// Número máximo de trabajadores permitido
		unsigned int _maxWorkers;

		// Número actual de trabajadores
		unsigned int _currentWorkers;

		// Número de trabajadores solicitados a cambiar para el próximo tick
		int _changeNumWorkers;

	}; // class CWorkBuilding

	REG_FACTORY(CWorkBuilding);

} // namespace Logic

#endif // WORK_BUILDING_H_