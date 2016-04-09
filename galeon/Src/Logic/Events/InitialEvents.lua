local launchInitialEvents = false;

if launchInitialEvents then
	print("Loading InitialEvents.lua...")
	
	-- Building destruction
	CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(40 * 1000));
	CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(75 * 1000));

	-- Tutorial
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(1));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(2));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(3));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(4));
	CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent(5));

	-- End game
	CEventManager.getSingletonPtr():addConditionEvent(CEndGameEvent(true));
end