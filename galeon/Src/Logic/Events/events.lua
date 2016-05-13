-- class 'Event'

if godTraitsIncluded == nil
then
	if loadFromCpp ~= nil
	then
		dofile("../Src/Logic/Events/godTraits.lua")
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
	
	-- Por defecto, los eventos permiten ser lanzados por el jefe
	self.allowsBoss = true
	
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
BuildingDestructionEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent.addCBuildingDestructionEvent(timeToLaunch, true));
end

MinorSoulsSpeedReductionEvent = Event:new("MinorSoulsSpeedReductionEvent")
MinorSoulsSpeedReductionEvent.good = 0
MinorSoulsSpeedReductionEvent.evil = 0.4
MinorSoulsSpeedReductionEvent.godTraits.cheater = 1.0
MinorSoulsSpeedReductionEvent.allowsBoss = true
MinorSoulsSpeedReductionEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(timeToLaunch, 0.25, 20*1000, true, false));
end

MajorSpeedReductionEvent = Event:new(MajorSpeedReductionEvent)
MajorSpeedReductionEvent.good = 0
MajorSpeedReductionEvent.evil = 0.7
MajorSpeedReductionEvent.godTraits.cheater = 1.0
MajorSpeedReductionEvent.allowsBoss = true
MajorSpeedReductionEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(timeToLaunch, 0.1, 20*1000, true, false));
end

MinorSoulSpeedIncreaseEvent = Event:new("MinorSoulSpeedIncreaseEvent")
MinorSoulSpeedIncreaseEvent.good = 0.3
MinorSoulSpeedIncreaseEvent.evil = 0
MinorSoulSpeedIncreaseEvent.godTraits.generous = 1.0
MinorSoulSpeedIncreaseEvent.allowsBoss = true
MinorSoulSpeedIncreaseEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(timeToLaunch, 2, 20*1000, true, false));
end

MajorSoulSpeedIncreaseEvent = Event:new("MajorSoulSpeedIncreaseEvent")
MajorSoulSpeedIncreaseEvent.good = 0.5
MajorSoulSpeedIncreaseEvent.evil = 0
MajorSoulSpeedIncreaseEvent.godTraits.generous = 1.0
MajorSoulSpeedIncreaseEvent.allowsBoss = true
MajorSoulSpeedIncreaseEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent.addCSoulSpeedAlterationEvent(timeToLaunch, 4, 20*1000, true, false));
end

MinorGodBoostEvent = Event:new("MinorGodBoostEvent")
MinorGodBoostEvent.good = 0
MinorGodBoostEvent.evil = 0.1
MinorGodBoostEvent.godTraits.cheater = 1.0
MinorGodBoostEvent.allowsBoss = false
MinorGodBoostEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "God Boost"
	local description = god.name .. " is cheating! Now " .. god.genre .. " will get Hades Favour faster for a while..."
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(timeToLaunch, god.name, title, description, image, 1, 20*1000, true));
end

MajorGodBoostEvent = Event:new("MajorGodBoostEvent")
MajorGodBoostEvent.good = 0
MajorGodBoostEvent.evil = 0.3
MajorGodBoostEvent.godTraits.cheater = 1.0
MajorGodBoostEvent.allowsBoss = false
MajorGodBoostEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "God Boost"
	local description = god.name .. " is cheating! Now " .. god.genre .. " will get Hades Favour faster for a while..."
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(timeToLaunch, god.name, title, description, image, 2, 20*1000, true));
end

MinorGodPenalizationEvent = Event:new("MinorGodPenalizationEvent")
MinorGodPenalizationEvent.good = 0.1
MinorGodPenalizationEvent.evil = 0
MinorGodPenalizationEvent.godTraits.clumsy = 1.0
MinorGodPenalizationEvent.allowsBoss = false
MinorGodPenalizationEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "God Penalization"
	local description = god.name .. " has problems! Now " .. god.genre .. " will get Hades Favour slower for a while..."
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(timeToLaunch, god.name, title, description, image, -0.5, 20*1000, true));
end

MajorGodPenalizationEvent = Event:new("MajorGodPenalizationEvent")
MajorGodPenalizationEvent.good = 0.3
MajorGodPenalizationEvent.evil = 0
MajorGodPenalizationEvent.godTraits.clumsy = 1.0
MajorGodPenalizationEvent.allowsBoss = false
MajorGodPenalizationEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "God Penalization"
	local description = god.name .. " has problems! Now " .. god.genre .. " will get Hades Favour slower for a while..."
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CGodBoostEvent.addCGodBoostEvent(timeToLaunch, god.name, title, description, image, -0.9, 20*1000, true));
end

MinorResourceLossEvent = Event:new("MinorResourceLossEvent")
MinorResourceLossEvent.good = 0
MinorResourceLossEvent.evil = 0.5
MinorResourceLossEvent.godTraits.cheater = 1.0
MinorResourceLossEvent.allowsBoss = true
MinorResourceLossEvent.throw = function(god)
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
	
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "Resource Lost"
	local description = god.name .. " has made you lose some of your " .. resourceText .. " reserves"
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, -0.2, 0, resourceType, true));
end

MajorResourceLossEvent = Event:new("MajorResourceLossEvent")
MajorResourceLossEvent.good = 0
MajorResourceLossEvent.evil = 0.7
MajorResourceLossEvent.godTraits.cheater = 1.0
MajorResourceLossEvent.allowsBoss = true
MajorResourceLossEvent.throw = function(god)
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
	
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "Resource Lost"
	local description = god.name .. " has made you lose some of your " .. resourceText .. " reserves"
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, -0.5, 0, resourceType, true));
end

MinorResourceGiftEvent = Event:new("MinorResourceGiftEvent")
MinorResourceGiftEvent.good = 0.4
MinorResourceGiftEvent.evil = 0
MinorResourceGiftEvent.godTraits.generous = 1.0
MinorResourceGiftEvent.allowsBoss = true
MinorResourceGiftEvent.throw = function(god)
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
	
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "Resource Gift"
	local description = god.name .. " has sent you a gift with some " .. resourceText
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, 0, 0.2, resourceType, true));
end

MajorResourceGiftEvent = Event:new("MajorResourceGiftEvent")
MajorResourceGiftEvent.good = 0.6
MajorResourceGiftEvent.evil = 0
MajorResourceGiftEvent.godTraits.generous = 1.0
MajorResourceGiftEvent.allowsBoss = true
MajorResourceGiftEvent.throw = function(god)
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
	
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "Resource Gift"
	local description = god.name .. " has sent you a gift with some " .. resourceText
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CPlayerResourcesChangeEvent.addCPlayerResourcesChangeEvent(
		timeToLaunch, god.name, title, description, image, 0, 0.5, resourceType, true));
end

MinorTimeExtendedEvent = Event:new("MinorTimeExtendedEvent")
MinorTimeExtendedEvent.good = 0.6
MinorTimeExtendedEvent.evil = 0
MinorTimeExtendedEvent.onlyBoss = true
MinorTimeExtendedEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "Time Extended"
	local description = god.name .. " has given you a little extra time for this round"
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CRoundTimeChangeEvent.addCRoundTimeChangeEvent(
		timeToLaunch, god.name, title, description, image, 5, true));
end

MajorTimeExtendedEvent = Event:new("MajorTimeExtendedEvent")
MajorTimeExtendedEvent.good = 0.8
MajorTimeExtendedEvent.evil = 0
MajorTimeExtendedEvent.onlyBoss = true
MajorTimeExtendedEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "Time Extended"
	local description = god.name .. " has given you HUGE extra time for this round"
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CRoundTimeChangeEvent.addCRoundTimeChangeEvent(
		timeToLaunch, god.name, title, description, image, 15, true));
end

MinorTimeShortenedEvent = Event:new("MinorTimeShortenedEvent")
MinorTimeShortenedEvent.good = 0
MinorTimeShortenedEvent.evil = 0.8
MinorTimeShortenedEvent.onlyBoss = true
MinorTimeShortenedEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "Time Shortened"
	local description = god.name .. " has reduced a little this round's time"
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CRoundTimeChangeEvent.addCRoundTimeChangeEvent(
		timeToLaunch, god.name, title, description, image, -5, true));
end

MajorTimeShortenedEvent = Event:new("MajorTimeShortenedEvent")
MajorTimeShortenedEvent.good = 0
MajorTimeShortenedEvent.evil = 1
MajorTimeShortenedEvent.onlyBoss = true
MajorTimeShortenedEvent.throw = function(god)
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	local title = "Time Shortened"
	local description = god.name .. " has reduced a LOT this round's time"
	local image = "EventSoulsSpeedReduction"
	CEventManager.getSingletonPtr():addTimeEvent(CRoundTimeChangeEvent.addCRoundTimeChangeEvent(
		timeToLaunch, god.name, title, description, image, -15, true));
end


-- CResearchChallengeEvent
-- CResourceDemandEvent
-- CResourceStealEvent
-- CSoulHellocaustEvent
-- CSoulStealEvent

events =
{
	BuildingDestructionEvent,
	MinorSoulsSpeedReductionEvent,
	MajorSoulsSpeedReductionEvent,
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