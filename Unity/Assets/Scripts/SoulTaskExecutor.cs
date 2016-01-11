using UnityEngine;
using System.Collections;

public class SoulTaskExecutor : MonoBehaviour {

    private SoulTask task;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    public void setTask(SoulTask task)
    {
        this.task = task;
    }

    /// <summary>
    /// Execute task at the given building
    /// </summary>
    /// <param name="building"></param>
    public void executeTask(Building building)
    {
        switch (task)
        {
            case SoulTask.Work: 
                // TODO Notify building the soul has just reached and it's ready to work
                building.SendMessage("soulReadyToWork", this, SendMessageOptions.RequireReceiver);
                break;

            case SoulTask.Burn:
                building.SendMessage("soulReadyToBurn", this, SendMessageOptions.RequireReceiver);
                break;
        }
    }
}
