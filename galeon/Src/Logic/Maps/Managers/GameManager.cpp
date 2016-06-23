//---------------------------------------------------------------------------
// GameManager.cpp
//---------------------------------------------------------------------------

/**
@file GameManager.cpp

Contiene la implementaci�n del gestor del estado de la partida.

@author �lvaro Valera
@date Enero, 2016
*/

#include "GameManager.h"

#include <cassert>
#include <iostream>

#include "AI/Manager/AIManager.h"
#include "Logic/HFManager.h"
#include "Logic/Events/EventManager.h"
#include "Logic/Events/EndRoundEvent.h"
#include "Application/BaseApplication.h"
#include <Audio/Server.h>

namespace Logic {

	CGameManager* CGameManager::_instance = nullptr;

	//--------------------------------------------------------

	CGameManager::CGameManager()
	{
		_instance = this;

	} // CGameManager

	//--------------------------------------------------------

	CGameManager::~CGameManager()
	{
		assert(_instance);
		_instance = nullptr;

	} // ~CGameManager

	//--------------------------------------------------------

	bool CGameManager::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CGameManager no permitida!");

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
		assert(_instance && "Logic::CGameManager no est� inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CGameManager::open()
	{
		_victory = false;
		return true;

	} // open

	//--------------------------------------------------------

	void CGameManager::close()
	{
	} // close

	void CGameManager::roundFinished(){
		// Cogemos la puntuaci�n del jugador
		int playerScore = static_cast<int>(HFManager::getSingletonPtr()->getHadesFavor());

		// Buscamos al dios activo con menos puntuaci�n
		AI::CGod* worstActiveGod = AI::CAIManager::getSingletonPtr()->getWorstActiveGod();

		// Deber�amos haber encontrado uno
		if (worstActiveGod == nullptr){
			assert(false && "Round ended but there are no gods left");
			return;
		}

		std::cout << "God eliminated: " << worstActiveGod->getName() << std::endl;

		// Determinamos si el jugador gan� o perdi� la ronda comparando su score con el del peor dios activo
		_victory = (playerScore > worstActiveGod->getScore());

		// Si perdi� el jugador la ronda acabamos la partida (con derrota)
		if (!_victory) {
			Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
		}

		// Desactivamos al dios
		AI::CAIManager::getSingletonPtr()->eliminateGod(worstActiveGod->getName());

		// Determinamos si es final de ronda o de partida intentando obtener el siguiente dios activo con menor puntuaci�n
		// Si no hay m�s dioses -> final de partida
		bool endGame = AI::CAIManager::getSingletonPtr()->getWorstActiveGod() == nullptr;

		// Si es final de partida, cambiamos el estado y acabamos la partida (con victoria)
		if (endGame) {
			Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
			return;
		}

		// Sonido de fin de ronda
		Audio::CServer::getSingletonPtr()->playSfxSound("round_finished");

		// En otro caso es final de ronda, as� que notificamos con un evento
		bool eventThrown = Logic::CEventManager::getSingletonPtr()->addTimeEvent(new CEndRoundEvent(_victory, 100));
		assert(eventThrown && "Event couldn't be thrown");

		// En caso de fin de ronda, notificamos a los managers de tiempo e IA
		CTimeManager::getSingletonPtr()->startNextRound();
		AI::CAIManager::getSingletonPtr()->startNextRound();
	}

	void CGameManager::tick(unsigned int msecs){
	}

} // namespace Logic
