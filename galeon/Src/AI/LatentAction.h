/**
@file CLatentAction.h

Clase que implementa las acciones latentes

@author Marco Antonio G�mez Mart�n
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_LatentAction_H
#define __AI_LatentAction_H

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"

using namespace Logic;

namespace AI 
{

	/**
	Clase base que representa una acci�n latente del sistema;
	esta clase est� pensada para ser extendida con clases hijas
	que son las que implementan las distintas acciones latentes
	del sistema.

	La existencia de esta clase permite un tratamiento general de
	todas ellas.

	Las acciones latentes despu�s ser�n ejecutadas por una entidad
	externa (por ejemplo dentro de un �rbol de comportamiento, o
	como funci�n latente de un lenguaje de script).

	El responsable de esa ejecuci�n simplemente llamar� al m�todo
	Tick() hasta que la acci�n latente termine. El Tick() es un
	template method, de forma que las clases hija deben (o mejor, pueden)
	implementar los siguientes m�todos:

	- OnStart(): invocado cuando la acci�n latente comienza su
	ejecuci�n.

	- OnRun(): invocado c�clicamente para que la acci�n latente
	contin�e su ejecuci�n.

	- OnStop(): cuando OnRun() notifica que la acci�n ha terminado,
	se invoca a este m�todo por si se quieren liberar recursos.

	- OnAbort(): este m�todo es llamado �nicamente en situaciones
	especiales; en concreto cuando el comportamiento al que pertenece
	la acci�n latente es cancelado. Permite a la acci�n latente
	liberar los recursos que considere oportunos, desengancharse
	de observadores, etc.

	Una acci�n latente se encuentra siempre en uno de estos estados:

	- READY: la acci�n latente est� lista para empezar a ser ejecutada.

	- RUNNING: la acci�n latente est� ejecut�ndose.

	- SUSPENDED: la acci�n latente est� suspendida a la espera de 
	alg�n evento externo y no necesita ciclos de CPU.

	- SUCCESS: la acci�n latente ha finalizado con �xito.

	- FAIL: la acci�n latente ha finalizado con fallo.

	En la mayor�a de los casos la acci�n latente necesitar� conocer
	cu�l es la CEntity ejecutable de su ejecuci�n. Para ello hay que 
	llamar al m�todo setEntity

	@author Marco Antonio G�mez Mart�n
	*/
	class CLatentAction : public MessageHandler
	{
	public:
		enum LAStatus {
			/// Indica que la acci�n latente a�n no se ha empezado
			/// a ejecutar
			READY,
			/// Indica que la acci�n latente est� ejecut�ndose,
			/// y por lo tanto necesita ciclos de CPU
			RUNNING,
			/// Indica que la acci�n latente est� suspendida a
			/// la espera de alg�n evento externo, y por tanto no
			/// necesita (al menos por el momento) ciclos de CPU.
			SUSPENDED,
			/// Indica que la acci�n latente ha terminado su
			/// tarea con �xito.
			SUCCESS,
			/// Indica que la acci�n latente ha terminado su
			/// tarea con fallo.
			FAIL
		};
		
		/**
		Constructor
		*/
		CLatentAction() : _status(READY), _stopping(false) {};

		CLatentAction(CEntity* entity) : _entity(entity), _status(READY), _stopping(false) {};

		/**
		Establece la entidad que ejecuta la acci�n.
		*/
		void setEntity(CEntity* entity) { _entity = entity; };

		/**
		M�todo llamado c�clicamente por el responsable de la ejecuci�n
		de la acci�n latente.
		@return Devuelve el estado de la funci�n latente: si �sta ha
		terminado su ejecuci�n, si necesita que se siga invocando a
		la funci�n Tick() c�clicamente o si, a�n sin haber terminado,
		no necesita (de momento) la invocaci�n a Tick().
		*/
		LAStatus tick();
		/**
		Cancela la tarea que se est� ejecutando; se entiende que este
		m�todo es llamado cuando el comportamiento al que pertenece
		la tarea es anulado.
		*/
		void abort();
		/**
		Reinicia la acci�n, que queda en un estado listo para ser ejecutada
		de nuevo (READY). Dependiendo del estado de la acci�n en el momento
		de llamar a reset (si �ste es RUNNING o SUSPENDED) se considera que 
		la acci�n ha sido abortada y se llama a OnAbort.
		*/
		void reset();
		/**
		Devuelve true si a la acci�n le interesa el tipo de mensaje
		enviado como par�metro.
		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		//virtual bool accept(const TMessage &message) = 0;
		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		//virtual void process(const TMessage &message) = 0;
		/**
		Devuelve el estado actual de la acci�n.
		*/
		LAStatus getStatus() {return _status; };

	private:
		/**
		Estado de la acci�n
		*/
		LAStatus _status;

	protected:
		/**
		Entidad que ejecuta la acci�n
		*/
		CEntity *_entity;
		/**
		Se pone a cierto cuando se solicita la finalizaci�n de la acci�n.
		*/
		bool _stopping;

		/**
		M�todo invocado al principio de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al principio (y no durante toda la vida de la acci�n).
		@return Estado de la al que pasa la acci�n; si se indica que la
		acci�n ha terminado (LatentAction::Completed), se invocar�
		al OnStop().
		*/
		virtual LAStatus OnStart() { return READY;}

		/**
		M�todo invocado al final de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al final (y no durante toda la vida de la acci�n).

		En la mayor�a de los casos este m�todo no hace nada.
		*/
		virtual void OnStop() {}

		/**
		M�todo invocado c�clicamente para que se contin�e con la
		ejecuci�n de la acci�n.

		@return Estado de la acci�n tras la ejecuci�n del m�todo;
		permite indicar si la acci�n ha terminado o se ha suspendido,
		o si sigue en ejecuci�n.
		*/
		virtual LAStatus OnRun() { return SUCCESS;}

		/**
		M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
		al que pertenece se ha abortado por cualquier raz�n).

		La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada".
		
		@return Estado de la acci�n tras la ejecuci�n del m�todo.

		@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
		de OnStop().
		*/
		virtual LAStatus OnAbort() { return FAIL; }
		/**
		Solicita la finalizaci�n de la acci�n en el siguiente tick, 
		estableciendo el estado	a SUCCESS o FAIL seg�n el valor del
		par�metro de entrada.
		*/
		virtual void finish(bool success);
		/**
		Contin�a la ejecuci�n de la acci�n cuando esta se encuentra suspendida
		Cuidado: �nicamente realiza el cambio de estado de SUSPENDED a RUNNING.
		Si el estado original es otro, no tiene efecto
		*/
		void resume() {if (_status == SUSPENDED) _status = RUNNING; };


	}; // class CLatentAction	


} // namespace AI 

#endif // __AI_LatentAction_H
