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

#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "OgreParticleSystem.h"

#include "Logic/Entity/ParticleType.h"

namespace Graphics 
{
	CEntity::CEntity(const std::string &name, const std::string &mesh, const Vector3 &meshDimensions)
		: _entity(0), _entityNode(0), _scene(0), _loaded(false)
	{
		_name = name;
		_mesh = mesh;
		_meshDimensions = meshDimensions;

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
		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_node");
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
	
	void CEntity::setDiffuseColor(const Vector3 &color)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entity)
		{
			for (unsigned int i = 0; i < _entity->getNumSubEntities(); ++i)
			{
				Ogre::MaterialPtr entityMaterial = _entity->getSubEntity(i)->getMaterial();
				std::string entityMaterialName = entityMaterial->getName();

				std::size_t found = entityMaterialName.find("_cl_");
				if (found != std::string::npos)
				{
					entityMaterial->getTechnique(0)->getPass(0)->
						setDiffuse(Ogre::ColourValue(color.x, color.y, color.z, 1.0f));
				}
				else
				{
					Ogre::MaterialPtr entityMaterialClone = entityMaterial->
						clone(_entityNode->getName() + "_cl_" + entityMaterial->getName());

					entityMaterialClone->getTechnique(0)->getPass(0)->
						setDiffuse(Ogre::ColourValue(color.x, color.y, color.z, 1.0f));

					_entity->getSubEntity(i)->setMaterial(entityMaterialClone);
				}
			}
		}

	} // setDiffuseColor

	//--------------------------------------------------------
	
	void CEntity::setEmissiveColor(const Vector3 &color)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entity)
		{
			for (unsigned int i = 0; i < _entity->getNumSubEntities(); ++i)
			{
				Ogre::MaterialPtr entityMaterial = _entity->getSubEntity(i)->getMaterial();
				std::string entityMaterialName = entityMaterial->getName();

				std::size_t found = entityMaterialName.find("_cl_");
				if (found != std::string::npos)
				{
					entityMaterial->getTechnique(0)->getPass(0)->
						setEmissive(Ogre::ColourValue(color.x, color.y, color.z, 1.0f));
				}
				else
				{
					Ogre::MaterialPtr entityMaterialClone = entityMaterial->
						clone(_entityNode->getName() + "_cl_" + entityMaterial->getName());

					entityMaterialClone->getTechnique(0)->getPass(0)->
						setEmissive(Ogre::ColourValue(color.x, color.y, color.z, 1.0f));

					_entity->getSubEntity(i)->setMaterial(entityMaterialClone);
				}
			}
		}

	} // setEmissiveColor
	
	//--------------------------------------------------------
	
	Vector3 CEntity::getDiffuseColor()
	{
		if (_entityNode) {
			Ogre::MaterialPtr entityMaterial = _entity->getSubEntity(0)->getMaterial();
			Ogre::ColourValue entityMatColor = entityMaterial->getTechnique(0)->getPass(0)->getDiffuse();
			return Vector3(entityMatColor.r, entityMatColor.g, entityMatColor.b);
		}

		throw new std::exception("La entidad no ha sido cargada");

	} // getDiffuseColor
	
	//--------------------------------------------------------
	
	Vector3 CEntity::getEmissiveColor()
	{
		if (_entityNode) {
			Ogre::MaterialPtr entityMaterial = _entity->getSubEntity(0)->getMaterial();
			Ogre::ColourValue entityMatColor = entityMaterial->getTechnique(0)->getPass(0)->getEmissive();
			return Vector3(entityMatColor.r, entityMatColor.g, entityMatColor.b);
		}

		throw new std::exception("La entidad no ha sido cargada");

	} // getEmissiveColor
	
	//--------------------------------------------------------
	
	void CEntity::makeDarkerColor(float factor)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		assert((factor > 0) && (factor < 1) && "Factor must be in (0,1)");

		if (_entity)
		{
			for (unsigned int i = 0; i < _entity->getNumSubEntities(); ++i)
			{
				Ogre::MaterialPtr entityMaterial = _entity->getSubEntity(i)->getMaterial();
				std::string entityMaterialName = entityMaterial->getName();

				std::size_t found = entityMaterialName.find("_cl_");
				if (found != std::string::npos)
				{
					// Cogemos el color actual
					Ogre::ColourValue color = entityMaterial->getTechnique(0)->getPass(0)->getDiffuse();

					entityMaterial->getTechnique(0)->getPass(0)->
						setDiffuse(color.r * factor, color.g * factor, color.b * factor, color.a);
				}
				else
				{
					Ogre::MaterialPtr entityMaterialClone = entityMaterial->
						clone(_entityNode->getName() + "_cl_" + entityMaterial->getName());

					Ogre::ColourValue color = entityMaterial->getTechnique(0)->getPass(0)->getDiffuse();
					entityMaterialClone->getTechnique(0)->getPass(0)->
						setDiffuse(color.r * factor, color.g * factor, color.b * factor, color.a);

					_entity->getSubEntity(i)->setMaterial(entityMaterialClone);
				}
			}
		}

	} // makeDarkerColor

	//--------------------------------------------------------

	void CEntity::makeClearerColor(float factor)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		assert((factor > 0) && (factor < 1) && "Factor must be in (0,1)");

		if (_entity)
		{
			for (unsigned int i = 0; i < _entity->getNumSubEntities(); ++i)
			{
				Ogre::MaterialPtr entityMaterial = _entity->getSubEntity(i)->getMaterial();
				std::string entityMaterialName = entityMaterial->getName();

				std::size_t found = entityMaterialName.find("_cl_");
				if (found != std::string::npos)
				{
					// Cogemos el color actual
					Ogre::ColourValue color = entityMaterial->getTechnique(0)->getPass(0)->getDiffuse();

					// Y se le aplica la multiplicación inversa que hicimos al desactivar para devolverla al color inicial
					entityMaterial->getTechnique(0)->getPass(0)->
						setDiffuse(color.r / factor, color.g / factor, color.b / factor, color.a);
				}
				else
				{
					Ogre::MaterialPtr entityMaterialClone = entityMaterial->
						clone(_entityNode->getName() + "_cl_" + entityMaterial->getName());

					Ogre::ColourValue color = entityMaterial->getTechnique(0)->getPass(0)->getDiffuse();
					entityMaterialClone->getTechnique(0)->getPass(0)->
						setDiffuse(color.r / factor, color.g / factor, color.b / factor, color.a);

					_entity->getSubEntity(i)->setMaterial(entityMaterialClone);
				}
			}
		}

	} // makeClearerColor

	//--------------------------------------------------------

	void CEntity::setMaterialName(const std::string &materialName)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entity) {
			for (unsigned int i = 0; i < _entity->getNumSubEntities(); ++i)
				_entity->getSubEntity(i)->setMaterialName(materialName);
		}

	} // setMaterialName

	//---------------------------------------------------------

	void CEntity::setCustomParameter(int index, Vector4 param)
	{
		assert(_entityNode && "La entidad no ha sido cargada");
		if (_entity) {
			for (unsigned int i = 0; i < _entity->getNumSubEntities(); ++i)
				_entity->getSubEntity(i)->setCustomParameter(index, param);
		}

	} // setCustomParameter
	
	//--------------------------------------------------------
	
	const std::string& CEntity::getMaterialName()
	{
		if (_entityNode)
			return _entity->getSubEntity(0)->getMaterialName();

		throw new std::exception("La entidad no ha sido cargada");

	} // getMaterialName

	//--------------------------------------------------------

	void CEntity::addParticles(Logic::ParticleType particleType) {
		Graphics::CScene* scene = Graphics::CServer::getSingletonPtr()->getActiveScene();

		// Creamos el sistema de partículas con un nombre único
		std::string templateName = getParticleSystemTemplateName(particleType);
		std::string id = getParticleSystemId(particleType);
		Ogre::ParticleSystem* particleSystem = scene->getSceneMgr()->createParticleSystem(id, templateName);

		//  Lo añadimos a la escena
		if (particleSystem != nullptr)
			this->_entityNode->attachObject(particleSystem);
		else
			assert(false && "Can't add particles");
	}

	//--------------------------------------------------------

	void CEntity::removeParticles(Logic::ParticleType particleType) {
		Graphics::CScene* scene = Graphics::CServer::getSingletonPtr()->getActiveScene();

		// Eliminamos el sistema de partículas
		std::string id = getParticleSystemId(particleType);

		// Lo quitamos de la escena
		Ogre::MovableObject* o = this->_entityNode->getAttachedObject(id);
		if (o != nullptr){
			o->detachFromParent();
			scene->getSceneMgr()->destroyParticleSystem(id);
		}
	}

	//--------------------------------------------------------

	std::string CEntity::getParticleSystemTemplateName(Logic::ParticleType particleType) {
		switch (particleType)
		{
		case Logic::ParticleType::CONSTRUCTION_SMOKE:
			return "ConstructionSmoke";

		case Logic::ParticleType::DESTRUCTION_SMOKE:
			return "DestructionSmoke";

		case Logic::ParticleType::SOUL_MOVEMENT:
			return "SoulMovement";

		case Logic::ParticleType::FIRE_SMOKE:
			return "FireSmoke";

		case Logic::ParticleType::SOUL_DESTRUCTION:
			return "SoulDestruction";

		default:
			assert(false && "No name for this particleType");
			return "";
		}
	}

	//--------------------------------------------------------

	std::string CEntity::getParticleSystemId(Logic::ParticleType particleType) {
		return "Particle_" + getParticleSystemTemplateName(particleType) + "_" + _entity->getName();
	}

} // namespace Graphics
