#include "RankUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include "AI/God.h"
#include "AI/Manager/AIManager.h"

namespace GUI
{
	RankUI::RankUI()
	{
	}


	RankUI::~RankUI()
	{
	}

	void RankUI::init()
	{
		// Cargamos la ventana que muestra la barra superior
		_uiRankWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIRank.layout");
	}

	void RankUI::release()
	{
		//_uiBarsWindow->destroy();
	}

	void RankUI::activate()
	{
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiRankWindow);
		// Activamos la interfaz de usuario
		_uiRankWindow->setVisible(true);
		_uiRankWindow->activate();
		
		// Pintamos por primera vez el ranking
		updateGodRank();
	}

	void RankUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uiRankWindow->deactivate();
		_uiRankWindow->setVisible(false);
	}

	/*void RankUI::updateGodRank()
	{
		std::vector<AI::IRankedCharacter*> ranking = AI::CAIManager::getSingletonPtr()->getGodRanking();

		if (!start)
		{
			int i = 1;
			for (auto it = ranking.begin(); it != ranking.end(); ++it)
			{
				_uiRankWindow->removeChild("God" + std::to_string(i));
				++i;
			}
		}
		else
			start = false;
		float verticalpos = 0.05;
		int i = 1;
		for (auto it = ranking.begin(); it != ranking.end(); ++it)
			{
				_uiRankWindow->createChild("OgreTray/StaticText", "God" + std::to_string(i));
				_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("Area", "{{0.10,0},{" + std::to_string(verticalpos) + ",0},{1,0},{" + std::to_string(verticalpos + 0.05) + ",0}}");
				_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
				_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("Font", "Jura-10");
				_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("AlwaysOnTop", "True");
				_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("VertFormatting", "CentreAligned");
				_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("HorzFormatting", "LeftAligned");
				verticalpos += 0.06;

				_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("Text", std::to_string(i) + ". " + (*it)->getName() + " " + std::to_string((*it)->getScore()));
				++i;
			}
	}*/

	void RankUI::reDrawUI()
	{
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_uiRankWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiRankWindow);

	}

	void RankUI::updateGodRank()
	{
		std::vector<AI::IRankedCharacter*> ranking = AI::CAIManager::getSingletonPtr()->getGodRanking();

		if (start)
		{
			start = false;
			if (round == 0){
				int i = 1;
				float verticalpos = 0.30;
				for (auto it = ranking.begin(); it != ranking.end(); ++it)
				{
					_uiRankWindow->createChild("OgreTray/StaticText", "God" + std::to_string(i));
					_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("Area", "{{0.30,0},{" + std::to_string(verticalpos) + ",0},{1,0},{" + std::to_string(verticalpos + 0.05) + ",0}}");
					_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("Font", "videophreak-10");
					_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("AlwaysOnTop", "True");
					_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("VertFormatting", "CentreAligned");
					_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("HorzFormatting", "LeftAligned");
					_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
					verticalpos += 0.1;
					++i;
				}
			}
			else{	
				int defeatedgod = ranking.size() - round + 1;
				_uiRankWindow->getChild("God" + std::to_string(defeatedgod))->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
			}
		}	

		int i = 1;
		for (auto it = ranking.begin(); it != ranking.end(); ++it)
		{
			_uiRankWindow->getChild("God" + std::to_string(i))->setProperty("Text", (*it)->getName() + " " + std::to_string((*it)->getScore()));
			++i;
		}
	}

	void RankUI::tick(unsigned int msecs)
	{
		// updateGodRank();

		// Blinking ranking
		if (_blinking)
		{
			_blinkingTickCount += msecs;

			if (_blinkingTickCount >= _blinkingTickLimit && _blinkingRankFrameWindow != nullptr)
			{
				_blinkingTickCount = 0;

				bool blinkingNormalFrame = _blinkingRankFrameWindow->getProperty("Image") == rankFrame;
				_blinkingRankFrameWindow->setProperty("Image", blinkingNormalFrame ? rankFrameWhite : rankFrame);
			}
		}
	}

	//--------------------------------------------------------

	void RankUI::godsRankingBlinkStart()
	{
		// Marco del ranking
		CEGUI::Window *rankFrameWindow = _uiRankWindow;

		// Si no se encuentra...
		if (rankFrameWindow == nullptr)
			return;

		// Activar parpadeo y guardar referencia al [marco del] ranking. El cambio de imagen
		// periódico se lleva a cabo en el tick
		_blinking = true;
		_blinkingRankFrameWindow = rankFrameWindow;
		_blinkingTickCount = _blinkingTickLimit;
	}

	void RankUI::godsRankingBlinkStop()
	{
		// Comprobamos que el ranking esté parpadeando
		if (!_blinking)
			return;

		// Marco del ranking
		CEGUI::Window *rankFrameWindow = _uiRankWindow;

		// Si no coincide, que nunca debería pasar...
		if (rankFrameWindow == nullptr || rankFrameWindow != _blinkingRankFrameWindow)
			return;

		// Reestablecer la imagen original del marco
		_blinkingRankFrameWindow->setProperty("Image", rankFrame);

		// Desactivar parpadeo y limpiar referencia al [marco del] ranking
		_blinking = false;
		_blinkingRankFrameWindow = nullptr;
	}

}