using UnityEngine;
using System.Collections.Generic;
using System;

public class TownHall : MonoBehaviour
{
    // Time (s) beteween soul spawns
    public int soulSpawnTime = 3;

    // Time (s) elapsed between last soul spawn
    private float timeSinceLastSpawn;

    private int numSouls;
    private int numFreeWorkers;

    // Use this for initialization
    void Start()
    {
        numSouls = 0;
        numFreeWorkers = 0;
        timeSinceLastSpawn = 0;
    }

    public void FixedUpdate()
    {
        timeSinceLastSpawn += Time.deltaTime;
        if (timeSinceLastSpawn >= soulSpawnTime)
        {
            increaseNumSouls();
            timeSinceLastSpawn -= soulSpawnTime;
        }
    }

    public void Update()
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

    public int getNumSouls()
    {
        return numSouls;
    }

    public void increaseNumSouls()
    {
        ++numSouls;
    }

    public void decreaseNumSouls()
    {
        --numSouls;
    }

}
