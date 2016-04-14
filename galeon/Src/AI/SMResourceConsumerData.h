#ifndef SM_RESOURCE_CONSUMER_DATA_H_
#define SM_RESOURCE_CONSUMER_DATA_H_

namespace AI {
	class CSMResourceConsumerData {

	public:
		CSMResourceConsumerData() : _consumption(0), _reservedForConsume(0) {}

		virtual ~CSMResourceConsumerData() {}

		// Devuelve la cantidad de recurso que se debe consumir en cada periodo
		int getConsumption() const {
			return _consumption;
		}

		// Modifica el consumo en la cantidad dada
		void modifyConsumption(int change) {
			_consumption += change;
		}

		// Resetea el consumo
		void cleanConsumption() {
			_consumption = 0;
		}

		// Devuelve la cantidad de recurso que tenemos reservada
		int getReservedForConsume() const {
			return _reservedForConsume;
		}

		// Establece la cantidad de recurso que tenemos reservada
		void setReservedForConsume(int reservedForConsume) {
			_reservedForConsume = reservedForConsume;
		}

	private:
		// Cantidad de recurso que se debe consumir en cada periodo
		int _consumption;

		// Cantidad de recurso que tenemos reservada
		int _reservedForConsume;
	};
}

#endif