/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::TMessage

@author David Llans� Garc�a
*/
#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>

#include "BaseSubsystems/Math.h"
#include "MessageHandler.h"
#include "Entity.h"

// Predeclaraciones
namespace Logic {
	class CEntity;
};

namespace Logic
{
	/**
	Namespace para los tipos de mensajes posibles.
	*/
	namespace TMessage
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

    typedef TMessage::MessageType MessageType;

	/**
	Contiene el tipo de datos de un mensaje. Tiene una serie de
	atributos gen�ricos que se interpretar�n en funci�n del tipo 
	de mensaje.
	<p>
	@remarks <b>??ESTO NO ES ESCALABLE!!</b> En tu proyecto esto 
	deber�a ser cambiado.
	Lo suyo ser�a tener una clase CMesage base y luego clases que
	especializasen a �sta con los atributos necesarios para el 
	mensaje concreto en vez de tener una serie de atributos
	gen�ricos como es esta estructura que deben de ser interpretados
	externamente en funci�n del tipo de mensaje.
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llans� Garc�a
	@date Julio, 2010
    @ingroup grupoEntidad
	*/
	
	class Message
	{
	public:
		MessageType _type;
		virtual void Dispatch(MessageHandler& handler) const = 0;
	};

	// SET_TRANSFORM
	class TransformMessage : public Message
	{
	public:
		Matrix4 _transform;
		
		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};

	// SET_SCALE
	class ScaleMessage : public Message
	{
	public:
		Vector3 _scale;

		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	// SET_ANIMATION, STOP_ANIMATION
	class AnimationMessage : public Message
	{
	public:		
		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	// CONTROL
	class ControlMessage : public Message
	{
	public:
        std::string _action;
        float _degreesMoved;

		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	// AVATAR_WALK,
	// KINEMATIC_MOVE
    class MoveMessage : public Message
    {
    public:
        Vector3 _point;

        virtual void Dispatch(MessageHandler& handler) const
        {
            handler.HandleMessage(*this);
        }
    };


	// TOUCHED, UNTOUCHED
	class TouchMessage : public Message
	{
	public:
		CEntity& _entity;
		
		virtual void Dispatch(MessageHandler& handler) const
		{
			handler.HandleMessage(*this);
		}
	};
	
	// SWITCH,
	// DAMAGED
	class DamageMessage : public Message
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