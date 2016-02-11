#include "LARoutes.h"

#include "WaypointGraph.h"
#include "Server.h"


namespace AI 
{
	/**
	Método invocado al principio de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al principio (y no durante toda la vida de la acción).
	<p>
	En este caso, se debe enviar un mensaje al componente 
	CRouteTo y cambiar al estado SUSPENDED.

	@return Estado de la función; si se indica que la
	acción a terminado (LatentAction::Completed), se invocará
	al OnStop().
	*/
	CLatentAction::LAStatus CLARouteToRandom::OnStart()
	{	
		// TODO PRÁCTICA IA
		// Al comenzar la ejecución, la acción envía un mensaje al 
		// componente que calcula las rutas. Mientras este componente
		// se ejecuta, la acción se queda suspendida, esperando recibir
		// el mensaje de que ha llegado a destino o no se ha podido 
		// calcular la ruta.

		// 1. Obtenemos el grafo de navegación del server
		AI::CWaypointGraph *wg = AI::CServer::getSingletonPtr()->getNavigationGraph();
		// 2. Sacamos un punto aleatorio del grafo
		int waypoints = wg->getWaypointCount();
		int targetId = rand() % waypoints;
		Vector3 target = wg->getWaypoint(targetId);
		// 3. Enviamos un mensaje al componente que se ocupa de calcular rutas
		sendMoveMessage(true, target);
		// 4. Devolver el estado en el que queda la acción latente (suspendida)
		return SUSPENDED;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLARouteToRandom::OnStop()
	{
		// TODO PRÁCTICA IA
		// Enviamos un mensaje al componente de rutas para que pare.
		// En teoría no debería ser necesario porque si el seguimiento
		// de la ruta termina en condiciones normales el componente sabe
		// que tiene que parar y si no, al llamar a OnAbort se detiene;
		// pero tampoco está de más.
		// En teoría no se debía de llegar a ejecutar, pero por si acaso
		sendMoveMessage();
	}

	/**
	Método invocado cíclicamente para que se continúe con la
	ejecución de la acción.
	<p>
	En este caso no hace nada.

	@return Estado de la acción tras la ejecución del método;
	permite indicar si la acción ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLARouteToRandom::OnRun() {
		// Si el flujo de ejecución llega a entrar en este método
		// significa que hay algo que no va bien. Se supone que 
		// la acción, nada más iniciarse se queda suspendida (por 
		// lo que no se llega a llamar a este método) y cuando 
		// recibe los mensajes pasa directamente a terminar con
		// éxito o con fallo.
		return RUNNING;
	}

	/**
	Envía un mensaje de tipo ROUTE_TO.
	*/
	void CLARouteToRandom::sendMoveMessage(bool walk, Vector3 target)
	{
		// Envía un mensaje para calcular una ruta
		Message message2;
		message2._type = MessageType::ROUTE_TO;
		// El bool indica si el receptor debe calcular la ruta 
		// o debe dejar de recorrer la ruta actual y parar
		message2._bool = walk;
		// Destino de la ruta. Si el parámetro anterior es false
		// este valor no se utiliza
		message2._vector3 = target;
		_entity->emitMessage(message2, NULL);
	}

	/**
	Método invocado cuando la acción ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier razón).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
	de OnStop().
	*/
	CLatentAction::LAStatus CLARouteToRandom::OnAbort() 
	{
		// TODO PRÁCTICA IA
		// Cuando se interrumpe una acción de seguimiento de una ruta
		// hay que enviar un mensaje al componente de seguimiento de rutas
		// para que se detenga si hay una ruta a medioseguir.

		// Enviamos un mensaje al componente de enrutamiento para que pare.
		sendMoveMessage();
		return FAIL;
	}

	/**
	Devuelve true si a la acción le interesa el tipo de mensaje
	enviado como parámetro.
	<p>
	Esta acción acepta mensajes del tipo FAILED_ROUTE y FINISHED_ROUTE

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acción está en principio interesada
	por ese mensaje.
	*/

	bool CLARouteToRandom::accept(const MessageType &message)
	{
		// TODO PRÁCTICA IA
		// Esta acción acepta mensajes del tipo FINISHED_ROUTE

		return (message._type == MessageType::FINISHED_ROUTE);
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.
	<p>
	Si recibe FINISHED_ROUTE la acción finaliza con éxito. Si recibe
	FAILED_ROUTE finaliza con fallo.

	@param msg Mensaje recibido.
	*/
	void CLARouteToRandom::process(const MessageType &message)
	{
		// TODO PRÁCTICA IA
		// Si se recibe un mensaje de fallo de la ruta hay que terminar con fallo.
		// Si es de finalización de la ruta hay que terminar con éxito.
		// Para terminar una acción latente usamos el método finish (ver LatentAction.h)

		if (message._type == MessageType::FINISHED_ROUTE) {
			finish(message._bool);
		}
	}

	
	/**
	Método invocado al principio de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al principio (y no durante toda la vida de la acción).
	<p>
	En este caso, se debe enviar un mensaje al componente 
	CRouteTo y cambiar al estado SUSPENDED.

	@return Estado de la función; si se indica que la
	acción a terminado (LatentAction::Completed), se invocará
	al OnStop().
	*/
	CLatentAction::LAStatus CLARouteTo::OnStart()
	{	
		// TODO PRÁCTICA IA
		// Al comenzar la ejecución, la acción envía un mensaje al 
		// componente que calcula las rutas. Mientras este componente
		// se ejecuta, la acción se queda suspendida, esperando recibir
		// el mensaje de que ha llegado a destino o no se ha podido 
		// calcular la ruta.
		// 1. Enviar un mensaje a la entidad que se ocupa de calcular rutas 
		// con el destino (_target)
		sendMoveMessage(true, _target);
		// 2. Devolver el estado en el que queda la acción latente
		return SUSPENDED;
	}


} //namespace AI 