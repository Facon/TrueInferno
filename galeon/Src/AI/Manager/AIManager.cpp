//---------------------------------------------------------------------------
// AIManager.cpp
//---------------------------------------------------------------------------

/**
@file AIManager.cpp

Contiene la implementaci�n del gestor de eventos del juego. Los eventos ser�n
creados desde un script de LUA, procesados (en caso de que sea necesario)
y encolados hasta que llegue el momento de su lanzamiento.

@see Logic::CScriptManager

@author �lvaro Valera
@date Abril, 2016
*/

#include <cassert>
#include <iostream>
#include <algorithm>

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
		_timeSinceLastRankingDisplay = 0;

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

		// Apuntamos theBoss a null porque su memoria ya deber�a estar liberada tras vaciar _gods
		_theBoss = nullptr;

	} // ~CAIManager

	//--------------------------------------------------------

	bool CAIManager::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CAIManager no permitida!");

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
		assert(_instance && "Logic::CAIManager no est� inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CAIManager::tick(unsigned int msecs)
	{
		// Tick Lua AIManager
		ScriptManager::CScriptManager::GetPtrSingleton()->executeProcedure("tickAIManager", msecs);

		// Tick gods
		for (auto it = _gods.begin(); it != _gods.end(); ++it){
			(it->second)->tick(msecs);
		}

		// TODO: Quitar en cuanto se muestre el ranking en la GUI
		_timeSinceLastRankingDisplay += msecs;
		if (_timeSinceLastRankingDisplay >= 5000){
			std::cout << "Ranking" << std::endl;
			
			std::vector<CGod*> ranking = getGodRanking();

			for (auto it = ranking.begin(); it != ranking.end(); ++it)
				std::cout << (*it)->getName() << ": " << (*it)->getScore() << std::endl;
			std::cout << std::endl;

			_timeSinceLastRankingDisplay = 0;
		}
		
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

		// TODO Leer valores de fichero de configuraci�n
		int base = 1000;
		assignGodTargetScores(base, (int) (base * 0.25));

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
				.def("addGod", &CAIManager::addGod)
				.scope
				[
					luabind::def("getSingletonPtr", &CAIManager::getSingletonPtr)
				]
			];

	} // luaRegister

	//--------------------------------------------------------

	void CAIManager::addGod(const std::string& name, bool isBoss){
		// Creamos el nuevo dios
		CGod* newGod = new CGod(name, isBoss);
		
		// Lo insertamos en el contenedor de dioses
		_gods[name] = newGod;

		// Si es el jefe lo marcamos aparte
		if (isBoss) {
			_theBoss = newGod;
		}

		// Si no, lo a�adimos al ranking
		else { // isBoss == false
			_ranking.push_back(newGod);
		}
	}

	bool CAIManager::eliminateGod(const std::string& name){
		// Localizamos al dios
		CGod* god = _gods[name];
		
		// Chequeamos que el dios exista
		if (god == nullptr){
			assert(false && "God can't be eliminated because it's not in the game");
			return false;
		}

		// No permitimos que se elimine al jefe
		if (god->isBoss()){
			assert(false && "Can't eliminate boss god");
			return false;
		}
			
		// Notificamos al dios que est� eliminado en C++
		god->eliminate();

		// Notificamos al dios que est� eliminado en Lua
		ScriptManager::CScriptManager::GetPtrSingleton()->executeProcedure("eliminateGodAIManager", name);

		// TODO �Lo eliminamos del ranking o de la lista de dioses?

		return true;
	}

	/** Funci�n para ordenar dioses en orden de score descendiente */
	struct st_godScoreCompare {
		bool operator() (CGod* lhs, CGod* rhs)
		{
			return lhs->getScore() > rhs->getScore();
		}
	} godScoreCompare;

	std::vector<CGod*> CAIManager::getGodRanking() {
		// Reordenamos los dioses por puntuaci�n
		std::sort(_ranking.begin(), _ranking.end(), godScoreCompare);

		return _ranking;
	}

	CGod* CAIManager::getWorstActiveGod(){
		// Obtenemos el ranking actualizado
		std::vector<CGod*> ranking = getGodRanking();

		// Desde el final (i.e. el dios con peor puntuaci�n) buscamos al primer dios activo
		for (auto it = ranking.rbegin(); it != ranking.rend(); ++it){
			if (!(*it)->isEliminated())
				return (*it);
		}

		// Si llegamos al final es que no hab�a ning�n dios activo en el ranking
		return nullptr;
	}

	void CAIManager::startNextRound(){
		// No hace falta eliminar dios porque ya se ha hecho desde el GameManager

		// Asignamos nueva puntuaci�n objetivo a los dioses
		// TODO Leer externamente o definir f�rmula de incremento
		int base = getGodRanking().front()->getScore() + 1000;
		assignGodTargetScores(base, (int) (base * 0.25));
	}

	bool CAIManager::HandleMessage(const Message& msg)
	{
		assert(!"Est�s enviando un Message sin tipo.");

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

	bool CAIManager::HandleMessage(const SoulBurnMessage& msg)
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

	void CAIManager::assignGodTargetScores(int baseScore, int maxDifference){
		for (auto it = _gods.begin(); it != _gods.end(); ++it){
			int randomDifference = - maxDifference + std::rand() % (2 * maxDifference);
			it->second->setTargetScore(baseScore + randomDifference);
		}
	}

} // namespace Logic
