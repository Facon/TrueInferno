/**
@file AvatarController.h

Contiene la declaraci�n del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes m�todos que permiten avances o giros. El uso de este componente
	es un poco at�pico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las �rdenes que se deben
	llevar a cabo mediante llamadas a m�todos p�blicos del componente. Puede
	no obstante ampliarse este componente para aceptar mensajes tradicionales
	con las �rdenes, sin embargo de momento as� es suficiente.
	
    @ingroup logicGroup

	@author David Llansó Garc�a
	@date Agosto, 2010
*/
	class CAvatarController : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(CAvatarController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAvatarController() : IComponent(), _walking(false), _walkingBack(false), 
			_strafingLeft(false), _strafingRight(false), _speed(0.05f) {}
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra as� mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y rat�n.

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra as� mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y rat�n.
		*/
		virtual void deactivate();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CONTROL.

		/**
		Provoca que la entidad avance.
		*/
		void walk();

		/**
		Provoca que la entidad retroceda.
		*/
		void walkBack();

		/**
		Provoca que la entidad cese el avance.
		*/
		void stopWalk();

		/**
		Provoca que la entidad se desplace lateralmente a la izquierda.
		*/
		void strafeLeft();

		/**
		Provoca que la entidad se desplace lateralmente a la derecha.
		*/
		void strafeRight();

		/**
		Provoca que la entidad cese el desplazamiento lateral.
		*/
		void stopStrafe();
		
		/**
		Provoca que la entidad gire. N�meros Positivos para	giro a 
		derechas, negativos para giro izquierdas.

		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void turn(float amount);

		bool HandleMessage(const ControlMessage& msg);

	protected:

		/**
		Atributo para saber si la entidad est� avanzando.
		*/
		bool _walking;

		/**
		Atributo para saber si la entidad est� retrocediendo.
		*/
		bool _walkingBack;

		/**
		Atributo para saber si la entidad est� moviendose lateralmente
		a la izquierda.
		*/
		bool _strafingLeft;

		/**
		Atributo para saber si la entidad est� moviendose lateralmente
		a la derecha.
		*/
		bool _strafingRight;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _speed;

	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H
