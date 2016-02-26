/**
@file ErrorManager.h

Contiene la declaración del gestor de errores para PhysX.

@see Physics::CErrorManager

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Physics_ErrorManager_H
#define __Physics_ErrorManager_H

#include <foundation/PxErrorCallback.h>


// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	 Gestor de errores para PhysX. Cuando PhysX detecta que se intenta hacer algo incorrecto 
	 (por ejemplo, llamadas a funciones con parámetros no válidos), invoca a este gestor 
	 que a su vez imprime el mensaje en la consola.

	 @ingroup physicGroup

	 @author Antonio Sánchez Ruiz-Granados
	 @date Noviembre, 2012
	 */
	class CErrorManager : public physx::PxErrorCallback
	{
	public:
		/**
		 * Constructor por defecto.
		 */
		CErrorManager();

		/**
		 * Destructor.
		 */
		~CErrorManager();

		/**
		 * Método invocado por PhysX cada vez que se produce un error. El mensaje de error se 
		 * imprime por pantalla.
		 */
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);

		/**
		* Cambia el valor del flag para los mensajes.
		*
		* @param off Nuevo valor del flag. 
		*/
		void setOff(const bool off) { _off = off; }

	private:

		/**
		 * Flag para desactivar temporalmente los mensajes de log de PhysX (normalmente por
		 * la repetición masiva de alguno concreto que inunda la ventana de log y no deja ver
		 * nada más).
		 *
		 * Esto es una solución rápida para la parte de desarrollo, tarde o temprano habrá
		 * que quitar esto y buscar la forma correcta de hacer todo aquello que provoque
		 * mensajes de error o warning!
		 */
		bool _off = false;

	}; // classCErrorManager

}; // namespace Physics

#endif // __Physics_ErrorManager_H