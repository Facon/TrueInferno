//---------------------------------------------------------------------------
// TileManager.cpp
//---------------------------------------------------------------------------

/**
@file TileManager.cpp

Contiene la implementaci�n del gestor de la matriz de tiles.

@see Logic::CTileManager

@author Ra�l Segura
@date Enero, 2016
*/

#include "TileManager.h"

#include <cassert>

namespace Logic {

	CTileManager* CTileManager::_instance = 0;

	//--------------------------------------------------------

	CTileManager::CTileManager()
	{
		_instance = this;

	} // CTileManager

	//--------------------------------------------------------

	CTileManager::~CTileManager()
	{
		assert(_instance);
		_instance = 0;

	} // ~CTileManager

	//--------------------------------------------------------

	bool CTileManager::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CTileManager no permitida!");

		new CTileManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CTileManager::Release()
	{
		assert(_instance && "Logic::CTileManager no est� inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CTileManager::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CTileManager::close()
	{
	} // close

} // namespace Logic
