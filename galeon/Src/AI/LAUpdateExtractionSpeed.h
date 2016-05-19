#ifndef LA_UPDATE_EXTRACTION_SPEED_H_
#define LA_UPDATE_EXTRACTION_SPEED_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceExtractorData.h"

namespace AI {
	class CLAUpdateExtractionSpeed : public CLatentAction {
		RTTI_DECL;

	public:
		CLAUpdateExtractionSpeed(CEntity* entity, CSMResourceExtractorData& smData) : CLatentAction(entity), _smData(smData), _received(false) {}

		virtual ~CLAUpdateExtractionSpeed() {}

		/** Gestor de mensajes de trabajadores. Modifica o informa acerca de los trabajadores del edificio */
		virtual bool HandleMessage(const WorkerMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceExtractorData& _smData;

		// Flag a true cuando se ha recibido el mensaje de información
		bool _received;

		// Mensaje con la información de los trabajadores
		WorkerMessage _workerInfo;
	};
}

#endif // LA_UPDATE_EXTRACTION_SPEED_H_