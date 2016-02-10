#include "ResourcesUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"

namespace GUI
{
	ResourcesUI::ResourcesUI()
	{
	}


	ResourcesUI::~ResourcesUI()
	{
	}

	CEGUI::Window* ResourcesUI::getResourceWindow(){
		return _uibarsWindow;
	}

	void ResourcesUI::init()
	{
		// Cargamos la ventana que muestra la barra superior
		_uibarsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIResourceBar.layout");
	}

	void ResourcesUI::activate()
	{
		// Activamos la interfaz de usuario
		_uibarsWindow->setVisible(true);
		_uibarsWindow->activate();
	}

	void ResourcesUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uibarsWindow->deactivate();
		_uibarsWindow->setVisible(false);
	}

	void ResourcesUI::tick(unsigned int msecs)
	{
		_timeManager.tick(msecs);

		float total_seconds = _timeManager.getTime() / 1000.0f;
		unsigned int minutes = static_cast<unsigned int>(trunc(total_seconds / 60.0f));
		unsigned int seconds = static_cast<unsigned int>(((total_seconds / 60.0f) - minutes) * 60.0f);

		// Changing Resources UI
		
		_uibarsWindow->getChild("Mineral")->setText("Mineral  " + std::to_string(static_cast<int>(trunc(_resourceManager.getMineral()))));
		_uibarsWindow->getChild("Gas")->setText("Gas  " + std::to_string(static_cast<int>(trunc(_resourceManager.getGas()))));
		_uibarsWindow->getChild("Coke")->setText("Coke  " + std::to_string(static_cast<int>(trunc(_resourceManager.getCoke()))));
		_uibarsWindow->getChild("Crude")->setText("Crude  " + std::to_string(static_cast<int>(trunc(_resourceManager.getCrude()))));
		_uibarsWindow->getChild("Pure")->setText("Pure  " + std::to_string(static_cast<int>(trunc(_resourceManager.getPure()))));
		_uibarsWindow->getChild("Refined")->setText("Refined  " + std::to_string(static_cast<int>(trunc(_resourceManager.getRefined()))));
		_uibarsWindow->getChild("TimeLeft")->setText("Time:  " + std::to_string(minutes) + ":" + ((seconds > 9) ? std::to_string(seconds) : "0" + std::to_string(seconds)));
		_uibarsWindow->getChild("HadesFavor")->setText("HF: " + std::to_string(static_cast<int>(trunc(_resourceManager.getHadesFavor()))));
	}

}