/**
@file Camera.cpp

Contiene la implementaci�n del componente que controla la c�mara gr�fica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Camera.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

namespace Logic 
{
	RTTI_ROOT_IMPL(CCamera);
	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	bool CCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicsCamera = _entity->getMap()->getScene()->getCamera();
		if(!_graphicsCamera)
			return false;

		if(entityInfo->hasAttribute("distance"))
			_distance = entityInfo->getFloatAttribute("distance");

		if(entityInfo->hasAttribute("height"))
			_height = entityInfo->getFloatAttribute("height");

		if(entityInfo->hasAttribute("targetDistance"))
			_targetDistance = entityInfo->getFloatAttribute("targetDistance");

		if(entityInfo->hasAttribute("targetHeight"))
			_targetHeight = entityInfo->getFloatAttribute("targetHeight");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CCamera::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();

		return true;

	} // activate
	
	//---------------------------------------------------------

	void CCamera::deactivate()
	{
		_target = 0;

	} // deactivate
	
	//---------------------------------------------------------

	void CCamera::tick(unsigned int msecs)
	{
		// Updating camera position and direction...
		Vector3 position = Vector3(_entity->getPosition());
		position.x += -8.0f;
		position.y = _height;
		position.z += -8.0f;

		// #perspectiva Para perspectiva isométrica: comentar el primero y activar el segundo
		//_graphicsCamera->setCameraPosition(position + _distance * Vector3(0, 0, 1));
		_graphicsCamera->setCameraPosition(position + _distance * Vector3(1, 0, 1));

		//Vector3 direction = Vector3(200, -5, 200);
		//Vector3 direction = Vector3(0, -5, 0);
		//_graphicsCamera->setTargetCameraPosition(position + direction);

	} // tick

} // namespace Logic

