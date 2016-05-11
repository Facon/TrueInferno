//---------------------------------------------------------------------------
// GodBoostEvent.h
//---------------------------------------------------------------------------

/**
@file GodBoostEvent.h

Declaraci�n de la clase CGodBoostEvent.

@see Logic::CEvent

@author �lvaro Valera
@date Mayo, 2016
*/

#ifndef GOD_BOOST_EVENT_H_
#define GOD_BOOST_EVENT_H_

#include <string>

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
	Clase que representa un evento que otorga a un dios un boost
	temporal sobre la velocidad de incrementeo del score durante
	un tiempo determinado.

	@ingroup logicGroup

	@author �lvaro Valera
	@date Mayo, 2016
	*/
	class CGodBoostEvent : public CEvent
	{

	public:

		/**  Constructor para crear el evento. */
		CGodBoostEvent(
			unsigned long time, const std::string& godName, const std::string& title, const std::string& description, const std::string& image,
			float factor, int duration, bool absoluteTime = true) :
			
			CEvent(INFO, time, absoluteTime), _godName(godName), _title(title), _description(description), _image(image),
			_factor(factor), _duration(duration), _restore(false) {};

		/**  Constructor para detener el efecto el evento. */
		CGodBoostEvent(
			unsigned long time, const std::string& godName, float factor, bool absoluteTime = true) :

			CEvent(INFO, time, absoluteTime), _godName(godName), _title(""), _description(""), _image(""),
			_factor(factor), _duration(0), _restore(true) {};

		static CGodBoostEvent* addCGodBoostEvent(
			unsigned long time, const std::string& godName, const std::string& title, const std::string& description, const std::string& image,
			float factor, int duration, bool absoluteTime = true) {
			return new CGodBoostEvent(time, godName, title, description, image, factor, duration, absoluteTime);
		}

		/**
		Destructor.
		*/
		~CGodBoostEvent() {};

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este m�todo desde CEventManager::luaRegister.
		*/
		static void luaRegister();

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
		Factor por el cual se altera la velocidad del score del dios. 
		Si factor < 1.0, el evento ralentiza. Si factor > 1, el evento acelera.
		*/
		float _factor;

		/** Duraci�n (ms) del evento */
		int _duration;

		/** Nombre del dios afectado por el evento */
		std::string _godName;
		
		/** Texto descriptivo del evento para mostrar en el panel */
		std::string _description;

		/** T�tulo para el panel */
		std::string _title;

		/** Identificador de imagen de fondo para el panel */
		std::string _image;

	}; // class CGodBoostEvent

} // namespace Logic

#endif // GOD_BOOST_EVENT_H_
