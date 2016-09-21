//---------------------------------------------------------------------------
// GameState.cpp
//---------------------------------------------------------------------------

/**
@file GameState.cpp

Contiene la implementación del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llansó
@date Agosto, 2010
*/

#include "GameState.h"

#include "Logic/Server.h"
#include "Logic/Events/EventManager.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/GameRuntimeContext.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "GUI/UIManager.h"

#include "Physics/Server.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

#include <cmath>

namespace Application {

	bool CGameState::init()
	{
		CApplicationState::init();

		// Crear la escena física.
		Physics::CServer::getSingletonPtr()->createScene();

		// Config de parámetos según el entorno
		std::string managersFile = "managers.txt";
		std::string mapFile = "map.txt";

		Logic::GameRuntimeContext context = Logic::CServer::getSingletonPtr()->getGameRuntimeContext();
		switch (context) {
		case Logic::GameRuntimeContext::DEV:
			managersFile = "managers_dev.txt";
			mapFile = "map_dev.txt";
			break;
		case Logic::GameRuntimeContext::SCRIPTED_DEMO:
			managersFile = "managers_script.txt";
			mapFile = "map_script.txt";
			break;
		default:
			managersFile = "managers.txt";
			mapFile = "map.txt";
			break;
		}

		// Cargamos el archivo con los valores de configuración de los managers.
		if (!Logic::CServer::getSingletonPtr()->loadManagersConfigurationValues(managersFile))
			return false;

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;

		// Cargamos el nivel a partir del nombre del mapa.
		if (!Logic::CServer::getSingletonPtr()->loadLevel(mapFile))
			return false;

		// Cargamos la ventana que muestra el tiempo de juego transcurrido.
		_timeWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Time.layout");

		_uiManager = new GUI::UIManager();
		_uiManager->init();

		return true;

	} // init

	//--------------------------------------------------------

	void CGameState::release() 
	{
		Logic::CEventManager::getSingletonPtr()->unloadEvents();

		Logic::CServer::getSingletonPtr()->unLoadLevel();

		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		// Liberamos la escena física.
		Physics::CServer::getSingletonPtr()->destroyScene();

		_uiManager->release();

		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CGameState::activate()
	{
		CApplicationState::activate();
		
		// Activamos el mapa que ha sido cargado para la partida.
		Logic::CServer::getSingletonPtr()->activateMap();

		// Queremos que el GUI maneje al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		// Activamos la ventana que nos muestra el tiempo transcurrido.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_timeWindow);
		_timeWindow->setVisible(true);
		_timeWindow->activate();

		_uiManager->activate();

	} // activate

	//--------------------------------------------------------

	void CGameState::deactivate() 
	{
		// Desactivamos la ventana de tiempo.
		_timeWindow->deactivate();
		_timeWindow->setVisible(false);

		_uiManager->deactivate();

		// Desactivamos la clase que procesa eventos de entrada para 
		// controlar al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();
		
		// Desactivamos el mapa de la partida.
		Logic::CServer::getSingletonPtr()->deactivateMap();
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameState::tick(unsigned int msecs) 
	{
		if (_paused == false)
		{
			CApplicationState::tick(msecs);

			// Simulación física
			Physics::CServer::getSingletonPtr()->tick(msecs);

			// Actualizamos la lógica de juego.
			Logic::CServer::getSingletonPtr()->tick(msecs);

			// Changing resources displays info
			_uiManager->tick(msecs);
		}
		else
		{
			if (_uiManager->getPauseMenu()->_exit == true)
			{
				_paused = !_paused;
				_uiManager->setPauseMenu(_paused);
				_uiManager->getPauseMenu()->_exit = false;
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorPoint");
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorPoint");
				_app->setState("menu");

			}
		}

	} // tick

	//--------------------------------------------------------

	bool CGameState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_paused = !_paused;
			_uiManager->setPauseMenu(_paused);
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameState::mousePressed(const GUI::CMouseState &mouseState)
	{
		//if (mouseState.button == GUI::Button::TButton::LEFT){
			//_uiManager->getSideBarUI()->playerInteractionWithLeftClick();
		//}

		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
