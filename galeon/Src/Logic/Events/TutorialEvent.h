//---------------------------------------------------------------------------
// TutorialEvent.h
//---------------------------------------------------------------------------

/**
@file TutorialEvent.h

Declaración de un evento de tipo INFO consistente únicamente en mostrar
uno de los paneles del tutorial.

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#ifndef __Logic_TutorialEvent_H
#define __Logic_TutorialEvent_H

#include "Event.h"
#include "ConditionEvents.h"

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
	Clase que representa un evento de tutorial encargado simplemente de
	mostrar uno de los paneles del mismo (cuyo índice recibirá como
	parámetro en el constructor).

	@ingroup logicGroup

	@author Raúl Segura
	@date Marzo, 2016
	*/
	class CTutorialEvent : public CEvent
	{

	public:

		/**
		Constructor.
		*/
		CTutorialEvent(int tutorialpanelIndex) :
			CEvent(INFO, ConditionEventType::TUTORIAL), _tutorialPanelIndex(tutorialpanelIndex)	{};

		/**
		Destructor.
		*/
		virtual ~CTutorialEvent() {};

	protected:

		/**
		Índice del panel del tutorial a mostrar.
		*/
		int _tutorialPanelIndex;

		/**
		Método encargado únicamente de ejecutar la lógica específica
		asociada al evento.
		*/
		virtual void execute();
		
	}; // class CTutorialEvent

} // namespace Logic

#endif // __Logic_TutorialEvent_H
