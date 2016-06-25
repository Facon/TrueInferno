//---------------------------------------------------------------------------
// ObstacleGrowthEvent.h
//---------------------------------------------------------------------------

/**
@file ObstacleGrowthEvent.h

Declaraci�n de la clase CObstacleGrowthEvent.

@see Logic::CEvent

@author �lvaro Valera
@date Mayo, 2016
*/

#ifndef OBSTACLE_GROWTH_EVENT_H_
#define OBSTACLE_GROWTH_EVENT_H_

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
	Clase que representa un evento que crea obst�culos por el mapa

	@ingroup logicGroup

	@author �lvaro Valera
	@date Mayo, 2016
	*/
	class CObstacleGrowthEvent : public CEvent
	{

	public:

		/**  Constructor basado en tiempo */
		CObstacleGrowthEvent(
			unsigned long time, const std::string& godName, const std::string& title, 
			const std::string& description, const std::string& image, 
			float quantity, bool isFactor, bool absoluteTime = true) :
						
			CEvent(INFO, time, absoluteTime, false, godName), _title(title), 
			_description(description), _image(image), 
			_quantity(quantity), _isFactor(isFactor) {};

		/** Constructor est�tico. Permite invocar desde Lua pero mantener el ciclo de vida en C++ */
		static CObstacleGrowthEvent* addCObstacleGrowthEvent(
			unsigned long time, const std::string& godName, const std::string& title, 
			const std::string& description, const std::string& image, 
			float quantity, bool isFactor, bool absoluteTime = true) {

			return new CObstacleGrowthEvent(
				time, godName, title, 
				description, image, 
				quantity, isFactor, absoluteTime);
		}

		/**
		Destructor.
		*/
		~CObstacleGrowthEvent() {};

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
		/** Cantidad absoluta o factor (en escala unitaria) sobre la cantidad total de tiles que se van a convertir en obst�culos */
		float _quantity;
		
		/** Flag a true si tileFactor*/
		bool _isFactor;

		/** Texto descriptivo del evento para mostrar en el panel */
		std::string _description;

		/** T�tulo para el panel */
		std::string _title;

		/** Identificador de imagen de fondo para el panel */
		std::string _image;

	}; // class CObstacleGrowthEvent

} // namespace Logic

#endif // OBSTACLE_GROWTH_EVENT_H_
