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
		static const float RES_1_BASE_GATHERING_SPEED = 20.f / 60.f;
		static const float RES_2_BASE_GATHERING_SPEED = 12.f / 60.f;
		
		static const res1GatheringSpeed = RES_1_BASE_GATHERING_SPEED;
		static const res2GatheringSpeed = RES_2_BASE_GATHERING_SPEED;
		
		float res1 = 0.0f;
		float res2 = 0.0f;
		float coke = 0.0f;
		float crude = 0.0f;
		float pure = 0.0f;
		float refined = 0.0f;
		
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
		void incRes1(int workers, float time);
		
		void incRes2(int workers, float time);
		
		// TODO Refactorize/Generalize all resources
		void increaseResources(ResourceType type, float num);
	};
}