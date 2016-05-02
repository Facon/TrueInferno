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

#include "BaseSubsystems\Math.h"

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

	void CSoulsTrialManager::tick(unsigned int msecs)
	{
		_timeSinceLastSoulsGeneration += msecs;

		if (_timeSinceLastSoulsGeneration >= _timeForNextSoulsGeneration)
		{
			processNewGroupOfSouls();
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
