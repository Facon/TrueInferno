//---------------------------------------------------------------------------
// BuildingDestructionEvent.h
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.h

Declaración de un evento de tipo INFO consistente en la destrucción de
un edificio de forma aleatoria.

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#ifndef __Logic_BuildingDestructionEvent_H
#define __Logic_BuildingDestructionEvent_H

#include "Event.h"

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
	Clase que representa un evento de destrucción de un edificio
	de forma aleatoria.

	@ingroup logicGroup

	@author Raúl Segura
	@date Marzo, 2016
	*/
	class CBuildingDestructionEvent : public CEvent
	{

	public:

		/* TODO 
		CBuildingDestructionEvent(ConditionTriggerType condEventType, bool dummy = true) : CEvent(INFO, condEventType) {};
		*/

		/**  Constructor basado en tiempo */
		CBuildingDestructionEvent(
			unsigned long time, const std::string& godName, const std::string& title,
			const std::string& description, const std::string& image,
			bool absoluteTime = true) :

			CEvent(INFO, time, absoluteTime, false, godName), 
			_title(title), _description(description), _image(image) {};

		/** Constructor estático. Permite invocar desde Lua pero mantener el ciclo de vida en C++ */
		static CBuildingDestructionEvent* addCBuildingDestructionEvent(
			unsigned long time, const std::string& godName, const std::string& title,
			const std::string& description, const std::string& image,
			bool absoluteTime = true) {

			return new CBuildingDestructionEvent(
				time, godName, title,
				description, image, absoluteTime);
		}

		/**
		Destructor.
		*/
		~CBuildingDestructionEvent() {};

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este método desde CEventManager::luaRegister.
		*/
		static void luaRegister();
		
		/** Devuelve la imagen para la GUI */
		std::string getGUIImageName() const;

		/** Devuelve el título para la GUI */
		std::string getGUITitle() const;

		/** Devuelve el campo de texto para la GUI */
		std::string getGUIText() const;

		/** Devuelve el campo adicional de texto para la GUI */
		std::string getGUIResumeText() const;
		
	protected:

		/**
		Método encargado únicamente de ejecutar la lógica específica
		asociada al evento.
		*/
		void execute();
		
	private:	
		/** Texto descriptivo del evento para mostrar en el panel */
		std::string _description;

		/** Título para el panel */
		std::string _title;

		/** Identificador de imagen de fondo para el panel */
		std::string _image;
		
	}; // class CBuildingDestructionEvent

} // namespace Logic

#endif // __Logic_BuildingDestructionEvent_H
