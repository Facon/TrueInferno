/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::Message

@author David Llansó García
*/
#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>
#include <memory>

#include "MessageHandler.h"

#include "Logic/Events/EventManager.h"

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/ScriptManager.h"

#include "AI/SoulTask.h"
#include "Logic/ResourcesManager.h"

// Predeclaraciones
namespace Logic
{
	class CEntity;
	class CPlaceable;
	enum LogicRequirement;
};

namespace Logic
{
	/**
	Clase raíz de la jerarquía de mensajes.
	Contiene únicamente el tipo de mensaje.

	@ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó Garc�a
	@date Julio, 2010
    @ingroup grupoEntidad
	*/
	class Message
	{
	public:

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
			PLACEABLE_CONSUME_COST,
			HELLQUARTERS_REQUEST,
			HELLQUARTERS_RESPONSE,
			SOUL_SENDER_REQUEST,
			SOUL_SENDER_RESPONSE,
			SOUL_REQUEST,
			SOUL_RESPONSE,
			FURNACE_BURN_SOULS,
			RESOURCES_ASK,
			RESOURCES_INFO,
			RESOURCES_CHANGE,
			RESOURCES_RESERVE,
			RESOURCES_RESERVED,
			RESOURCES_FREE,
			RESOURCES_CLAIM,
			LOGISTICS_DEMAND_RESOURCES,
			LOGISTICS_PROVIDE_RESOURCES,
			WORKER_ASSIGNED,
			WORKER_ACTIVATED,
			POWER_REQUEST_ATTACHMENT,
			POWER_ATTACHMENT_INFO,
			CONSUMPTION_START,
			CONSUMPTION_STOP,
			CONSUMPTION_STOPPED,
			CONSUMPTION_CHANGE,
			TOGGLE_REQUEST,
			TOGGLE_INFO,
		};

		MessageType _type;
		
		Message() : _type(MessageType::UNASSIGNED)
		{}
		
		Message(MessageType type) : _type(type)
		{}

		virtual bool Dispatch(MessageHandler& handler) const = 0;

		/**
		Registra las clases de mensaje necesarias en el contexto de Lua.
		IMPORTANTE: Llamar a este método desde CEventManager::luaRegister.
		*/
		static void luaRegister();
	};

	/*
	Tipo de mensaje.
	*/
	typedef Logic::Message::MessageType MessageType;

	#define RESEND(message) \
		CEventManager::getSingletonPtr()->HandleMessage(message);

	// SET_TRANSFORM
	class TransformMessage : public Message
	{
	public:
		const Matrix4& _transform;

		TransformMessage(const Matrix4& transform) : Message(MessageType::SET_TRANSFORM), _transform(transform)
		{}
		
		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	class GetCostPlaceableMessage : public Message
	{
	public:
		GetCostPlaceableMessage(MessageType type, CEntity* entity) : Message(type), _entity(entity) {}

		CEntity* _entity;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	/** Mensajes de trabajadores:
	- WORKER_ASSIGNED: Cantidad a aumentar o disminuir de trabajadores asignados al edificio (i.e. un trabajador va a ir a trabajar al edificio)
	- WORKER_ACTIVATED: Cantidad a aumentar o disminuir de trabajadores activados en el edificio (i.e. un trabajador ha llegado al edificio y empieza a trabajar)
	*/
	class WorkerMessage : public Message
	{
	public:
		WorkerMessage(MessageType type, int change) : Message(type), _change(change) {}

		// Cambio solicitado
		unsigned int _change;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	// TODO Remember to discuss if we should separate this in 2 classes
	// REQUEST_WALK_SOUL_PATH, RETURN_WALK_SOUL_PATH, PERFORM_WALK_SOUL_PATH, WALKING_SOUL_PATH_FINISHED
	class WalkSoulPathMessage : public Message
	{
	public:
		// Petición de ruta (REQUEST_WALK_SOUL_PATH)
		WalkSoulPathMessage(TEntityID target) : Message(MessageType::REQUEST_WALK_SOUL_PATH), _target(target), _path(nullptr) {}
		
		// Respuesta/orden con la ruta (RETURN_WALK_SOUL_PATH, PERFORM_WALK_SOUL_PATH)
		WalkSoulPathMessage(std::vector<Vector3>* const path) : _target(EntityID::UNASSIGNED), _path(path) {}

		// Ruta finalizada (WALKING_SOUL_PATH_FINISHED)
		WalkSoulPathMessage() : Message(MessageType::WALK_SOUL_PATH_FINISHED), _target(EntityID::UNASSIGNED), _path(nullptr) {}

		WalkSoulPathMessage(MessageType type) : Message(type), _target(EntityID::UNASSIGNED), _path(nullptr) {}

		TEntityID _target;
		std::vector<Vector3>* _path;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	// REQUEST_SEND_SOUL_WORK, REQUEST_SEND_SOUL_BURN
	class HellQuartersMessage : public Message
	{
	public:

		enum HellQuartersAction {
			SEND_SOUL_BURN,
			SEND_SOUL_WORK,
		};

		HellQuartersMessage(HellQuartersAction action) : Message(MessageType::HELLQUARTERS_REQUEST), _action(action) {}

		HellQuartersAction _action;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

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
			RESEND(*this);

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
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	/** Mensaje para enviar números enteros: FURNACE_BURN_SOULS */
	class NumberMessage : public Message
	{
	public:
		NumberMessage(MessageType type, int number) : Message(type), _number(number) {}

		int _number;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	/** Mensajes relativos a los recursos: 
		- RESOURCES_ASK: Solicitud de información de recursos de la entidad
		- RESOURCES_INFO: Información de recursos de la entidad
		- RESOURCES_CHANGE: Solicitud de modificación de recursos almacenados
		- RESOURCES_RESERVE: Solicitud de reserva de recursos disponibles para que otras solicitudes no puedan reservarlos simultáneamente
		- RESOURCES_RESERVED: Confirmación de cantidad final reservada de recursos
		- RESOURCES_FREE: Solicitud de liberación de recursos reservados previamente
		- RESOURCES_CLAIM: Solicitud de reclamación de los recursos reservados previamente
		(TODO Añadir, si procede, RESOURCES_CHANGED: Confirmación a la solicitud de modificación de recursos. De momento la confirmación equivale a que el HandleMessage devuelva true)
	*/
	class ResourceMessage : public Message
	{
	public:
		/** Constructor vacío por defecto. 
		Necesario para poder tener variables auxiliares sin necesidad de punteros (e.g. en una LA de una SM)
		*/
		ResourceMessage() : Message(MessageType::UNASSIGNED), _resourceType(ResourceType::NONE), 
			_quantity(0), _stored(0), _available(0), _max(0), 
			_provided(false), _caller(EntityID::UNASSIGNED) {}

		/** RESOURCES_ASK: Solicita información sobre el recurso del tipo dado y el ID de la entidad que envía la solicitud */
		void assembleResourcesAsk(const ResourceType& resourceType, TEntityID caller) {
			_type = MessageType::RESOURCES_ASK;
			_resourceType = resourceType;
			_caller = caller;
		}

		/** RESOURCES_INFO: Devuelve la cantidad real, disponible y máxima del recurso indicado, si el recurso se provee externamente y el ID de la entidad que tiene los recursos */
		void assembleResourcesInfo(const ResourceType& resourceType, int stored, int available, unsigned int max, bool provided, TEntityID caller) {
			_type = MessageType::RESOURCES_INFO;
			_resourceType = resourceType;
			_stored = stored;
			_available = available;
			_max = max;
			_provided = provided;
			_caller = caller;
		}

		/** RESOURCES_CHANGE: Solicita el cambio (positivo/negativo) en la cantidad de recursos del tipo dado */
		void assembleResourcesChange(const ResourceType& resourceType, int quantity) {
			_type = MessageType::RESOURCES_CHANGE;
			_resourceType = resourceType;
			_quantity = quantity;
		}

		/** RESOURCES_RESERVE: Intenta reservar todo lo posible hasta la cantidad de recursos indicada del tipo dado para la entidad indicada. 
		No cambia la cantidad real almacenada sino la cantidad disponible para evitar que otras solicitudes simultáneas nos quiten nuestra reserva.
		El que reserva recursos contrae la responsabilidad de liberarlos (RESOURCES_FREE) o reclamarlos (RESOURCES_CLAIM).
		Tras la reserva la entidad indicada recibirá un mensaje RESOURCES_RESERVED con la cantidad finalmente reservada.
		*/
		void assembleResourcesReserve(const ResourceType& resourceType, int quantity, TEntityID caller) {
			_type = MessageType::RESOURCES_RESERVE;
			_resourceType = resourceType;
			_quantity = quantity;
			_caller = caller;
		}

		/** RESOURCES_RESERVED: Informa de la cantidad de recursos finalmente reservada del tipo dado.
		No cambia la cantidad real almacenada sino la cantidad disponible para evitar que otras solicitudes simultáneas nos quiten nuestra reserva.
		El que reserva recursos contrae la responsabilidad de liberarlos (RESOURCES_FREE) o reclamarlos (RESOURCES_CLAIM).
		*/
		void assembleResourcesReserved(const ResourceType& resourceType, int quantity) {
			_type = MessageType::RESOURCES_RESERVED;
			_resourceType = resourceType;
			_quantity = quantity;
		}

		/** RESOURCES_FREE: Libera la cantidad de recursos reservada del tipo dado para que otras solicitudes puedan volverlo a reservar */
		void assembleResourcesFree(const ResourceType& resourceType, int quantity) {
			_type = MessageType::RESOURCES_FREE;
			_resourceType = resourceType;
			_quantity = quantity;
		}

		/** RESOURCES_CLAIM: Reclama (i.e. libera la reserva y modifica) la cantidad de recursos del tipo dado para efectivamente consumir lo reservado */
		void assembleResourcesClaim(const ResourceType& resourceType, int quantity) {
			_type = MessageType::RESOURCES_CLAIM;
			_resourceType = resourceType;
			_quantity = quantity;
		}

		ResourceType _resourceType;
		int _quantity;
		int _stored;
		int _available;
		int _max;
		bool _provided;
		TEntityID _caller;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	/** Mensajes de comunicación relacionados con la logística de recursos */
	class LogisticsMessage : public Message
	{
	public:
		LogisticsMessage() : Message(MessageType::UNASSIGNED), _resourceType(ResourceType::NONE), 
			_resourceQuantity(0), _target(EntityID::UNASSIGNED) {}

		// LOGISTICS_DEMAND_RESOURCES: Solicita buscar la cantidad de recursos indicada del tipo dado
		void assembleDemandResources(ResourceType resourceType, unsigned int resourceQuantity) {
			_type = MessageType::LOGISTICS_DEMAND_RESOURCES;
			_resourceType = resourceType;
			_resourceQuantity = resourceQuantity;
			_target = EntityID::UNASSIGNED;
		}

		// LOGISTICS_PROVIDE_RESOURCES: Obliga a proveer la cantidad indicada del recurso dado al objetivo proporcionado
		void assembleProvideResources(ResourceType resourceType, unsigned int resourceQuantity, const TEntityID& target) {
			_type = MessageType::LOGISTICS_PROVIDE_RESOURCES;
			_resourceType = resourceType;
			_resourceQuantity = resourceQuantity;
			_target = target;
		}

		ResourceType _resourceType;
		unsigned int _resourceQuantity;
		TEntityID _target;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	/** Mensajes relacionados con el habilitado/deshabilitado lógico de una entidad */
	class ToggleMessage : public Message
	{
	public:
		// TOGGLE_REQUEST: Solicita añadir o eliminar un requisito para permitir el habilitado de la entidad
		ToggleMessage(LogicRequirement requirement, bool add) :
			Message(MessageType::TOGGLE_REQUEST), _requirement(requirement), _add(add) {}

		// TOGGLE_INFO: Informa de un cambio en el habilitado o deshabilitado de la entidads
		ToggleMessage(bool enabled) :
			Message(MessageType::TOGGLE_INFO), _enabled(enabled) {}

		// Criterio por el que se quiere habilitar/deshabilitar
		//std::string _criterion;

		// Requisito lógico por el que se quiere habilitar o deshabilitar
		LogicRequirement _requirement;

		// Flag a true para indicar que se quiere añadir un requisito al habilitado de la entidad
		bool _add;

		// Flag a true si se está informando del habilitado de la entidad o false para el deshbilitado
		bool _enabled;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			RESEND(*this);

			return handler.HandleMessage(*this);
		}
	};

	/** Mensajes para el protocolo de comunicación entre generadores y consumidores de energía: 
		- POWER_REQUEST_ATTACHMENT: Petición de conexión a un generador
		- POWER_ATTACHMENT_INFO: Comunicación de consumidor conectado/desconectado
	*/
	class PowerMessage : public Message
	{
	public:
		PowerMessage() : 
			Message(MessageType::UNASSIGNED), _caller(EntityID::UNASSIGNED), 
			_attach(false), _consumptionUnits(0), _consumptionPeriod(0) {}

		// POWER_REQUEST_ATTACHMENT: Solicita la conexión de un consumidor nuevo (la desconexión nunca se solicita, se envía directamente POWER_ATTACHMENT_INFO para informar del cambio)
		void assemblePowerRequestAttachment(TEntityID caller, int consumptionUnits, int consumptionPeriod) {
			_type = MessageType::POWER_REQUEST_ATTACHMENT;
			_caller = caller;
			_consumptionUnits = consumptionUnits;
			_consumptionPeriod = consumptionPeriod;
		}

		// POWER_ATTACHMENT_INFO: Informa de la conexión o desconexión de un consumidor
		void assemblePowerAttachmentInfo(TEntityID caller, bool attach, int consumptionUnits, int consumptionPeriod) {
			_type = MessageType::POWER_ATTACHMENT_INFO;
			_caller = caller;
			_attach = attach;
			_consumptionUnits = consumptionUnits;
			_consumptionPeriod = consumptionPeriod;
		}

		// Entidad a conectar/desconectar
		TEntityID _caller;

		// Flag a true si se quiere informar de una conexión o false para desconexión
		bool _attach;

		// Unidades de consumo de la entidad
		int _consumptionUnits;

		// Periodo de consumo de la entidad
		int _consumptionPeriod;

		virtual bool Dispatch(MessageHandler& handler) const
		{
			return handler.HandleMessage(*this);
		}
	};

	class ConsumptionMessage : public Message
	{
	public:
		ConsumptionMessage() : 
			Message(MessageType::UNASSIGNED), _resourceType(ResourceType::NONE),
			_consumptionChange(0), _consumptionPeriod(0) {}

		// CONSUMPTION_START: Solicita el comienzo de los ciclos de consumo (i.e. se ha conectado el primer consumidor)
		void assembleConsumptionStart(ResourceType resourceType) {
			_type = MessageType::CONSUMPTION_START;
			_resourceType = resourceType;
		}

		// CONSUMPTION_STOP: Solicita el fin de los ciclos de consumo (i.e. se ha desconectado el último consumidor)
		void assembleConsumptionStop(ResourceType resourceType) {
			_type = MessageType::CONSUMPTION_STOP;
			_resourceType = resourceType;
		}

		// CONSUMPTION_STOPPED: Informa de que los ciclos de consumo han parado (i.e. se han acabado los recursos)
		void assembleConsumptionStopped(ResourceType resourceType) {
			_type = MessageType::CONSUMPTION_STOPPED;
			_resourceType = resourceType;
		}

		// CONSUMPTION_CHANGE: Informa de la variación deseada en el consumo
		void assembleConsumptionChange(ResourceType resourceType, int consumptionChange, int consumptionPeriod) {
			_type = MessageType::CONSUMPTION_CHANGE;
			_resourceType = resourceType;
			_consumptionChange = consumptionChange;
			_consumptionPeriod = consumptionPeriod;
		}

		// Cambio de consumo
		int _consumptionChange;

		// Periodo (ms) con que se consume
		int _consumptionPeriod;

		// Recurso consumido
		ResourceType _resourceType;

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