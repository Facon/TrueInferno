using UnityEngine;
using System.Collections;

public class BuildingGenerator : MonoBehaviour {

    public void CreateBuilding(GameObject building)
    {
        GameObject edifGO = Instantiate(building, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        Building buildcomp = building.GetComponent<Building>();
        if (buildcomp)
            edifGO.transform.localScale = new Vector3(buildcomp.Length1, 0.25f, buildcomp.Length2);
    }

}
