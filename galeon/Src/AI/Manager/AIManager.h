//---------------------------------------------------------------------------
// AIManager.h
//---------------------------------------------------------------------------

/**
@file AIManager.h

Contiene la declaraci�n del gestor de la IA de los enemigos en el juego. 

@see Logic::CScriptManager

@author �lvaro Valera
@date Abril, 2016
*/

#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include <string>
#include <set>
#include <map>

#include "AI/God.h"
#include "Logic/Events/Event.h"
#include "Logic/Entity/MessageHandler.h"

using namespace Logic;

namespace AI
{
	/**
	Manager singleton que gestiona la IA de los enemigos en el juego.

	@author �lvaro Valera
	@date Abril, 2016
	*/
	class CAIManager : public MessageHandler
	{
	public:

		/**
		Devuelve la �nica instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static CAIManager *getSingletonPtr() { return _instance; }

		/**
		Inicializa la instancia y los recursos est�ticos.
		@return true si la inicializaci�n se hizo correctamente.
		*/
		static bool Init();

		/**
		Libera la propia instancia y los recursos est�ticos.
		Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Si ha pasado determinado tiempo desde el �ltimo evento, lanzar� (desde Lua) un evento aleatorio nuevo.

		@param msecs milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		Carga un script de LUA encargado de crear y lanzar eventos.

		@param filename nombre del script.
		@return true si la carga se hizo correctamente.
		*/
		bool loadAIScript(const std::string& filename);

		/**
		Devuelve el tiempo transcurrido (ms) de la aplicaci�n.
		Es una funci�n-puente para que Lua no tenga que acceder a otras partes del motor.

		@return Tiempo (ms) que la aplicaci�n lleva activa.
		*/
		long getElapsedTime() const;

		/** 
		A�ade un dios al juego.
		T�picamente querremos invocar este m�todo desde Lua que es donde se configuran los dioses.
		*/
		void addGod(const std::string& name, bool isBoss);

		/** 
		Devuelve el ranking de dioses ordenado por puntuaci�n (de mayor a menor)
		*/
		const std::map<std::string, CGod*> CAIManager::getGodRanking();

		// Manejo de mensajes, tiene que manejar todos los tipos de mensajes sin excepci�n.
		bool HandleMessage(const Message& msg);
		bool HandleMessage(const TransformMessage& msg);
		bool HandleMessage(const PositionMessage& msg);
		bool HandleMessage(const RotationMessage& msg);
		bool HandleMessage(const DimensionsMessage& msg);
		bool HandleMessage(const ColorMessage& msg);
		bool HandleMessage(const MaterialMessage& msg);
		bool HandleMessage(const AnimationMessage& msg);
		bool HandleMessage(const ControlMessage& msg);
		bool HandleMessage(const PhysicMessage& msg);
		bool HandleMessage(const TouchMessage& msg);
		bool HandleMessage(const DamageMessage& msg);
		bool HandleMessage(const WorkerMessage& msg);
		bool HandleMessage(const WalkSoulPathMessage& msg);
		bool HandleMessage(const HellQuartersMessage& msg);
		bool HandleMessage(const MovePlaceableMessage& msg);
		bool HandleMessage(const SoulSenderMessage& msg);
		bool HandleMessage(const SoulMessage& msg);
		bool HandleMessage(const CheckValidPositionPlaceableMessage& msg);
		bool HandleMessage(const NumberMessage& msg);
		bool HandleMessage(const ResourceMessage& msg);
		bool HandleMessage(const GetCostPlaceableMessage& msg);
		bool HandleMessage(const LogisticsMessage& msg);
		bool HandleMessage(const ToggleMessage& msg);

	protected:

		/**
		Constructor.
		*/
		CAIManager();

		/**
		Destructor.
		*/
		virtual ~CAIManager();

		/**
		M�todo encargado de registrar en el contexto de Lua todas aquellas
		clases y funciones necesarias para el completo manejo de los eventos
		de juego desde Lua.
		*/
		void luaRegister();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones
		est�ticas.

		@return true si la inicializaci�n se hizo correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar
		los recursos de la propia instancia, la liberaci�n de los recursos
		est�ticos se hace en Release().
		*/
		void close();

	private:
		/**
		�nica instancia de la clase.
		*/
		static CAIManager *_instance;

		/** Dioses de la partida indexados por nombre */
		std::map<std::string, CGod*> _gods;

		/** Funci�n para ordenar dioses por score en el ranking */
		//bool godScoreCompare(CGod* lhs, CGod* rhs) { return lhs->getScore() < rhs->getScore(); }
		struct godScoreCompare {
			bool operator() (CGod* lhs, CGod* rhs) const
			{
				return lhs->getScore() < rhs->getScore();
			}
		};

		/** Ranking de dioses ordenados por score creciente */
		std::multiset<CGod*, godScoreCompare> _ranking;

		/** El jefe */
		CGod* _theBoss;

	}; // class AIManager

} // namespace Logic

#endif // AI_MANAGER_H_
