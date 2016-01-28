using UnityEngine;
using System.Collections;

public class EventPanelDestructor : MonoBehaviour {

    public void DestroyPanel(float delay)
    {
        // Set Especial color for destruction
        Destroy(gameObject, delay);
    }
}
