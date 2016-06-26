-- Flag para habilitar/deshabilitar los eventos scriptados
launchInitialEvents = true

-- Flag para habilitar/deshabilitar fácilmente los eventos de IA
godEventsEnabled = false

if launchInitialEvents then
	print("[LUA] Loading InitialEvents_script.lua...");

-- pool de eventos de events.lua:
	-- BuildingDestructionEvent,
	-- MinorSoulSpeedReductionEvent,
	-- MajorSoulSpeedReductionEvent,
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
	
	-- Ronda 1
	MajorSoulSpeedIncreaseEvent.duration = 10 * 1000
	MajorSoulSpeedIncreaseEvent.throw(Hades, 45000)
	
	MajorObstacleGrowthEvent.throw(Hades, 65000)
	
	BuildingDestructionEvent.throw(Hades, 90000)
	
	-- Ronda 2
	BuildingDestructionEvent.throw(Hades, (2 * 60 + 50) * 1000)
	
	BuildingDestructionEvent.throw(Hades, (3 * 60 + 5) * 1000)
	
	BuildingDestructionEvent.throw(Hades, (3 * 60 + 45) * 1000)
	
	MajorSoulSpeedReductionEvent.duration = 15 * 1000
	MajorSoulSpeedReductionEvent.throw(Hades, (4 * 60 + 30) * 1000)

end