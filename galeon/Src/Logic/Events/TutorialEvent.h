//---------------------------------------------------------------------------
// TutorialEvent.h
//---------------------------------------------------------------------------

/**
@file TutorialEvent.h

Declaraci�n de un evento de tipo INFO consistente �nicamente en mostrar
uno de los paneles del tutorial.

@see Logic::CEvent

@author Ra�l Segura
@date Marzo, 2016
*/

#ifndef __Logic_TutorialEvent_H
#define __Logic_TutorialEvent_H

#include "Event.h"

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
	Clase que representa un evento de tutorial encargado simplemente de
	mostrar uno de los paneles del mismo (cuyo �ndice recibir� como
	par�metro en el constructor).

	@ingroup logicGroup

	@author Ra�l Segura
	@date Marzo, 2016
	*/
	class CTutorialEvent : public CEvent
	{

	public:

		/**
		Constructor.
		*/
		CTutorialEvent(int tutorialpanelIndex) :
			CEvent(INFO, CEvent::ConditionTriggerType::TUTORIAL), _tutorialPanelIndex(tutorialpanelIndex)	{};

		static CTutorialEvent* addCTutorialEvent(int tutorialpanelIndex) {
			return new CTutorialEvent(tutorialpanelIndex);
		};

		/**
		Destructor.
		*/
		~CTutorialEvent() {};

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este m�todo desde CEventManager::luaRegister.
		*/
		static void luaRegister();

	protected:

		/**
		�ndice del panel del tutorial a mostrar.
		*/
		int _tutorialPanelIndex;

		/**
		M�todo encargado �nicamente de ejecutar la l�gica espec�fica
		asociada al evento.
		*/
		void execute();
		
	}; // class CTutorialEvent

} // namespace Logic

#endif // __Logic_TutorialEvent_H
