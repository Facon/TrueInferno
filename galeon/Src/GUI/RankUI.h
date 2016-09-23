#ifndef RANKUI_H_
#define RANKUI_H_

#include <string>

namespace CEGUI
{
	class Window;
}

namespace GUI
{
	class RankUI
	{
	protected:
		// Window for displaying resources
		CEGUI::Window* _uiRankWindow;

		// Blinking ranking
		bool _blinking = false;
		CEGUI::Window* _blinkingRankFrameWindow = nullptr;

		int _blinkingTickLimit = 500;
		int _blinkingTickCount = _blinkingTickLimit;

		std::string rankFrameName = "RankFrame";
		std::string rankFrame = "TrueInfernoUIBars/EventFrame";
		std::string rankFrameWhite = "TrueInfernoUIBars/ButtonFrameWhite";

	public:
		int round = 0;
		bool start = true;
		RankUI();
		~RankUI();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void reDrawUI();

		/** Solicita actualización del ranking de dioses */
		void updateGodRank();

		/** Parpadeo del ranking de dioses */
		void godsRankingBlinkStart();
		void godsRankingBlinkStop();
	};
}

#endif // RANKUI_H_