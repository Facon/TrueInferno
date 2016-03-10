#include "LABurnSouls.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	CLatentAction::LAStatus CLABurnSouls::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLABurnSouls::OnRun(unsigned int msecs) {
		// Quemamos las almas encoladas actualmente
		std::cout << "Souls to burn = " << _smData.getNumSoulsToBurn() << std::endl;

		// Notificamos el incremento de recursos
		ResourceMessage mCoke(Logic::ResourceType::COKE, _smData.getNumSoulsToBurn()*_cokePerSoul);
		if (!mCoke.Dispatch(*_entity))
			return LAStatus::RUNNING;

		ResourceMessage mCrude(Logic::ResourceType::CRUDE, _smData.getNumSoulsToBurn()*_crudePerSoul);
		if (!mCrude.Dispatch(*_entity)){
			// Deshacemos el incremento de coke ya realizado
			mCoke._number *= -1;

			if (!mCoke.Dispatch(*_entity)){
				assert(false && "Coke increase hasn't been undone");
				return LAStatus::FAIL;
			}

			return LAStatus::RUNNING;
		}

		// Limpiamos las almas quemadas
		_smData.setNumSoulsToBurn(0);

		return LAStatus::SUCCESS;
	}
}