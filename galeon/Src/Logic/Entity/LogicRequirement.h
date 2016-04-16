#ifndef LOGIC_REQUIREMENT_H_
#define LOGIC_REQUIREMENT_H_

#include <string>
#include <cassert>

namespace Logic {
	enum LogicRequirement {
		Workers,
		Energy,
		Player,
		Undefined,	// Valor especial indefinido
		All,		// Valor especial que simboliza todos los requisitos
	};

	static LogicRequirement parseLogicRequirement(const std::string& name){
		if (name == "Workers")
			return Workers;

		else if (name == "Energy")
			return Energy;

		else if (name == "Player")
			return Player;

		else if (name == "All")
			return All;

		else{
			assert(false && "LogicRequirement name unknown");
			return Undefined;
		}
	}

	static std::string printLogicRequirement(LogicRequirement requirement){
		switch (requirement){
		case Workers:
			return "Workers";
	
		case Energy:
			return "Energy";

		case Player:
			return "Player";

		case All:
			return "All";

		case Undefined:
			return "Undefined";

		default:
			assert(false && "Undefined LogicRequirement name");
			return "";
		}
	}

} // namespace Logic

#endif // LOGIC_REQUIREMENT_H_