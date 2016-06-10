-- Aumentamos la frecuencia de eventos
aiManager.timeUntilFirstEvent = 3000
aiManager.minTimeBetweenEvents = 3000
aiManager.maxTimeBetweenEvents = 3000

-- Se fuerza el primer tick para poder ver la traza del error con breakpoint en CAIManager::open() de C++
aiManager:tick(aiManager.timeUntilFirstEvent)

-- Se usa únicamente el evaluador de tiempo desde el último evento para que se prueben todos
weightedEviluators = 
{
	{
		eviluator=TimeSinceLastEventTypeEviluator, 
		weight=1,
	},
}