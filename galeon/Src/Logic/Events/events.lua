-- class 'Event'

Event = {}
function Event:new (name)
	local self = setmetatable({}, Event)
	
	self.name = name
	
	self.gameStages = 
	{
		tutorial = 1,
		middle = 1,
		final = 1,
	}
	
	self.roundStages = 
	{
		starting = 1,
		middle = 1,
		ending = 1,
	}
	
	self.godTraits = {}
	self.godTraits.generous = 1
	self.godTraits.clumsy = 1
	self.godTraits.crazy = 1
	self.godTraits.businessman = 1
	self.godTraits.aggressive = 1
	self.godTraits.cheater = 1
	self.godTraits.arrogant = 1
	self.godTraits.vengeful = 1
	
	self.good = 1
	self.evil = 1
	
	return self
end

BuildingDestructionEvent = Event:new("BuildingDestructionEvent")
BuildingDestructionEvent.good = 0
BuildingDestructionEvent.evil = 0

SoulsSpeedReductionEvent = Event:new("SoulsSpeedReductionEvent")
SoulsSpeedReductionEvent.good = 0
SoulsSpeedReductionEvent.evil = 0.8

-- CResearchChallengeEvent
-- CResourceDemandEvent
-- CResourceGiftEvent
-- CResourceStealEvent
-- CSoulHellocaustEvent
-- CSoulStealEvent

events =
{
	BuildingDestructionEvent,
	SoulsSpeedReductionEvent,
}

--[[
for eventIndex,event in pairs(events) 
do 
	for name,value in pairs(event) 
	do
		if type(value)=="table"
		then
			for tname,tvalue in pairs(value)
			do
				print(tname,tvalue)
				end
		else
			print(name,value)
		end
	end
end
--]]

print ("events loaded!")