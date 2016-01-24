/**
@file Graphics.h

Contiene la declaraci�n del componente que controla la representaci�n
gr�fica de la entidad.

@see Logic::CGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Logic_Graphics_H
#define __Logic_Graphics_H

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CEntity;
	class CScene;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representaci�n gr�fica de una entidad.
	En concreto se encarga de las entidades con representaciones gr�ficas
	no animadas. Para otros tipos de representaciones hay otros componentes
	que especializan �ste (CAnimatedGraphics y CStaticGraphics).
	<p>
	Acepta mensajes de:
	1) cambio de posici�n y orientaci�n (matriz de transformaci�n) mediante
	el mensaje SET_TRANSFORM.
	2) cambio de escala mediante el mensaje SET_SCALE.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
*/
	class CGraphics : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(CGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CGraphics() : IComponent(), _graphicsEntity(0) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CGraphics();
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deber� cargar e invoca al m�todo virtual
		createGraphicsEntity() para generar la entidad gr�fica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	protected:

		/**
		M�todo virtual que construye la entidad gr�fica de la entidad. Otros
		componentes pueden sobreescribirlo para inicializar otro tipo de
		entidades gr�ficas (animadas, etc.).
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Entidad gr�fica creada, NULL si hubo alg�n problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(const Map::CEntity *entityInfo);

		/**
		Atributo con el nombre del modelo gr�fico de la entidad.
		*/
		std::string _model;
		
		/**
		Entidad gr�fica.
		*/
		Graphics::CEntity *_graphicsEntity;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la destrucci�n de la entidad gr�fica.
		*/
		Graphics::CScene* _scene;

	}; // class CGraphics

	REG_FACTORY(CGraphics);

} // namespace Logic

#endif // __Logic_Graphics_H
