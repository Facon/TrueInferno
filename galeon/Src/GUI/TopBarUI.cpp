#include "TopBarUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

#include "Logic/TimeManager.h"
#include "Logic/ResourcesManager.h"
#include "Logic/HFManager.h"

namespace GUI
{
	TopBarUI::TopBarUI(Logic::ResourcesManager& rm, Logic::HFManager& hfm, Logic::TimeManager& tm) : _resourceManager(rm), _hfManager(hfm), _timeManager(tm)
	{
	}


	TopBarUI::~TopBarUI()
	{
	}

	void TopBarUI::init()
	{
		// Cargamos la ventana que muestra la barra superior
		_uiBarsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIResourceBar.layout");
	}

	void TopBarUI::activate()
	{
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiBarsWindow);
		// Activamos la interfaz de usuario
		_uiBarsWindow->setVisible(true);
		_uiBarsWindow->activate();
	}

	void TopBarUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uiBarsWindow->deactivate();
		_uiBarsWindow->setVisible(false);
	}

	void TopBarUI::tick(unsigned int msecs)
	{
		_timeManager.tick(msecs);

		float total_seconds = _timeManager.getTime() / 1000.0f;
		unsigned int minutes = static_cast<unsigned int>(trunc(total_seconds / 60.0f));
		unsigned int seconds = static_cast<unsigned int>(((total_seconds / 60.0f) - minutes) * 60.0f);

		// Changing Resources UI
		
		_uiBarsWindow->getChild("Mineral")->setText("Mineral  " + std::to_string(static_cast<int>(trunc(_resourceManager.getMineral()))));
		_uiBarsWindow->getChild("Gas")->setText("Gas  " + std::to_string(static_cast<int>(trunc(_resourceManager.getGas()))));
		_uiBarsWindow->getChild("Coke")->setText("Coke  " + std::to_string(static_cast<int>(trunc(_resourceManager.getCoke()))));
		_uiBarsWindow->getChild("Crude")->setText("Crude  " + std::to_string(static_cast<int>(trunc(_resourceManager.getCrude()))));
		_uiBarsWindow->getChild("Pure")->setText("Pure  " + std::to_string(static_cast<int>(trunc(_resourceManager.getPure()))));
		_uiBarsWindow->getChild("Refined")->setText("Refined  " + std::to_string(static_cast<int>(trunc(_resourceManager.getRefined()))));
		_uiBarsWindow->getChild("TimeLeft")->setText("Time:  " + std::to_string(minutes) + ":" + ((seconds > 9) ? std::to_string(seconds) : "0" + std::to_string(seconds)));
		_uiBarsWindow->getChild("HadesFavor")->setText("HF: " + std::to_string(static_cast<int>(trunc(_hfManager.getHadesFavor()))));
	}

}