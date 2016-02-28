//---------------------------------------------------------------------------
// ScriptManager.h
//---------------------------------------------------------------------------

/**
 * @file ScriptManager.h
 *
 * Contiene la declaraci�n de la clase que oculta la
 * gesti�n de los scripts en Lua.
 *
 * @see Scriptmanager::CScriptManager
 *
 * @author Pedro Pablo y Marco Antonio G�mez Mart�n
 * @date Enero, 2011
 */

#ifndef __ScriptManager_ScriptManager_H
#define __ScriptManager_ScriptManager_H

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
// al ahorrarnos la inclusi�n de ficheros .h en otros .h

// Estructura con el contexto (estado) del int�rprete de Lua.
struct lua_State;

typedef int (*lua_CFunction) (lua_State *L);

#include <string>

namespace ScriptManager {

/**
 * Clase principal del gestor de script (en Lua) de Galeon.
 * Es el responsable de inicializar y cerrar el contexto de
 * Lua, y proporciona funcionalidad para cargar <em>scripts</em>
 * de Lua, ejecutar c�digo, acceder a variables y tablas,
 * etc�tera.
 *
 * Funciona como un <em>singleton</em> de inicializaci�n
 * explicita; es necesario invocar a Init() al inicio (antes
 * de usarlo), y llamar a Release() al terminar.
 */
class CScriptManager {
public:

	/**
	 * M�todo est�tico para inicializar el <em>singleton</em>.
	 * Debe ser invocado antes de hacer uso de la clase
	 * (llamando a GetSingleton() o GetPtrSingleton().
	 *
	 * @return Cierto si todo fue bien, y falso en otro caso.
	 * Si hay alg�n problema, el objeto <em>no podr� usarse</em>.
	 */
	static bool Init();

	/**
	 * Destruye la �nica instancia del objeto. Debe llamarse
	 * al terminar si Init() acab� con �xito. Tras su
	 * invocaci�n, no podr� utilizarse GetSingleton() o
	 * GetPtrSingleton() salvo que se llame de nuevo a Init().
	 */
	static void Release();

	/**
	 * Devuelve la �nica instancia posible de esta clase.
	 * Debe haberse llamado previamente a Init() con �xito
	 * (y no haberse llamado a Release()).
	 *
	 * @return �nica instancia de la clase.
	 */
	static CScriptManager &GetSingleton();

	/**
	 * Devuelve la �nica instancia posible de esta clase.
	 * Debe haberse llamado previamente a Init() con �xito
	 * (y no haberse llamado a Release()).
	 *
	 * @return �nica instancia de la clase.
	 */
	static CScriptManager *GetPtrSingleton();

	/**
	 * Carga un fichero con un <em>script</em> de Lua,
	 * y lo ejecuta.
	 *
	 * @param file Nombre del fichero a cargar (y ejecutar).
	 * @return Cierto si todo fue bien, y falso en otro
	 * caso.
	 *
	 * @note Esta funci�n es bastante mejorable. Si
	 * quisi�ramos hacer un gestor de scripts serio, la
	 * clase deber�a controlar qu� scripts han sido ya
	 * cargados y no volver a cargar por segunda vez
	 * aquellos que se solicitaron previamente.
	 *
	 * Adem�s, para permitir la recarga de ficheros ya
	 * cargados ("actualizaciones en caliente"), algo
	 * �til en depuraci�n en producci�n para hacer
	 * cambios r�pidos sin tener que recompilar o
	 * volver a ejecutar, a�adir�amos un m�todo nuevo
	 * como reloadScript() para recargar uno, o
	 * reloadScripts() para recargarlos todos.
	 *
	 * Con la inclusi�n de executeScript() habr�a que
	 * indicar que los scripts ejecutados en modo
	 * inmediato <em>no</em> se volver�an a ejecutar...
	 *
	 * @note Otro punto de mejora es la gesti�n de
	 * errores. Internamente se utiliza
	 * <tt>lua_call()</tt>, que no permite un
	 * control sencillo de los errores. Si el c�digo
	 * Lua tiene alg�n error, la aplicaci�n en C
	 * <em>acaba inmediatamente</em>. Una mejora clara
	 * ser�a hacer uso de <tt>lua_pcall()</tt> y
	 * una gesti�n m�s fina de los errores. La clase
	 * llevar�a la pista del �ltimo error que se ha
	 * producido, pudiendo devolver una cadena
	 * explicativa por ejemplo. Esto queda fuera
	 * del objetivo de la clase.
	 */
	bool loadScript(const char *script);

	/**
	 * Ejecuta el script inmediato recibido en la cadena
	 * del par�metro.
	 *
	 * @param script C�digo Lua que queremos ejecutar.
	 * @return Cierto si se pudo ejecutar.
	 *
	 * @note Igual que en loadScript() un punto de
	 * mejora es la gesti�n de errores. Ahora mismo se
	 * devuelve falso s�lo si hay un error de sint�xis.
	 * Si hay alg�n error en ejecuci�n... ��se cierra el
	 * programa!!
	 */
	bool executeScript(const char *script);

	/**
	 * Obtiene el valor de una variable global en Lua
	 * de tipo num�rico a partir de su nombre.
	 *
	 * @param name Nombre de la variable global que
	 * se quiere obtener.
	 * @param defaultValue Valor que se devolver� 
	 * si no existe esa variable global en Lua.
	 *
	 * @return Valor de la variable global de Lua, 
	 * o <tt>defaultValue</tt> si no exist�a.
	 *
	 * @note En Lua el tipo num�rico es en principio
	 * <tt>double</tt>. Aqu� sin embargo devolvemos
	 * un entero.
	 */
	int getGlobal(const char *name, int defaultValue);

	/**
	 * Obtiene el valor de una variable global en Lua
	 * de tipo boolean a partir de su nombre.
	 *
	 * @param name Nombre de la variable global que
	 * se quiere obtener.
	 * @param defaultValue Valor que se devolver� 
	 * si no existe esa variable global en Lua.
	 *
	 * @return Valor de la variable global de Lua, 
	 * o <tt>defaultValue</tt> si no exist�a.
	 */
	bool getGlobal(const char *name, bool defaultValue);

	/**
	 * Obtiene el valor de una variable global en Lua
	 * de tipo cadena a partir de su nombre.
	 *
	 * @param name Nombre de la variable global que
	 * se quiere obtener.
	 * @param defaultValue Valor que se devolver� 
	 * si no existe esa variable global en Lua.
	 *
	 * @return Valor de la variable global de Lua, 
	 * o <tt>defaultValue</tt> si no exist�a.
	 *
	 * @note Esta funci�n tiene un peligro potencial;
	 * Lua soporta cadenas con <tt>'\\0'</tt> en su
	 * interior. Aqu� <em>asumiremos que no lo tienen</em>
	 * porque usamos el <tt>'\\0'</tt> como marca de
	 * fin de cadena por simplicidad.
	 * En modo debug se comprueba (con <tt>assert</tt>) que
	 * la cadena devuelta por Lua no tiene <tt>'\\0'</tt>
	 * dentro, pero esta comprobaci�n no es tampoco muy
	 * fiable, porque un script se puede cambiar a
	 * posteriori cuando ya s�lo se usa la versi�n release...
	 */
	std::string getGlobal(const char *name, const char *defaultValue);

	/**
	 * Obtiene el valor de un campo de tipo num�rico de
	 * una tabla global.
	 *
	 * @param table Nombre de la tabla (global) a la que
	 * acceder.
	 * @param field Nombre del campo (�ndice) cuyo valor se
	 * desea obtener.
	 * @param defaultValue Valor que se devolver� 
	 * si no existe la tabla o ese campo (�ndice) no
	 * es de tipo num�rico (o no hay una conversi�n posible).
	 *
	 * @return Valor del campo solicitado, 
	 * o <tt>defaultValue</tt> si no exist�a.
	 *
	 * @note En Lua el tipo num�rico es en principio
	 * <tt>double</tt>. Aqu� sin embargo devolvemos
	 * un entero.
	 */
	int getField(const char *table, const char *field, int defaultValue);

	/**
	 * Obtiene el valor de un campo de tipo booleano de
	 * una tabla global.
	 *
	 * @param table Nombre de la tabla (global) a la que
	 * acceder.
	 * @param field Nombre del campo (�ndice) cuyo valor se
	 * desea obtener.
	 * @param defaultValue Valor que se devolver� 
	 * si no existe la tabla o ese campo (�ndice) no
	 * es de tipo booleano (o no hay una conversi�n posible).
	 *
	 * @return Valor del campo solicitado, 
	 * o <tt>defaultValue</tt> si no exist�a.
	 */
	bool getField(const char *table, const char *field, bool defaultValue);

	/**
	 * Obtiene el valor de un campo de tipo cadena de
	 * una tabla global.
	 *
	 * @param table Nombre de la tabla (global) a la que
	 * acceder.
	 * @param field Nombre del campo (�ndice) cuyo valor se
	 * desea obtener.
	 * @param defaultValue Valor que se devolver� 
	 * si no existe la tabla o ese campo (�ndice) no
	 * es de tipo booleano (o no hay una conversi�n posible).
	 *
	 * @return Valor del campo solicitado, 
	 * o <tt>defaultValue</tt> si no exist�a.
	 *
	 * @note Esta funci�n tiene un peligro potencial;
	 * Lua soporta cadenas con <tt>'\\0'</tt> en su
	 * interior. Aqu� <em>asumiremos que no lo tienen</em>
	 * porque usamos el <tt>'\\0'</tt> como marca de
	 * fin de cadena por simplicidad.
	 * En modo debug se comprueba (con <tt>assert</tt>) que
	 * la cadena devuelta por Lua no tiene <tt>'\\0'</tt>
	 * dentro, pero esta comprobaci�n no es tampoco muy
	 * fiable, porque un script se puede cambiar a
	 * posteriori cuando ya s�lo se usa la versi�n release...
	 */
	std::string getField(const char *table, const char *field, const char *defaultValue);

	/**
	 * Ejecuta una funci�n LUA.
	 */
	bool executeProcedure(const char *subroutineName);

	bool executeProcedure(const char *subroutineName, int param1);

	bool executeFunction(const char *subroutineName, int param1, int &result);

	/**
	 * Registra una funci�n en el contexto de Lua.
	 *
	 * @param f Puntero a la funci�n de C. Deber� recibir como
	 * par�metro un contexto de Lua (para poder acceder a la pila
	 * de Lua), y devolver el n�mero de valores devueltos (en la
	 * pila de Lua)
	 * @param luaName Nombre Lua con el que se conocer� a la
	 * funci�n (que ser� global).
	 */
	void registerFunction(lua_CFunction f, const char *luaName);

	lua_State *getNativeInterpreter() { return _lua; }

protected:

	/**
	 * Constructor. La inicializaci�n se realiza
	 * en dos pasos; es necesario invocar a open() tras
	 * crear el objeto. En open() se realizan las acciones
	 * que pueden fallar, de manera que devolver� falso
	 * si hay problemas (algo que el constructor no
	 * podr�a hacer).
	 *
	 * No obstante, ni el constructor ni open() ser�n
	 * usados por los clientes de la clase. Ambos
	 * son protegidos dado que la clase funciona como
	 * un <em>singleton</em>. Deber� usarse Init()
	 * en su lugar.
	 */
	CScriptManager();

	/**
	 * Destructor. Libera los recursos.
	 *
	 * Es protegido porque los clientes no podr�n destruir
	 * el objeto. En su lugar deben usar Release().
	 */
	~CScriptManager();

	/**
	 * Segunda fase de la inicializaci�n del objeto.
	 * Es llamado autom�ticamente desde el m�todo
	 * est�tico Init() para que el usuario no tenga
	 * que preocuparse.
	 *
	 * @return Cierto si todo fue bien, y falso si hubo
	 * alg�n problema. El objeto <em>no</em> deber�a usarse
	 * en ese caso, y deber�a ser destru�do.
	 */
	bool open();

	/**
	 * M�todo inverso a open(). Es llamado autom�ticamente
	 * desde el destructor.
	 */
	void close();

	/**
	 * �nica instancia (singleton) de la clase.
	 * Se inicializa en Init() y se destruye en Release().
	 */
	static CScriptManager *_instance;

	/**
	 * Contexto (int�rprete) de Lua usado.
	 */
	lua_State *_lua;

}; // class CScriptManager

} // namespace ScriptManager

#endif // __ScriptManager_ScriptManager_H