using UnityEngine;
using System.Collections;

public class TutorialPanelSwitcher : MonoBehaviour {

    public void PanelSwitch() {
        PopUpManager popManager = GameObject.Find("PopUpManager").GetComponent<PopUpManager>();
        popManager.SwitchTutorialPopUp(gameObject);

    }
}
