local launchInitialEvents = false;

if launchInitialEvents then
	print("[LUA] Loading InitialEvents.lua...");
	
	-- Time events
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(40 * 1000));
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(35 * 1000));
	-- CEventManager.getSingletonPtr():addTimeEvent(CSoulsSpeedReductionEvent(100 * 1000, false));

	-- Tutorial
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(1));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(2));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(3));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(4));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(5));
	
	-- End game
	-- CEventManager.getSingletonPtr():addConditionEvent(CEndGameEvent(true));
	
	-- TODO TEST
	-- CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent(15*1000, 0.25, 30*1000, true));
	-- CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent(30*1000, 2, 30*1000, true));
	
end
