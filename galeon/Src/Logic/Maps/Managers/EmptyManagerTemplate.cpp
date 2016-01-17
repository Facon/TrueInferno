//---------------------------------------------------------------------------
// EmptyManagerTemplate.cpp
//---------------------------------------------------------------------------

/**
THIS IS A TEMPLATE OF AN EMPTY SINGLETON MANAGER, CONTAINING ONLY THE
MANAGER INSTANCE AND ALL REQUIRED FUNCTIONS FOR INITIALIZATION AND
DESTRUCTION OF STATIC RESOURCES.

TO CREATE A NEW MANAGER FROM THIS, SIMPLY:
1) COPY-PASTE THIS CODE.
2) CLOSE COMMENTS.
3) CHANGE CLASS NAME AND EVERY OTHER THING YOU NEED.
4) REMEMBER ALSO TO REVIEW AND CHANGE COMMENTS IF NEEDED.
*/

/**
@file TileManager.cpp

Contiene la implementación del gestor de la matriz de tiles.

@see Logic::CTileManager

@author Raúl Segura
@date Enero, 2016
/

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
		assert(!_instance && "Segunda inicialización de Logic::CTileManager no permitida!");

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
		assert(_instance && "Logic::CTileManager no está inicializado!");

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

*/
