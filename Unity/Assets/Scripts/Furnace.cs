using UnityEngine;
using System.Collections;

public class Furnace : MonoBehaviour {
    public TileManager tileManager;
    public ResourceManager resourceManager;

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

    public void buildingBuilt()
    {
        tileManager.SendMessage("furnaceBuilt", this, SendMessageOptions.RequireReceiver);
    }
    
    public void soulReadyToBurn(SoulTaskExecutor s)
    {
        if (GetComponent<Building>().isWorking())
            resourceManager.burnSouls(1);
    }
}
