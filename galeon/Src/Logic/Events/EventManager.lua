-- Gestor de eventos del juego. Ser� el encargado de crear y lanzar eventos en
-- funci�n del estado de la partida y ciertas acciones o condiciones.

-- @author Ra�l Segura

-- Clase EventManager
class 'EventManager'

function EventManager:__init()
    -- Constructor
	print("[LUA] EventManager:__init");
end

-- Creaci�n del EventManager global
eventManager = EventManager();
