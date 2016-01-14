using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Assets.Scripts;

public class WorkerManager : MonoBehaviour {
    public GameObject prefabWorker;
    private TileManager tileManager;

	// Use this for initialization
	void Start () 
    {
        GameObject goMap = GameObject.FindGameObjectWithTag("Map");
        tileManager = goMap.GetComponent<TileManager>();
	}
	
	// Update is called once per frame
	void Update ()
    {
	}

    /// <summary>
    // Orders a soul from town hall to go to the furnace
    /// <summary>
    public void soul2Furnace()
    {
        TownHall townHall = tileManager.getTownHall().GetComponent<TownHall>();
        Furnace furnace = tileManager.getFurnace();

        if (furnace != null && townHall != null && townHall.getNumSouls()>0) 
        {
            if(sendSoulToBuilding(townHall.GetComponent<Building>(), furnace.GetComponent<Building>(), new BurnTask()))
                townHall.decreaseNumSouls();
        }
    }

    /// <summary>
    // Turns a soul into a free worker in the town hall
    /// <summary>
    public void soul2Worker() 
    {
        TownHall townHall = tileManager.getTownHall().GetComponent<TownHall>();

        if (townHall != null && townHall.getNumSouls() > 0)
        {
            townHall.decreaseNumSouls();
            townHall.increaseNumFreeWorkers();
        }
    }

    /// <summary>
    /// Sends a soul from town hall to the target building. Returns true if the soul was successfully set on route
    /// </summary>
    public bool sendSoulToBuilding(Building fromBuilding, Building targetBuilding, SoulTask task)
    {
        // Select random road tile of the townhall
        List<Tile> surrRoadTiles = fromBuilding.getSurrRoadTiles();

        if (surrRoadTiles.Count == 0)
            return false;

        Tile fromRoadTile = surrRoadTiles[UnityEngine.Random.Range(0, surrRoadTiles.Count)];

        // Get its surrounding road tiles
        surrRoadTiles = targetBuilding.getSurrRoadTiles();

        if (surrRoadTiles.Count == 0)
            return false;

        // Select random road tile of the target building
        Tile targetRoadTile = surrRoadTiles[UnityEngine.Random.Range(0, surrRoadTiles.Count)];

        // Try to find path
        List<Tile> path = GetComponent<PathFinder>().GetWorkerPath(fromRoadTile, targetRoadTile);

        if (path == null)
            return false;

        // Create soul graphics object
        GameObject soul = GameObject.Instantiate(prefabWorker, fromRoadTile.transform.position + Vector3.up, Quaternion.identity) as GameObject;
        soul.GetComponent<PathFollower>().AddToQueue(path);

        // Send him/her
        soul.GetComponent<PathFollower>().setBuilding(targetBuilding);

        // Change color of soul to reflect where is going
        soul.GetComponent<Renderer>().material.CopyPropertiesFromMaterial(targetBuilding.GetComponent<Renderer>().material);

        // Assign task
        soul.GetComponent<SoulTaskExecutor>().setTask(task);

        return true;
    }

    public void disableWorker(GameObject soul)
    {
        //soul.SetActive(false);
        GameObject.Destroy(soul);
    }

}
