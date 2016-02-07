/**
@file Movement.h

Declaraci�n de la interfaz IMovement, que define los 
m�todos comunes de las clases de movimiento cinem�tico 
y din�mico.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/
#pragma once

#ifndef __AI_Movement_H
#define __AI_Movement_H

#include "Logic/Entity/Entity.h"


using namespace Logic;

namespace AI 
{
	/**
	En esta interfaz se declaran los m�todos comunes para las clases que
	implementar�n el movimiento, tanto din�mico como cinem�tico.
	*/
	class IMovement 
	{
	public:
		/**
		Constante en la que se almacena un valor de tiempo que medir� cu�nto
		se aten�a un movimiento en el caso de que tenga que frenar.
		*/
		const static float ATTENUATION;
		/**
		Constante en la que se almacena un valor de distancia que medir� a qu� 
		distancia se empieza a ralentizar el movimiento para llegar a un destino.
		*/
		const static float SLOW_RADIUS;

		/**
		Esta estructura se utiliza como entrada y salida de los m�todos de movimiento.
		*/
		struct DynamicMovement {
			Vector3 linearSpeed;
			double angularSpeed;
			Vector3 linearAccel;
			double angularAccel;

			DynamicMovement() : linearSpeed(Vector3::ZERO), angularSpeed(0), 
				linearAccel(Vector3::ZERO), angularAccel(0) {};
		};

		/**
		Distintos tipos de movimiento para el m�todo de factor�a.
		*/
		enum EnmMovementType {
			MOVEMENT_NONE = 0,
			MOVEMENT_KINEMATIC_SEEK,
			MOVEMENT_KINEMATIC_ARRIVE,
			MOVEMENT_KINEMATIC_ALIGN_TO_SPEED,
			MOVEMENT_KINEMATIC_FLEE,
			MOVEMENT_DYNAMIC_SEEK,
			MOVEMENT_DYNAMIC_ARRIVE,
			MOVEMENT_DYNAMIC_FLEE
		};

		/** 
		Constructor.

		@param maxLinearSpeed Velocidad lineal m�xima.
		@param maxAngularSpeed Velocidad angular m�xima.
		@param maxLinearAccel Aceleraci�n lineal m�xima.
		@param maxAngularAccel Aceleraci�n angular m�xima.
		*/
		IMovement(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
			_entity(0), _maxLinearSpeed(maxLinearSpeed), _maxAngularSpeed(maxAngularSpeed), 
				_maxLinearAccel(maxLinearAccel), _maxAngularAccel(maxAngularAccel), 
				_maxLinearSpeed2(maxLinearSpeed * maxLinearSpeed), _maxLinearAccel2(maxLinearAccel * maxLinearAccel) {};
		/**
		Destructor
		*/
		~IMovement() {};
		/**
		Establece cu�l es la entidad que se est� moviendo.
		*/
		void setEntity(CEntity* entity) { _entity = entity; };
		/** 
		Establece la posici�n de destino del movimiento.
		*/
		void setTarget(Vector3 &target) { _target = target; };
		/** 
		Devuelve la posici�n de destino del movimiento. 
		*/
		Vector3 getTarget() { return _target; };
		/**
		M�todo virtual que tendr�n que implementar todos los movimientos.
		
		@param msecs Duraci�n del movimiento.
		@param currentProperties Par�metro de entrada/salida en el que se pasan las velocidades
		y aceleraciones actuales y se devuelven modificadas de acuerdo al tipo de movimiento.
		*/
		virtual void move(unsigned int msecs, DynamicMovement& currentProperties) = 0;
		/**
		M�todo de factor�a que devuelve un movimiento a partir de un enumerado.
		*/
		static IMovement* getMovement(int type, float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel);

	protected:
		/**
		Entidad que se mueve. En general la vamos a usar para obtener la posici�n.
		*/
		CEntity* _entity;
		/**
		Destino del movimiento
		*/
		Vector3 _target;
		/** 
		L�mites de velocidad y aceleraci�n lineales y angulares 
		*/
		float _maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel;
		/** 
		L�mites de velocidad y aceleraci�n cuadr�ticos. Se pueden usar para compararlos 
		con la distancia al cuadrado, que es m�s barata de calcular que la distancia normal.
		*/
		float _maxLinearSpeed2, _maxLinearAccel2;


	}; // class IMovement 

} //namespace AI 

#endif //__AI_Movement_H