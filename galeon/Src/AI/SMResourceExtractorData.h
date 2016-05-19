#ifndef SM_RESOURCE_EXTRACTOR_DATA_
#define SM_RESOURCE_EXTRACTOR_DATA_

namespace AI {
	class CSMResourceExtractorData {

	public:
		CSMResourceExtractorData() : _speedFactor(0) {}

		virtual ~CSMResourceExtractorData() {}

		// Devuelve el factor [0,1] que se aplica a la velocidad de producci�n
		float getSpeedFactor() const {
			return _speedFactor;
		}

		// Establece el factor [0,1] que se aplica a la velocidad de producci�n
		void setSpeedFactor(float speedFactor) {
			_speedFactor = speedFactor;
		}

	private:
		// Factor [0,1] que se aplica a la velocidad de producci�n
		float _speedFactor;
	};
}

#endif