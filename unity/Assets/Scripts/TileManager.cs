using System;
using System.Collections.Generic;
using UnityEngine;
using Assets.Scripts;

public enum TileType
{
    EMPTY, OBSTACLE, ROAD, BUILDING
}

public class TileManager : MonoBehaviour
{
    private const uint SIZE_X = 15;
    private const uint SIZE_Z = 15;

    public Tile tile;
    public GameObject prefabTownHall;
    public GameObject prefabElevator;

    private GameObject townHall;
    private TownHall townHallComp;

    private Furnace furnace;

    public List<GameObject> buildingList;

    public WorkerManager workerManager;

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
        workerManager = GameObject.FindGameObjectWithTag("WorkerManager").GetComponent<WorkerManager>();
        if (workerManager == null)
            Debug.LogError("No WorkerManager found");

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

        // Create townhall and elevator
        //MouseMoveLock mMove = GameObject.Find("GameController").GetComponent<MouseMoveLock>();

        Building buildcomp = prefabTownHall.GetComponent<Building>();
        townHall = Instantiate(prefabTownHall, new Vector3(tiles[13, 1].posX, buildcomp.sizeY / 2 + 0.1f, tiles[13, 1].posZ), Quaternion.identity) as GameObject;
        townHall.transform.localScale = new Vector3(buildcomp.sizeX, buildcomp.sizeY, buildcomp.sizeZ);

        Building townHallBuildingComp = townHall.GetComponent<Building>();
        //mMove.setOcuppiedTile(buildcomp, tiles[2, 8].posX, tiles[2, 8].posZ);
        tiles[14, 0].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[14, 0]);
        tiles[13, 0].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[13, 0]);
        tiles[12, 0].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[12, 0]);
        tiles[14, 1].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[14, 1]);
        tiles[13, 1].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[13, 1]);
        tiles[12, 1].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[12, 1]);
        tiles[14, 2].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[14, 2]);
        tiles[13, 2].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[13, 2]);
        tiles[12, 2].buildingType = TileType.BUILDING; townHallBuildingComp.addTile(tiles[12, 2]);

        townHallComp = townHall.GetComponent<TownHall>();

        buildcomp = prefabElevator.GetComponent<Building>();
        GameObject Elevator = Instantiate(prefabElevator, new Vector3(tiles[13, 8].posX+0.5f, buildcomp.sizeY / 2 +0.1f, tiles[13, 8].posZ), Quaternion.identity) as GameObject;
        Elevator.transform.localScale = new Vector3(buildcomp.sizeX, buildcomp.sizeY, buildcomp.sizeZ);

        Building elevatorBuildingComp = townHall.GetComponent<Building>();
        tiles[14, 8].buildingType = TileType.BUILDING; elevatorBuildingComp.addTile(tiles[14, 8]);
        tiles[13, 8].buildingType = TileType.BUILDING; elevatorBuildingComp.addTile(tiles[14, 8]);
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

        override
        public int GetHashCode()
        {
            return (x + "_" + z).GetHashCode();
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
    public static HashSet<Vector2xz> getCrossAdyacents(uint x, uint z)
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
        if (townHallComp.getNumFreeWorkers() > 0 && buildingList.Count > 0)
        {
            // Select random building
            Building targetBuilding =
                buildingList[UnityEngine.Random.Range(0, buildingList.Count)].GetComponent<Building>();

            // Send worker to the target building
            if (targetBuilding != null && workerManager.sendSoulToBuilding(townHall.GetComponent<Building>(), targetBuilding, new WorkTask()))
                townHallComp.decreaseNumFreeWorkers();
        }
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

    public void furnaceBuilt(Furnace furnace)
    {
        this.furnace = furnace;
    }

    public GameObject getTownHall()
    {
        return townHall;
    }

    public Furnace getFurnace()
    {
        return furnace;
    }

    /// <summary>
    /// Find every building that accepts the resource param as input
    /// </summary>
    /// <param name="resource"></param>
    /// <returns></returns>
    public List<GameObject> findResourceBuildingByInput(ResourceType resource)
    {
        List<GameObject> list = new List<GameObject>();

        // Check every game object
        foreach(GameObject building in buildingList)
        {
            // If it has a ResourceTransformer component
            ResourceTransformer resourceTransformer = building.GetComponent<ResourceTransformer>();
            if (resourceTransformer != null && resourceTransformer.inputResourceType == resource)
                list.Add(building);
        }

        return list;
    }
}