#include "HellQuarters.h"

#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Logic/BuildingManager.h"
#include "AI/Server.h"
#include <iostream>
#include <cassert>

namespace Logic {
	RTTI_ROOT_IMPL(CHellQuarters);
	IMP_FACTORY(CHellQuarters);

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

		_soulsToWorkRequest = false;
		_soulsToBurnRequest = false;

		_sendingSoulToWorkState = SendingSoulToWorkState::Idle;
		_pathReceived = nullptr;

		return true;
	} // spawn

	void CHellQuarters::tick(unsigned int msecs){
		spawnSouls(msecs);

		// TODO TEST
		/*if (_timeSinceLastSpawn >= 1000)
			sendSoulToWork();*/
	} // tick

	// TODO TEST Chequear por qué NO está entrando aquí!
	bool CHellQuarters::HandleMessage(const WalkSoulPathMessage& msg){
		// Nos aseguramos que estamos recibiendo una respuesta y que estábamos en estado de esperarla
		if (msg._type != MessageType::WALK_SOUL_PATH_RESPONSE || _sendingSoulToWorkState != WaitingForPath)
			return false;

		assert(msg._path && "Message received with null path");
		_pathReceived = msg._path;

		// Cambiamos al estado de path recibido
		_sendingSoulToWorkState = SendingSoulToWorkState::PathReceived;

		return true;
	}

	void CHellQuarters::spawnSouls(unsigned int msecs){
		// Chequeamos si ha pasado tiempo suficiente
		_timeSinceLastSpawn += msecs;
		if (_timeSinceLastSpawn >= _timeBetweenSpawns){
			_timeSinceLastSpawn -= _timeBetweenSpawns;
			_numAvailableSouls += _numSpawnedSouls;

			std::cout << "available souls = " << _numAvailableSouls << std::endl;
			
			/*
			CMap* map = CServer::getSingletonPtr()->getMap();
			CEntity* newSoul = CEntityFactory::getSingletonPtr()->createEntity("Soul", map);

			PositionMessage m;
			m._type = MessageType::SET_POSITION;
			m._position = Vector3(0.0f, 2.0f, 0.0f);

			m.Dispatch(*newSoul);

			CBuildingManager* bm = CBuildingManager::getSingletonPtr();

			AI::CServer::getSoulPathAStarRoute(bm->)
			*/

			// TODO ¿Reproducimos algún sonido o animación de almas nuevas?
		}
	}

	bool CHellQuarters::sendSoulToWork(){
		if (_numAvailableSouls <= 0)
			return false;

		switch (_sendingSoulToWorkState){
		// En caso de estar parados, solicitamos ruta
		case Idle:{
			// TODO De momento, enviamos siempre al Evilator
			// Localizamos el Evilator
			CPlaceable *evilator = CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::Evilator);

			// Enviamos un mensaje para obtener la ruta hasta el Evilator
			WalkSoulPathMessage message;
			message._target = evilator;
			message._type = MessageType::WALK_SOUL_PATH_REQUEST;
			if (!message.Dispatch(*(this->getEntity()))){
				return false;
			}

			_sendingSoulToWorkState = SendingSoulToWorkState::WaitingForPath;
			break;
		}

		// En el caso de estar esperando no hacemos nada a la espera de que llegue el mensaje
		case WaitingForPath:{
			break;
		}

		// En ruta recibida, creamos el alma y le pasamos la ruta
		case PathReceived:{
			// TODO Crear alma y pasarle _pathReceived
			std::cout << "Path received!" << std::endl;
			for (auto it = _pathReceived->cbegin(); it != _pathReceived->cend(); ++it){
				std::cout << "Node=" << (*it) << std::endl;
			}

			// TODO Instanciar alma y toda la pesca de Assiertion
			--_numAvailableSouls;

			// Liberamos recursos
			delete(_pathReceived);
			_pathReceived = nullptr;

			break;
		}

		default:{
			assert(true && "Unimplemented logic for all states");
			break;
		}
		}

		return true;
	}

} // namespace Logic