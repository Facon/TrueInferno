//---------------------------------------------------------------------------
// LoadingState.cpp
//---------------------------------------------------------------------------

/**
@file LoadingState.cpp

Contiene la implementaci�n del estado de carga.

@see Application::CApplicationState
@see Application::CLoadingState

@author Ra�l Segura
@date Junio, 2016
*/

#include "LoadingState.h"

#include "Logic/Server.h"
#include "GUI/Server.h"

#include <CEGUI/CEGUI.h>
#include <iostream>

namespace Application {

	CLoadingState::~CLoadingState() 
	{
	} // ~CLoadingState

	//--------------------------------------------------------

	bool CLoadingState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el men�
		_loadingWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Loading.layout");

		return true;

	} // init

	//--------------------------------------------------------

	void CLoadingState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CLoadingState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el men� y activamos el rat�n.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_loadingWindow);
		_loadingWindow->setVisible(true);
		_loadingWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

	} // activate

	//--------------------------------------------------------

	void CLoadingState::deactivate()
	{		
		// Desactivamos la ventana GUI con el men� y el rat�n.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		_loadingWindow->deactivate();
		_loadingWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CLoadingState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CLoadingState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CLoadingState::keyReleased(GUI::TKey key)
	{
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CLoadingState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLoadingState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CLoadingState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
