#ifndef RANKUI_H_
#define RANKUI_H_



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
	};
}

#endif // RANKUI_H_