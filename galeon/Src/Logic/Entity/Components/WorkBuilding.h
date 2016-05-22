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

		/** Gestor de mensajes de trabajadores. Modifica la cantidad de trabajadores presentes en el edificio */
		virtual bool HandleMessage(const WorkerMessage& msg);

		/**
		Métodos para incrementar y decrementar el número de trabajadores asignados al edificio.
		Necesarios en ciertas partes del código en las que se necesita realizar el cambio de
		forma síncrona (como los algoritmos implicados en el Juicio de Almas y la reordenación
		de trabajadores, que no pueden esperar al tick o la entrega de mensajes).
		<p>
		Usar solo cuando sea absolutamente imprescindible, para todo lo demás: WorkerMessage.
		*/
		void increaseAssignedWorkers(int numWorkers);
		void decreaseAssignedWorkers(int numWorkers);

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

		int getVirtualAssignedWorkers() const {
			return _assignedWorkers + _changeAssigned;
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

		// Número de trabajadores asignados (= activos + los que están de camino + los
		// que están en espera de ser enviados al procesar su tarea)
		unsigned int _assignedWorkers;

		// Cantidad (positiva o negativa) de trabajadores activos pendiente de cambiar
		int _changeActive;

		// Cantidad (positiva o negativa) de trabajadores asignados pendiente de cambiar
		int _changeAssigned;

	}; // class CWorkBuilding

	REG_FACTORY(CWorkBuilding);

} // namespace Logic

#endif // WORK_BUILDING_H_