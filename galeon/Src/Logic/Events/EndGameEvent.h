//---------------------------------------------------------------------------
// EndGameEvent.h
//---------------------------------------------------------------------------

/**
@file EndGameEvent.h

Declaración de un evento de tipo INFO consistente únicamente en mostrar
el panel de fin del juego (con victoria o derrota).

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#ifndef __Logic_EndGameEvent_H
#define __Logic_EndGameEvent_H

#include <string>

#include "Event.h"

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
	Clase que representa un evento de fin del juego encargado simplemente
	de mostrar el panel final de victoria o derrota.

	@ingroup logicGroup

	@author Raúl Segura
	@date Marzo, 2016
	*/
	class CEndGameEvent : public CEvent
	{

	public:
		/**
		Constructor basado en trigger.
		*/
		CEndGameEvent(bool victory) :
			CEvent(INFO, CEvent::ConditionTriggerType::END_GAME, true, ""), _victory(victory) {
			initGUIConstants();
		};

		/**
		Constructor basado en tiempo.

		@param victory Flag a true si el resultado de la ronda es de victoria para el jugador, a false para derrota
		@param delay Retraso en ms para lanzar el evento desde el instante de tiempo actual
		*/
		CEndGameEvent(bool victory, int delay) :
			CEvent(INFO, delay, false, true, ""), _victory(victory) {
			initGUIConstants();
		};

		/**
		Destructor.
		*/
		~CEndGameEvent() {};

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
		Resultado de la partida (Victoria o Derrota).
		*/
		bool _victory;

		/**
		Método encargado únicamente de ejecutar la lógica específica
		asociada al evento.
		*/
		void execute();
		
	private:
		std::string _eventImage;
		std::string _eventTitle;
		std::string _eventText;
		std::string _eventTextResume;

		void initGUIConstants();
		
	}; // class CEndGameEvent

} // namespace Logic

#endif // __Logic_EndGameEvent_H
