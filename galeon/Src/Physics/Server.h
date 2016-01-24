/**
@file Server.h

Contiene la declaraci�n del servidor de f�sica. 

@see Physics::CServer

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "BaseSubsystems/Math.h"

// Predeclaraci�n de tipos
namespace Logic {
	class CEntity;
	class CPhysicController;
	class CPhysicEntity;
	class IPhysics;
};

namespace Physics {
	class CCollisionManager;
	class CErrorManager;
};

namespace physx {
	class PxActor;
	class PxAllocatorCallback;
	class PxCapsuleController;
	class PxController;
	class PxControllerManager;
	class PxCooking;
	class PxDefaultAllocator;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	class PxMaterial;
	class PxPhysics;
	class PxProfileZoneManager;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxRigidStatic;
	class PxScene;
	class PxCudaContextManager;

	namespace debugger {
		namespace comm {
			class PvdConnection;
		};
	};
};



// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	Servidor de f�sica. Se encarga de gestionar las entidades f�sicas y simular su 
	comportamiento. Cumple varias finalidades:
	
	<ul>
	<li> Proporciona una interfaz com�n con el resto de servidores de la aplicaci�n (gr�ficos, GUI, etc). </li> 
	<li> A�sla el resto de la aplicaci�n del motor de f�sica concreto que usamos (hasta cierto punto).
	<li> Realiza las conversiones entre los tipos l�gicos y los de PhysX (vectores, matrices, etc).
	</ul>
	<p>
	Las entidades f�sicas de PhysX y los componentes l�gicos que proporcionan la representaci�n f�sica de la entidad
	est�n relacionados de dos formas:
	<ul>
	<li> Los componentes l�gicos almacenan internamente sus actores de PhysX asociados. </li>
	<li> Usamos el atributo userData de los actores de PhysX para almacenar el componente l�gico asociado. </li>
	</ul>
	De esta forma, la capa l�gica pueden mover los objetos f�sicos (actores cinem�ticos, character controllers, 
	aplicar fuerzas a actores din�micos, etc) y la capa de f�sica puede comunicar las colisiones a los componentes
	l�gicos.
	<p>
	Es aconsejable utilizar los m�todos del servidor para cambiar / recuperar las posiciones de las
	entidades f�sicas, ya que el servidor realiza las conversiones necesarias entre sistemas de coordenadas.
	Aunque la l�gica y PhysX usan el mismo tipo de sistema de coordenadas, PhysX asume que el origen est� 
	en medio del objeto mientras que la l�gica asume que el origen est� a los pies del objeto. Usando los 
	m�todos que	proporciona el servidor, la l�gica no tiene que preocuparse de estas conversiones.
	<p>
	Esta clase est� implementada como un Singleton de inicializaci�n expl�cita: es necesario 
	invocar al m�todo Init() al principio de la aplicaci�n y al m�todo Release() al final.

	@ingroup physicGroup

	@author Antonio S�nchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CServer 
	{
	public:

		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor f�sico.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor f�sico. Esta operaci�n s�lo es necesario realizarla
		una vez durante la inicializaci�n de la aplicaci�n. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor f�sico. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Realiza la simulaci�n f�sica y actualiza la posici�n de todas las 
		entidades f�sicas. 

		@param secs Millisegundos transcurridos desde la �ltima actualizaci�n.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(unsigned int msecs);


		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena f�sica. Se asume que s�lo existir� una escena f�sica, por lo que 
		s�lo debe invocarse una vez.
		*/
		void createScene ();

		/**
		Destruye la escena f�sica.
		*/
		void destroyScene ();

		/**
		Establece si debe haber colisiones entre dos grupos de colisi�n. Al crear los objetos
		f�sicos se establece el grupo de colisi�n al que pertenecen. Usando este m�todo es
		posible activar / desactivar las colisiones entre grupos.

		@param group1 Primer grupo de colisi�n.
		@param group2 Segundo grupo de colisi�n
		@param enable Activar o desactivar las colisiones entre los dos grupos anteriores.
		*/
		void setGroupCollisions(int group1, int group2, bool enable);


		//------------------------------
		// Gesti�n de entidades simples
		//------------------------------

		/**
		Crea un plano est�tico en la escena.

		@param point Punto del plano.
		@param normal Vector normal al plano.
		@param group Grupo de colisi�n.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidStatic* createPlane(const Vector3 &point, const Vector3 &normal, 
			                              int group, const Logic::IPhysics *component);

		/**
		Crea una caja est�tica en la escena.

		@param position Posici�n de la caja en coordenadas l�gicas (el origen de coordenadas 
		       est� en el centro de la cara inferior del cubo).
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de 
		       la caja medir�n dimensions*2.
		@param trigger Indica si la entidad f�sica representa un trigger.
		@param group Grupo de colisi�n.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidStatic* createStaticBox(const Vector3 &position, const Vector3 &dimensions, 
			                                  bool trigger, int group, const Logic::IPhysics *component); 

		/**
		Crea una caja din�mica en la escena.

		@param position Posici�n de la caja en coordenadas l�gicas (el origen de coordenadas 
		       est� en el centro de la cara inferior del cubo).
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de 
		       la caja medir�n dimensions*2.
		@param mass Masa distribuida uniformemente en el volumen de la entidad.
		@param kinematic Indica si la entidad es cinem�tica.
		@param trigger Indica si la entidad f�sica representa un trigger.
		@param group Grupo de colisi�n.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidDynamic* createDynamicBox(const Vector3 &position, const Vector3 &dimensions, 
			                                    float mass, bool kinematic, bool trigger, int group, 
												const Logic::IPhysics *component); 

		/**
		Crea una entidad f�sica en la escena a partir de un fichero RepX exportado con el 
		plug-in de PhysX para 3ds Max. Asume que el fichero contiene �nicamente la 
		descripci�n de un actor.

		@param file Fichero generado con el plug-in de PhysX.
		@param group Grupo de colisi�n en el que debe ser incluida la entidad.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidActor* createFromFile(const std::string &file, int group, 
			                                const Logic::IPhysics *component);

		/**
		Elimina una entidad f�sica de la escena y libera los recursos que tenga asociados.
		NO DEBE USARSE CON CHARACTER CONTROLLERS.

		@param actor Actor f�sico asociado a la entidad.
		 */
		void destroyActor(physx::PxActor *actor);

		/**
		Devuelve la posici�n y rotaci�n de una entidad f�sica.

		@param actor Actor f�sico del que se desea conocer la posici�n y orientaci�n.
		@return Matriz 4x4 con la posici�n y orientaci�n de la entidad.
		 */
		Matrix4 getActorTransform(const physx::PxRigidActor *actor);

		/**
		Mueve un actor cinem�tico.

		@param actor Actor cinem�tico que se desea mover.
		@param transform Nueva posici�n y orientaci�n. 
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform);

		/**
		Aplica una traslaci�n a un actor cinem�tico.

		@param actor Actor cinem�tico que se desea mover.
		@param displ Desplazamiento a realizar
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, const Vector3 &displ);

		/**
		Indica si un actor din�mico es cinem�tico.
		 */
		bool isKinematic(const physx::PxRigidDynamic *actor);
		

		//----------------------------------
		// Gesti�n de character controllers
		//----------------------------------

		/**
		Crea un controller de tipo capsula en la escena.

		@param position Posici�n del controller en coordenadas l�gicas (el origen de coordenadas 
		       est� en los pies del controller).
		@param radius Radio de la c�psula.
		@param height Altura de la c�psula.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return character controller creado
		*/
		physx::PxCapsuleController* createCapsuleController(const Vector3 &position, float radius, 
															float height, 
															const Logic::CPhysicController *component);


		/**
		Mueve el controller por la escena.

		@param controller Controller que se desea mover.
		@param movement Movimiento que se quiere realizar.
		@param msecs Millisegundos transcurridos desde la �ltima actualizaci�n.
		@return Flags de colisi�n, un conjunto de physx::PxControllerFlag.
		*/
		unsigned moveController(physx::PxController *controller, const Vector3 &movement,
			                             unsigned int msecs);

		/**
		Devuelve la posici�n del controller.

		@param controller Controller del que se desea conocer su posici�n.
		@return posici�n del controller en coordenadas l�gicas.
		*/
		Vector3 getControllerPosition(const physx::PxCapsuleController *controller);

		/**
		Establece la posici�n del controller.

		@param controller Controller del que se desea conocer su posici�n.
		@param posici�n del controller en coordenadas l�gicas.
		*/
		void setControllerPosition(physx::PxCapsuleController *controller, const Vector3 &position);

		
		//----------------------------------
		// Consultas 
		//----------------------------------

		/**
		 Lanza un rayo y devuelve la primera entidad l�gica contra la que interseca. Si el rayo
		 no choca contra ninguna entidad devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia m�xima de la primera intersecci�n.
		 @return Primera entidad l�gica alcanzada o NULL.
		 */
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist) const; 

		/**
		 Lanza un rayo y devuelve la primera entidad l�gica contra la que interseca que pertenezca 
		 al grupo de colisi�n indicado. Si el rayo no choca contra ninguna entidad de ese grupo
		 devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia m�xima de la primera intersecci�n.
		 @param group Grupo de colisi�n de la entidad buscada.
		 @return Primera entidad l�gica alcanzada de ese grupo o NULL.
		 */
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist, int group) const; 

	private:

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		// Instancia �nica de la clase.
		static CServer *_instance;

		// Gestor de errores
		physx::PxErrorCallback *_errorManager;

		// Gestor de memoria
		physx::PxAllocatorCallback *_allocator;

		// Distintos atributos relacionados con PhysX
		physx::PxFoundation *_foundation;
		physx::PxProfileZoneManager *_profileZoneManager;
		physx::PxDefaultCpuDispatcher *_cpuDispatcher;
		physx::PxCudaContextManager *_cudaContextManager;
		physx::debugger::comm::PvdConnection *_pvdConnection;
		physx::PxCooking *_cooking;

		// SDK de PhysX
		physx::PxPhysics *_physics;

		// Escena f�sica
		physx::PxScene *_scene;

		// Gestor de controller managers
		physx::PxControllerManager* _controllerManager;

		// Material que se aplica por defecto a las nuevas entidades f�sicas
		physx::PxMaterial *_defaultMaterial;

		// Gestion de colisiones
		CCollisionManager *_collisionManager;

	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H
