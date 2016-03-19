/**
@file StateMachineExecutor.h

Contiene la declaración de la clase CStateMachineExecutor, que se encarga de
ejecutar máquinas de estado de la clase CStateMachine.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __Logic_StateMachineExecutor_H
#define __Logic_StateMachineExecutor_H

#include "Logic/Entity/Component.h"
#include "AI/StateMachine.h"

namespace Logic
{
	/**
	Componente que se encarga de ejecutar máquinas de estado.
	*/
	template <class SharedData>
	class CStateMachineExecutor : public IComponent
	{
	public:
		/**
		Constructor
		*/
		CStateMachineExecutor() : _currentStateMachine(0), _currentAction(0) {};

		/**
		Destructor
		*/
		virtual ~CStateMachineExecutor() { 
			if (_currentStateMachine != 0) delete _currentStateMachine;
		};

		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity).

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
		fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
		{
			if (!IComponent::spawn(entity, map, entityInfo))
				return false;

			// Creamos la instancia de la máquina de estados
			_currentStateMachine = getStateMachine();
			assert(_currentStateMachine && "StateMachine created");

			// La spawneamos con los atributos que necesite
			return _currentStateMachine->spawn(entity, map, entityInfo);
		}

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Este método actualiza la máquina de estado. Si hay un cambio de estado,
		se actualiza el valor del atributo _currentAction, que es el que contiene
		la acción latente que se está ejecutando. Por último, se llama al tick de
		la acción latente para que avance su ejecución.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs)
		{
			IComponent::tick(msecs);

			// En cada tick del ejecutor de máquinas de estado tenemos que 
			// realizar 2 acciones principales:
			// 1. Actualizar la máquina de estado.
			// 1.1. Si la máquina cambia de estado reseteamos la acción que 
			//		estábamos ejecutando hasta ahora para que se pueda volver
			//		a ejecutar y cambiamos la acción latente actual por la del 
			//		nuevo estado.
			if (_currentStateMachine != NULL) {
				// Si cambiamos de nodo
				if (_currentStateMachine->nextState()) {
					// Dejamos la acción anterior lista para que pueda
					// volver a ser ejecutada
					if (_currentAction != NULL)
						_currentAction->reset();

					// Sacamos la nueva acción
					_currentAction = _currentStateMachine->getCurrentNode();
				}
			}
			// 2. Ejecutar la acción latente correspondiente al estado actual
			if (_currentAction != NULL) {
				_currentAction->tick(msecs);
			}
		}
		
		SM_EXECUTOR_HANDLE_MESSAGE(WalkSoulPathMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(HellQuartersMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(SoulSenderMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(SoulMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(NumberMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(LogisticsMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(ResourceMessage);

	protected:
		/**
		Almacena la máquina de estado que se está ejecutando
		*/
		AI::CStateMachine<AI::CLatentAction, SharedData>* _currentStateMachine;

		/**
		Acción que se está ejecutando.
		*/
		AI::CLatentAction* _currentAction;

		/** Instancia la máquina de estados. Debe ser implementado por la subclase */
		virtual AI::CStateMachine<AI::CLatentAction, SharedData>* getStateMachine() = 0;

		/** Inicializa los datos compartidos por la SM. Debe ser implementado por la subclase */
		//virtual SharedData* getStateMachine() = 0;

	}; // class CStateMachineExecutor 

}// namespace Logic

#endif // __Logic_StateMachineExecutor_H
