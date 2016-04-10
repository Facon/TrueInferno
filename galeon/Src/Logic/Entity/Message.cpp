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
				// MessageHandler
				luabind::class_<MessageHandler>("MessageHandler"),
				// Root
				luabind::class_<Message>("Message")
				.enum_("MessageType")
				[
					luabind::value("SEND_SOUL_BURN", 0),
					luabind::value("SEND_SOUL_WORK", 1),
					luabind::value("RESOURCES_ASK", 2),
					luabind::value("RESOURCES_INFO", 3),
					luabind::value("RESOURCES_CHANGE", 4),
					luabind::value("RESOURCES_RESERVE", 5),
					luabind::value("RESOURCES_RESERVED", 6),
					luabind::value("RESOURCES_FREE", 7),
					luabind::value("RESOURCES_CLAIM", 8)
				],
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