using UnityEngine;
using System.Collections;

public class MouseMoveLock : MonoBehaviour {

    RaycastHit hit;
    private float raycastLength = 1000;

    // Use this for initialization
    void Start () {
	
	}

    // Update is called once per frame
    void Update() //Allows Object to Follow Mouse like in Original Script
    {
        

        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit[] hits = Physics.RaycastAll(ray, raycastLength);
        
        if (hits.Length > 0) {

            for (int i = 0; i < hits.Length; i++)
            {
                if (hits[i].collider.gameObject.tag == "Tile")
                {

                    var obj = hits[i].collider;
                    Building buildcomp = gameObject.GetComponent<Building>();

                    float posX = obj.gameObject.transform.position.x;
                    float posY = obj.gameObject.transform.position.y;
                    float posZ = obj.gameObject.transform.position.z;


                    if (buildcomp.Length1 > 1)
                        posX += 1;


                    if (buildcomp.Length2 > 1)
                        posZ += 1;


                    gameObject.transform.position = new Vector3(posX, posY, posZ);
                }
            }
        }    
    }

}
