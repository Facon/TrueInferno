-- class 'Eviluator'

if godTraitsIncluded == nil
then
	if loadFromCpp == nil
	then
		dofile("godTraits.lua")
	else
		dofile("../Src/Logic/Events/godTraits.lua")
	end
end

Eviluator = {}
function Eviluator:new (name)
	local self = setmetatable({}, Eviluator)
	
	-- Nombre del eviluator
	self.name = name
	
	--[[ 
	-- Función que evalúa lo útil o deseable que es un godEvent de cara a decidir cuál lanzar.
	-- Devuelve un valor decimal entre 0 (utilidad nula) y 1 (utilidad máxima).
	-- Aquí se asigna un valor por defecto que devuelve un valor constante. 
	-- Cada evento debería sobreescribir la función.
	--]]
	self.evaluate = function(godEvent) return 1 end
	
	return self
end

-- Definición de los eviluators. Deben tener un nombre y una función evaluate(godEvent)
EvilEviluator = Eviluator:new("EvilEviluator")
EvilEviluator.evaluate = 
	function(godEvent) 
		return godEvent.event.evil 
	end

GoodEviluator = Eviluator:new("GoodEviluator")
GoodEviluator.evaluate = 
	function(godEvent) 
		return godEvent.event.good 
	end

RandomEviluator = Eviluator:new("RandomEviluator")
RandomEviluator.evaluate = 
	function(godEvent) 
		return math.random()
	end

GameStageEviluator = Eviluator:new("GameStageEviluator")
GameStageEviluator.evaluate = 
	function(godEvent) 
		return 1
	end

RoundStageEviluator = Eviluator:new("RoundStageEviluator")
RoundStageEviluator.evaluate = 
	function(godEvent) 
		return 1
	end
	
TimeSinceLastEventTypeEviluator = Eviluator:new("TimeSinceLastEventTypeEviluator")
TimeSinceLastEventTypeEviluator.evaluate = 
	function(godEvent) 
		return aiManager.timeSinceLastEventType[godEvent.event.name]
	end
	
TimeSinceLastGodEviluator = Eviluator:new("TimeSinceLastGodEviluator")
TimeSinceLastGodEviluator.evaluate = 
	function(godEvent) 
		return aiManager.timeSinceLastGod[godEvent.god.name]
	end
	
-- Configuración de los eviluators a usar y sus pesos relativos (no es necesario que sumen 1)
weightedEviluators = 
{
	{
		eviluator=EvilEviluator, 
		weight=1,
	},

	{
		eviluator=GoodEviluator, 
		weight=1,
	},
	
	{
		eviluator=RandomEviluator, 
		weight=0,
	},
	
	{
		eviluator=GameStageEviluator, 
		weight=0,
	},
	
	{
		eviluator=RoundStageEviluator, 
		weight=0,
	},
	
	{
		eviluator=TimeSinceLastEventTypeEviluator, 
		weight=1,
	},
	
	{
		eviluator=TimeSinceLastGodEviluator, 
		weight=1,
	},
}

-- Añadimos un eviluator por rasgo de dios que simboliza la afinidad de un dios a lanzar ciertos eventos
for index,godTrait in pairs(godTraits)
do
	local traitEviluator = Eviluator:new(godTrait.name .. "TraitEviluator")
	
	-- La evaluación es el producto del rasgo del dios por la presencia del rasgo en el evento
	traitEviluator.evaluate =
		function(godEvent)
			return godEvent.god.traits[godTrait.name] * godEvent.event.godTraits[godTrait.name]
		end
		
	-- Por defecto el peso es el mismo para todos los rasgos
	weightedEviluator =
	{
		eviluator = traitEviluator, 
		weight = 1,
	}
		
	-- Por ejemplo, si queremos acentuar el rasgo "aggressive"
	--[[
	if godTrait.name == "aggressive"
	then
		weightedEviluator.weight = 2
	end
	--]]
		
	-- Insertamos el eviluator con su peso
	table.insert(weightedEviluators, weightedEviluator)
end

--[[
for eviluatorIndex,eviluator in pairs(weightedEviluators) 
do 
	for name,value in pairs(eviluator) 
	do
		if type(value) == "table"
		then
			for tname,tvalue in pairs(value)
			do
				print(tname,tvalue)
				end
				
		elseif type(value) == "function"
		then
			print(name .. "()")
		
		else
			print(name .. " = ", value)
		end
	end
end
--]]

print (table.getn(weightedEviluators) .. " eviluators loaded!")