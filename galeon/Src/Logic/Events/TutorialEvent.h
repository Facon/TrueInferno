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

#include <cassert>

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

		/**  Constructor basado en trigger */
		CTutorialEvent(int tutorialpanelIndex) :
			CEvent(INFO, CEvent::ConditionTriggerType::TUTORIAL, true, std::string("")), _tutorialPanelIndex(tutorialpanelIndex) {
			initGUIConstants();
		};

		/** Constructor est�tico. Permite invocar desde Lua pero mantener el ciclo de vida en C++ */
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
		�ndice del panel del tutorial a mostrar.
		*/
		int _tutorialPanelIndex;

		/**
		M�todo encargado �nicamente de ejecutar la l�gica espec�fica
		asociada al evento.
		*/
		void execute();
		
	private:
		std::string _eventImage;
		std::string _eventTitle;
		std::string _eventText;
		std::string _eventTextResume;

		void initGUIConstants();

	}; // class CTutorialEvent

} // namespace Logic

#endif // __Logic_TutorialEvent_H
