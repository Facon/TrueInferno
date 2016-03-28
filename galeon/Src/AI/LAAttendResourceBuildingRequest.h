#ifndef LA_ATTEND_RESOURCE_BUILDING_REQUEST_H_
#define LA_ATTEND_RESOURCE_BUILDING_REQUEST_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceBuildingData.h"

namespace AI {
	class CLAAttendResourceBuildingRequest : public CLatentAction {
	public:
		CLAAttendResourceBuildingRequest(CEntity* entity, CSMResourceBuildingData& smData) : CLatentAction(entity), _smData(smData), 
			_requestReceived(false), _finallyReserved(0), _processDone(false) {}

		virtual ~CLAAttendResourceBuildingRequest() {}

		virtual bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceBuildingData& _smData;

		// Flag a true cuando ya se ha recibido una petici�n
		bool _requestReceived;

		// Mensaje con la petici�n recibida
		ResourceMessage _msgReceived;

		// Cantidad de recursos finalmente reservados
		int _finallyReserved;

		// Flag para indicar que el procesado ya est� hecho y s�lo falta que acepten el mensaje
		bool _processDone;

		//CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processResourcesChange();

		CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processAskResources();

		CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processReserveResources();
	};
}

#endif // LA_ATTEND_RESOURCE_BUILDING_REQUEST_H_