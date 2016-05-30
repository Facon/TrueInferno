/**
@file Message.cpp
@author Raúl Segura and Asier González
*/

#include "Message.h"

#include "BaseSubsystems/ScriptManager.h"
#include "AI/SoulTask.h"
#include "Logic/ResourcesManager.h"
#include "Logic/Events/EventManager.h"
#include "Logic/SoulsTrialManager.h"
#include "Logic/Entity/BuildingType.h"
#include "Logic/Entity/IconType.h"

namespace Logic
{
	#define RESEND(message) \
		CEventManager::getSingletonPtr()->HandleMessage(message);

	Message::Message() : _type(MessageType::UNASSIGNED)
	{}

	Message::Message(MessageType type) : _type(type)
	{}

	void Message::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				// MessageHandler
				luabind::class_<MessageHandler>("MessageHandler"),
				// Root
				luabind::class_<Message>("Message")
				.enum_("MessageType")
				[
					luabind::value("MT_SEND_SOUL_BURN", Message::SEND_SOUL_BURN),
					luabind::value("MT_SEND_SOUL_WORK", Message::SEND_SOUL_WORK),
					luabind::value("MT_RESOURCES_ASK", Message::RESOURCES_ASK),
					luabind::value("MT_RESOURCES_INFO", Message::RESOURCES_INFO),
					luabind::value("MT_RESOURCES_CHANGE", Message::RESOURCES_CHANGE),
					luabind::value("MT_RESOURCES_RESERVE", Message::RESOURCES_RESERVE),
					luabind::value("MT_RESOURCES_RESERVED", Message::RESOURCES_RESERVED),
					luabind::value("MT_RESOURCES_FREE", Message::RESOURCES_FREE),
					luabind::value("MT_RESOURCES_CLAIM", Message::RESOURCES_CLAIM)
				],
				// Children
				// HellQuartersMessages
				luabind::class_<HellQuartersMessage, Message>("HellQuartersMessage")
				// Atributos
				.def_readwrite("_action", &HellQuartersMessage::_action)
				.def_readwrite("_numSouls", &HellQuartersMessage::_numSouls)
				.def_readwrite("_soulsCategory", &HellQuartersMessage::_soulsCategory)
				// Constructor
				.def(luabind::constructor<HellQuartersMessage::HellQuartersAction, int, SoulsTrialManager::SoulsCategory>())
				// Enum
				.enum_("HellQuartersAction")
				[
					luabind::value("HQA_SEND_SOUL_BURN", HellQuartersMessage::HellQuartersAction::SEND_SOUL_BURN),
					luabind::value("HQA_SEND_SOUL_WORK", HellQuartersMessage::HellQuartersAction::SEND_SOUL_WORK)
				]
			];
	}

	TransformMessage::TransformMessage(const Matrix4& transform) : Message(MessageType::SET_TRANSFORM), _transform(transform)
	{}

	bool TransformMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	PositionMessage::PositionMessage(const Vector3& position) : Message(MessageType::SET_POSITION), _position(position)
	{}

	bool PositionMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	RotationMessage::RotationMessage(const Vector3& rotation) : Message(MessageType::SET_ROTATION), _rotation(rotation)
	{}

	bool RotationMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	DimensionsMessage::DimensionsMessage(const Vector3& dimensions) : Message(MessageType::SET_DIMENSIONS), _dimensions(dimensions)
	{}

	bool DimensionsMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	ColorMessage::ColorMessage(const Vector3& rgb) : Message(MessageType::SET_COLOR), _rgb(rgb)
	{}

	bool ColorMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	MaterialMessage::MaterialMessage(const std::string& materialName) : Message(MessageType::SET_MATERIAL_NAME), _name(materialName)
	{}

	bool MaterialMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	AnimationMessage::AnimationMessage(MessageType type, const std::string& action, bool activated) : Message(type), _action(action), _activated(activated)
	{}

	bool AnimationMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	ControlMessage::ControlMessage(ActionType action, float degreesMoved) : Message(MessageType::CONTROL), _action(action), _degreesMoved(degreesMoved)
	{}

	bool ControlMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	PhysicMessage::PhysicMessage(const Vector3& point) : Message(MessageType::AVATAR_WALK), _point(point)
	{}

	bool PhysicMessage::Dispatch(MessageHandler& handler) const
	{
		return handler.HandleMessage(*this);
	}

	KinematicMoveMessage::KinematicMoveMessage(const Vector3& shift) : Message(MessageType::KINEMATIC_MOVE), _shift(shift)
	{}

	bool KinematicMoveMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	TouchMessage::TouchMessage(MessageType type, const CEntity& entity) : Message(type), _entity(entity)
	{}

	bool TouchMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	DamageMessage::DamageMessage(MessageType type, unsigned int damage) : Message(type), _damage(damage)
	{}

	bool DamageMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	MovePlaceableMessage::MovePlaceableMessage() : Message(MessageType::PLACEABLE_PLACE)
	{}

	MovePlaceableMessage::MovePlaceableMessage(const Vector3& position, bool showFloating) : Message(MessageType::PLACEABLE_FLOAT_TO), _position(position), _showFloating(showFloating)
	{}

	bool MovePlaceableMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	CheckValidPositionPlaceableMessage::CheckValidPositionPlaceableMessage(MessageType type, CEntity* entity, const Vector3& position) : Message(type), _entity(entity), _position(position)
	{}

	bool CheckValidPositionPlaceableMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	GetCostPlaceableMessage::GetCostPlaceableMessage(MessageType type, CEntity* entity) : Message(type), _entity(entity)
	{}

	bool GetCostPlaceableMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	WorkerMessage::WorkerMessage(MessageType type, int change) : Message(type), _change(change)
	{}

	WorkerMessage::WorkerMessage()
	{}

	void WorkerMessage::assembleWorkerAssigned(int change)
	{
		_type = MessageType::WORKER_ASSIGNED;
		_change = change;
	}

	void WorkerMessage::assembleWorkerActivated(int change)
	{
		_type = MessageType::WORKER_ACTIVATED;
		_change = change;
	}

	void WorkerMessage::assembleWorkerAsk()
	{
		_type = MessageType::WORKER_ASK;
	}

	void WorkerMessage::assembleWorkerInfo(int minWorkers, int maxWorkers, int activeWorkers, int assignedWorkers)
	{
		_type = MessageType::WORKER_INFO;
		_minWorkers = minWorkers;
		_maxWorkers = maxWorkers;
		_activeWorkers = activeWorkers;
		_assignedWorkers = assignedWorkers;
	}

	bool WorkerMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	WalkSoulPathMessage::WalkSoulPathMessage(TEntityID target) : Message(MessageType::REQUEST_WALK_SOUL_PATH), _target(target), _path(nullptr)
	{}

	WalkSoulPathMessage::WalkSoulPathMessage(std::vector<Vector3>* const path) : _target(EntityID::UNASSIGNED), _path(path)
	{}

	WalkSoulPathMessage::WalkSoulPathMessage() : Message(MessageType::WALK_SOUL_PATH_FINISHED), _target(EntityID::UNASSIGNED), _path(nullptr)
	{}

	WalkSoulPathMessage::WalkSoulPathMessage(MessageType type) : Message(type), _target(EntityID::UNASSIGNED), _path(nullptr)
	{}

	bool WalkSoulPathMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	HellQuartersMessage::HellQuartersMessage(HellQuartersAction action, int numSouls, SoulsTrialManager::SoulsCategory soulsCategory) :
		Message(MessageType::HELLQUARTERS_REQUEST), _action(action), _numSouls(numSouls), _soulsCategory(soulsCategory)
	{}

	bool HellQuartersMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	SoulSenderMessage::SoulSenderMessage(AI::CSoulTask* task, int numSouls) :
		Message(MessageType::SOUL_SENDER_REQUEST), _task(task), _numSouls(numSouls)
	{}

	bool SoulSenderMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	SoulMessage::SoulMessage(AI::CSoulTask* task) : Message(MessageType::SOUL_REQUEST), _task(task)
	{}

	SoulMessage::SoulMessage() : Message(MessageType::SOUL_RESPONSE), _task(0)
	{}

	bool SoulMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	SoulBurnMessage::SoulBurnMessage(int numSouls, SoulsTrialManager::SoulsCategory soulsCategory) :
		Message(MessageType::FURNACE_BURN_SOULS), _numSouls(numSouls), _soulsCategory(soulsCategory)
	{}

	bool SoulBurnMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	ResourceMessage::ResourceMessage() : Message(MessageType::UNASSIGNED), _resourceType(ResourceType::NONE),
		_quantity(0), _stored(0), _available(0), _max(0),_provided(false), _caller(EntityID::UNASSIGNED)
	{}

	void ResourceMessage::assembleResourcesAsk(const ResourceType& resourceType, TEntityID caller)
	{
		_type = MessageType::RESOURCES_ASK;
		_resourceType = resourceType;
		_caller = caller;
	}

	void ResourceMessage::assembleResourcesInfo(const ResourceType& resourceType, int stored, int available, unsigned int max, bool provided, TEntityID caller)
	{
		_type = MessageType::RESOURCES_INFO;
		_resourceType = resourceType;
		_stored = stored;
		_available = available;
		_max = max;
		_provided = provided;
		_caller = caller;
	}

	void ResourceMessage::assembleResourcesChange(const ResourceType& resourceType, int quantity)
	{
		_type = MessageType::RESOURCES_CHANGE;
		_resourceType = resourceType;
		_quantity = quantity;
	}

	void ResourceMessage::assembleResourcesReserve(const ResourceType& resourceType, int quantity, TEntityID caller)
	{
		_type = MessageType::RESOURCES_RESERVE;
		_resourceType = resourceType;
		_quantity = quantity;
		_caller = caller;
	}

	void ResourceMessage::assembleResourcesReserved(const ResourceType& resourceType, int quantity)
	{
		_type = MessageType::RESOURCES_RESERVED;
		_resourceType = resourceType;
		_quantity = quantity;
	}

	void ResourceMessage::assembleResourcesFree(const ResourceType& resourceType, int quantity)
	{
		_type = MessageType::RESOURCES_FREE;
		_resourceType = resourceType;
		_quantity = quantity;
	}

	void ResourceMessage::assembleResourcesClaim(const ResourceType& resourceType, int quantity)
	{
		_type = MessageType::RESOURCES_CLAIM;
		_resourceType = resourceType;
		_quantity = quantity;
	}

	bool ResourceMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	LogisticsMessage::LogisticsMessage() : Message(MessageType::UNASSIGNED), _resourceType(ResourceType::NONE),
		_resourceQuantity(0), _target(EntityID::UNASSIGNED)
	{}

	void LogisticsMessage::assembleDemandResources(ResourceType resourceType, unsigned int resourceQuantity)
	{
		_type = MessageType::LOGISTICS_DEMAND_RESOURCES;
		_resourceType = resourceType;
		_resourceQuantity = resourceQuantity;
		_target = EntityID::UNASSIGNED;
	}

	void LogisticsMessage::assembleProvideResources(ResourceType resourceType, unsigned int resourceQuantity, const TEntityID& target)
	{
		_type = MessageType::LOGISTICS_PROVIDE_RESOURCES;
		_resourceType = resourceType;
		_resourceQuantity = resourceQuantity;
		_target = target;
	}

	bool LogisticsMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	ToggleMessage::ToggleMessage(LogicRequirement requirement, bool add) :
		Message(MessageType::TOGGLE_REQUEST), _requirement(requirement), _add(add)
	{}

	ToggleMessage::ToggleMessage(bool enabled) :
		Message(MessageType::TOGGLE_INFO), _enabled(enabled)
	{}

	bool ToggleMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	PowerMessage::PowerMessage() :
		Message(MessageType::UNASSIGNED), _caller(EntityID::UNASSIGNED),
		_attach(false), _consumptionUnits(0), _consumptionPeriod(0)
	{}

	void PowerMessage::assemblePowerRequestAttachment(TEntityID caller, int consumptionUnits, int consumptionPeriod)
	{
		_type = MessageType::POWER_REQUEST_ATTACHMENT;
		_caller = caller;
		_consumptionUnits = consumptionUnits;
		_consumptionPeriod = consumptionPeriod;
	}

	void PowerMessage::assemblePowerAttachmentInfo(TEntityID caller, bool attach, int consumptionUnits, int consumptionPeriod)
	{
		_type = MessageType::POWER_ATTACHMENT_INFO;
		_caller = caller;
		_attach = attach;
		_consumptionUnits = consumptionUnits;
		_consumptionPeriod = consumptionPeriod;
	}

	bool PowerMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);
		
		return handler.HandleMessage(*this);
	}

	ConsumptionMessage::ConsumptionMessage() : Message(MessageType::UNASSIGNED), _resourceType(ResourceType::NONE),
		_consumptionChange(0), _consumptionPeriod(0)
	{}

	void ConsumptionMessage::assembleConsumptionStart(ResourceType resourceType)
	{
		_type = MessageType::CONSUMPTION_START;
		_resourceType = resourceType;
	}

	void ConsumptionMessage::assembleConsumptionStop(ResourceType resourceType)
	{
		_type = MessageType::CONSUMPTION_STOP;
		_resourceType = resourceType;
	}

	void ConsumptionMessage::assembleConsumptionStopped(ResourceType resourceType)
	{
		_type = MessageType::CONSUMPTION_STOPPED;
		_resourceType = resourceType;
	}

	void ConsumptionMessage::assembleConsumptionChange(ResourceType resourceType, int consumptionChange, int consumptionPeriod)
	{
		_type = MessageType::CONSUMPTION_CHANGE;
		_resourceType = resourceType;
		_consumptionChange = consumptionChange;
		_consumptionPeriod = consumptionPeriod;
	}

	bool ConsumptionMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

	IconMessage::IconMessage(IconType::IconType icon) : Message(MessageType::ICON), _icon(icon), _iconReplacement(IconType::NONE)
	{}

	IconMessage::IconMessage(MessageType type, IconType::IconType icon) : Message(type), _icon(icon), _iconReplacement(IconType::NONE)
	{}

	IconMessage::IconMessage(MessageType type, IconType::IconType icon, IconType::IconType iconReplacement) : Message(type), _icon(icon), _iconReplacement(iconReplacement)
	{}

	bool IconMessage::Dispatch(MessageHandler& handler) const
	{
		RESEND(*this);

		return handler.HandleMessage(*this);
	}

} // namespace Logic