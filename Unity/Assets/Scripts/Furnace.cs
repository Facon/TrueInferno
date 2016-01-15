using UnityEngine;
using System.Collections;

public class Furnace : MonoBehaviour {
    private TileManager tileManager;
    private ResourceManager resourceManager;

    /// <summary>
    /// Single resource increment per soul used.
    /// </summary>
    private const float COKE_PER_SOUL = 6;
    private const float CRUDE_PER_SOUL = 4;

	// Use this for initialization
	void Start () 
    {
        GameObject goMap = GameObject.FindGameObjectWithTag("Map");
        tileManager = goMap.GetComponent<TileManager>();

        resourceManager = GameObject.FindGameObjectWithTag("ResourceManager").GetComponent<ResourceManager>();
	}
	
	// Update is called once per frame
	void Update () 
    {
	}
    
    public void soulReadyToBurn()
    {
        if (GetComponent<Building>().isWorking())
        {
            resourceManager.increaseResources(ResourceType.Coke, COKE_PER_SOUL);
            SendMessage("addInputResources", CRUDE_PER_SOUL, SendMessageOptions.RequireReceiver);
        }
    }
}
