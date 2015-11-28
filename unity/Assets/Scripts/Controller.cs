using UnityEngine;
using System.Collections;

public class Controller : MonoBehaviour {
    public GameObject building;
    public GameObject road;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        var vRay = Camera.main.ScreenPointToRay(Input.mousePosition);
        Vector3 positionClick;
        RaycastHit vHit;

        if (Physics.Raycast(vRay, out vHit))
        {
            Debug.DrawRay(vRay.origin, vRay.direction * 20, Color.red);
        }

        try
        {
            var obj = vHit.collider;

            if (obj.gameObject.tag == "Building" || obj.transform.childCount > 0)
            {
                return;
            }

            positionClick.x = obj.gameObject.transform.position.x;
            positionClick.z = obj.gameObject.transform.position.z;

            if (Input.GetMouseButtonDown(0))
            {
                GameObject edifGO = Instantiate(building, new Vector3(positionClick.x, 0.5f, positionClick.z), Quaternion.identity) as GameObject;
                edifGO.transform.parent = obj.transform;
            }
            else if (Input.GetMouseButtonDown(1))
            {
                GameObject roadGO = Instantiate(road, new Vector3(positionClick.x, 0.15f, positionClick.z), Quaternion.identity) as GameObject;
                roadGO.transform.parent = obj.transform;
            }
        }
        catch (System.NullReferenceException)
        {
                        
        }
    }
}
