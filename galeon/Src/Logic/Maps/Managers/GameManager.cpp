//---------------------------------------------------------------------------
// GameManager.cpp
//---------------------------------------------------------------------------

/**
@file GameManager.cpp

Contiene la implementación del gestor del estado de la partida.

@author Álvaro Valera
@date Enero, 2016
*/

#include "GameManager.h"

#include <cassert>
#include <iostream>

#include "AI/Manager/AIManager.h"
#include "Logic/HFManager.h"
#include "Logic/Events/EventManager.h"
#include "Logic/Events/EndGameEvent.h"
#include "Logic/Events/EndRoundEvent.h"

namespace Logic {

	CGameManager* CGameManager::_instance = 0;

	//--------------------------------------------------------

	CGameManager::CGameManager()
	{
		_instance = this;

	} // CGameManager

	//--------------------------------------------------------

	CGameManager::~CGameManager()
	{
		assert(_instance);
		_instance = 0;

	} // ~CGameManager

	//--------------------------------------------------------

	bool CGameManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CGameManager no permitida!");

		new CGameManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CGameManager::Release()
	{
		assert(_instance && "Logic::CGameManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CGameManager::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CGameManager::close()
	{
	} // close

	void CGameManager::roundFinished(){
		// Cogemos la puntuación del jugador
		int playerScore = HFManager::getSingletonPtr()->getHadesFavor();

		// Buscamos al dios activo con menos puntuación
		AI::CGod* worstActiveGod = AI::CAIManager::getSingletonPtr()->getWorstActiveGod();

		// Deberíamos haber encontrado uno
		if (worstActiveGod == nullptr){
			assert(false && "Round ended but there are no gods left");
			return;
		}

		std::cout << "God eliminated: " << worstActiveGod->getName() << std::endl;

		// Determinamos si el jugador ganó o perdió comparando su score con el del dios
		bool victory = playerScore >= worstActiveGod->getScore();

		// Desactivamos al dios
		AI::CAIManager::getSingletonPtr()->eliminateGod(worstActiveGod->getName());

		// Determinamos si es final de ronda o de partida intentando obtener el siguiente dios activo con menor puntuación
		// Si no hay más dioses -> final de partida
		bool endGame = AI::CAIManager::getSingletonPtr()->getWorstActiveGod() == nullptr;

		// Notificamos al jugador con un evento u otro según si es final de ronda o partida
		bool eventThrown = false;
		if (endGame)
			eventThrown = Logic::CEventManager::getSingletonPtr()->addTimeEvent(new CEndGameEvent(victory, 100));
		else
			eventThrown = Logic::CEventManager::getSingletonPtr()->addTimeEvent(new CEndRoundEvent(victory, 100));

		assert(eventThrown && "Event couldn't be thrown");

		// En caso de fin de partida...
		if (endGame){
			// TODO
		}

		// En caso de fin de ronda, notificamos a los managers de tiempo e IA
		else{
			CTimeManager::getSingletonPtr()->startNextRound();
			AI::CAIManager::getSingletonPtr()->startNextRound();
		}
	}

	void CGameManager::tick(unsigned int msecs){

	}

} // namespace Logic
