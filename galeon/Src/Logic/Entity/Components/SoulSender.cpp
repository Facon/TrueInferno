#include "SoulSender.h"


#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Logic/BuildingManager.h"
#include "AI/Server.h"
#include <iostream>
#include <cassert>

namespace Logic {
	RTTI_ROOT_IMPL(CSoulSender);
	IMP_FACTORY(CSoulSender);

	const float CSoulSender::SOUL_ON_TILE_HEIGHT = 2.0;

	CSoulSender::CSoulSender() : IComponent() {
	}

	bool CSoulSender::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		_numSoulsToSend = 0;

		_sendingSoulState = SendingSoulState::Idle;
		_pathReceived = nullptr;

		return true;
	} // spawn

	bool CSoulSender::HandleMessage(const SoulSenderRequestMessage& msg){
		// Ignoramos peticiones si no estamos en estado inactivo
		if (_sendingSoulState != SendingSoulState::Idle){
			assert(false && "SoulSender is busy");
		}

		_sendingSoulState = SendingSoulState::Requested;
		_numSoulsToSend += msg._numSouls;

		return true;
	}

	bool CSoulSender::HandleMessage(const WalkSoulPathMessage& msg){
		// Nos aseguramos que estamos recibiendo una respuesta y que estábamos en estado de esperarla
		if (msg._type != MessageType::RETURN_WALK_SOUL_PATH || _sendingSoulState != WaitingForPath)
			return false;

		// Guardamos la ruta devuelta. Puede ser NULL si no se encontró ruta al destino solicitado
		_pathReceived = msg._path;

		// Cambiamos al estado de path recibido
		_sendingSoulState = SendingSoulState::PathReceived;

		return true;
	}

	void CSoulSender::tick(unsigned int msecs){
		switch (_sendingSoulState){
		// En caso de estar parados, no se hace nada
		case Idle:{
			break;
		}

		case Requested:{
			// Reservamos las almas
			_numAvailableSouls -= _numSoulsToSend;

			if (_numAvailableSouls < 0){
				std::cout << "There are no available souls" << std::endl;
				_sendingSoulState = SendingSoulState::Fail;
				return;
			}

			// TODO De momento, enviamos siempre al Evilator
			// Localizamos el Evilator
			CPlaceable *evilator = CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::Evilator);

			// Enviamos un mensaje para obtener la ruta hasta el Evilator
			WalkSoulPathMessage message(evilator);
			message._type = MessageType::REQUEST_WALK_SOUL_PATH;
			
			// Si nadie atendió al mensaje
			if (!message.Dispatch(*this->getEntity())){
				std::cout << "No one answered the REQUEST_WALK_SOUL_PATH message" << std::endl;
				_sendingSoulState = SendingSoulState::Fail;
				return;
			}

			// Nos ponemos a esperar a la ruta
			_sendingSoulState = SendingSoulState::WaitingForPath;
			break;
		}

		// En el caso de estar esperando no hacemos nada y seguimos a la espera de que llegue el mensaje
		case WaitingForPath:{
			break;
		}

		// En ruta recibida, creamos el alma y le pasamos la ruta
		case PathReceived:{
			// Si la ruta recibida es válida
			if (_pathReceived != nullptr && _pathReceived->size()>0){
				std::cout << "Path received! :)" << std::endl;
				
				// Creamos y enviamos el alma a trabajar
				if (createAndSendSoulToWork()){
					// Si se llevó a cabo
					_sendingSoulState = SendingSoulState::Success;
					return;
				}
			}

			// Si no es válida
			else{
				std::cout << "There is no path :(" << std::endl;
			}

			// Si llegamos hasta aquí: fallo
			_sendingSoulState = SendingSoulState::Fail;
			break;
		}

		case Fail:{
			// Recuperamos las almas reservadas dado que no han podido enviarse satisfactoriamente a trabajar
			_numAvailableSouls += _numSoulsToSend;

			// Pasamos al estado de limpieza
			_sendingSoulState = SendingSoulState::Clean;

			break;
		}

		case Success:{
			// Pasamos al estado de limpieza
			_sendingSoulState = SendingSoulState::Clean;

			break;
		}

		case Clean:{
			_numSoulsToSend = 0;

			// Liberamos memoria
			if (_pathReceived){
				delete(_pathReceived);
				_pathReceived = nullptr;
			}

			_sendingSoulState = SendingSoulState::Idle;
			break;
		}

		default:{
			assert(true && "Unimplemented logic for all states");
			break;
		}
		}
	}

	bool CSoulSender::createAndSendSoulToWork(){
		CMap* map = CServer::getSingletonPtr()->getMap();
		CEntity* newSoul = CEntityFactory::getSingletonPtr()->createEntity("Soul", map);

		if (!newSoul){
			std::cout << "Can´t create new soul" << std::endl;
			return false;
		}

		// La ubicamos en la posición inicial de la ruta
		PositionMessage m;
		m._type = MessageType::SET_POSITION;
		m._position = (*_pathReceived)[0];
		m._position.y += SOUL_ON_TILE_HEIGHT;
		if (!m.Dispatch(*newSoul)){
			std::cout << "Can´t set soul on initial position" << std::endl;
			return false;
		}

		// Le indicamos la ruta que tiene que recorrer
		WalkSoulPathMessage m2(_pathReceived);
		m2._type = MessageType::PERFORM_WALK_SOUL_PATH;
		if (!m2.Dispatch(*newSoul)){
			std::cout << "Can´t assign path to soul" << std::endl;
			return false;
		}

		return true;
	}

} // namespace Logic