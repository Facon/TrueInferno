#include "HellQuarters.h"


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
	RTTI_ROOT_IMPL(CHellQuarters);
	IMP_FACTORY(CHellQuarters);

	const float CHellQuarters::SOUL_ON_TILE_HEIGHT = 2.0;

	CHellQuarters::CHellQuarters() : IComponent() {
	}

	bool CHellQuarters::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		_timeSinceLastSpawn = 0;

		assert(entityInfo->hasAttribute("numInitialSouls") && "numInitialSouls is not defined");
		_numAvailableSouls = entityInfo->getIntAttribute("numInitialSouls");

		assert(entityInfo->hasAttribute("secondsBetweenSpawns") && "secondsBetweenSpawns is not defined");
		_timeBetweenSpawns = 1000 * entityInfo->getIntAttribute("secondsBetweenSpawns");

		assert(entityInfo->hasAttribute("numSpawnedSouls") && "numSpawnedSouls is not defined");
		_numSpawnedSouls = entityInfo->getIntAttribute("numSpawnedSouls");

		_numSoulsToWork = 0;
		_numSoulsToBurn = 0;

		_sendingSoulToWorkState = SendingSoulToWorkState::Idle;
		_pathReceived = nullptr;

		return true;
	} // spawn

	void CHellQuarters::tick(unsigned int msecs){
		tickSpawnSouls(msecs);

		tickSendSoulToWork(msecs);
	} // tick

	void CHellQuarters::tickSpawnSouls(unsigned int msecs){
		// Chequeamos si ha pasado tiempo suficiente
		_timeSinceLastSpawn += msecs;
		if (_timeSinceLastSpawn >= _timeBetweenSpawns){
			_timeSinceLastSpawn -= _timeBetweenSpawns;
			_numAvailableSouls += _numSpawnedSouls;

			std::cout << "available souls = " << _numAvailableSouls << std::endl;
			
			// TODO ¿Reproducimos algún sonido o animación de almas nuevas?
		}
	}

	bool CHellQuarters::HandleMessage(const WalkSoulPathMessage& msg){
		// Nos aseguramos que estamos recibiendo una respuesta y que estábamos en estado de esperarla
		if (msg._type != MessageType::RETURN_WALK_SOUL_PATH || _sendingSoulToWorkState != WaitingForPath)
			return false;

		// Guardamos la ruta devuelta. Puede ser NULL si no se encontró ruta al destino solicitado
		_pathReceived = msg._path;

		// Cambiamos al estado de path recibido
		_sendingSoulToWorkState = SendingSoulToWorkState::PathReceived;

		return true;
	}

	bool CHellQuarters::HandleMessage(const HellQuartersActionMessage& msg){
		// Ignoramos peticiones si no estamos en estado inactivo para mensajes de enviar a trabajar
		if ((_sendingSoulToWorkState != SendingSoulToWorkState::Idle) || (msg._type != MessageType::SEND_SOUL_WORK))
			return false;

		_numSoulsToWork += msg._numSouls;
		_sendingSoulToWorkState = SendingSoulToWorkState::Requested;

		return true;
	}

	void CHellQuarters::requestSendSoulToBurn(){
		// TODO Interfaz pública para iniciar la lógica de envío de almas a arder
	}

	void CHellQuarters::tickSendSoulToWork(unsigned int msecs){
		switch (_sendingSoulToWorkState){
		// En caso de estar parados, no se hace nada
		case Idle:{
			break;
		}

		case Requested:{
			// Reservamos las almas
			_numAvailableSouls -= _numSoulsToWork;

			if (_numAvailableSouls < 0){
				std::cout << "There are no available souls" << std::endl;
				_sendingSoulToWorkState = SendingSoulToWorkState::Fail;
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
				_sendingSoulToWorkState = SendingSoulToWorkState::Fail;
				return;
			}

			// Nos ponemos a esperar a la ruta
			_sendingSoulToWorkState = SendingSoulToWorkState::WaitingForPath;
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
					_sendingSoulToWorkState = SendingSoulToWorkState::Success;
					return;
				}
			}

			// Si no es válida
			else{
				std::cout << "There is no path :(" << std::endl;
			}

			// Si llegamos hasta aquí: fallo
			_sendingSoulToWorkState = SendingSoulToWorkState::Fail;
			break;
		}

		case Fail:{
			// Recuperamos las almas reservadas dado que no han podido enviarse satisfactoriamente a trabajar
			_numAvailableSouls += _numSoulsToWork;

			// Pasamos al estado de limpieza
			_sendingSoulToWorkState = SendingSoulToWorkState::Clean;

			break;
		}

		case Success:{
			// Pasamos al estado de limpieza
			_sendingSoulToWorkState = SendingSoulToWorkState::Clean;

			break;
		}

		case Clean:{
			_numSoulsToWork = 0;

			// Liberamos memoria
			if (_pathReceived){
				delete(_pathReceived);
				_pathReceived = nullptr;
			}

			_sendingSoulToWorkState = SendingSoulToWorkState::Idle;
			break;
		}

		default:{
			assert(true && "Unimplemented logic for all states");
			break;
		}
		}
	}

	bool CHellQuarters::createAndSendSoulToWork(){
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