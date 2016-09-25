#ifndef TOPBARUI_H_
#define TOPBARUI_H_

#include <string>

namespace CEGUI
{
	class Window;
}

namespace Logic
{
	class ResourcesManager;
	class HFManager;
	class CTimeManager;
}

namespace GUI
{
	class TopBarUI
	{
	protected:
		// Window for displaying resources
		CEGUI::Window* _uiBarsWindow;

		// Blinking resources
		bool _blinking = false;
		CEGUI::Window* _blinkingMineralFrameWindow = nullptr;
		CEGUI::Window* _blinkingGasFrameWindow = nullptr;
		CEGUI::Window* _blinkingPowerFrameWindow = nullptr;
		CEGUI::Window* _blinkingEvilFrameWindow = nullptr;
		CEGUI::Window* _blinkingHadesFavorFrameWindow = nullptr;

		int _blinkingTickLimit = 500;
		int _blinkingTickCount = _blinkingTickLimit;

		std::string buttonFrame = "TrueInfernoUIBars/ButtonFrame";
		std::string buttonFrameWhite = "TrueInfernoUIBars/ButtonFrameWhite";

		// Función auxiliar para el parpadeo
		CEGUI::Window* getFrameWindowFromName(std::string frameName);

	public:
		TopBarUI();
		~TopBarUI();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);

		void resourcesBlinkStart();
		void resourcesBlinkStop();
	};
}

#endif // TOPBARUI_H_