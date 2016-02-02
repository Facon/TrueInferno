using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class ScoreManager : MonoBehaviour {

    public Text hadesFavorDisplay;

    private int hadesFavor;

	// Use this for initialization
	void Start () {
        hadesFavor = 0;
	}

    public int GetHadesFavor()
    {
        return hadesFavor;
    }

	// Update is called once per frame
	void Update () {
        hadesFavorDisplay.text = hadesFavor.ToString();
	}

    public void increaseHadesFavor(int hadesFavor) {
        this.hadesFavor += hadesFavor;
    }
}
