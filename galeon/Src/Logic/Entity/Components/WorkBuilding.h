#ifndef WORK_BUILDING_H_
#define WORK_BUILDING_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

namespace Logic {
	/**
	Componente que gestiona los trabajadores de un edifcio.
	Los edificios tienen un mínimo y un máximo número de trabajadores.
	Los trabajadores se asignan a los edificios cuando empiezan a viajar hacia ellos. Una vez en el edificio los trabajadores pasan a estar activos.
	*/
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

		/** Gestor de mensajes de trabajadores. Modifica o informa acerca de los trabajadores del edificio */
		virtual bool HandleMessage(const WorkerMessage& msg);

		/**
		Decrementa el número de trabajadores asignados al edificio. Método utilizado únicamente en ciertas
		partes del código en las que se necesita realizar el cambio de forma síncrona. Usar solo cuando sea
		absolutamente imprescindible, para todo lo demás: WorkerMessage.
		*/
		void decrementAssignedWorkers(int numWorkers);

		int getMinWorkers() const {
			return _minWorkers;
		}

		int getMaxWorkers() const {
			return _maxWorkers;
		}

		int getActiveWorkers() const {
			return _activeWorkers;
		}

		int getAssignedWorkers() const {
			return _assignedWorkers;
		}

	protected:
		virtual void CWorkBuilding::defineSkippedRequirements();

	private:
		// Número mínimo de trabajadores para que funcione el edificio
		unsigned int _minWorkers;

		// Número máximo de trabajadores permitido
		unsigned int _maxWorkers;

		// Número de trabajadores activos
		unsigned int _activeWorkers;

		// Número de trabajadores asignados (= activos + los que están de camino)
		unsigned int _assignedWorkers;

		// Cantidad (positiva o negativa) de trabajadores activos pendiente de cambiar
		int _changeActive;

		// Cantidad (positiva o negativa) de trabajadores asignados pendiente de cambiar
		int _changeAssigned;

		// Flag a true cuando se ha recibido un mensaje WORKER_ASK y hay que responder
		bool _askReceived;

	}; // class CWorkBuilding

	REG_FACTORY(CWorkBuilding);

} // namespace Logic

#endif // WORK_BUILDING_H_