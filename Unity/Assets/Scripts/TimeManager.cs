using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class TimeManager : MonoBehaviour 
{
    /// <summary>
    /// Time counter limit in minutes
    /// </summary>
    public float minutesTimeLimit = 2;

    /// <summary>
    /// Remaining time
    /// </summary>
    private float remainingTime;

    public Text timeDisplay;

	// Use this for initialization
	void Start () 
    {
        remainingTime = 60 * minutesTimeLimit;
	}
	
	// Update is called once per frame
	void Update () 
    {
        remainingTime -= Time.deltaTime;
        if (remainingTime <= 0)
            remainingTime = 0;

        timeDisplay.text = formatTime();
	}

    private string formatTime() 
    {
        int minutes = (int)remainingTime / 60;
        int seconds = (int)remainingTime % 60;

        return string.Format("{0}:{1:00}", minutes , seconds);
    }
}
