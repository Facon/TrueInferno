using UnityEngine;
using System.Collections;
using System.Reflection;

public class Version : MonoBehaviour {

	// Use this for initialization
	void Start () {
        Debug.Log(System.Environment.Version);

        System.Type type = System.Type.GetType("Mono.Runtime");
        if (type != null)
        {
            MethodInfo displayName = type.GetMethod("GetDisplayName", BindingFlags.NonPublic | BindingFlags.Static);
            if (displayName != null)
                Debug.Log(displayName.Invoke(null, null));
        }
    }
	
	// Update is called once per frame
	void Update () {
	
	}
}
