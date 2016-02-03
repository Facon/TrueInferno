/**
@file Graphics.cpp

Contiene la implementaci�n del componente que controla la representaci�n
gr�fica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"

namespace Logic 
{
	RTTI_ROOT_IMPL(CGraphics);
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicsEntity)
		{
			_scene->removeEntity(_graphicsEntity);
			delete _graphicsEntity;
			_graphicsEntity = 0;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		_scene = _entity->getMap()->getScene();

		if (entityInfo->hasAttribute("model"))
			_model = entityInfo->getStringAttribute("model");

		_graphicsEntity = createGraphicsEntity(entityInfo);
		if (!_graphicsEntity)
			return false;

		if (entityInfo->hasAttribute("visible")) {
			bool visible = entityInfo->getBoolAttribute("visible");
			_graphicsEntity->setVisible(visible);
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		bool isStatic = false;
		if(entityInfo->hasAttribute("static"))
			isStatic = entityInfo->getBoolAttribute("static");

		if(isStatic)
		{
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(),_model);
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity))
				return 0;
		}
		else
		{
			_graphicsEntity = new Graphics::CEntity(_entity->getName(),_model);
			if(!_scene->addEntity(_graphicsEntity))
				return 0;
		}

		_graphicsEntity->setTransform(_entity->getTransform());
		_graphicsEntity->setDimensions(_entity->getDimensions());

		if (entityInfo->hasAttribute("material"))
		{
			std::string materialName = entityInfo->getStringAttribute("material");
			_graphicsEntity->setMaterialName(materialName);
		}

		if (entityInfo->hasAttribute("color"))
		{
			Vector3 color = entityInfo->getVector3Attribute("color");
			_graphicsEntity->setColor(color);
		}

		return _graphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------
	/*
	bool CGraphics::accept(const TMessage &message)
	{
		return message._type == Message::SET_TRANSFORM ||
			message._type == Message::SET_DIMENSIONS ||
			message._type == Message::SET_COLOR ||
			message._type == Message::SET_MATERIAL_NAME;

	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::SET_TRANSFORM:
			_graphicsEntity->setTransform(message._transform);
			break;
		case Message::SET_DIMENSIONS:
			_graphicsEntity->setDimensions(message._vector3);
			break;
		case Message::SET_COLOR:
			_graphicsEntity->setColor(message._vector3);
			break;
		case Message::SET_MATERIAL_NAME:
			_graphicsEntity->setMaterialName(message._string);
			break;
		}

	} // process
	*/
} // namespace Logic

