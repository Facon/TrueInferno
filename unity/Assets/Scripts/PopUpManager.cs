using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class PopUpManager : MonoBehaviour {

    public GameObject panel;

    public void GeneratePopUp (float tamX, float tamY, string text) {
        Transform canvastrans = GameObject.Find("Canvas").gameObject.transform;
        GameObject popup = Instantiate(panel, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        popup.transform.SetParent(canvastrans, false);
        popup.transform.localScale = new Vector3(tamX, tamY, 0);
        popup.GetComponentInChildren<Text>().text=text;
    }

}
