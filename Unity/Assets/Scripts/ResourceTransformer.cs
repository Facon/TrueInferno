using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Assets.Scripts;

public class ResourceTransformer : MonoBehaviour 
{
    public ResourceType inputResource;
    public ResourceType outputResource;
    public float conversionRate = 0.5f;
    public float transportSpeed = 0.8f;

    private TileManager tileManager;

    private float inputResources;
    private float outputResources;

	// Use this for initialization
	void Start () 
    {
        inputResources = 0;
        outputResources = 0;

        GameObject goMap = GameObject.FindGameObjectWithTag("Map");
        tileManager = goMap.GetComponent<TileManager>();
	}
	
	// Update is called once per frame
	void Update () 
    {
        // If there are input resources they are converted to output
        if(inputResources>0)
        {
            outputResources += inputResources * conversionRate;
            inputResources = 0;
        }
        
        // If there are output resources we try to send them
        if(outputResources>0)
        {
            // Decide how much we will send
            float batchSize = outputResources;

            // Find buildings that accepts as input resource our output
            List<GameObject> builidngs = tileManager.findResourceBuildingByInput(outputResource);

            // If exists at least one
            if(builidngs.Count>0)
            {
                // TODO Get nearest one
                // TODO Check if it's in range
                Building targetBuilding = builidngs[0].GetComponent<Building>();

                // Ask other component to send a soul with the task of transporting resources
                SendMessage("transportResources", new TransportTask(batchSize, outputResource, targetBuilding));
            }
        }
	}

    /// <summary>
    /// Internally store the given resources
    /// </summary>
    /// <param name="numResources"></param>
    public void addInputResources(float numResources)
    {
        inputResources += numResources;
        Debug.Log("inputResources=" + inputResources + ", outputResources=" + outputResources);
    }

    /// <summary>
    /// Internally remove the given resources
    /// </summary>
    /// <param name="numResources"></param>
    public void removeOutputResources(float numResources)
    {
        outputResources -= numResources;
        Debug.Log("inputResources=" + inputResources + ", outputResources=" + outputResources);
    }
}
