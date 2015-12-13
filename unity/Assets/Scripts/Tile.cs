using UnityEngine;
using System.Collections.Generic;
using System;

public class Tile : MonoBehaviour {
    public uint posX;
    public uint posZ;

    public int type;
    public List<Material> materials;

	// Use this for initialization
	void Start () {
    }

    internal void init()
    {
        this.GetComponent<MeshRenderer>().material = materials[type];
    }
}
