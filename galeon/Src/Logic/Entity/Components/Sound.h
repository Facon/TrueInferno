#ifndef LOGIC_SOUND_H_
#define LOGIC_SOUND_H_

#include <unordered_map>
#include <FMOD/lowlevel/fmod.hpp>

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

namespace Logic
{
	class Sound : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(Sound);

	public:
		Sound() : IComponent(), _transform({ { 0.0f } })
		{}

		virtual ~Sound();

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool HandleMessage(const TransformMessage& m);
		//bool HandleMessage(const PositionMessage& m);
		//bool HandleMessage(const PositionMessage& m);
		bool HandleMessage(const SoundMessage& m);

	protected:
		static const std::unordered_map<std::string, FMOD_MODE> modeConversor;

		void adjustTransform(const Matrix4& transform);

		FMOD_3D_ATTRIBUTES _transform;
		FMOD::Sound* _sound;
	};

	REG_FACTORY(Sound);
}

#endif //LOGIC_SOUND_H_