//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp

Contiene la implementación de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llansó
@date Julio, 2010
*/

#include "Scene.h"
#include "Camera.h"
#include "Server.h"
#include "StaticEntity.h"
#include "GlowMaterialListener.h"

#include "BaseSubsystems/Server.h"

#include "Logic/Entity/Entity.h"

#include "Physics/Server.h"

#include <assert.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreStaticGeometry.h>
#include <OgreColourValue.h>
#include <OgreBillboardSet.h>

namespace Graphics 
{
	CScene::CScene(const std::string& name) : _viewport(0), _staticGeometry(0),
		_directionalLight(0), _spotlightLight(0),
		_spotlightAcumTime(0), _spotlightThresholdTime(0.1f)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, name);

		_camera = new CCamera(name,this);
		_name = name;

	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		delete _camera;

		if (_sceneMgr != nullptr)
			_root->destroySceneManager(_sceneMgr);

	} // ~CScene

	//--------------------------------------------------------

	bool CScene::addEntity(CEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_dynamicEntities.push_back(entity);
		return true;

	} // addEntity

	//--------------------------------------------------------

	bool CScene::addStaticEntity(CStaticEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_staticEntities.push_back(entity);
		return true;

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::removeEntity(CEntity* entity)
	{
		entity->deattachFromScene();
		_dynamicEntities.remove(entity);

	} // addEntity

	//--------------------------------------------------------

	void CScene::removeStaticEntity(CStaticEntity* entity)
	{
		entity->deattachFromScene();
		_staticEntities.remove(entity);

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::activate()
	{
		buildStaticGeometry();

		// Viewport
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
			->addViewport(_camera->getCamera());

		// Glow shader
		Ogre::CompositorManager::getSingleton().addCompositor(_viewport, "Glow");
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(_viewport, "Glow", true);

		GlowMaterialListener *gml = new GlowMaterialListener();
		Ogre::MaterialManager::getSingleton().addListener(gml);

		// Background
		Ogre::ColourValue backgroundColor = Ogre::ColourValue(0.2f, 0.1f, 0.1f, 0.5f);
		_viewport->setBackgroundColour(backgroundColor);

		// Luz ambiente
		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));

		// Sombras
		_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
		_sceneMgr->setShadowColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f, 0.5f));

		// Luz direccional
		_directionalLight = _sceneMgr->createLight("DirectionalLight");
		_directionalLight->setDiffuseColour(0.8f, 0.8f, 0.8f);
		_directionalLight->setSpecularColour(0.8f, 0.8f, 0.8f);
		_directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
		_directionalLight->setPosition(200, 200, 50);
		_directionalLight->setDirection(-200, -200, -50);
		_directionalLight->setCastShadows(true);

		_spotlightLight = _sceneMgr->createLight("SpotlightLight");
		_spotlightLight->setDiffuseColour(Ogre::ColourValue(1.0f, 0.1f, 0.1f, 0.0f));
		_spotlightLight->setSpecularColour(Ogre::ColourValue(1.0f, 0.1f, 0.1f, 0.0f));
		_spotlightLight->setType(Ogre::Light::LT_SPOTLIGHT);
		_spotlightLight->setPosition(0, 0, 0);  // Posición y dirección temporales. Se van actualizando
		_spotlightLight->setDirection(0, 0, 0); // junto con la posición de la cámara
		_spotlightLight->setSpotlightRange(Ogre::Degree(3), Ogre::Degree(5));
		_spotlightLight->setAttenuation(1500.0f, 1.0f, 0.05f, 0.0f);

	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_directionalLight)
		{
			_sceneMgr->destroyLight(_directionalLight);
			_directionalLight = 0;
		}
		if (_spotlightLight)
		{
			_sceneMgr->destroyLight(_spotlightLight);
			_spotlightLight = 0;
		}
		if(_viewport)
		{
			BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->
					removeViewport(_viewport->getZOrder());
			_viewport = 0;
		}

	} // deactivate
	
	//--------------------------------------------------------

	void CScene::tick(float secs)
	{
		TEntityList::const_iterator it = _dynamicEntities.begin();
		TEntityList::const_iterator end = _dynamicEntities.end();
		for(; it != end; it++)
			(*it)->tick(secs);

		// Actualizamos posición y dirección de la luz focal que se mueve
		// junto con el cursor
		_spotlightAcumTime += secs;

		if (_spotlightAcumTime >= _spotlightThresholdTime)
		{
			Vector3 cameraPosition = _camera->getCameraPosition();

			Logic::CEntity *mouseEntity = Physics::CServer::getSingletonPtr()->getEntityFromRaycast();
			Vector3 mouseWorldPosition = (mouseEntity != nullptr) ? mouseEntity->getPosition() : cameraPosition;

			_spotlightLight->setPosition(cameraPosition);
			_spotlightLight->setDirection(mouseWorldPosition - cameraPosition);

			_spotlightAcumTime = 0.0f;
		}

	} // tick

	//--------------------------------------------------------

	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticEntities.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TStaticEntityList::const_iterator it = _staticEntities.begin();
			TStaticEntityList::const_iterator end = _staticEntities.end();
			for(; it != end; it++)
				(*it)->addToStaticGeometry();

			_staticGeometry->build();
		}

	} // buildStaticGeometry

	//--------------------------------------------------------

	Ogre::BillboardSet* CScene::createBillboardSet(CEntity* entity, const std::string& name, const Ogre::Vector3& position) const
	{
		static unsigned int index = 0;

		Ogre::BillboardSet* bbSet = _sceneMgr->createBillboardSet(name + "_" + std::to_string(index));

		//Ogre::SceneNode* childNode = entity->_entityNode->getParentSceneNode()->createChildSceneNode(Vector3(0.0f, 2.0f, 0.0f));
		Ogre::SceneNode* childNode = entity->_entityNode->createChildSceneNode();
		childNode->setInheritScale(false);
		childNode->setPosition(position);
		childNode->attachObject(bbSet);

		++index;

		return bbSet;

	} // createBillboardSet

	//--------------------------------------------------------

	Ogre::BillboardSet* CScene::createBillboardSet(CEntity* entity, const std::string& name) const
	{
		return createBillboardSet(entity, name, Vector3(0.0f, 100.0f, 0.0f));

	} // createBillboardSet

} // namespace Graphics
