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
#include "MessageHandler.h"

// Predeclaraciones
namespace Logic {
	class CEntity;
	class CPlaceable;
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
			SET_POSITION,
			SET_ROTATION,
			SET_DIMENSIONS,
			SET_COLOR,
			SET_MATERIAL_NAME,
			SET_ANIMATION,
			STOP_ANIMATION,
			CONTROL,
			AVATAR_WALK,
			KINEMATIC_MOVE,
			TOUCHED,
			UNTOUCHED,
			SWITCH,
			DAMAGED,
			MOVE_TO,
			ROUTE_TO,
			FINISHED_ROUTE,
			FINISHED_MOVE,
			REQUEST_WALK_SOUL_PATH,
			RETURN_WALK_SOUL_PATH,
			PERFORM_WALK_SOUL_PATH,
			SEND_SOUL_WORK, 
			SEND_SOUL_BURN,
			PLACEABLE_FLOAT_TO,
			PLACEABLE_PLACE,
			PLACEABLE_CHECKPOSITION,
		};
	}

    typedef TMessage::MessageType MessageType;

	/**
	Contiene el tipo de datos de un mensaje. Tiene una serie de
	atributos gen�ricos que se interpretar�n en funci�n del tipo 
	de mensaje.

	@ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó Garc�a
	@date Julio, 2010
    @ingroup grupoEntidad
	*/
	
	class Message
	{
	public:
		MessageType _type;
		
		Message() : _type(MessageType::UNASSIGNED)
		{}
		
		Message(MessageType type) : _type(type)
		{}

		virtual bool Dispatch(MessageHandler& handler) const = 0;
	};

	// SET_TRANSFORM
	class TransformMessage : public Message
	{
	public:
		const Matrix4& _transform;

		TransformMessage(const Matrix4& transform) : Message(MessageType::SET_TRANSFORM), _transform(transform)
		{}
		
		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// SET_POSITION
	class PositionMessage : public Message
	{
	public:
		const Vector3& _position;

		PositionMessage(const Vector3& position) : Message(MessageType::SET_POSITION), _position(position)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// SET_ROTATION
	class RotationMessage : public Message
	{
	public:
		// Rotación con respecto a los ejes X(pitch), Y(yaw) y Z(roll).
		const Vector3& _rotation;

		RotationMessage(const Vector3& rotation) : Message(MessageType::SET_ROTATION), _rotation(rotation)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// SET_DIMENSIONS
	class DimensionsMessage : public Message
	{
	public:
		const Vector3& _dimensions;

		DimensionsMessage(const Vector3& dimensions) : Message(MessageType::SET_DIMENSIONS), _dimensions(dimensions)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};
	
	// SET_COLOR
	class ColorMessage : public Message
	{
	public:
		const Vector3& _rgb;

		ColorMessage(const Vector3& rgb) : Message(MessageType::SET_COLOR), _rgb(rgb)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// SET_MATERIAL_NAME
	class MaterialMessage : public Message
	{
	public:
		const std::string _name; // Material doesn't work well using references in Ogre :(

		MaterialMessage(const std::string& materialName) : Message(MessageType::SET_MATERIAL_NAME), _name(materialName)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// SET_ANIMATION, STOP_ANIMATION
	class AnimationMessage : public Message
	{
	public:
        const std::string& _action;
        bool _activated;

		AnimationMessage(MessageType type, const std::string& action, bool activated) : Message(type), _action(action), _activated(activated)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
            return handler.HandleMessage(*this);
		}
	};
	
	enum ActionType
	{
		UNASSIGNED = 0xFFFFFFFF,
		MOVE_FORWARD = 0,
		MOVE_BACKWARD,
		STRAFE_LEFT,
		STRAFE_RIGHT,
		TURN,
		STOP_MOVE,
		STOP_STRAFE
	};

	// CONTROL
	class ControlMessage : public Message
	{
	public:
        ActionType _action;
        float _degreesMoved;

		ControlMessage(ActionType action = ActionType::UNASSIGNED, float degreesMoved = 0.0f) : Message(MessageType::CONTROL), _action(action), _degreesMoved(degreesMoved)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
            return handler.HandleMessage(*this);
		}
	};
	
	// AVATAR_WALK
    class PhysicMessage : public Message
    {
    public:
        const Vector3& _point;

		PhysicMessage(const Vector3& point) : Message(MessageType::AVATAR_WALK), _point(point)
		{}

        virtual bool Dispatch(MessageHandler& handler) const
        {
            return handler.HandleMessage(*this);
        }
    };

	// KINEMATIC_MOVE
	class KinematicMoveMessage : public Message
	{
	public:
        const Vector3& _shift;

		KinematicMoveMessage(const Vector3& shift) : Message(MessageType::KINEMATIC_MOVE), _shift(shift)
		{}

        virtual bool Dispatch(MessageHandler& handler) const
        {
            return handler.HandleMessage(*this);
        }
	};

	// TOUCHED, UNTOUCHED
	class TouchMessage : public Message
	{
	public:
		const CEntity& _entity;
		
		TouchMessage(MessageType type, const CEntity& entity) : Message(type), _entity(entity)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
            return handler.HandleMessage(*this);
		}
	};
	
	// SWITCH,
	// DAMAGED
	class DamageMessage : public Message
	{
	public:
		unsigned int _damage;
		
		DamageMessage(MessageType type, unsigned int damage) : Message(type), _damage(damage)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
            return handler.HandleMessage(*this);
		}
	};
	
	// PLACEABLE_FLOAT_TO, PLACEABLE_PLACE
	class MovePlaceableMessage : public Message
	{
	public:
		const Vector3& _position;
		
		MovePlaceableMessage(MessageType type, const Vector3& position) : Message(type), _position(position)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// PLACEABLE_CHECKPOSITION
	class CheckValidPositionPlaceableMessage : public Message
	{
	public:
		CheckValidPositionPlaceableMessage(MessageType type, CEntity* entity, Vector3 position) : Message(type), _entity(entity), _position(position) {}

		CEntity* _entity;
		Vector3 _position;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// MOVE ??
	class MoveMessage : public Message
	{
	public:
		Vector3 _point;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	/*class SoulTaskMessage : public Message
	{
	public:
		AI::SoulTask task;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};*/

	/*
	TODO Select MessageType for current Message. 

	Mensaje con la cantidad de trabajadores a añadir (numWorkers > 0) o quitar (numWorkers < 0)
	*/
	class WorkerMessage : public Message
	{
	public:
		unsigned int _numWorkers;

		WorkerMessage(unsigned int numWorkers) : Message(MessageType::CONTROL), _numWorkers(numWorkers)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// TODO Remember to discuss if we should separate this in 2 classes
	// REQUEST_WALK_SOUL_PATH, RETURN_WALK_SOUL_PATH, PERFORM_WALK_SOUL_PATH
	class WalkSoulPathMessage : public Message
	{
	public:
		CPlaceable* const _target;
		std::vector<Vector3>* const _path;

		WalkSoulPathMessage(CPlaceable* const target) : Message(MessageType::REQUEST_WALK_SOUL_PATH), _target(target), _path(nullptr)
		{}

		WalkSoulPathMessage(std::vector<Vector3>* const path) : Message(MessageType::RETURN_WALK_SOUL_PATH), _target(nullptr), _path(path)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// SEND_SOUL_WORK, SEND_SOUL_BURN
	class HellQuartersActionMessage : public Message
	{
	public:
		unsigned int _numSouls;

		HellQuartersActionMessage(MessageType type, unsigned int numSouls) : Message(type), _numSouls(numSouls)
		{}

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
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