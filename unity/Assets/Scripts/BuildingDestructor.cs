using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class BuildingDestructor : MonoBehaviour {


    public void DelayedDestruction(float seconds)
    {
        StartCoroutine(_destroy(seconds));
    }

    IEnumerator _destroy(float time)
    {
        yield return new WaitForSeconds(time);

        // Removes Building from building list
        TileManager tm = GameObject.Find("Map").GetComponent<TileManager>();
        tm.buildingList.Remove(gameObject);

        //Get Map Tiles before destruction
        List<Tile> tiles = gameObject.GetComponent<Building>().getTiles();


        foreach (Tile tile in tiles)
        {
            tile.buildingType = TileType.EMPTY;
        }

        Destroy(gameObject);
        
    }

	// Use this for initialization
    public void DestroyBuilding(float delay)
    {
        // Set Especial color for destruction
        gameObject.GetComponent<Renderer>().material.SetColor("_SpecColor", Color.blue);
        DelayedDestruction(delay);
    }
}
