local launchInitialEvents = false;

if launchInitialEvents then
	print("[LUA] Loading InitialEvents.lua...");
	
	-- Time events
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent.addCBuildingDestructionEvent(
	--	25 * 1000, "Osiris", "Title", "Description", "EventBuildingDestruction", true));
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent.addCBuildingDestructionEvent(
	--	35 * 1000, "Osiris", "Title", "Description", "EventBuildingDestruction", true));
		
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(25 * 1000, "", "", "", ""));
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(35 * 1000));
	-- CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(5*1000, 0.25, 20*1000, true, false));
	-- CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(7*1000, 0.25, 20*1000, true, false));
	
	-- Tutorial
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(1));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(2));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(3));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(4));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(5));
	
	-- End game
	-- CEventManager.getSingletonPtr():addConditionEvent(CEndGameEvent(true));	
end

-- TODO Hay que refactorizar los includes y las inicializaciones para poder aprovechar los eventos prehechos sin cargar doblemente los dioses
--dofile("../Src/Logic/Events/events.lua")
--dofile("../Src/Logic/Events/gods.lua")
--MajorSoulSpeedReductionEvent.duration = 7*1000
--MajorSoulSpeedReductionEvent.throw(Hades,15000)
--MajorSoulSpeedIncreaseEvent.duration = 5*1000
--MajorSoulSpeedIncreaseEvent.throw(Hades,20000)