using System;
using System.Collections.Generic;
using UnityEngine;

public enum TileType
{
    EMPTY, OBSTACLE, ROAD, BUILDING
}

public class TileManager : MonoBehaviour
{
    private const uint SIZE_X = 15;
    private const uint SIZE_Z = 15;

    public Tile tile;
    public GameObject prefabWorker;

    /// <summary>
    ///  Matrix[x,z] with the tile GameObject. 
    /// </summary>
    public Tile[,] tiles;

    /// <summary>
    /// Char delim for tile names
    /// </summary>
    private char TILE_NAME_DELIM = '_';

    // Use this for initialization
    void Start()
    {
        int[,] map = parseMap();

        Debug.Log("Tamaño Array: " + map.Length);

        tiles = new Tile[SIZE_X, SIZE_Z];

        for (uint x = 0; x < SIZE_X; ++x)
        {
            for (uint z = 0; z < SIZE_Z; ++z)
            {
                Tile newTile = Instantiate(tile, new Vector3(x, 0, z), Quaternion.identity) as Tile;
                newTile.transform.parent = this.transform;
                newTile.name = getTileNameFromCoords(x, z);
                newTile.tag = "Tile";
                var features = newTile.GetComponent<Tile>();
                //Debug.Log("X: " + x + ", Z: " + z);
                features.posX = x;
                features.posZ = z;
                features.buildingType = TileType.EMPTY;
                features.type = map[x, z];
                //Debug.Log("tile "+z+" "+x+" es de tipo "+features.type);
                features.init();

                // Update tile map
                tiles[x, z] = newTile;
            }
        }

        // Create test workers
        GameObject worker1 = GameObject.Instantiate(prefabWorker, tiles[0, 0].transform.position + Vector3.up, Quaternion.identity) as GameObject;
        Tile from = tiles[3, 0];
        Tile to = tiles[7, 8];
        //List<Tile> path = GetComponent<PathFinder>().GetWorkerPath(from, to);
        List<Tile> path = GetComponent<PathFinder>().GetRoadPath(from, to);
        worker1.GetComponent<PathFollower>().AddToQueue(path);

        GameObject worker2 = GameObject.Instantiate(prefabWorker, tiles[2,2].transform.position + Vector3.up, Quaternion.identity) as GameObject;
        from = tiles[0, 8];
        to = tiles[14, 13];
        //List<Tile> path = GetComponent<PathFinder>().GetWorkerPath(from, to);
        path = GetComponent<PathFinder>().GetRoadPath(from, to);
        worker2.GetComponent<PathFollower>().AddToQueue(path);
    }

    /// <summary>
    ///  Bean for our 2D points with x and z coordinates
    /// </summary>
    public class Vector2xz
    {
        public float x;
        public float z;

        public Vector2xz(float x, float z)
        {
            this.x = x;
            this.z = z;
        }
    }

    public Vector2xz getTileCoords(Tile tile)
    {
        return getCoordsFromTileName(tile.name);
    }

    public Vector2xz getCoordsFromTileName(String name)
    {
        String[] parts = name.Split(TILE_NAME_DELIM);
        return new Vector2xz(float.Parse(parts[1]), float.Parse(parts[2]));
    }

    public string getTileNameFromCoords(uint x, uint z)
    {
        return "Tile" + TILE_NAME_DELIM + x + TILE_NAME_DELIM + z;
    }

    /// <summary>
    /// Convenience wrapper method for getCrossAdyacents(int x, int z)
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    public HashSet<Vector2xz> getCrossAdyacents(Vector2xz v)
    {
        return getCrossAdyacents((uint)v.x, (uint)v.z);
    }

    /// <summary>
    /// Get 4 cross-adyacent x,z coordinate pairs for a given pair of coordinates. e.g. for (1,2) returns {(1,1),(1,3),(0,2),(2,2)}.
    /// Takes into consideration min and max values for each dimension
    /// </summary>
    /// <param name="x"></param>
    /// <param name="z"></param>
    /// <returns></returns>
    public HashSet<Vector2xz> getCrossAdyacents(uint x, uint z)
    {
        if (x < 0 || x >= SIZE_X || z < 0 || z >= SIZE_Z)
            throw new Exception("x=" + x + " must be in [0," + (SIZE_X - 1) + "] and z=" + z + " must be in [0," + (SIZE_Z - 1) + "]");

        HashSet<Vector2xz> ret = new HashSet<Vector2xz>();

        if (x >= 1)
            ret.Add(new Vector2xz(x - 1, z));

        if (x <= SIZE_X - 2)
            ret.Add(new Vector2xz(x + 1, z));

        if (z >= 1)
            ret.Add(new Vector2xz(x, z - 1));

        if (z <= SIZE_Z - 2)
            ret.Add(new Vector2xz(x, z + 1));

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

        int[,] data = new int[15, 15]
        {
            { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0 },
            { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2 },
            { 1, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 0 },
            { 0, 0, 0, 0, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 1, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0 },
            { 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
        };

        return data;
    }
}