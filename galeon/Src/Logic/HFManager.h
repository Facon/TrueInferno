#ifndef HFMANAGER_H_
#define HFMANAGER_H_

namespace Logic
{
	class HFManager
	{
	protected:
		static HFManager _instance;
		float _hadesFavor;

	private:
		HFManager() : _hadesFavor(0.0f)
		{}

	public:
		static HFManager& getSingleton()
		{ return _instance; }

		static HFManager* getSingletonPtr()
		{ return &_instance; }

		float getHadesFavor() const { return _hadesFavor; }

		void setHadesFavor(const float hadesFavor) { _hadesFavor = hadesFavor; }
	};
}

#endif //HFMANAGER_H_