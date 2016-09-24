//---------------------------------------------------------------------------
// Scene.h
//---------------------------------------------------------------------------

/**
@file Scene.h

Contiene la declaraci�n de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_Scene_H
#define __Graphics_Scene_H

#include <string>
#include <list>

namespace Ogre 
{
	class Vector3;
	class Root;
	class Viewport;
	class SceneManager;
	class StaticGeometry;
	class Light;
	class BillboardSet;
}
namespace Graphics 
{
	class CServer;
	class CCamera;
	class CEntity;
	class CStaticEntity;
}

namespace Graphics 
{
	/**
	Clase que controla todos los elementos de una escena. Puede ser de 
	un estado como el men� o un nivel del juego. Su equivalente en la 
	l�gica del juego en el caso de ser un nivel ser�a el mapa. la escena 
	es un contenedor de todas las entidades gr�ficas que pueden ser 
	reenderizadas por un viewport asociado a esa escena. Se asume que una
	escena solo va a ser reenderizada desde un punto de vista por lo que
	por defecto se crea una �nica c�mara y cuando se activa la escena se
	crea un �nico viewport mostrado en la totalidad de la ventana de 
	reenderizado. En funci�n del tipo de juego puede quererse hacer m�s de
	un reenderizado de la misma escena a la vez y mostrarlo en diferentes 
	viewports en la ventana. Por ejemplo en un juego de coches para mostrar
	la imagen de los retrovisores del coche.
	<p>
	Tr�s crear la escena se deben a�adir las diferentes entidades y 
	entidades est�ticas. Al a�adir las entidades a la escena se fuerza su
	carga. Las entidades est�ticas se almacenan en una estructura. Una vez 
	a�adidas todas las entidades se debe crear la geometr�a est�tica ya 
	que hasta que no se invoque a buildStaticGeometry() las entidades no
	pasar�n a formar parte de la geometr�a est�tica. Al activar la escena
	se fuerza la creaci�n de la geometr�a est�tica si no se hab�a creado.
	<p>
	@remarks Una vez activada la escena las modificaciones realizadas en
	los valores de las entidades est�ticas no tendr�n efecto.
	<p>
	@remarks Solo el servidor gr�fico (Graphics::CServer) puede crear o 
	liberar escenas.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CScene 
	{
	public:

		/**
		Devuelve la c�mara de la escena.

		@return Puntero a la c�mara de la escena.
		*/
		CCamera *getCamera() {return _camera;}

		/**
		Devuelve el nombre de la escena.

		@return Nombre de la escena.
		*/
		const std::string& getName() {return _name;}

		/**
		A�ade una entidad gr�fica a la escena.
		<p>
		@remarks La escena NO se hace responsable de destruir la
		entidad.

		@param entity Entidad gr�fica que se quiere a�adir a la escena.
		@return Cierto si la entidad se a�adi� y carg� correctamente.
		*/
		bool addEntity(CEntity* entity);

		/**
		A�ade una entidad gr�fica est�tica a la escena. No sirve
		addEntity porque estas entidades deben ser almacenadas en otra
		lista para cuando se cree la geometr�a est�tica de la escena poder 
		recuperarlas y cargarlas.
		<p>
		@remarks La escena NO se hace responsable de destruir la
		entidad.

		@param entity Entidad gr�fica que se quiere a�adir a la escena.
		@return Cierto si la entidad se a�adi� y carg� correctamente.
		*/
		bool addStaticEntity(CStaticEntity* entity);

		/**
		Elimina una entidad gr�fica de la escena. 
		<p>
		@remarks Este m�todo NO destrulle la entidad, �sta solo deja de
		ser parte de la escena.

		@param entity Entidad gr�fica que se quiere eliminar de la escena.
		*/
		void removeEntity(CEntity* entity);

		/**
		Elimina una entidad gr�fica est�tica de la escena. 
		<p>
		@remarks Este m�todo NO destrulle la entidad, �sta solo deja de
		ser parte de la escena.

		@param entity Entidad gr�fica est�tica que se quiere eliminar de 
		la escena.
		*/
		void removeStaticEntity(CStaticEntity* entity);

		/**
		Crear billboards.
		*/
		Ogre::BillboardSet* createBillboardSet(CEntity* entity, const std::string& name, const Ogre::Vector3& position) const;
		Ogre::BillboardSet* createBillboardSet(CEntity* entity, const std::string& name) const;

		/**
		Activar y desactivar la luz focal para edificios.
		*/
		void turnOnBuildingLight(Ogre::Vector3 buildingPosition);
		void turnOffBuildingLight();

	protected:

		/**
		Clase amiga. Solo el servidor gr�fico puede crear o liberar escenas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;
		
		/**
		Constructor de la clase.
		*/
		CScene(const std::string& name);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CScene();

		/**
		Despierta la escena y crea un viewport que ocupa toda la
		pantalla.
		*/
		void activate();

		/**
		Duerme la escena y destruye su viewport para que no se siga 
		reenderizando.
		*/
		void deactivate();
		
		/**
		Actualiza el estado de la escena cada ciclo. Llama a su vez a 
		todas las entidades.
		
		@param secs N�mero de segundos transcurridos desde la �ltima 
		llamada.
		*/
		void tick(float secs);

		/**
		A�ade las entidades est�ticas a la geometr�a est�tica del nivel
		y la construlle. Si la geometr�a est�tica ya ha sido construida
		no se vuelve a construir.
		<p>
		@remarks Una vez construida la geometr�a est�tica no se pueden 
		modificar los valores de las entidades est�ticas.
		*/
		void buildStaticGeometry();

		/**
		Clase amiga. Solo las entidades y la c�mara pueden acceder al 
		gestor de la escena de Ogre.
		*/
		friend class CEntity;
		friend class CCamera;

		/**
		Devuelve el gestor de la escena de Ogre

		@return Puntero al gestor de la escena de Ogre.
		*/
		Ogre::SceneManager *getSceneMgr() { return _sceneMgr; }

		/**
		Clase amiga. Solo las entidades pueden acceder al gestor de la
		escena de Ogre.
		*/
		friend class CStaticEntity;

		/**
		Devuelve la geometr�a est�tica de la escena de Ogre

		@return Puntero a la geometr�a est�tica de la escena de Ogre.
		*/
		Ogre::StaticGeometry *getStaticGeometry() {return _staticGeometry;}
		
		/**
		Nombre de la escena.
		*/
		std::string _name;
		
		/**
		Punto de entrada al sistema Ogre.
		*/
		Ogre::Root *_root;

		/** 
		Marco en la ventana de reenderizado donde se pinta lo captado por
		una c�mara. Solo puede haber una c�mara asociada a un viewport,
		sin embargo una ventana de reenderizado puede tener diferentes
		viewports al mismo tiempo.
		*/
		Ogre::Viewport *_viewport;
		
		/**
		Controla todos los elementos Ogre de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		Ogre::SceneManager *_sceneMgr;
		
		/**
		Luz direccional que se crea por defecto en la escena. Gr�ficamente
		mejora la escena bastante respecto a tener solo luz ambiente ya que
		se ven mejor los vol�menes de las entidades.
		*/
		Ogre::Light *_directionalLight;

		/**
		Luz focal que apunta desde la cámara al cursor.
		*/
		Ogre::Light *_spotlightLight;

		/**
		Luz focal que ilumina un edificio concreto durante el tutorial.
		*/
		Ogre::Light *_buildingHighlight;

		float _spotlightAcumTime;
		float _spotlightThresholdTime;

		/**
		Camara desde la que se ver� la escena. Puede haber c�maras m�s
		sofisticadas y m�s tipos de c�maras desde el punto de vista l�gico,
		ellas se encargar�n de mover esta instancia.
		*/
		CCamera *_camera;

		/**
		Tipos para la lista de entidades.
		*/
		typedef std::list<CStaticEntity*> TStaticEntityList;

		/**
		Tipos para la lista de entidades.
		*/
		typedef std::list<CEntity*> TEntityList;

		/**
		Lista de entidades est�ticas.
		*/
		TStaticEntityList _staticEntities;

		/**
		Lista de entidades din�micas.
		*/
		TEntityList _dynamicEntities;
		
		/**
		Geometr�a est�tica de la escena.
		*/
		Ogre::StaticGeometry *_staticGeometry;

	}; // class CScene

} // namespace Graphics

#endif // __Graphics_Scene_H
