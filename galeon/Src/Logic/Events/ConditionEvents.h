//---------------------------------------------------------------------------
// ConditionEvents.h
//---------------------------------------------------------------------------

/**
@file ConditionEvents.h

Contiene la declaraci�n de los diferentes tipos de eventos condicionales
que pueden ser lanzados durante la partida tras una determinada acci�n
del jugador o situaci�n de juego.

@see Logic::CEvent

@author Ra�l Segura
@date Marzo, 2016
*/

#ifndef __Logic_ConditionEvents_H
#define __Logic_ConditionEvents_H

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
	Posibles tipos de eventos condicionales (_trigger == CONDITION).

	@ingroup logicGroup

	@author Ra�l Segura
	@date Marzo, 2016
	*/
	enum ConditionEventType
	{
		// Eventos disparados como parte del tutorial para guiar al
		// jugador durante los primeros instantes de la partida.
		TUTORIAL,
		// Evento disparado al final de la partida para mostrar el
		// resultado de la misma y la puntuaci�n final.
		END_GAME
	};

} // namespace Logic

#endif // __Logic_ConditionEvents_H
