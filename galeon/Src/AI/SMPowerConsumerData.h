#ifndef SM_POWER_CONSUMER_DATA_H_
#define SM_POWER_CONSUMER_DATA_H_

#include "Logic\Entity\Entity.h"

namespace AI {
	class CSMPowerConsumerData {

	public:
		CSMPowerConsumerData() : _powerGenerator(EntityID::UNASSIGNED), _attached(false) {}
		virtual ~CSMPowerConsumerData() {}

		TEntityID getPowerGenerator() const{
			return _powerGenerator;
		}

		void setPowerGenerator(TEntityID powerGenerator){
			_powerGenerator = powerGenerator;
		}
	
		bool getAttached() const{
			return _attached;
		}

		void setAttached(bool attached){
			_attached = attached;
		}

	private:
		TEntityID _powerGenerator;
		bool _attached;
	};
}

#endif // SM_POWER_CONSUMER_DATA_H_