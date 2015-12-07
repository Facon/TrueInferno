using UnityEngine;
using System.Collections.Generic;

public class Building : MonoBehaviour {
    public uint sizeX;
    public uint sizeY;
    public uint sizeZ;
    private bool visible = true;
    private List<GameObject> list = new List<GameObject>();

    // Use this for initialization
    void Start() {
        if (sizeX < 1 || sizeY < 1 || sizeZ < 1)
            throw new System.Exception("El tamaño de edificio tiene que ser mayor que 1x1x1");

        Vector3 center = gameObject.transform.position;

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

        }
    }

    // Update is called once per frame
    void FixedUpdate () {
        RaycastHit hitInfo;

        foreach (GameObject go in list)
        {
            Vector3 point = go.transform.position;
            if (Physics.Raycast(point, Vector3.down, out hitInfo))
            {
                if (hitInfo.collider.gameObject.tag == "Tile")
                {
                    Debug.Log("Tile HIT:" + hitInfo.collider.gameObject.name);
                }
            }
        }
	}
}