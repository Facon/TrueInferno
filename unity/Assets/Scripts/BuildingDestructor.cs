using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class BuildingDestructor : MonoBehaviour {

    private bool destruction;
    Color materialColor;

    void Start()
    {
        destruction = false;
        materialColor = gameObject.GetComponent<Renderer>().material.color;
    }

    public void AvoidDestruction() {

        if (destruction) {
            destruction = false;
            gameObject.GetComponent<Renderer>().material.SetColor("_SpecColor", materialColor);
        }

    }

    public bool CheckIncomingDestruction() {
        return destruction;
    }

    public void DelayedDestruction(float seconds)
    {
        StartCoroutine(_destroy(seconds));
    }

    IEnumerator _destroy(float time)
    {
        destruction = true;

        yield return new WaitForSeconds(time);

        // Removes Building from building list
        if (destruction)
        {
            TileManager tm = GameObject.Find("Map").GetComponent<TileManager>();
            tm.buildingList.Remove(gameObject);

            //Get Map Tiles before destruction
            List<Tile> tiles = gameObject.GetComponent<Building>().getTiles();


            foreach (Tile tile in tiles)
            {
                tile.buildingType = TileType.RUINS;
                tile.type = 3;
                tile.init();
            }

            Destroy(gameObject);
        }
        
    }

	// Use this for initialization
    public void DestroyBuilding(float delay)
    {
        // Set Especial color for destruction
        gameObject.GetComponent<Renderer>().material.SetColor("_SpecColor", Color.yellow);
        DelayedDestruction(delay);
    }
}
