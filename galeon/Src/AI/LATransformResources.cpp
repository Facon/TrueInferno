#include "LATransformResources.h"

#include "Logic\ResourcesManager.h"
#include "Logic\Entity\Components\Billboard.h"
#include "Logic\Entity\ParticleType.h"

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
			// Notificamos la necesidad con icono
			IconMessage m(MessageType::ICON_ADD, Billboard::getResourceIcon(_resourceFrom));
			bool result = m.Dispatch(*_entity);
			assert(result && "Can't add resource icon");

			return LAStatus::SUCCESS;
		}

		// Intentamos pagar el coste de transformación para todos los recursos a transformar: costeMáx = cantidadMáx * ratio
		if ((_costRatio > ZERO_COST_RATIO) && (_costResource != ResourceType::NONE)){
			int paidCost;
			ResourcesManager::getSingletonPtr()->decrementResources(_costResource, (int) ceil(_transformed * _costRatio), false, true, paidCost);

			/* En base al coste válido que se ha podido pagar, paidCost, sabemos cuánto podemos transformar realmente:
			* costeVáldo = cantidadVálida * ratio => cantidadVálida = costeVáldo / ratio */
			_transformed = (int)truncf(paidCost / _costRatio);
		}

		// Si no hay nada que transformar tras asjustar por costes, hemos acabado
		if (_transformed == 0){
			// Notificamos la necesidad con icono
			IconMessage m(MessageType::ICON_ADD, Billboard::getResourceIcon(_costResource));
			bool result = m.Dispatch(*_entity);
			assert(result && "Can't add resource icon");

			return LAStatus::SUCCESS;
		}

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

		// Activamos partículas de acción de edificio
		ParticleMessage m(ParticleType::BUILDING_ACTION, _transformParticlesDuration);
		bool result = m.Dispatch(*_entity);
		assert(true && "Can't set building action particles");

		return LAStatus::SUCCESS;
	}
}