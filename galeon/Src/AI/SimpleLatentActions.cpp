#include "SimpleLatentActions.h"

#include "Application/BaseApplication.h"

#include "StateMachine.h"

namespace AI 
{


//////////////////////////////
//	Implementaci�n de CLAWait
//////////////////////////////

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
	CLatentAction::LAStatus CLAWait::OnStart()
	{
		// TODO PR�CTICA IA
		// Al iniciar el wait, calculamos el instante de tiempo en 
		// el que se tiene que finalizar la acci�n. 
		// El tiempo de espera se guarda en _time.
		// Para obtener el instante actual podemos usar 
		// Application::CBaseApplication::getSingletonPtr()->getAppTime()
		//
		// Otra opci�n es usar
		// BaseSubsystems::CServer::getSingletonPtr()->getAppTimer()
		// para sacar el timer de la aplicaci�n y usar su m�todo 
		// getMilliseconds.

		unsigned int currentTime = Application::CBaseApplication::getSingletonPtr()->getAppTime();
		_endingTime = currentTime + _time;
		return RUNNING; 
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLAWait::OnStop()
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
	CLatentAction::LAStatus CLAWait::OnRun() 
	{
		// TODO PR�CTICA IA
		// En cada paso de ejecuci�n tendremos que comprobar si hemos
		// superado el tiempo de espera. Seg�n lo hayamos superado o no,
		// la acci�n tendr� que pasar a un estado de terminado con �xito o
		// quedarse en el mismo estado en ejecuci�n.

		unsigned int currentTime = Application::CBaseApplication::getSingletonPtr()->getAppTime();
		return (currentTime > _endingTime) ? SUCCESS : RUNNING;
	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLAWait::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
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
	/*bool CLAWait::accept(const TMessage &message)
	{
		// TODO PR�CTICA IA
		// La acci�n no acepta mensajes
		return false;
	}*/
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	/*void CLAWait::process(const TMessage &message)
	{
		// TODO PR�CTICA IA
		// La acci�n no procesa mensajes
	}*/


} //namespace AI 