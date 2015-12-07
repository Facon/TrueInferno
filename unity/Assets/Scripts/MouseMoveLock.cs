using UnityEngine;
using System.Collections;

public class MouseMoveLock : MonoBehaviour {


    public bool bValidPosition; 

    // Use this for initialization
    void Start () {
	
	}

    // Update is called once per frame
    public void BuildingMouseMove(GameObject building) 
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


                    if (((float)buildcomp.sizeX % 2) == 0)
						posX += 0.5f;

                    if (((float)buildcomp.sizeZ % 2) == 0)
						posZ += 0.5f;

                    building.transform.position = new Vector3(posX, posY, posZ);

                }
            }
        }
        bValidPosition = true;//aprobar por rayos de asier
    }    
}
