using UnityEngine;
using System.Collections;

public class BuildingGenerator : MonoBehaviour {
    public GameObject newBuilding;

    public void start() {
        newBuilding = null;
    }

    public void CreateBuilding(GameObject building)
    {
        newBuilding = Instantiate(building, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        Building buildcomp = building.GetComponent<Building>();
        if (buildcomp)
        {
            newBuilding.transform.position = new Vector3(0f,(float) buildcomp.sizeY / 2 + 0.1f,0f);
            newBuilding.transform.localScale = new Vector3(buildcomp.sizeX, buildcomp.sizeY, buildcomp.sizeY);
            
        }

		UnityEngine.Cursor.visible = true; // TODO Cambiar a false
    }

}
