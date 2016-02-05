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
		HADES_FAVOR
	};

	class ResourceManager
	{
	protected:
		static const float MINERAL_GATHERING_SPEED;
		static const float GAS_GATHERING_SPEED;
		
		float _mineral = 0.0f;
		float _gas = 0.0f;
		float _coke = 0.0f;
		float _crude = 0.0f;
		float _pure = 0.0f;
		float _refined = 0.0f;
		float _aether = 0.0f;
		float _hadesFavor = 0.0f;
		
		/*
		// TextDisplays shouldn't be here, there should be a class
		// that deals with resources and another class dealing with it
		
		public Text res1Display; // mineral
		public Text res2Display; // gas
		public Text cokeDisplay; // coke
		public Text crudeDisplay; // crude
		public Text pureDisplay; // pure
		public Text refinedDisplay; // refined
		*/
		
	public:
		float getMineral() const { return _mineral; }
		float getGas() const { return _gas; }
		float getCoke() const { return _coke; }
		float getCrude() const { return _crude; }
		float getPure() const { return _pure; }
		float getRefined() const { return _refined; }
		float getAether() const { return _aether; }
		float getHadesFavor() const { return _hadesFavor; }

		void setMineral(const float mineral) { _mineral = mineral; }
		void setGas(const float gas) { _gas = gas; }
		void setCoke(const float coke) { _coke = coke; }
		void setCrude(const float crude) { _crude = crude; }
		void setPure(const float pure) { _pure = pure; }
		void setRefined(const float refined) { _refined = refined; }
		void setAether(const float aether) { _aether = aether; }
		void setHadesFavor(const float hadesFavor) { _hadesFavor = hadesFavor; }

		void incMineral(int workers, float time);
		
		void incGas(int workers, float time);
		
		// TODO Refactorize/Generalize all resources
		void increaseResources(ResourceType type, float num);
	};
}