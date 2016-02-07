/**
@file CSimpleLatentActions.h

En este fichero se implementan algunas acciones 
latentes b�sicas.

@author Gonzalo Fl�rez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_SimpleLatentActions_H
#define __AI_SimpleLatentActions_H

#include "LatentAction.h"

#include "Logic/Entity/Entity.h"

using namespace Logic;

namespace AI 
{

	/**
	Esta acci�n espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLAWait : public CLatentAction
	{
	public:
		/**
		Constructor.
		
		@param time Tiempo de espera
		*/
		CLAWait(unsigned long time) : CLatentAction(), _time(time) {};
		/**
		Destructor
		*/
		~CLAWait() {};

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
		virtual LAStatus OnStart();

		/**
		M�todo invocado al final de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al final (y no durante toda la vida de la acci�n).

		En la mayor�a de los casos este m�todo no hace nada.
		*/
		virtual void OnStop();

		/**
		M�todo invocado c�clicamente para que se contin�e con la
		ejecuci�n de la acci�n.
		<p>
		En cada paso de ejecuci�n se obtiene el tiempo actual 
		y se comprueba si se debe acabar la acci�n.

		@return Estado de la acci�n tras la ejecuci�n del m�todo;
		permite indicar si la acci�n ha terminado o se ha suspendido.
		*/
		virtual LAStatus OnRun() ;

		/**
		M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
		al que pertenece se ha abortado por cualquier raz�n).

		La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada".

		@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
		de OnStop().
		*/
		virtual LAStatus OnAbort() ;
		/**
		Devuelve true si a la acci�n le interesa el tipo de mensaje
		enviado como par�metro.
		<p>
		Esta acci�n no acepta mensajes de ning�n tipo.

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool accept(const MessageType &message);
		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void process(const MessageType &message);

	protected:
		/**
		Tiempo de espera
		*/
		unsigned long _time;
		/**
		Tiempo en el que se termina la acci�n
		*/
		unsigned long _endingTime;
	};

} //namespace AI 

#endif // __AI_SimpleLatentActions_H