using UnityEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Assets.Scripts
{
    public class PriorityQueue<T>
    {
        private SortedList<float,List<T>> data;

        public PriorityQueue()
        {
            data = new SortedList<float,List<T>>();
        }

        public void Add(T e, float score)
        {
            //Debug.Log("Before adding " + e + " with score " + score + ": " + ToString());

            // Get the appropriate score list
            List<T> scoreList = null;
            if (data.ContainsKey(score))
            {
                scoreList = data[score];
            }
            
            // If it doesn't exist
            if (scoreList == null)
            {
                scoreList = new List<T>();
                data[score] = scoreList;
            }

            // Add element to the score list
            scoreList.Add(e);

            //Debug.Log("After add: " + ToString());
        }

        public Boolean isEmpty()
        {
            return data.Count == 0;
        }

        public T popMin()
        {
            //Debug.Log("Before pop: " + ToString());

            // Get the [score,list] pair with the minimum score
            float minScore = data.Keys.Min();

            // Get the first element of the list
            List<T> scoreList = data[minScore];
            T ret = scoreList.First();

            // Remove element from list
            scoreList.Remove(ret);

            // If it was the last one
            if (scoreList.Count() == 0)
                // Delete score list
                data.Remove(minScore);

            //Debug.Log("After pop: " + ToString());

            return ret;
        }

        override
        public string ToString()
        {
            string ret = "";

            int pos=0;
            foreach(KeyValuePair<float,List<T>> e in data)
            {
                ret += (pos++) + ": " + e.Key;

                foreach(T elem in e.Value)
                {
                    ret += (" " + elem + ";");
                }

                ret += "\n";
            }

            return ret;
        }
    }
}
