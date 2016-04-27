-- class 'God'

God = {}
function God:new (name)
	local self = setmetatable({}, God)
	
	self.name = name
	
	self.generous = 0
	self.clumsy = 0
	self.crazy = 0
	self.businessman = 0
	self.aggressive = 0
	self.cheater = 0
	self.arrogant = 0
	self.vengeful = 0
	
	return self
end

--[[
function God:new (generous, clumsy, crazy, businessman, aggressive, cheater, arrogant)
	self.generous = generous;
	self.clumsy = clumsy;
	self.crazy = crazy;
	self.businessman = businessman;
	self.aggressive = aggressive;
	self.cheater = cheater;
	self.arrogant = arrogant;
end
--]]

--Hades = God:new(0.1, 0.1, 0.5, 0.5, 1, 1, 1);
Hades = God:new("Hades");
Hades.crazy = 1;
Hades.arrogant = 1;
Hades.vengeful = 1;

Lucifer = God:new("Lucifer");
Lucifer.aggressive  = 1;
Lucifer.arrogant = 1;
Lucifer.vengeful = 1;

Osiris = God:new("Osiris");
Osiris.businessman = 1;
Osiris.generous = 1;
Osiris.cheater = 1;

Hel = God:new("Hel");
Hel.crazy = 1;
Hel.arrogant = 1;
Hel.generous = 1;

Cthulhu = God:new("Cthulhu");
Cthulhu.generous = 1;
Cthulhu.clumsy = 1;
Cthulhu.crazy = 1;

gods = 
{
	Hades,
	Lucifer,
	Osiris,
	Hel,
	Cthulhu,
}

--[[
for godIndex,god in pairs(gods) 
do 
	for traitName,traitValue in pairs(god) 
	do
		print(traitName,traitValue) 
	end
end
--]]

print ("gods loaded!")