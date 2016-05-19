#include "LAUpdateExtractionSpeed.h"

#include "Logic\Entity\Message.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {
	RTTI_IMPL(CLAUpdateExtractionSpeed, CLatentAction);
	
	bool CLAUpdateExtractionSpeed::HandleMessage(const WorkerMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición
		if (msg._type != MessageType::WORKER_INFO)
			return false;

		// No se aceptan peticiones simultáneas
		if (_received)
			return false;

		_received = true;

		// TODO
		// Guardamos datos en la memoria compartida de la SM
		//_smData.setXXX(msg._xxx);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAUpdateExtractionSpeed::OnStart() {
		// Inicializamos
		_received = false;

		// Solicitamos info de los trabajadores
		WorkerMessage m;
		m.assembleWorkerAsk();

		// Si nos lo aceptan
		if (m.Dispatch(*_entity)){
			// Suspendemos la LA hasta que llegue un mensaje de petición
			return LAStatus::SUSPENDED;
		}

		// En otro caso
		else {
			// Esperamos otro tick
			return LAStatus::READY;
		}

	}

	CLatentAction::LAStatus CLAUpdateExtractionSpeed::OnRun(unsigned int msecs) {
		if (!_received){
			assert(false && "No info received");
			return LAStatus::FAIL;
		}

		// TODO

		return LAStatus::SUCCESS;
	}

}