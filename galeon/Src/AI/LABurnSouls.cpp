#include "LABurnSouls.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	RTTI_IMPL(CLABurnSouls, CLatentAction);

	CLatentAction::LAStatus CLABurnSouls::OnStart() {
		_cokeIncreased = false;
		_crudeIncreased = false;
		
		//std::cout << "Souls to burn = " << _soulsToBurn << std::endl;

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLABurnSouls::OnRun(unsigned int msecs) {
		// Quemamos las almas encoladas actualmente
		unsigned int _soulsToBurn = _smData.getNumSoulsToBurn();

		// Notificamos el incremento de coke si no está hecho ya
		if (!_cokeIncreased){
			ResourceMessage mCoke;
			mCoke.assembleResourcesChange(Logic::ResourceType::COKE, _soulsToBurn * _cokePerSoul);

			// Si falla intentaremos en el siguiente tick
			if (!mCoke.Dispatch(*_entity)){
				return LAStatus::RUNNING;
			}
			else
				_cokeIncreased = true;
		}

		// Notificamos el incremento de crude si no está hecho ya
		if (!_crudeIncreased){
			ResourceMessage mCrude;
			mCrude.assembleResourcesChange(Logic::ResourceType::CRUDE, _soulsToBurn * _crudePerSoul);

			// Si falla intentaremos en el siguiente tick
			if (!mCrude.Dispatch(*_entity)){
				return LAStatus::RUNNING;
			}
			else
				_crudeIncreased = true;
		}

		// Limpiamos las almas quemadas
		_smData.setNumSoulsToBurn(_smData.getNumSoulsToBurn() - _soulsToBurn);

		return LAStatus::SUCCESS;
	}
}