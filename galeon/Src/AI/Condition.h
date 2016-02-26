/**
@file CCondition.h

En este fichero se define la interfaz para las condiciones
de las aristas de las m�quinas de estado, as� como cuatro 
condiciones de ejemplo: CConditionTrue (una condici�n que 
siempre se eval�a como verdadera), CConditionFinished (es 
verdadera si la acci�n asociada al nodo ha terminado), 
CConditionSuccess (es  verdadera si la acci�n asociada al 
nodo ha terminado con �xito) y CConditionFail (es verdadera 
si la acci�n asociada al nodo ha terminado con fallo).

@author Gonzalo Fl�rez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_Condition_H
#define __AI_Condition_H

#include "Logic/Entity/Entity.h"
#include "LatentAction.h"

using namespace Logic;

namespace AI 
{
	/**
	Interfaz que deber�n implementar las condiciones de 
	las m�quinas de estado.
	*/
	template <class TNode>
	class ICondition : public MessageHandler
	{
	public:
		/**
		En este m�todo se comprobar� la condici�n. Recibe el 
		nodo al que pertenece la condici�n (de esta manera 
		podemos comprobar si el nodo ha terminado y en qu� 
		condiciones) y la entidad que ejecuta el comportamiento 
		(que puede ser NULL).

		@param currentNode Nodo al que pertenece la condici�n.
		@param entity Entidad que ejecuta el comportamiento
		@return true o false seg�n si se cumple la condici�n o no.
		*/
		virtual bool check(TNode* currentNode, CEntity* entity) = 0;
	}; // class CCondition

	/**
	Esta clase define una condici�n que siempre se eval�a a true.
	*/
	template <class TNode>
	class CConditionTrue : public ICondition<TNode>
	{
	public:
		bool check(TNode* currentNode, CEntity* entity) { return true; }
	};
	
	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado.
	*/
	class CConditionFinished : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			// TODO PR�CTICA IA
			// Comprobar si el estado del nodo que se est� ejecutando es terminado
			return (currentNode->getStatus() == CLatentAction::SUCCESS) || 
				(currentNode->getStatus() == CLatentAction::FAIL);
		}
	};

	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado con �xito.
	*/
	class CConditionSuccess : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			// TODO PR�CTICA IA
			// Comprobar si el estado del nodo que se est� ejecutando es �xito
			return currentNode->getStatus() == CLatentAction::SUCCESS;
		}
	};

	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado con fallo.
	*/
	class CConditionFail : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			
			// TODO PR�CTICA IA
			// Comprobar si el estado del nodo que se est� ejecutando es fallo
			return currentNode->getStatus() == CLatentAction::FAIL;			
		}
	};

	/**
	Esta clase define una condici�n que devuelve true 
	si se recibi� un mensaje de un tipo concreto en el �ltimo tick
	*/
	template <class TNode>
	class CConditionMessage : public ICondition<TNode>
	{
	public:
		/**
		En el constructor recibimos el tipo de mensaje que 
		har� saltar la condici�n
		@param messageType Tipo de mensaje que estamos escuchando
		*/
		CConditionMessage(MessageType messageType) : _received(false), _messageType(messageType) {};

		/**
		En el check s�lo tenemos que comprobar el flag _received. Este flag
		se habr� activado en process si durante este tick hemos recibido
		un mensaje del tipo adecuado.
		Una vez hecha la comprobaci�n, reseteamos el flag a false.
		@param currentNode Nodo al que pertenece la condici�n (no se usa)
		@param entity Entidad que ejecuta el comportamiento (no se usa)
		@return true o false seg�n si se cumple la condici�n o no.

		*/
		bool check(TNode* currentNode, CEntity* entity) { 
			// TODO PR�CTICA IA
			// Tenemos que comprobar el flag _received y luego actualizarlo
			// de nuevo a false para el siguiente tick
			bool receivedThisTick = _received;
			_received = false;
			return receivedThisTick;
		}

		/**
		Devuelve true si el tipo del mensaje recibido coincide con el 
		que estamos esperando
		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		/*virtual bool accept(const MessageType &message) {
			// TODO PR�CTICA IA
			// La condici�n debe aceptar mensajes del tipo indicado en _messageType
			return message._type == _messageType;
		};*/

		/**
		Procesa el mensaje recibido. Como s�lo aceptamos mensajes del
		tipo en el que estamos interesados aqu� directamente ponemos 
		el flag a true.

		@param msg Mensaje recibido.
		*/
		/*virtual void process(const MessageType &message) {
			// TODO PR�CTICA IA
			// Actualizamos el flag _received si el mensaje es del tipo _messageType
			_received = _received || (message._type == _messageType); 
		};*/

		bool HandleMessage(const WalkSoulPathMessage& msg){
			if (msg._type == _messageType){
				// Actualizamos el flag _received si el mensaje es del tipo _messageType
				_received = true;
				return true;
			}
			else
				return false;
		}

	private:
		/** Flag que se activa cuando recibimos un mensaje del tipo en el que estamos interesados */
		bool _received;

		/** Tipo del mensaje que esperamos */
		MessageType _messageType;
	};

	/**
	Esta clase define una condici�n que se eval�a a true si la 
	entidad se encuentra en la posici�n indicada, con una tolerancia
	*/
	template <class TNode>
	class CConditionNear : public ICondition<TNode>
	{
	public:
		CConditionNear(Vector3 position, float tolerance) : _position(position), _tolerance(tolerance) {}

		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			
			// TODO PR�CTICA IA
			// Tiene que devolver true cuando la posici�n de la entidad sea
			// igual a la posici�n que ha recibido como par�metro (con la tolerancia)
			return entity->getPosition().positionEquals(_position, _tolerance);
		}

	protected:
		Vector3 _position;
		float _tolerance;
	};

} // namespace AI 

#endif // __AI_Condition_H
