#ifndef __LOGIC_MessageHandler_H
#define __LOGIC_MessageHandler_H

#include <stdexcept>

namespace Logic
{
	class MessageHandler
	{
	public:
		virtual void HandleMessage(const Message& msg)
		{}
		
		virtual void HandleMessage(const TransformMessage& msg)
		{}
		
		virtual void HandleMessage(const AnimationMessage& msg)
		{}
		
		virtual void HandleMessage(const ControlMessage& msg)
		{}
	}
} // namespace Logic

#endif // __LOGIC_MessageHandler_H