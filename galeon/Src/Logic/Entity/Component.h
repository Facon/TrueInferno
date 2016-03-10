/**
@file Component.h

Contiene la declaraci�n de la clase base de los componentes.

@see Logic::IComponent
@see Logic::CCommunicationPort

@author David Llansó
@date Julio, 2010
*/
#ifndef __Logic_Component_H
#define __Logic_Component_H

#include "BaseSubsystems/RTTI.h"
#include "Logic/Maps/ComponentFactory.h"
#include "Logic/Entity/MessageHandler.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class CEntity;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Clase base de los componentes que forman las entidades.
	<p>
	Un componente puede recibir mensajes y reaccionar ante ellos
	enviando otros mensajes al resto de componentes hermanos o
	realizando alguna acci�n sobre el entorno.
	<p>
	Para procesar los mensajes y para realizar cualquier otro
	comportamiento, la entidad a la que el componente pertenece
	invoca peri�dicamente a la funci�n tick() del componente. 
	La implementaci�n por defecto de ese m�todo simplemente procesa todos
	los mensajes pendientes recibidos por el componente.
	<p>
	Si las clases hija sobreescriben este m�todo, <em>son responsables</em>
	de invocar al m�todo de procesado de mensajes (o en su defecto al
	m�todo tick() de la clase padre). En caso de no hacerlo, el
	componente <em>no procesar� ning�n mensaje</em>.
	<p>
	El ciclo de vida de un componente t�pico viene determinado por
	el ciclo de vida de la entidad a la que pertenece. El componente 
	puede estar activado o desactivado:
	<ul>
	   <li> El componente es creado cuando se crea la entidad a la que pertenece
	   al leer el mapa. Despu�s de llamar al constructor de la clase
	   se invoca al m�todo spawn() que podr� leer los par�metros leidos del mapa
	   para inicializar sus atributos. En ese momento se pueden crear recursos
	   que necesite el componente, como las entidades gr�ficas, f�sicas, etc.</li>

	   <li> El componente es <em>activado</em> llamando al m�todo activate().
	   Un componente se activa cuando <em>se activa el mapa donde est� la
	   entidad a la que pertenece</em>,
	   es decir, cuando el motor de juego decide que se empezar� a simular
	   el entorno de ese mapa.</li>

	   <li> El componente finalmente es <em>desactivado</em>, cuando el motor del
	   juego establece que no se desea simular el entorno virtual del mapa
	   donde est� el componente. En ese caso, se invoca al m�todo deactivate().</li>

	   <li> Cuando se destruye el mapa cargado (t�picamente al final del juego/estado
	   del juego), se invoca al destructor de la entidad.</li>
	</ul>
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó Garc�a
	@date Julio, 2010
*/
	class IComponent : public MessageHandler
	{
        RTTI_DECL;
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		IComponent() : _entity(0) {}

		/**
		Destructor (virtual); en la clase base no hace nada.
		*/
		virtual ~IComponent() {}
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). El m�todo es extendido
		en las clases hijas, que hacen cosas m�s inteligentes, dependiendo
		del tipo de componente concreto, pero siempre deber�n invocar a esta
		antes de hacer nada.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		La implementaci�n registrar� al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cron�metro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate() {return true;}
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementaci�n eliminar� al componente de algunos observers en los 
		que pueda estar registrado (como el cron�metro del sistema, etc.).
		*/
		virtual void deactivate() {}

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Las clases hijas deber�n sobreescribir este m�todo con las 
		instrucciones que quieran realizar cada ciclo.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		M�todo que devuelve la entidad a la que pertenece el componente.

		@return La entidad a la que pertenece el componente.
		*/
		CEntity *getEntity() const { return _entity; }

	protected:

		/**
		clase amiga que puede establecerse como poseedor del componente.
		*/
		friend class CEntity;

		/**
		M�todo que establece la entidad a la que pertenece el componente.

		@param entity Entidad a la que pertenece el componente.
		*/
		void setEntity(CEntity *entity) { _entity = entity; }

		/**
		Entidad que contiene al componente. Para reenviar el mensaje a 
		los otros componentes
		*/
		CEntity *_entity;

	}; // class IComponent


/////////////////////////////////////////////////////////////
// Macros para la adici�n de los componentes a la factor�a // 
// de componentes.                                         //
/////////////////////////////////////////////////////////////
	
/** 
Macro para la declaraci�n de los m�todos necesarios para que 
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factor�a.
*/
#define DEC_FACTORY(Class) \
public: \
	/** \
	Crea un componente de la clase en la que se declara. \
	*/ \
    static IComponent* create(); \
	/** \
	Registra el componente de la clase en la factor�a. \
	*/ \
	static bool regist(); \

/** 
Macro para la implementaci�n de los m�todos necesarios para que
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factor�a.
*/
#define IMP_FACTORY(Class) \
IComponent* Class::create() \
{ \
	IComponent* res = new Class(); \
	return res; \
} \
bool Class::regist() \
{ \
	if (!CComponentFactory::getSingletonPtr()->has(#Class)) \
	{ \
		CComponentFactory::getSingletonPtr()->add(Class::create, #Class); \
	} \
	return true; \
}

/** 
Macro que invoca al m�todo que registra la clase en la factor�a.
*/
#define REG_FACTORY(Class) \
static bool RegisteredFactory_##Class = Class::regist();

} // namespace Logic

#define SM_HANDLE_MESSAGE(Class) \
bool HandleMessage(const Class& msg){ \
	bool ret = false; \
	/* Si no hay un nodo actual no hay aristas interesadas así que lo primero es comprobar si hay un nodo válido en _currentNodeId */ \
	if (_currentNodeId != -1) {  \
		/* Buscamos la lista de aristas que salen del nodo actual */ \
		EdgeList::iterator it = _edges->find(_currentNodeId); \
		if (it != _edges->end()) { \
			PairVector* vector = (*it).second; \
			/* Para cada elemento del vector (arista que sale del nodo actual) */ \
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){ \
				/* Procesamos en la arista (o sea, la condición) */ \
				ret |= (edgeIt->first->HandleMessage(msg)); /* Si alguna arista acepta, aceptaremos al final */ \
			} \
		}  \
	} \
return ret; \
}


#endif // __Logic_Component_H
