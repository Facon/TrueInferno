#include "LatentAction.h"

namespace AI 
{
	/**
	M�todo llamado c�clicamente por el responsable de la ejecuci�n
	de la funci�n latente.
	@return Devuelve el estado de la funci�n latente: si �sta ha
	terminado su ejecuci�n, si necesita que se siga invocando a
	la funci�n Tick() c�clicamente o si, a�n sin haber terminado,
	no necesita (de momento) la invocaci�n a Tick().
	*/
	CLatentAction::LAStatus CLatentAction::tick() 
	{

		// �Hay que empezar la tarea?
		if (_status == READY) {
			_status = this->OnStart();
		}

		// Llamamos al Tick, si el OnStart no termin�
		// con la ejecuci�n
		if ((_status == RUNNING)) {
			_status = this->OnRun();
			// Si hemos pasado de RUNNING a un estado de finalizaci�n (SUCCESS o FAIL) 
			// a�n tenemos que parar
			if (_status == SUCCESS || _status == FAIL)
				_stopping = true;
		}

		// Si OnRun() termin�, llamamos al OnStop() y terminamos;
		// si est�bamos terminando (se solicit� la terminaci�n
		// de forma as�ncrona), tambi�n.
		if ((_status == SUCCESS || _status == FAIL) && (_stopping)) {
			// Paramos y decimos que ya no hay que volver a ejecutar OnStop
			this->OnStop();
			_stopping = false;
		}
		return _status;
	} // tick

	/**
	Cancela la tarea que se est� ejecutando; se entiende que este
	m�todo es llamado cuando el comportamiento al que pertenece
	la tarea es anulado.
	*/
	void CLatentAction::abort() {
		_stopping = false;
		_status = this->OnAbort();
	}

	/**
	Reinicia la acci�n, que queda en un estado listo para ser ejecutada
	de nuevo (READY). Dependiendo del estado de la acci�n en el momento
	de llamar a reset (si �ste es RUNNING o SUSPENDED) se considera que 
	la acci�n ha sido abortada y se llama a OnAbort.
	*/
	void CLatentAction::reset() {
		// S�lo hacemos algo si ya hemos empezado a ejecutar la acci�n
		if (_status != READY) {
			// Si estamos en ejecuci�n (normal o suspendida) 
			// tenemos que llamar a onAbort (porque en realidad
			// abortamos la acci�n)
			if (_status == RUNNING || _status == SUSPENDED)
				this->OnAbort();
			// Si hemos terminado la ejecuci�n pero no hemos parado (OnStop) tenemos que 
			// llamar a OnStop
			if ((_status == SUCCESS || _status == FAIL) && (_stopping)) 
				this->OnStop();
			// Dejamos el estado listo para volver a ejecutarla
			_status = READY;
			_stopping = false;
		}
	}

	/**
	Solicita la finalizaci�n de la acci�n en el siguiente tick, 
	estableciendo el estado	a SUCCESS o FAIL seg�n el valor del
	par�metro de entrada.
	*/
	void CLatentAction::finish(bool success) { 
		_status = (success ? SUCCESS : FAIL);
		_stopping = true;
	}


}// namespace AI 