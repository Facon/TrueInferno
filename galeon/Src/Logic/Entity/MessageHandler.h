#ifndef __LOGIC_MessageHandler_H
#define __LOGIC_MessageHandler_H

namespace Logic
{
    // Predefiniciones
    class Message;
    class TransformMessage;
    class ScaleMessage;
    class AnimationMessage;
    class ControlMessage;
    class MoveMessage;
    class TouchMessage;
    class DamageMessage;

	class MessageHandler
	{
	public:
		virtual bool HandleMessage(const TransformMessage& msg)
		{}

		virtual bool HandleMessage(const ScaleMessage& msg)
		{}
		
		virtual bool HandleMessage(const AnimationMessage& msg)
		{}
		
		virtual bool HandleMessage(const ControlMessage& msg)
		{}

        virtual bool HandleMessage(const MoveMessage& msg)
        {}

        virtual bool HandleMessage(const TouchMessage& msg)
        {}

        virtual bool HandleMessage(const DamageMessage& msg)
        {}
	};
} // namespace Logic

#endif // __LOGIC_MessageHandler_H