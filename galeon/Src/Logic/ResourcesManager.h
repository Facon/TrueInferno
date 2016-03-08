#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <string>

namespace Logic
{
	enum ResourceType
	{
		NONE = 0xFFFFFFFF,
		MINERAL,
		GAS,
		COKE,
		CRUDE,
		PURE,
		REFINED,
		AETHER,
		//HADES_FAVOR
	};

	class ResourcesManager
	{
	protected:
		/*static const float MINERAL_GATHERING_SPEED;
		static const float GAS_GATHERING_SPEED;*/

		float _mineral;
		float _gas;
		float _coke;
		float _crude;
		float _pure;
		float _refined;
		float _aether;
		//float _hadesFavor = 0.0f;

	public:
		static ResourceType parseResourceType(const std::string& name);

		ResourcesManager() : _mineral(0), _gas(0), _coke(0), _crude(0), _pure(0), _refined(0), _aether(0) {}

		float getMineral() const { return _mineral; }
		float getGas() const { return _gas; }
		float getCoke() const { return _coke; }
		float getCrude() const { return _crude; }
		float getPure() const { return _pure; }
		float getRefined() const { return _refined; }
		float getAether() const { return _aether; }
		//float getHadesFavor() const { return _hadesFavor; }

		/*void setMineral(const float mineral) { _mineral = mineral; }
		void setGas(const float gas) { _gas = gas; }
		void setCoke(const float coke) { _coke = coke; }
		void setCrude(const float crude) { _crude = crude; }
		void setPure(const float pure) { _pure = pure; }
		void setRefined(const float refined) { _refined = refined; }
		void setAether(const float aether) { _aether = aether; }*/
		//void setHadesFavor(const float hadesFavor) { _hadesFavor = hadesFavor; }

		/*void incMineral(int workers, float time);
		
		void incGas(int workers, float time);*/
		
		// TODO Refactorize/Generalize all resources
		void increaseResources(ResourceType type, float num);
	};

}

#endif //RESOURCEMANAGER_H_