using UnityEngine;
using System.Collections.Generic;
using Assets.Scripts;

public class Building : MonoBehaviour {
    public uint sizeX;
    public float sizeY;
    public uint sizeZ;
    public TileType BuildingType;
    public int validTileType;

    private TileManager tileManager;

    private bool visible = true;
    private List<GameObject> list = new List<GameObject>();
    
    private List<Tile> tiles = new List<Tile>();

    // Current number of workers
    private int numWorkers = 0;
    
    // Number of workers so the building is considered active and ready to produce
    private int minWorkers = 1;

    // TODO Number of workers to get the highest production rate from the building
    private int maxWorkers = 4;

    public int resourceType = 0;

    private ResourceManager resourceManager;

    private WorkerManager workerManager;
    
    // Use this for initialization
    void Start() {
        if (sizeX < 1 || sizeY < 0.1 || sizeZ < 1)
            throw new System.Exception("El tamaño de edificio tiene que ser mayor que 1x1x1");

        GameObject goMap = GameObject.FindGameObjectWithTag("Map");
        tileManager = goMap.GetComponent<TileManager>();

        GameObject goResManager = GameObject.FindGameObjectWithTag("ResourceManager");
        resourceManager = goResManager.GetComponent<ResourceManager>();

        workerManager = GameObject.FindGameObjectWithTag("WorkerManager").GetComponent<WorkerManager>();

        /*Vector3 center = gameObject.transform.position;

        // Pitagoras
        var hip = Mathf.Sqrt(Mathf.Pow((float)sizeX, 2) + Mathf.Pow((float)sizeZ, 2));

        var c = hip / 5.7f; // Distance from P(0,0)

        Debug.Log("C: " + c);

        for (uint i = 0; i < sizeX; i += 1)
        {
            for (uint j = 0; j < sizeZ; j += 1)
            {
                GameObject cube;

                if (visible == true)
                    cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
                else
                    cube = new GameObject();

                Vector3 position = center;

                position.x += i;// - c;
                position.z += j;// - c;

                cube.transform.position = position;
                cube.transform.parent = this.transform;

                list.Add(cube);
            }

        }*/
    }

    // Update is called once per frame
    void FixedUpdate () {
        /*
        RaycastHit hitInfo;

        foreach (GameObject go in list)
        {
            Vector3 point = go.transform.position;
            if (Physics.Raycast(point, Vector3.down, out hitInfo))
            {
                if (hitInfo.collider.gameObject.tag == "Tile")
                {
                    Debug.DrawLine(point, hitInfo.point);
                    Debug.Log("Tile HIT:" + hitInfo.collider.gameObject.name);
                }
            }
        }
        */
    }

    void Update()
    {
        incMapResource();
    }

    public List<Tile> getTiles()
    {
        if (tiles == null)
            Debug.LogError("No tiles have been defined for building " + this);

        return tiles;
    }

    public List<Tile> getSurrRoadTiles()
    {
        List<Tile> surrRoadTiles = new List<Tile>();

        // Get building's tiles
        List<Tile> buildingTiles = getTiles();

        // For each tile of the building
        foreach (Tile buildingTile in buildingTiles)
        {
            // For each surrounding tile of the building tile
            foreach (Tile surrTile in getSurrTiles(buildingTile))
            {
                if (surrTile.buildingType == TileType.ROAD)
                    surrRoadTiles.Add(surrTile);
            }
        }

        return surrRoadTiles;
    }

    public HashSet<Tile> getSurrTiles(Tile tile)
    {
        HashSet<Tile> surrTiles = new HashSet<Tile>();

        foreach (TileManager.Vector2xz pos in TileManager.getCrossAdyacents(tile.posX, tile.posZ))
        {
            surrTiles.Add(tileManager.tiles[(uint)pos.x, (uint)pos.z]);
        }

        return surrTiles;
    }

    public void addTile(Tile tile)
    {
        tiles.Add(tile);
    }

    public void incNumWorkers()
    {
        ++numWorkers;
    }

    public void incMapResource()
    {
        if (isWorking())
        {
            if (resourceType == 1)
            {
                resourceManager.incRes1(numWorkers, Time.deltaTime);
            }
            else if (resourceType == 2)
            {
                resourceManager.incRes2(numWorkers, Time.deltaTime);
            }
        }
    }

    public void setMinWorkers(int minWorkers)
    {
        this.minWorkers = minWorkers;
    }

    public bool isWorking()
    {
        return numWorkers >= minWorkers;
    }

    /** Returns float factor in [0,1] proportional to the current number of workers: 0 = inactive; 1 = max work speed */
    public float getWorkFactor()
    {
        if (numWorkers < minWorkers)
            return 0f;
        else if (numWorkers >= maxWorkers)
            return 1f;
        else
            return (numWorkers - minWorkers + 1f) / (maxWorkers - minWorkers + 1f);
    }

    public void soulReadyToWork()
    {
        incNumWorkers();
    }

    /// <summary>
    /// We are going to transport resources by sending a worker
    /// </summary>
    public bool transportResources(TransportTask task)
    {
        if(numWorkers>=minWorkers)
        {
            if (workerManager.sendSoulToBuilding(this, task.getTargetBuilding(), task))
            {
                return true;
            }
        }

        return false;
    }
}