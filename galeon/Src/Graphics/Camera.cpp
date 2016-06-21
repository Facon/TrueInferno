//---------------------------------------------------------------------------
// Camera.cpp
//---------------------------------------------------------------------------

/**
@file Camera.cpp

Contiene la implementación de la clase que maneja la cámara.

@see Graphics::CCamera

@author David Llansó
@date Julio, 2010
*/

#include "Camera.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CCamera::CCamera(const std::string &name, CScene *scene)
		: _scene(0), _targetNode(nullptr)
	{
		_name = name;
		_scene = scene;

		// Creamos la estructura de nodos de la cámara. Los nodos cuelgan
		// de la raiz, son globales.
		_cameraNode = _scene->getSceneMgr()->getRootSceneNode()->
					createChildSceneNode(name + "_camera_node");
		//_targetNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(name + "_target_node");

		// Hacemos que el nodo de la cámara siempre esté mirando al nodo
		// objetivo.
		//_cameraNode->setAutoTracking(true, _targetNode);
		// Fijamos el viraje de la cámara para se mantenga paralelo al
		// suelo.
		_cameraNode->setFixedYawAxis(true);

		_camera = scene->getSceneMgr()->createCamera(name + "_camera");
		//_camera->setPosition(Vector3(0.0f, -20.0f, 0.0f));
		//HACK: Valores cableados de las distancias para reenderizar. 
		// Deberían poder configurarse.
		_camera->setNearClipDistance(5);
		_camera->setFarClipDistance(500);

		// We need 30 degrees in Field Of View
		_camera->setFOVy(Ogre::Radian(Ogre::Degree(30)));

		_camera->move(Vector3(20.0f, 30.0f, 0.0f));
		_camera->rotate(Vector3(1.0f, 0.0f, 0.0f), Ogre::Degree(-45.0f));
		
		_cameraNode->setPosition(_camera->getRealPosition());

		// Finalmente adjuntamos la cámara a su nodo.
		_cameraNode->attachObject (_camera);

	} // CCamera

	//--------------------------------------------------------

	CCamera::~CCamera() 
	{
		// desacoplamos la cámara de su nodo
		_cameraNode->detachAllObjects();
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_cameraNode);
		if (_targetNode)
			_scene->getSceneMgr()->destroySceneNode(_targetNode);

	} // ~CCamera

	//--------------------------------------------------------
	
	const Vector3 &CCamera::getCameraPosition() 
	{
		return _cameraNode->getPosition();
	}

	//--------------------------------------------------------

	const Vector3 &CCamera::getTargetCameraPosition() 
	{
		return _targetNode->getPosition();
	}

	//--------------------------------------------------------

	const Quaternion &CCamera::getCameraOrientation() 
	{
		return _camera->getOrientation();
	}

	//--------------------------------------------------------

	void CCamera::setCameraPosition(const Vector3 &newPosition)
	{
		_cameraNode->setPosition(newPosition);
	}

	//--------------------------------------------------------

	void CCamera::setTargetCameraPosition(const Vector3 &newPosition)
	{
		_targetNode->setPosition(newPosition);
	}

	Ray CCamera::getCameraToViewportRay(float x, float y){
		return _camera->getCameraToViewportRay(x, y);
	}

	int CCamera::getViewportWidth(){
		return _camera->getViewport()->getActualWidth();
	}

	int CCamera::getViewportHeight(){
		return _camera->getViewport()->getActualHeight();
	}


} // namespace Graphics
