//---------------------------------------------------------------------------
// AIManager.cpp
//---------------------------------------------------------------------------

/**
@file AIManager.cpp

Contiene la implementación del gestor de eventos del juego. Los eventos serán
creados desde un script de LUA, procesados (en caso de que sea necesario)
y encolados hasta que llegue el momento de su lanzamiento.

@see Logic::CScriptManager

@author Álvaro Valera
@date Abril, 2016
*/

#include <cassert>

#include "AIManager.h"
#include "Logic/Events/BuildingDestructionEvent.h"
#include "Logic/Events/EndGameEvent.h"
#include "Logic/Events/SoulsSpeedReductionEvent.h"
#include "Logic/Events/TutorialEvent.h"
#include "Application/GaleonApplication.h"
#include "BaseSubsystems/ScriptManager.h"
#include "Logic/TimeManager.h"
#include "Logic/Entity/Message.h"

namespace AI {

	CAIManager* CAIManager::_instance = 0;

	//--------------------------------------------------------

	CAIManager::CAIManager()
	{
		_instance = this;

	} // CAIManager

	//--------------------------------------------------------

	CAIManager::~CAIManager()
	{
		assert(_instance);
		_instance = 0;

		// Borramos todos los dioses
		for (auto it = _gods.begin(); it != _gods.end(); ++it){
			delete(it->second);
			it->second = nullptr;
		}

		// Limpiamos las estructuras
		_gods.clear();
		_ranking.clear();

		// Apuntamos theBoss a null porque su memoria ya debería estar liberada en _gods
		_theBoss = nullptr;

	} // ~CAIManager

	//--------------------------------------------------------

	bool CAIManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CAIManager no permitida!");

		new CAIManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CAIManager::Release()
	{
		assert(_instance && "Logic::CAIManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CAIManager::tick(unsigned int msecs)
	{
		ScriptManager::CScriptManager::GetPtrSingleton()->executeProcedure("tickAIManager", msecs);

	} // tick

	//--------------------------------------------------------

	bool CAIManager::loadAIScript(const std::string& filename)
	{
		if (!ScriptManager::CScriptManager::GetPtrSingleton()->loadScript(filename.c_str()))
			return false;

		return true;

	} // loadEventsScript

	//--------------------------------------------------------

	bool CAIManager::open() {
		luaRegister();
		loadAIScript("AIManager.lua");

		return true;

	} // open

	//--------------------------------------------------------

	void CAIManager::close() {
	} // close

	//--------------------------------------------------------

	void CAIManager::luaRegister() {
		// AIManager.
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CAIManager>("CAIManager")
				.def("getElapsedTime", &CAIManager::getElapsedTime)
				.def("addGod", &CAIManager::addGod)
				.scope
				[
					luabind::def("getSingletonPtr", &CAIManager::getSingletonPtr)
				]
			];

	} // luaRegister

	//--------------------------------------------------------

	long CAIManager::getElapsedTime() const {
		return Logic::TimeManager::getSingletonPtr()->getElapsedTime();
	}

	void CAIManager::addGod(const std::string& name, bool isBoss){
		// Creamos el nuevo dios
		CGod* newGod = new CGod(name, isBoss);
		
		// Lo insertamos en el contenedor de dioses
		_gods[name] = newGod;

		// Si es el jefe lo marcamos aparte
		if (isBoss) {
			_theBoss = newGod;
		}

		// Si no, lo añadimos al ranking
		else { // isBoss == false
			_ranking.insert(newGod);
		}
	}

	const std::map<std::string, CGod*> CAIManager::getGodRanking() {
		return _gods;
	}

	bool CAIManager::HandleMessage(const Message& msg)
	{
		assert(!"Estás enviando un Message sin tipo.");

		return false;
	}

	bool CAIManager::HandleMessage(const TransformMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const PositionMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const RotationMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const DimensionsMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const ColorMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const MaterialMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const AnimationMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const ControlMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const PhysicMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const TouchMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const DamageMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const WorkerMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const WalkSoulPathMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const HellQuartersMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const MovePlaceableMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const SoulSenderMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const SoulMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const CheckValidPositionPlaceableMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const NumberMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const ResourceMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const GetCostPlaceableMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const LogisticsMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

	bool CAIManager::HandleMessage(const ToggleMessage& msg)
	{
		//std::cout << msg._type << "\n";

		return false;
	}

} // namespace Logic
