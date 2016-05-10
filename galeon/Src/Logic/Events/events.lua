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
BuildingDestructionEvent.throw = function()
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	CEventManager.getSingletonPtr():addTimeEvent(CBuildingDestructionEvent(timeToLaunch));
end

SoulsSpeedReductionEvent = Event:new("SoulsSpeedReductionEvent")
SoulsSpeedReductionEvent.good = 0
SoulsSpeedReductionEvent.evil = 0.4
SoulsSpeedReductionEvent.godTraits.cheater = 1.0
SoulsSpeedReductionEvent.throw = function()
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent(timeToLaunch, 0.25, 10*1000, true));
end

SoulSpeedIncreaseEvent = Event:new("SoulSpeedIncreaseEvent")
SoulSpeedIncreaseEvent.good = 0.3
SoulSpeedIncreaseEvent.evil = 0
SoulSpeedIncreaseEvent.godTraits.generous = 1.0
SoulSpeedIncreaseEvent.throw = function()
	local timeToLaunch = CTimeManager.getSingletonPtr():getElapsedGlboalTime() + 100
	CEventManager.getSingletonPtr():addTimeEvent(CSoulSpeedAlterationEvent(timeToLaunch, 4, 10*1000, true));
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
	SoulsSpeedReductionEvent,
	SoulSpeedIncreaseEvent,
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