#ifndef __LOGIC_MessageHandler_H
#define __LOGIC_MessageHandler_H

#include "Message.h"

namespace Logic
{
    // Predefiniciones
    class Message;
    class TransformMessage;
    class DimensionsMessage;
    class AnimationMessage;
    class ControlMessage;
	class PhysicMessage;
    class MoveMessage;
    class TouchMessage;
    class DamageMessage;

	class MessageHandler
	{
	public:
		virtual bool HandleMessage(const TransformMessage& msg)
		{ return false; }

		virtual bool HandleMessage(const DimensionsMessage& msg)
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
	};
} // namespace Logic

#endif // __LOGIC_MessageHandler_H