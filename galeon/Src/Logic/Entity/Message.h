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

#include "BaseSubsystems/Math.h"
#include "MessageHandler.h"

// Predeclaraciones
namespace Logic
{
	typedef unsigned int TEntityID;
	class CEntity;
	class CPlaceable;
	enum LogicRequirement;
	
	namespace ResourceType
	{
		enum ResourceType;
	}

	class CSoulsTrialManager;
	
	namespace SoulsTrialManager
	{
		enum SoulsCategory;
	}
};

namespace AI
{
	class CSoulTask;
}

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
			ICON,
		};

		MessageType _type;
		
		Message();
		Message(MessageType type);

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

	// SET_TRANSFORM
	class TransformMessage : public Message
	{
	public:
		const Matrix4& _transform;

		TransformMessage(const Matrix4& transform);
		
		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// SET_POSITION
	class PositionMessage : public Message
	{
	public:
		const Vector3& _position;

		PositionMessage(const Vector3& position);

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// SET_ROTATION
	class RotationMessage : public Message
	{
	public:
		// Rotación con respecto a los ejes X(pitch), Y(yaw) y Z(roll).
		const Vector3& _rotation;

		RotationMessage(const Vector3& rotation);

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// SET_DIMENSIONS
	class DimensionsMessage : public Message
	{
	public:
		const Vector3& _dimensions;

		DimensionsMessage(const Vector3& dimensions);

		bool Dispatch(MessageHandler& handler) const;
	};
	
	// SET_COLOR
	class ColorMessage : public Message
	{
	public:
		const Vector3& _rgb;

		ColorMessage(const Vector3& rgb);

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// SET_MATERIAL_NAME
	class MaterialMessage : public Message
	{
	public:
		const std::string _name; // Material doesn't work well using references in Ogre :(

		MaterialMessage(const std::string& materialName);

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// SET_ANIMATION, STOP_ANIMATION
	class AnimationMessage : public Message
	{
	public:
        const std::string& _action;
        bool _activated;

		AnimationMessage(MessageType type, const std::string& action, bool activated);

		virtual bool Dispatch(MessageHandler& handler) const;
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

		ControlMessage(ActionType action = ActionType::UNASSIGNED, float degreesMoved = 0.0f);

		virtual bool Dispatch(MessageHandler& handler) const;
	};
	
	// AVATAR_WALK
    class PhysicMessage : public Message
    {
    public:
        const Vector3& _point;

		PhysicMessage(const Vector3& point);

		virtual bool Dispatch(MessageHandler& handler) const;
    };

	// KINEMATIC_MOVE
	class KinematicMoveMessage : public Message
	{
	public:
        const Vector3& _shift;

		KinematicMoveMessage(const Vector3& shift);

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// TOUCHED, UNTOUCHED
	class TouchMessage : public Message
	{
	public:
		const CEntity& _entity;
		
		TouchMessage(MessageType type, const CEntity& entity);

		virtual bool Dispatch(MessageHandler& handler) const;
	};
	
	// SWITCH,
	// DAMAGED
	class DamageMessage : public Message
	{
	public:
		unsigned int _damage;
		
		DamageMessage(MessageType type, unsigned int damage);

		virtual bool Dispatch(MessageHandler& handler) const;
	};
	
	// PLACEABLE_FLOAT_TO, PLACEABLE_PLACE
	class MovePlaceableMessage : public Message
	{
	public:
		const Vector3 _position;
		bool _showFloating;
		// Coloca el placeable en la posición actual
		MovePlaceableMessage();
		
		// Hace flotar al placeable hasta la posición dada
		MovePlaceableMessage(const Vector3& position, bool showFloating);

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// PLACEABLE_CHECKPOSITION
	class CheckValidPositionPlaceableMessage : public Message
	{
	public:
		CheckValidPositionPlaceableMessage(MessageType type, CEntity* entity, const Vector3& position);

		CEntity* _entity;
		const Vector3& _position;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	class GetCostPlaceableMessage : public Message
	{
	public:
		GetCostPlaceableMessage(MessageType type, CEntity* entity);

		CEntity* _entity;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	/** Mensajes de trabajadores:
	- WORKER_ASSIGNED: Cantidad a aumentar o disminuir de trabajadores asignados al edificio (i.e. un trabajador va a ir a trabajar al edificio)
	- WORKER_ACTIVATED: Cantidad a aumentar o disminuir de trabajadores activados en el edificio (i.e. un trabajador ha llegado al edificio y empieza a trabajar)
	*/
	class WorkerMessage : public Message
	{
	public:
		WorkerMessage(MessageType type, int change);

		// Cambio solicitado
		unsigned int _change;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// TODO Remember to discuss if we should separate this in 2 classes
	// REQUEST_WALK_SOUL_PATH, RETURN_WALK_SOUL_PATH, PERFORM_WALK_SOUL_PATH, WALKING_SOUL_PATH_FINISHED
	class WalkSoulPathMessage : public Message
	{
	public:
		// Petición de ruta (REQUEST_WALK_SOUL_PATH)
		WalkSoulPathMessage(TEntityID target);
		
		// Respuesta/orden con la ruta (RETURN_WALK_SOUL_PATH, PERFORM_WALK_SOUL_PATH)
		WalkSoulPathMessage(std::vector<Vector3>* const path);

		// Ruta finalizada (WALKING_SOUL_PATH_FINISHED)
		WalkSoulPathMessage();

		WalkSoulPathMessage(MessageType type);

		TEntityID _target;
		std::vector<Vector3>* _path;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// REQUEST_SEND_SOUL_WORK, REQUEST_SEND_SOUL_BURN
	class HellQuartersMessage : public Message
	{
	public:

		enum HellQuartersAction {
			SEND_SOUL_BURN,
			SEND_SOUL_WORK,
		};

		HellQuartersMessage(HellQuartersAction action, int numSouls, SoulsTrialManager::SoulsCategory soulsCategory);

		HellQuartersAction _action;
		unsigned int _numSouls;
		SoulsTrialManager::SoulsCategory _soulsCategory;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// SOUL_SENDER_REQUEST, SOUL_SENDER_RESPONSE
	class SoulSenderMessage : public Message
	{
	public:
		SoulSenderMessage(AI::CSoulTask* task, int numSouls);

		//std::unique_ptr<AI::CSoulTask> _task;
		AI::CSoulTask* _task;
		unsigned int _numSouls;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// SOUL_REQUEST, SOUL_RESPONSE
	class SoulMessage : public Message
	{
	public:
		SoulMessage(AI::CSoulTask* task);
		SoulMessage();

		//std::unique_ptr<AI::CSoulTask> _task;
		AI::CSoulTask* _task;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	// FURNACE_BURN_SOULS
	class SoulBurnMessage : public Message
	{
	public:
		SoulBurnMessage(int numSouls, SoulsTrialManager::SoulsCategory soulsCategory);

		int _numSouls;
		SoulsTrialManager::SoulsCategory _soulsCategory;

		virtual bool Dispatch(MessageHandler& handler) const;
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
		ResourceMessage();

		/** RESOURCES_ASK: Solicita información sobre el recurso del tipo dado y el ID de la entidad que envía la solicitud */
		void assembleResourcesAsk(const ResourceType::ResourceType& resourceType, TEntityID caller);

		/** RESOURCES_INFO: Devuelve la cantidad real, disponible y máxima del recurso indicado, si el recurso se provee externamente y el ID de la entidad que tiene los recursos */
		void assembleResourcesInfo(const ResourceType::ResourceType& resourceType, int stored, int available, unsigned int max, bool provided, TEntityID caller);

		/** RESOURCES_CHANGE: Solicita el cambio (positivo/negativo) en la cantidad de recursos del tipo dado */
		void assembleResourcesChange(const ResourceType::ResourceType& resourceType, int quantity);

		/** RESOURCES_RESERVE: Intenta reservar todo lo posible hasta la cantidad de recursos indicada del tipo dado para la entidad indicada. 
		No cambia la cantidad real almacenada sino la cantidad disponible para evitar que otras solicitudes simultáneas nos quiten nuestra reserva.
		El que reserva recursos contrae la responsabilidad de liberarlos (RESOURCES_FREE) o reclamarlos (RESOURCES_CLAIM).
		Tras la reserva la entidad indicada recibirá un mensaje RESOURCES_RESERVED con la cantidad finalmente reservada.
		*/
		void assembleResourcesReserve(const ResourceType::ResourceType& resourceType, int quantity, TEntityID caller);

		/** RESOURCES_RESERVED: Informa de la cantidad de recursos finalmente reservada del tipo dado.
		No cambia la cantidad real almacenada sino la cantidad disponible para evitar que otras solicitudes simultáneas nos quiten nuestra reserva.
		El que reserva recursos contrae la responsabilidad de liberarlos (RESOURCES_FREE) o reclamarlos (RESOURCES_CLAIM).
		*/
		void assembleResourcesReserved(const ResourceType::ResourceType& resourceType, int quantity);

		/** RESOURCES_FREE: Libera la cantidad de recursos reservada del tipo dado para que otras solicitudes puedan volverlo a reservar */
		void assembleResourcesFree(const ResourceType::ResourceType& resourceType, int quantity);

		/** RESOURCES_CLAIM: Reclama (i.e. libera la reserva y modifica) la cantidad de recursos del tipo dado para efectivamente consumir lo reservado */
		void assembleResourcesClaim(const ResourceType::ResourceType& resourceType, int quantity);

		ResourceType::ResourceType _resourceType;
		int _quantity;
		int _stored;
		int _available;
		int _max;
		bool _provided;
		TEntityID _caller;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	/** Mensajes de comunicación relacionados con la logística de recursos */
	class LogisticsMessage : public Message
	{
	public:
		LogisticsMessage();

		// LOGISTICS_DEMAND_RESOURCES: Solicita buscar la cantidad de recursos indicada del tipo dado
		void assembleDemandResources(ResourceType::ResourceType resourceType, unsigned int resourceQuantity);

		// LOGISTICS_PROVIDE_RESOURCES: Obliga a proveer la cantidad indicada del recurso dado al objetivo proporcionado
		void assembleProvideResources(ResourceType::ResourceType resourceType, unsigned int resourceQuantity, const TEntityID& target);

		ResourceType::ResourceType _resourceType;
		unsigned int _resourceQuantity;
		TEntityID _target;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	/** Mensajes relacionados con el habilitado/deshabilitado lógico de una entidad */
	class ToggleMessage : public Message
	{
	public:
		// TOGGLE_REQUEST: Solicita añadir o eliminar un requisito para permitir el habilitado de la entidad
		ToggleMessage(LogicRequirement requirement, bool add);

		// TOGGLE_INFO: Informa de un cambio en el habilitado o deshabilitado de la entidads
		ToggleMessage(bool enabled);

		// Criterio por el que se quiere habilitar/deshabilitar
		//std::string _criterion;

		// Requisito lógico por el que se quiere habilitar o deshabilitar
		LogicRequirement _requirement;

		// Flag a true para indicar que se quiere añadir un requisito al habilitado de la entidad
		bool _add;

		// Flag a true si se está informando del habilitado de la entidad o false para el deshbilitado
		bool _enabled;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	/** Mensajes para el protocolo de comunicación entre generadores y consumidores de energía: 
		- POWER_REQUEST_ATTACHMENT: Petición de conexión a un generador
		- POWER_ATTACHMENT_INFO: Comunicación de consumidor conectado/desconectado
	*/
	class PowerMessage : public Message
	{
	public:
		PowerMessage();

		// POWER_REQUEST_ATTACHMENT: Solicita la conexión de un consumidor nuevo (la desconexión nunca se solicita, se envía directamente POWER_ATTACHMENT_INFO para informar del cambio)
		void assemblePowerRequestAttachment(TEntityID caller, int consumptionUnits, int consumptionPeriod);

		// POWER_ATTACHMENT_INFO: Informa de la conexión o desconexión de un consumidor
		void assemblePowerAttachmentInfo(TEntityID caller, bool attach, int consumptionUnits, int consumptionPeriod);

		// Entidad a conectar/desconectar
		TEntityID _caller;

		// Flag a true si se quiere informar de una conexión o false para desconexión
		bool _attach;

		// Unidades de consumo de la entidad
		int _consumptionUnits;

		// Periodo de consumo de la entidad
		int _consumptionPeriod;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	class ConsumptionMessage : public Message
	{
	public:
		ConsumptionMessage();

		// CONSUMPTION_START: Solicita el comienzo de los ciclos de consumo (i.e. se ha conectado el primer consumidor)
		void assembleConsumptionStart(ResourceType::ResourceType resourceType);

		// CONSUMPTION_STOP: Solicita el fin de los ciclos de consumo (i.e. se ha desconectado el último consumidor)
		void assembleConsumptionStop(ResourceType::ResourceType resourceType);

		// CONSUMPTION_STOPPED: Informa de que los ciclos de consumo han parado (i.e. se han acabado los recursos)
		void assembleConsumptionStopped(ResourceType::ResourceType resourceType);

		// CONSUMPTION_CHANGE: Informa de la variación deseada en el consumo
		void assembleConsumptionChange(ResourceType::ResourceType resourceType, int consumptionChange, int consumptionPeriod);

		// Cambio de consumo
		int _consumptionChange;

		// Periodo (ms) con que se consume
		int _consumptionPeriod;

		// Recurso consumido
		ResourceType::ResourceType _resourceType;

		virtual bool Dispatch(MessageHandler& handler) const;
	};

	namespace IconType
	{
		enum IconType
		{
			OK = 0,
			BURNING, // WTF?
			SHOVEL,
			CLOSE,
			ROCKS,
			STAR, // WTF?
			NOT_OK,
			FLAMES,
			SOUL_PATH,
			COMPASS_ROSE,
			FURNACE,
			TOXIC, // WTF?
			HELLQUARTERS, // WTF?
			MINE, // WTF?
			CROSS_SWORD, // WTF?
			SOUL, // WTF?
			WIFI, // WTF?
			TOXIC2, // WTF?
			TOXIC3, // WTF?
			REFINERY, 
			REPAIR,
			RESEARCH_LABS,
			POWER_GENERATOR,
			CLOCK,
			JUDGEMENT,
			COGS, // WTF?
			WAREHOUSE,
		};
	}

	class IconMessage : public Message
	{
	public:
		IconType::IconType _icon;

		IconMessage(IconType::IconType icon);

		virtual bool Dispatch(MessageHandler& handler) const;
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