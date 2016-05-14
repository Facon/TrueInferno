//---------------------------------------------------------------------------
// ObstacleGrowthEvent.h
//---------------------------------------------------------------------------

/**
@file ObstacleGrowthEvent.h

Declaración de la clase CObstacleGrowthEvent.

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#ifndef OBSTACLE_GROWTH_EVENT_H_
#define OBSTACLE_GROWTH_EVENT_H_

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
	Clase que representa un evento que crea obstáculos por el mapa

	@ingroup logicGroup

	@author Álvaro Valera
	@date Mayo, 2016
	*/
	class CObstacleGrowthEvent : public CEvent
	{

	public:

		/**  Constructor */
		CObstacleGrowthEvent(
			unsigned long time, const std::string& godName, const std::string& title, const std::string& description, const std::string& image, 
			float tileFactor, bool absoluteTime = true) :
						
			CEvent(INFO, time, absoluteTime), _godName(godName), _title(title), _description(description), _image(image), 
			_tileFactor(tileFactor) {};

		static CObstacleGrowthEvent* addCObstacleGrowthEvent(
			unsigned long time, const std::string& godName, const std::string& title, const std::string& description, const std::string& image, 
			float tileFactor, bool absoluteTime = true) {

			return new CObstacleGrowthEvent(
				time, godName, title, description, image, 
				tileFactor, absoluteTime);
		}

		/**
		Destructor.
		*/
		~CObstacleGrowthEvent() {};

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
		/** Factor (en escala unitaria) sobre la cantidad total de tiles que se van a convertir en obstáculos */
		float _tileFactor;
		
		/** Nombre del dios que provoca el evento */
		std::string _godName;
		
		/** Texto descriptivo del evento para mostrar en el panel */
		std::string _description;

		/** Título para el panel */
		std::string _title;

		/** Identificador de imagen de fondo para el panel */
		std::string _image;

	}; // class CObstacleGrowthEvent

} // namespace Logic

#endif // OBSTACLE_GROWTH_EVENT_H_
