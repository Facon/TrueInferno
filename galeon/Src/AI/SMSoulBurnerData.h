#ifndef SM_SOUL_H_BURNER_DATA_
#define SM_SOUL_H_BURNER_DATA_

#include <queue>
#include "Logic\SoulsTrialManager.h"

namespace AI
{
	class CSMSoulBurnerData
	{

	public:

		CSMSoulBurnerData()
		{
			_soulsCategoryToBurn = TSoulsCategoryQueue();
			_numAssignedSoulsToBurn = 0;
		}

		virtual ~CSMSoulBurnerData() {}

		unsigned int getNumSoulsToBurn()
		{
			return _soulsCategoryToBurn.size() + _numAssignedSoulsToBurn;
		}

		void increaseAssignedSoulsToBurn()
		{
			++_numAssignedSoulsToBurn;
		}

		void addSoulToBurn(CSoulsTrialManager::SoulsCategory soulCategory)
		{
			_soulsCategoryToBurn.push(soulCategory);
			--_numAssignedSoulsToBurn;
		}

		CSoulsTrialManager::SoulsCategory getNextSoulToBurn()
		{
			CSoulsTrialManager::SoulsCategory nextSoul = _soulsCategoryToBurn.front();
			_soulsCategoryToBurn.pop();

			return nextSoul;
		}

		void cleanSoulsToBurn()
		{
			while (!_soulsCategoryToBurn.empty())
			{
				_soulsCategoryToBurn.pop();
			}

			_numAssignedSoulsToBurn = 0;
		}

	private:

		/** Tipo cola de almas a quemar */
		typedef std::queue<CSoulsTrialManager::SoulsCategory> TSoulsCategoryQueue;

		/** (Categoría de las) Almas preparadas para ser quemadas */
		TSoulsCategoryQueue _soulsCategoryToBurn;

		/**
		Número de almas a quemar asignadas al horno que se encuentran
		aún en camino.
		*/
		int _numAssignedSoulsToBurn;

	};

}

#endif