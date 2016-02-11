/**
@file KinematicMovement.h

Contiene distintas clases que heredan de IMovement y que 
implementan distintos tipos de movimiento cinem�ticos.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/

#pragma once

#ifndef __AI_KinematicMovement_H
#define __AI_KinematicMovement_H

#include "movement.h"

namespace AI 
{

	/**
	Movimiento Seek. 
	Intenta llegar a un punto de destino empleando siempre la m�xima velocidad posible. 
	En realidad lo m�s probable es que el movimiento nunca llegue al destino sino que se pase de 
	largo, de la vuelta y permanezca oscilando hasta que hagamos que se detenga.
	*/
	class CKinematicSeek : public IMovement
	{
	public:
		/**
		Constructor
		*/
		CKinematicSeek(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
		  IMovement(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel) { };
		/**
		Efect�a el movimiento.
		
		@param msecs Tiempo que dura el movimiento.
		@param currentProperties Par�metro de entrada/salida donde se reciben las velocidades actuales y 
		en �l se devuelven los nuevos valores de velocidad.
		*/
		void move(unsigned int msecs, DynamicMovement& currentProperties);

	}; // class CKinematicSeek

	/**
	Movimiento Arrive. 
	Intenta llegar a un punto de destino disminuyendo la velocidad seg�n se acerca. De esta 
	manera nos aseguramos de que llegue sin pasarse de largo.
	*/
	class CKinematicArrive : public IMovement
	{
	public:
		/**
		Constructor
		*/
		CKinematicArrive(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
			IMovement(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel) { };
		/**
		Efect�a el movimiento.
		
		@param msecs Tiempo que dura el movimiento.
		@param currentProperties Par�metro de entrada/salida donde se reciben las velocidades actuales y 
		en �l se devuelven los nuevos valores de velocidad.
		*/
		void move(unsigned int msecs, DynamicMovement& currentProperties);

	}; // class CKinematicArrive

	/**
	Movimiento Flee. 
	Intenta huir de un punto de destino a la m�xima velocidad. 
	*/
	class CKinematicFlee : public IMovement
	{
	public:
		/**
		Constructor
		*/
		CKinematicFlee(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
			IMovement(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel) { };
		/**
		Efect�a el movimiento.
		
		@param msecs Tiempo que dura el movimiento.
		@param currentProperties Par�metro de entrada/salida donde se reciben las velocidades actuales y 
		en �l se devuelven los nuevos valores de velocidad.
		*/
		void move(unsigned int msecs, DynamicMovement& currentProperties);

	}; // class CKinematicFlee

	/**
	Este movimiento modifica la velocidad angular para hacer que la orientaci�n de la entidad coincida con la 
	orientaci�n del vector de velocidad lineal (i.e. que mire hacia donde se mueve).
	*/
	class CKinematicAlignToSpeed : public IMovement
	{
	public:
		/**
		Constructor
		*/
		CKinematicAlignToSpeed(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
			IMovement(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel) { };
		/**
		Efect�a el movimiento.
		
		@param msecs Tiempo que dura el movimiento.
		@param currentProperties Par�metro de entrada/salida donde se reciben las velocidades actuales y 
		en �l se devuelven los nuevos valores de velocidad.
		*/
		void move(unsigned int msecs, DynamicMovement& currentProperties);
	}; // class CKinematicAlignToSpeed

} //namespace AI 

#endif // __AI_KinematicMovement_H
