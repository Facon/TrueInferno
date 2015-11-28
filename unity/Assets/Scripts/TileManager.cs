using UnityEngine;
using System.Collections;

public class TileManager : MonoBehaviour {
    public GameObject tile;
    public GameObject[,] tiles;

	// Use this for initialization
	void Start () {
        var maxX = 4;
        var maxZ = 4;

        tiles = new GameObject[maxZ, maxX];

        for(var z = 0; z < maxZ; ++z)
        {
            for (var x = 0; x < maxX; ++x)
            {
                GameObject newTile = Instantiate(tile, new Vector3(x, 0, z), Quaternion.identity) as GameObject;
                newTile.transform.parent = this.transform;
                newTile.name = "Tile_x" + x + "_z" + (z + 1);
            }
        }
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
