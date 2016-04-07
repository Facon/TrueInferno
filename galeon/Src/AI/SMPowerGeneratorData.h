#ifndef SM_POWER_GENERATOR_DATA_H_
#define SM_POWER_GENERATOR_DATA_H_

#include "Logic\Entity\Entity.h"
#include <map>

namespace AI {
	class CSMPowerGeneratorData {

	public:
		CSMPowerGeneratorData() : _newConsumer(EntityID::UNASSIGNED), _newConsumption(0), _totalConsumption(0), _currentCoke(0), _newConsumerAccepted(false) {}
		virtual ~CSMPowerGeneratorData() {}

		TEntityID getNewConsumer() const{
			return _newConsumer;
		}

		void setNewConsumer(TEntityID newConsumer){
			_newConsumer = newConsumer;
		}

		int getNewConsumption() const{
			return _newConsumption;
		}

		void setNewConsumption(int newConsumption){
			_newConsumption = newConsumption;
		}

		// Añade un nuevo consumidor con su correspondiente consumo periódico
		void addConsumer(TEntityID consumer, int consumption){
			// Si el consumidor existe
			if (_consumers.count(consumer) > 0){
				assert(false && "Can't add twice the same consumer");
				return;
			}

			// Actualizamos la lista de consumidores
			_consumers[consumer] = consumption;

			// Y el consumo total
			_totalConsumption += consumption;
		}

		// Elimina un consumidor y recalcula el consumo periódico total
		void removeConsumer(TEntityID consumer){
			// Si el consumidor no existe
			if (_consumers.count(consumer) == 0){
				assert(false && "Can't remove a consumer not present in the consumer list");
				return;
			}

			// Actualizamos el consumo total
			_totalConsumption -= _consumers[consumer];

			// Actualizamos la lista de consumidores
			_consumers.erase(consumer);
		}

		int getTotalConsumption() const{
			return _totalConsumption;
		}

		void setCurrentCoke(int currentCoke){
			_currentCoke = currentCoke;
		}

		int getCurrentCoke() const{
			return _currentCoke;
		}

		void setNewConsumerAccepted(bool accepted){
			_newConsumerAccepted = accepted;
		}

		bool getNewConsumerAccepted() const{
			return _newConsumerAccepted;
		}

	private:
		// Entidad consumidora nueva
		TEntityID _newConsumer;

		// Consumo de la nueva entidad
		int _newConsumption;

		// Consumidores actuales
		std::map<TEntityID, int> _consumers;

		// Consumo actual total
		int _totalConsumption;

		// Cantidad de coke actual
		int _currentCoke;

		// Decisión de aceptar o rechazar al nuevo consumidor
		bool _newConsumerAccepted;
	};
}

#endif // SM_POWER_GENERATOR_DATA_H_