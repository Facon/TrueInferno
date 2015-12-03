using UnityEngine;
using System.Collections.Generic;
using System;

//[ExecuteInEditMode]
public class TileManager : MonoBehaviour {
    public GameObject tile;
    //public GameObject[,] tiles;
    public List<List<GameObject>> tiles;

    // Use this for initialization
    void Start () {
        int[,] map = parseMap();
        int sizeX = (int)Mathf.Sqrt(map.Length);
        int sizeZ = sizeX;

        Debug.Log("Tamaño Array: " + map.Length);

        for (uint z = 0; z < sizeZ; ++z)
        {
            for (uint x = 0; x < sizeX; ++x)
            {
                GameObject newTile = Instantiate(tile, new Vector3(x, 0, sizeZ - z), Quaternion.identity) as GameObject;
                newTile.transform.parent = this.transform;
                newTile.name = "Tile_x" + x + "_z" + z;
                var features = newTile.GetComponent<TileFeatures>();
                Debug.Log("X: " + x + ", Z: " + z);
                features.type = map[z, x];
                features.init();
            }
        }

        /*
        var maxX = 20;
        var maxZ = 20;

        tiles = new GameObject[maxZ, maxX];

        for(var z = 0; z < maxZ; ++z)
        {
            for (var x = 0; x < maxX; ++x)
            {
                GameObject newTile = Instantiate(tile, new Vector3(x, 0, z), Quaternion.identity) as GameObject;
                newTile.transform.parent = this.transform;
                newTile.name = "Tile_x" + x + "_z" + z;
            }
        }
        */
    }

    private int[,] parseMap()
    {
        int sizeX = 10;
        int sizeY = sizeX;

        int[,] data = new int[10, 10]
        {
            { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, },
            { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, },
            { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, },
            { 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, },
            { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, },
            { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, },
            { 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, },
            { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, },
            { 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, },
            { 1, 2, 2, 0, 0, 0, 0, 0, 0, 1, },
        };

        return data;
    }

    // Update is called once per frame
    void Update () {
	
	}
}
