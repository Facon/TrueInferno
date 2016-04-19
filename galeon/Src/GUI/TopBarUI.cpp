#include "TopBarUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

#include "Logic/TimeManager.h"
#include "Logic/ResourcesManager.h"
#include "Logic/HFManager.h"

namespace GUI
{
	TopBarUI::TopBarUI()
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

	void TopBarUI::release()
	{
		//_uiBarsWindow->destroy();
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
		using namespace Logic;

		Logic::TimeManager& tm = Logic::TimeManager::getSingleton();
		tm.tick(msecs);

		float total_seconds = tm.getTime() / 1000.0f;
		unsigned int minutes = static_cast<unsigned int>(trunc(total_seconds / 60.0f));
		unsigned int seconds = static_cast<unsigned int>(((total_seconds / 60.0f) - minutes) * 60.0f);

		// Changing Resources UI
		
		Logic::ResourcesManager& rm = Logic::ResourcesManager::getSingleton();
		Logic::HFManager& hf = Logic::HFManager::getSingleton();

		_uiBarsWindow->getChild("Mineral")->setText("Mineral  " + std::to_string(static_cast<int>(trunc(rm.getDisplayedResources(ResourceType::MINERAL)))));
		_uiBarsWindow->getChild("Gas")->setText("Gas  " + std::to_string(static_cast<int>(trunc(rm.getDisplayedResources(ResourceType::GAS)))));
		_uiBarsWindow->getChild("Coke")->setText("Coke  " + std::to_string(static_cast<int>(trunc(rm.getDisplayedResources(ResourceType::COKE)))));
		_uiBarsWindow->getChild("Crude")->setText("Crude  " + std::to_string(static_cast<int>(trunc(rm.getDisplayedResources(ResourceType::CRUDE)))));
		_uiBarsWindow->getChild("Pure")->setText("Pure  " + std::to_string(static_cast<int>(trunc(rm.getDisplayedResources(ResourceType::PURE)))));
		_uiBarsWindow->getChild("Refined")->setText("Refined  " + std::to_string(static_cast<int>(trunc(rm.getDisplayedResources(ResourceType::REFINED)))));
		_uiBarsWindow->getChild("TimeLeft")->setText("Time:  " + std::to_string(minutes) + ":" + ((seconds > 9) ? std::to_string(seconds) : "0" + std::to_string(seconds)));
		_uiBarsWindow->getChild("HadesFavor")->setText("HF: " + std::to_string(static_cast<int>(trunc(hf.getHadesFavor()))));
	}

}