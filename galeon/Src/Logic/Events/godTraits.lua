-- class 'GodTrait'

-- TODO Usar require
godTraitsIncluded = 1

GodTrait = {}
function GodTrait:new (name)
	local self = setmetatable({}, GodTrait)
	
	-- Nombre del rasgo de dios
	self.name = name
	
	return self
end

-- Definición de los rasgos de dios
generous = GodTrait:new("generous")
clumsy = GodTrait:new("clumsy")
crazy = GodTrait:new("crazy")
businessman = GodTrait:new("businessman")
aggressive = GodTrait:new("aggressive")
cheater = GodTrait:new("cheater")
arrogant = GodTrait:new("arrogant")
vengeful = GodTrait:new("vengeful")

-- Configuración de los rasgos de dios que se van a usar
godTraits = 
{
	generous,
	clumsy,
	crazy,
	businessman,
	aggressive,
	cheater,
	arrogant,
	vengeful,
}

print (table.getn(godTraits) .. " godTraits loaded!")