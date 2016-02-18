/**
@file Entity.cpp

Contiene la implementaci�n de la clase Entity, que representa una entidad
de juego. Es una colecci�n de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Julio, 2010
*/

#include <Logic/Entity/Components/AnimatedGraphics.h>
#include "Entity.h"

#include "Component.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include "Map/MapEntity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID),
				_map(0), _type(""), _name(""),
				_position(Vector3(0, 0, 0)), _rotation(Vector3(0, 0, 0)),
				_dimensions(Vector3(1, 1, 1)),
				_isPlayer(false), _activated(false)
	{
	} // CEntity
	
	//---------------------------------------------------------

	CEntity::~CEntity()
	{
		assert(!_map && "¡¡Antes de destruir la entidad debe desacoplarse del mapa!!");

		destroyAllComponents();

	} // ~CEntity
	
	//---------------------------------------------------------

	bool CEntity::spawn(CMap *map, const Map::CEntity *entityInfo) 
	{
		// Leemos las propiedades comunes
		_map = map;
		_type = entityInfo->getType();

		if(entityInfo->hasAttribute("name"))
			_name = entityInfo->getStringAttribute("name");

		if (entityInfo->hasAttribute("position"))
			_position = entityInfo->getVector3Attribute("position");

		if (entityInfo->hasAttribute("orientation"))
			_rotation.y = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation"));

		if (entityInfo->hasAttribute("dimensions"))
			_dimensions = entityInfo->getVector3Attribute("dimensions");

		if (entityInfo->getName() == "Camera")
			_isPlayer = true;

		// Inicializamos los componentes
		TComponentList::const_iterator it;

		bool correct = true;

		for( it = _components.begin(); it != _components.end() && correct; ++it )
			correct = (*it)->spawn(this,map,entityInfo) && correct;

		return correct;

	} // spawn

	//---------------------------------------------------------

	bool CEntity::activate() 
	{
		// Si somos jugador, se lo decimos al servidor
		// y nos registramos para que nos informen
		// de los movimientos que debemos realizar
		if (isPlayer())
		{
			CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(this);
		}

		// Activamos los componentes
		TComponentList::const_iterator it;

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;

		for( it = _components.begin(); it != _components.end(); ++it )
			_activated = (*it)->activate() && _activated;

		return _activated;

	} // activate

	//---------------------------------------------------------

	void CEntity::deactivate() 
	{
		// Si �ramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		if (isPlayer())
		{
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(0);
			CServer::getSingletonPtr()->setPlayer(0);
		}

		TComponentList::const_iterator it;

		// Desactivamos los componentes
		for( it = _components.begin(); it != _components.end(); ++it )
			(*it)->deactivate();

		_activated = false;

	} // deactivate

	//---------------------------------------------------------

	void CEntity::tick(unsigned int msecs) 
	{
		TComponentList::const_iterator it;

		for( it = _components.begin(); it != _components.end(); ++it )
			(*it)->tick(msecs);

	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component)
	{
		_components.push_back(component);
		component->setEntity(this);

	} // addComponent

	//---------------------------------------------------------

	bool CEntity::removeComponent(IComponent* component)
	{
		TComponentList::const_iterator it = _components.begin();

		bool removed = false;
		// Buscamos el componente hasta el final, por si aparec�a
		// m�s de una vez... (no tendr�a mucho sentido, pero por si
		// acaso).
		while (it != _components.end()) 
		{
			if (*it == component)
			{
				it = _components.erase(it);
				removed = true;
			}
			else
				++it;
		}
		if (removed)
			component->setEntity(0);
		return removed;

	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents()
	{
		IComponent* c;
		while(!_components.empty())
		{
			c = _components.back();
			_components.pop_back();
			delete c;
		}

	} // destroyAllComponents

	//---------------------------------------------------------

	#define SEND_MESSAGE_TO_ALL_COMPONENTS \
		bool received = false; \
		\
		for (auto it = _components.cbegin(); it != _components.cend(); ++it) \
		{ \
			received |= (*it)->HandleMessage(msg); \
		} \
		\
		return received;

	//---------------------------------------------------------

	bool CEntity::HandleMessage(const TransformMessage& msg)
	{
		updateTransformValuesFromMatrix(msg._transform);
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const PositionMessage& msg)
	{
		_position = msg._position;
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const RotationMessage& msg)
	{
		_rotation = msg._rotation;
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const DimensionsMessage& msg)
	{
		_dimensions = msg._dimensions;
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const ColorMessage& msg)
	{
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const MaterialMessage& msg)
	{
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const AnimationMessage& msg)
    {
		SEND_MESSAGE_TO_ALL_COMPONENTS;
    }

	bool CEntity::HandleMessage(const ControlMessage& msg)
    {
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

    bool CEntity::HandleMessage(const PhysicMessage& msg)
    {
		SEND_MESSAGE_TO_ALL_COMPONENTS;
    }

	bool CEntity::HandleMessage(const TouchMessage& msg)
    {
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const DamageMessage& msg)
    {
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const PlaceMessage& msg)
	{
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const WorkerMessage& msg)
	{
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	bool CEntity::HandleMessage(const WalkSoulPathMessage& msg)
	{
		SEND_MESSAGE_TO_ALL_COMPONENTS;
	}

	//---------------------------------------------------------

	void CEntity::updateTransformValuesFromMatrix(const Matrix4 &transform)
	{
		Vector3 scale;
		Quaternion orientation;

		transform.decomposition(_position, scale, orientation);

		_rotation = Vector3(orientation.getPitch().valueDegrees(),
			orientation.getYaw().valueDegrees(),
			orientation.getRoll().valueDegrees());

		// @TODO _dimensions = Obtener dimensiones en función de la escala
		// Valorar si deberíamos guardar también la escala junto con las dimensiones
		_dimensions = scale * 100;
	}

	//---------------------------------------------------------

	void CEntity::setTransform(const Matrix4 &transform)
	{
		updateTransformValuesFromMatrix(transform);

		TransformMessage m;
		m._type = MessageType::SET_TRANSFORM;
		m._transform = transform;

		m.Dispatch(*this);

	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position)
	{
		_position = position;

		PositionMessage m;
		m._type = MessageType::SET_POSITION;
		m._position = _position;

        m.Dispatch(*this);

	} // setPosition

	//---------------------------------------------------------

	void CEntity::setRotation(const Vector3 &rotation)
	{
		_rotation = rotation;

		RotationMessage m;
		m._type = MessageType::SET_ROTATION;
		m._rotation = _rotation;

		m.Dispatch(*this);

	} // setRotation

	//---------------------------------------------------------

	void CEntity::setDimensions(const Vector3 &dimensions)
	{
		_dimensions = dimensions;

		DimensionsMessage m;
		m._type = MessageType::SET_DIMENSIONS;
		m._dimensions = _dimensions;

        m.Dispatch(*this);

	} // setDimensions

	//---------------------------------------------------------

	Matrix4 CEntity::getTransform() const
	{
		Vector3 scale;
		Quaternion orientation;

		// @TODO scale = Obtener escala en función de las dimensiones
		// Valorar si deberíamos guardar también la escala junto con las dimensiones
		scale = _dimensions / 100;

		orientation = Math::getOrientationFromRadians(
			Radian(_rotation.x), Radian(_rotation.y), Radian(_rotation.z));

		Matrix4 transform;
		transform.makeTransform(_position, scale, orientation);

		return transform;

	} // getTransform

} // namespace Logic
