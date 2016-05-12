-- class 'Eviluator'

if godTraitsIncluded == nil
then
	if loadFromCpp ~= nil
	then
		dofile("../Src/Logic/Events/godTraits.lua")
	else
		dofile("godTraits.lua")
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

-- Definición de los eviluators: 
-- Deben tener un nombre y una función evaluate(godEvent) que devuelve un valor decimal entre 0 (utilidad nula) y 1 (utilidad máxima).

-- DifficultyEviluator: Asigna mayor utilidad cuanto más se ajuste la dificultad de la partida al factor de bondad/maldad del evento
DifficultyEviluator = Eviluator:new("DifficultyEviluator")
DifficultyEviluator.evaluate = 
	function(godEvent)
		-- TODO precalcular para todos los eventos en la inicialización
		-- 1 si el evento totalmente malvado (evil=1.0), 0 si el evento es totalmente bueno (good=1.0)
		local eventDifficulty = (1 + godEvent.event.evil - godEvent.event.good) / 2
		
		-- Calculamos la diferencia absoluta entre dificultades y truncamos su valor a 1
		local diff = math.abs(aiManager.desiredDifficulty - eventDifficulty)
		if(diff > 1)
		then
			diff = 1
		end
		
		-- Cuanto más cercanos los valores de dificultad, la diferencia es más próxima a 0, y el complemento a 1 es más próximo a 1
		return 1 - diff
	end
	
-- RandomEviluator: Asigna utilidad al azar
RandomEviluator = Eviluator:new("RandomEviluator")
RandomEviluator.evaluate = 
	function(godEvent)
		return math.random()
	end

-- GameStageEviluator: Asigna utilidad mayor según se adecúe la fase actual del juego a las preferencias del evento o dios
GameStageEviluator = Eviluator:new("GameStageEviluator")
GameStageEviluator.evaluate = 
	function(godEvent)
		return 1 -- TODO
	end

-- GameStageEviluator: Asigna utilidad mayor según se adecúe la fase actual de la ronda a las preferencias del evento o dios
RoundStageEviluator = Eviluator:new("RoundStageEviluator")
RoundStageEviluator.evaluate = 
	function(godEvent) 
		return 1 -- TODO
	end
	
-- TimeSinceLastEventTypeEviluator: Asigna mayor utilidad a los tipos de evento que hace más tiempo que no se lanzan para fomentar variedad de eventos
TimeSinceLastEventTypeEviluator = Eviluator:new("TimeSinceLastEventTypeEviluator")
TimeSinceLastEventTypeEviluator.evaluate = 
	function(godEvent)
		-- Normalización por el valor máximo para obtener [0,1]
		-- TODO Pre-evaluar todo para no calcular la norma N veces
		local maxValue = 0.0001
		for index,event in pairs(events)
		do
			local value = aiManager.timeSinceLastEventType[event.name]
			if value > maxValue
			then
				maxValue = value
			end
		end
	
		return aiManager.timeSinceLastEventType[godEvent.event.name] / maxValue
	end
	
-- TimeSinceLastGodEviluator: Asigna mayor utilidad a los dioses que hace más tiempo que no lanzan evento para fomentar variedad de dioses
TimeSinceLastGodEviluator = Eviluator:new("TimeSinceLastGodEviluator")
TimeSinceLastGodEviluator.evaluate = 
	function(godEvent) 
		-- Normalización por el valor máximo para obtener [0,1]
		-- TODO Pre-evaluar todo para no calcular la norma N veces
		local maxValue = 0.0001
		for index,god in pairs(gods)
		do
			local value = aiManager.timeSinceLastGod[god.name]
			if value > maxValue
			then
				maxValue = value
			end
		end
	
		return aiManager.timeSinceLastGod[godEvent.god.name] / maxValue
	end

-- PersonalityEviluator: Añadimos un eviluator que tiene en cuenta los rasgos de dios y asigna mayor utilidad si el dios es afín a lanzar evento de esas características
PersonalityEviluator = Eviluator:new("PersonalityEviluator")
PersonalityEviluator.evaluate =
	function(godEvent)
		-- Inicializamos score
		local score = 0
	
		-- Para cada rasgo de dios
		for index,godTrait in pairs(godTraits)
		do
			-- Agregamos el producto de la presencia del rasgo en el dios por la del evento
			score = score + godEvent.god.traits[godTrait.name] * godEvent.event.godTraits[godTrait.name]			
		end
		
		-- Devolvemos el score promediado por la cantidad de rasgos
		return score / table.getn(godTraits)
	end
	
-- Configuración de los eviluators a usar y sus pesos relativos (no es necesario que sumen 1)
-- Asignar peso 0 es equivalente a no querer usar el eviluator
weightedEviluators = 
{
	---[[
	{
		eviluator=DifficultyEviluator, 
		weight=1.0,
	},
	--]]
	
	---[[
	{
		eviluator=PersonalityEviluator,
		weight=1.0,
	},
	--]]
	
	--[[	
	{
		eviluator=RandomEviluator, 
		weight=0.1,
	},
	--]]
	
	--[[	
	{
		eviluator=GameStageEviluator, 
		weight=0,
	},
	--]]

	--[[	
	{
		eviluator=RoundStageEviluator, 
		weight=0,
	},
	--]]

	---[[	
	{
		eviluator=TimeSinceLastEventTypeEviluator, 
		weight=0.1,
	},
	--]]

	---[[	
	{
		eviluator=TimeSinceLastGodEviluator, 
		weight=0.2,
	},
	--]]	
}

-- TODO Normalización genérica de los eviluators:
-- 1) Se crea una función preevaluate que hace lo que la función evaulate actual
-- 2) Se obtiene y guarda el valor máximo de la pre-evaluación
-- 3) Se cambia el evaluate para devolver el valor cacheado de la pre-evaluación normalizado por el valor máximo

-- DEBUG
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