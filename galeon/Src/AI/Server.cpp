/**
@file Server.cpp

Servidor de IA.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/
#include "Server.h"

#include <assert.h>


namespace AI {
	/////////////////////////////////////////
	/**
	Instancia �nica de la clase.
	*/
	CServer* CServer::_instance = 0;
	/////////////////////////////////////////
	/**
	Constructor de la clase.
	*/
	CServer::CServer(void)
	{
		assert(!_instance && "Segunda inicializaci�n de AI::CServer no permitida!");
		_f = new CAStarFunctionsGaleon();
		_aStar = new micropather::MicroPather(_f);
	}
	/////////////////////////////////////////
	/**
	Destructor de la clase.
	*/
	CServer::~CServer(void)
	{
		assert(_instance);
		delete _aStar;
		delete _f;
	}
	/////////////////////////////////////////
	/**
	Inicializa el servidor de IA. Esta operaci�n s�lo es necesario realizarla
	una vez durante la inicializaci�n de la aplicaci�n. 

	@return Devuelve false si no se ha podido inicializar.
	*/
	bool CServer::Init() 
	{
		_instance = new CServer();
		return true;
	}
	/////////////////////////////////////////
	/**
	Libera el servidor de IA. Debe llamarse al finalizar la aplicaci�n.
	*/
	void CServer::Release()
	{
		if (_instance)
			delete _instance;
		_instance = 0;
	}
	/////////////////////////////////////////
	/**
	A�ade un nuevo nodo al grafo de navegaci�n.
	*/
	void CServer::addWaypoint(Vector3 waypoint)
	{
		_wpg.addWaypoint(waypoint);
	}

	/////////////////////////////////////////
	/**
	Calcula una ruta usando A*.
	*/
	std::vector<Vector3> *CServer::getAStarRoute(Vector3 from, Vector3 to)
	{
		// Dadas dos posiciones devuelve una ruta para llegar de una a otra.
		// En primer lugar utilizamos los m�todos del grafo de waypoints para obtener los nodos
		// m�s cercanos al origen y al destino (_wpg.getClosestWaypoint). 
		// A continuaci�n usamos A* para calcular la ruta (_aStar->Solve).
		// Por �ltimo tenemos que devolver la ruta en un vector de posiciones. Para 
		// ello tendremos que convertir los ids de los nodos en sus posiciones (_wpg.getNode) 
		// y a�adir las posiciones de origen y destino.
		int idFrom = _wpg.getClosestWaypoint(from);
		int idTo = _wpg.getClosestWaypoint(to);
		vector<void*>* path = new vector<void*>();
		float cost = 0.0f;
		int solved = _aStar->Solve((void*) idFrom, (void*) idTo, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return NULL;
		}

		vector<Vector3>* out = new vector<Vector3>();
		// A�adimos el punto inicial si no coincide con el primer nodo
		if (!from.positionEquals(_wpg.getWaypoint(
				(int) path->at(0)), 5.0f))
			out->push_back(from);
		for (vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			out->push_back(_wpg.getWaypoint((int) (*it)));
		}
		// A�adimos el punto final si no coincide con el �ltimo nodo
		if (!to.positionEquals(_wpg.getWaypoint(
				(int) path->at(path->size() - 1)), 5.0f))
			out->push_back(to);
		delete path;
		return out;
	}
	/////////////////////////////////////////
	/**
	Dado un �ngulo en radianes lo lleva al intervalo [-PI, PI]
	*/
	double CServer::correctAngle(double angle)
	{
		while (angle > Math::PI)
			angle -= 2 * Math::PI;
		while (angle < -Math::PI)
			angle += 2 * Math::PI;
		return angle;
	}

} // namespace AI