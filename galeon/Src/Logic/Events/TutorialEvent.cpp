//---------------------------------------------------------------------------
// TutorialEvent.cpp
//---------------------------------------------------------------------------

/**
@file TutorialEvent.cpp

Implementación de un evento de tipo INFO consistente únicamente en mostrar
uno de los paneles del tutorial.

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#include "TutorialEvent.h"
#include "Logic/TutorialManager.h"
#include "BaseSubsystems/ScriptManager.h"

namespace Logic {

	void CTutorialEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CTutorialEvent, CEvent>("CTutorialEvent")
				.scope
				[
					luabind::def("addCTutorialEvent", &CTutorialEvent::addCTutorialEvent)
				]
			];
	}

	//--------------------------------------------------------

	std::string CTutorialEvent::getGUIImageName() const {
		return _eventImage;
	}

	std::string CTutorialEvent::getGUITitle() const {
		return _eventTitle;
	}

	std::string CTutorialEvent::getGUIText() const {
		return _eventText;
	}

	std::string CTutorialEvent::getGUIResumeText() const {
		return "";
	}
	
	//--------------------------------------------------------

	void CTutorialEvent::execute()
	{
	} // execute
	
	//--------------------------------------------------------
	
	void CTutorialEvent::initGUIConstants()
	{
		switch (_tutorialPanelIndex)
		{
		case TutorialStage::HELLQUARTERS_CLICK:
			_eventImage = "TrueInfernoBuildings/HellQuarters";
			_eventTitle = "Welcome to Hell";
			_eventText  = "Well, well, sweet newbie, welcome to Hell! Not as bad as you imagined it, right? I called you here because I am getting tired of this boring-ass job, so you have been chosen to become my successor. That is, if you can beat the rest of candidates. I will show you how. First, click on the HellQuarters (the biggest building on the map).";
			break;

		case TutorialStage::HELLQUARTERS_CLICK_SOULS_TRIAL:
			_eventImage = "TrueInfernoBuildings/HellQuarters";
			_eventTitle = "HellQuarters";
			_eventText  = "Good, very good! That was an impressive demonstration of skill. Was it hard? Anyway, look at this beautiful building. In the HellQuarters you can decide the fate of the poor souls that fall to hell. You can enslave them to work for you... or you can burn them, ha, ha! Why would you do that? Because It is the way to keep me happy! Click on the left icon.";
			break;

		case TutorialStage::SOULS_TRIAL:
			_eventImage = "TrueInfernoBuildings/Trial";
			_eventTitle = "Souls Trial";
			_eventText  = "Now you will see that there are different types of souls. Some of them are better suited for burning, some others for working... Of course, it is just an indication. They will go where they are told to, like good slaves. If you want them to work for you, type how many of each group you would like to enslave on the boxes of the right column. If you want them to BURN FOR ETERNITY, do the same in the boxes of the left column.";
			break;

		case TutorialStage::REFINED_EVIL_AS_OBJ_1:
			_eventImage = "TrueInfernoBuildings/Trial";
			_eventTitle = "Refine Evil to win";
			_eventText  = "Marvelous, I can already smell the crispy stench in the air. What I demand in this competition is to get as much Refined Evil as possible. That stuff pleases me. When you burn souls, you will get Crude Evil and Coke. Refine that Evil and send it to me. But in order to burn the damned souls, you first need to build some facilities. Select them from the right panel. We will come back to the HellQuarters later.";
			break;

		case TutorialStage::REFINED_EVIL_AS_OBJ_2:
			_eventImage = "TrueInfernoBuildings/Trial";
			_eventTitle = "Refine Evil to win";
			_eventText = "Remember that building facilities will require resources, and their extraction will require some working souls that you must send from the HellQuarters. You remember how, don't you? You can check how much resources of each type you have in the top side of the screen. From left to right: [Mineral] [Gas] [Power] [Crude Evil | Pure Evil | Refined Evil] [Hades'Favor]. I guess you are ready now to start building.";
			break;

		case TutorialStage::ROAD:
			_eventImage = "TrueInfernoUIBars/StartMenu";
			_eventTitle = "Roads";
			_eventText  = "These are roads. You have to use them to connect all of your buildings. If a building is not connected to your city, your worker souls will not be able to reach it, so do not forget to build nice paths. By the way, you will need Mineral for it.";
			break;

		case TutorialStage::MINE:
			_eventImage = "TrueInfernoBuildings/Mine";
			_eventTitle = "Mine";
			_eventText  = "This is a Mine. Mines can only be placed on top of lava pools (the red tiles of the map, obviously). They will extract Mineral from them, which is the main resource required to build other structures. Basic stuff is basic.";
			break;

		case TutorialStage::GAS_PLANT:
			_eventImage = "TrueInfernoBuildings/GasPlant";
			_eventTitle = "Gas Plant";
			_eventText  = "Ah, the Gas Plant. As its name slightly suggests, it extracts Gas from the yellow pools of the map. You will need that Gas to create some buildings and, sometimes, to make them work properly as well.";
			break;

		case TutorialStage::FURNACE:
			_eventImage = "TrueInfernoBuildings/Furnace";
			_eventTitle = "Furnace";
			_eventText = "The good old fire pit has been upgraded to this modern Furnace. Here is where souls burn in agony. Once they have burnt completely, you will be able to gather Crude Evil and Coke. Use the Coke to generate Power for buildings, and send the Crude Evil to the Evilworks in order to be processed for the first time.";
			break;

		case TutorialStage::EVILWORKS:
			_eventImage = "TrueInfernoBuildings/EvilWorks";
			_eventTitle = "Evilworks";
			_eventText = "The Evilworks takes the Crude Evil obtained from the Furnace and converts it into Pure Evil. Congratulations, the product is one step closer to be perfect for delivery. Next step is sending it to the Refinery. Do not forget, buildings need power and workers to be fully functional.";
			break;

		case TutorialStage::REFINERY:
			_eventImage = "TrueInfernoBuildings/Refinery";
			_eventTitle = "Refinery";
			_eventText = "Finally, newbie, this is the last step in the refining process. This building transforms Pure Evil into Refined Evil. Sweet, sweet Refined Evil. Send all that stuff to me through the Evilator (the other initial building, located close to the HellQuarters) and I will grant you loads of Hades Favor, the only score that truly matters in the end. Remember that you can see all resources in the top panel of the screen.";
			break;

		case TutorialStage::POWER_GENERATOR:
			_eventImage = "TrueInfernoBuildings/PowerGenerator";
			_eventTitle = "Power Generator";
			_eventText = "Probably one of the most important structures around here, the Power Generators supply other buildings with... *drums* Power! That's it, your facilities will not work without the Power generated on these. But of course, Power is not free, so you need Coke in order to start the engines. I hope you remember that Coke was produced after burning souls in the Furnace.";
			break;

		case TutorialStage::WAREHOUSE:
			_eventImage = "TrueInfernoBuildings/Warehouse";
			_eventTitle = "Warehouse";
			_eventText = "Got too much Mineral? No problem, you can store it in the Warehouse. Got too much Gas? Try eating less beans. HAHA, I am hilarious... You can store excess of Gas here too. But pay attention, you cannot store Coke, Power or Evil (any kind of it) in the Warehouse.";
			break;

		case TutorialStage::CLEAR_TERRAIN:
			_eventImage = "TrueInfernoUIBars/StartMenu";
			_eventTitle = "Clear Terrain";
			_eventText = "Ruins, obstacles... This is Hell kid. Did you expect it to be a nice place? Use this tool to remove unwanted stuff from the map. Though it is not free either.";
			break;

		case TutorialStage::EVILATOR:
			_eventImage = "TrueInfernoBuildings/Evilator";
			_eventTitle = "Evilator";
			_eventText = "All right then, here is the last building you must know: the Evilator. As I told you before, this structure has been designed to send all your Refined Evil directly to my office. You better take care of it, I definitely would not like to see rubbish coming up... Now click on the Evilator, I just want to make sure that you know what building it is. You can also click on any other building to see its details.";
			break;

		case TutorialStage::RANKING:
			_eventImage = "TrueInfernoBuildings/Missions";
			_eventTitle = "Gods Ranking";
			_eventText = "Well done! So after all these devilish lessons, you are good to go! Finally, take a look at the Gods ranking. There you will see how well you are doing in the competition. Try reaching enough Hades Favor (your score) to avoid the last position, because every round the last one of you will get eliminated. Be the only survivor of this contest and you will become my successor! Good luck, and do not disappoint me...";
			break;

		default:
			assert(false && "No values defined for GUI");
			_eventImage = "";
			_eventTitle = "";
			_eventText = "";
		}
	}

} // namespace Logic
