using UnityEngine;
using System.Collections.Generic;
using System;

public class TownHall : MonoBehaviour
{
    private static int numFreeWorkers = 0;

    // Use this for initialization
    void Start()
    {
    }

    public int getNumFreeWorkers()
    {
        return numFreeWorkers;
    }

    public void increaseNumFreeWorkers()
    {
        ++numFreeWorkers;
    }

    public void decreaseNumFreeWorkers()
    {
        --numFreeWorkers;
    }
}
