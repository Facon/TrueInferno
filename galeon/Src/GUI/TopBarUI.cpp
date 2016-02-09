#include "TopBarUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

#include "Logic/TimeManager.h"
#include "Logic/ResourcesManager.h"

namespace GUI
{
	TopBarUI::TopBarUI(Logic::ResourcesManager& rm, Logic::TimeManager& tm) : _resourceManager(rm), _timeManager(tm)
	{
	}


	TopBarUI::~TopBarUI()
	{
	}

	void TopBarUI::init()
	{
		// Cargamos la ventana que muestra la barra superior
		_uibarsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIbars.layout");
	}

	void TopBarUI::activate()
	{
		// Activamos la interfaz de usuario
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_uibarsWindow);
		_uibarsWindow->setVisible(true);
		_uibarsWindow->activate();
	}

	void TopBarUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uibarsWindow->deactivate();
		_uibarsWindow->setVisible(false);
	}

	void TopBarUI::tick(unsigned int msecs)
	{
		_timeManager.tick(msecs);

		float total_seconds = _timeManager.getTime() / 1000.0f;
		unsigned int minutes = static_cast<unsigned int>(trunc(total_seconds / 60.0f));
		unsigned int seconds = static_cast<unsigned int>(((total_seconds / 60.0f) - minutes) * 60.0f);

		// Changing Resources UI
		_uibarsWindow->getChild("TopBar/Mineral")->setText("Mineral  " + std::to_string(static_cast<int>(trunc(_resourceManager.getMineral()))));
		_uibarsWindow->getChild("TopBar/Gas")->setText("Gas  " + std::to_string(static_cast<int>(trunc(_resourceManager.getGas()))));
		_uibarsWindow->getChild("TopBar/Coke")->setText("Coke  " + std::to_string(static_cast<int>(trunc(_resourceManager.getCoke()))));
		_uibarsWindow->getChild("TopBar/Crude")->setText("Crude  " + std::to_string(static_cast<int>(trunc(_resourceManager.getCrude()))));
		_uibarsWindow->getChild("TopBar/Pure")->setText("Pure  " + std::to_string(static_cast<int>(trunc(_resourceManager.getPure()))));
		_uibarsWindow->getChild("TopBar/Refined")->setText("Refined  " + std::to_string(static_cast<int>(trunc(_resourceManager.getRefined()))));
		_uibarsWindow->getChild("TopBar/TimeLeft")->setText("Time:  " + std::to_string(minutes) + ":" + ((seconds > 9) ? std::to_string(seconds) : "0" + std::to_string(seconds)));
		_uibarsWindow->getChild("TopBar/HadesFavor")->setText("HF: " + std::to_string(static_cast<int>(trunc(_resourceManager.getHadesFavor()))));
	}
}