//---------------------------------------------------------------------------
// PlayerResourcesChangeEvent.h
//---------------------------------------------------------------------------

/**
@file PlayerResourcesChangeEvent.h

Declaración de la clase CPlayerResourcesChangeEvent.

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#ifndef PLAYER_RESORUCES_CHANGE_EVENT_H_
#define PLAYER_RESORUCES_CHANGE_EVENT_H_

#include <string>

#include "Event.h"
#include "Logic/ResourcesManager.h"

// Predeclaración de clases para ahorrar tiempo de compilación.
namespace Logic
{
}

/**
Namespace que engloba la lógica del juego. Engloba desde el mapa lógico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para más información ver @ref LogicGroup).

@author David Llansó
@date Agosto, 2010
*/
namespace Logic
{
	/**
	Clase que representa un evento que da o quita recursos al jugador.

	@ingroup logicGroup

	@author Álvaro Valera
	@date Mayo, 2016
	*/
	class CPlayerResourcesChangeEvent : public CEvent
	{

	public:

		/**  Constructor */
		CPlayerResourcesChangeEvent(
			unsigned long time, const std::string& godName, const std::string& title, const std::string& description, const std::string& image, 
			float resourceStoredFactor, float resourceStorageFactor, ResourceType resourceType, bool absoluteTime = true) :
						
			CEvent(INFO, time, absoluteTime), _godName(godName), _title(title), _description(description), _image(image),
			_resourceStoredFactor(resourceStoredFactor), _resourceStorageFactor(resourceStorageFactor), _resourceType(resourceType)
			{};

		static CPlayerResourcesChangeEvent* addCPlayerResourcesChangeEvent(
			unsigned long time, const std::string& godName, const std::string& title, const std::string& description, const std::string& image, 
			float resourceStoredFactor, float resourceStorageFactor, ResourceType resourceType, bool absoluteTime = true) {

			return new CPlayerResourcesChangeEvent(
				time, godName, title, description, image, 
				resourceStoredFactor, resourceStorageFactor, resourceType, absoluteTime);
		}

		/**
		Destructor.
		*/
		~CPlayerResourcesChangeEvent() {};

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este método desde CEventManager::luaRegister.
		*/
		static void luaRegister();

	protected:

		/**
		Método encargado de ejecutar el evento.
		*/
		void execute();

	private:
		/** Mínimo factor en valor absoluto considerado mayor que 0 */
		const float ZERO_FACTOR = 0.0001;

		/** Factor (en escala unitaria) sobre la cantidad total de recursos almacenado que se van a añadir (>0) o quitar (<0) */
		float _resourceStoredFactor;
		
		/** Factor (en escala unitaria) sobre la capacidad total de almacenamiento de recursos que se van a añadir (>0) o quitar (<0) */
		float _resourceStorageFactor;

		/** Tipo del recurso que se da o se quita */
		ResourceType _resourceType;

		/** Nombre del dios que provoca el evento */
		std::string _godName;
		
		/** Texto descriptivo del evento para mostrar en el panel */
		std::string _description;

		/** Título para el panel */
		std::string _title;

		/** Identificador de imagen de fondo para el panel */
		std::string _image;

	}; // class CPlayerResourcesChangeEvent

} // namespace Logic

#endif // PLAYER_RESORUCES_CHANGE_EVENT_H_
