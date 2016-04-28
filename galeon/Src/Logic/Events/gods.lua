-- class 'God'

if godTraitsIncluded == nil
then
	if loadFromCpp == nil
	then
		dofile("godTraits.lua")
	else
		dofile("../Src/Logic/Events/godTraits.lua")
	end
end

God = {}
function God:new (name)
	local self = setmetatable({}, God)
	
	-- Nombre del dios
	self.name = name
	
	--[[
	Lista de rasgos del dios. 
	Los rasgos se indexan por nombre y tienen un valor entre 0 (nada marcado) y 1 (totalmente marcado). 
	Inicialmente se asigna a cada rasgo la misma  cantidad fija.
	Cada dios deberá sobreescribir los rasgos que interesen para definir su personalidad
	--]]
	self.traits = {}
	
	-- Inicializamos los rasgos a un valor por defecto bajo no nulo. Así se diferencia del caso de un dios que no queremos que presente un rasgo en absoluto.
	for godTraitIndex,godTrait in pairs(godTraits)
	do
		self.traits[godTrait.name] = 0.2
	end
	
	return self
end

-- Definición de los dioses y sus correspondientes atributos (ver godTraits.lua)
-- TODO Convendría normalizar rasgos en cada dios para evitar que los dioses con muchos rasgos tengan preferencia
Hades = God:new("Hades")
Hades.traits.crazy = 1
Hades.traits.arrogant = 1
Hades.traits.vengeful = 1
Hades.traits.clumsy = 0

Lucifer = God:new("Lucifer")
Lucifer.traits.aggressive = 1
Lucifer.traits.arrogant = 1
Lucifer.traits.vengeful = 1

Osiris = God:new("Osiris")
Osiris.traits.businessman = 1
Osiris.traits.generous = 1
Osiris.traits.cheater = 1

Hel = God:new("Hel")
Hel.traits.crazy = 1
Hel.traits.arrogant = 1
Hel.traits.generous = 1

Cthulhu = God:new("Cthulhu")
Cthulhu.traits.generous = 1
Cthulhu.traits.clumsy = 1
Cthulhu.traits.crazy = 1

gods = 
{
	Hades,
	Lucifer,
	Osiris,
	Hel,
	Cthulhu,
}

-- DEBUG
--[[
for index,god in pairs(gods) 
do 
	print(index)
	print("name = " .. god.name)
	for name,value in pairs(god.traits) 
	do
		print(name .. " = " .. value)
	end
end
--]]

print (table.getn(gods) .. " gods loaded!")