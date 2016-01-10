using UnityEngine;
using System.Collections;

public class Furnace : MonoBehaviour {
    public TileManager tileManager;

	// Use this for initialization
	void Start () 
    {
        GameObject goMap = GameObject.FindGameObjectWithTag("Map");
        tileManager = goMap.GetComponent<TileManager>();
	}
	
	// Update is called once per frame
	void Update () 
    {
	}

    public void buildingBuilt()
    {
        tileManager.SendMessage("furnaceBuilt", this, SendMessageOptions.RequireReceiver);
    }

}
