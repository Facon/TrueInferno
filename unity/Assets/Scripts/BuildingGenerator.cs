using UnityEngine;
using System.Collections;

public class BuildingGenerator : MonoBehaviour {
    public GameObject newBuilding;

    public void start() {
        newBuilding = null;
    }

    public void CreateBuilding(GameObject building)
    {
        if (newBuilding)
            Destroy(newBuilding);
        newBuilding = Instantiate(building, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        Building buildcomp = building.GetComponent<Building>();
        if (buildcomp)
        {
            newBuilding.transform.position = new Vector3(0f,(float) buildcomp.sizeY / 2 + 0.6f,0f);
            newBuilding.transform.localScale = new Vector3(buildcomp.sizeX, buildcomp.sizeY, buildcomp.sizeZ);
            Renderer rend = newBuilding.GetComponent<Renderer>();
            rend.material.SetColor("_SpecColor", Color.red);
        }

		UnityEngine.Cursor.visible = true; // TODO Cambiar a false
    }

}
