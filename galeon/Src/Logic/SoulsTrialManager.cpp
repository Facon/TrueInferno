//---------------------------------------------------------------------------
// SoulsTrialManager.cpp
//---------------------------------------------------------------------------

/**
@file SoulsTrialManager.cpp

Contiene la implementaci�n del gestor del Juicio de Almas.

@see Logic::CSoulsTrialManager

@author Ra�l Segura
@date Abril, 2016
*/

#include "SoulsTrialManager.h"
#include <cassert>
#include <cstdio>
#include <algorithm>

#include "BaseSubsystems/Math.h"
#include "Logic/BuildingManager.h"

#include "Logic/Entity/Components/Soul.h"
#include "BaseSubsystems/ScriptManager.h"

namespace Logic {

	CSoulsTrialManager* CSoulsTrialManager::_instance = 0;

	//--------------------------------------------------------

	CSoulsTrialManager::CSoulsTrialManager()
	{
		_instance = this;

	} // CSoulsTrialManager

	//--------------------------------------------------------

	CSoulsTrialManager::~CSoulsTrialManager()
	{
		assert(_instance);
		_instance = nullptr;

	} // ~CSoulsTrialManager

	//--------------------------------------------------------

	bool CSoulsTrialManager::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CSoulsTrialManager no permitida!");

		new CSoulsTrialManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CSoulsTrialManager::Release()
	{
		assert(_instance && "Logic::CSoulsTrialManager no est� inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CSoulsTrialManager::spawn(const Map::CEntity *managerInfo)
	{
		if (managerInfo->hasAttribute("minSoulsGenerationTimeSeconds"))
			_minSoulsGenerationTime = managerInfo->getIntAttribute("minSoulsGenerationTimeSeconds") * 1000;

		if (managerInfo->hasAttribute("maxSoulsGenerationTimeSeconds"))
			_maxSoulsGenerationTime = managerInfo->getIntAttribute("maxSoulsGenerationTimeSeconds") * 1000;

		if (managerInfo->hasAttribute("minSoulsPerGroup"))
			_minSoulsPerGroup = managerInfo->getIntAttribute("minSoulsPerGroup");

		if (managerInfo->hasAttribute("maxSoulsPerGroup"))
			_maxSoulsPerGroup = managerInfo->getIntAttribute("maxSoulsPerGroup");

	} // Spawn

	//--------------------------------------------------------

	void CSoulsTrialManager::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
		[
			luabind::class_<CSoulsTrialManager>("CSoulsTrialManager")
			.enum_("SoulsCategory")
			[
				luabind::value("STC_UNKNOWN", SoulsTrialManager::SoulsCategory::UNKNOWN),
				luabind::value("STC_HEAVY", SoulsTrialManager::SoulsCategory::HEAVY),
				luabind::value("STC_WASTED", SoulsTrialManager::SoulsCategory::WASTED),
				luabind::value("STC_LIGHT", SoulsTrialManager::SoulsCategory::LIGHT)
			]
		];

	} // luaRegister

	//--------------------------------------------------------

	void CSoulsTrialManager::tick(unsigned long msecs)
	{
		_timeSinceLastSoulsGeneration += msecs;

		if (_timeSinceLastSoulsGeneration >= _timeForNextSoulsGeneration)
		{
			// TODO Borrar cuando funcione el Juicio de Almas
			//std::cout << "pre souls = [" << _souls[0] << "," << _souls[1] << "," << _souls[2] << "," << _souls[3] << "]" << std::endl; //

			processNewGroupOfSouls();
			// TODO �Reproducimos alg�n sonido o notificaci�n de almas nuevas?

			// TODO Borrar cuando funcione el Juicio de Almas
			//std::cout << "mid souls = [" << _souls[0] << "," << _souls[1] << "," << _souls[2] << "," << _souls[3] << "]" << std::endl; //
			//unsigned int toWork[] = { 2, 1, 2, 1 }; //
			//unsigned int toBurn[] = { 2, 2, 2, 1 }; //
			//SoulsCategory category = createSouls(toWork, toBurn); //
			//std::cout << "create si o no = " << (category == SoulsCategory::NONE) << std::endl; //
			//std::cout << "pst souls = [" << _souls[0] << "," << _souls[1] << "," << _souls[2] << "," << _souls[3] << "]" << std::endl; //

			_timeForNextSoulsGeneration = Math::random(_minSoulsGenerationTime, _maxSoulsGenerationTime);
			_timeSinceLastSoulsGeneration = 0;
		}

	} // tick

	//--------------------------------------------------------

	void CSoulsTrialManager::levelUp()
	{
		if (_judgeLevel == JUDGE_MAX_LEVEL) return;
		_judgeLevel++;
		
	} // levelUp

	//--------------------------------------------------------
	/*
	std::string CSoulsTrialManager::getSoulsCategories()
	{
		std::string soulsCategories[4] = { "Unknown", "Heavy", "Wasted", "Light" };
		return soulsCategories;

	} // getSoulsCategories
	*/
	//--------------------------------------------------------

	void CSoulsTrialManager::processNewGroupOfSouls()
	{
		unsigned int numSouls = Math::random(_minSoulsPerGroup, _maxSoulsPerGroup);

		for (unsigned int i = 0; i < numSouls; ++i)
		{
			unsigned int rndReveal = Math::random(0, 100);

			if (rndReveal > _judgeLevel * REVEAL_SOUL_PROB) {
				_souls[SoulsCategory::UNKNOWN]++; // Unknown soul
			} else {
				unsigned int rndCategory = Math::random(0, 100);

				if (rndCategory < _heavySoulProb) _souls[SoulsCategory::HEAVY]++; // Heavy soul
				else if (rndCategory < _wastedSoulProb) _souls[SoulsCategory::WASTED]++; // Wasted soul
				else _souls[SoulsCategory::LIGHT]++; // Light soul
			}
		}

	} // processNewGroupOfSouls

	//--------------------------------------------------------
	
	std::vector<SoulsTrialManager::SoulsCategory> CSoulsTrialManager::createSouls(
		unsigned int numSoulsToWork[4], unsigned int numSoulsToBurn[4])
	{
		// Comprobación del número de almas recibido y disponible de cada categoría
		std::vector<SoulsTrialManager::SoulsCategory> wrongCategories;

		for (unsigned int i = 0; i < 4 ; ++i)
		{
			SoulsTrialManager::SoulsCategory soulsCategory = static_cast<SoulsTrialManager::SoulsCategory>(i);
			if (_souls[soulsCategory] < numSoulsToWork[soulsCategory] + numSoulsToBurn[soulsCategory])
				wrongCategories.push_back(soulsCategory);
		}

		if (!wrongCategories.empty())
			return wrongCategories;

		// Creación de las almas
		for (unsigned int i = 0; i < 4; ++i)
		{
			SoulsTrialManager::SoulsCategory soulsCategory = static_cast<SoulsTrialManager::SoulsCategory>(i);
			createSoulsToWork(numSoulsToWork[soulsCategory], soulsCategory); // Almas a trabajar
			createSoulsToBurn(numSoulsToBurn[soulsCategory], soulsCategory); // Almas a quemar

			_souls[soulsCategory] -= numSoulsToWork[soulsCategory];
			_souls[soulsCategory] -= numSoulsToBurn[soulsCategory];
		}

		return wrongCategories;
	} // createSouls

	//--------------------------------------------------------

	bool CSoulsTrialManager::calculateSoulEvil(SoulsTrialManager::SoulsCategory soulCategory, int& soulCrude, int& soulCoke)
	{
		if (soulCategory == SoulsTrialManager::SoulsCategory::NONE)
			return false;
		
		int soulAether = 0;
		calculateSoulComposition(soulCategory, soulCrude, soulCoke, soulAether);

		return true;

	} // calculateSoulEvil

	//--------------------------------------------------------

	bool CSoulsTrialManager::calculateSoulAether(SoulsTrialManager::SoulsCategory soulCategory, int& soulAether)
	{
		if (soulCategory == SoulsTrialManager::SoulsCategory::NONE)
			return false;

		int soulCrude = 0;
		int soulCoke = 0;
		calculateSoulComposition(soulCategory, soulCrude, soulCoke, soulAether);

		return true;

	} // calculateSoulAether

	//--------------------------------------------------------

	void CSoulsTrialManager::createSoulsToWork(unsigned int numSouls, SoulsTrialManager::SoulsCategory soulsCategory)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersMessage::HellQuartersAction::SEND_SOUL_WORK, numSouls, soulsCategory);
		Logic::CPlaceable *hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);

		m.Dispatch(*hellQuarters->getEntity());

	} // createSoulsToWork

	//--------------------------------------------------------

	void CSoulsTrialManager::createSoulsToBurn(unsigned int numSouls, SoulsTrialManager::SoulsCategory soulsCategory)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersMessage::HellQuartersAction::SEND_SOUL_BURN, numSouls, soulsCategory);
		Logic::CPlaceable *hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);

		m.Dispatch(*hellQuarters->getEntity());

	} // createSoulsToBurn

	//--------------------------------------------------------

	void CSoulsTrialManager::calculateSoulComposition(SoulsTrialManager::SoulsCategory soulCategory,
		int& soulCrude, int& soulCoke, int& soulAether)
	{
		soulCrude = soulCoke = soulAether = 0;
		int unitsLeft = 21;
		
		using namespace SoulsTrialManager;

		switch (soulCategory)
		{
		case SoulsCategory::UNKNOWN:
			// ??
			soulCrude = Math::random(5, 21);
			unitsLeft -= soulCrude;

			if (unitsLeft > 0)
			{
				soulAether = Math::random(0, std::min(unitsLeft, 5));
				unitsLeft -= soulAether;
			}

			soulCoke = unitsLeft;
			break;

		case SoulsCategory::HEAVY:
			// +11 Crude
			soulCrude = Math::random(11, 21);
			unitsLeft -= soulCrude;

			if (unitsLeft > 0)
			{
				soulAether = Math::random(0, std::min(unitsLeft, 5));
				unitsLeft -= soulAether;
			}

			soulCoke = unitsLeft;
			break;

		case SoulsCategory::WASTED:
			// +11 Coke
			soulCoke = Math::random(11, 16);
			unitsLeft -= soulCoke;

			soulCrude = Math::random(5, unitsLeft);
			unitsLeft -= soulCrude;

			soulAether = unitsLeft;
			break;

		case SoulsCategory::LIGHT:
			// +3 Aether
			soulAether = Math::random(3, 5);
			unitsLeft -= soulAether;

			soulCrude = Math::random(5, unitsLeft);
			unitsLeft -= soulCrude;

			soulCoke = unitsLeft;
			break;

		default:
			assert(false && "Invalid soul category for composition calculation");
			return;
		}

	} // calculateSoulComposition

	//--------------------------------------------------------

	bool CSoulsTrialManager::open()
	{
		for (unsigned int i = 0; i < 4; ++i)
			_souls[i] = 0;

		_timeForNextSoulsGeneration = (_minSoulsGenerationTime + _maxSoulsGenerationTime) / 2;
		return true;

	} // open

	//--------------------------------------------------------

	void CSoulsTrialManager::close()
	{
	} // close

} // namespace Logic
