/**
@file Entity.h

Contiene la declaración de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Logic_Entity_H
#define __Logic_Entity_H

#include "BaseSubsystems/Math.h"

#include "Logic/Maps/EntityID.h"

// Mensaje
#include "Logic/Entity/MessageHandler.h"
#include "Logic/Entity/Message.h"

#include <list>
#include <string>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class IComponent;
	class CEntityFactory;
}

// Declaraciï¿½n de la clase
namespace Logic 
{
	/**
	Clase que representa una entidad en el entorno virtual. Las entidades
	son meros contenedores de componentes, y su funcionamiento depende
	de cuï¿½les tenga. Ademï¿½s, la clase contiene una serie de atributos que
	pueden ser accedidos desde los componentes (Id, nombre, etc.).
	<p>
	Tras la creaciï¿½n de la entidad, donde se inicializan los valores de la
	entidad a su valor por defecto, se invocarï¿½ a su mï¿½todo spawn() en el
	que se inicializarï¿½n los atributos de la entidad con los valores leidos 
	del fichero del mapa. Tras esto, en algï¿½n momento, cuando se active
	el mapa se llamarï¿½ al mï¿½todo activate() de la entidad. En este momento
	los componentes harï¿½n visibles los componentes grï¿½ficos y demï¿½s ya que 
	apartir de ahï¿½ es cuando la entidad empieza su funciï¿½n siendo actualizada 
	por el tick() en cada vuelta de bucle.

    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansï¿½
	@date Agosto, 2010
	*/
	class CEntity : public MessageHandler
	{
	protected:

		/**
		Clase amiga que crea y destruye objetos de la clase.
		*/
		friend class CEntityFactory;

		/**
		Constructor protegido de la clase (para crear CEntity debe
		utilizarse la factorï¿½a CEntityFactory). El constructor
		no hace mï¿½s que poner el identificador ï¿½nico de la entidad
		, pues la inicializaciï¿½n efectiva se hace en el mï¿½todo spawn().
		
		@param entityID Identificador ï¿½nico de la entidad.
		*/
		CEntity(TEntityID entityID);

		/**
		Destructor de la clase. Es un mï¿½todo protegido pues para
		eliminar CEntity debe utilizarse la factorï¿½a
		CEntityFactory. El destructor invoca al destructor de
		cada componente.
		Cuando el destructor es invocado, ï¿½ste _ya ha sido desenganchado
		del mapa al que perteneciï¿½_, por lo que los destructores de los
		componentes no pueden utilizar el mapa.
		*/
		~CEntity();

		/**
		Inicializaciï¿½n del objeto Logic, utilizando la informaciï¿½n extraï¿½da de
		la entidad leï¿½da del mapa (Map::CEntity). Avisarï¿½ a los componentes
		de la entidad para que se inicialicen.

		@param map Mapa Logic en el que se registrarï¿½ la entidad.
		@param entity Informaciï¿½n de construcciï¿½n de la entidad leï¿½da del
		fichero de disco.
		@return Cierto si la inicializaciï¿½n ha sido satisfactoria.
		*/
		bool spawn(CMap *map, const Map::CEntity *entity);

	public:

		/**
		Activa la entidad. Esto significa que el mapa ha sido activado.
		<p>
		El mï¿½todo llama al activate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true si todo fue bien.
		*/
		bool activate();

		/**
		Desactiva la entidad. Esto significa que el mapa ha sido desactivado.
		<p>
		El mï¿½todo llama al deactivate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Funciï¿½n llamada en cada frame para que se realicen las funciones
		de actualizaciï¿½n adecuadas.
		<p>
		Llamarï¿½ a los mï¿½todos tick() de todos sus componentes.

		@param msecs Milisegundos transcurridos desde el ï¿½ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		Mï¿½todo que aï¿½ade un nuevo componente a la lista de la entidad.

		@param component Componente a aï¿½adir.
		*/
		void addComponent(IComponent* component);

		/**
		Mï¿½todo que quita un componente de la lista.

		El componente es eliminado de la lista de componentes de la
		entidad, pero <b>no</b> es desactivado ni eliminado (con delete);
		la responsabilidad de esa tarea se deja al invocante.

		@param component Componente a borrar.
		@return true si se borrï¿½ el componente (false si el componente
		no estaba en el objeto).
		*/
		bool removeComponent(IComponent* component);
		
		/**
		Mï¿½todo que destruye todos los componentes de una entidad.
		*/
		void destroyAllComponents();

		/**
		Recibe un mensaje que envia a todos los componentes de la lista menos 
		al componente que lo envia, si ï¿½ste se especifica en el segundo campo.

		@param message Mensaje a enviar.
		@param emitter Componente emisor, si lo hay. No se le enviarï¿½ el mensaje.
		@return true si al menos un componente aceptï¿½ el mensaje
		*/
		//bool emitMessage(const Message &message, IComponent* emitter = 0);

		// Para tipo de mensaje se hace la gestiÃ³n apropiada.
		bool HandleMessage(const TransformMessage& msg);
		bool HandleMessage(const DimensionsMessage& msg);
		bool HandleMessage(const ColorMessage& msg);
		bool HandleMessage(const MaterialMessage& msg);
		bool HandleMessage(const AnimationMessage& msg);
		bool HandleMessage(const ControlMessage& msg);
		bool HandleMessage(const PhysicMessage& msg);
		bool HandleMessage(const TouchMessage& msg);
		bool HandleMessage(const DamageMessage& msg);
		bool HandleMessage(const PlaceMessage& msg);

		/**
		Devuelve el identificador ï¿½nico de la entidad.

		@return Identificador.
		*/
		Logic::TEntityID getEntityID() const { return _entityID; }

		/**
		Mï¿½todo que indica si la entidad es o no el jugador.
		Seguro que hay formas mejores desde el punto de vista de
		diseï¿½o de hacerlo, pero esta es la mï¿½s rï¿½pida: la entidad 
		con la descripciï¿½n de la entidad tiene esta descripciï¿½n que
		establece en el spawn().
		
		@return true si la entidad es el jugador.
		*/
		bool isPlayer() { return _isPlayer; }

		/**
		Devuelve el mapa donde estï¿½ la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		CMap *getMap() { return _map; }

		/**
		Devuelve el mapa donde estï¿½ la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		const CMap *getMap() const { return _map; }

		/**
		Devuelve el nombre de la entidad.

		@return Nombre de la entidad.
		*/
		const std::string &getName() const { return _name; }

		/**
		Devuelve el tipo de la entidad. Este atributo es leido de
		la entidad del mapa en spawn().

		@return Tipo de la entidad.
		*/
		const std::string &getType() const { return _type; }

		/**
		Establece la matriz de transformaciï¿½n de la entidad. Avisa a los 
		componentes del cambio.

		@param transform Nueva matriz de transformaciï¿½n de la entidad.
		*/
		void setTransform(const Matrix4& transform);

		/**
		Devuelve la metriz de transformaciï¿½n de la entidad.
		<p>
		La posiciï¿½n es inicialmente leï¿½da del mapa (si no aparece,
		se colocarï¿½ a (0, 0, 0)) y la orientaciï¿½n es tambiï¿½n inicialmente 
		leï¿½da del mapa, como un simple viraje (si no aparece, se colocarï¿½ 
		a 0). Obviamente, pueden cambiar con el tiempo.

		@return Matriz de transformaciï¿½n de la entidad en el entorno.
		*/
		Matrix4 getTransform() const { return _transform; }

		/**
		Establece la posiciï¿½n de la entidad. Avisa a los componentes
		del cambio.

		@param position Nueva posiciï¿½n.
		*/
		void setPosition(const Vector3 &position);

		/**
		Devuelve la posiciï¿½n de la entidad.
		<p>
		La posiciï¿½n es inicialmente leï¿½da del mapa (si no aparece,
		se colocarï¿½ a (0, 0, 0)), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Posiciï¿½n de la entidad en el entorno.
		*/
		Vector3 getPosition() const { return _transform.getTrans(); }

		/**
		Establece las dimensiones de la entidad. Avisa a los componentes
		del cambio.

		@param dimensions Nuevas dimensiones.
		*/
		void setDimensions(const Vector3 &dimensions);

		/**
		Devuelve las dimensiones de la entidad.
		<p>
		Las dimensiones son inicialmente leídas del mapa (si no aparecen,
		serán las originales de la malla), aunque, obviamente, pueden
		cambiar con el tiempo.

		@return Dimensiones de la entidad.
		*/
		Vector3 getDimensions() const { return _dimensions; };

		/**
		Establece la orientaciï¿½n de la entidad. Avisa a los componentes
		del cambio.

		@param pos Nueva orientación.
		*/
		void setOrientation(const Matrix3& orientation);

		/**
		Devuelve la matriz de rotaciï¿½n de la entidad.
		<p>
		La orientaciï¿½n es inicialmente leï¿½da del mapa como un simple 
		viraje (si no aparece, se colocarï¿½ a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Orientaciï¿½n en el entorno.
		*/
		Matrix3 getOrientation() const;

		/**
		Establece el viraje de la entidad. Avisa a los componentes
		del cambio.

		@param yaw Nuevo viraje.
		*/
		void setYaw(float yaw);

		/**
		Vira la entidad. Avisa a los componentes del cambio.

		@param yaw Viraje a aplicar.
		*/
		void yaw(float yaw);

		/**
		Devuelve el viraje de la entidad.
		<p>
		La orientaciï¿½n es inicialmente leï¿½da del mapa como un simple 
		viraje (si no aparece, se colocarï¿½ a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Viraje en el entorno.
		*/
		float getYaw() const { return Math::getYaw(_transform); }

		/**
		Indica si la entidad se encuentra activa.

		@return true si la entidad estï¿½ activa.
		*/
		bool isActivated() {return _activated;}

	protected:

		/**
		Clase amiga que puede modificar los atributos (en concreto se 
		usa para modificar el mapa.
		*/
		friend class CMap;

		/**
		Identificador ï¿½nico de la entidad.
		*/
		Logic::TEntityID _entityID;

		/**
		Tipo para la lista de componetes.
		*/
		typedef std::list<IComponent*> TComponentList;

		/**
		Lista de los componentes de la entidad.
		*/
		TComponentList _components;

		/**
		Indica si la entidad estï¿½ activa.
		*/
		bool _activated;

		/**
		Tipo de la entidad declarado en el archivo blueprints.
		*/
		std::string _type;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Mapa lï¿½gico donde estï¿½ la entidad.
		*/
		Logic::CMap *_map;

		/**
		Matriz de transformaciï¿½n de la entidad. Contiene posiciï¿½n y orientaciï¿½n.
		*/
		Matrix4 _transform;

		/*
		Posiciï¿½n de la entidad.
		*
		Vector3 _position;

		/*
		Orientaciï¿½n de la entidad.
		*
		float _orientation;

		/**
		Dimensiones de la entidad.
		*/
		Vector3 _dimensions;

		/**
		Atributo que indica si la entidad es el jugador; por defecto
		es false a no ser que se lea otra cosa de los atributos.
		*/
		bool _isPlayer;

	}; // class CEntity

} // namespace Logic

#endif // __Logic_Entity_H
