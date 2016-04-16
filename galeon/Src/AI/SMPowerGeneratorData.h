#ifndef SM_POWER_GENERATOR_DATA_H_
#define SM_POWER_GENERATOR_DATA_H_

#include "Logic\Entity\Consumer.h"
#include "Logic\Entity\Entity.h"
#include <map>

namespace AI {
	class CSMPowerGeneratorData {

	public:
		CSMPowerGeneratorData() : 
			_newConsumer(EntityID::UNASSIGNED),
			_newConsumptionUnits(0), _newConsumptionPeriod(0),
			_totalConsumptionUnits(0), _totalConsumptionPeriod(-1),
			_currentCoke(0), _newConsumerAccepted(false) {}

		virtual ~CSMPowerGeneratorData() {}

		TEntityID getNewConsumer() const{
			return _newConsumer;
		}

		void setNewConsumer(TEntityID newConsumer){
			_newConsumer = newConsumer;
		}

		int getNewConsumptionUnits() const{
			return _newConsumptionUnits;
		}

		void setNewConsumptionUnits(int newConsumptionUnits){
			_newConsumptionUnits = newConsumptionUnits;
		}

		int getNewConsumptionPeriod() const{
			return _newConsumptionPeriod;
		}

		void setNewConsumptionPeriod(int newConsumptionPeriod){
			_newConsumptionPeriod = newConsumptionPeriod;
		}

		// Añade un nuevo consumidor con su correspondiente consumo periódico. Devuelve true si se añadió realmente un nuevo elemento
		bool addConsumer(Logic::Consumer consumer){
			// Si el consumidor existe, de momento, no aceptaríamos volverlo a añadir
			if (_consumers.count(consumer.id) > 0){
				assert(false && "Can't add twice the same consumer");
				return false;
			}

			// Verificamos que el periodo sea el mismo si ya había alguno indicado
			if (_totalConsumptionPeriod == -1)
				_totalConsumptionPeriod = consumer.period;
			else
				assert(_totalConsumptionPeriod == consumer.period && "Can't mix different consumption periods");

			// Actualizamos la lista de consumidores
			_consumers[consumer.id] = consumer;

			// Y el consumo total
			_totalConsumptionUnits += consumer.consumption;

			return true;
		}

		// Elimina un consumidor y recalcula el consumo periódico total. Devuelve true si realmente se eliminó algún consumidor
		bool removeConsumer(int consumerId){
			// Si el consumidor no existe
			if (_consumers.count(consumerId) == 0){
				return false;
			}

			Logic::Consumer consumer = _consumers[consumerId];

			// Actualizamos el consumo total
			_totalConsumptionUnits -= consumer.consumption;

			// Actualizamos la lista de consumidores
			_consumers.erase(consumer.id);

			return true;
		}

		// Devuelve la cantidad de consumidores actualmente registrados
		int getNumConsumers() const {
			return _consumers.size();
		}

		// Devuelve los consumidores actualmente registrados
		std::map<int, Logic::Consumer>& getConsumers() {
			return _consumers;
		}

		int getTotalConsumptionUnits() const{
			return _totalConsumptionUnits;
		}

		int getTotalConsumptionPeriod() const{
			return _totalConsumptionPeriod;
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

		// Unidades de consumo de la nueva entidad
		int _newConsumptionUnits;

		// Periodo (ms) de consumo de la nueva entidad
		int _newConsumptionPeriod;

		// Consumidores actuales
		std::map<int, Logic::Consumer> _consumers;

		// Consumo actual total
		int _totalConsumptionUnits;

		// Periodo (ms) del consumo total
		int _totalConsumptionPeriod;

		// Cantidad de coke actual
		int _currentCoke;

		// Decisión de aceptar o rechazar al nuevo consumidor
		bool _newConsumerAccepted;
	};
}

#endif // SM_POWER_GENERATOR_DATA_H_