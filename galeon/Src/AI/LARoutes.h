/**
@file LARoutes.h

En este fichero se implementan algunas acciones 
latentes relacionadas con el seguimiento de rutas.

@author Gonzalo Fl�rez
@date Enero 2013

*/

#pragma once
#ifndef __AI_LARoutes_H
#define __AI_LARoutes_H

#include "LatentAction.h"

#include "Logic/Entity/Entity.h"

namespace AI 
{

	/**
	Acci�n latente que sigue una ruta desde el punto actual
	hasta un punto aleatorio del mapa de Galeon.
	<p>
	Utiliza el componente CRouteTo para calcular la ruta y 
	moverse.
	<p>
	Acepta mensajes de tipo FAILED_ROUTE y FINISHED_ROUTE, 
	que indican si debe terminar con fallo o con �xito.
	*/
	class CLARouteToRandom : public CLatentAction
	{
	public:
		/**
		Constructor.
		*/
		CLARouteToRandom(CEntity* entity) : CLatentAction() {this->setEntity(entity);};
		/**
		Destructor.
		*/
		~CLARouteToRandom() {};

		/**
		Devuelve true si a la acci�n le interesa el tipo de mensaje
		enviado como par�metro.
		<p>
		Esta acci�n acepta mensajes del tipo FAILED_ROUTE y FINISHED_ROUTE

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool accept(const MessageType &message);
		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.
		<p>
		Si recibe FINISHED_ROUTE la acci�n finaliza con �xito. Si recibe
		FAILED_ROUTE finaliza con fallo.

		@param msg Mensaje recibido.
		*/
		virtual void process(const MessageType &message);

	protected:
		/**
		M�todo invocado al principio de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al principio (y no durante toda la vida de la acci�n).
		<p>
		En este caso, se debe enviar un mensaje al componente 
		CRouteTo y cambiar al estado SUSPENDED.

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
		En este caso no hace nada.

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
		Env�a un mensaje de tipo ROUTE_TO.
		*/
		void sendMoveMessage(bool walk = false, Vector3 target = Vector3::ZERO);

	};

	/**
	Acci�n latente que sigue una ruta desde el punto actual
	hasta un punto aleatorio del mapa de Galeon.
	<p>
	Utiliza el componente CRouteTo para calcular la ruta y 
	moverse.
	<p>
	Acepta mensajes de tipo FAILED_ROUTE y FINISHED_ROUTE, 
	que indican si debe terminar con fallo o con �xito.
	*/
	class CLARouteTo : public CLARouteToRandom
	{
	private:
		Vector3 _target;
	public:
		/**
		Constructor.
		*/
		CLARouteTo(CEntity* entity, Vector3 &target) : CLARouteToRandom(entity), _target(target) {};
		/**
		Destructor.
		*/
		~CLARouteTo() {};

	protected:
		/**
		M�todo invocado al principio de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al principio (y no durante toda la vida de la acci�n).
		<p>
		En este caso, se debe enviar un mensaje al componente 
		CRouteTo y cambiar al estado SUSPENDED.

		@return Estado de la funci�n; si se indica que la
		acci�n a terminado (LatentAction::Completed), se invocar�
		al OnStop().
		*/
		virtual LAStatus OnStart();

	};


} // namespace AI

#endif // __AI_LARoutes_H