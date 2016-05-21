local launchInitialEvents = false;

if launchInitialEvents then
	print("[LUA] Loading InitialEvents.lua...");
	
	-- Time events
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(40 * 1000));
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
