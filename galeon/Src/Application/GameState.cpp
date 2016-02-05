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
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

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
		//Physics::CServer::getSingletonPtr()->setGroupCollisions(0,1,false);
		Physics::CServer::getSingletonPtr()->createScene();

		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;

		// Cargamos el nivel a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->loadLevel("map.txt"))
			return false;

		// Cargamos la ventana que muestra el tiempo de juego transcurrido.
		_timeWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Time.layout");

		// Cargamos la ventana que muestra la barra superior
		_uibarsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIbars.layout");


		return true;

	} // init

	//--------------------------------------------------------

	void CGameState::release() 
	{
		Logic::CServer::getSingletonPtr()->unLoadLevel();

		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		// Liberamos la escena física.
		Physics::CServer::getSingletonPtr()->destroyScene();

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

		// Activamos la ventana que nos muestra el tiempo transcurrido.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_timeWindow);
		_timeWindow->setVisible(true);
		_timeWindow->activate();

		// Activamos la interfaz de usuario
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_uibarsWindow);
		_uibarsWindow->setVisible(true);
		_uibarsWindow->activate();

	} // activate

	//--------------------------------------------------------

	void CGameState::deactivate() 
	{
		// Desactivamos la ventana de tiempo.
		_timeWindow->deactivate();
		_timeWindow->setVisible(false);

		// Desactivamos la ventana de UiBars.
		_uibarsWindow->deactivate();
		_uibarsWindow->setVisible(false);

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
		CApplicationState::tick(msecs);

		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		// Changing Time UI
		_time += msecs;
		
		std::stringstream text;
		text << "Time: " << _time/1000;
		_timeWindow->setText(text.str());

		// Changing Resources UI
		_uibarsWindow->getChild("TopBar/Mineral")->setText("Mineral  " + std::to_string(static_cast<int>(trunc(_resourceManager.getMineral()))));
		_uibarsWindow->getChild("TopBar/Gas")->setText("Gas  " + std::to_string(static_cast<int>(trunc(_resourceManager.getGas()))));
		_uibarsWindow->getChild("TopBar/Coke")->setText("Coke  " + std::to_string(static_cast<int>(trunc(_resourceManager.getCoke()))));
		_uibarsWindow->getChild("TopBar/Crude")->setText("Crude  " + std::to_string(static_cast<int>(trunc(_resourceManager.getCrude()))));
		_uibarsWindow->getChild("TopBar/Pure")->setText("Pure  " + std::to_string(static_cast<int>(trunc(_resourceManager.getPure()))));
		_uibarsWindow->getChild("TopBar/Refined")->setText("Refined  " + std::to_string(static_cast<int>(trunc(_resourceManager.getRefined()))));
		// TODO Change to our format
		_uibarsWindow->getChild("TopBar/TimeLeft")->setText("Time:  " + std::to_string(_time/1000));
		_uibarsWindow->getChild("TopBar/HadesFavor")->setText("HF: " + std::to_string(static_cast<int>(trunc(_resourceManager.getHadesFavor()))));
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
			_app->setState("menu");
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
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
