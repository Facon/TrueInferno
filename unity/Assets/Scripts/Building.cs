using UnityEngine;
using System.Collections.Generic;

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

    // Use this for initialization
    void Start() {
        if (sizeX < 1 || sizeY < 0.1 || sizeZ < 1)
            throw new System.Exception("El tamaño de edificio tiene que ser mayor que 1x1x1");

        GameObject goMap = GameObject.FindGameObjectWithTag("Map");
        tileManager = goMap.GetComponent<TileManager>();

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
        /* Code for resources gathering buildings:
        GameObject goResManager = GameObject.FindGameObjectWithTag("ResourceManager");
        ResourceManager resourceManager = goResManager.GetComponent<ResourceManager>();
        resourceManager.incRes1(1, Time.deltaTime);
        */
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

}