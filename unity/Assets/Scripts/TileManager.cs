using System;
using System.Collections.Generic;
using UnityEngine;

public enum TileType
{
    EMPTY, OBSTACLE, ROAD, BUILDING
}

public class TileManager : MonoBehaviour
{
    private const uint SIZE_X = 10;
    private const uint SIZE_Z = 10;

    public Tile tile;

    /// <summary>
    ///  Matrix[z,x] with the tile GameObject. 
    /// </summary>
    public Tile[,] tiles;

    /// <summary>
    ///  Matrix[z,x] with the tile type info. 
    /// TODO A tile type should be attached somehow to its tile GameObject. It may be included
    /// in a Component so its value could be changed by reading a specific message
    /// </summary>
    //public TileType[,] tileTypes;

    /// <summary>
    /// Char delim for tile names
    /// </summary>
    private char TILE_NAME_DELIM = '_';

    // Use this for initialization
    void Start()
    {
        int[,] map = parseMap();
        //int sizeX = (int)Mathf.Sqrt(map.Length);
        //int sizeZ = sizeX;

        Debug.Log("Tamaño Array: " + map.Length);

        tiles = new Tile[sizeX, sizeZ];
        //tileTypes = new TileType[sizeZ, sizeX];

        for (uint z = 0; z < sizeZ; ++z)
        {
            for (uint x = 0; x < sizeX; ++x)
            {
                Tile newTile = Instantiate(tile, new Vector3(x, 0, sizeZ - z), Quaternion.identity) as Tile;
                newTile.transform.parent = this.transform;
                //newTile.name = "Tile_x" + x + "_z" + z;
                newTile.name = getTileNameFromCoords(z, x);
                newTile.tag = "Tile";
                var features = newTile.GetComponent<Tile>();
                //Debug.Log("X: " + x + ", Z: " + z);
                features.posX = x;
                features.posZ = z;
                features.buildingType = TileType.EMPTY;
                features.type = map[z, x];
                //Debug.Log("tile "+z+" "+x+" es de tipo "+features.type);
                features.init();

                // Update tile map
                tiles[x, z] = newTile;

                // TODO quick-fix...
                // Update tile type map
                //switch(map[z, x])
                //{
                  //  case 0: tileTypes[z, x] = TileType.EMPTY; break;
                    //default: tileTypes[z, x] = TileType.OBSTACLE; break;
                //}
            }
        }
    }

    /// <summary>
    ///  Bean for our 2D points with z and x coordinates
    /// </summary>
    public class Vector2zx
    {
        public float z;
        public float x;

        public Vector2zx(float z, float x)
        {
            this.z = z;
            this.x = x;
        }
    }

    public Vector2zx getTileCoords(Tile tile)
    {
        return getCoordsFromTileName(tile.name);
    }

    public Vector2zx getCoordsFromTileName(String name)
    {
        String[] parts = name.Split(TILE_NAME_DELIM);
        return new Vector2zx(float.Parse(parts[2]), float.Parse(parts[1]));
    }

    public string getTileNameFromCoords(uint z, uint x)
    {
        return "Tile" + TILE_NAME_DELIM + x + TILE_NAME_DELIM + z;
    }

    /// <summary>
    /// Convenience wrapper method for getCrossAdyacents(int z, int x)
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    public HashSet<Vector2zx> getCrossAdyacents(Vector2zx v)
    {
        return getCrossAdyacents((uint)v.z, (uint)v.x);
    }

    /// <summary>
    /// Get 4 cross-adyacent z,x coordinate pairs for a given pair of coordinates. e.g. for (1,2) returns {(1,1),(1,3),(0,2),(2,2)}.
    /// Takes into consideration min and max values for each dimension
    /// </summary>
    /// <param name="z"></param>
    /// <param name="x"></param>
    /// <returns></returns>
    public HashSet<Vector2zx> getCrossAdyacents(uint z, uint x)
    {
        if (z < 0 || z >= SIZE_Z || x < 0 || x >= SIZE_X)
            throw new Exception("z=" + z + " must be in [0," + (SIZE_Z - 1) + "] and x=" + x + " must be in [0," + (SIZE_X - 1) + "]");

        HashSet<Vector2zx> ret = new HashSet<Vector2zx>();

        if (z >= 1)
            ret.Add(new Vector2zx(z - 1, x));

        if (z <= SIZE_Z - 2)
            ret.Add(new Vector2zx(z + 1, x));

        if (x >= 1)
            ret.Add(new Vector2zx(z, x - 1));

        if (x <= SIZE_X - 2)
            ret.Add(new Vector2zx(z, x + 1));

        return ret;
    }

    public uint getSizeX()
    {
        return SIZE_X;
    }

    public uint getSizeZ()
    {
        return SIZE_Z;
    }

    // Update is called once per frame
    void Update()
    {

    }

    private int[,] parseMap()
    {

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
}