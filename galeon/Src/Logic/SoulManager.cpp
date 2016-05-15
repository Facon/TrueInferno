//---------------------------------------------------------------------------
// SoulManager.cpp
//---------------------------------------------------------------------------

/**
@file SoulManager.cpp

Contiene la implementación del gestor de almas.

@see Logic::CSoulManager

@author Raúl Segura
@date Abril, 2016
*/

#include "SoulManager.h"
#include <cassert>

namespace Logic {

	CSoulManager* CSoulManager::_instance = 0;

	//--------------------------------------------------------

	CSoulManager::CSoulManager()
	{
		_instance = this;

	} // CSoulManager

	//--------------------------------------------------------

	CSoulManager::~CSoulManager()
	{
		assert(_instance);
		_instance = nullptr;

	} // ~CSoulManager

	//--------------------------------------------------------

	bool CSoulManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CSoulManager no permitida!");

		new CSoulManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CSoulManager::Release()
	{
		assert(_instance && "Logic::CSoulManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CSoulManager::modifyMovementSpeed(float factor)
	{
		assert(0 < factor && "La velocidad de movimiento debe multiplicarse por un factor positivo");

		_realMovementSpeed = _movementSpeed;
		_movementSpeed *= factor;

	} // modifyMovementSpeed

	//--------------------------------------------------------

	void CSoulManager::restoreMovementSpeed()
	{
		_movementSpeed = _realMovementSpeed;

	} // restoreMovementSpeed

	//--------------------------------------------------------

	bool CSoulManager::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CSoulManager::close()
	{
	} // close

} // namespace Logic
