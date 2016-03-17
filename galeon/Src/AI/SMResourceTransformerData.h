#ifndef SM_RESOURCE_TRANSFORMER_DATA_
#define SM_RESOURCE_TRANSFORMER_DATA_

namespace AI {
	class CSMResourceTransformerData {

	public:
		CSMResourceTransformerData() : _numSoulsToBurn(0) {}

		virtual ~CSMResourceTransformerData() {}

		unsigned int getNumSoulsToBurn(){
			return _numSoulsToBurn;
		}

		void setNumSoulsToBurn(unsigned numSouls){
			_numSoulsToBurn = numSouls;
		}

		void increaseNumSoulsToBurn(unsigned numSouls){
			_numSoulsToBurn += numSouls;
		}

	private:
		/** Almas preparadas para ser quemadas */
		unsigned int _numSoulsToBurn;
	};

}

#endif