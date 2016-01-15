using UnityEngine;
using System.Collections;
using Assets.Scripts;

public class SoulTaskExecutor : MonoBehaviour {

    private SoulTask task;

    public void setTask(SoulTask task)
    {
        this.task = task;
    }

    public void executeTask(Building targetBuilding)
    {
        task.executeTask(targetBuilding);
    }

}
