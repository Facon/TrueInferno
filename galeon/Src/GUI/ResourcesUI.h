#ifndef RESOURCESUI_H_
#define RESOURCESUI_H_

#include "Logic/ResourceManager.h"
#include "Logic/TimeManager.h"

namespace CEGUI
{
	class Window;
	class EventArgs;
}

namespace GUI
{
	class ResourcesUI
	{
	protected:
		// Resource Manager used ingame
		Logic::ResourceManager _resourceManager;

		// Time of round
		Logic::TimeManager _timeManager;

		// Window for displaying resources
		CEGUI::Window* _uibarsWindow;

	public:
		ResourcesUI();
		~ResourcesUI();
		void init();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		CEGUI::Window* ResourcesUI::getResourceWindow();
	};
}

#endif // RESOURCESUI_H_