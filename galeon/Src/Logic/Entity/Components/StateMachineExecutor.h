/**
@file StateMachineExecutor.h

Contiene la declaraci�n de la clase CStateMachineExecutor, que se encarga de
ejecutar m�quinas de estado de la clase CStateMachine.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/
#pragma once

#ifndef __Logic_StateMachineExecutor_H
#define __Logic_StateMachineExecutor_H

#include "Logic/Entity/Component.h"
#include "AI/StateMachine.h"
#include "Logic/Entity/Components/Toggleable.h"

/** Macro para manejar mensajes en una componente ejecutor de SM (.e. hijos de StateMachineExecutor)
Propaga el mensaje adecuadamente para que llegue a la propia SM y a la acci�n actual */
#define SM_EXECUTOR_HANDLE_MESSAGE(Class) \
bool HandleMessage(const Class& msg){ \
	/* Chequeamos si estamos deshabilitados a nivel l�gico */ \
	CToggleable* toggleAble = _entity->getComponent<CToggleable>(); \
\
	/* Si lo estamos, evitamos el tick  */ \
	if (toggleAble != nullptr && !toggleAble->isLogicEnabled(_skippedRequirements)){ \
		return false; \
		} \
\
	if (_currentStateMachine != NULL && _currentStateMachine->HandleMessage(msg)) \
		return true; \
	if (_currentAction != NULL) \
		return _currentAction->HandleMessage(msg); \
	return false; \
}

namespace Logic
{
	/**
	Componente que se encarga de ejecutar m�quinas de estado.
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
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity).

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
		fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
		{
			if (!IComponent::spawn(entity, map, entityInfo))
				return false;

			// Creamos la instancia de la m�quina de estados
			_currentStateMachine = getStateMachine();
			assert(_currentStateMachine && "StateMachine created");

			// La spawneamos con los atributos que necesite
			return _currentStateMachine->spawn(entity, map, entityInfo);
		}

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Este m�todo actualiza la m�quina de estado. Si hay un cambio de estado,
		se actualiza el valor del atributo _currentAction, que es el que contiene
		la acci�n latente que se est� ejecutando. Por �ltimo, se llama al tick de
		la acci�n latente para que avance su ejecuci�n.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs)
		{
			// Chequeamos si estamos deshabilitados a nivel l�gico
			CToggleable* toggleAble = _entity->getComponent<CToggleable>();
			
			// Si lo estamos, evitamos el tick
			if (toggleAble != nullptr && !toggleAble->isLogicEnabled(_skippedRequirements)){
				return;
			}

			IComponent::tick(msecs);

			// En cada tick del ejecutor de m�quinas de estado tenemos que 
			// realizar 2 acciones principales:
			// 1. Actualizar la m�quina de estado.
			// 1.1. Si la m�quina cambia de estado reseteamos la acci�n que 
			//		est�bamos ejecutando hasta ahora para que se pueda volver
			//		a ejecutar y cambiamos la acci�n latente actual por la del 
			//		nuevo estado.
			if (_currentStateMachine != NULL) {
				// Si cambiamos de nodo
				if (_currentStateMachine->nextState()) {
					// Dejamos la acci�n anterior lista para que pueda
					// volver a ser ejecutada
					if (_currentAction != NULL)
						_currentAction->reset();

					// Sacamos la nueva acci�n
					_currentAction = _currentStateMachine->getCurrentNode();
				}
			}
			// 2. Ejecutar la acci�n latente correspondiente al estado actual
			if (_currentAction != NULL) {
				_currentAction->tick(msecs);
			}
		}

		/** Desactiva la SM y la acci�n actual */
		virtual void deactivate() {
			if (_currentStateMachine != NULL)
				_currentStateMachine->deactivate();
			if (_currentAction != NULL)
				_currentAction->deactivate();
		}
		
		SM_EXECUTOR_HANDLE_MESSAGE(WalkSoulPathMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(HellQuartersMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(SoulSenderMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(SoulMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(NumberMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(LogisticsMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(ResourceMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(PowerMessage);

		SM_EXECUTOR_HANDLE_MESSAGE(ConsumptionMessage);

	protected:
		/**
		Almacena la m�quina de estados que se est� ejecutando
		*/
		AI::CStateMachine<AI::CLatentAction, SharedData>* _currentStateMachine;

		/**
		Acci�n que se est� ejecutando.
		*/
		AI::CLatentAction* _currentAction;

		/** Instancia la m�quina de estados. Debe ser implementado por la subclase */
		virtual AI::CStateMachine<AI::CLatentAction, SharedData>* getStateMachine() = 0;

		/** 
		Sobreescribimos los requisitos a evitar. Por defecto no ignoraremos NING�N requisito ya que las SMs normalmente son L�gica.
		Si alg�n SMExecutor quiere evitar un requisito en concreto deber� especificarlo.
		*/
		virtual void defineSkippedRequirements(){
			_skippedRequirements.clear();
		}

	}; // class CStateMachineExecutor 

}// namespace Logic

#endif // __Logic_StateMachineExecutor_H
