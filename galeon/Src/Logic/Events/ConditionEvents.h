//---------------------------------------------------------------------------
// ConditionEvents.h
//---------------------------------------------------------------------------

/**
@file ConditionEvents.h

Contiene la declaración de los diferentes tipos de eventos condicionales
que pueden ser lanzados durante la partida tras una determinada acción
del jugador o situación de juego.

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#ifndef __Logic_ConditionEvents_H
#define __Logic_ConditionEvents_H

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
	Posibles tipos de eventos condicionales (_trigger == CONDITION).

	@ingroup logicGroup

	@author Raúl Segura
	@date Marzo, 2016
	*/
	enum ConditionEventType
	{
		// Eventos disparados como parte del tutorial para guiar al
		// jugador durante los primeros instantes de la partida.
		TUTORIAL,
		// Evento disparado al final de la partida para mostrar el
		// resultado de la misma y la puntuación final.
		END_GAME
	};

} // namespace Logic

#endif // __Logic_ConditionEvents_H
