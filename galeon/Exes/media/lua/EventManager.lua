-- Gestor de eventos del juego. Será el encargado de crear y lanzar eventos en
-- función del estado de la partida y ciertas acciones o condiciones.

-- @author Raúl Segura

-- Clase EventManager
class 'EventManager'

function EventManager:__init()
    -- Constructor
	print("[LUA] EventManager:__init");
end

-- Creación del EventManager global
eventManager = EventManager();
