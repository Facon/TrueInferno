using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Assets.Scripts;

public class ResourceTransformer : MonoBehaviour 
{
    public ResourceType inputResourceType;
    public ResourceType outputResourceType;
    public float conversionRate = 0.5f;
    public float transportSpeed = 0.8f;
    public float maxSqrDistance = 50;

    private TileManager tileManager;
    private ResourceManager resourceManager;
    private ScoreManager scoreManager;

    private float inputResources;
    private float outputResources;

	// Use this for initialization
	void Start () 
    {
        inputResources = 0;
        outputResources = 0;

        GameObject goMap = GameObject.FindGameObjectWithTag("Map");
        tileManager = goMap.GetComponent<TileManager>();

        resourceManager = GameObject.FindGameObjectWithTag("ResourceManager").GetComponent<ResourceManager>();

        scoreManager = GameObject.FindGameObjectWithTag("ScoreManager").GetComponent<ScoreManager>();
	}
	
	// Update is called once per frame
	void Update ()
    {
        // Don't do anything if it's not working
        if (!GetComponent<Building>().isWorking())
            return;

        // If there are output resources and the output type is different from none, try to send them
        if(outputResources>0 && outputResourceType!=ResourceType.None)
        {
            // Decide how much will be sent
            int batchSize = (int)outputResources;

            // If produces HadesFavor, notify directly to the score manager
            if (outputResourceType == ResourceType.HadesFavor)
            {
                scoreManager.increaseHadesFavor(batchSize);
                removeOutputResources(batchSize);
                return;
            }

            // Find buildings that accepts as input resource our output
            List<GameObject> initialBuildings = tileManager.findResourceBuildingByInput(outputResourceType);

            // Filter buildings
            List<GameObject> filteredBuildings = new List<GameObject>();
            foreach(GameObject building in initialBuildings)
            {
                // Skip non-working buildings
                if (!building.GetComponent<Building>().isWorking())
                    continue;

                // And retain the closest ones
                float sqrDist = (building.transform.position - this.gameObject.transform.position).sqrMagnitude;
                if(sqrDist <= maxSqrDistance)
                    filteredBuildings.Add(building); // Add to filtered list
            }

            // If there's still at least one target candidate
            if (filteredBuildings.Count > 0)
            {
                // Get a random one
                int selected = Random.Range(0, filteredBuildings.Count);
                Building targetBuilding = filteredBuildings[selected].GetComponent<Building>();

                // Ask our building component to send a soul with the task of transporting ouput resources
                bool transportSent = GetComponent<Building>().transportResources(new TransportTask(batchSize, outputResourceType, targetBuilding));

                // If it was possible, update our output resources
                if (transportSent)
                    removeOutputResources(batchSize);
            }
        }
	}

    // Transform input resources into output with a conversion rate. Input is added to our resources
    private void transformResources(float numResources)
    {
        // Calculate the fraction of output resources obtained
        float newOutputResources = numResources * conversionRate;

        // Register them in the manager
        resourceManager.increaseResources(outputResourceType, newOutputResources);
        
        // Increase the local storage
        outputResources += newOutputResources;

        // Consume the appropiate local input resources
        inputResources -= numResources;
    }

    /// <summary>
    /// Internally store the given resources
    /// </summary>
    /// <param name="numResources"></param>
    public void addInputResources(float numResources)
    {
        inputResources += numResources;
        Debug.Log("input(" + inputResourceType + ")=" + inputResources + ", output(" + outputResourceType + ")=" + outputResources);

        // Transform the new input resources into output
        if(numResources > 0)
        {
            transformResources(numResources);
        }
    }

    /// <summary>
    /// Internally remove the given resources
    /// </summary>
    /// <param name="numResources"></param>
    public void removeOutputResources(float numResources)
    {
        outputResources -= numResources;
        Debug.Log("input(" + inputResourceType + ")=" + inputResources + ", output(" + outputResourceType + ")=" + outputResources);
    }
}
