#include "HFManager.h"

#include "GUI\UIManager.h"
#include "GUI\Server.h"

namespace Logic
{
	HFManager HFManager::_instance = HFManager();

	void HFManager::changeHadesFavor(const float hadesFavor) {
		if (hadesFavor == 0)
			return;

		_hadesFavor += hadesFavor;

		/** Actualizamos la GUI => Fresquísimo acoplamiento */
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getRankUI()->updateGodRank();
	}
}