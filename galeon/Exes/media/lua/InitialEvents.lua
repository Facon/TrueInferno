local launchInitialEvents = true;

if launchInitialEvents then
	print("[LUA] Loading InitialEvents.lua...");

	-- BuildingDestructionEvent.throw(Osiris, 10000)

	-- MajorSoulSpeedReductionEvent.duration = 7 * 1000
	-- MajorSoulSpeedReductionEvent.throw(Hades, 20000)

	-- MajorSoulSpeedIncreaseEvent.duration = 5 * 1000
	-- MajorSoulSpeedIncreaseEvent.throw(Hades, 30000)
	
	-- Ronda 1
	MajorSoulSpeedIncreaseEvent.duration = 10 * 1000
	MajorSoulSpeedIncreaseEvent.throw(Hades, 45000)
	
	MajorObstacleGrowthEvent.throw(Hades, 65000)
	
	BuildingDestructionEvent.throw(Hades, 90000)
	
	-- Ronda 2
	BuildingDestructionEvent.throw(Hades, (2 * 60 + 50) * 1000)
	
	BuildingDestructionEvent.throw(Hades, (3 * 60 + 5) * 1000)
	
	BuildingDestructionEvent.throw(Hades, (3 * 60 + 45) * 1000)
	
	MajorSoulsSpeedReductionEvent.duration = 15 * 1000
	MajorSoulsSpeedReductionEvent.throw(Hades, (4 * 60 + 30) * 1000)
	
-- pool de eventos de events.lua:
	-- BuildingDestructionEvent,
	-- MinorSoulSpeedReductionEvent,
	-- MajorSoulsSpeedReductionEvent,
	-- MinorSoulSpeedIncreaseEvent,
	-- MajorSoulSpeedIncreaseEvent,
	-- MinorGodBoostEvent,
	-- MajorGodBoostEvent,
	-- MinorGodPenalizationEvent,
	-- MajorGodPenalizationEvent,
	-- MinorResourceLossEvent,
	-- MinorResourceGiftEvent,
	-- MajorResourceLossEvent,
	-- MajorResourceGiftEvent,
	-- MinorTimeExtendedEvent,
	-- MajorTimeExtendedEvent,
	-- MinorTimeShortenedEvent,
	-- MajorTimeShortenedEvent,
	-- MinorObstacleGrowthEvent,
	-- MajorObstacleGrowthEvent,
	-- MinorResourceStealEvent,
	-- MajorResourceStealEvent,
		
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(25 * 1000, "", "", "", ""));
	-- CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(35 * 1000));
	-- CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(5*1000, 0.25, 20*1000, true, false));
	-- CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(7*1000, 0.25, 20*1000, true, false));
	
	-- Tutorial
	-- CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(1));
	-- CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(2));
	-- CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(3));
	-- CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(4));
	-- CEventManager.getSingletonPtr():addConditionEvent(CTutorialEvent.addCTutorialEvent(5));
	
	-- End game
	-- CEventManager.getSingletonPtr():addConditionEvent(CEndGameEvent(true));
end