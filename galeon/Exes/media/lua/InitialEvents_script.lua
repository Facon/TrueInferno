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
	
-- "BT_EVILATOR"
-- "BT_EVIL_WORKS"
-- "BT_FURNACE"
-- "BT_GAS_PLANT"
-- "BT_HQ"
-- "BT_MINE"
-- "BT_NON_BUILDING"
-- "BT_POWER_GENERATOR"
-- "BT_REFINERY"
-- "BT_RESEARCH_LABS"
-- "BT_UNASSIGNED"
-- "BT_WAREHOUSE"	

	BuildingDestructionEvent.buildingType = BuildingManager.BT_MINE
	BuildingDestructionEvent.throw(Hades, 90000)
	
	-- Ronda 2
	BuildingDestructionEvent.buildingType = BuildingManager.BT_GAS_PLANT
	BuildingDestructionEvent.throw(Hades, (2 * 60 + 50) * 1000)
	
	BuildingDestructionEvent.buildingType = BuildingManager.BT_UNASSIGNED
	BuildingDestructionEvent.throw(Hades, (3 * 60 + 45) * 1000)
	
	MajorSoulSpeedReductionEvent.duration = 15 * 1000
	MajorSoulSpeedReductionEvent.throw(Hades, (4 * 60 + 30) * 1000)

end