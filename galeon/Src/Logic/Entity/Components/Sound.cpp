#include "Sound.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Audio/Server.h"

namespace Logic
{
	RTTI_ROOT_IMPL(Sound);
	IMP_FACTORY(Sound);

	Sound::~Sound()
	{}

	bool Sound::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		//Audio::CServer* singleton = Audio::CServer::getSingletonPtr();

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

		singleton->playSfxSound(m._soundName);

		return true;
	}
}