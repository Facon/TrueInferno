-- Gestor de la IA del juego.

-- @author �lvaro Valera

dofile("../Src/Logic/Events/gods.lua")
dofile("../Src/Logic/Events/events.lua")
dofile("../Src/Logic/Events/eviluators.lua")

--dofile("gods.lua")
--dofile("events.lua")
--dofile("eviluators.lua")

-- Clase AIManager
class 'AIManager'

--AIManager = {}
function AIManager:__init()
    -- Constructor
	print("[LUA] AIManager:__init");

	-- Aleatorizaci�n de la semilla para los n�meros aleatorios
	math.randomseed( os.time() )

	-- Tiempo (ms) hasta el primer evento aleatorio
	timeUntilFirstEvent = 1000

	-- Tiempo (ms) m�nimo entre eventos aleatorios
	minTimeBetweenEvents = 2000

	-- Tiempo (ms) m�ximo entre eventos aleatorios
	maxTimeBetweenEvents = 3000
	
	-- Tiempo (ms) que debe transcurrir hasta el pr�ximo evento aleatorio. Se inicializa al valor de espera para el primer evento
	timeUntilNextEvent = timeUntilFirstEvent

	-- Combinaciones de evento-dios
	godEvents = {}
	for godIndex,god in pairs(gods) 
	do
		for eventIndex,event in pairs(events) 
		do
			local godEvent = {god = god, event = event}
			table.insert(godEvents, godEvent)
		end
	end
	
end

function AIManager:tick(msecs)
	-- Reducimos el tiempo para el pr�ximo evento
	timeUntilNextEvent = timeUntilNextEvent - msecs

	-- Si toca lanzar
	if timeUntilNextEvent <= 0
	then
		-- TODO Evaluar todos los eventos y decidir cu�l se lanza
	
		-- Escogemos un evento aleatorio
		local randomIndex = math.random(table.getn(godEvents))
		local randomEvent = godEvents[randomIndex]
	
		print("Random event! " .. randomEvent.event.name .. " - " .. randomEvent.god.name)
		
		-- Reiniciamos el tiempo hasta el pr�ximo evento de forma aleatoria
		timeUntilNextEvent = math.random(minTimeBetweenEvents, maxTimeBetweenEvents)
	end
	
end

-- Funci�n global para llamar al tick del objeto aiManager
function tickAIManager(msecs)
	aiManager:tick(msecs);
end

-- Creaci�n del AIManager global
aiManager = AIManager();