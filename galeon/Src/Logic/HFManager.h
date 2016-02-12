#ifndef HFMANAGER_H_
#define HFMANAGER_H_

namespace Logic
{
	class HFManager
	{
	protected:
		float _hadesFavor = 0.0f;

	public:
		float getHadesFavor() const { return _hadesFavor; }

		void setHadesFavor(const float hadesFavor) { _hadesFavor = hadesFavor; }
	};
}

#endif //HFMANAGER_H_