using UnityEngine;
using System.Collections.Generic;
using System;
using Assets.Scripts;

public class PathFinder : MonoBehaviour {
    /// <summary>
    /// GameObject with TileManager component attached
    /// </summary>
    public TileManager tileManager;

    /// <summary>
    /// Flag to activate test finding a random path on each update
    /// </summary>
    public Boolean test = false;

    // Use this for initialization
    void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        if (test) 
        {
            // TEST
            int fromX = UnityEngine.Random.Range(0, (int)tileManager.getSizeX());
            int fromZ = UnityEngine.Random.Range(0, (int)tileManager.getSizeZ());
            int toX = UnityEngine.Random.Range(0, (int)tileManager.getSizeX());
            int toZ = UnityEngine.Random.Range(0, (int)tileManager.getSizeZ());

            Tile from = tileManager.tiles[fromZ, fromX];
            Tile to = tileManager.tiles[toZ, toX];

            Debug.Log("Finding path from " + from.name + " to " + to.name + "...\n");
            List<Tile> path = GetRoadPath(from, to);

            if (path != null)
            {
                Debug.Log("Path found!\n");

                foreach (Tile tile in path)
                {
                    Debug.Log(tile.name + "\n");
                }
            }
            else
            {
                Debug.Log("Path not found!\n");
            }
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

    /// <summary>
    ///  Transform a tile-based problem into an abstract node-based. It only considers empty and road tiles
    /// </summary>
    /// <param name="startTile"></param>
    /// <param name="goalTile"></param>
    /// <returns></returns>
    private RoadPathProblem<Tile> getRoadPathProblem(Tile startTile, Tile goalTile)
    {
        if(startTile==null)
        {
            Debug.Log("startTile must be not-null");
            return null;
        }
        if(goalTile==null)
        {
            Debug.Log("goalTile must be not-null");
            return null;
        }

        RoadPathProblem<Tile> problem = new RoadPathProblem<Tile>();
        problem.nodes = new Dictionary<string,Node<Tile>>();

        //TileType[,] tileTypes = tileManager.tileTypes;
        Tile[,] tiles = tileManager.tiles;
        
        // Loop over tile set: Create nodes and locate goal and start
        for (int z = 0; z < tileManager.getSizeZ(); ++z)
            for (int x = 0; x < tileManager.getSizeX(); ++x)
                // Only empty tiles and roads are considered for the graph
                //if (tileTypes[z, x] == TileType.EMPTY || tileTypes[z, x] == TileType.ROAD)

                if (tiles[z, x].buildingType == TileType.EMPTY || tiles[z, x].buildingType == TileType.ROAD)
                {
                    Tile tile = tiles[z, x];

                    // Create node
                    Node<Tile> newNode = new Node<Tile>(tile.name, tile);

                    // Store it in node set
                    problem.nodes.Add(tile.name, newNode);

                    // Locate start and goal tiles to assign start and goal nodes
                    if (tile == startTile)
                        problem.startNode = newNode;
                    if (tile == goalTile)
                        problem.goalNode = newNode;
                }

        // Check we've found start and goal
        string error = "";
        if (problem.startNode == null)
        {
            error += "Start tile can't be mapped into a valid node. ";
        }
        if (problem.goalNode == null)
        {
            error += "Goal tile can't be mapped into a valid node. ";
        }
        if (error != "")
        {
            Debug.Log("Can't find path: " + error + "\n");
            return null;
        }

        // Loop over node set: Assign neighbours
        foreach (Node<Tile> node in problem.nodes.Values)
        {
            TileManager.Vector2zx coords = tileManager.getTileCoords(node.getData());

            // Add adyacent tiles as neighbours if they are also EMPTY or ROAD
            foreach (TileManager.Vector2zx adyacent in tileManager.getCrossAdyacents(coords))
            {
                uint adyacentZ = (uint)adyacent.z;
                uint adyacentX = (uint)adyacent.x;
                

                if (tiles[adyacentZ, adyacentX].buildingType == TileType.EMPTY || tiles[adyacentZ, adyacentX].buildingType == TileType.ROAD)
                {
                    Node<Tile> adyacentNode = problem.nodes[getNodeId(adyacentZ, adyacentX)];
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
    /// Gets road path from a start tile to a goal passing through empty or road tile types
    /// </summary>
    /// <param name="start"></param>
    /// <param name="goal"></param>
    /// <returns></returns>
    public List<Tile> GetRoadPath(Tile startTile, Tile goalTile)
    {
        float start = Time.realtimeSinceStartup;

        List<Tile> path = new List<Tile>();

        // Transform our tile-based world into a node-based one
        RoadPathProblem<Tile> roadPathProblem = getRoadPathProblem(startTile, goalTile);

        if (roadPathProblem == null)
        {
            Debug.Log("Can't instantiate road path problem\n");
            return null;
        }

        // Instantiate A* search problem
        AStar<Tile> problem = new AStar<Tile>(roadPathProblem.nodes, roadPathProblem.startNode, roadPathProblem.goalNode, manhattanDist);

		// And solve it
		List<Node<Tile>> problemPath = problem.solve();
        if (problemPath == null)
        {
            Debug.Log("Can't solve A* search problem\n");
            return null;
        }

		// Transform node list into a tile list
		foreach(Node<Tile> node in problemPath){
			path.Add(node.getData());
		}

        float timeElapsed = Time.realtimeSinceStartup - start;
        Debug.Log("(GetRoadPath) Time elapsed = "+timeElapsed+" seconds");

        return path;
    }

    private delegate float HeuristicCost<T>(Node<T> from, Node<T> to);

    private float manhattanDist(Node<Tile> from, Node<Tile> to)
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
            float startTime = Time.realtimeSinceStartup;

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
                if (current == goal)
                {
                    float timeElapsed = Time.realtimeSinceStartup - startTime;
                    Debug.Log("(AStar.solve) Time elapsed = " + (Time.realtimeSinceStartup - startTime) + " seconds");

                    return getPath(goal);
                }
				
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

			// No path possible
            Debug.Log("There is no possible path\n");
            Debug.Log("(AStar.solve) Time elapsed = " + (Time.realtimeSinceStartup - startTime) + " seconds");

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

        /// <summary>
        /// Reconstruct a path where the last node is the given
        /// </summary>
        /// <param name="last"></param>
        /// <returns></returns>
		private List<Node<T>> getPath(Node<T> last){
			List<Node<T>> path = new List<Node<T>>();
			
            // Start from the last node
            path.Add (last);

            // Go backwards
			while(last.getPrevious()!=null){
				path.Add(last.getPrevious());
				last = last.getPrevious();
			}

            // Fix order
            path.Reverse();

			return path;
		}
		
    }
}
