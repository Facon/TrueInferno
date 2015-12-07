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
                if (hits[i].collider.gameObject.tag == "Tile" && hits[i].point.y>=0.12)
                {

                    var obj = hits[i].collider;
                    Building buildcomp = gameObject.GetComponent<Building>();

                    float posX = obj.gameObject.transform.position.x;
                    float posY = obj.gameObject.transform.position.y;
                    float posZ = obj.gameObject.transform.position.z;


                    if ((buildcomp.Length1 % 2) == 0)
						posX += 0.5f;
                        //posX += 0.5+buildcomp.Length1;


                    if ((buildcomp.Length2 % 2) == 0)
                        //posZ += 0.5+buildcomp.Length2;
						posZ += 0.5f;

                    gameObject.transform.position = new Vector3(posX, posY, posZ);
                }
            }
        }    
    }

}
