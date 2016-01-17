using UnityEngine;
using System.Collections;

public class BuildingDestructor : MonoBehaviour {

	// Use this for initialization
    public void DestroyBuilding(float delay) 
    {
        // Set Especial color for destruction
        gameObject.GetComponent<Renderer>().material.SetColor("_SpecColor", Color.blue);

        // Removes Building from building list
        TileManager tm = GameObject.Find("Map").GetComponent<TileManager>();
        tm.buildingList.Remove(gameObject);

        // Destroy the building in a delayed time
        Destroy(gameObject, delay);
    }
}
