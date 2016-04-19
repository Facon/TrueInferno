#include "LATransformResources.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	RTTI_IMPL(CLATransformResources, CLatentAction);
	
	CLatentAction::LAStatus CLATransformResources::OnStart() {
		// Inicializamos
		_decrementDone = false;
		_incrementDone = false;

		// Leemos la cantidad de recursos que van a ser transformados
		_transformed = _smData.getAvailableResources();

		// Si no hay nada que transformar, hemos acabado
		if (_transformed == 0){
			return LAStatus::SUCCESS;
		}

		// Intentamos pagar el coste de transformaci�n para todos los recursos a transformar: costeM�x = cantidadM�x * ratio
		if ((_costRatio > ZERO_COST_RATIO) && (_costResource != ResourceType::NONE)){
			int paidCost;
			ResourcesManager::getSingleton().decrementResources(_costResource, (int) ceil(_transformed * _costRatio), false, true, paidCost);

			/* En base al coste v�lido que se ha podido pagar, paidCost, sabemos cu�nto podemos transformar realmente:
			* costeV�ldo = cantidadV�lida * ratio => cantidadV�lida = costeV�ldo / ratio */
			_transformed = (int)truncf(paidCost / _costRatio);
		}

		// Si no hay nada que transformar tras asjustar por costes, hemos acabado
		if (_transformed == 0){
			return LAStatus::SUCCESS;
		}

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLATransformResources::OnRun(unsigned int msecs) {
		// Notificamos el decremento de recursos de entrada si no est� hecho ya
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
			// Notificamos el incremento de recursos de salida aplicando el ratio de conversi�n
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