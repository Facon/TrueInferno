#ifndef LA_ATTEND_RESOURCE_BUILDING_REQUEST_
#define LA_ATTEND_RESOURCE_BUILDING_REQUEST_

#include "AI\LatentAction.h"
#include "AI\SMResourceBuildingData.h"

namespace AI {
	class CLAAttendResourceBuildingRequest : public CLatentAction {
	public:
		CLAAttendResourceBuildingRequest(CEntity* entity, CSMResourceBuildingData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAAttendResourceBuildingRequest() {}

		virtual bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceBuildingData& _smData;

		// Flag a true cuando ya se ha recibido una petición
		bool _requestReceived;

		// Mensaje con la petición recibida
		ResourceMessage _msgReceived;

		CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processResourcesChange();

		CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processResourcesInfo();
	};
}

#endif // LA_ATTEND_RESOURCE_BUILDING_REQUEST_