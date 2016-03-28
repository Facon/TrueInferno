#ifndef LA_TRANSFORM_RESOURCES_
#define LA_TRANSFORM_RESOURCES_

#include "Logic\Entity\Message.h"
#include "AI\LatentAction.h"
#include "AI\SMResourceTransformerData.h"

namespace AI {
	class CLATransformResources : public CLatentAction {
	public:
		CLATransformResources(CEntity* entity, CSMResourceTransformerData& smData, 
			ResourceType resourceFrom, ResourceType resourceInto, 
			float transformRatio, ResourceType costResource, float costRatio) :

			CLatentAction(entity), _smData(smData), 
			_resourceFrom(resourceFrom), _resourceInto(resourceInto),
			_transformRatio(transformRatio), _costResource(costResource), _costRatio(costRatio),
			_transformed(0), _decrementDone(false), _incrementDone(false) {}

		virtual ~CLATransformResources() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceTransformerData& _smData;

		/** Recurso de entrada */
		const ResourceType _resourceFrom;

		/** Recurso de salida */
		const ResourceType _resourceInto;

		/** Ratio de conversi�n del recurso de entrada en el recurso de salida */
		const float _transformRatio;

		/** Tipo de recurso para los costes */
		const ResourceType _costResource;

		/** Ratio del recurso de costes sobre los recursos de entrada */
		const float _costRatio;

		/** Cantidad de recursos que ser� transformada */
		unsigned int _transformed;

		/** Flag para se�alar que el decremento de los recursos de entrada ya est� hecho */
		bool _decrementDone;

		/** Flag para se�alar que el incremento de los recursos de salida ya est� hecho */
		bool _incrementDone;
	};
}

#endif // LA_TRANSFORM_RESOURCES_