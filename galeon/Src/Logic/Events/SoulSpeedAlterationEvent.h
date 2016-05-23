//---------------------------------------------------------------------------
// SoulSpeedAlterationEvent.h
//---------------------------------------------------------------------------

/**
@file SoulSpeedAlterationEvent.h

Declaraci�n de un evento de tipo INFO consistente en alterar la velocidad
de movimiento y de recolecci�n de recursos de las almas durante un cierto
tiempo.

@see Logic::CEvent
@see Logic::CSoulsSpeedReductionEvent

@author �lvaro Valera
@date Mayo, 2016
*/

#ifndef SOUL_SPEED_ALTERATION_EVENT_H_
#define SOUL_SPEED_ALTERATION_EVENT_H_

#include "Event.h"

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
	Clase que representa un evento de alteraci�n de todas las almas
	de la ciudad.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Abril, 2016
	*/
	class CSoulSpeedAlterationEvent : public CEvent
	{

	public:
		/**  Constructor basado en tiempo */
		CSoulSpeedAlterationEvent(
			unsigned long time, const std::string& godName, const std::string& title,
			const std::string& description, const std::string& image,
			float factor, int duration, bool absoluteTime = true, bool restore = false) :

			CEvent(INFO, time, absoluteTime, false, godName), _title(title),
			_description(description), _image(image),
			_factor(factor), _duration(duration), _restore(restore) {};

		/** Constructor est�tico. Permite invocar desde Lua pero mantener el ciclo de vida en C++ */
		static CSoulSpeedAlterationEvent* addCSoulSpeedAlterationEvent(
			unsigned long time, const std::string& godName, const std::string& title,
			const std::string& description, const std::string& image,
			float factor, int duration, bool absoluteTime = true) {

			return new CSoulSpeedAlterationEvent(
				time, godName, title,
				description, image,
				factor, duration, absoluteTime);
		}

		/**
		Destructor.
		*/
		~CSoulSpeedAlterationEvent() {};

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
		Este evento no es instant�neo, sino que tiene un efecto prolongado
		en el tiempo, por lo que se usar� este flag para diferenciar si
		debe aplicarse el evento (en este caso, la alteraci�n de las
		almas) o restaurar la situaci�n habitual de juego.
		*/
		bool _restore;

		/**
		M�todo encargado de ejecutar el evento.
		*/
		void execute();

		/**
		M�todo encargado de llevar a cabo la l�gica espec�fica asociada
		al evento.
		*/
		void apply();

		/**
		M�todo encargado de restaurar, a su valor habitual, la l�gica
		modificada previamente por el evento.
		*/
		void restore();

	private:
		/**
		Factor por el cual se altera la velocidad de las almas. 
		Si factor < 1.0, el evento ralentiza. Si factor > 1, el evento acelera.
		*/
		float _factor;

		/** Duraci�n (ms) de la alteraci�n de velocidad */
		int _duration;
		
		/** Texto descriptivo del evento para mostrar en el panel */
		std::string _description;

		/** T�tulo para el panel */
		std::string _title;

		/** Identificador de imagen de fondo para el panel */
		std::string _image;
		
	}; // class CSoulSpeedAlterationEvent

} // namespace Logic

#endif // SOUL_SPEED_ALTERATION_EVENT_H_
