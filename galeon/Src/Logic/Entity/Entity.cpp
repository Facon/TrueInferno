/**
@file Entity.cpp

Contiene la implementaci�n de la clase Entity, que representa una entidad
de juego. Es una colecci�n de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llans�
@date Julio, 2010
*/

#include <Logic/Entity/Components/AnimatedGraphics.h>
#include "Entity.h"

// Componentes
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
				_transform(Matrix4::IDENTITY), _dimensions(Vector3(1, 1, 1)),
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

		if(entityInfo->hasAttribute("position"))
		{
			Vector3 position = entityInfo->getVector3Attribute("position");
			_transform.setTrans(position);
		}

		if (entityInfo->hasAttribute("dimensions"))
			_dimensions = entityInfo->getVector3Attribute("dimensions");

		// Por comodidad en el mapa escribimos los �ngulos en grados.
		if(entityInfo->hasAttribute("orientation"))
		{
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation"));
			Math::yaw(yaw,_transform);
		}

		if(entityInfo->hasAttribute("isPlayer"))
			_isPlayer = entityInfo->getBoolAttribute("isPlayer");
		

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
    /*
	bool CEntity::emitMessage(const TMessage &message, IComponent* emitter)
	{
		// Interceptamos los mensajes que adem�s de al resto de los
		// componentes, interesan a la propia entidad.
		switch(message._type)
		{
		case Message::SET_TRANSFORM:
			_transform = message._transform;
			break;
		case Message::SET_DIMENSIONS:
			_dimensions = message._vector3;
			break;
		}

		TComponentList::const_iterator it;
		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		for( it = _components.begin(); it != _components.end(); ++it )
		{
			// Al emisor no se le envia el mensaje.
			if( emitter != (*it) )
				anyReceiver = (*it)->set(message) || anyReceiver;
		}
		return anyReceiver;

	} // emitMessage
    */

#define SEND_MESSAGE_TO_ALL_COMPONENTS \
	bool received = false; \
	\
	for (auto it = _components.cbegin(); it != _components.cend(); ++it) \
	{ \
		received |= (*it)->HandleMessage(msg); \
	} \
	\
	return received;

	bool CEntity::HandleMessage(const TransformMessage& msg)
    {
        this->_transform = msg._transform;

		SEND_MESSAGE_TO_ALL_COMPONENTS;
    }

    bool CEntity::HandleMessage(const DimensionsMessage& msg)
    {
        this->_dimensions = msg._dimensions;

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

	//---------------------------------------------------------

	void CEntity::setTransform(const Matrix4& transform) 
	{
        TransformMessage m;

        m._type = MessageType::SET_TRANSFORM;
        m._transform = _transform;

        m.Dispatch(*this);
	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position)  
	{
		_transform.setTrans(position);

		// Avisamos a los componentes del cambio.
		TransformMessage m;

		m._type = MessageType::SET_TRANSFORM;
		m._transform = _transform;

        m.Dispatch(*this);
	} // setPosition

	//---------------------------------------------------------

	void CEntity::setDimensions(const Vector3 &dimensions)
	{
		_dimensions = dimensions;

		// Avisamos a los componentes del cambio.
		DimensionsMessage message;
		message._type = MessageType::SET_DIMENSIONS;
		message._dimensions = _dimensions;

        message.Dispatch(*this);
	} // setScale

	//---------------------------------------------------------

	void CEntity::setOrientation(const Matrix3& orientation) 
	{
		// Avisamos a los componentes del cambio.
		TransformMessage message;
		message._type = MessageType::SET_TRANSFORM;
		message._transform = orientation;

        message.Dispatch(*this);
	} // setOrientation

	//---------------------------------------------------------

	Matrix3 CEntity::getOrientation() const
	{
		Matrix3 orientation;
		_transform.extract3x3Matrix(orientation);
		return orientation;
	} // getOrientation

	//---------------------------------------------------------

	void CEntity::setYaw(float yaw) 
	{
		Math::setYaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		TransformMessage message;
		message._type = MessageType::SET_TRANSFORM;
		message._transform = _transform;

        message.Dispatch(*this);
	} // setYaw

	//---------------------------------------------------------

	void CEntity::yaw(float yaw) 
	{
		Math::yaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		TransformMessage message;
		message._type = MessageType::SET_TRANSFORM;
		message._transform = _transform;

        message.Dispatch(*this);
	} // yaw

} // namespace Logic
