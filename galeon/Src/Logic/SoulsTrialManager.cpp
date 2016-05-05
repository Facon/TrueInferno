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

	void CSoulsTrialManager::tick(unsigned long msecs)
	{
		_timeSinceLastSoulsGeneration += msecs;

		if (_timeSinceLastSoulsGeneration >= _timeForNextSoulsGeneration)
		{
			//std::cout << "pre souls = [" << _souls[0] << "," << _souls[1] << "," << _souls[2] << "," << _souls[3] << "]" << std::endl; //

			processNewGroupOfSouls();

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
	
	bool CSoulsTrialManager::createSouls(unsigned int numSoulsToWork[4], unsigned int numSoulsToBurn[4])
	{
		// Comprobación del número de almas recibido y disponible de cada categoría
		for (unsigned int i = 0; i < 4 ; ++i)
		{
			if (_souls[i] < numSoulsToWork[i] + numSoulsToBurn[i])
				return false;
		}

		// Creación de las almas
		for (unsigned int i = 0; i < 4; ++i)
		{
			createSoulsToWork(numSoulsToWork[i]); // Almas a trabajar
			createSoulsToBurn(numSoulsToBurn[i]); // Almas a quemar

			_souls[i] -= numSoulsToWork[i];
			_souls[i] -= numSoulsToBurn[i];
		}

		return true;

	} // createSouls

	//--------------------------------------------------------

	void CSoulsTrialManager::createSoulsToWork(unsigned int numSouls)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersMessage::HellQuartersAction::SEND_SOUL_WORK, numSouls);
		Logic::CPlaceable *hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);

		m.Dispatch(*hellQuarters->getEntity());

	} // createSoulsToWork

	//--------------------------------------------------------

	void CSoulsTrialManager::createSoulsToBurn(unsigned int numSouls)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersMessage::HellQuartersAction::SEND_SOUL_BURN, numSouls);
		Logic::CPlaceable *hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);

		m.Dispatch(*hellQuarters->getEntity());

	} // createSoulsToBurn

	//--------------------------------------------------------

	bool CSoulsTrialManager::open()
	{
		_timeForNextSoulsGeneration = (_minSoulsGenerationTime + _maxSoulsGenerationTime) / 2;
		return true;

	} // open

	//--------------------------------------------------------

	void CSoulsTrialManager::close()
	{
	} // close

} // namespace Logic
