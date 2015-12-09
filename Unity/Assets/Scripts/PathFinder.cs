using UnityEngine;
using System.Collections.Generic;
using System;
using Assets.Scripts;

public class PathFinder : MonoBehaviour {
    public TileManager tileManager;

    // Use this for initialization
    void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        // TEST
        int fromX = UnityEngine.Random.Range(0, (int)tileManager.getSizeX());
        int fromZ = UnityEngine.Random.Range(0, (int)tileManager.getSizeZ());
        int toX = UnityEngine.Random.Range(0, (int)tileManager.getSizeX());
        int toZ = UnityEngine.Random.Range(0, (int)tileManager.getSizeZ());

        GameObject from = tileManager.tiles[fromZ, fromX];
        GameObject to = tileManager.tiles[toZ, toX];

        Debug.Log("Finding path from " + from.name + " to " + to.name + "...\n");
        List<GameObject> path = GetRoadPath(from, to);

        if (path != null)
        {
            Debug.Log("Path found!\n");

            foreach (GameObject tile in path)
            {
                Debug.Log(tile.name + "\n");
            }
        }
        else
        {
            Debug.Log("Path not found!\n");
        }
	}

    private class Node<T>
    {
        private string id;
        private T data;
        private HashSet<Node<T>> neighbours;
        private Node<T> previous;

        public Node(string id, T data)
        {
            this.id = id;
            this.data = data;
            neighbours = new HashSet<Node<T>>();
            previous = null;
        }

        public void addNeighbour(Node<T> neighbour)
        {
            neighbours.Add(neighbour);
        }

        public HashSet<Node<T>> getNeighbours()
        {
            return neighbours;
        }

        public Node<T> getPrevious()
        {
            return previous;
        }

        public void setPrevious(Node<T> previous)
        {
            this.previous = previous;
        }

        public T getData()
        {
            return data;
        }

        public string getId()
        {
            return id;
        }
        
        override
        public int GetHashCode()
        {
            return id.GetHashCode();
        }

        override
        public string ToString()
        {
            return "id="+id;
        }
    }

    private class RoadPathProblem<T>
    {
        public Node<T> startNode = null;
        public Node<T> goalNode = null;
        public Dictionary<string,Node<T>> nodes = null;
    }

    RoadPathProblem<GameObject> getRoadPathProblem(GameObject startTile, GameObject goalTile)
    {
        RoadPathProblem<GameObject> problem = new RoadPathProblem<GameObject>();
        problem.nodes = new Dictionary<string,Node<GameObject>>();

        TileType[,] tileTypes = tileManager.tileTypes;
        GameObject[,] tiles = tileManager.tiles;
        
        // Loop over tile set: Create nodes and locate goal and start
        for (int z = 0; z < tileManager.getSizeZ(); ++z)
            for (int x = 0; x < tileManager.getSizeX(); ++x)
                // Only empty tiles and roads are considered for the graph
                if (tileTypes[z, x] == TileType.EMPTY || tileTypes[z, x] == TileType.ROAD)
                {
                    GameObject tile = tiles[z, x];

                    // Create node
                    Node<GameObject> newNode = new Node<GameObject>(tile.name, tile);

                    // Store it in node set
                    problem.nodes.Add(tile.name, newNode);

                    // Locate start and goal tiles to assign start and goal nodes
                    if (tile == startTile)
                        problem.startNode = newNode;
                    else if (tile == goalTile)
                        problem.goalNode = newNode;
                }

        // Check we've found start and goal
        if (problem.startNode == null || problem.goalNode == null)
        {
            Debug.Log("Can't find path: Start or goal tiles couldn't be found\n");
            return null;
        }

        // Loop over node set: Assign neighbours
        foreach (Node<GameObject> node in problem.nodes.Values)
        {
            TileManager.Vector2zx coords = tileManager.getTileCoords(node.getData());

            // Add adyacent tiles as neighbours if they are also EMPTY or ROAD
            foreach (TileManager.Vector2zx adyacent in tileManager.getCrossAdyacents(coords))
            {
                uint adyacentZ = (uint)adyacent.z;
                uint adyacentX = (uint)adyacent.x;

                if (tileTypes[adyacentZ, adyacentX] == TileType.EMPTY || tileTypes[adyacentZ, adyacentX] == TileType.ROAD)
                {
                    Node<GameObject> adyacentNode = problem.nodes[getNodeId(adyacentZ, adyacentX)];
                    node.addNeighbour(adyacentNode);
                }
            }
        }

        return problem;
    }

    private string getNodeId(uint z, uint x)
    {
        return tileManager.getTileNameFromCoords(z, x);
    }

    /// <summary>
    /// Gets path from a start tile to a goal
    /// </summary>
    /// <param name="start"></param>
    /// <param name="goal"></param>
    /// <returns></returns>
    public List<GameObject> GetRoadPath(GameObject startTile, GameObject goalTile)
    {
        List<GameObject> path = new List<GameObject>();

        // Transform our tile-based world into a node-based one
        RoadPathProblem<GameObject> roadPathProblem = getRoadPathProblem(startTile, goalTile);

        // Instantiate A* search problem
        AStar<GameObject> problem = new AStar<GameObject>(roadPathProblem.nodes, roadPathProblem.startNode, roadPathProblem.goalNode, manhattanDist);

		// And solve it
		List<Node<GameObject>> problemPath = problem.solve();

		// Transform node list into a tile list
		foreach(Node<GameObject> node in problemPath){
			path.Add(node.getData());
		}
				
        return path;
    }

    private delegate float HeuristicCost<T>(Node<T> from, Node<T> to);

    private float manhattanDist(Node<GameObject> from, Node<GameObject> to)
    {
        TileManager.Vector2zx fromCoords = tileManager.getTileCoords(from.getData());
        TileManager.Vector2zx toCoords = tileManager.getTileCoords(to.getData());

        return Math.Abs(fromCoords.x - toCoords.x) + Math.Abs(fromCoords.z - toCoords.z);
    }

    private class AStar<T>
    {
		private Dictionary<string,Node<T>> nodes;
        private Node<T> start, goal;
        private HeuristicCost<T> heuristicCost;

        public AStar(Dictionary<string, Node<T>> nodes, Node<T> start, Node<T> goal, HeuristicCost<T> heuristicCost)
        {
			this.nodes = nodes;
            this.start = start;
            this.goal = goal;
            this.heuristicCost = heuristicCost;
        }

        public List<Node<T>> solve()
        {
			// Init the closed set as empty
            HashSet<Node<T>> closed = new HashSet<Node<T>>();
            
			// Init the open set with the start node
			HashSet<Node<T>> opened = new HashSet<Node<T>>();
            opened.Add(start);
			
			// Init scores (all infinite)
            Dictionary<Node<T>, float> gScores = new Dictionary<Node<T>, float>(nodes.Count);
            PriorityQueue<Node<T>> fScores = new PriorityQueue<Node<T>>();
			foreach(Node<T> node in nodes.Values){
                gScores.Add(node, int.MaxValue);
                fScores.Add(node, int.MaxValue);
			}
			
			// But the origin
            gScores[start] = 0;
            fScores.Add(start, heuristicCost(start, goal));
			
			while(opened.Count>0){
				Node<T> current = fScores.popMin();
				
				// Target reached!
				if(current == goal)
					return getPath(goal);
				
				// Close current node
				opened.Remove(current);
				closed.Add(current);
				
				// For each neighbour
				foreach(Node<T> neighbour in current.getNeighbours()){
					// Ignore already closed nodes
					if(closed.Contains(neighbour))
						continue;
					
					// Check path through this neighbour
					float possibleGScore = 
						gScores[current] + distance(current, neighbour);
						
					// Open neighbour if it's not
					if(!opened.Contains(neighbour)){
						opened.Add(neighbour);
					}
					
					// If this path is not better than the best so far
					else if(possibleGScore >= gScores[neighbour])
						continue;
					
					// Best path found
					neighbour.setPrevious(current);
					gScores[neighbour] = possibleGScore;
                    fScores.Add(neighbour, possibleGScore + heuristicCost(neighbour, goal));
				}
			}

			// No path found
			return null;
        }
		
		/** Cost between 2 (adjacent) nodes (i.e. edge weight) */
		private float distance(Node<T> from, Node<T> to){
			// Inifite if they aren't neighbours
			if(!from.getNeighbours().Contains(to))
				return float.MaxValue;

            // Assume all edges have same cost = 1
			return 1; // TODO For generalizing include this in the node structure
		}

		private List<Node<T>> getPath(Node<T> node){
			List<Node<T>> path = new List<Node<T>>();
			path.Add (node);

			while(node.getPrevious()!=null){
				path.Add(node.getPrevious());
				node = node.getPrevious();
			}

			return path;
		}
		
    }
}
