/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::TMessage

@author David Llansó García
*/
#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>
#include <memory>

#include "BaseSubsystems/Math.h"
#include "MessageHandler.h"
#include "AI/SoulTask.h"

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
			WALK_SOUL_PATH_FINISHED,
			SEND_SOUL_WORK, 
			SEND_SOUL_BURN,
			PLACEABLE_FLOAT_TO,
			PLACEABLE_PLACE,
			PLACEABLE_CHECKPOSITION,
			HELLQUARTERS_REQUEST,
			HELLQUARTERS_RESPONSE,
			SOUL_SENDER_REQUEST,
			SOUL_SENDER_RESPONSE,
			SOUL_REQUEST,
			SOUL_RESPONSE,
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
		const Vector3 _position;
		bool _showFloating;
		// Coloca el placeable en la posición actual
		MovePlaceableMessage() : Message(MessageType::PLACEABLE_PLACE)
		{}
		
		// Hace flotar al placeable hasta la posición dada
		MovePlaceableMessage(const Vector3& position, bool showFloating) : Message(MessageType::PLACEABLE_FLOAT_TO), _position(position), _showFloating(showFloating)
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

	/** Mensaje con la cantidad de trabajadores a añadir (numWorkers > 0) o quitar (numWorkers < 0) */
	class WorkerMessage : public Message
	{
	public:
		WorkerMessage(int numWorkers) : Message(MessageType::UNASSIGNED), _numWorkers(numWorkers) {}

		int _numWorkers;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// TODO Remember to discuss if we should separate this in 2 classes
	// REQUEST_WALK_SOUL_PATH, RETURN_WALK_SOUL_PATH, PERFORM_WALK_SOUL_PATH, WALKING_SOUL_PATH_FINISHED
	class WalkSoulPathMessage : public Message
	{
	public:
		// Petición de ruta (REQUEST_WALK_SOUL_PATH)
		WalkSoulPathMessage(CPlaceable* const target) : Message(MessageType::REQUEST_WALK_SOUL_PATH), _target(target), _path(nullptr) {}
		
		// Respuesta/orden con la ruta (RETURN_WALK_SOUL_PATH, PERFORM_WALK_SOUL_PATH)
		WalkSoulPathMessage(std::vector<Vector3>* const path) : _target(nullptr), _path(path) {}

		// Ruta finalizada (WALKING_SOUL_PATH_FINISHED)
		WalkSoulPathMessage() : Message(MessageType::WALK_SOUL_PATH_FINISHED), _target(nullptr), _path(nullptr) {}

		WalkSoulPathMessage(MessageType type) : Message(type), _target(nullptr), _path(nullptr) {}

		CPlaceable* _target;
		std::vector<Vector3>* _path;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	enum HellQuartersAction{
		SEND_SOUL_BURN,
		SEND_SOUL_WORK,
	};

	// REQUEST_SEND_SOUL_WORK, REQUEST_SEND_SOUL_BURN
	class HellQuartersMessage : public Message
	{
	public:
		HellQuartersMessage(HellQuartersAction action, int numSouls) : Message(MessageType::HELLQUARTERS_REQUEST), _action(action), _numSouls(numSouls) {}

		HellQuartersAction _action;
		int _numSouls;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// SOUL_SENDER_REQUEST, SOUL_SENDER_RESPONSE
	class SoulSenderMessage : public Message
	{
	public:
		SoulSenderMessage(AI::CSoulTask* task, int numSouls) : Message(MessageType::SOUL_SENDER_REQUEST), _task(task), _numSouls(numSouls) {}

		//std::unique_ptr<AI::CSoulTask> _task;
		AI::CSoulTask* _task;
		unsigned int _numSouls;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	// SOUL_REQUEST, SOUL_RESPONSE
	class SoulMessage : public Message
	{
	public:
		SoulMessage(AI::CSoulTask* task) : Message(MessageType::SOUL_REQUEST), _task(task) {}
		SoulMessage() : Message(MessageType::SOUL_RESPONSE), _task(0) {}

		//std::unique_ptr<AI::CSoulTask> _task;
		AI::CSoulTask* _task;

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