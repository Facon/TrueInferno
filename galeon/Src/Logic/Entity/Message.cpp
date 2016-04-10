/**
@file Message.cpp
@author Raúl Segura
*/

#include "Message.h"

namespace Logic
{

	void Message::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				// Root
				luabind::class_<MessageHandler>("MessageHandler"),
				luabind::class_<Message>("Message"),
				// Children
				// HellQuartersMessages
				luabind::class_<HellQuartersMessage, Message>("HellQuartersMessage")
				.def_readwrite("_action", &HellQuartersMessage::_action)
				.def(luabind::constructor<HellQuartersMessage::HellQuartersAction>())
				.enum_("HellQuartersAction")
				[
					luabind::value("SEND_SOUL_BURN", 0),
					luabind::value("SEND_SOUL_WORK", 1)
				]
			];
	}

} // namespace Logic