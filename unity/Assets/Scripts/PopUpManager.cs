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
    public string[] tutorialDialogs;
    public Sprite[] endgameSprites;
    public string[] endgameDialogs;
    public string[] endgameTitles;
    private int tutorialIndex;
    private int tutorialIndexEnd;

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

    public void GenerateEndGamePopUp(float tamX, float tamY, int score, int goalScore)
    {
        Transform canvastrans = GameObject.Find("Canvas").gameObject.transform;
        GameObject popup = Instantiate(endgamePanel, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        popup.transform.SetParent(canvastrans, false);
        popup.transform.localScale = new Vector3(tamX, tamY, 0);
        if (score >= goalScore)
        {
            popup.GetComponentsInChildren<Text>()[0].text = endgameTitles[0];
            popup.GetComponentsInChildren<Text>()[1].text = endgameDialogs[0];
            popup.GetComponentsInChildren<Image>()[1].sprite = endgameSprites[0];
        }
        else
        {
            popup.GetComponentsInChildren<Text>()[0].text = endgameTitles[1];
            popup.GetComponentsInChildren<Text>()[1].text = endgameDialogs[1];
            popup.GetComponentsInChildren<Image>()[1].sprite = endgameSprites[1];
        }
        popup.GetComponentsInChildren<Text>()[2].text = "Score: " + score;
    }

    public void GenerateTutorialPopUp(float tamX, float tamY, int tutorialState)
    {
        Transform canvastrans = GameObject.Find("Canvas").gameObject.transform;
        GameObject popup = Instantiate(tutorialPanel, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        popup.transform.SetParent(canvastrans, false);
        popup.transform.localScale = new Vector3(tamX, tamY, 0);

        int tutorialStartPoint = 0;
        switch (tutorialState) 
        {
            case 1:
                tutorialStartPoint = 2;
                tutorialIndexEnd = 3;
                tutorialIndex = 2;
                break;
            case 2:
                tutorialStartPoint = 3;
                tutorialIndexEnd = 5;
                tutorialIndex = 3;
                break;
            default:
                tutorialStartPoint = 0;
                tutorialIndexEnd = 2;
                tutorialIndex = 0;
                break;
        }

        popup.GetComponentsInChildren<Text>()[0].text = tutorialTitles[tutorialStartPoint];
        popup.GetComponentsInChildren<Text>()[1].text = tutorialDialogs[tutorialStartPoint];
        popup.GetComponentsInChildren<Text>()[2].text = tutorialDesc[tutorialStartPoint];
        popup.GetComponentsInChildren<Image>()[1].sprite = tutorialSprites[tutorialStartPoint];
       
    }

    public void SwitchTutorialPopUp(GameObject panel){

        ++tutorialIndex;

        if (tutorialIndex >= tutorialIndexEnd)
        {
            Destroy(panel);
            return;
        }

        panel.GetComponentsInChildren<Text>()[0].text = tutorialTitles[tutorialIndex];
        panel.GetComponentsInChildren<Text>()[1].text = tutorialDialogs[tutorialIndex];
        panel.GetComponentsInChildren<Text>()[2].text = tutorialDesc[tutorialIndex];
        panel.GetComponentsInChildren<Image>()[1].sprite = tutorialSprites[tutorialIndex];
    }


}
