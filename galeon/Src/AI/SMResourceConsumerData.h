#ifndef SM_RESOURCE_CONSUMER_DATA_H_
#define SM_RESOURCE_CONSUMER_DATA_H_

namespace AI {
	class CSMResourceConsumerData {

	public:
		CSMResourceConsumerData() : _consumption(0) {}

		virtual ~CSMResourceConsumerData() {}

		// Devuelve la cantidad de recurso que se debe consumir en cada periodo
		int getConsumption() const{
			return _consumption;
		}

		// Modifica el consumo en la cantidad dada
		void modifyConsumption(int change) {
			_consumption += change;
		}

	private:
		// Cantidad de recurso que se debe consumir en cada periodo
		int _consumption;
	};
}

#endif