-- Gestor de la IA del juego.

-- @author �lvaro Valera

-- Clase AIManager
class 'AIManager'

-- Configuraci�n de inclusi�n de ficheros. Cargando desde C++ las rutas son relativas a una ruta definida en el ScriptManager
loadFromCpp = 1

-- TODO usar require para no cargar m�dulos duplicados
-- Carga de m�dulos desde la ruta base definida en C++
dofile("../Src/Logic/Events/gods.lua")
dofile("../Src/Logic/Events/events.lua")
dofile("../Src/Logic/Events/eviluators.lua")

--AIManager = {}
function AIManager:__init()
    -- Constructor
	print("[LUA] AIManager:__init");

	-- Aleatorizaci�n de la semilla para los n�meros aleatorios
	math.randomseed( os.time() )

	-- Tiempo (ms) hasta el primer evento aleatorio
	self.timeUntilFirstEvent = 15000

	-- Tiempo (ms) m�nimo entre eventos aleatorios
	self.minTimeBetweenEvents = 15000

	-- Tiempo (ms) m�ximo entre eventos aleatorios
	self.maxTimeBetweenEvents = 15000

	-- Tiempo (ms) que debe transcurrir hasta el pr�ximo evento aleatorio. Se inicializa al valor de espera para el primer evento
	self.timeUntilNextEvent = self.timeUntilFirstEvent

	-- Combinaciones de evento-dios
	self.godEvents = {}
	for godIndex,god in pairs(gods) 
	do
		for eventIndex,event in pairs(events) 
		do
			local godEvent = {god = god, event = event}
			table.insert(self.godEvents, godEvent)
		end
	end
	
	-- Inicializamos los tiempos desde el �ltimo evento lanzado por cada tipo. Se deja preparado para cuando se lance evento
	self.timeSinceLastEventType = {}
	for eventIndex,event in pairs(events) 
	do
		self.timeSinceLastEventType[event.name] = self.timeUntilFirstEvent
	end
		
	-- Inicializamos los tiempos desde el �ltimo evento lanzado por cada dios. Se deja preparado para cuando se lance evento
	self.timeSinceLastGod = {}
	for godIndex,god in pairs(gods) 
	do
		self.timeSinceLastGod[god.name] = self.timeUntilFirstEvent
	end
end

-- Funci�n global para llamar al tick del objeto aiManager
function tickAIManager(msecs)
	aiManager:tick(msecs);
end

-- Tick peri�dico con que el AIManager es invocado desde C++
function AIManager:tick(msecs)
	-- Reducimos el tiempo para el pr�ximo evento
	self.timeUntilNextEvent = self.timeUntilNextEvent - msecs

	-- Si toca lanzar
	if self.timeUntilNextEvent <= 0
	then
		-- Seleccionamos un evento-dios a lanzar
		local chosenGodEvent = self:chooseGodEvent()
	
		print("Chosen event! " .. chosenGodEvent.event.name .. " - " .. chosenGodEvent.god.name)

		-- Lanzamos el evento
		chosenGodEvent.event.throw()

		-- Se actualizan los contadores de tiempo en funci�n del evento-dios lanzado
		self:updateTimeCounters(chosenGodEvent)
	end
end

-- Funci�n de actualizaci�n de los contadores de tiempo
function AIManager:updateTimeCounters(chosenGodEvent)
		-- Reiniciamos el tiempo que pasar� hasta el pr�ximo evento de forma aleatoria
		self.timeUntilNextEvent = math.random(self.minTimeBetweenEvents, self.maxTimeBetweenEvents)
		
		-- Actualizamos los tiempos desde el �ltimo evento lanzado por cada tipo. Se deja preparado para el siguiente lanzamiento
		self.timeSinceLastEventType[chosenGodEvent.event.name] = 0
		for eventIndex,event in pairs(events) 
		do
			self.timeSinceLastEventType[event.name] = self.timeSinceLastEventType[event.name] + self.timeUntilNextEvent
		end
					
		-- Actualizamos los tiempos desde el �ltimo evento lanzado por cada dios. Se deja preparado para el siguiente lanzamiento
		self.timeSinceLastGod[chosenGodEvent.god.name] = 0
		for godIndex,god in pairs(gods) 
		do
			self.timeSinceLastGod[god.name] = self.timeSinceLastGod[god.name] + self.timeUntilNextEvent
		end
end

-- Funci�n que determina el evento-dios a lanzar
function AIManager:chooseGodEvent()
	-- Buscamos el evento-dios con mejor score
	local bestScore = 0
	local bestGodEvent = nil

	-- Para cada evento-dios
	for godEventIndex,godEvent in pairs(self.godEvents) 
	do
		-- Inicializamos su score
		local score = 0
		
		-- Acumulamos el score de cada eviluator
		for eviluatorIndex,weightedEviluator in pairs(weightedEviluators)
		do
			-- S�lo aplicamos los eviluators con peso
			if(weightedEviluator.weight > 0)
			then
				score = score + weightedEviluator.weight * weightedEviluator.eviluator.evaluate(godEvent)
			end
		end

		--print("Event! " .. godEvent.event.name .. " - " .. godEvent.god.name .. " -> " .. score)
		
		-- Actualizaci�n del mejor encontrado
		if score > bestScore
		then
			bestScore = score
			bestGodEvent = godEvent
		end
	end
	
	return bestGodEvent
end

-- Creaci�n del AIManager global
aiManager = AIManager();

-- DEBUG Se fuerza el primer tick para poder ver la traza del error con breakpoint en CAIManager::open() de C++
--aiManager:tick(aiManager.timeUntilFirstEvent)