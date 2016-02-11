//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CEntity::CEntity(const std::string &name, const std::string &mesh)
		: _entity(0), _entityNode(0), _scene(0), _loaded(false),
		  _meshDimensions(Vector3(100.f, 100.f, 100.f))
	{
		_name = name;
		_mesh = mesh;

	} // CEntity

	//--------------------------------------------------------

	CEntity::~CEntity() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		
	} // ~CEntity
	
	//--------------------------------------------------------
		
	bool CEntity::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡La entidad debe asociarse a una escena!!");
		// Si la entidad está cargada por otro gestor de escena.
		if(_loaded && (_scene != scene))
			return false;

		// Si no est� cargada forzamos su carga.
		if (!_loaded)
		{
			_scene = scene;
			return load();
		}

		// Si ya estaba cargada en la escena se devuelve cierto.
		return true;

	} // attachToScene
	
	//--------------------------------------------------------
		
	bool CEntity::deattachFromScene()
	{
		// Si la entidad no está cargada no se puede quitar de
		// una escena. Ya que no pertenecer� a ninguna.
		if(!_loaded)
			return false;
		// Si la entidad est� cargada forzamos su descarga.
		else
		{
			assert(_scene && "¡¡La entidad debe estar asociada a una escena!!");
			unload();
			_scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	bool CEntity::load()
	{
		try
		{
			_entity = _scene->getSceneMgr()->createEntity(_name, _mesh);
		}
		catch(std::exception e)
		{
			return false;
		}
		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->
								createChildSceneNode(_name + "_node");
		_entityNode->attachObject(_entity);
		_loaded = true;

		return true;

	} // load
	
	//--------------------------------------------------------
		
	void CEntity::unload()
	{
		if(_entityNode)
		{
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}
		if(_entity)
		{
			_scene->getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
		}

	} // load

	//--------------------------------------------------------
		
	void CEntity::tick(float secs)
	{
	} // tick
	
	//--------------------------------------------------------
	
	void CEntity::setTransform(const Matrix4 &transform)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
		{
			Vector3 position, scale;
			Quaternion orientation;
			
			transform.decomposition(position, scale, orientation);

			_entityNode->setPosition(position);
			_entityNode->setOrientation(orientation);
			_entityNode->setScale(scale);
		}

	} // setTransform
	
	//--------------------------------------------------------
		
	void CEntity::setPosition(const Vector3 &position)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entityNode)
			_entityNode->setPosition(position);

	} // setPosition
	
	//--------------------------------------------------------
		
	void CEntity::setRotation(const Vector3 &rotation)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entityNode) {
			Quaternion orientation = Math::getOrientationFromRadians(
				Radian(rotation.x), Radian(rotation.y), Radian(rotation.z));
			_entityNode->setOrientation(orientation);
		}

	} // setRotation
	
	//--------------------------------------------------------
		
	void CEntity::setDimensions(const Vector3 &dimensions)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entityNode)
			setScale(dimensions / _meshDimensions);

	} // setDimensions
	
	//--------------------------------------------------------
		
	void CEntity::setScale(const Vector3 &scale)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entityNode)
			_entityNode->setScale(scale);

	} // setScale

	//--------------------------------------------------------

	void CEntity::setScale(const float scale)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entityNode)
			setScale(Vector3(scale, scale, scale));

	} // setScale
	
	//--------------------------------------------------------
		
	void CEntity::setVisible(bool visible)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if(_entityNode)
			_entityNode->setVisible(visible);

	} // setVisible
	
	//--------------------------------------------------------
	
	const bool CEntity::getVisible()
	{
		if(_entityNode)
			return _entity->isVisible();

		throw new std::runtime_error("La entidad no ha sido cargada");

	} // getVisible
	
	//--------------------------------------------------------
	
	const Vector3& CEntity::getScale()
	{
		if (_entityNode)
			return _entityNode->getScale();

		throw new std::runtime_error("La entidad no ha sido cargada");

	} // getScale

	//--------------------------------------------------------
	
	void CEntity::setColor(const Vector3 &color)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entity) {
			Ogre::MaterialPtr entityMaterial = _entity->getSubEntity(0)->getMaterial();
			entityMaterial->getTechnique(0)->getPass(0)->setAmbient(color.x, color.y, color.z);
		}

	} // setColor
	
	//--------------------------------------------------------
	
	const Vector3& CEntity::getColor()
	{
		if (_entityNode) {
			Ogre::MaterialPtr entityMaterial = _entity->getSubEntity(0)->getMaterial();
			Ogre::ColourValue entityMatColor = entityMaterial->getTechnique(0)->getPass(0)->getAmbient();
			return Vector3(entityMatColor.r, entityMatColor.g, entityMatColor.b);
		}

		throw new std::exception("La entidad no ha sido cargada");

	} // getColor
	
	//--------------------------------------------------------
	
	void CEntity::setMaterialName(const std::string &materialName)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entity)
			_entity->getSubEntity(0)->setMaterialName(materialName);

	} // setMaterial
	
	//--------------------------------------------------------
	
	const std::string& CEntity::getMaterialName()
	{
		if (_entityNode)
			return _entity->getSubEntity(0)->getMaterialName();

		throw new std::exception("La entidad no ha sido cargada");

	} // getMaterialName

} // namespace Graphics
