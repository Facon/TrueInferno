-- class 'Event'

if godTraitsIncluded == nil
then
	if loadFromCpp ~= nil
	then
		dofile("media/lua/godTraits.lua")
	else
		dofile("godTraits.lua")
	end
end

Event = {}
function Event:new (name)
	local self = setmetatable({}, Event)
	
	-- Nombre del evento
	self.name = name

	-- Función para lanzar el evento en C++. Debe ser implementada por cada evento
	self.throw = function() end
	
	-- Fases del juego en las que aplica
	self.gameStages = 
	{
		tutorial = 0.5,
		middle = 0.5,
		final = 0.5,
	}
	
	-- Fases de la ronda actual en las que aplica
	self.roundStages = 
	{
		starting = 0.5,
		middle = 0.5,
		ending = 0.5,
	}
	
	-- Por defecto, los eventos permiten ser lanzados por cualquiera
	self.allowsBoss = true
	self.allowsNonBoss = true
	
	--[[
	Inicializamos la afinidad del evento a cada uno de los rasgos de los dioses.
	Por defecto no hay afinidad especial del evento por un rasgo dado. Hay que definirlo para cada evento-rasgo
	--]]
	self.godTraits = {}
	for godTraitIndex,godTrait in pairs(godTraits)
	do
		self.godTraits[godTrait.name] = 0
	end

	-- Cómo de bueno es el evento por defecto
	self.good = 0
	
	-- Cómo de malo es el evento por defecto
	self.evil = 0
	
	return self
end

-- Definición de los eventos
BuildingDestructionEvent = Event:new("BuildingDestructionEvent")
BuildingDestructionEvent.good = 0
BuildingDestructionEvent.evil = 1.0
BuildingDestructionEvent.godTraits.aggressive = 1.0
BuildingDestructionEvent.allowsBoss = true
BuildingDestructionEvent.allowsNonBoss = true
BuildingDestructionEvent.buildingType = BuildingManager.BT_UNASSIGNED
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
BuildingDestructionEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Building Destruction"
	local description = "A building has been destroyed!"
	local image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent.addCBuildingDestructionEvent(
		timeToLaunch, god.name, title, description, image, BuildingDestructionEvent.buildingType, true));
end

MinorSoulSpeedReductionEvent = Event:new("MinorSoulSpeedReductionEvent")
MinorSoulSpeedReductionEvent.good = 0
MinorSoulSpeedReductionEvent.evil = 0.5
MinorSoulSpeedReductionEvent.godTraits.cheater = 1.0
MinorSoulSpeedReductionEvent.allowsBoss = true
MinorSoulSpeedReductionEvent.allowsNonBoss = true
MinorSoulSpeedReductionEvent.duration = 20*1000
MinorSoulSpeedReductionEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Useless souls"
	local description = "The movement and gathering speed of your souls has been decreased, but don't worry, they will be fine again soon...hopefully..."
	local image = "EventSpeedDown"
	
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(
		timeToLaunch, god.name, title, description, image, 0.25, MinorSoulSpeedReductionEvent.duration, true));
end

MajorSoulSpeedReductionEvent = Event:new("MajorSoulSpeedReductionEvent")
MajorSoulSpeedReductionEvent.good = 0
MajorSoulSpeedReductionEvent.evil = 0.7
MajorSoulSpeedReductionEvent.godTraits.cheater = 1.0
MajorSoulSpeedReductionEvent.allowsBoss = true
MajorSoulSpeedReductionEvent.allowsNonBoss = true
MajorSoulSpeedReductionEvent.duration = 20*1000
MajorSoulSpeedReductionEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Useless souls"
	local description = "The movement and gathering speed of your souls has been decreased, but don't worry, they will be fine again soon...hopefully..."
	local image = "EventSpeedDown"
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(
		timeToLaunch, god.name, title, description, image, 0.1, MajorSoulSpeedReductionEvent.duration, true));
end

MinorSoulSpeedIncreaseEvent = Event:new("MinorSoulSpeedIncreaseEvent")
MinorSoulSpeedIncreaseEvent.good = 0.5
MinorSoulSpeedIncreaseEvent.evil = 0
MinorSoulSpeedIncreaseEvent.godTraits.generous = 1.0
MinorSoulSpeedIncreaseEvent.allowsBoss = true
MinorSoulSpeedIncreaseEvent.allowsNonBoss = false
MinorSoulSpeedIncreaseEvent.duration = 20*1000
MinorSoulSpeedIncreaseEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Useful souls"
	local description = "The movement and gathering speed of your souls has been temporally increased! Go, souls, go!"
	local image = "EventSpeedUp"
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(
		timeToLaunch, god.name, title, description, image, 2, MinorSoulSpeedIncreaseEvent.duration, true));
end

MajorSoulSpeedIncreaseEvent = Event:new("MajorSoulSpeedIncreaseEvent")
MajorSoulSpeedIncreaseEvent.good = 0.7
MajorSoulSpeedIncreaseEvent.evil = 0
MajorSoulSpeedIncreaseEvent.godTraits.generous = 1.0
MajorSoulSpeedIncreaseEvent.allowsBoss = true
MajorSoulSpeedIncreaseEvent.allowsNonBoss = false
MajorSoulSpeedIncreaseEvent.duration = 20*1000
MajorSoulSpeedIncreaseEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Useful souls"
	local description = "The movement and gathering speed of your souls has been temporally increased! Go, souls, go!"
	local image = "EventSpeedUp"
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(
		timeToLaunch, god.name, title, description, image, 4, MajorSoulSpeedIncreaseEvent.duration, true));
end

MinorGodBoostEvent = Event:new("MinorGodBoostEvent")
MinorGodBoostEvent.good = 0
MinorGodBoostEvent.evil = 0.1
MinorGodBoostEvent.godTraits.cheater = 1.0
MinorGodBoostEvent.allowsBoss = false
MinorGodBoostEvent.allowsNonBoss = true
MinorGodBoostEvent.duration = 20*1000
MinorGodBoostEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "God Boost"
	local description = god.name .. " is cheating! Now " .. god.genre .. " will get Hades Favour faster for a while..."
	local image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(
		timeToLaunch, god.name, title, description, image, 1, MinorGodBoostEvent.duration, true));
end

MajorGodBoostEvent = Event:new("MajorGodBoostEvent")
MajorGodBoostEvent.good = 0
MajorGodBoostEvent.evil = 0.3
MajorGodBoostEvent.godTraits.cheater = 1.0
MajorGodBoostEvent.allowsBoss = false
MajorGodBoostEvent.allowsNonBoss = true
MajorGodBoostEvent.duration = 20*1000
MajorGodBoostEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "God Boost"
	local description = god.name .. " is cheating! Now " .. god.genre .. " will get Hades Favour faster for a while..."
	local image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(
		timeToLaunch, god.name, title, description, image, 2, MajorGodBoostEvent.duration, true));
end

MinorGodPenalizationEvent = Event:new("MinorGodPenalizationEvent")
MinorGodPenalizationEvent.good = 0.1
MinorGodPenalizationEvent.evil = 0
MinorGodPenalizationEvent.godTraits.clumsy = 1.0
MinorGodPenalizationEvent.allowsBoss = false
MinorGodPenalizationEvent.allowsNonBoss = true
MinorGodPenalizationEvent.duration = 20*1000
MinorGodPenalizationEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "God Penalization"
	local description = god.name .. " has problems! Now " .. god.genre .. " will get Hades Favour slower for a while..."
	local image = "EventObstacles"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(
		timeToLaunch, god.name, title, description, image, -0.5, MinorGodPenalizationEvent.duration, true));
end

MajorGodPenalizationEvent = Event:new("MajorGodPenalizationEvent")
MajorGodPenalizationEvent.good = 0.3
MajorGodPenalizationEvent.evil = 0
MajorGodPenalizationEvent.godTraits.clumsy = 1.0
MajorGodPenalizationEvent.allowsBoss = false
MajorGodPenalizationEvent.allowsNonBoss = true
MajorGodPenalizationEvent.duration = 20*1000
MajorGodPenalizationEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "God Penalization"
	local description = god.name .. " has problems! Now " .. god.genre .. " will get Hades Favour slower for a while..."
	local image = "EventObstacles"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(
		timeToLaunch, god.name, title, description, image, -0.9, MajorGodPenalizationEvent.duration, true));
end

MinorResourceLossEvent = Event:new("MinorResourceLossEvent")
MinorResourceLossEvent.good = 0
MinorResourceLossEvent.evil = 0.5
MinorResourceLossEvent.godTraits.cheater = 1.0
MinorResourceLossEvent.allowsBoss = true
MinorResourceLossEvent.allowsNonBoss = true
MinorResourceLossEvent.throw = function(god,delay)
	-- Elegir aleatoriamente gas o mineral
	local resourceType = ResourcesManager.RT_NONE
	local resourceText = ""
	if(math.random() > 0.5)
	then
		resourceText = "mineral"
		resourceType = ResourcesManager.RT_MINERAL
	else
		resourceText = "gas"
		resourceType = ResourcesManager.RT_GAS
	end
	
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Resource Lost"
	local description = god.name .. " has made you lose some of your " .. resourceText .. " reserves"
	local image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, -0.2, 0, resourceType, true));
end

MajorResourceLossEvent = Event:new("MajorResourceLossEvent")
MajorResourceLossEvent.good = 0
MajorResourceLossEvent.evil = 0.7
MajorResourceLossEvent.godTraits.cheater = 1.0
MajorResourceLossEvent.allowsBoss = true
MajorResourceLossEvent.allowsNonBoss = true
MajorResourceLossEvent.throw = function(god,delay)
	-- Elegir aleatoriamente gas o mineral
	local resourceType = ResourcesManager.RT_NONE
	local resourceText = ""
	if(math.random() > 0.5)
	then
		resourceText = "mineral"
		resourceType = ResourcesManager.RT_MINERAL
	else
		resourceText = "gas"
		resourceType = ResourcesManager.RT_GAS
	end
	
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Resource Lost"
	local description = god.name .. " has made you lose some of your " .. resourceText .. " reserves"
	local image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, -0.5, 0, resourceType, true));
end

MinorResourceStealEvent = Event:new("MinorResourceStealEvent")
MinorResourceStealEvent.good = 0
MinorResourceStealEvent.evil = 0.6
MinorResourceStealEvent.godTraits.cheater = 1.0
MinorResourceStealEvent.godTraits.businessman = 1.0
MinorResourceStealEvent.allowsBoss = false
MinorResourceStealEvent.allowsNonBoss = true
MinorResourceStealEvent.throw = function(god,delay)
	-- Elegir aleatoriamente gas o mineral
	local resourceType = ResourcesManager.RT_NONE
	local resourceText = ""
	if(math.random() > 0.5)
	then
		resourceText = "mineral"
		resourceType = ResourcesManager.RT_MINERAL
	else
		resourceText = "gas"
		resourceType = ResourcesManager.RT_GAS
	end
	
	-- Pérdida de recursos para el jugador
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Resource Steal"
	local description = god.name .. " has made you lose some of your " .. resourceText .. " reserves"
	local image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, -0.2, 0, resourceType, true));
	
	-- Boost para el dios
	title = "God Boost"
	description = god.name .. " is taking benefit from your resources! Now " .. god.genre .. " will get Hades Favour faster for a while..."
	image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(
		timeToLaunch+3000, god.name, title, description, image, 1, 20*1000, true));
end

MajorResourceStealEvent = Event:new("MajorResourceStealEvent")
MajorResourceStealEvent.good = 0
MajorResourceStealEvent.evil = 0.8
MajorResourceStealEvent.godTraits.cheater = 1.0
MajorResourceStealEvent.godTraits.businessman = 1.0
MajorResourceStealEvent.allowsBoss = false
MajorResourceStealEvent.allowsNonBoss = true
MajorResourceStealEvent.throw = function(god,delay)
	-- Elegir aleatoriamente gas o mineral
	local resourceType = ResourcesManager.RT_NONE
	local resourceText = ""
	if(math.random() > 0.5)
	then
		resourceText = "mineral"
		resourceType = ResourcesManager.RT_MINERAL
	else
		resourceText = "gas"
		resourceType = ResourcesManager.RT_GAS
	end
	
	-- Pérdida de recursos para el jugador
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Resource Steal"
	local description = god.name .. " has made you lose some of your " .. resourceText .. " reserves"
	local image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, -0.5, 0, resourceType, true));
		
	-- Boost para el dios
	title = "God Boost"
	description = god.name .. " is taking benefit from your resources! Now " .. god.genre .. " will get Hades Favour faster for a while..."
	image = "EventBuildingDestruction"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(
		timeToLaunch+3000, god.name, title, description, image, 2, 20*1000, true));
end

MinorResourceGiftEvent = Event:new("MinorResourceGiftEvent")
MinorResourceGiftEvent.good = 0.4
MinorResourceGiftEvent.evil = 0
MinorResourceGiftEvent.godTraits.generous = 1.0
MinorResourceGiftEvent.allowsBoss = true
MinorResourceGiftEvent.allowsNonBoss = true
MinorResourceGiftEvent.throw = function(god,delay)
	-- Elegir aleatoriamente gas o mineral
	local resourceType = ResourcesManager.RT_NONE
	local resourceText = ""
	if(math.random() > 0.5)
	then
		resourceText = "mineral"
		resourceType = ResourcesManager.RT_MINERAL
	else
		resourceText = "gas"
		resourceType = ResourcesManager.RT_GAS
	end
	
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Resource Gift"
	local description = god.name .. " has sent you a gift with some " .. resourceText
	local image = "EventObstacles"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, 0, 0.2, resourceType, true));
end

MajorResourceGiftEvent = Event:new("MajorResourceGiftEvent")
MajorResourceGiftEvent.good = 0.6
MajorResourceGiftEvent.evil = 0
MajorResourceGiftEvent.godTraits.generous = 1.0
MajorResourceGiftEvent.allowsBoss = true
MajorResourceGiftEvent.allowsNonBoss = true
MajorResourceGiftEvent.throw = function(god,delay)
	-- Elegir aleatoriamente gas o mineral
	local resourceType = ResourcesManager.RT_NONE
	local resourceText = ""
	if(math.random() > 0.5)
	then
		resourceText = "mineral"
		resourceType = ResourcesManager.RT_MINERAL
	else
		resourceText = "gas"
		resourceType = ResourcesManager.RT_GAS
	end
	
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Resource Gift"
	local description = god.name .. " has sent you a gift with some " .. resourceText
	local image = "EventObstacles"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, 0, 0.5, resourceType, true));
end

MinorTimeExtendedEvent = Event:new("MinorTimeExtendedEvent")
MinorTimeExtendedEvent.good = 0.6
MinorTimeExtendedEvent.evil = 0
MinorTimeExtendedEvent.allowsBoss = true
MinorTimeExtendedEvent.allowsNonBoss = false
MinorTimeExtendedEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Time Extended"
	local description = god.name .. " has given you a little extra time for this round"
	local image = "EventSpeedUp"
	CEventManager.getSingletonPtr():addTimeEvent(CRoundTimeChangeEvent.addCRoundTimeChangeEvent(
		timeToLaunch, god.name, title, description, image, 5, true));
end

MajorTimeExtendedEvent = Event:new("MajorTimeExtendedEvent")
MajorTimeExtendedEvent.good = 0.8
MajorTimeExtendedEvent.evil = 0
MajorTimeExtendedEvent.allowsBoss = true
MajorTimeExtendedEvent.allowsNonBoss = false
MajorTimeExtendedEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Time Extended"
	local description = god.name .. " has given you HUGE extra time for this round"
	local image = "EventSpeedUp"
	CEventManager.getSingletonPtr():addTimeEvent(CRoundTimeChangeEvent.addCRoundTimeChangeEvent(
		timeToLaunch, god.name, title, description, image, 15, true));
end

MinorTimeShortenedEvent = Event:new("MinorTimeShortenedEvent")
MinorTimeShortenedEvent.good = 0
MinorTimeShortenedEvent.evil = 0.8
MinorTimeShortenedEvent.allowsBoss = true
MinorTimeShortenedEvent.allowsNonBoss = false
MinorTimeShortenedEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Time Shortened"
	local description = god.name .. " has reduced a little this round's time"
	local image = "EventSpeedDown"
	CEventManager.getSingletonPtr():addTimeEvent(CRoundTimeChangeEvent.addCRoundTimeChangeEvent(
		timeToLaunch, god.name, title, description, image, -5, true));
end

MajorTimeShortenedEvent = Event:new("MajorTimeShortenedEvent")
MajorTimeShortenedEvent.good = 0
MajorTimeShortenedEvent.evil = 1
MajorTimeShortenedEvent.allowsBoss = true
MajorTimeShortenedEvent.allowsNonBoss = false
MajorTimeShortenedEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Time Shortened"
	local description = god.name .. " has reduced a LOT this round's time"
	local image = "EventSpeedDown"
	CEventManager.getSingletonPtr():addTimeEvent(CRoundTimeChangeEvent.addCRoundTimeChangeEvent(
		timeToLaunch, god.name, title, description, image, -15, true));
end

MinorObstacleGrowthEvent = Event:new("MinorObstacleGrowthEvent")
MinorObstacleGrowthEvent.good = 0
MinorObstacleGrowthEvent.evil = 0.3
MinorObstacleGrowthEvent.allowsBoss = true
MinorObstacleGrowthEvent.allowsNonBoss = false
MinorObstacleGrowthEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Obstacle Growth"
	local description = "Some obstacles have appeared"
	local image = "EventObstacles"
	CEventManager.getSingletonPtr():addTimeEvent(CObstacleGrowthEvent.addCObstacleGrowthEvent(
		timeToLaunch, god.name, title, description, image, 2, false, true));
end

MajorObstacleGrowthEvent = Event:new("MajorObstacleGrowthEvent")
MajorObstacleGrowthEvent.good = 0
MajorObstacleGrowthEvent.evil = 0.6
MajorObstacleGrowthEvent.allowsBoss = true
MajorObstacleGrowthEvent.allowsNonBoss = false
MajorObstacleGrowthEvent.throw = function(god,delay)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + delay
	local title = "Obstacle Growth"
	local description = "A LOT of obstacles have appeared"
	local image = "EventObstacles"
	CEventManager.getSingletonPtr():addTimeEvent(CObstacleGrowthEvent.addCObstacleGrowthEvent(
		timeToLaunch, god.name, title, description, image, 10, false, true));
end

events =
{
	BuildingDestructionEvent,
	MinorSoulSpeedReductionEvent,
	MajorSoulSpeedReductionEvent,
	MinorSoulSpeedIncreaseEvent,
	MajorSoulSpeedIncreaseEvent,
	MinorGodBoostEvent,
	MajorGodBoostEvent,
	MinorGodPenalizationEvent,
	MajorGodPenalizationEvent,
	MinorResourceLossEvent,
	MinorResourceGiftEvent,
	MajorResourceLossEvent,
	MajorResourceGiftEvent,
	MinorTimeExtendedEvent,
	MajorTimeExtendedEvent,
	MinorTimeShortenedEvent,
	MajorTimeShortenedEvent,
	MinorObstacleGrowthEvent,
	MajorObstacleGrowthEvent,
	MinorResourceStealEvent,
	MajorResourceStealEvent,
}

-- DEBUG
--[[
for eventIndex,event in pairs(events) 
do 
	for name,value in pairs(event) 
	do
		if type(value)=="table"
		then
			for tname,tvalue in pairs(value)
			do
				print(tname,tvalue)
				end
		else
			print(name,value)
		end
	end
end
--]]

print (table.getn(events) .. " events loaded!")