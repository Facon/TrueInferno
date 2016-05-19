#include "LAUpdateExtractionSpeed.h"

#include "Logic\Entity\Message.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {
	RTTI_IMPL(CLAUpdateExtractionSpeed, CLatentAction);
	
	bool CLAUpdateExtractionSpeed::HandleMessage(const WorkerMessage& msg) {
		// Rechazamos lo que no sean mensajes de petici�n
		if (msg._type != MessageType::WORKER_INFO)
			return false;

		// No se aceptan peticiones simult�neas
		if (_received)
			return false;

		_received = true;

		// Guardamos datos para procesarlos despu�s
		_workerInfo = msg;

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
			// Suspendemos la LA hasta que llegue un mensaje de petici�n
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

		// Cacheamos la informaci�n de los trabajadores
		int minWorkers = _workerInfo._minWorkers;
		int maxWorkers = _workerInfo._maxWorkers;
		int activeWorkers = _workerInfo._activeWorkers;

		// Determinamos el factor [0,1] de velocidad de extracci�n
		float speedFactor = 0;

		// Controlamos que el m�ximo de trabajadores no sea 0
		if (maxWorkers == 0){
			assert(false && "Max Workers can't be 0");
			speedFactor = 0;
		}

		// El edificio no extrae si no llega al m�nimo de trabajadores
		else if (activeWorkers < minWorkers)
			speedFactor = 0;

		// Si supera el m�ximo, el factor es el m�ximo posible
		else if (activeWorkers > maxWorkers)
			speedFactor = 1;

		// Escalamos linealmente el resto de valores a [1.0/max, max]. E.g.: Si el m�ximo es 5 el factor quedar� en [0.2, 1]
		else
			speedFactor = Math::linearScale((float) minWorkers, (float) maxWorkers, (float) 1.0 / maxWorkers, 1.0, (float) activeWorkers);

		_smData.setSpeedFactor(speedFactor);

		_received = false;

		return LAStatus::SUCCESS;
	}

}