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
				.def_readwrite("_action", &HellQuartersMessage::_action)
				.def(luabind::constructor<HellQuartersMessage::HellQuartersAction>())
				.enum_("HellQuartersAction")
				[
					luabind::value("HQA_SEND_SOUL_BURN", HellQuartersMessage::HellQuartersAction::SEND_SOUL_BURN),
					luabind::value("HQA_SEND_SOUL_WORK", HellQuartersMessage::HellQuartersAction::SEND_SOUL_WORK)
				]
			];
	}

} // namespace Logic