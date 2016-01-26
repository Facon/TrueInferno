using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class PopUpManager : MonoBehaviour {

    public GameObject eventPanel;
    public GameObject tutorialPanel;
    public GameObject endgamePanel;
    public Sprite[] eventSprites;
    public Sprite[] tutorialSprites;
    public string[] tutorialTitles;
    public string[] tutorialDesc;
    public Sprite endgameSprite;
    private int tutorialIndex;

    public void GenerateEventPopUp (float tamX, float tamY, string title, string desc, int sprite, float duration) {
        Transform canvastrans = GameObject.Find("Canvas").gameObject.transform;
        GameObject popup = Instantiate(eventPanel, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        popup.transform.SetParent(canvastrans, false);
        popup.transform.localScale = new Vector3(tamX, tamY, 0);
        popup.GetComponentsInChildren<Text>()[0].text = title;
        popup.GetComponentsInChildren<Text>()[1].text = desc;
        popup.GetComponentsInChildren<Image>()[1].sprite = eventSprites[sprite];
        if (duration > 0)
            Destroy(popup.gameObject, duration);
    }

    public void GenerateEndGamePopUp(float tamX, float tamY, string title, string desc)
    {
        Transform canvastrans = GameObject.Find("Canvas").gameObject.transform;
        GameObject popup = Instantiate(endgamePanel, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        popup.transform.SetParent(canvastrans, false);
        popup.transform.localScale = new Vector3(tamX, tamY, 0);
        popup.GetComponentsInChildren<Text>()[0].text = title;
        popup.GetComponentsInChildren<Text>()[1].text = desc;
        popup.GetComponentsInChildren<Image>()[1].sprite = endgameSprite;
    }

    public void GenerateTutorialPopUp(float tamX, float tamY)
    {
        Transform canvastrans = GameObject.Find("Canvas").gameObject.transform;
        GameObject popup = Instantiate(tutorialPanel, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        popup.transform.SetParent(canvastrans, false);
        popup.transform.localScale = new Vector3(tamX, tamY, 0);
        popup.GetComponentsInChildren<Text>()[0].text = tutorialTitles[0];
        popup.GetComponentsInChildren<Text>()[1].text = tutorialDesc[0];
        popup.GetComponentsInChildren<Image>()[1].sprite = tutorialSprites[0];
        tutorialIndex = 0;
    }

    public void SwitchTutorialPopUp(GameObject panel){

        ++tutorialIndex;

        if (tutorialIndex >= tutorialTitles.Length)
        {
            Destroy(panel);
            return;
        }

        panel.GetComponentsInChildren<Text>()[0].text = tutorialTitles[tutorialIndex];
        panel.GetComponentsInChildren<Text>()[1].text = tutorialDesc[tutorialIndex];
        panel.GetComponentsInChildren<Image>()[1].sprite = tutorialSprites[tutorialIndex];
    }


}
