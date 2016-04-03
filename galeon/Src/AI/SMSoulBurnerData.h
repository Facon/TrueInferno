#ifndef SM_SOUL_H_BURNER_DATA_
#define SM_SOUL_H_BURNER_DATA_

namespace AI {
	class CSMSoulBurnerData {

	public:
		CSMSoulBurnerData() : _numSoulsToBurn(0) {}

		virtual ~CSMSoulBurnerData() {}

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