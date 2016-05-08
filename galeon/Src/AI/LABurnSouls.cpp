#include "LABurnSouls.h"
#include "Logic\ResourcesManager.h"

namespace AI
{
	RTTI_IMPL(CLABurnSouls, CLatentAction);

	CLatentAction::LAStatus CLABurnSouls::OnStart()
	{
		_cokeIncreased = _crudeIncreased = false;
		_totalCokeToIncrease = _totalCrudeToIncrease = 0;

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLABurnSouls::OnRun(unsigned int msecs)
	{
		// Quemamos las almas encoladas actualmente
		int soulCoke = 0;
		int soulCrude = 0;

		while (_smData.getNumSoulsToBurn() > 0)
		{
			CSoulsTrialManager::SoulsCategory nextSoulCategory = _smData.getNextSoulToBurn();
			// @TODO Get from category
			soulCoke = 5; soulCrude = 10;

			_totalCokeToIncrease += soulCoke;
			_totalCrudeToIncrease += soulCrude;
		}

		// Notificamos el incremento de Coke si no está hecho ya
		if (!_cokeIncreased)
		{
			ResourceMessage mCoke;
			mCoke.assembleResourcesChange(Logic::ResourceType::COKE, _totalCokeToIncrease);

			// Si falla intentaremos en el siguiente tick
			if (!mCoke.Dispatch(*_entity))
			{
				return LAStatus::RUNNING;
			}
			else
				_cokeIncreased = true;
		}

		// Notificamos el incremento de Crude si no está hecho ya
		if (!_crudeIncreased)
		{
			ResourceMessage mCrude;
			mCrude.assembleResourcesChange(Logic::ResourceType::CRUDE, _totalCrudeToIncrease);

			// Si falla intentaremos en el siguiente tick
			if (!mCrude.Dispatch(*_entity))
			{
				return LAStatus::RUNNING;
			}
			else
				_crudeIncreased = true;
		}

		// Si ya se han realizado ambos incrementos...
		return LAStatus::SUCCESS;
	}
}