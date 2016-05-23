//---------------------------------------------------------------------------
// RoundTimeChangeEvent.h
//---------------------------------------------------------------------------

/**
@file RoundTimeChangeEvent.h

Declaración de la clase CRoundTimeChangeEvent.

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#ifndef ROUND_TIME_CHANGE_EVENT_H_
#define ROUND_TIME_CHANGE_EVENT_H_

#include <string>

#include "Event.h"
#include "Logic/TimeManager.h"

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
	Clase que representa un evento que acorta o extiende el tiempo restante de la ronda actual.

	@ingroup logicGroup

	@author Álvaro Valera
	@date Mayo, 2016
	*/
	class CRoundTimeChangeEvent : public CEvent
	{

	public:
		/**  Constructor basado en tiempo */
		CRoundTimeChangeEvent(
			unsigned long time, const std::string& godName, const std::string& title,
			const std::string& description, const std::string& image,
			int roundTimeChange, bool absoluteTime = true) :

			CEvent(INFO, time, absoluteTime, false, godName), _title(title),
			_description(description), _image(image),
			_roundTimeChange(roundTimeChange) {};

		/** Constructor estático. Permite invocar desde Lua pero mantener el ciclo de vida en C++ */
		static CRoundTimeChangeEvent* addCRoundTimeChangeEvent(
			unsigned long time, const std::string& godName, const std::string& title,
			const std::string& description, const std::string& image,
			int roundTimeChange, bool absoluteTime = true) {

			return new CRoundTimeChangeEvent(
				time, godName, title,
				description, image,
				roundTimeChange, absoluteTime);
		}

		/**
		Destructor.
		*/
		~CRoundTimeChangeEvent() {};

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
		Método encargado de ejecutar el evento.
		*/
		void execute();

	private:
		/** Tiempo (s) que se va a añadir (>0) o quitar (<0) del tiempo restante de la ronda actual */
		int _roundTimeChange;

		/** Texto descriptivo del evento para mostrar en el panel */
		std::string _description;

		/** Título para el panel */
		std::string _title;

		/** Identificador de imagen de fondo para el panel */
		std::string _image;

	}; // class CRoundTimeChangeEvent

} // namespace Logic

#endif // ROUND_TIME_CHANGE_EVENT_H_
