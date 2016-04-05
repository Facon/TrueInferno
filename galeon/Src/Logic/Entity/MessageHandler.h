#ifndef __LOGIC_MessageHandler_H
#define __LOGIC_MessageHandler_H

namespace Logic
{
    // Predefinición de cada tipo de mensaje
    class Message;
	class TransformMessage;
	class PositionMessage;
	class RotationMessage;
	class DimensionsMessage;
	class ColorMessage;
	class MaterialMessage;
    class AnimationMessage;
    class ControlMessage;
	class PhysicMessage;
	class KinematicMoveMessage;
    class TouchMessage;
    class DamageMessage;
	class WorkerMessage;
	class WalkSoulPathMessage;
	class HellQuartersMessage;
	class MovePlaceableMessage;
	class SoulSenderMessage;
	class SoulMessage;
	class CheckValidPositionPlaceableMessage;
	class MoveMessage;
	class NumberMessage;
	class ResourceMessage;
	class GetCostPlaceableMessage;
	class LogisticsMessage;
	class ToggleMessage;
	class PowerMessage;

	class MessageHandler
	{
	public:

		virtual bool HandleMessage(const TransformMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const PositionMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const RotationMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const DimensionsMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const ColorMessage& msg)
		{ return false;	}

		virtual bool HandleMessage(const MaterialMessage& msg)
		{ return false; }
		
		virtual bool HandleMessage(const AnimationMessage& msg)
		{ return false; }
		
		virtual bool HandleMessage(const ControlMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const PhysicMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const KinematicMoveMessage& msg)
        { return false; }

        virtual bool HandleMessage(const TouchMessage& msg)
        { return false; }

        virtual bool HandleMessage(const DamageMessage& msg)
        { return false; }

		virtual bool HandleMessage(const WorkerMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const WalkSoulPathMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const HellQuartersMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const MovePlaceableMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const SoulSenderMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const SoulMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const CheckValidPositionPlaceableMessage& msg)
		{ return false; }

        virtual bool HandleMessage(const MoveMessage& msg)
        { return false; }

		virtual bool HandleMessage(const NumberMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const ResourceMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const GetCostPlaceableMessage& msg)
		{ return false;  }

		virtual bool HandleMessage(const LogisticsMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const ToggleMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const PowerMessage& msg)
		{ return false; }
	};
} // namespace Logic

#endif // __LOGIC_MessageHandler_H