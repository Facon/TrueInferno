using UnityEngine;
using System.Collections;

public class Controller : MonoBehaviour {
    public GameObject road;
    //public GameObject[] road;
    private BuildingGenerator bGenerator;
    private MouseMoveLock mMove;
    
	// Use this for initialization
	void Start () {
        mMove = GetComponent<MouseMoveLock>();
        bGenerator = GetComponent<BuildingGenerator>();
	}
	
	// Update is called once per frame
    void FixedUpdate() {

    }

	void Update () {

        if (bGenerator.newBuilding){
            if (mMove.BuildingMouseMove(bGenerator.newBuilding, Input.GetMouseButtonDown(0), bGenerator.bRoad, road)){
                bGenerator.newBuilding.SendMessage("buildingBuilt", SendMessageOptions.DontRequireReceiver);
                bGenerator.newBuilding = null;
                UnityEngine.Cursor.visible = true;
            }
        }


       

        /*
         * if (Input.GetMouseButtonDown(0) && (mMove.bValidPosition) && (bGenerator.newBuilding))
        {
            bGenerator.newBuilding = null;
            UnityEngine.Cursor.visible = true;
        }

        */


      /*  var vRay = Camera.main.ScreenPointToRay(Input.mousePosition);
        Vector3 positionClick;
        RaycastHit vHit;

        if (Physics.Raycast(vRay, out vHit))
        {
            Debug.DrawRay(vRay.origin, vRay.direction * 20, Color.red);
        }

        try
        {
            var obj = vHit.collider;

            if (Time.time % 100000.0f == 0.0f)
                Debug.Log("Tile actual seleccionado: " + obj.name);

            if (obj.gameObject.tag == "Building" || obj.transform.childCount > 0)
            {
                return;
            }


        catch (System.NullReferenceException)
        {
                        
        }*/
    }
}
