#include "LAExecuteSM.h"

#include "StateMachine.h"

namespace AI 
{

		/**
	Método invocado al principio de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al principio (y no durante toda la vida de la acción).
	<p>
	Al comenzar se obtiene el tiempo actual y se calcula el 
	tiempo en el que terminará la acción mediante el atributo _time

	@return Estado de la función; si se indica que la
	acción a terminado (LatentAction::Completed), se invocará
	al OnStop().
	*/
	CLatentAction::LAStatus CLAExecuteSM::OnStart()
	{
		// TODO PRÁCTICA IA
		// Al comenzar reiniciamos la máquina de estado para asegurarnos que estamos en el estado inicial
		
		_stateMachine->resetExecution();		
		return RUNNING; 
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLAExecuteSM::OnStop()
	{
	}

	/**
	Método invocado cíclicamente para que se continúe con la
	ejecución de la acción.
	<p>
	En cada paso de ejecución se obtiene el tiempo actual 
	y se comprueba si se debe acabar la acción.

	@return Estado de la acción tras la ejecución del método;
	permite indicar si la acción ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLAExecuteSM::OnRun() 
	{
		// TODO PRÁCTICA IA
		// En cada tick hay que ejecutar la máquina de estado
		// Para eso llamamos primero al método nextState para 
		// comprobar si hay transiciones. Si es así, reseteamos
		// la acción actual para cuando se vuelva a ejecutar y 
		// la actualizamos llamando a getCurrentNode
		// Independientemente de si ha habido transición, ejecutamos
		// la acción actual (_currentAction) si tenemos alguna.
		// Para ello, llamamos a su método tick()
		// Por último, el método siempre devuelve RUNNING.
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
	Método invocado cuando la acción ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier razón).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
	de OnStop().
	*/
	CLatentAction::LAStatus CLAExecuteSM::OnAbort() 
	{
		_stateMachine->resetExecution(); // Redundante
		return FAIL;
	}
	/**
	Devuelve true si a la acción le interesa el tipo de mensaje
	enviado como parámetro.
	<p>
	Esta acción no acepta mensajes de ningún tipo.

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acción está en principio interesada
	por ese mensaje.
	*/
	bool CLAExecuteSM::accept(const MessageType &message)
	{
		// TODO PRÁCTICA IA
		// Para saber si un mensaje acepta, debemos delegar por 
		// un lado en la máquina de estado (_stateMachine) y por otro
		// en la acción actual (_currentAction)
		if (_stateMachine != NULL && _stateMachine->accept(message))
			return true;
		if (_currentAction != NULL)
			return _currentAction->accept(message);
		return false;
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAExecuteSM::process(const MessageType &message)
	{
		// TODO PRÁCTICA IA
		// Igual que en accept, la responsabilidad de procesar
		// los mensajes se delega por un lado en la máquina de
		// de estado que se está ejecutando (porque algunas condiciones
		// dependen del paso de mensajes) y por otro en la acción 
		// actual (algunas acciones latentes también dependen de
		// los mensajes)
		if (_stateMachine != NULL)
			_stateMachine->process(message);

		if (_currentAction != NULL)
			_currentAction->process(message);
	}


} // namespace AI 
