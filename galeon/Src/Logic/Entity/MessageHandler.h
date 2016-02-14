#ifndef __LOGIC_MessageHandler_H
#define __LOGIC_MessageHandler_H

namespace Logic
{
    // Predefiniciones
    class Message;
    class TransformMessage;
    class DimensionsMessage;
	class ColorMessage;
	class MaterialMessage;
    class AnimationMessage;
    class ControlMessage;
	class PhysicMessage;
    class MoveMessage;
    class TouchMessage;
    class DamageMessage;
	class PlaceMessage;
	class WorkerMessage;
	
	class MessageHandler
	{
	public:
		virtual bool HandleMessage(const TransformMessage& msg)
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

        virtual bool HandleMessage(const MoveMessage& msg)
        { return false; }

        virtual bool HandleMessage(const TouchMessage& msg)
        { return false; }

        virtual bool HandleMessage(const DamageMessage& msg)
        { return false; }

		virtual bool HandleMessage(const PlaceMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const WorkerMessage& msg)
		{ return false; }
	};
} // namespace Logic

#endif // __LOGIC_MessageHandler_H