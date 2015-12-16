using UnityEngine;
using System.Collections.Generic;
using System;

public class TownHall : MonoBehaviour
{
    private int numFreeWorkers;

    // Use this for initialization
    void Start()
    {
        // TODO TEST
        numFreeWorkers = 2;
    }

    internal void init()
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
