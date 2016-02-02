using UnityEngine;
using System.Collections;

public class EventManager : MonoBehaviour {

    private TimeManager timeManager;
    private TileManager tileManager;
    private PopUpManager popManager;
    private ScoreManager scoreManager;
    private WorkerManager workerManager;

    //Event parameters
    public float destroyInterval;
    public float destroyDelay;
    public int destroyChance;

    // Event time controller
    private float remainingDestroyEventTime;
    private float previousTime;
    private bool endGame;
    private bool gameStart;

    //EventVictory parameters
    public int goalScore;

    private bool tutorial;

    private int buildingTutorialState;
    private int buildingRoadState;
    private TownHall townHall;

	// Use this for initialization
	void Start () 
    {
        
        popManager = GameObject.Find("PopUpManager").GetComponent<PopUpManager>();
        scoreManager = GameObject.Find("ScoreManager").GetComponent<ScoreManager>();
        timeManager = GameObject.Find("TimeManager").GetComponent<TimeManager>();
        tileManager = GameObject.Find("Map").GetComponent<TileManager>();
        workerManager = GameObject.Find("WorkerManager").GetComponent<WorkerManager>();
        previousTime = 60 * timeManager.minutesTimeLimit;
        remainingDestroyEventTime = destroyInterval;
        endGame = false;
        gameStart = true;
        tutorial = true;
        buildingTutorialState = 0;
        buildingRoadState = 0;
        townHall = tileManager.getTownHall().GetComponent<TownHall>();
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

        if (buildingTutorialState == 0)
            buildingTutorialState = tileManager.buildingList.Count - 1;

        if (buildingRoadState == 0){
            if(workerManager.checkSoulPathAvaible(townHall.GetComponent<Building>(), tileManager.buildingList[UnityEngine.Random.Range(0, tileManager.buildingList.Count)].GetComponent<Building>()))
                buildingRoadState = 1;
        }

	}


    // Method for event controlling
    void EventChecker() 
    {
        if (gameStart) 
        {
            TutorialPopUpEvent(0);
        }

        if (buildingTutorialState > 0) 
        {
            TutorialPopUpEvent(1);
            buildingTutorialState = -1;
        }

        if (buildingRoadState == 1)
        {
            TutorialPopUpEvent(2);
            buildingRoadState = -1;
        }

        if (remainingDestroyEventTime <= 0)
            BuildingDestructionEvent();

        if (timeManager.remainingTime <= 0 && !endGame)
            EndGamePopUpEvent();
    }

    //Event Methods
    void TutorialPopUpEvent(int tutorialState)
    {
        gameStart = false;
        popManager.GenerateTutorialPopUp(0.25f, 0.75f, tutorialState);
    }

    void EndGamePopUpEvent() {
        endGame = true;
        popManager.GenerateEndGamePopUp(0.25f, 0.75f, scoreManager.GetHadesFavor(), goalScore);
    }

    void BuildingDestructionEvent()
    {
        remainingDestroyEventTime = destroyInterval;
        
        bool eventSuccess = UnityEngine.Random.Range(1,100) <= destroyChance;
        if (eventSuccess)
        {
            eventSuccess = false;
            foreach (GameObject building in tileManager.buildingList)
            { 
                if (building.GetComponent<BuildingDestructor>()!=null)
                {
                    eventSuccess = true;
                    break;
                }
            }
        }
        
        if ( !tutorial && eventSuccess && tileManager.buildingList.Count > 0)
        {
            eventSuccess = false;
            BuildingDestructor targetBuildingDestructor;

            while (!eventSuccess)
            {
                targetBuildingDestructor = tileManager.buildingList[UnityEngine.Random.Range(0, tileManager.buildingList.Count)].GetComponent<BuildingDestructor>();
                if (targetBuildingDestructor != null)
                {
                    popManager.GenerateEventPopUp(0.25f, 0.75f, "Building Destruction", "Incoming building destruction", 0, 5f);
                    targetBuildingDestructor.DestroyBuilding(destroyDelay);
                    eventSuccess = true;
                }
            }
        }

        if (buildingRoadState == -1 && tutorial)
            tutorial = false;

    }
}
