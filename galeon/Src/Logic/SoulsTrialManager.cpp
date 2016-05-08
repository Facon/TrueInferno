//---------------------------------------------------------------------------
// SoulsTrialManager.cpp
//---------------------------------------------------------------------------

/**
@file SoulsTrialManager.cpp

Contiene la implementación del gestor del Juicio de Almas.

@see Logic::CSoulsTrialManager

@author Raúl Segura
@date Abril, 2016
*/

#include "SoulsTrialManager.h"
#include <cassert>
#include <cstdio>

#include "BaseSubsystems/Math.h"
#include "Logic/BuildingManager.h"

#include "Logic/Entity/Components/Soul.h"

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
		assert(!_instance && "Segunda inicialización de Logic::CSoulsTrialManager no permitida!");

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
		assert(_instance && "Logic::CSoulsTrialManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CSoulsTrialManager::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
		[
			luabind::class_<CSoulsTrialManager>("CSoulsTrialManager")
			.enum_("SoulsCategory")
			[
				luabind::value("STC_UNKNOWN", CSoulsTrialManager::SoulsCategory::UNKNOWN),
				luabind::value("STC_HEAVY", CSoulsTrialManager::SoulsCategory::HEAVY),
				luabind::value("STC_WASTED", CSoulsTrialManager::SoulsCategory::WASTED),
				luabind::value("STC_LIGHT", CSoulsTrialManager::SoulsCategory::LIGHT)
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
			// TODO ¿Reproducimos algún sonido o notificación de almas nuevas?

			// TODO Borrar cuando funcione el Juicio de Almas
			//std::cout << "mid souls = [" << _souls[0] << "," << _souls[1] << "," << _souls[2] << "," << _souls[3] << "]" << std::endl; //
			//unsigned int toWork[] = { 5, 3, 3, 3 }; //
			//unsigned int toBurn[] = { 5, 2, 2, 4 }; //
			//bool createsouls = createSouls(toWork, toBurn); //
			//std::cout << "create si o no = " << createsouls << std::endl; //
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

	std::string* CSoulsTrialManager::getSoulsCategories()
	{
		std::string soulsCategories[4] = { "Unknown", "Heavy", "Wasted", "Light" };
		return soulsCategories;

	} // getSoulsCategories

	//--------------------------------------------------------

	void CSoulsTrialManager::processNewGroupOfSouls()
	{
		unsigned int numSouls = Math::random(_minSoulsPerGroup, _maxSoulsPerGroup);

		for (unsigned int i = 0; i < numSouls; ++i)
		{
			unsigned int rndReveal = Math::random(0, 100);

			if (rndReveal > _judgeLevel * REVEAL_SOUL_PROB) {
				_souls[0]++; // Unknown soul
			} else {
				unsigned int rndCategory = Math::random(0, 100);

				if (rndCategory < _heavySoulProb) _souls[1]++; // Heavy soul
				else if (rndCategory < _wastedSoulProb) _souls[2]++; // Wasted soul
				else _souls[3]++; // Light soul
			}
		}

	} // processNewGroupOfSouls

	//--------------------------------------------------------
	
	CSoulsTrialManager::SoulsCategory CSoulsTrialManager::createSouls(unsigned int numSoulsToWork[4], unsigned int numSoulsToBurn[4])
	{
		// Comprobación del número de almas recibido y disponible de cada categoría
		for (unsigned int i = 0; i < 4 ; ++i)
		{
			if (_souls[i] < numSoulsToWork[i] + numSoulsToBurn[i])
				return static_cast<SoulsCategory>(i);
		}

		// Creación de las almas
		for (unsigned int i = 0; i < 4; ++i)
		{
			createSoulsToWork(numSoulsToWork[i], static_cast<SoulsCategory>(i)); // Almas a trabajar
			createSoulsToBurn(numSoulsToBurn[i], static_cast<SoulsCategory>(i)); // Almas a quemar

			_souls[i] -= numSoulsToWork[i];
			_souls[i] -= numSoulsToBurn[i];
		}

		return SoulsCategory::NONE;

	} // createSouls

	//--------------------------------------------------------

	void CSoulsTrialManager::createSoulsToWork(unsigned int numSouls, SoulsCategory soulsCategory)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersMessage::HellQuartersAction::SEND_SOUL_WORK, numSouls, soulsCategory);
		Logic::CPlaceable *hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);

		m.Dispatch(*hellQuarters->getEntity());

	} // createSoulsToWork

	//--------------------------------------------------------

	void CSoulsTrialManager::createSoulsToBurn(unsigned int numSouls, SoulsCategory soulsCategory)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersMessage::HellQuartersAction::SEND_SOUL_BURN, numSouls, soulsCategory);
		Logic::CPlaceable *hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);

		m.Dispatch(*hellQuarters->getEntity());

	} // createSoulsToBurn

	//--------------------------------------------------------

	bool CSoulsTrialManager::open()
	{
		_souls[4] = { 0 };
		_timeForNextSoulsGeneration = (_minSoulsGenerationTime + _maxSoulsGenerationTime) / 2;
		return true;

	} // open

	//--------------------------------------------------------

	void CSoulsTrialManager::close()
	{
	} // close

} // namespace Logic
