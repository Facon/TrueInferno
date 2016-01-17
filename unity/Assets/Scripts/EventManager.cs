using UnityEngine;
using System.Collections;

public class EventManager : MonoBehaviour {

    private TimeManager timeManager;
    private TileManager tileManager;

    //Event parameters
    public float destroyInterval;
    public float destroyDelay;
    public int destroyChance;

    // Event time controller
    private float remainingDestroyEventTime;

    private float previousTime;

	// Use this for initialization
	void Start () 
    {
        timeManager = GameObject.Find("TimeManager").GetComponent<TimeManager>();
        tileManager = GameObject.Find("Map").GetComponent<TileManager>();
        previousTime = 60 * timeManager.minutesTimeLimit;
        remainingDestroyEventTime = destroyInterval;
	}
	
	// Update is called once per frame
	void Update ()
    {
        // Update event timers
        remainingDestroyEventTime -= (previousTime - timeManager.remainingTime);

        // Check for event timers
        EventChecker();

        // Update timer
        previousTime = timeManager.remainingTime;
	}


    // Method for event controlling
    void EventChecker() 
    {
        if (remainingDestroyEventTime <= 0)
            BuildingDestructionEvent();
    }

    //Event Methods
    void BuildingDestructionEvent()
    {
        remainingDestroyEventTime = destroyInterval;
        bool eventSuccess = UnityEngine.Random.Range(1,100) <= destroyChance;
        if ( eventSuccess && tileManager.buildingList.Count > 0)
        {
            GameObject targetBuilding = tileManager.buildingList[UnityEngine.Random.Range(0, tileManager.buildingList.Count)];
            targetBuilding.GetComponent<BuildingDestructor>().DestroyBuilding(destroyDelay);
        }
    }
}
