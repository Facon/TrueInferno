﻿using UnityEngine;
using System.Collections;

public class MouseMoveLock : MonoBehaviour {


    public bool bValidPosition; 

    // Use this for initialization
    void Start () {
        bValidPosition = false;
	}

    // Update is called once per frame
    public bool BuildingMouseMove(GameObject building, bool placeBuilding) 
    {
        bValidPosition = false;
        float raycastLength = 1000;
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit[] hits = Physics.RaycastAll(ray, raycastLength);
        
        if (hits.Length > 0) {

            for (int i = 0; i < hits.Length; i++)
            {
                if (hits[i].collider.gameObject.tag == "Tile" && hits[i].point.y>=0.12)
                {

                    var obj = hits[i].collider;
                    Building buildcomp = building.GetComponent<Building>();

                    float posX = obj.gameObject.transform.position.x;
                    float posY = building.transform.position.y;
                    float posZ = obj.gameObject.transform.position.z;


                    if (isEven(buildcomp.sizeX))
						posX += 0.5f;

                    if (isEven(buildcomp.sizeZ))
						posZ += 0.5f;

                    building.transform.position = new Vector3(posX, posY, posZ);
                    bValidPosition = checkValidPosition(buildcomp, hits[i].collider.gameObject.GetComponent<Tile>().posX, hits[i].collider.gameObject.GetComponent<Tile>().posZ);

                    Renderer rend = building.GetComponent<Renderer>();
                    if (placeBuilding && bValidPosition)
                    {
                        rend.material.SetColor("_SpecColor", Color.black);
                        setOcuppiedTile(buildcomp, hits[i].collider.gameObject.GetComponent<Tile>().posX, hits[i].collider.gameObject.GetComponent<Tile>().posZ);
                        building.transform.position = new Vector3(posX, posY-0.5f, posZ);
                        return true;
                    }
                    else { 
                        if (bValidPosition)
                            rend.material.SetColor("_SpecColor", Color.green);
                        else
                            rend.material.SetColor("_SpecColor", Color.red);
                    }

                }
            }
        }
        return false;
    }

    bool checkValidPosition(Building building, uint x, uint z)
    {
        Debug.Log(x + " " + z);

        TileManager tm = GameObject.Find("Map").GetComponent<TileManager>();
        Tile[,] tiles = tm.tiles;
        uint mapSizeX = tm.getSizeX();
        uint mapSizeZ = tm.getSizeZ();
        int liminfX = 0, liminfZ = 0, limsupX = 0, limsupZ = 0;

        liminfX = (int)x - (int)building.sizeX / 2;
        limsupX = (int)x + (int)building.sizeX / 2;
        liminfZ = (int)z - (int)building.sizeZ / 2;
        limsupZ = (int)z + (int)building.sizeZ / 2;

        if (isEven(building.sizeX)) // Even
            ++liminfX;
        if (isEven(building.sizeZ)) // Even
            ++liminfZ;

        if (liminfX < 0 || liminfZ < 0 || limsupX >= mapSizeX || limsupZ >= mapSizeZ)
            return false;

        for (int i = liminfX; i <= limsupX; ++i) {
            for (int j = liminfZ; j <= limsupZ; ++j)
            {
                //Debug.Log("Tile "+i+" "+j+" tipo "+tiles[i,j].type+" tipo buscado "+building.validTileType);
                if ((tiles[i,j].buildingType != TileType.EMPTY)||(tiles[i,j].type != building.validTileType))
                    return false;
            }
        }
        return true;
    }

    void setOcuppiedTile(Building building, uint x, uint z)
    {
        TileManager tm = GameObject.Find("Map").GetComponent<TileManager>();
        Tile[,] tiles = tm.tiles;
        int liminfX = 0, liminfZ = 0, limsupX = 0, limsupZ = 0;

        liminfX = (int)x - (int)building.sizeX / 2;
        limsupX = (int)x + (int)building.sizeX / 2;
        liminfZ = (int)z - (int)building.sizeZ / 2;
        limsupZ = (int)z + (int)building.sizeZ / 2;

        if (isEven(building.sizeX)) // Even
            ++liminfX;
        if (isEven(building.sizeZ)) // Even
            ++liminfZ;

        for (int i = liminfX; i <= limsupX; ++i)
        {
            for (int j = liminfZ; j <= limsupZ; ++j)
            {
                tiles[i,j].buildingType = building.BuildingType;
            }
        }
    }

    bool isEven(float num)
    {
        return (num % 2) == 0;
    }
}
