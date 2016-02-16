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
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Consulta si el edificio tiene suficientes trabajadores para considerarse activo */
		bool isActive();

		/** Gestor de mensajes de trabajadores. Modifica la cantidad de trabajadores presentes en el edificio */
		virtual bool CWorkBuilding::HandleMessage(const WorkerMessage& msg);

	private:
		// N�mero m�nimo de trabajadores para que funcione el edificio
		unsigned int _minWorkers;

		// N�mero m�ximo de trabajadores permitido
		unsigned int _maxWorkers;

		// N�mero actual de trabajadores
		unsigned int _currentWorkers;

		// N�mero de trabajadores solicitados a cambiar para el pr�ximo tick
		int _changeNumWorkers;

	}; // class CWorkBuilding

	REG_FACTORY(CWorkBuilding);

} // namespace Logic

#endif // WORK_BUILDING_H_