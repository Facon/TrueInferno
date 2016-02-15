/**
@file Graphics.h

Contiene la declaración del componente que controla la representación
gráfica de la entidad.

@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_Graphics_H
#define __Logic_Graphics_H

#include "BaseSubsystems/RTTI.h"
#include "BaseSubsystems/Math.h"

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CEntity;
	class CScene;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representación gráfica de una entidad.
	En concreto se encarga de las entidades con representaciones gráficas
	no animadas. Para otros tipos de representaciones hay otros componentes
	que especializan éste (CAnimatedGraphics y CStaticGraphics).
	<p>
	Acepta mensajes de:
	1) cambio de posición, orientación y dimensiones (escala)
	   mediante SET_POSITION, SET_ROTATION y SET_DIMENSIONS.
	2) cambio de la matriz de transformación (posición +
	   orientación + escala) mediante el mensaje SET_TRANSFORM.
	3) cambio de color mediante el mensaje SET_COLOR.
	4) cambio de material mediante el mensaje SET_MATERIAL_NAME.
	
    @ingroup logicGroup

	@author David Llansó
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
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deberá cargar e invoca al m�todo virtual
		createGraphicsEntity() para generar la entidad gr�fica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa lógico en el que se registrará el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Mensajes.
		*/
		bool HandleMessage(const TransformMessage& m);
		bool HandleMessage(const PositionMessage& msg);
		bool HandleMessage(const RotationMessage& msg);
		bool HandleMessage(const DimensionsMessage& msg);
		bool HandleMessage(const ColorMessage& m);
		bool HandleMessage(const MaterialMessage& m);

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
