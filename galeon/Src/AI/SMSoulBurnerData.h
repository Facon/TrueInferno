#ifndef SM_SOUL_H_BURNER_DATA_
#define SM_SOUL_H_BURNER_DATA_

#include <queue>
#include "Logic\SoulsTrialManager.h"

namespace AI
{
	class CSMSoulBurnerData
	{

	public:

		CSMSoulBurnerData() { _soulsCategoryToBurn = TSoulsCategoryQueue(); }

		virtual ~CSMSoulBurnerData() {}

		unsigned int getNumSoulsToBurn()
		{
			return _soulsCategoryToBurn.size();
		}

		void addSoulToBurn(SoulsTrialManager::SoulsCategory soulCategory)
		{
			_soulsCategoryToBurn.push(soulCategory);
		}

		SoulsTrialManager::SoulsCategory getNextSoulToBurn()
		{
			SoulsTrialManager::SoulsCategory nextSoul = _soulsCategoryToBurn.front();
			_soulsCategoryToBurn.pop();

			return nextSoul;
		}

		void cleanSoulsToBurn()
		{
			while (!_soulsCategoryToBurn.empty())
			{
				_soulsCategoryToBurn.pop();
			}
		}

	private:

		/** Tipo cola de almas a quemar */
		typedef std::queue<SoulsTrialManager::SoulsCategory> TSoulsCategoryQueue;

		/** (Categoría de las) Almas preparadas para ser quemadas */
		TSoulsCategoryQueue _soulsCategoryToBurn;

	};

}

#endif