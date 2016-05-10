-- Gestor de la IA del juego.

-- @author �lvaro Valera

-- Clase AIManager

-- Configuraci�n de inclusi�n de ficheros. Cargando desde C++ las rutas son relativas a una ruta definida en el ScriptManager
-- Comentar para testear directamente en Lua
loadFromCpp = 1

-- Flag para habilitar/deshabilitar f�cilmente la IA
aiEnabled = false

-- TODO usar require para no cargar m�dulos duplicados
-- Carga de m�dulos desde la ruta base definida en C++
if loadFromCpp ~= nil
then
	dofile("../Src/Logic/Events/gods.lua")
	dofile("../Src/Logic/Events/events.lua")
	dofile("../Src/Logic/Events/eviluators.lua")

-- O carga desde la ubicaci�n actual si se ejecuta directamente en Lua
else
	dofile("gods.lua")
	dofile("events.lua")
	dofile("eviluators.lua")
end

-- Inicializaci�n dependiente de si es C++ o Lua directo
if loadFromCpp ~= nil
then
	-- En C++ declaramos el m�dulo como clase
	class 'AIManager'	
else
	-- En Lua directo simplemente creamos una tabla vac�a
	AIManager = {}
end

function AIManager:__init()
    -- Constructor
	print("[LUA] AIManager:__init");

	-- Aleatorizaci�n de la semilla para los n�meros aleatorios
	math.randomseed( os.time() )

	-- Tiempo (ms) hasta el primer evento aleatorio
	self.timeUntilFirstEvent = 30000

	-- Tiempo (ms) m�nimo entre eventos aleatorios
	self.minTimeBetweenEvents = 15000

	-- Tiempo (ms) m�ximo entre eventos aleatorios
	self.maxTimeBetweenEvents = 25000

	-- Tiempo (ms) que debe transcurrir hasta el pr�ximo evento aleatorio. Se inicializa al valor de espera para el primer evento
	self.timeUntilNextEvent = self.timeUntilFirstEvent

	-- Dificultad deseada inicialmente. Debe actualizarse conforme avanza la partida. Ha de estar siempre entre 0 (m�s f�cil) y 1 (m�s dif�cil)
	self.desiredDifficulty = 0.2
	
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

-- Funci�n global para llamar al tick del objeto aiManager desde C++
function tickAIManager(msecs)
	aiManager:tick(msecs)
end

-- Tick peri�dico
function AIManager:tick(msecs)
	-- Reducimos el tiempo para el pr�ximo evento
	self.timeUntilNextEvent = self.timeUntilNextEvent - msecs

	-- Si toca lanzar
	if self.timeUntilNextEvent <= 0
	then
		-- Seleccionamos un evento-dios a lanzar
		local chosenGodEvent = self:chooseGodEvent()
	
		if(chosenGodEvent ~= nil)
		then
			print("Chosen event! " .. chosenGodEvent.event.name .. " - " .. chosenGodEvent.god.name)

			-- Lanzamos el evento
			if(aiEnabled)
			then
				chosenGodEvent.event.throw()
			end

			-- Se actualizan los contadores de tiempo en funci�n del evento-dios lanzado
			self:updateTimeCounters(chosenGodEvent)
	
		else
			print("No event was chosen")
		
		end
		
		-- Se actualiza el nivel de dificultad deseado
		self:updateDifficulty()
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

-- Funci�n de actualizaci�n del nivel de dificultad
function AIManager:updateDifficulty()
	-- TODO Actualizar en funci�n de los puntos del jugador y la curva deseada
	if(CTimeManager.getSingletonPtr():getElapsedGlboalTime() <= 30000)
	then
		self.desiredDifficulty = 0.0

	elseif(CTimeManager.getSingletonPtr():getElapsedGlboalTime() <= 60000)
	then
		self.desiredDifficulty = 0.25
		
	elseif(CTimeManager.getSingletonPtr():getElapsedGlboalTime() <= 120000)
	then
		self.desiredDifficulty = 0.5

	elseif(CTimeManager.getSingletonPtr():getElapsedGlboalTime() <= 180000)
	then
		self.desiredDifficulty = 0.75

	else
		self.desiredDifficulty = 1.0
		
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
		if(score > bestScore)
		then
			bestScore = score
			bestGodEvent = godEvent
		end
	end
	
	return bestGodEvent
end

-- Funci�n global para llamar a eliminateGod del objeto aiManager desde C++
function eliminateGodAIManager(name)
	aiManager:eliminateGod(name)
	aiManager:removeEventsFromGod(name)
end

-- Elimina un dios dado su nombre
function AIManager:eliminateGod(name)
	for index,god in pairs(gods)
	do
		-- Si es el dios buscado
		if(god.name == name)
		then
			-- Lo borramos
			table.remove(gods, index)

			--print("[LUA] God eliminated: " .. god.name)
			
			-- No hay que buscar m�s
			return
		end
	end	
end

-- Elimina los eventos de un dios
function AIManager:removeEventsFromGod(name)
	local eventsRemoved = 0
	
	--print("[LUA] Events before: " .. table.getn(self.godEvents))
	
	-- Iteramos desde el final ya que table.remove aumenta el �ndice y salta un elemento adicional al borrar
	for i=#self.godEvents,1,-1
	do
		-- Si el evento corresponde al dios buscado
		if(self.godEvents[i].god.name == name)
		then
			-- Eliminamos el evento y seguimos buscando
			table.remove(self.godEvents, i)
			eventsRemoved = eventsRemoved + 1
		end
    end
	
	--print("[LUA] Events after: " .. table.getn(self.godEvents))
	--print("[LUA] Events removed from god: " .. eventsRemoved)
end

-- Creaci�n de la "instancia" de AIManager
if loadFromCpp ~= nil
then
	-- Viniendo de C++ se crea la instancia
	aiManager = AIManager()
else
	-- En Lua directo se llama expl�citamente al init
	AIManager:__init()
	
	-- Y "creamos" la instancia apuntando a la tabla global
	aiManager = AIManager
end

-- DEBUG Se fuerza el primer tick para poder ver la traza del error con breakpoint en CAIManager::open() de C++
--aiManager:tick(aiManager.timeUntilFirstEvent)