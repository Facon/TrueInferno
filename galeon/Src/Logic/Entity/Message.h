/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::TMessage

@author David Llansó García
*/
#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>

#include "BaseSubsystems/Math.h"

// Predeclaraciones
namespace Logic {
	class CEntity;
};

namespace Logic
{
	/**
	Namespace para los tipos de mensajes posibles.
	*/
	namespace Message
	{
		enum MessageType
		{
			UNASSIGNED = 0xFFFFFFFF,
			SET_TRANSFORM,
			SET_SCALE,
			SET_ANIMATION,
			STOP_ANIMATION,
			CONTROL,
			AVATAR_WALK,
			KINEMATIC_MOVE,
			TOUCHED,
			UNTOUCHED,
			SWITCH,
			DAMAGED
		};
	}
	
	/**
	Tipo copia para los mensajes. Por simplicidad.
	*/
	typedef Message::MessageType MessageType;

	/**
	Contiene el tipo de datos de un mensaje. Tiene una serie de
	atributos genéricos que se interpretarán en función del tipo 
	de mensaje.
	<p>
	@remarks <b>??ESTO NO ES ESCALABLE!!</b> En tu proyecto esto 
	debería ser cambiado.
	Lo suyo sería tener una clase CMesage base y luego clases que
	especializasen a ésta con los atributos necesarios para el 
	mensaje concreto en vez de tener una serie de atributos
	genéricos como es esta estructura que deben de ser interpretados
	externamente en función del tipo de mensaje.
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó García
	@date Julio, 2010
    @ingroup grupoEntidad
	*/
	/*
	typedef struct
	{
		/**
		Tipo del mensaje.
		*/
		TMessageType _type;

		/**
		Atributo para almacenar una matriz de transformación.
		*/
		Matrix4 _transform;
		
		/**
		Atributo para almacenar un valor float.
		*/
		float _float;
		
		/**
		Atributo para almacenar un valor booleano.
		*/
		bool _bool;
		
		/**
		Atributo para almacenar un string.
		*/
		std::string _string;

	} TMessage;
	*/
	
	class Message
	{
	public:
		MessageType _type;
		virtual void Dispatch(MessageHandler& handler) const = 0;
	};

	// SET_TRANSFORM, SET_SCALE
	class TransformMessage
	{
	public:
		Matrix4 _transform;
		
		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	// SET_ANIMATION, STOP_ANIMATION
	class AnimationMessage
	{
	public:		
		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	// CONTROL
	class ControlMessage
	{
	public:
		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	// AVATAR_WALK,
	// KINEMATIC_MOVE,
	// TOUCHED, UNTOUCHED
	class TouchMessage
	{
	public:
		bool touched = false;
		
		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	// SWITCH,
	// DAMAGED
	class DamageMessage
	{
	public:
		unsigned int damage = 0;
		
		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	/*
	GameObject* receiver_object = blah;
	
	// Send the "Up arrow pressed" message
	KeyMessage msg;
	msg._type = MSG_KEY_DOWN;
	msg._keyCode = KEY_CODE_UP_ARROW;
	msg.Dispatch(*receiver_object);
	*/
} // namespace Logic

#endif // __Logic_Message_H