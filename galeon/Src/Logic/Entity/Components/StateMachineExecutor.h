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

namespace Logic
{
	/**
	Componente que se encarga de ejecutar m�quinas de estado.
	*/
	class CStateMachineExecutor : public IComponent
	{
		DEC_FACTORY(CStateMachineExecutor);
	
	public:
		/**
		Constructor
		*/
		CStateMachineExecutor(void) : _currentStateMachine(0), _currentAction(0) {};

		/**
		Destructor
		*/
		~CStateMachineExecutor(void) { 
			if (_currentStateMachine != 0) delete _currentStateMachine;
		};

		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		behavior, que indica el nombre de la m�quina de estado a ejecutar.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Este m�todo actualiza la m�quina de estado. Si hay un cambio de estado, 
		se actualiza el valor del atributo _currentAction, que es el que contiene
		la acci�n latente que se est� ejecutando. Por �ltimo, se llama al tick de
		la acci�n latente para que avance su ejecuci�n.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);
			
		virtual bool HandleMessage(const WalkSoulPathMessage& msg);

	protected:
		/**
		Almacena la m�quina de estado que se est� ejecutando
		*/
		// AI::CStateMachine<int>* _currentStateMachine;
		AI::CStateMachine<AI::CLatentAction> * _currentStateMachine;

		/**
		Acci�n que se est� ejecutando.
		*/
		AI::CLatentAction* _currentAction;

		/** Instancia la m�quina de estados. Debe ser implementado por la clase hija */
		virtual AI::CStateMachine<AI::CLatentAction> *getStateMachine();

	}; // class CStateMachineExecutor 

	REG_FACTORY(CStateMachineExecutor);

}// namespace Logic

#endif // __Logic_StateMachineExecutor_H
