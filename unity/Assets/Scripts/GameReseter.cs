using UnityEngine;
using System.Collections;

public class GameReseter : MonoBehaviour {

	// Use this for initialization
    public void Reset()
    {
        Application.LoadLevel(Application.loadedLevelName);
    }
}
