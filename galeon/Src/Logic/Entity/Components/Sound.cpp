#include "Sound.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Audio/Server.h"

namespace Logic
{
	RTTI_ROOT_IMPL(Sound);
	IMP_FACTORY(Sound);

	const std::unordered_map<std::string, FMOD_MODE> Sound::modeConversor =
	{
		{ "DEFAULT", FMOD_DEFAULT },
		{ "3D", FMOD_3D },
	};

	Sound::~Sound()
	{}

	bool Sound::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		Audio::CServer* singleton = Audio::CServer::getSingletonPtr();

		if (entityInfo->hasAttribute("sound_file"))
		{
			std::string soundFile = entityInfo->getStringAttribute("sound_file");
			int soundMode;

			if (entityInfo->hasAttribute("sound_mode"))
			{
				soundMode = modeConversor.at(entityInfo->getStringAttribute("sound_mode"));
			}
			else
			{
				soundMode = modeConversor.at("DEFAULT");
			}

			_sound = singleton->createSound(soundFile, soundMode);
		}

		adjustTransform(_entity->getTransform());

		return true;
	}
	
	void Sound::adjustTransform(const Matrix4& transform)
	{
		Vector3 position;
		Vector3 scale;
		Quaternion rotation;

		transform.decomposition(position, scale, rotation);

		_transform.position.x = position.x;
		_transform.position.y = position.y;
		_transform.position.z = position.z;
		_transform.forward.x = rotation.zAxis().x;
		_transform.forward.y = rotation.zAxis().y;
		_transform.forward.z = rotation.zAxis().z;
		_transform.up.x = rotation.yAxis().x;
		_transform.up.y = rotation.yAxis().y;
		_transform.up.z = rotation.yAxis().z;
	}
	
	bool Sound::HandleMessage(const TransformMessage& m)
	{
		adjustTransform(m._transform);
		
		return true;
	}

	bool Sound::HandleMessage(const SoundMessage& m)
	{
		Audio::CServer* singleton = Audio::CServer::getSingletonPtr();
		singleton->playSound(_sound);

		return true;
	}
}