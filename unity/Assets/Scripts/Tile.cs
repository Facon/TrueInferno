using UnityEngine;
using System.Collections.Generic;
using System;

public class Tile : MonoBehaviour {
    public uint posX;
    public uint posZ;

    public int type;
    public TileType buildingType;
    public List<Material> materials = new List<Material>(5);

	// Use this for initialization
	void Start () {
        //MeshRenderer m = GetComponent<MeshRenderer>();
        //m.material = new Material(Material.)
	}

    internal void init()
    {
        this.GetComponent<MeshRenderer>().material = materials[type];
    }
}
