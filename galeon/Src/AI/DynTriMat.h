/**
@File DynTriMat.h

Implementaci�n de una clase para representar matrices 
triangulares din�micas.

@see CDynTriMat

@author Gonzalo Fl�rez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_DynTriMat_H
#define __AI_DynTriMat_H

#include <vector>
#include <assert.h>

/**
Clase que implementa una matriz triangular. Adem�s, su implementaci�n 
es din�mica, en el sentido de que se pueden a�adir m�s elementos a la 
matriz en cualquier momento.
*/
template <class T>
class CDynTriMat
{
public:
	/**
	Constructor.
	*/
	CDynTriMat(void) : _maxSize(0) {};
	/**
	Destructor.
	*/
	~CDynTriMat(void) {};
	/**
	Devuelve el elemento en la posici�n de la matriz [a, b]. 
	Es el mismo que el que se encuentra en [b, a].
	<p>
	Si la fila o columna indicad por a o b es mayor o igual que 
	el tama�o de la matriz, falla.
	
	@return Elemento en [a, b].
	*/
	T getElement(int a, int b) const;
	/**
	Establece el valor del elemento en la posici�n [a, b].
	Al ser una matriz triangular, el elemento de la posici�n 
	[b, a] tomar� el mismo valor. 
	<p>
	Si el tama�o de la matriz no es lo suficientemente grande 
	como para llegar a esta posici�n la matriz crecer�.
	@param a Columna de la matriz.
	@param b Fila de la matriz.
	@param value Valor del elemento.
	*/
	void setElement(int a, int b, T value);
	/**
	Devuelve las dimensiones de la matriz. Al ser una matriz cuadrada 
	s�lo devuelve un valor.

	@return Dimensi�n de la matriz.
	*/
	int getCurrentSize();

private:
	/**
	Vector donde se almacenar�n los datos.
	*/
	std::vector<T> _data;
	/**
	Dimensi�n de la matriz.
	*/
	int _maxSize;
	/**
	Esta funci�n transforma un valor de [columna, fila] en la posici�n 
	correspondiente del vector _data
	*/
	int getPosition(int a, int b) const;
};

template <class T>
inline T CDynTriMat<T>::getElement(int a, int b) const {
	assert(a < _maxSize && b < _maxSize);
	int pos = getPosition(a, b);
	return _data[pos];
}

template <class T>
inline void CDynTriMat<T>::setElement(int a, int b, T value) {
	// Nos aseguramos que a >= b
	if (a < b) setElement(b, a, value); 
	int pos = getPosition(a, b);
	// Si es una posici�n nueva generamos la fila y columna entera
	if ((int)_data.size() < pos + 1) {
		// Obtenemos la posici�n del �ltimo elemento de la nueva fila
		int ultPos = getPosition(a, a);
		_data.resize(ultPos + 1);
		_maxSize = a + 1;
	}
	_data[pos] = value;
}

template <class T>
inline int CDynTriMat<T>::getPosition(int a, int b) const {
	if (a < b) return getPosition(b, a);
	return a * (a + 1) / 2 + b;
}

template <class T>
inline int CDynTriMat<T>::getCurrentSize() { 
	return _maxSize; 
}

/*
Por si es necesario en el futuro:
Para invertir la operaci�n de acceso y sacar los �ndices de la matriz
a partir de una posici�n del vector, las f�rmulas que hay que aplicar son:
a(v) = floor(-1 + sqrt(1 + 8 * v)) / 2
b(v) = v - a(v) * (a(v) + 1) / 2
Devuelve los �ndices en la matriz triangular superior.
*/

#endif