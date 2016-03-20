#include "LATransformResources.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	CLatentAction::LAStatus CLATransformResources::OnStart() {
		// Leemos la cantidad de recursos que van a ser transformados
		_transformed = _smData.getAvailableResources();

		// TODO Quitar lo no transformable por coste
		// transformed = ...

		// Si no hay nada que transformar, hemos acabado
		if (_transformed == 0){
			return LAStatus::SUCCESS;
		}
		
		_decrementDone = false;
		_incrementDone = false;

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLATransformResources::OnRun(unsigned int msecs) {
		// Notificamos el decremento de recursos de entrada si no está hecho ya
		if (!_decrementDone){
			ResourceMessage mDecrementFrom;
			mDecrementFrom.assembleResourcesChange(_resourceFrom, -(int)_transformed);

			// Si falla intentaremos en el siguiente tick
			if (!mDecrementFrom.Dispatch(*_entity)){
				return LAStatus::RUNNING;
			}
			else
				_decrementDone = true;
		}

		if (!_incrementDone){
			// Notificamos el incremento de recursos de salida aplicando el ratio de conversión
			ResourceMessage mIncrementInto;
			mIncrementInto.assembleResourcesChange(_resourceInto, (int)(_transformed * _transformRatio));

			// Si falla intentaremos en el siguiente tick
			if (!mIncrementInto.Dispatch(*_entity)){
				return LAStatus::RUNNING;
			}
			else
				_incrementDone = true;
		}

		return LAStatus::SUCCESS;
	}
}