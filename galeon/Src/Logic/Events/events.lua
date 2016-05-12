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

-- CResearchChallengeEvent
-- CResourceDemandEvent
-- CResourceGiftEvent
-- CResourceStealEvent
-- CSoulHellocaustEvent
-- CSoulStealEvent
-- CGodMistakeEvent

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