/**
@file AnimatedGraphics.cpp

Contiene la implementaci�n del componente que controla la representaci�n
gr�fica de una entidad est�tica.
 
@see Logic::CAnimatedGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "AnimatedGraphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"

namespace Logic 
{
	RTTI_ROOT_IMPL(CAnimatedGraphics);
	IMP_FACTORY(CAnimatedGraphics);
	
	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		_animatedGraphicsEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model);
		if(!_scene->addEntity(_animatedGraphicsEntity))
			return 0;

		_animatedGraphicsEntity->setTransform(_entity->getTransform());
		
		if(entityInfo->hasAttribute("defaultAnimation"))
		{
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
			_animatedGraphicsEntity->setObserver(this);
		}

		return _animatedGraphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------
	/*
	bool CAnimatedGraphics::accept(const TMessage &message)
	{
		return CGraphics::accept(message) ||
			   message._type == Message::SET_ANIMATION ||
			   message._type == Message::STOP_ANIMATION;

	} // accept
	
	//---------------------------------------------------------

	void CAnimatedGraphics::process(const TMessage &message)
	{
		CGraphics::process(message);

		switch(message._type)
		{
		case Message::SET_ANIMATION:
			// Paramos todas las animaciones antes de poner una nueva.
			// Un control m�s sofisticado deber�a permitir interpolaci�n
			// de animaciones. Galeon no lo plantea.
			_animatedGraphicsEntity->stopAllAnimations();
			_animatedGraphicsEntity->setAnimation(message._string,message._bool);
			break;
		case Message::STOP_ANIMATION:
			_animatedGraphicsEntity->stopAnimation(message._string);
			break;
		}

	} // process
	*/
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		// Si acaba una animaci�n y tenemos una por defecto la ponemos
		_animatedGraphicsEntity->stopAllAnimations();
		_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
	}

} // namespace Logic

