#include "Sound.h"

#include "Logic/Entity/Entity.h"

namespace Logic
{
	Sound::~Sound()
	{}

	bool Sound::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		adjustTransform(_entity->getTransform());
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

	bool HandleMessage(const TransformMessage& m)
	{
		return false;
	}
}