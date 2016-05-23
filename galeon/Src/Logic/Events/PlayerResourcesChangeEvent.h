//---------------------------------------------------------------------------
// PlayerResourcesChangeEvent.h
//---------------------------------------------------------------------------

/**
@file PlayerResourcesChangeEvent.h

Declaraci�n de la clase CPlayerResourcesChangeEvent.

@see Logic::CEvent

@author �lvaro Valera
@date Mayo, 2016
*/

#ifndef PLAYER_RESORUCES_CHANGE_EVENT_H_
#define PLAYER_RESORUCES_CHANGE_EVENT_H_

#include <string>

#include "Event.h"
#include "Logic/ResourcesManager.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n.
namespace Logic
{
}

/**
Namespace que engloba la l�gica del juego. Engloba desde el mapa l�gico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para m�s informaci�n ver @ref LogicGroup).

@author David Llans�
@date Agosto, 2010
*/
namespace Logic
{
	/**
	Clase que representa un evento que da o quita recursos al jugador.

	@ingroup logicGroup

	@author �lvaro Valera
	@date Mayo, 2016
	*/
	class CPlayerResourcesChangeEvent : public CEvent
	{

	public:
		/**  Constructor basado en tiempo */
		CPlayerResourcesChangeEvent(
			unsigned long time, const std::string& godName, const std::string& title,
			const std::string& description, const std::string& image,
			float resourceStoredFactor, float resourceStorageFactor, ResourceType resourceType, bool absoluteTime = true) :

			CEvent(INFO, time, absoluteTime, false, godName), _title(title),
			_description(description), _image(image),
			_resourceStoredFactor(resourceStoredFactor), _resourceStorageFactor(resourceStorageFactor), _resourceType(resourceType) {};

		/** Constructor est�tico. Permite invocar desde Lua pero mantener el ciclo de vida en C++ */
		static CPlayerResourcesChangeEvent* addCPlayerResourcesChangeEvent(
			unsigned long time, const std::string& godName, const std::string& title,
			const std::string& description, const std::string& image,
			float resourceStoredFactor, float resourceStorageFactor, ResourceType resourceType, bool absoluteTime = true) {

			return new CPlayerResourcesChangeEvent(
				time, godName, title,
				description, image,
				resourceStoredFactor, resourceStorageFactor, resourceType, absoluteTime);
		}

		/**
		Destructor.
		*/
		~CPlayerResourcesChangeEvent() {};

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este m�todo desde CEventManager::luaRegister.
		*/
		static void luaRegister();
		
		/** Devuelve la imagen para la GUI */
		std::string getGUIImageName() const;

		/** Devuelve el t�tulo para la GUI */
		std::string getGUITitle() const;

		/** Devuelve el campo de texto para la GUI */
		std::string getGUIText() const;

		/** Devuelve el campo adicional de texto para la GUI */
		std::string getGUIResumeText() const;
		
	protected:

		/**
		M�todo encargado de ejecutar el evento.
		*/
		void execute();

	private:
		/** M�nimo factor en valor absoluto considerado mayor que 0 */
		const float ZERO_FACTOR = 0.0001;

		/** Factor (en escala unitaria) sobre la cantidad total de recursos almacenado que se van a a�adir (>0) o quitar (<0) */
		float _resourceStoredFactor;
		
		/** Factor (en escala unitaria) sobre la capacidad total de almacenamiento de recursos que se van a a�adir (>0) o quitar (<0) */
		float _resourceStorageFactor;

		/** Tipo del recurso que se da o se quita */
		ResourceType _resourceType;

		/** Texto descriptivo del evento para mostrar en el panel */
		std::string _description;

		/** T�tulo para el panel */
		std::string _title;

		/** Identificador de imagen de fondo para el panel */
		std::string _image;

	}; // class CPlayerResourcesChangeEvent

} // namespace Logic

#endif // PLAYER_RESORUCES_CHANGE_EVENT_H_
