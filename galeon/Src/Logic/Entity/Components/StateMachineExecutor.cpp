#include "StateMachineExecutor.h"
#include "Map/MapEntity.h"

namespace Logic 
{

	//---------------------------------------------------------

	//IMP_FACTORY(CStateMachineExecutor);
	RTTI_ROOT_IMPL(CStateMachineExecutor);

	//---------------------------------------------------------

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
	bool CStateMachineExecutor::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// Obtiene el nombre de la m�quina de estados
		/*if(entityInfo->hasAttribute("behavior")) {
			std::string smName = entityInfo->getStringAttribute("behavior");
			if (_currentStateMachine != 0) delete _currentStateMachine;

			// Saca una instancia de la m�quina de estado de la factor�a
			_currentStateMachine = AI::CStateMachineFactory::getStateMachine(smName, entity);

			// Creamos la instancia de m�quina de estados
			_currentStateMachine = getStateMachine();
		}*/

		// Creamos la instancia de m�quina de estados
		_currentStateMachine = getStateMachine();

		return true;
	}

	//---------------------------------------------------------

	/**
	M�todo llamado en cada frame que actualiza el estado del componente.
	<p>
	Este m�todo actualiza la m�quina de estado. Si hay un cambio de estado, 
	se actualiza el valor del atributo _currentAction, que es el que contiene
	la acci�n latente que se est� ejecutando. Por �ltimo, se llama al tick de
	la acci�n latente para que avance su ejecuci�n.

	@param msecs Milisegundos transcurridos desde el �ltimo tick.
	*/
	void CStateMachineExecutor::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		// TODO PR�CTICA IA
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
			_currentAction->tick();
		}
	}

	//---------------------------------------------------------

	/**
	Este m�todo delega en el m�todo HandleMessage de la acci�n latente que se
	est� ejecutando (_currentAction).
	*/
	bool CStateMachineExecutor::HandleMessage(const WalkSoulPathMessage& msg)
	{
		if (_currentStateMachine != NULL && _currentStateMachine->HandleMessage(msg))
			return true;
		if (_currentAction != NULL)
			return _currentAction->HandleMessage(msg);
		return false;
	}

	bool CStateMachineExecutor::HandleMessage(const HellQuartersMessage& msg)
	{
		if (_currentStateMachine != NULL && _currentStateMachine->HandleMessage(msg))
			return true;
		if (_currentAction != NULL)
			return _currentAction->HandleMessage(msg);
		return false;
	}

	bool CStateMachineExecutor::HandleMessage(const SoulSenderMessage& msg)
	{
		if (_currentStateMachine != NULL && _currentStateMachine->HandleMessage(msg))
			return true;
		if (_currentAction != NULL)
			return _currentAction->HandleMessage(msg);
		return false;
	}

	//---------------------------------------------------------

	/**
	Este m�todo delega en el m�todo accept de la acci�n latente que se 
	est� ejecutando (_currentAction).
	*/
	/*bool CStateMachineExecutor::accept(const TMessage &message)
	{
		// TODO PR�CTICA IA
		// El m�todo accept delega en el m�todo accept de la acci�n latente actual
		if (_currentStateMachine != NULL && _currentStateMachine->accept(message))
			return true;
		if (_currentAction != NULL)
			return _currentAction->accept(message);
		return false;
	}*/

	//---------------------------------------------------------

	/**
	Este m�todo delega en el m�todo process de la acci�n latente que se 
	est� ejecutando (_currentAction).
	*/
	/*void CStateMachineExecutor::process(const TMessage &message)
	{
		// TODO PR�CTICA IA
		// El m�todo process delega en el m�todo process de la acci�n latente actual
		if (_currentStateMachine != NULL)
			_currentStateMachine->process(message);
		if (_currentAction != NULL) 
			_currentAction->process(message);
	}*/

	//---------------------------------------------------------

}