#ifndef CONSUMER_H_
#define CONSUMER_H_

namespace Logic {

	// Estructura para modelar un consumidor
	struct Consumer {
		// Identificador del consumidor. Si sólo hay un consumidor por entidad, puede usarse directamente el id de entidad
		int id;

		// Unidades de consumo
		int consumption;

		// Periodo (ms) de consumo
		int period;
	};

}

#endif