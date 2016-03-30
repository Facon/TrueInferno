//---------------------------------------------------------------------------
// ScriptManager.cpp
//---------------------------------------------------------------------------

/**
 * @file ScriptManager.cpp
 *
 * Contiene la definici�n de la clase que oculta la
 * gesti�n de los scripts en Lua.
 *
 * @see Scriptmanager::CScriptManager
 *
 * @author Pedro Pablo y Marco Antonio G�mez Mart�n
 * @date Enero, 2011
 */

#include "ScriptManager.h"

// Inclu�mos las cabeceras de Lua.
// Como es c�digo C (no C++), hay que indicarselo al
// compilador para que asuma el convenio de nombres
// de C en el c�digo objeto.
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "luabind.hpp"

#include <cassert>
#include <iostream>

namespace ScriptManager {

// �nica instancia de la clase.
CScriptManager *CScriptManager::_instance = 0;

//---------------------------------------------------------

bool CScriptManager::Init() {

	assert(_instance == NULL);
	_instance = new CScriptManager();
	if (!_instance->open()) {
		// ��Vaya!!
		Release();
		return false;
	}

	return true;

} // Init

//---------------------------------------------------------

void CScriptManager::Release() {

	assert(_instance);

	delete _instance;
	_instance = NULL;

} // Release

//---------------------------------------------------------

CScriptManager &CScriptManager::GetSingleton() {

	assert(_instance);
	return *_instance;

} // GetSingleton

//---------------------------------------------------------

CScriptManager *CScriptManager::GetPtrSingleton() {

	assert(_instance);
	return _instance;

} // GetPtrSingleton

//---------------------------------------------------------

bool CScriptManager::loadScript(const char *script) {

	assert(_lua);

	if (luaL_loadfile(_lua, script) != 0) {
		std::cout << "ScriptManager WARNING: error de sintaxis en el fichero " << script << std::endl;
		return false;
	}

	if (lua_pcall(_lua, 0, 0, 0) != 0) {
		std::cout << "Error en la ejecuci�n de " << script << ": " <<
			lua_tostring(_lua, -1) << std::endl;
		lua_pop(_lua, 1);
		return false;
	}

	return true;

} // loadScript

//---------------------------------------------------------

bool CScriptManager::executeScript(const char *script) {

	assert(_lua);

	if (luaL_loadstring(_lua, script) != 0)
		// Devuelve 0 si todo va bien, y 0 en otro caso
		// con un c�digo de error, que ignoramos.
		return false;

	lua_call(_lua, 0, 0);

	return true;

} // executeScript

//---------------------------------------------------------

int CScriptManager::getGlobal(const char *name, int defaultValue) {

	assert(_lua);

	// LUABIND
	luabind::object obj = luabind::globals(_lua)[name];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TNUMBER))
		return defaultValue;
	else
		return luabind::object_cast<int>(obj);

	// LUA
	/*
	#ifdef _DEBUG
		int topLua = lua_gettop(_lua);
	#endif

	int result;

	lua_getglobal(_lua, name);

	if (!lua_isnumber(_lua, -1))
		result = defaultValue;
	else
		result = (int) lua_tonumber(_lua, -1);

	// Quitamos de la pila de Lua el elemento apilado por
	// lua_getglobal.
	// Si la variable global no exist�a, nos habr�
	// apilado nil, de modo que siempre hay algo :-)
	lua_pop(_lua, 1);

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert s�lo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return result;
	*/

} // getGlobal(int)

//---------------------------------------------------------

bool CScriptManager::getGlobal(const char *name, bool defaultValue) {

	assert(_lua);

	// LUABIND
	luabind::object obj = luabind::globals(_lua)[name];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TBOOLEAN))
		return defaultValue;
	else
		return luabind::object_cast<bool>(obj);

	// LUA
	/*
	#ifdef _DEBUG
		int topLua = lua_gettop(_lua);
	#endif

	bool result;

	lua_getglobal(_lua, name);

	if (!lua_isboolean(_lua, -1))
		result = defaultValue;
	else
		result = lua_toboolean(_lua, -1)==0?false:true;

	// Quitamos de la pila de Lua el elemento apilado por
	// lua_getglobal.
	// Si la variable global no exist�a, nos habr�
	// apilado nil, de modo que siempre hay algo :-)
	lua_pop(_lua, 1);

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert s�lo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return result;
	*/

} // getGlobal(bool)

//---------------------------------------------------------

std::string CScriptManager::getGlobal(const char *name, const char *defaultValue) {

	assert(_lua);

	// LUABIND
	luabind::object obj = luabind::globals(_lua)[name];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TSTRING))
		return defaultValue;
	else
		return luabind::object_cast<std::string>(obj);

	// LUA
	/*
	#ifdef _DEBUG
		int topLua = lua_gettop(_lua);
	#endif

	size_t len;
	const char *result; // Antes de hacer la copia.

	lua_getglobal(_lua, name);

	if (!lua_isstring(_lua, -1))
		result = defaultValue;
	else {
		result = lua_tolstring(_lua, -1, &len);

		// lua_tolstring siempre nos pone el \0
		// al final, pero podr�a haber alguno m�s
		// por el medio.
		// Si es as�, entonces strlen(result)
		// (que busca el \0) parar� "antes de
		// tiempo" y habr� diferencias en las
		// longitudes apreciables.
		assert(len == strlen(result));
	}

	// Hacemos la copia. La hacemos antes de quitar
	// el elemento de la pila, porque Lua podr�a decidir
	// recoger la basura y liberar la cadena result.
	std::string resultCopy(result);

	// Quitamos de la pila de Lua el elemento apilado por
	// lua_getglobal.
	// Si la variable global no exist�a, nos habr�
	// apilado nil, de modo que siempre hay algo :-)
	lua_pop(_lua, 1);

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert s�lo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return resultCopy;
	*/

} // getGlobal(char*)

//---------------------------------------------------------

int CScriptManager::getField(const char *table,
                             const char *field,
                             int defaultValue) {

	assert(_lua);

#ifdef _DEBUG
	int topLua = lua_gettop(_lua);
#endif

	int result;

	// Tenemos que conseguir una referencia a la
	// tabla en la cima de la pila de Lua. La tabla
	// es una variable global...
	lua_getglobal(_lua, table);

	if (!lua_istable(_lua, -1)) {
		// Vaya, pues la tabla no existe (o no es
		// una tabla).
		result = defaultValue;
		// Limpiamos la pila
		lua_pop(_lua, 1);
	}
	else {
		// Tenemos la tabla. Ahora apilamos el
		// nombre del campo (�ndice).
		lua_pushstring(_lua, field);

		// Para acceder a un campo se usa lua_gettable(lua_state, index).
		// Mira en la posici�n index de la tabla Lua, y ah� espera
		// encontrar la tabla de donde leer. El nombre del campo
		// al que acceder lo saca de la cima, lo desapila, y
		// apila el valor.
		// Ahora en la pila tenemos en la cima (�ndice -1)
		// el campo (acabamos de apilarlo).
		// Debajo (�ndice -2) tenemos la referencia a la tabla
		// a la que queremos acceder.
		lua_gettable(_lua, -2);
		// Fijate que en la pila el �nico cambio est� en la cima:
		// el �ndice se sustituye por el valor. Pero la referencia
		// a la tabla NO SE TOCA (podr�a de hecho estar muy
		// profunda en la pila de Lua... pues estamos dando
		// directamente el �ndice en la pila a su posici�n).

		// En la cima tendremos el valor.
		if (!lua_isnumber(_lua, -1))
			result = defaultValue;
		else
			result = (int) lua_tonumber(_lua, -1);

		// Limpiamos la pila. Hay que quitar el valor conseguido,
		// y la referencia a la tabla.
		lua_pop(_lua, 2);
	} // if-else la tabla exist�a

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert s�lo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return result;

} // getField(int)

//---------------------------------------------------------

bool CScriptManager::getField(const char *table,
                             const char *field,
                             bool defaultValue) {

	assert(_lua);

#ifdef _DEBUG
	int topLua = lua_gettop(_lua);
#endif

	bool result;

	// Tenemos que conseguir una referencia a la
	// tabla en la cima de la pila de Lua. La tabla
	// es una variable global...
	lua_getglobal(_lua, table);

	if (!lua_istable(_lua, -1)) {
		// Vaya, pues la tabla no existe (o no es
		// una tabla).
		result = defaultValue;
		// Limpiamos la pila
		lua_pop(_lua, 1);
	}
	else {
		// Tenemos la tabla. Ahora apilamos el
		// nombre del campo (�ndice).
		lua_pushstring(_lua, field);

		// Para acceder a un campo se usa lua_gettable(lua_state, index).
		// Mira en la posici�n index de la tabla Lua, y ah� espera
		// encontrar la tabla de donde leer. El nombre del campo
		// al que acceder lo saca de la cima, lo desapila, y
		// apila el valor.
		// Ahora en la pila tenemos en la cima (�ndice -1)
		// el campo (acabamos de apilarlo).
		// Debajo (�ndice -2) tenemos la referencia a la tabla
		// a la que queremos acceder.
		lua_gettable(_lua, -2);
		// Fijate que en la pila el �nico cambio est� en la cima:
		// el �ndice se sustituye por el valor. Pero la referencia
		// a la tabla NO SE TOCA (podr�a de hecho estar muy
		// profunda en la pila de Lua... pues estamos dando
		// directamente el �ndice en la pila a su posici�n).

		// En la cima tendremos el valor.
		if (!lua_isboolean(_lua, -1))
			result = defaultValue;
		else
			result = lua_toboolean(_lua, -1)==0?false:true;

		// Limpiamos la pila. Hay que quitar el valor conseguido,
		// y la referencia a la tabla.
		lua_pop(_lua, 2);
	} // if-else la tabla exist�a

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert s�lo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return result;

} // getField(bool)

//---------------------------------------------------------


std::string CScriptManager::getField(const char *table,
                               const char *field,
                               const char *defaultValue) {

	size_t len;
	assert(_lua);

#ifdef _DEBUG
	int topLua = lua_gettop(_lua);
#endif

	const char *result; // Antes de hacer la copia.
	std::string ret;

	// Tenemos que conseguir una referencia a la
	// tabla en la cima de la pila de Lua. La tabla
	// es una variable global...
	lua_getglobal(_lua, table);

	if (!lua_istable(_lua, -1)) {
		// Vaya, pues la tabla no existe (o no es
		// una tabla).
		result = defaultValue;
		// Hacemos la copia. La hacemos antes de quitar
		// el elemento de la pila, porque Lua podr�a decidir
		// recoger la basura y liberar la cadena result.
		ret = std::string(result);

		// Limpiamos la pila
		lua_pop(_lua, 1);
	}
	else {
		// Tenemos la tabla. Ahora apilamos el
		// nombre del campo (�ndice).
		lua_pushstring(_lua, field);

		// Para acceder a un campo se usa lua_gettable(lua_state, index).
		// Mira en la posici�n index de la tabla Lua, y ah� espera
		// encontrar la tabla de donde leer. El nombre del campo
		// al que acceder lo saca de la cima, lo desapila, y
		// apila el valor.
		// Ahora en la pila tenemos en la cima (�ndice -1)
		// el campo (acabamos de apilarlo).
		// Debajo (�ndice -2) tenemos la referencia a la tabla
		// a la que queremos acceder.
		lua_gettable(_lua, -2);
		// Fijate que en la pila el �nico cambio est� en la cima:
		// el �ndice se sustituye por el valor. Pero la referencia
		// a la tabla NO SE TOCA (podr�a de hecho estar muy
		// profunda en la pila de Lua... pues estamos dando
		// directamente el �ndice en la pila a su posici�n).

		// En la cima tendremos el valor.
		if (!lua_isstring(_lua, -1))
			result = defaultValue;
		else {
			result = lua_tolstring(_lua, -1, &len);

			// lua_tolstring siempre nos pone el \0
			// al final, pero podr�a haber alguno m�s
			// por el medio.
			// Si es as�, entonces strlen(result)
			// (que busca el \0) parar� "antes de
			// tiempo" y habr� diferencias en las
			// longitudes apreciables.
			assert(len == strlen(result));
		}

		// Hacemos la copia. La hacemos antes de quitar
		// el elemento de la pila, porque Lua podr�a decidir
		// recoger la basura y liberar la cadena result.
		ret = std::string(result);

		// Limpiamos la pila. Hay que quitar el valor conseguido,
		// y la referencia a la tabla.
		lua_pop(_lua, 2);
	} // if-else la tabla exist�a

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert s�lo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return ret;

} // getField(char*)

//---------------------------------------------------------

bool CScriptManager::executeProcedure(const char *subroutineName) {

	assert(_lua);
	
	// LUABIND
	luabind::object obj = luabind::globals(_lua)[subroutineName];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TFUNCTION))
		return false;
	else {
		obj();
		return true;
	}
	
	// LUA
	/*
	#ifdef _DEBUG
		int topLua = lua_gettop(_lua);
	#endif

	// Lo primero es buscar la funci�n (global) con ese
	// nombre.
	lua_getglobal(_lua, subroutineName);

	if (!lua_isfunction(_lua, -1)) {
		// ��Vaya!!
		lua_pop(_lua, 1);
		return false;
	}

	// Llamamos a la funci�n, sin argumentos y sin
	// resultado.
	lua_call(_lua, 0, 0);

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert s�lo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return true;
	*/

} // executeProcedure

//---------------------------------------------------------

bool CScriptManager::executeProcedure(const char *subroutineName, int param1) {

	assert(_lua);

	// LUABIND
	try {
		luabind::globals(_lua)[subroutineName](param1);
	}
	catch (luabind::error &ex) {
		std::cout << ex.what() << std::endl;
		return false;
	}

	return true;

	// LUA
	/*
	#ifdef _DEBUG
		int topLua = lua_gettop(_lua);
	#endif

	// Lo primero es buscar la funci�n (global) con ese
	// nombre.
	lua_getglobal(_lua, subroutineName);

	if (!lua_isfunction(_lua, -1)) {
		// ��Vaya!!
		lua_pop(_lua, 1);
		return false;
	}

	// Apilamos el par�metro.
	lua_pushnumber(_lua, param1);

	// Llamamos a la funci�n, con un argumento y sin
	// resultado.
	lua_call(_lua, 1, 0);

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert s�lo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return true;
	*/

} // executeProcedure(int)

//---------------------------------------------------------

bool CScriptManager::executeFunction(const char *subroutineName,
                                     int param1, int &result) {

	assert(_lua);

	// LUABIND
	try {
		luabind::object res;
		res = luabind::globals(_lua)[subroutineName](param1);

		if (!res.is_valid() || (luabind::type(res) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<int>(res);
	}
	catch (luabind::error &ex) {
		std::cout << ex.what() << std::endl;
		return false;
	}

	return true;

} // executeFunction

//---------------------------------------------------------

void CScriptManager::registerFunction(lua_CFunction f, const char *luaName) {

	assert(_lua);

	lua_register(_lua, luaName, f);

} // registerFunction

//---------------------------------------------------------
//                   M�todos protegidos
//---------------------------------------------------------

CScriptManager::CScriptManager() : _lua(NULL) {
} // Constructor

//---------------------------------------------------------

CScriptManager::~CScriptManager() {
	close();
} // Destructor

//---------------------------------------------------------

bool CScriptManager::open() {

	_lua = lua_open();

	if (!_lua)
		return false;

	// Abrimos la librer�a base para hacer disponible
	// print() en Lua.
	luaopen_base(_lua);

	// A�adimos el namespace luabind (que es C++!)
	luabind::open(_lua);

	return true;

} // open

//---------------------------------------------------------

void CScriptManager::close() {

	if (_lua)
		lua_close(_lua);

} // close

} // namespace ScriptManager