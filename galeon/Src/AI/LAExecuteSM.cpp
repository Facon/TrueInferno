#include "LAExecuteSM.h"

#include "StateMachine.h"

namespace AI 
{

		/**
	M�todo invocado al principio de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al principio (y no durante toda la vida de la acci�n).
	<p>
	Al comenzar se obtiene el tiempo actual y se calcula el 
	tiempo en el que terminar� la acci�n mediante el atributo _time

	@return Estado de la funci�n; si se indica que la
	acci�n a terminado (LatentAction::Completed), se invocar�
	al OnStop().
	*/
	CLatentAction::LAStatus CLAExecuteSM::OnStart()
	{
		// TODO PR�CTICA IA
		// Al comenzar reiniciamos la m�quina de estado para asegurarnos que estamos en el estado inicial
		
		_stateMachine->resetExecution();		
		return RUNNING; 
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLAExecuteSM::OnStop()
	{
	}

	/**
	M�todo invocado c�clicamente para que se contin�e con la
	ejecuci�n de la acci�n.
	<p>
	En cada paso de ejecuci�n se obtiene el tiempo actual 
	y se comprueba si se debe acabar la acci�n.

	@return Estado de la acci�n tras la ejecuci�n del m�todo;
	permite indicar si la acci�n ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLAExecuteSM::OnRun() 
	{
		// TODO PR�CTICA IA
		// En cada tick hay que ejecutar la m�quina de estado
		// Para eso llamamos primero al m�todo nextState para 
		// comprobar si hay transiciones. Si es as�, reseteamos
		// la acci�n actual para cuando se vuelva a ejecutar y 
		// la actualizamos llamando a getCurrentNode
		// Independientemente de si ha habido transici�n, ejecutamos
		// la acci�n actual (_currentAction) si tenemos alguna.
		// Para ello, llamamos a su m�todo tick()
		// Por �ltimo, el m�todo siempre devuelve RUNNING.
		if (_stateMachine->nextState()) {
			if (_currentAction != NULL)
				_currentAction->reset();
			_currentAction = _stateMachine->getCurrentNode();
		}
		if (_currentAction != NULL)
			_currentAction->tick();
		return RUNNING;
	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLAExecuteSM::OnAbort() 
	{
		_stateMachine->resetExecution(); // Redundante
		return FAIL;
	}
	/**
	Devuelve true si a la acci�n le interesa el tipo de mensaje
	enviado como par�metro.
	<p>
	Esta acci�n no acepta mensajes de ning�n tipo.

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acci�n est� en principio interesada
	por ese mensaje.
	*/
	bool CLAExecuteSM::accept(const MessageType &message)
	{
		// TODO PR�CTICA IA
		// Para saber si un mensaje acepta, debemos delegar por 
		// un lado en la m�quina de estado (_stateMachine) y por otro
		// en la acci�n actual (_currentAction)
		if (_stateMachine != NULL && _stateMachine->accept(message))
			return true;
		if (_currentAction != NULL)
			return _currentAction->accept(message);
		return false;
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAExecuteSM::process(const MessageType &message)
	{
		// TODO PR�CTICA IA
		// Igual que en accept, la responsabilidad de procesar
		// los mensajes se delega por un lado en la m�quina de
		// de estado que se est� ejecutando (porque algunas condiciones
		// dependen del paso de mensajes) y por otro en la acci�n 
		// actual (algunas acciones latentes tambi�n dependen de
		// los mensajes)
		if (_stateMachine != NULL)
			_stateMachine->process(message);

		if (_currentAction != NULL)
			_currentAction->process(message);
	}


} // namespace AI 
